#pragma once

#include <iostream>  
#include <cstdio>  
#include <cstring>
#include <Winsock2.h>
#include <algorithm>
using namespace std;

#pragma comment(lib, "ws2_32.lib") 


class CTcp
{
private:
    SOCKET server_sock_fd;
    int mpt[25][25];
    int gameover;//游戏是否结束标志
    int gamewin;
    int gamelose;
    int status[8];
    int start;//游戏开始标志
    char chartmsg[1024];//聊天消息

public:
    int login;//是否登录成功
    int reg;//是否注册成功
    int Connect();
    void SolveMsg(char *msg);
    int Send_Tcp_Msg(char *msg);
    void GetMap(int mp[25][25]);
    int IsGameOver();
    void GetStatus(int st[8]);
    int IsStart();
    void GetMsg(char* str);

    CTcp();
    virtual ~CTcp();
};
