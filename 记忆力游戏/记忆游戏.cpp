#include<cstdio>
#include<cstring>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;

#include "cv.h"
#include "highgui.h"

#include "Game.h"


//#include "mmsystem.h"      
//#pragma comment(lib,"winmm.lib")

int main()
{
	//≤•∑≈±≥æ∞“Ù¿÷
	//mciSendString("play E:\\Game.mp3",NULL,0,NULL);
	CGame m_game;
	m_game.Init();
	m_game.GameRun();
	return 0;
}