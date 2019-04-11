#pragma once

#include<cstdio>
#include<cstring>
#include<iostream>
#include<queue>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;


#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

class CGame
{
private:
    char username[105];
    char password[105];
    char impassword[105];
    char sendmsg[205];
    int overflag;//游戏结束标志
    IplImage *img_page;
    IplImage *img_login;
    IplImage *img_temp_login;
    IplImage *img_register;
    IplImage *img_temp_register;
    IplImage *img_people_red;//人物
    IplImage *img_people_blue;//人物

    IplImage *img_boom;//爆炸效果
    IplImage *img_background;//游戏背景图
    IplImage *img_bubble;//泡泡
    IplImage *img_obstacle;//障碍物
    IplImage *img_box;
    IplImage *img_tool_bubble;
    IplImage *img_tool_shoes;
    IplImage *img_tool_magic;
    IplImage *img_gameover;
    IplImage *img_gamewin;
    IplImage *img_gamelose;
    IplImage *img_gameintro;
    IplImage *img_die;
    IplImage *img_gamemenu;
    IplImage *img_temp_gamemenu;

public:
    IplImage *img_people;
    IplImage *img_my_people;//人物
    void Init();
    void GameRun();
    void Login();
    void Register();
    void GameIntro();
    void ShowWindow();
    void SendMsg(char* msg);
    void Menu();

    CGame();
    virtual ~CGame();
};
