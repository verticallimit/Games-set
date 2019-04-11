#include "PublicAPI.h"
#include "Map.h"
//固定地图框架

//地图1

void Build_Map(int mpt[13][18])
{
	memset(mpt, 0, sizeof(mpt));
	//随机40个障碍物
	int num = 30;
	while(num > 0)
	{
		int x = rand()%13;
		int y = rand()%18;
		if(mpt[x][y] == 0)
		{
			mpt[x][y] = 1;
			num --;
		}
	}
	//随机100个箱子
	num = 60;
	while(num > 0)
	{
		int x = rand()%13;
		int y = rand()%18;
		if(mpt[x][y] == 0)
		{
			mpt[x][y] = 3;
			num --;
		}
	}
	/*
	//随机10个泡泡数量道具
	num = 10;
	while(num > 0)
	{
		int x = rand()%20;
		int y = rand()%20;
		if(mpt[x][y] == 0)
		{
			mpt[x][y] = 4;
			num --;
		}
	}
	//随机10个泡泡威力道具
	num = 10;
	while(num > 0)
	{
		int x = rand()%20;
		int y = rand()%20;
		if(mpt[x][y] == 0)
		{
			mpt[x][y] = 5;
			num --;
		}
	}
	//随机5个人物速度道具
	num = 5;
	while(num > 0)
	{
		int x = rand()%20;
		int y = rand()%20;
		if(mpt[x][y] == 0)
		{
			mpt[x][y] = 6;
			num --;
		}
	}
	*/
}
