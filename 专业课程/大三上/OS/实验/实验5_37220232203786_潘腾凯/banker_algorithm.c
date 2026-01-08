#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESS 10
#define MAX_RESOURCE 10

// 全局变量
int n_process;      // 进程数
int n_resource;     // 资源数
int Max[MAX_PROCESS][MAX_RESOURCE];        // 最大需求矩阵
int Allocation[MAX_PROCESS][MAX_RESOURCE]; // 分配矩阵
int Need[MAX_PROCESS][MAX_RESOURCE];       // 需求矩阵
int Available[MAX_RESOURCE];               // 可用资源向量
int Total[MAX_RESOURCE];                   // 总资源向量
bool Finish[MAX_PROCESS];                  // 进程完成标志
bool Process_Started[MAX_PROCESS];         // 进程启动标志

// 函数声明
void init_system();
void print_state();
void print_safe_sequence(int sequence[], int len);
bool is_safe_state(int safe_sequence[], int *seq_len);
bool try_allocate(int process_id, int request[]);
void release_resource(int process_id);
void run_simulation();
void wait_for_enter();

// 初始化系统
void init_system() {
    printf("========== 银行家算法死锁避免系统 ==========\n\n");
    
    printf("请输入进程数量: ");
    scanf("%d", &n_process);
    
    printf("请输入资源种类数量: ");
    scanf("%d", &n_resource);
    
    printf("\n请输入总资源向量 R (共%d个资源):\n", n_resource);
    for (int i = 0; i < n_resource; i++) {
        printf("资源 %d: ", i);
        scanf("%d", &Total[i]);
        Available[i] = Total[i];  // 初始时所有资源都可用
    }
    
    printf("\n请输入最大需求矩阵 C (%d个进程 x %d种资源):\n", n_process, n_resource);
    for (int i = 0; i < n_process; i++) {
        printf("进程 P%d 的最大需求: ", i);
        for (int j = 0; j < n_resource; j++) {
            scanf("%d", &Max[i][j]);
            Need[i][j] = Max[i][j];        // 初始需求等于最大需求
            Allocation[i][j] = 0;           // 初始分配为0
        }
        Finish[i] = false;
        Process_Started[i] = false;
    }
    
    printf("\n系统初始化完成！\n");
    printf("初始状态：所有进程未启动\n");
}

// 打印当前系统状态
void print_state() {
    printf("\n========== 当前系统状态 ==========\n");
    
    // 打印分配矩阵
    printf("\n分配矩阵 Allocation:\n");
    printf("进程\t");
    for (int j = 0; j < n_resource; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    for (int i = 0; i < n_process; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < n_resource; j++) {
            printf("%d\t", Allocation[i][j]);
        }
        printf("\n");
    }
    
    // 打印需求矩阵
    printf("\n需求矩阵 Need:\n");
    printf("进程\t");
    for (int j = 0; j < n_resource; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    for (int i = 0; i < n_process; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < n_resource; j++) {
            printf("%d\t", Need[i][j]);
        }
        printf("\n");
    }
    
    // 打印可用资源向量
    printf("\n可用资源向量 Available:\n");
    for (int i = 0; i < n_resource; i++) {
        printf("R%d: %d\t", i, Available[i]);
    }
    printf("\n");
    
    // 打印进程启动状态
    printf("\n进程启动状态:\n");
    for (int i = 0; i < n_process; i++) {
        printf("P%d: %s\t", i, Process_Started[i] ? "已启动" : "未启动");
    }
    printf("\n");
}

// 打印安全序列
void print_safe_sequence(int sequence[], int len) {
    printf("安全序列: ");
    if (len == 0) {
        printf("(空)\n");
        return;
    }
    for (int i = 0; i < len; i++) {
        printf("P%d", sequence[i]);
        if (i < len - 1) printf(" -> ");
    }
    printf("\n");
}

// 安全性检查算法
bool is_safe_state(int safe_sequence[], int *seq_len) {
    int Work[MAX_RESOURCE];
    bool Finish_temp[MAX_PROCESS];
    int count = 0;
    
    // 初始化工作向量
    for (int i = 0; i < n_resource; i++) {
        Work[i] = Available[i];
    }
    
    // 初始化完成标志
    for (int i = 0; i < n_process; i++) {
        Finish_temp[i] = false;
    }
    
    // 寻找安全序列
    bool found;
    int iteration = 0;
    do {
        found = false;
        for (int i = 0; i < n_process; i++) {
            // 只考虑已启动的进程
            if (Process_Started[i] && !Finish_temp[i]) {
                // 检查是否满足 Need[i] <= Work
                bool can_allocate = true;
                for (int j = 0; j < n_resource; j++) {
                    if (Need[i][j] > Work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                
                if (can_allocate) {
                    // 模拟分配资源并回收
                    for (int j = 0; j < n_resource; j++) {
                        Work[j] += Allocation[i][j];
                    }
                    Finish_temp[i] = true;
                    safe_sequence[count++] = i;
                    found = true;
                    
                    // 调试
                    // printf("  [安全性检查] 第%d轮: 进程P%d可完成, Work更新为: ", iteration, i);
                    // for (int k = 0; k < n_resource; k++) printf("%d ", Work[k]);
                    // printf("\n");
                }
            }
        }
        iteration++;
    } while (found && iteration < n_process * 2); // 防止无限循环
    
    // 检查是否所有已启动的进程都能完成
    for (int i = 0; i < n_process; i++) {
        if (Process_Started[i] && !Finish_temp[i]) {
            *seq_len = 0;  // 不安全，返回空序列
            return false;  // 存在已启动但无法完成的进程，不安全
        }
    }
    
    *seq_len = count;  // 设置安全序列长度
    return true;  // 所有已启动的进程都能完成，安全
}

// 尝试分配资源
bool try_allocate(int process_id, int request[]) {
    printf("\n========== 处理申请 ==========\n");
    printf("进程 P%d 申请资源: ", process_id);
    for (int i = 0; i < n_resource; i++) {
        printf("R%d=%d ", i, request[i]);
    }
    printf("\n");
    
    // 检查进程是否已启动
    if (!Process_Started[process_id]) {
        printf(">> 这是进程 P%d 的首次申请（进程启动检查）...\n", process_id);
    }
    
    // 检查申请是否超过需求
    for (int i = 0; i < n_resource; i++) {
        if (request[i] > Need[process_id][i]) {
            printf(">> 拒绝：申请资源R%d=%d 超过当前需求 Need=%d！\n", 
                   i, request[i], Need[process_id][i]);
            return false;
        }
    }
    
    // 检查申请是否超过可用资源
    for (int i = 0; i < n_resource; i++) {
        if (request[i] > Available[i]) {
            printf(">> 拒绝：资源R%d不足（申请=%d，可用=%d），进程需等待！\n", 
                   i, request[i], Available[i]);
            return false;
        }
    }
    
    // 试探性分配
    printf(">> 试探性分配资源...\n");
    for (int i = 0; i < n_resource; i++) {
        Available[i] -= request[i];
        Allocation[process_id][i] += request[i];
        Need[process_id][i] -= request[i];
    }
    
    // 暂时标记进程为已启动（用于安全性检查）
    bool was_started = Process_Started[process_id];
    Process_Started[process_id] = true;
    
    // 安全性检查
    int safe_sequence[MAX_PROCESS];
    int seq_len = 0;
    printf(">>> 执行安全性检查...\n");
    
    if (is_safe_state(safe_sequence, &seq_len)) {
        printf(">> 结果：系统处于安全状态！\n");
        print_safe_sequence(safe_sequence, seq_len);
        return true;
    } else {
        printf(">> 结果：系统处于不安全状态！\n");
        printf(">> 拒绝分配，恢复原状态...\n");
        // 恢复资源
        for (int i = 0; i < n_resource; i++) {
            Available[i] += request[i];
            Allocation[process_id][i] -= request[i];
            Need[process_id][i] += request[i];
        }
        Process_Started[process_id] = was_started; // 恢复启动状态
        return false;
    }
}

// 释放进程资源
void release_resource(int process_id) {
    printf("\n========== 进程完成 ==========\n");
    printf("进程 P%d 执行完毕，释放资源: ", process_id);
    for (int i = 0; i < n_resource; i++) {
        printf("R%d=%d ", i, Allocation[process_id][i]);
        Available[i] += Allocation[process_id][i];
        Allocation[process_id][i] = 0;
        Need[process_id][i] = Max[process_id][i]; // 恢复需求为最大需求
    }
    printf("\n");
    Process_Started[process_id] = false;
    Finish[process_id] = true;
}

// 等待用户按Enter键继续
void wait_for_enter() {
    printf("\n按 Enter 键继续下一个申请...");
    while (getchar() != '\n');
    getchar();
}

// 运行模拟
void run_simulation() {
    int n_requests;
    printf("\n请输入申请序列的数量: ");
    scanf("%d", &n_requests);
    
    printf("\n申请格式说明：\n");
    printf("  1.资源申请：输入进程ID，然后输入申请的资源向量\n");
    printf("  示例：0 [Enter] 1 0 2 表示进程P0申请资源(1,0,2)\n");
    printf("  2.进程完成：输入-1，然后输入要释放资源的进程ID\n");
    printf("  示例：-1 [Enter] 0 表示进程P0完成并释放资源\n\n");
    
    for (int i = 0; i < n_requests; i++) {
        int process_id;
        int request[MAX_RESOURCE];
        
        printf("\n==========================================\n");
        printf("[申请 %d/%d]\n", i + 1, n_requests);
        printf("==========================================\n");
        printf("进程ID (-1表示释放资源): ");
        scanf("%d", &process_id);
        
        // 处理资源释放
        if (process_id == -1) {
            printf("要释放资源的进程ID: ");
            int release_id;
            scanf("%d", &release_id);
            if (release_id >= 0 && release_id < n_process && Process_Started[release_id]) {
                release_resource(release_id);
                print_state();
            } else {
                printf("错误：无效的进程ID或进程未启动！\n");
            }
            if (i < n_requests - 1) wait_for_enter();
            continue;
        }
        
        // 验证进程ID
        if (process_id < 0 || process_id >= n_process) {
            printf("错误：无效的进程ID（应在0-%d之间）！\n", n_process - 1);
            i--;
            continue;
        }
        
        // 输入申请资源向量
        printf("申请资源向量 (共%d个): ", n_resource);
        for (int j = 0; j < n_resource; j++) {
            scanf("%d", &request[j]);
        }
        
        // 尝试分配
        if (try_allocate(process_id, request)) {
            printf("\n 分配成功！\n");
            print_state();
        } else {
            printf("\n 分配失败！\n");
            print_state();
        }
        
        if (i < n_requests - 1) {
            wait_for_enter();
        }
    }
}

int main() {
    init_system();
    print_state();
    run_simulation();
    
    printf("\n========== 模拟结束 ==========\n");
    return 0;
}