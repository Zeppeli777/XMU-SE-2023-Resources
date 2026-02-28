# Git 通关指南
> 不管是命令行还是GUI，其实只要明白git能**做成什**、使用时需要**避免什么**就行，很多都是现查现用。下面这个网站能带你快速过一遍git的能力，了解版本管理思维和git原理。
[学习git分支](https://learngitbranching.js.org/?locale=zh_CN)

# 实际开发中常用的命令
> 在实际开发中，常常 使用主分支-预上线分支-各自功能开发分支 的结构，每个常用指令都有其特定使用背景。稍微梳理了比较高频使用的场景和命令。

- 拉取
	- 首次拉取仓库
		```bash
		git clone <仓库地址>
		```
	- 日常同步远端主分支（保持本地更新）
		```bash
		git checkout main
		git pull origin main
		```
	- 查看远端分支但不合并
		```bash
		git fetch origin
		```

- 新建分支
	- 基于最新主分支创建功能分支
		```bash
		git checkout main
		git pull origin main
		git checkout -b feat/login
		```
	- 将本地分支推送到远端并建立追踪关系
		```bash
		git push -u origin feat/login
		```

- 暂存
	- 查看当前改动
		```bash
		git status
		```
	- 暂存全部改动
		```bash
		git add .
		```
	- 只暂存某个文件
		```bash
		git add path/to/file
		```
	- 交互式暂存（按代码块选择）
		```bash
		git add -p
		```

- 提交
	- 提交已暂存改动
		```bash
		git commit -m "feat: 添加登录接口"
		```
	- 修改最近一次提交信息（未推送时使用）
		```bash
		git commit --amend -m "feat: 调整登录接口参数校验"
		```
	- 查看提交历史
		```bash
		git log --oneline --graph --decorate -20
		```

- 推送
	- 推送当前分支
		```bash
		git push
		```
	- 首次推送新分支
		```bash
		git push -u origin <branch-name>
		```
	- 推送标签
		```bash
		git push origin --tags
		```

- 合并分支
	- 将功能分支合并到主分支（本地示例）
		```bash
		git checkout main
		git pull origin main
		git merge --no-ff feat/login
		git push origin main
		```
	- 发生冲突后：编辑冲突文件、重新暂存并完成合并
		```bash
		git add .
		git commit
		```

- 回退提交
	- 安全回退（推荐，保留历史，适合已推送提交）
		```bash
		git revert <commit-hash>
		git push
		```
	- 本地撤销最近一次提交但保留改动到暂存区
		```bash
		git reset --soft HEAD~1
		```
	- 本地强制回退（会丢失改动，谨慎使用）
		```bash
		git reset --hard HEAD~1
		```

- 变基拉取
	- 让本地提交“接在”远端最新提交之后，保持更线性的提交历史
		```bash
		git pull --rebase origin main
		```
	- 处理冲突后的续跑/放弃
		```bash
		git add .
		git rebase --continue
		# 或
		git rebase --abort
		```

> 小建议：日常开发优先使用 `git fetch + git rebase` 或 `git pull --rebase` 同步；已推送到共享分支的提交，尽量使用 `git revert`，避免 `git reset --hard` + 强推影响他人。