#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

// 结构体定义
typedef struct {
    int *pages;           // 页面序列
    int length;           // 序列长度
    int frame_count;      // 物理帧数
} PageSequence;

typedef struct {
    int page_faults;      // 缺页次数
    int page_hits;        // 命中次数
    double fault_rate;    // 缺页率
    double hit_rate;      // 命中率
} Statistics;

// 函数声明
void print_menu();
void input_sequence(PageSequence *seq);
void display_sequence(PageSequence *seq);
Statistics fifo_algorithm(PageSequence *seq);
Statistics lru_algorithm(PageSequence *seq);
Statistics opt_algorithm(PageSequence *seq);
void print_statistics(const char *algorithm_name, Statistics stats, int total);
bool is_in_frame(int *frames, int frame_count, int page);
int find_optimal_victim(PageSequence *seq, int *frames, int frame_count, int current_pos);

int main() {
    PageSequence seq;
    seq.pages = NULL;
    
    int choice;
    
    printf("内存页面置换算法模拟\n\n");

    
    while (1) {
        print_menu();
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                input_sequence(&seq);
                break;
            case 2:
                if (seq.pages == NULL) {
                    printf("\n请先输入页面序列！\n");
                } else {
                    display_sequence(&seq);
                }
                break;
            case 3:
                if (seq.pages == NULL) {
                    printf("\n请先输入页面序列！\n");
                } else {
                    Statistics stats = fifo_algorithm(&seq);
                    print_statistics("FIFO (先进先出)", stats, seq.length);
                }
                break;
            case 4:
                if (seq.pages == NULL) {
                    printf("\n请先输入页面序列！\n");
                } else {
                    Statistics stats = lru_algorithm(&seq);
                    print_statistics("LRU (最近最少使用)", stats, seq.length);
                }
                break;
            case 5:
                if (seq.pages == NULL) {
                    printf("\n请先输入页面序列！\n");
                } else {
                    Statistics stats = opt_algorithm(&seq);
                    print_statistics("OPT (最佳置换)", stats, seq.length);
                }
                break;
            case 6:
                if (seq.pages == NULL) {
                    printf("\n请先输入页面序列！\n");
                } else {
                    printf("\n算法对比分析:\n\n");
                    Statistics fifo_stats = fifo_algorithm(&seq);
                    print_statistics("FIFO", fifo_stats, seq.length);
                    
                    Statistics lru_stats = lru_algorithm(&seq);
                    print_statistics("LRU", lru_stats, seq.length);
                    
                    Statistics opt_stats = opt_algorithm(&seq);
                    print_statistics("OPT", opt_stats, seq.length);
                    

                }
                break;
            case 0:
                if (seq.pages != NULL) {
                    free(seq.pages);
                }
                printf("\n结束程序\n");
                return 0;
            default:

                printf("\n无效选择，请重新输入\n");
        }
        printf("\n");
    }
    
    return 0;
}

void print_menu() {
    printf("\n----------------------------------------------\n");
    printf("1. 输入页面访问序列\n");
    printf("2. 显示当前页面序列\n");
    printf("3. FIFO算法模拟\n");
    printf("4. LRU算法模拟\n");
    printf("5. OPT算法模拟\n");
    printf("6. 对比所有算法\n");
    printf("0. 退出程序\n");
    printf("----------------------------------------------\n");
}

void input_sequence(PageSequence *seq) {
    int choice;
    
    printf("\n选择输入方式：\n");
    printf("1. 手动输入\n");
    printf("2. 使用默认测试序列\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    if (choice == 2) {
        // 默认测试序列
        int default_pages[] = {1, 5, 3, 4, 2, 1, 3, 4, 5, 7, 9, 1, 2, 3, 4, 5};
        seq->length = 16;
        seq->frame_count = 3;
        
        if (seq->pages != NULL) {
            free(seq->pages);
        }
        seq->pages = (int *)malloc(seq->length * sizeof(int));
        memcpy(seq->pages, default_pages, seq->length * sizeof(int));
        
        printf("\n已加载默认测试序列！\n");
        display_sequence(seq);
    } else {
        printf("\n请输入页面访问序列的长度: ");
        scanf("%d", &seq->length);
        
        if (seq->length <= 0 || seq->length > MAX_PAGES) {
            printf("长度无效！请输入1-%d之间的值。\n", MAX_PAGES);
            return;
        }
        
        printf("请输入物理帧数量 (1-%d): ", MAX_FRAMES);
        scanf("%d", &seq->frame_count);
        
        if (seq->frame_count <= 0 || seq->frame_count > MAX_FRAMES) {
            printf("帧数无效！请输入1-%d之间的值。\n", MAX_FRAMES);
            return;
        }
        
        if (seq->pages != NULL) {
            free(seq->pages);
        }
        seq->pages = (int *)malloc(seq->length * sizeof(int));
        
        printf("请输入%d个页面号（用空格分隔）: ", seq->length);
        for (int i = 0; i < seq->length; i++) {
            scanf("%d", &seq->pages[i]);
        }
        
        printf("\n页面序列输入完成！\n");
    }
}

void display_sequence(PageSequence *seq) {
    printf("\n当前页面访问序列：\n");
    printf("物理帧数: %d\n", seq->frame_count);
    printf("序列长度: %d\n", seq->length);
    printf("页面序列: ");
    for (int i = 0; i < seq->length; i++) {
        printf("%d ", seq->pages[i]);
    }
    printf("\n");
}

bool is_in_frame(int *frames, int frame_count, int page) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

// FIFO算法实现
Statistics fifo_algorithm(PageSequence *seq) {
    Statistics stats = {0, 0, 0.0, 0.0};
    int frames[MAX_FRAMES];
    int queue_pointer = 0;  // 指向最早进入的页面
    
    // 初始化帧数组
    for (int i = 0; i < seq->frame_count; i++) {
        frames[i] = -1;
    }
    
    printf("\n--- FIFO算法执行过程 ---\n");
    printf("访问页面 | 物理帧状态          | 结果\n");
    printf("---------|---------------------|--------\n");
    
    for (int i = 0; i < seq->length; i++) {
        int page = seq->pages[i];
        bool hit = false;
        
        // 检查页面是否已在帧中
        if (is_in_frame(frames, seq->frame_count, page)) {
            stats.page_hits++;
            hit = true;
        } else {
            // 缺页
            stats.page_faults++;
            frames[queue_pointer] = page;
            queue_pointer = (queue_pointer + 1) % seq->frame_count;
        }
        
        // 打印当前状态
        printf("   %2d    | ", page);
        for (int j = 0; j < seq->frame_count; j++) {
            if (frames[j] != -1) {
                printf("%2d ", frames[j]);
            } else {
                printf("-- ");
            }
        }
        printf("| %s\n", hit ? "命中" : "缺页");
    }
    
    stats.fault_rate = (double)stats.page_faults / seq->length * 100;
    stats.hit_rate = (double)stats.page_hits / seq->length * 100;
    
    return stats;
}

// LRU算法实现
Statistics lru_algorithm(PageSequence *seq) {
    Statistics stats = {0, 0, 0.0, 0.0};
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];  // 记录每个帧最后使用的时间
    
    // 初始化
    for (int i = 0; i < seq->frame_count; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }
    
    printf("\n--- LRU算法执行过程 ---\n");
    printf("访问页面 | 物理帧状态          | 结果\n");
    printf("---------|---------------------|--------\n");
    
    for (int i = 0; i < seq->length; i++) {
        int page = seq->pages[i];
        bool hit = false;
        int victim_index = -1;
        
        // 检查页面是否已在帧中
        for (int j = 0; j < seq->frame_count; j++) {
            if (frames[j] == page) {
                stats.page_hits++;
                last_used[j] = i;
                hit = true;
                break;
            }
        }
        
        if (!hit) {
            // 缺页，需要找到替换位置
            stats.page_faults++;
            
            // 找一个空帧或最久未使用的帧
            int min_time = i;
            for (int j = 0; j < seq->frame_count; j++) {
                if (frames[j] == -1) {
                    victim_index = j;
                    break;
                }
                if (last_used[j] < min_time) {
                    min_time = last_used[j];
                    victim_index = j;
                }
            }
            
            frames[victim_index] = page;
            last_used[victim_index] = i;
        }
        
        // 打印当前状态
        printf("   %2d    | ", page);
        for (int j = 0; j < seq->frame_count; j++) {
            if (frames[j] != -1) {
                printf("%2d ", frames[j]);
            } else {
                printf("-- ");
            }
        }
        printf("| %s\n", hit ? "命中" : "缺页");
    }
    
    stats.fault_rate = (double)stats.page_faults / seq->length * 100;
    stats.hit_rate = (double)stats.page_hits / seq->length * 100;
    
    return stats;
}

// 查找最优替换页面（未来最长时间不使用的页面）
int find_optimal_victim(PageSequence *seq, int *frames, int frame_count, int current_pos) {
    int victim_index = 0;
    int farthest = current_pos;
    
    for (int i = 0; i < frame_count; i++) {
        int next_use = seq->length;  // 如果未来不再使用，设为最大值
        
        // 查找该页面下次使用的位置
        for (int j = current_pos + 1; j < seq->length; j++) {
            if (seq->pages[j] == frames[i]) {
                next_use = j;
                break;
            }
        }
        
        // 选择未来最晚使用的页面
        if (next_use > farthest) {
            farthest = next_use;
            victim_index = i;
        }
    }
    
    return victim_index;
}

// OPT算法实现
Statistics opt_algorithm(PageSequence *seq) {
    Statistics stats = {0, 0, 0.0, 0.0};
    int frames[MAX_FRAMES];
    
    // 初始化
    for (int i = 0; i < seq->frame_count; i++) {
        frames[i] = -1;
    }
    
    printf("\n--- OPT算法执行过程 ---\n");
    printf("访问页面 | 物理帧状态          | 结果\n");
    printf("---------|---------------------|--------\n");
    
    for (int i = 0; i < seq->length; i++) {
        int page = seq->pages[i];
        bool hit = false;
        
        // 检查页面是否已在帧中
        if (is_in_frame(frames, seq->frame_count, page)) {
            stats.page_hits++;
            hit = true;
        } else {
            // 缺页
            stats.page_faults++;
            
            // 查找空帧
            int empty_index = -1;
            for (int j = 0; j < seq->frame_count; j++) {
                if (frames[j] == -1) {
                    empty_index = j;
                    break;
                }
            }
            
            if (empty_index != -1) {
                // 有空帧，直接放入
                frames[empty_index] = page;
            } else {
                // 没有空帧，找最优替换页面
                int victim_index = find_optimal_victim(seq, frames, seq->frame_count, i);
                frames[victim_index] = page;
            }
        }
        
        // 打印当前状态
        printf("   %2d    | ", page);
        for (int j = 0; j < seq->frame_count; j++) {
            if (frames[j] != -1) {
                printf("%2d ", frames[j]);
            } else {
                printf("-- ");
            }
        }
        printf("| %s\n", hit ? "命中" : "缺页");
    }
    
    stats.fault_rate = (double)stats.page_faults / seq->length * 100;
    stats.hit_rate = (double)stats.page_hits / seq->length * 100;
    
    return stats;
}

void print_statistics(const char *algorithm_name, Statistics stats, int total) {
    printf("\n---------- %s 统计结果 ----------\n", algorithm_name);
    printf("总访问次数: %d\n", total);
    printf("缺页次数:   %d\n", stats.page_faults);
    printf("命中次数:   %d\n", stats.page_hits);
    printf("缺页率:     %.2f%%\n", stats.fault_rate);
    printf("命中率:     %.2f%%\n", stats.hit_rate);
    printf("----------------------------------------------\n");
}