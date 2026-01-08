#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define READER_NUM 3
#define WRITER_NUM 2
#define READ_COUNT 5
#define WRITE_COUNT 3

// 信号量索引
#define MUTEX 0      // 保护读者计数
#define WRITE_SEM 1  // 写信号量

// 共享内存结构
struct shared_data {
    int data;
    int read_count;
};

// semun联合体定义
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// P操作（等待）
void P(int sem_id, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("P operation failed");
        exit(1);
    }
}

// V操作（信号）
void V(int sem_id, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("V operation failed");
        exit(1);
    }
}

// 初始化信号量
int init_semaphore() {
    int sem_id;
    union semun arg;
    unsigned short values[2];
    
    // 创建信号量集
    sem_id = semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }
    
    // 初始化信号量值
    values[MUTEX] = 1;     // 互斥信号量
    values[WRITE_SEM] = 1; // 写信号量
    
    arg.array = values;
    if (semctl(sem_id, 0, SETALL, arg) == -1) {
        perror("semctl SETALL failed");
        exit(1);
    }
    
    return sem_id;
}

// 读者进程
void reader(int sem_id, struct shared_data *shared, int id) {
    srand(time(NULL) + getpid());
    
    for (int i = 0; i < READ_COUNT; i++) {
        // 进入读者计数临界区
        P(sem_id, MUTEX);
        shared->read_count++;
        if (shared->read_count == 1) {
            // 第一个读者需要锁定写操作
            P(sem_id, WRITE_SEM);
        }
        V(sem_id, MUTEX);
        
        // 读取数据
        printf("读者 %d: 读取数据 = %d (当前读者数: %d)\n", 
               id, shared->data, shared->read_count);
        sleep(rand() % 2 + 1);
        
        // 离开读者计数临界区
        P(sem_id, MUTEX);
        shared->read_count--;
        if (shared->read_count == 0) {
            // 最后一个读者释放写锁
            V(sem_id, WRITE_SEM);
        }
        V(sem_id, MUTEX);
        
        sleep(rand() % 3);
    }
    
    printf("读者 %d 完成任务\n", id);
}

// 写者进程
void writer(int sem_id, struct shared_data *shared, int id) {
    srand(time(NULL) + getpid());
    
    for (int i = 0; i < WRITE_COUNT; i++) {
        // 请求写锁
        P(sem_id, WRITE_SEM);
        
        // 写入数据
        int new_data = rand() % 1000;
        shared->data = new_data;
        printf("写者 %d: 写入数据 = %d\n", id, new_data);
        sleep(rand() % 2 + 1);
        
        // 释放写锁
        V(sem_id, WRITE_SEM);
        
        sleep(rand() % 3);
    }
    
    printf("写者 %d 完成任务\n", id);
}

int main() {
    int sem_id, shm_id;
    struct shared_data *shared;
    pid_t pid;
    
    printf("=== 读者-写者问题实验（读者优先） ===\n");
    printf("读者数量: %d\n", READER_NUM);
    printf("写者数量: %d\n", WRITER_NUM);
    printf("每个读者读取次数: %d\n", READ_COUNT);
    printf("每个写者写入次数: %d\n\n", WRITE_COUNT);
    
    // 创建共享内存
    shm_id = shmget(IPC_PRIVATE, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    
    // 连接共享内存
    shared = (struct shared_data *)shmat(shm_id, NULL, 0);
    if (shared == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    // 初始化共享数据
    shared->data = 0;
    shared->read_count = 0;
    
    // 初始化信号量
    sem_id = init_semaphore();
    
    // 创建读者进程
    for (int i = 0; i < READER_NUM; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            // 子进程：读者
            reader(sem_id, shared, i + 1);
            exit(0);
        }
    }
    
    // 创建写者进程
    for (int i = 0; i < WRITER_NUM; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            // 子进程：写者
            writer(sem_id, shared, i + 1);
            exit(0);
        }
    }
    
    // 等待所有子进程结束
    for (int i = 0; i < READER_NUM + WRITER_NUM; i++) {
        wait(NULL);
    }
    
    printf("\n所有进程执行完毕！\n");
    printf("最终数据值: %d\n", shared->data);
    
    // 清理资源
    shmdt(shared);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    
    return 0;
}