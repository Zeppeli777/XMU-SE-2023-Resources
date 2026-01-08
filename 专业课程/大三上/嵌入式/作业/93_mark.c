#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int global = 22; // 全局变量：fork 后父子进程各自拷贝一份 
char buf[] = "the test content!\n"; 

int main(void)
{
	int test = 0, stat; // test 为局部变量，fork 后父子各自有独立拷贝；stat 未使用（保留用于 wait 等） 
	pid_t pid;

	// 直接使用 write 向 STDOUT 写入，展示低级 IO 与 stdio 的混合用法 
	if (write(STDOUT_FILENO, buf, sizeof(buf)) != sizeof(buf))
	{
		perror("write error!");
	}

	printf(" fork test!\n");

	// 调用 fork 创建子进程，返回值 pid 在父进程为子 PID，在子进程为 0，失败返回 -1
	pid = fork(); 
	if (pid == -1)
	{
		// fork 出错，打印并退出 
		perror("fork");
		exit(0);
	}
	else if (pid == 0)
	{
		// 子进程路径：修改全局变量与局部变量（仅对子进程的内存有效） 
		global++;
		test++;
		printf("global=%d test=%d Child,my PID is %d\n", global, test, getpid());
		exit(0); // 子进程结束 
	}

	// 父进程路径（pid > 0）：对子进程没有影响的修改
	global += 2;
	test += 2;
	printf("global=%d test=%d Parent,my PID is %d\n", global, test, getpid());
	exit(0);

}
	
