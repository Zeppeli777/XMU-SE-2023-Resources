/*
简单 UDP 客户端示例（与 server.c 配合使用）
从标准输入读取一行，发送到服务器，等待并打印服务器返回的回显
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define N 64 // 缓冲区大小（字节）

int main(int argc, char *argv[]) // ./client ip port 
{
    int sockfd; // UDP socket 描述符 
    struct sockaddr_in servaddr; // 服务器地址 
    char buf[N] = {0}; // 发送/接收缓冲区 

    // 检查命令行参数，需要服务器 IP 和端口 
    if (argc < 3)
    {
        printf("usage:%s ip port\n", argv[0]);
        return 0;
    }

    // 创建 UDP 套接字 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 填充服务器地址 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // 将端口字符串转换为整数，然后转为网络字节序 
    servaddr.sin_port = htons(atoi(argv[2])); // "6000" -> 6000 -> htons(6000) 
    // 将点分十进制的 IP 字符串转换为 in_addr_t 
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    // 主循环：从 stdin 读取一行并发送给服务器，然后等待回显 
    while (1)
    {
        printf(">");  
        // 从标准输入读取一行（包含换行符），最多读取 N-1 字节 
        fgets(buf, N, stdin);

        // 发送到服务器，包含字符串终止符（+1），以便服务器端按字符串处理 
        sendto(sockfd, buf, strlen(buf)+1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr));

        // 清空缓冲区并接收服务器返回的数据（阻塞） 
        memset(buf, 0, sizeof(buf));
        recvfrom(sockfd, buf, N, 0, NULL, NULL);
        // 打印接收到的回显（服务器应该返回与发送相同的内容） 
        printf("%s\n", buf);
    }

    // 关闭 socket（通常不会到达这里） 
    close(sockfd);

    return 0;
}
