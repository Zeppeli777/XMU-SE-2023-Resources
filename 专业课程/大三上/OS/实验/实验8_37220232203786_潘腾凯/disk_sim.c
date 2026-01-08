/*
  disk_sim.c
  模拟 500 个块（每块 2KB）的磁盘，使用基于块的索引分配。
  生成 1.txt..50.txt 随机文件（2KB-10KB），删除奇数文件，
  新建 A...E 并输出分配表与空闲块状态。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TOTAL_BLOCKS 500
#define BLOCK_SIZE 2048 // 字节 

typedef struct {
    char name[32];
    int index_block; // 索引块号（1开始），-1表示无 
    int *data_blocks; // 数据块号数组
    int data_count;   // 数据块数量
    int size_bytes;
} FileEntry;

int bitmap[TOTAL_BLOCKS+1]; // 1...500，0表示空闲，1表示已用

void init_bitmap(){
    for(int i=1;i<=TOTAL_BLOCKS;i++) bitmap[i]=0;
}

/* 
  分配count个空闲块，存储到out[]数组（调用者保证空间足够）。
  成功返回0，空间不足返回-1。
  使用首次适配策略（选择最小的空闲块号）。
 */
int allocate_blocks(int count, int out[]){
    int found=0;
    for(int i=1;i<=TOTAL_BLOCKS && found<count;i++){
        if(bitmap[i]==0){
            bitmap[i]=1;
            out[found++]=i;
        }
    }
    if(found<count){
        // 空间不足，回滚已分配块
        for(int j=0;j<found;j++) bitmap[out[j]]=0;
        return -1;
    }
    return 0;
}

void free_block(int b){ if(b>=1 && b<=TOTAL_BLOCKS) bitmap[b]=0; }

void free_blocks(int arr[], int n){ for(int i=0;i<n;i++) free_block(arr[i]); }

// 使用索引分配创建文件：分配1个索引块 + 多个数据块 
int create_file(FileEntry *f){
    int need_data = (f->size_bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int need_total = 1 + need_data; // 1个索引块 + 数据块
    int *tmp = malloc(sizeof(int)*(need_total));
    if(!tmp) return -1;
    if(allocate_blocks(need_total, tmp)==-1){
        free(tmp);
        return -1;
    }
    f->index_block = tmp[0];
    f->data_count = need_data;
    f->data_blocks = malloc(sizeof(int)*need_data);
    for(int i=0;i<need_data;i++) f->data_blocks[i]=tmp[1+i];
    free(tmp);
    return 0;
}

void delete_file(FileEntry *f){
    if(f->index_block!=-1) free_block(f->index_block);
    if(f->data_blocks){
        free_blocks(f->data_blocks, f->data_count);
        free(f->data_blocks);
        f->data_blocks = NULL;
    }
    f->index_block = -1;
    f->data_count = 0;
}

void print_free_blocks(){
    printf("空闲块:\n");
    int cnt=0;
    for(int i=1;i<=TOTAL_BLOCKS;i++){
        if(bitmap[i]==0){
            printf("%d ", i);
            cnt++;
            if(cnt%20==0) printf("\n");
        }
    }
    if(cnt%20!=0) printf("\n");
    printf("空闲块总数: %d\n", cnt);
}

void print_file_table(FileEntry *f){
    if(f->index_block==-1){
        printf("%s: (未分配)\n", f->name);
        return;
    }
    printf("%s:\n", f->name);
    printf("  索引块: %d\n", f->index_block);
    printf("  数据块 (%d): ", f->data_count);
    for(int i=0;i<f->data_count;i++){
        printf("%d", f->data_blocks[i]);
        if(i+1<f->data_count) printf(",");
    }
    printf("\n");
}

int main(){
    init_bitmap();
    srand(12345); // 固定种子以便结果可复现

    // 1. 生成 1.txt..50.txt，大小 2KB-10KB
    int N = 50;
    FileEntry files[50];
    for(int i=0;i<N;i++){
        snprintf(files[i].name, sizeof(files[i].name), "%d.txt", i+1);
        files[i].index_block = -1;
        files[i].data_blocks = NULL;
        files[i].data_count = 0;
        int sz_kb = (rand() % (10 - 2 + 1)) + 2; /* 2-10 KB */
        files[i].size_bytes = sz_kb * 1024;
        if(create_file(&files[i])==-1){
            printf("分配失败：%s (大小 %d KB)\n", files[i].name, sz_kb);
        }
    }

    // 2. 删除奇数.txt（1,3,...,49）
    for(int i=0;i<N;i+=2){
        delete_file(&files[i]);
    }

    // 3. 新建 A...E 文件
    const char *names[] = {"A.txt","B.txt","C.txt","D.txt","E.txt"};
    int sizes_bytes[] = {7*1024, 5*1024, 2*1024, 9*1024, (int)(3.5*1024)}; // 3.5KB转为3584字节
    FileEntry newf[5];
    for(int i=0;i<5;i++){
        strncpy(newf[i].name, names[i], sizeof(newf[i].name));
        newf[i].index_block = -1;
        newf[i].data_blocks = NULL;
        newf[i].data_count = 0;
        newf[i].size_bytes = sizes_bytes[i];
        if(create_file(&newf[i])==-1){
            printf("分配失败：%s (大小 %d 字节)\n", newf[i].name, newf[i].size_bytes);
        }
    }

   
    printf("\n=== 文件分配表（A..E） ===\n");
    for(int i=0;i<5;i++) print_file_table(&newf[i]);


    printf("\n=== 空闲区块状态 ===\n");
    print_free_blocks();

    return 0;
}
