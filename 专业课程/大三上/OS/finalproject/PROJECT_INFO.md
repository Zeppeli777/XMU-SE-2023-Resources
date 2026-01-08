# MYSHELL 项目说明

## 项目概述

这是一个操作系统课程的 Shell 程序设计项目，实现了一个功能完整的命令行解释器。

## 项目结构

```
finalproject/
├── myshell.c          # 主程序文件
├── utility.c          # 工具函数和内置命令实现
├── myshell.h          # 头文件
├── makefile           # 编译配置文件
├── readme             # 用户手册（必读）
├── test_batch.txt     # 批处理测试文件示例
└── docs/
    └── project.md     # 项目要求文档
```

## 功能特性

### 1. 内置命令
- **cd** - 改变当前目录
- **clr** - 清屏
- **dir** - 列出目录内容
- **environ** - 显示环境变量
- **echo** - 显示文本
- **help** - 显示用户手册
- **pause** - 暂停等待用户输入
- **quit** - 退出 Shell

### 2. 外部程序执行
支持执行任何外部程序，如 ls、gcc、ps 等系统命令。

### 3. I/O 重定向
- 输入重定向：`command < inputfile`
- 输出重定向：`command > outputfile`（覆盖）
- 输出重定向：`command >> outputfile`（追加）

### 4. 后台执行
使用 `&` 符号将程序放到后台运行：`command &`

### 5. 批处理模式
支持从文件读取并执行命令：`./myshell batchfile`

### 6. 环境变量
自动设置 shell 环境变量为可执行文件的完整路径。

## 编译说明

在 Ubuntu/Linux 环境中编译：

```bash
# 清理之前的编译文件
make clean

# 编译项目
make

# 编译后会生成 myshell 可执行文件
```

## 使用说明

### 交互模式
```bash
./myshell
```

启动后会显示当前目录的提示符，等待用户输入命令。

### 批处理模式
```bash
./myshell test_batch.txt
```

从文件读取命令并执行，执行完毕后自动退出。

## 使用示例

### 基本命令
```bash
/home/user> cd /tmp
/tmp> dir
/tmp> echo Hello World
/tmp> clr
/tmp> quit
```

### I/O 重定向
```bash
/home/user> dir > filelist.txt
/home/user> echo Log entry >> log.txt
/home/user> sort < unsorted.txt > sorted.txt
```

### 后台执行
```bash
/home/user> sleep 100 &
[Background] PID: 12345
/home/user>
```

### 外部命令
```bash
/home/user> ls -la
/home/user> gcc myprogram.c -o myprogram
/home/user> ./myprogram
```

## 注意事项

1. **编译环境**：需要在 Linux 环境（如 Ubuntu 虚拟机）中编译和运行
2. **依赖**：需要 gcc 编译器和标准 C 库
3. **文件命名**：
   - Makefile 必须全部小写：`makefile`
   - 用户手册必须命名为：`readme`（无扩展名）
   - 可执行文件必须命名为：`myshell`
4. **help 命令**：需要 readme 文件在当前目录才能正常工作

## 测试建议

1. **测试内置命令**：逐个测试所有内置命令
2. **测试 I/O 重定向**：测试输入输出重定向功能
3. **测试后台执行**：使用 sleep 命令测试后台执行
4. **测试批处理**：使用提供的 test_batch.txt 测试批处理模式
5. **测试外部命令**：执行各种系统命令

## 代码特点

- **详细注释**：每个函数都有详细的注释说明
- **错误处理**：完善的错误检查和提示
- **模块化设计**：主程序、工具函数、头文件分离
- **符合规范**：严格按照项目要求实现所有功能

## 提交清单

确保提交以下文件：
- [x] myshell.c
- [x] utility.c
- [x] myshell.h
- [x] makefile
- [x] readme

**不要提交**：
- 二进制文件（myshell 可执行文件）
- 目标文件（*.o）
- 其他临时文件

## 故障排除

### 编译失败
```bash
# 确保安装了 gcc
sudo apt-get install build-essential

# 检查文件名大小写是否正确
ls -l
```

### help 命令不工作
```bash
# 确保 readme 文件在当前目录
cp readme /path/to/current/directory/
```

### 权限错误
```bash
# 给予执行权限
chmod +x myshell
```

## 联系信息

操作系统课程项目
日期：2025年12月28日
