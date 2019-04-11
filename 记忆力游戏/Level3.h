// Level3.h: interface for the CLevel3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVEL3_H__7A8B2D57_DD74_4802_9B29_57FBD4CCFD7F__INCLUDED_)
#define AFX_LEVEL3_H__7A8B2D57_DD74_4802_9B29_57FBD4CCFD7F__INCLUDED_

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

class CLevel3  : public CLevel 
{
public:
	void SetScore();
	CLevel3();
	virtual ~CLevel3();

};

#endif // !defined(AFX_LEVEL3_H__7A8B2D57_DD74_4802_9B29_57FBD4CCFD7F__INCLUDED_)
