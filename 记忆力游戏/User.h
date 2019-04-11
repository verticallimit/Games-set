// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__99A0DAE3_FABF_43A8_9E1D_1751F28A152B__INCLUDED_)
#define AFX_USER_H__99A0DAE3_FABF_43A8_9E1D_1751F28A152B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include<cstdio>
#include<cstring>
#include<queue>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;


class CUser  
{
private:
	string username;
	string password;
	int score;
	int level;

public:
	int GetLevel();
	int GetScore();
	string GetPassword();
	string GetUsername();
	void SetLevel(int num);
	void SetScore(int num);
	void SetPassword(string pword);
	void SetUsername(string name);
	CUser();
	virtual ~CUser();

};

#endif // !defined(AFX_USER_H__99A0DAE3_FABF_43A8_9E1D_1751F28A152B__INCLUDED_)
