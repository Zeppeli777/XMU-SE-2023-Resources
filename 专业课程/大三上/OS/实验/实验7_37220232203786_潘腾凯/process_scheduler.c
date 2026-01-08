#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define MAX_PROCESSES 100

// 进程结构体
typedef struct {
    char name;              // 进程名
    int arrival_time;       // 到达时间
    int service_time;       // 服务时间
    int remaining_time;     // 剩余时间
    int completion_time;    // 完成时间
    int turnaround_time;    // 周转时间
    float response_ratio;   // 响应比
    int start_time;         // 首次执行时间
    int has_started;        // 是否已开始执行
} Process;

// 打印表头
void print_header(const char* algorithm) {
    printf("调度算法: %s\n", algorithm);
    printf("\n");
    printf("进程\t完成时间\t周转时间\t响应比\n");
}

// 打印进程结果
void print_process(Process p) {
    printf("%c\t%d\t\t%d\t\t%.2f\n", 
           p.name, p.completion_time, p.turnaround_time, p.response_ratio);
}

// 打印统计信息
void print_statistics(Process processes[], int n) {
    float avg_turnaround = 0;
    float avg_response_ratio = 0;
    
    for (int i = 0; i < n; i++) {
        avg_turnaround += processes[i].turnaround_time;
        avg_response_ratio += processes[i].response_ratio;
    }
    
    avg_turnaround /= n;
    avg_response_ratio /= n;
    
    printf("--------------------------------------------------\n");
    printf("平均周转时间: %.2f\n", avg_turnaround);
    printf("平均响应比: %.2f\n", avg_response_ratio);
    printf("\n");
}

// 初始化进程数组
void init_processes(Process dest[], Process src[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
        dest[i].remaining_time = src[i].service_time;
        dest[i].completion_time = 0;
        dest[i].turnaround_time = 0;
        dest[i].response_ratio = 0.0;
        dest[i].start_time = -1;
        dest[i].has_started = 0;
    }
}

// 计算周转时间和响应比
void calculate_metrics(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].response_ratio = (float)processes[i].turnaround_time / processes[i].service_time;
    }
}

// 1. 先来先服务 FCFS (First Come First Served)
void fcfs(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    init_processes(temp, processes, n);
    
    // 按到达时间排序
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j].arrival_time > temp[j + 1].arrival_time) {
                Process t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < temp[i].arrival_time) {
            current_time = temp[i].arrival_time;
        }
        current_time += temp[i].service_time;
        temp[i].completion_time = current_time;
    }
    
    calculate_metrics(temp, n);
    print_header("先来先服务 FCFS");
    for (int i = 0; i < n; i++) {
        print_process(temp[i]);
    }
    print_statistics(temp, n);
}

// 2. 轮转调度 RR (Round Robin, q=1)
void rr(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    init_processes(temp, processes, n);
    
    int current_time = 0;
    int completed = 0;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;
    int in_queue[MAX_PROCESSES] = {0};
    
    // 找到第一个到达的进程
    int first_arrival = temp[0].arrival_time;
    for (int i = 1; i < n; i++) {
        if (temp[i].arrival_time < first_arrival) {
            first_arrival = temp[i].arrival_time;
        }
    }
    current_time = first_arrival;
    
    // 将当前时间已到达的进程加入队列
    for (int i = 0; i < n; i++) {
        if (temp[i].arrival_time <= current_time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }
    
    while (completed < n) {
        if (front == rear) {
            // 队列为空，找下一个到达的进程
            int next_arrival = -1;
            for (int i = 0; i < n; i++) {
                if (temp[i].remaining_time > 0) {
                    if (next_arrival == -1 || temp[i].arrival_time < temp[next_arrival].arrival_time) {
                        next_arrival = i;
                    }
                }
            }
            if (next_arrival != -1) {
                current_time = temp[next_arrival].arrival_time;
                queue[rear++] = next_arrival;
                in_queue[next_arrival] = 1;
            }
        }
        
        int idx = queue[front++];
        
        // 执行一个时间片
        temp[idx].remaining_time--;
        current_time++;
        
        // 检查新到达的进程
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        // 如果进程未完成，重新加入队列
        if (temp[idx].remaining_time > 0) {
            queue[rear++] = idx;
        } else {
            temp[idx].completion_time = current_time;
            completed++;
            in_queue[idx] = 0;
        }
    }
    
    calculate_metrics(temp, n);
    print_header("轮转调度 RR (q=1)");
    for (int i = 0; i < n; i++) {
        print_process(temp[i]);
    }
    print_statistics(temp, n);
}

// 3. 最短进程优先 SPN  - 非抢占
void spn(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    init_processes(temp, processes, n);
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    
    while (completed < n) {
        int idx = -1;
        int min_service_time = 999999;
        
        // 选择已到达且服务时间最短的进程
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && temp[i].arrival_time <= current_time) {
                if (temp[i].service_time < min_service_time) {
                    min_service_time = temp[i].service_time;
                    idx = i;
                }
            }
        }
        
        if (idx == -1) {
            // 没有已到达的进程，找下一个最早到达的
            int next_arrival_time = 999999;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && temp[i].arrival_time < next_arrival_time) {
                    next_arrival_time = temp[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
            continue;
        }
        
        current_time += temp[idx].service_time;
        temp[idx].completion_time = current_time;
        is_completed[idx] = 1;
        completed++;
    }
    
    calculate_metrics(temp, n);
    print_header("最短进程优先 SPN");
    for (int i = 0; i < n; i++) {
        print_process(temp[i]);
    }
    print_statistics(temp, n);
}

// 4. 最短剩余时间 SRT - 抢占式
void srt(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    init_processes(temp, processes, n);
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int idx = -1;
        int min_remaining_time = 999999;
        
        // 选择剩余时间最短的已到达进程
        for (int i = 0; i < n; i++) {
            if (temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                if (temp[i].remaining_time < min_remaining_time) {
                    min_remaining_time = temp[i].remaining_time;
                    idx = i;
                }
            }
        }
        
        if (idx == -1) {
            // 没有已到达的进程，跳到下一个进程到达时间
            int next_arrival_time = 999999;
            for (int i = 0; i < n; i++) {
                if (temp[i].remaining_time > 0 && temp[i].arrival_time < next_arrival_time) {
                    next_arrival_time = temp[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
            continue;
        }
        
        // 执行一个时间单位
        temp[idx].remaining_time--;
        current_time++;
        
        if (temp[idx].remaining_time == 0) {
            temp[idx].completion_time = current_time;
            completed++;
        }
    }
    
    calculate_metrics(temp, n);
    print_header("最短剩余时间 SRT");
    for (int i = 0; i < n; i++) {
        print_process(temp[i]);
    }
    print_statistics(temp, n);
}

// 5. 最高响应比优先 HRRN 
void hrrn(Process processes[], int n) {
    Process temp[MAX_PROCESSES];
    init_processes(temp, processes, n);
    
    int current_time = 0;
    int completed = 0;
    int is_completed[MAX_PROCESSES] = {0};
    
    while (completed < n) {
        int idx = -1;
        float max_response_ratio = -1;
        
        // 计算所有已到达未完成进程的响应比，选择最高的
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && temp[i].arrival_time <= current_time) {
                int waiting_time = current_time - temp[i].arrival_time;
                float response_ratio = (waiting_time + temp[i].service_time) / (float)temp[i].service_time;
                
                if (response_ratio > max_response_ratio) {
                    max_response_ratio = response_ratio;
                    idx = i;
                }
            }
        }
        
        if (idx == -1) {
            // 没有已到达的进程，找下一个最早到达的
            int next_arrival_time = 999999;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && temp[i].arrival_time < next_arrival_time) {
                    next_arrival_time = temp[i].arrival_time;
                }
            }
            current_time = next_arrival_time;
            continue;
        }
        
        current_time += temp[idx].service_time;
        temp[idx].completion_time = current_time;
        is_completed[idx] = 1;
        completed++;
    }
    
    calculate_metrics(temp, n);
    print_header("最高响应比优先 HRRN");
    for (int i = 0; i < n; i++) {
        print_process(temp[i]);
    }
    print_statistics(temp, n);
}

int main() {
    // 示例进程数据
    Process processes[] = {
        {'A', 0, 3, 3, 0, 0, 0.0, -1, 0},
        {'B', 2, 6, 6, 0, 0, 0.0, -1, 0},
        {'C', 4, 4, 4, 0, 0, 0.0, -1, 0},
        {'D', 6, 5, 5, 0, 0, 0.0, -1, 0},
        {'E', 8, 2, 2, 0, 0, 0.0, -1, 0}
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    
    printf("\n");
    printf("        进程调度算法模拟实验\n");
    printf("\n");
    
    printf("\n原始进程信息:\n");
    printf("--------------------------------------------------\n");
    printf("进程\t到达时间\t服务时间\n");
    for (int i = 0; i < n; i++) {
        printf("%c\t%d\t\t%d\n", processes[i].name, 
               processes[i].arrival_time, processes[i].service_time);
    }
    printf("--------------------------------------------------\n");
    
    // 执行各种调度算法
    fcfs(processes, n);
    rr(processes, n);
    spn(processes, n);
    srt(processes, n);
    hrrn(processes, n);
    
    return 0;
}
