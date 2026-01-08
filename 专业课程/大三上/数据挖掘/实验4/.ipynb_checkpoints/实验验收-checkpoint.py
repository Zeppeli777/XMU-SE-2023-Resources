from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, ListFlowable, ListItem
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.cidfonts import UnicodeCIDFont

# 注册中文字体（解决乱码问题）
pdfmetrics.registerFont(UnicodeCIDFont('STSong-Light'))

# 创建 PDF 文件
pdf_path = "./DockerSwarm_MySQL_Experiment_Check_Guide_CN.pdf"
doc = SimpleDocTemplate(pdf_path, pagesize=A4, title="Docker Swarm 实验验收指南")

# 定义样式
styles = getSampleStyleSheet()
styles.add(ParagraphStyle(name="TitleCN", fontName="STSong-Light", fontSize=18, alignment=1, spaceAfter=20))
styles.add(ParagraphStyle(name="SubtitleCN", fontName="STSong-Light", fontSize=14, spaceAfter=10, textColor=colors.HexColor("#004080")))
styles.add(ParagraphStyle(name="BodyCN", fontName="STSong-Light", fontSize=11, leading=18))
styles.add(ParagraphStyle(name="CodeBlockCN", fontName="Courier", fontSize=10, textColor=colors.HexColor("#333333")))

# PDF 内容
content = []

content.append(Paragraph("实验二：熟悉服务器环境（验收现场操作指南）", styles["TitleCN"]))
content.append(Paragraph("一、现场验收可能要求", styles["SubtitleCN"]))
content.append(Paragraph("助教通常要求学生在华为云 Docker Swarm 环境中完成以下验证步骤：", styles["BodyCN"]))

content.append(ListFlowable(
    [
        ListItem(Paragraph("1. 修改 MySQL 数据库密码；", styles["BodyCN"])),
        ListItem(Paragraph("2. 重新构建或部署 productdemomybatis 服务；", styles["BodyCN"])),
        ListItem(Paragraph("3. 验证 MySQL 服务与 productdemomybatis 服务联通；", styles["BodyCN"])),
        ListItem(Paragraph("4. 展示 Swarm 服务状态、容器日志与数据库表内容。", styles["BodyCN"])),
    ],
    bulletType="1", leftIndent=20
))

content.append(Spacer(1, 10))
content.append(Paragraph("二、修改 MySQL 密码及重启服务步骤", styles["SubtitleCN"]))

content.append(Paragraph("（1）查看 mysql 服务所在节点：", styles["BodyCN"]))
content.append(Paragraph("docker service ps mysql", styles["CodeBlockCN"]))

content.append(Paragraph("（2）进入 mysql 所在节点并查看容器 ID：", styles["BodyCN"]))
content.append(Paragraph("docker ps | grep mysql", styles["CodeBlockCN"]))

content.append(Paragraph("（3）进入容器：", styles["BodyCN"]))
content.append(Paragraph("docker exec -it [CONTAINER_ID] sh", styles["CodeBlockCN"]))

content.append(Paragraph("（4）登录 mysql 并修改密码：", styles["BodyCN"]))
content.append(Paragraph("mysql -uroot -p", styles["CodeBlockCN"]))
content.append(Paragraph("ALTER USER 'demouser'@'%' IDENTIFIED BY '新密码';", styles["CodeBlockCN"]))
content.append(Paragraph("FLUSH PRIVILEGES;", styles["CodeBlockCN"]))

content.append(Paragraph("（5）退出容器后重新部署服务：", styles["BodyCN"]))
content.append(Paragraph("docker stack deploy -c productdemomybatis-docker.yml --with-registry-auth lab", styles["CodeBlockCN"]))

content.append(Spacer(1, 10))
content.append(Paragraph("三、检查服务与日志", styles["SubtitleCN"]))
content.append(Paragraph("1. 查看服务状态：", styles["BodyCN"]))
content.append(Paragraph("docker service ls", styles["CodeBlockCN"]))
content.append(Paragraph("2. 查看某个服务的详细运行情况：", styles["BodyCN"]))
content.append(Paragraph("docker service ps productmybatis", styles["CodeBlockCN"]))
content.append(Paragraph("3. 查看容器日志：", styles["BodyCN"]))
content.append(Paragraph("docker logs [CONTAINER_ID] | tail -n 50", styles["CodeBlockCN"]))

content.append(Spacer(1, 10))
content.append(Paragraph("四、常见问题速查", styles["SubtitleCN"]))
content.append(ListFlowable(
    [
        ListItem(Paragraph("服务部署失败 → 检查 yml 文件数据库密码是否与 MySQL 一致。", styles["BodyCN"])),
        ListItem(Paragraph("无法连接数据库 → 检查 overlay 网络或 label 是否配置正确。", styles["BodyCN"])),
        ListItem(Paragraph("镜像拉取失败 → 检查是否已登录华为 SWR。", styles["BodyCN"])),
        ListItem(Paragraph("日志乱码 → 使用 tail -f 命令查看实时输出。", styles["BodyCN"])),
    ],
    bulletType="bullet", leftIndent=20
))

content.append(Spacer(1, 10))
content.append(Paragraph("五、命令速查", styles["SubtitleCN"]))
content.append(Paragraph("""
docker swarm init
docker node ls
docker network create --driver overlay javaee-proxy
docker node update --label-add mysql=yes course-node1
docker stack deploy -c productdemomybatis-docker.yml --with-registry-auth lab
""", styles["CodeBlockCN"]))

content.append(Spacer(1, 20))
content.append(Paragraph("© 实验二 验收现场快速指南 — Docker Swarm + MySQL 环境", styles["BodyCN"]))

# 生成 PDF
doc.build(content)

pdf_path
