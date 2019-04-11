

#include "Map.h"

#include "Tcp.h"

#include "PublicAPI.h"

#include "Engine.h"
#include "time.h"

//随机地图模块


int mpt[13][18] = {0};
int force[13][18] = {0};
int boom[13][18] = {0};//泡泡爆炸时间
int dp[13][18] = {0};

int weili[8] = {0};//泡泡爆炸的威力

struct Person
{
	int x;
	int y;
}person[10];
int per_id = 0;

extern int gamerun[8];

extern int operate[8];

extern int die[8];

extern int status[8];

extern char chartmsg[1024];

extern int col[8];//颜色

int dir[4][2] = {0, 1, 1, 0, 0, -1, -1, 0};

void dfs(int x, int y, int p)
{
	if(x < 0 || x >= 13) return;
	if(y < 0 || y >= 18) return;
	if(dp[x][y] != 0) return;
	mpt[x][y] = 7;//泡泡爆炸
	boom[x][y] = 1;
	dp[x][y] = 1;
	//边界判断
	int i, j;
	//4个方向
	for(i = 0; i < 4; i++)
	{
		//weili[i]
		for(j = 1; j <= p; j++)
		{
			int nx = x + dir[i][0] * j;
			int ny = y + dir[i][1] * j;
			if(mpt[nx][ny] == 1) break;
			else if(mpt[nx][ny] == 3)
			{
				dp[nx][ny] = 1;
				int key = rand()%3;
				if(key == 0) mpt[nx][ny] = 4;
				else if(key == 1) mpt[nx][ny] = 5;
				else mpt[nx][ny] = 6;
				break;
			}
			else if(mpt[nx][ny] == 9) dfs(nx, ny, p);
			else if(dp[nx][ny] == 0)
			{
				dp[nx][ny] = 1;
				mpt[nx][ny] = 7;
				boom[nx][ny] = 1;
			}
		}
	}
}


void GameRunning()
{
	char msg[4096] = {0};
	int i, j;
	int build_flag = 0;
	srand(time(NULL));
	for(i = 0; i < 8; i++)
		weili[i] = 1;
	while(1)
	{
		//判断大厅是否都准备开始
		int w_flag = 0;
		int ss = 0;
		for(i = 0; i < 8; i++)
		{
			if(gamerun[i] == 1 && (status[i] == 1 || status[i] == 3))
			{
				w_flag = 1;
			}
			if(gamerun[i] == 1)
			{
				//die[i] = 1;
				ss++;
			}
		}

		//返回status
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "status");
		for(i = 0; i < 8; i++)
		{
			char temp[15] = {0};
			sprintf(temp, "%d", status[i]);
			strcat(msg, temp);
		}
		SendMsg(msg);

		printf("stausmsg:%s\n", msg);

		if(w_flag == 1 || ss == 0)
		{
			//发送聊天消息
			memset(msg, 0, sizeof(msg));
			strcpy(msg, "chart");
			strcat(msg, chartmsg);
			SendMsg(msg);
			usleep(500000);
			continue;
		}
		else
		{
			//发送游戏开始消息
			memset(msg, 0, sizeof(msg));
			strcpy(msg, "gamestart");
			SendMsg(msg);

			printf("startmsg:%s\n", msg);
		}

		//正式开始
		while(1)
		{
			/*
			int sg = 0;
			for(i = 0; i < 8; i++)
			{
				if(die[i] == 1) sg++;
			}
			if(sg == 0) break;//游戏结束
			*/
			int sum = 0;
			for(i = 0; i < 8; i++)
			{
				if(gamerun[i] == 0)//退出游戏
				{
					continue;
				}
				sum++;

				if(gamerun[i] == 1) 
				{
					gamerun[i] = 2;

					if(build_flag == 0)//第一个进入游戏
					{
						Build_Map(mpt);

						build_flag = 1;
					}

					//创建人物坐标
					int num = 1;
					int x, y;
					while(num > 0)
					{

						x = rand()%13;

						y = rand()%18;

						if(mpt[x][y] == 0)
						{
							//创建的时候根据大厅选择的颜色
							//2是红色
							//12是蓝色
							if(col[i] == 1)
								mpt[x][y] = 2;
							else if(col[i] == 2)
								mpt[x][y] = 12;
							num --;

						}

					}
					person[i].x = x;
					person[i].y = y;
				}

			}
			if(sum == 0)//没有人进入游戏
			{
				build_flag = 0;//地图重置
				memset(mpt, 0, sizeof(mpt));
				memset(boom, 0, sizeof(boom));
				memset(die, 0, sizeof(die));
				sleep(1);
				continue;
			}

			//游戏运行
			for(i = 0; i < 8; i++)
			{

				if(gamerun[i] >= 1 && operate[i] == 1)//space
				{

					mpt[person[i].x][person[i].y] = 9;
					boom[person[i].x][person[i].y] = 8;//3秒
					force[person[i].x][person[i].y] = weili[i];
				}

				else if(gamerun[i] >= 1 && operate[i] == 4 && mpt[person[i].x][person[i].y - 1] != 1 && mpt[person[i].x][person[i].y - 1] != 3 && mpt[person[i].x][person[i].y - 1] != 9)//up
				{

					if(mpt[person[i].x][person[i].y] == 2 || mpt[person[i].x][person[i].y] == 12)

						mpt[person[i].x][person[i].y] = 0;

					if(person[i].y - 1 >= 0)
					{
						//泡泡威力增加
						if(mpt[person[i].x][person[i].y - 1] == 5)
						{
							if(weili[i] < 6) weili[i]++;
						}
						if(col[i] == 1)
							mpt[person[i].x][person[i].y - 1] = 2;
						else
							mpt[person[i].x][person[i].y - 1] = 12;
						person[i].y = person[i].y - 1;
					}

				}

				else if(gamerun[i] >= 1 && operate[i] == 5 && mpt[person[i].x][person[i].y + 1] != 1 && mpt[person[i].x][person[i].y + 1] != 3 && mpt[person[i].x][person[i].y + 1] != 9)//down
				{

					if(mpt[person[i].x][person[i].y] == 2 || mpt[person[i].x][person[i].y] == 12)

						mpt[person[i].x][person[i].y] = 0;

					if(person[i].y + 1 < 18)
					{
						//泡泡威力增加
						if(mpt[person[i].x][person[i].y + 1] == 5)
						{
							if(weili[i] < 6) weili[i]++;
						}
						if(col[i] == 1)
							mpt[person[i].x][person[i].y + 1] = 2;
						else
							mpt[person[i].x][person[i].y + 1] = 12;
						person[i].y = person[i].y + 1;
					}

				}

				else if(gamerun[i] >= 1 && operate[i] == 2 && mpt[person[i].x - 1][person[i].y] != 1 && mpt[person[i].x - 1][person[i].y] != 3 && mpt[person[i].x - 1][person[i].y] != 9)//left
				{

					if(mpt[person[i].x][person[i].y] == 2 || mpt[person[i].x][person[i].y] == 12)

						mpt[person[i].x][person[i].y] = 0;

					if(person[i].x - 1 >= 0)
					{
						//泡泡威力增加
						if(mpt[person[i].x - 1][person[i].y] == 5)
						{
							if(weili[i] < 6) weili[i]++;
						}
						if(col[i] == 1)
							mpt[person[i].x - 1][person[i].y] = 2;
						else
							mpt[person[i].x - 1][person[i].y] = 12;
						person[i].x = person[i].x - 1;
					}

				}

				else if(gamerun[i] >= 1 && operate[i] == 3 && mpt[person[i].x + 1][person[i].y] != 1 && mpt[person[i].x + 1][person[i].y] != 3 && mpt[person[i].x + 1][person[i].y] != 9)//right
				{

					if(mpt[person[i].x][person[i].y] == 2 || mpt[person[i].x][person[i].y] == 12)

						mpt[person[i].x][person[i].y] = 0;

					if(person[i].x + 1 < 13)
					{
						//泡泡威力增加
						if(mpt[person[i].x + 1][person[i].y] == 5)
						{
							if(weili[i] < 6) weili[i]++;
						}
						if(col[i] == 1)
							mpt[person[i].x + 1][person[i].y] = 2;
						else
							mpt[person[i].x + 1][person[i].y] = 12;
						person[i].x = person[i].x + 1;
					}

				}

			}

			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					//判断自身
					if(boom[i][j] == 1)
					{
						boom[i][j] = 0;
						mpt[i][j] = 0;
					}
				}
			}
			memset(dp, 0, sizeof(dp));
			//泡泡爆炸的运行结果
			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					//判断自身
					if(boom[i][j] == 2)
					{
						dfs(i, j, force[i][j]);
						force[i][j] = 1;
					}
				}
			}
			int k;
			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					for(k = 0; k < 8; k++)
					if(boom[i][j] == 1 && person[k].x == i && person[k].y == j)
					{
						mpt[i][j] = 8;
						die[k] = 1;//玩家k死亡
					}
				}
			}
			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					//判断自身
					if(boom[i][j] > 2)
					{
						boom[i][j]--;
						//mpt[i][j] = 0;
					}
				}
			}

			memset(operate, 0, sizeof(operate));

			//生成地图状态字符串
			memset(msg, 0, sizeof(msg));

			sprintf(msg, "map");

			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					char temp[105] = {0};
					sprintf(temp,"+%d", mpt[i][j]);
					strcat(msg,temp);
				}
			}

			SendMsg(msg);

			for(i = 0; i < 13; i++)
			{
				for(j = 0; j < 18; j++)
				{
					printf(" %d", mpt[i][j]);
				}
				printf("\n");
			}

			
			//sleep(1);
			usleep(500000);
			IsGameOver();
		}
	}
}