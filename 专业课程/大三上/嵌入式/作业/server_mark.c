/*
简单 UDP 回显服务器 (echo server)
接收来自客户端的 UDP 数据报，打印来源地址与内容，然后把原数据返回给发送者
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

int main(int argc, char *argv[]) // ./server ip port 
{
    int sockfd; // UDP socket 描述符 
    struct sockaddr_in myaddr, peeraddr; // 本地地址和对端地址结构
    char buf[N] = {0}; // 接收/发送缓冲区
    socklen_t len; // 地址结构大小（recvfrom/ sendto 使用）
    ssize_t n; // recvfrom 返回的字节数 
    // 简单的参数检查，要求提供 IP 和端口
    if (argc < 3)
    {
        printf("usage:%s ip port\n", argv[0]);
        return 0;
    }

    // 创建 UDP 套接字(AF_INET, SOCK_DGRAM) 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }

    // 填充本地地址结构并 bind 到指定的 ip:port
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    // argv[2] 是端口字符串，atoi 转为整数再 htons 转网络字节序 
    myaddr.sin_port = htons(atoi(argv[2])); // "6000" -> 6000 -> htons(6000)
    // argv[1] 是点分十进制 IP 地址，inet_addr 转为 in_addr_t（网络字节序）
    myaddr.sin_addr.s_addr = inet_addr(argv[1]);

    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
    {
        // 绑定失败通常是地址/端口不可用，打印错误并退出
        perror("bind");
        exit(-1);
    }

    // 初始化对端地址结构，len 用来接收/发送时传递地址大小
    memset(&peeraddr, 0, sizeof(peeraddr));
    len = sizeof(peeraddr);

    // 进入循环，持续接收来自任意客户端的消息并回显 
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        
        //recvfrom: 从 sockfd 上读取数据，保存发送方地址到 peeraddr，
        //len 指向的值会被设置为实际地址结构大小
        
        n = recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&peeraddr, &len);

        // 打印发送方 IP:port 及接收到的消息内容（假定消息为字符串）
        printf("from %s:%d %s\n", inet_ntoa(peeraddr.sin_addr),
                ntohs(peeraddr.sin_port), buf);

        // 将接收到的数据原样发回给发送者（回显）
        // 注意：sendto 第五个参数传入 peeraddr 的大小（sizeof(peeraddr)）
        sendto(sockfd, buf, n, 0, (struct sockaddr *)&peeraddr, sizeof(peeraddr));
    }

    // 虽然程序通常不会走到这里，但良好习惯是关闭 socket 
    close(sockfd);

    return 0;
}
