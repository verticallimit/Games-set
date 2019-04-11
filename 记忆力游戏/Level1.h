// Level1.h: interface for the CLevel1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL1_H__6D62965A_6A74_4452_9748_94AA632D857B__INCLUDED_)
#define AFX_LEVEL1_H__6D62965A_6A74_4452_9748_94AA632D857B__INCLUDED_

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

class CLevel1 : public CLevel 
{
private:
public:
	void SetScore();
	CLevel1();
	virtual ~CLevel1();

};

#endif // !defined(AFX_LEVEL1_H__6D62965A_6A74_4452_9748_94AA632D857B__INCLUDED_)
