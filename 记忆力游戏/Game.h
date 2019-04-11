// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__8AD49AC1_B30E_4CE8_BBC8_B8C5B0E90164__INCLUDED_)
#define AFX_GAME_H__8AD49AC1_B30E_4CE8_BBC8_B8C5B0E90164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include<cstdio>
#include<cstring>
#include<iostream>
#include<queue>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
using namespace std;

#include "cv.h"
#include "highgui.h"
#include "User.h"
#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"

class CGame  
{
private:
	int find;//查找用户是否存在
	int score;
	vector<CUser *>userlist;
	char username[105];
	char password[105];
	char impassword[105];
	IplImage *img_p1;
	IplImage *img_p2;
	IplImage *img_p3;
	IplImage *img_p4;
	IplImage *img_p5;
	IplImage *img_p6;
	IplImage *img_p7;
	IplImage *img_back;
	IplImage *img_gameintro;

	IplImage *img_gameload;
	IplImage *img_background;
	IplImage *img_gameover;
	IplImage *img_block;
	IplImage *img_register;
	IplImage *img_temp_register;
	IplImage *img_login;
	IplImage *img_temp_login;
	IplImage *img_userinfo;
	IplImage *img_notfind;

public:
	void GameIntro();
	void Change(int x,char *name2);
	int FindUser(string username,string password);
	void Choose();
	void Register();
	void LoadUserInfo();
	void GameRun();
	void GameOver();
	void Init();
	CGame();
	virtual ~CGame();

};

#endif // !defined(AFX_GAME_H__8AD49AC1_B30E_4CE8_BBC8_B8C5B0E90164__INCLUDED_)
