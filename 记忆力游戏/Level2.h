// Level2.h: interface for the CLevel2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL2_H__A2FE936C_9F34_429E_80D2_2329E3DEA329__INCLUDED_)
#define AFX_LEVEL2_H__A2FE936C_9F34_429E_80D2_2329E3DEA329__INCLUDED_

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

class CLevel2 : public CLevel 
{
public:
	void SetScore();
	CLevel2();
	virtual ~CLevel2();

};

#endif // !defined(AFX_LEVEL2_H__A2FE936C_9F34_429E_80D2_2329E3DEA329__INCLUDED_)
