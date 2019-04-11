
#include "PublicAPI.h"
#include "Tcp.h"
#include "Engine.h"

struct UserNode
{
	char username[105];
	char password[105];
}user[105];
int user_num = 0;

void Init()
{
	//读取user.txt数据
	FILE *fp;
	fp = fopen("./user.txt", "r");
	while(fscanf(fp, "%s %s", user[user_num].username, user[user_num].password) != EOF)
	{
		user_num++;
	}
	fclose(fp);
}

void Add_User(char *username, char *password)
{
	strcpy(user[user_num].username, username);
	strcpy(user[user_num].password, password);
	user_num++;
	FILE *fp;
    fp = fopen("./user.txt", "w");
    int k;
    for(k = 0; k < user_num; k++)
    {
        fprintf(fp, "%s %s\n", user[k].username, user[k].password);
    }
    fflush(fp);
    fclose(fp);
}

int Check_User(char *username, char *password)
{
	int flag = 0;
	int k;
	for(k = 0; k < user_num; k ++)
    {
        if((strcmp(username, user[k].username) == 0) && (strcmp(password, user[k].password) == 0))
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main()
{
	//初始化服务端数据
	Init();

	//创建TCP通信线程
	pthread_t tcp_thread;
	int ret = pthread_create(&tcp_thread, NULL, (void *)Tcp_Connect, NULL);
	if(ret != 0)//创建线程失败
	{
		//写入日志
		//Daemon_Log("Tcp_Connect Thread create failed!");
		
		exit(1);//退出程序、排查原因
	}

	//游戏运行引擎
	pthread_t game_thread;
	ret = pthread_create(&game_thread, NULL, (void *)GameRunning, NULL);
	if(ret != 0)//创建线程失败
	{
		//写入日志
		//Daemon_Log("GameRunning Thread create failed!");
		
		exit(1);//退出程序、排查原因
	}

	while(1)
	{
		printf("running....\n");
		sleep(5);
	}

	return 0;	
}