#include<cstdio>
#include <iostream>  
#include<cstring>
#include<queue>
#include<vector>
#include <Windows.h>
#include<algorithm>
using namespace std;

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
using namespace cv;

#include "Game.h"

int CALLBACK WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow
)
{
    CGame m_game;
    m_game.Init();
    m_game.GameRun();
    printf("end\n");
    cvWaitKey(0);
    return 0;
}