// Level4.h: interface for the CLevel4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL4_H__F9C8B0BA_F8DA_43D0_A3D9_8FAF498281F2__INCLUDED_)
#define AFX_LEVEL4_H__F9C8B0BA_F8DA_43D0_A3D9_8FAF498281F2__INCLUDED_

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
#include "Level.h"

class CLevel4 : public CLevel 
{
public:
	void SetScore();
	CLevel4();
	virtual ~CLevel4();

};

#endif // !defined(AFX_LEVEL4_H__F9C8B0BA_F8DA_43D0_A3D9_8FAF498281F2__INCLUDED_)
