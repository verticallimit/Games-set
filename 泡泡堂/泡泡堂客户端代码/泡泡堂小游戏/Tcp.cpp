
#include "Tcp.h"
#include <iostream>  
#include <cstdio>  

CTcp::CTcp()
{
    gameover = 0;
    gamewin = 0;
    gamelose = 0;
    login = 0;
    reg = 0;
    start = 0;
    memset(chartmsg, '\0', sizeof(chartmsg));
}

CTcp::~CTcp()
{

}

int CTcp::Send_Tcp_Msg(char *msg)
{
    int len = strlen(msg);
    if (len == 0) return 0;

    printf("sendmsg:%s\n", msg);
    if (send(server_sock_fd, msg, strlen(msg), 0) == -1)
    {
        perror("发送消息出错!\n");
        return 1;
    }
    return 0;
}

int CTcp::Connect()
{
    char sendbuf[4096] = { 0 };
    char recvbuf[4096] = { 0 };
    printf("connnet...\n");
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }
    printf("1111\n");
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock_fd == INVALID_SOCKET)
    {
        printf("invalid socket !");
        return 0;
    }
    printf("2222\n");
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(6080);
    char ipstr[105] = { 0 };//192.168.59.151
    FILE *fp;
    fp = fopen(".\\img\\ip.txt", "r");
    while (fgets(ipstr, sizeof(ipstr), fp) != NULL);
    printf("ipstr:%s\n", ipstr);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ipstr);
    if (connect(server_sock_fd, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("connect error !:%d\n", GetLastError());
        closesocket(server_sock_fd);
        return 0;
    }
    printf("已连接上服务端...\n");

    fd_set server_fd_set;
    fd_set Wset, Rset;
    struct timeval tv;
    while (1)
    {
        printf("select...\n");
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO(&server_fd_set);
        FD_SET(0, &server_fd_set);
        FD_SET(server_sock_fd, &server_fd_set);

        select(server_sock_fd + 1, &server_fd_set, NULL, NULL, &tv);
        /*if (FD_ISSET(server_sock_fd, &server_fd_set))
        {
        printf("send\n");
        memset(sendbuf, 0, sizeof(sendbuf));
        fgets(sendbuf, strlen(sendbuf), stdin);
        if (send(server_sock_fd, sendbuf, strlen(sendbuf), 0) == -1)
        {
        perror("发送消息出错!\n");
        }
        }*/
        if (FD_ISSET(server_sock_fd, &server_fd_set))
        {
            printf("recv\n");
            memset(recvbuf, 0, sizeof(recvbuf));
            long byte_num = recv(server_sock_fd, recvbuf, sizeof(recvbuf), 0);
            if (byte_num > 0)
            {
                if (byte_num > sizeof(recvbuf))
                {
                    byte_num = sizeof(recvbuf) - 1;
                }
                recvbuf[byte_num] = '\0';
                printf("服务器:%s\n", recvbuf);
                //solve msg
                SolveMsg(recvbuf);
            }
            else if (byte_num < 0)
            {
                printf("接受消息出错!\n");
            }
            else
            {
                printf("服务器端退出!\n");
                exit(0);
            }
        }
    }

    closesocket(server_sock_fd);
    WSACleanup();
}

//map+  login+  register+   rank+   
void CTcp::SolveMsg(char *msg)
{
    if (strstr(msg, "register") != NULL)
    {
        if (msg[9] == '1')
        {
            reg = 1;
            printf("register success!\n");
        }
        else printf("register failed!\n");
    }
    if (strstr(msg, "login") != NULL)
    {
        if (msg[6] == '1')
        {
            login = 1;
            printf("login success!\n");
        }
        else printf("login failed!\n");
    }
    if (strstr(msg, "map") != NULL)
    {
        //重绘地图界面
        //map+0+0+0....   18*13
        //0表示空
        //1表示墙
        //2表示人物
        //3表示箱子
        //4表示泡泡数量道具
        //5表示泡泡威力道具
        //6表示人物速度道具
        //7表示泡泡爆炸
        //8表示人物被泡泡困住
        memset(mpt, 0, sizeof(mpt));
        int x = 0;
        int y = 0;
        int val = 0;
        for (int i = 4; i < strlen(msg); i++)
        {
            if (msg[i] == '+')
            {
                mpt[x][y] = val;
                y++;
                if (y >= 18)
                {
                    y = 0;
                    x++;
                }
                val = 0;
            }
            else
            {
                val = val * 10 + msg[i] - '0';
            }
        }
        mpt[x][y] = val;

        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                printf("%d ", mpt[i][j]);
            }
            printf("\n");
        }
    }
    if (strstr(msg, "gameover") != NULL)
    {
        gameover = 1;
    }
    if (strstr(msg, "gamewin") != NULL)
    {
        gamewin = 1;
    }
    if (strstr(msg, "gamelose") != NULL)
    {
        gamelose = 1;
    }
    if (strstr(msg, "gamestart") != NULL)
    {
        start = 1;
    }
    if (strstr(msg, "status") != NULL)
    {
        memset(status, 0, sizeof(status));
        for (int i = 6; i < strlen(msg) + 8; i++)
        {
            status[i - 6] = msg[i] - '0';
        }
    }
    if (strstr(msg, "chart") != NULL)
    {
        memset(chartmsg, 0, sizeof(chartmsg));
        for (int i = 5; i < strlen(msg); i++)
        {
            chartmsg[i - 5] = msg[i];
        }
    }
}

void CTcp::GetMap(int mp[25][25])
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            mp[i][j] = mpt[i][j];
        }
    }
}

int CTcp::IsGameOver()
{
    if (gamewin == 1) return 1;
    else if (gamelose == 1) return 2;
    else return 0;
}

int CTcp::IsStart()
{
    return start;
}

void CTcp::GetStatus(int st[8])
{
    for (int i = 0; i < 8; i++)
        st[i] = status[i];
}

void CTcp::GetMsg(char* st)
{
    if(strlen(chartmsg) > 0)
    strcpy(st, chartmsg);
}