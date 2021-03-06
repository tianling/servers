//
// Created by luoxinyu-xy on 2017/3/20.
//
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 1024

#define SERVER_STRING "Server: tianling-server\r\n"

int main(int argc, char** argv)
{
    /* 定义server和client的文件描述符 */
    int server_fd = -1;
    int client_fd = -1;

    u_short port = PORT;
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    char buf[BUFF_SIZE];
    char recv_buf[BUFF_SIZE];
    char hello_str[] = "Hello world!";

    int hello_len = 0;

    /*
     * 创建一个socket
     * AF_INET ipv4协议
     * SOCK_STREAM 字节流套接字
     * 传输层协议类型：0-TCP 1-UDP 2-SCTP
     * */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    /* 设置端口，IP，和TCP/IP协议族 */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* 绑定套接字到端口 */
    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    /* 启动socket监听请求，开始等待客户端发来的请求 */
    if (listen(server_fd, QUEUE_MAX_COUNT) < 0) {
        perror("listen");
        exit(-1);
    }

    printf("http server running on port %d\n", port);

    while (1)
    {
        /* 调用了accept函数，阻塞了程序，直到接收到客户端的请求 */
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                           &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            exit(-1);
        }
        printf("accept a client\n");

        printf("client socket fd: %d\n", client_fd);
        /* 调用recv函数接收客户端发来的请求信息 */
        hello_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);

        printf("receive %d\n", hello_len);//输出客户端发送数据长度
        printf("receive %s\n", recv_buf);//输出客户端发送数据内容
        printf("client port: %d \n",client_addr.sin_port);//输出客户端端口

        char* clientIp = inet_ntoa(client_addr.sin_addr);//输出客户端ip
        printf("client ip: %s \n",clientIp);

        /* 发送响应给客户端 */
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client_fd, buf, strlen(buf), 0);

        strcpy(buf, SERVER_STRING);
        send(client_fd, buf, strlen(buf), 0);

        sprintf(buf, "Content-Type: text/html\r\n");
        send(client_fd, buf, strlen(buf), 0);

        strcpy(buf, "\r\n");
        send(client_fd, buf, strlen(buf), 0);

        sprintf(buf, "Hello World\r\n");
        send(client_fd, buf, strlen(buf), 0);

        /* 关闭客户端套接字 */
        close(client_fd);


    }

    close(server_fd);

    return 0;

}