#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define BUFFER_SIZE 5
#define PRODUCER_NUM 2
#define CONSUMER_NUM 2
#define PRODUCE_NUM 10

// 信号量索引
#define MUTEX 0
#define EMPTY 1
#define FULL 2

// 共享内存结构
struct shared_buffer {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
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
    unsigned short values[3];
    
    // 创建信号量集
    sem_id = semget(IPC_PRIVATE, 3, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }
    
    // 初始化信号量值
    values[MUTEX] = 1;           // 互斥信号量
    values[EMPTY] = BUFFER_SIZE; // 空缓冲区数量
    values[FULL] = 0;            // 满缓冲区数量
    
    arg.array = values;
    if (semctl(sem_id, 0, SETALL, arg) == -1) {
        perror("semctl SETALL failed");
        exit(1);
    }
    
    return sem_id;
}

// 生产者进程
void producer(int sem_id, struct shared_buffer *buf, int id) {
    srand(time(NULL) + getpid());
    
    for (int i = 0; i < PRODUCE_NUM; i++) {
        int item = rand() % 100;
        
        // P(empty) - 等待空缓冲区
        P(sem_id, EMPTY);
        
        // P(mutex) - 进入临界区
        P(sem_id, MUTEX);
        
        // 生产物品到缓冲区
        buf->buffer[buf->in] = item;
        printf("生产者 %d: 生产物品 %d 到缓冲区位置 %d\n", id, item, buf->in);
        buf->in = (buf->in + 1) % BUFFER_SIZE;
        
        // V(mutex) - 离开临界区
        V(sem_id, MUTEX);
        
        // V(full) - 增加满缓冲区计数
        V(sem_id, FULL);
        
        sleep(rand() % 3);
    }
    
    printf("生产者 %d 完成任务\n", id);
}

// 消费者进程
void consumer(int sem_id, struct shared_buffer *buf, int id) {
    srand(time(NULL) + getpid());
    
    for (int i = 0; i < (PRODUCER_NUM * PRODUCE_NUM) / CONSUMER_NUM; i++) {
        // P(full) - 等待满缓冲区
        P(sem_id, FULL);
        
        // P(mutex) - 进入临界区
        P(sem_id, MUTEX);
        
        // 从缓冲区消费物品
        int item = buf->buffer[buf->out];
        printf("消费者 %d: 从缓冲区位置 %d 消费物品 %d\n", id, buf->out, item);
        buf->out = (buf->out + 1) % BUFFER_SIZE;
        
        // V(mutex) - 离开临界区
        V(sem_id, MUTEX);
        
        // V(empty) - 增加空缓冲区计数
        V(sem_id, EMPTY);
        
        sleep(rand() % 3);
    }
    
    printf("消费者 %d 完成任务\n", id);
}

int main() {
    int sem_id, shm_id;
    struct shared_buffer *buf;
    pid_t pid;
    
    printf("=== 生产者-消费者问题实验 ===\n");
    printf("缓冲区大小: %d\n", BUFFER_SIZE);
    printf("生产者数量: %d\n", PRODUCER_NUM);
    printf("消费者数量: %d\n", CONSUMER_NUM);
    printf("每个生产者生产数量: %d\n\n", PRODUCE_NUM);
    
    // 创建共享内存
    shm_id = shmget(IPC_PRIVATE, sizeof(struct shared_buffer), 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    
    // 连接共享内存
    buf = (struct shared_buffer *)shmat(shm_id, NULL, 0);
    if (buf == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    // 初始化共享缓冲区
    buf->in = 0;
    buf->out = 0;
    
    // 初始化信号量
    sem_id = init_semaphore();
    
    // 创建生产者进程
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            // 子进程：生产者
            producer(sem_id, buf, i + 1);
            exit(0);
        }
    }
    
    // 创建消费者进程
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            // 子进程：消费者
            consumer(sem_id, buf, i + 1);
            exit(0);
        }
    }
    
    // 等待所有子进程结束
    for (int i = 0; i < PRODUCER_NUM + CONSUMER_NUM; i++) {
        wait(NULL);
    }
    
    printf("\n所有进程执行完毕！\n");
    
    // 清理资源
    shmdt(buf);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    
    return 0;
}