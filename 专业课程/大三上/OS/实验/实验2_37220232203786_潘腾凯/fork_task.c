#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

// 随机休眠一段时间，模拟进程执行任务
void random_sleep() {
    // 生成1-3秒的随机休眠时间
    srand(time(NULL) ^ getpid()); // 使用异或pid确保不同进程随机数不同
    int sleep_time = (rand() % 3) + 1;
    sleep(sleep_time);
}

int main() {
    pid_t pid1, pid2;
    
    // 创建第一个子进程
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork失败");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        printf("子进程1正在执行..... (PID: %d)\n", getpid());
        random_sleep(); // 模拟子进程执行任务
        // 如果没被父进程杀死，会执行到这里
        printf("子进程1执行完毕\n");
        exit(EXIT_SUCCESS);
    }
    
    // 创建第二个子进程
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork失败");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        printf("子进程2正在执行..... (PID: %d)\n", getpid());
        random_sleep(); // 模拟子进程执行任务
        // 如果没被父进程杀死，会执行到这里
        printf("子进程2执行完毕\n");
        exit(EXIT_SUCCESS);
    }
    
    // 父进程代码
    printf("父进程正在执行..... (PID: %d)\n", getpid());
    
    // 让子进程运行一会儿
    sleep(2);
    
    // 杀死子进程1
    if (kill(pid1, SIGKILL) == 0) {
        printf("子进程1被父进程杀死\n");
    }
    
    // 杀死子进程2
    if (kill(pid2, SIGKILL) == 0) {
        printf("子进程2被父进程杀死\n");
    }
    
    // 等待子进程结束，避免僵尸进程
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    // 父进程结束
    printf("父进程结束\n");
    return 0;
}
    