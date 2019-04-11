// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "User.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{

}

CUser::~CUser()
{

}

void CUser::SetUsername(string name)
{
	username=name;
}

void CUser::SetPassword(string pword)
{
	password=pword;
}

void CUser::SetScore(int num)
{
	score=num;
}

void CUser::SetLevel(int num)
{
	level=num;
}

string CUser::GetUsername()
{
	return username;
}

string CUser::GetPassword()
{
	return password;
}

int CUser::GetScore()
{
	return score;
}

int CUser::GetLevel()
{
	return level;
}
