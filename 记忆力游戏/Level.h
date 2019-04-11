// Level.h: interface for the CLevel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL_H__05235C5F_8080_4FED_B151_398D98ABDC44__INCLUDED_)
#define AFX_LEVEL_H__05235C5F_8080_4FED_B151_398D98ABDC44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<cstdio>
#include<cstring>
#include<iostream>
#include<queue>
#include<string>
#include<vector>
#include<time.h>
#include<cstdlib>
#include<fstream>
#include<algorithm>
using namespace std;

#include "cv.h"
#include "highgui.h"

class CLevel  
{
protected:
	int add;
	int sub;
	int kind;//图片种类
	int number;//图片数量
	int level;
	int score;
	int vis[105][104];
	int mpt[105][104];
	IplImage *img_background;
	IplImage *img_block;
	IplImage *img_p1;
	IplImage *img_p2;
	IplImage *img_p3;
	IplImage *img_p4;
	IplImage *img_p5;
	IplImage *img_p6;
	IplImage *img_p7;
	IplImage *img_back;

public:
	
	void Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2);
	void Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4);
	void Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4,IplImage *p5,IplImage *p6);
	void Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4,IplImage *p5,IplImage *p6,IplImage *p7);
	int GetSocre();
	void Rand();
	void Judge();
	int width;
	int height;
	int push;
	int f_cx;
	int f_cy;
	int cx;
	int cy;
	void Click();
	void Drawblock();
	void Drawtoback();
	void Running();
	void Init();
	CLevel();
	virtual void SetScore();
	virtual ~CLevel();
};

#endif // !defined(AFX_LEVEL_H__05235C5F_8080_4FED_B151_398D98ABDC44__INCLUDED_)
