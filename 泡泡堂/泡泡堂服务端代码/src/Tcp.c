#include "PublicAPI.h" 
#include "Tcp.h"

#define BACKLOG 5     //完成三次握手但没有accept的队列的长度  
#define CONCURRENT_MAX 8   //应用层同时可以处理的连接  
#define SERVER_PORT 6080  
#define BUFFER_SIZE 1024  
#define QUIT_CMD ".quit"  
int client_fds[CONCURRENT_MAX];

int gamerun[CONCURRENT_MAX] = {0};//游戏是否开始
int server_sock_fd = -1;
int operate[CONCURRENT_MAX] = {0};
int die[CONCURRENT_MAX] = {0};
int status[CONCURRENT_MAX] = {0};

int col[CONCURRENT_MAX] = {0};

char chartmsg[1024] = {0};

char IPSTR[105] = {0};

void SendMsg(char *msg)
{
    int i;
    for(i = 0; i < CONCURRENT_MAX; i++)  
    {
        if(client_fds[i] != 0)  
        {  
            printf("client_fds[%d]=%d\n", i, client_fds[i]);  
            send(client_fds[i], msg, strlen(msg), 0);  
        }  
    }
    //通知玩家死亡
    for(i = 0; i < CONCURRENT_MAX; i++)
    {
        if(die[i] == 1)
        {
            //gamelose
            send(client_fds[i], "gamelose", 8, 0);
            die[i] = 0;
            col[i] = 0;
            //gameover[i] = 0;
        }
    }
}

void IsGameOver()
{
    int team_1 = 0;
    int team_2 = 0;
    //判断队伍颜色数量
    int i;
    for(i = 0; i < CONCURRENT_MAX; i++)
    {
        if(die[i] == 1 || client_fds[i] == 0) continue;
        if(col[i] == 1) team_1 = 1;
        if(col[i] == 2) team_2 = 1;
    }
    //只剩下一个队伍
    if(team_1 + team_2 == 1)
    {
        for(i = 0; i < CONCURRENT_MAX; i++)  
        {
            if(client_fds[i] != 0 && die[i] == 0)  
            {  
                send(client_fds[i], "gamewin", 7, 0);  
            }  
        }
    }
}

int Tcp_Connect()
{  
    //读取ip.txt数据
    FILE *fp;
    fp = fopen("./ip.txt", "r");
    while(fscanf(fp, "%s", IPSTR) != EOF);
    fclose(fp);

    char input_msg[BUFFER_SIZE];  
    char recv_msg[BUFFER_SIZE];  
    //本地地址  
    struct sockaddr_in server_addr;  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(SERVER_PORT);  
    server_addr.sin_addr.s_addr = inet_addr(IPSTR);  
    bzero(&(server_addr.sin_zero), 8);  
    //创建socket  
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if(server_sock_fd == -1)
    {
        perror("socket error");
        return 1;
    }
    //绑定socket  
    int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    if(bind_result == -1)  
    {
        perror("bind error");
        return 1;
    }
    //listen  
    if(listen(server_sock_fd, BACKLOG) == -1)
    {
        perror("listen error");  
        return 1;  
    }  
    //fd_set  
    fd_set server_fd_set;  
    int max_fd = -1;  
    struct timeval tv;  //超时时间设置  
    while(1)  
    {  
        tv.tv_sec = 1;  
        tv.tv_usec = 0;  
        FD_ZERO(&server_fd_set);  
        FD_SET(STDIN_FILENO, &server_fd_set);  
        if(max_fd <STDIN_FILENO)  
        {  
            max_fd = STDIN_FILENO;  
        }  
        //printf("STDIN_FILENO=%d\n", STDIN_FILENO);  
        //服务器端socket  
        FD_SET(server_sock_fd, &server_fd_set);  
       // printf("server_sock_fd=%d\n", server_sock_fd);  
        if(max_fd < server_sock_fd)  
        {  
            max_fd = server_sock_fd;  
        }  
        //客户端连接  
    	int i;
        for(i =0; i < CONCURRENT_MAX; i++)  
        {  
            //printf("client_fds[%d]=%d\n", i, client_fds[i]);  
            if(client_fds[i] != 0)  
            {  
                FD_SET(client_fds[i], &server_fd_set);  
                if(max_fd < client_fds[i])  
                {  
                    max_fd = client_fds[i];  
                }  
            }  
        }  
        int ret = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);  
        if(ret < 0)  
        {  
            perror("select 出错\n");  
            continue;  
        }  
        else if(ret == 0)  
        {  
            printf("select 超时\n");  
            continue;  
        }  
        else  
        {  
            //ret 为未状态发生变化的文件描述符的个数  
            if(FD_ISSET(STDIN_FILENO, &server_fd_set))  
            {  
                printf("发送消息：\n");  
                bzero(input_msg, BUFFER_SIZE);  
                fgets(input_msg, BUFFER_SIZE, stdin);  
                //输入“.quit"则退出服务器  
                if(strcmp(input_msg, QUIT_CMD) == 0)  
                {  
                    exit(0);  
                }  
                for(i = 0; i < CONCURRENT_MAX; i++)  
                {
                    if(client_fds[i] != 0)  
                    {  
                        printf("client_fds[%d]=%d\n", i, client_fds[i]);  
                        send(client_fds[i], input_msg, BUFFER_SIZE, 0);  
                    }  
                }
            }
            if(FD_ISSET(server_sock_fd, &server_fd_set))  
            {  
                //有新的连接请求  
                struct sockaddr_in client_address;  
                socklen_t address_len;  
                int client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);  
                printf("new connection client_sock_fd = %d\n", client_sock_fd);  
                if(client_sock_fd > 0)  
                {  
                    int index = -1;  
                    for(i = 0; i < CONCURRENT_MAX; i++)  
                    {  
                        if(client_fds[i] == 0)  
                        {  
                            index = i;  
                            client_fds[i] = client_sock_fd;  
                            break;  
                        }  
                    }  
                    if(index >= 0)  
                    {  
                        gamerun[index] = 0;
                        printf("新客户端(%d)加入成功 %s:%d\n", index, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));  
                    }  
                    else  
                    {  
                        bzero(input_msg, BUFFER_SIZE);  
                        strcpy(input_msg, "服务器加入的客户端数达到最大值,无法加入!\n");  
                        send(client_sock_fd, input_msg, BUFFER_SIZE, 0);  
                        printf("客户端连接数达到最大值，新客户端加入失败 %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));  
                    }  
                }  
            }  
            for(i =0; i < CONCURRENT_MAX; i++)  
            {  
                if(client_fds[i] !=0)  
                {  
                    if(FD_ISSET(client_fds[i], &server_fd_set))  
                    {  
                        //处理某个客户端过来的消息  
                        bzero(recv_msg, BUFFER_SIZE);  
                        memset(recv_msg, 0, sizeof(recv_msg));
                        long byte_num = recv(client_fds[i], recv_msg, BUFFER_SIZE, 0);  
                        if (byte_num > 0)
                        {
                            if(byte_num > BUFFER_SIZE)
                            {
                                byte_num = BUFFER_SIZE;
                            }
                            recv_msg[byte_num] = '\0';
                            operate[i] = 0;
                            printf("客户端(%d):%s\n", i, recv_msg);
                            //操作
                            if(strstr(recv_msg, "space") != NULL)
                            {
                                operate[i] = 1;
                            }
                            else if(strstr(recv_msg, "up") != NULL)
                            {
                                operate[i] = 2;
                            }
                            else if(strstr(recv_msg, "down") != NULL)
                            {
                                operate[i] = 3;
                            }
                            else if(strstr(recv_msg, "left") != NULL)
                            {
                                operate[i] = 4;
                            }
                            else if(strstr(recv_msg, "right") != NULL)
                            {
                                operate[i] = 5;
                            }
                            //0表示没有人
                            //1表示红色未准备
                            //2表示红色准备
                            //3表示蓝色未准备
                            //4表示蓝色准备
                            else if(strstr(recv_msg, "red") != NULL)
                            {
                                col[i] = 1;
                                if(status[i] == 1) status[i] = 1;
                                else if(status[i] == 2) status[i] = 2;
                                else if(status[i] == 3) status[i] = 1;
                                else if(status[i] == 4) status[i] = 2;
                            }
                            else if(strstr(recv_msg, "blue") != NULL)
                            {
                                col[i] = 2;
                                if(status[i] == 1) status[i] = 3;
                                else if(status[i] == 2) status[i] = 4;
                                else if(status[i] == 3) status[i] = 3;
                                else if(status[i] == 4) status[i] = 4;
                            }
                            else if(strstr(recv_msg, "ready") != NULL)
                            {
                                if(status[i] == 1) status[i] = 2;
                                else if(status[i] == 2) status[i] = 2;
                                else if(status[i] == 3) status[i] = 4;
                                else if(status[i] == 4) status[i] = 4;
                            }
                            else if(strstr(recv_msg, "cancel") != NULL)
                            {
                                if(status[i] == 1) status[i] = 1;
                                else if(status[i] == 2) status[i] = 1;
                                else if(status[i] == 3) status[i] = 3;
                                else if(status[i] == 4) status[i] = 3;
                            }
                            //聊天消息
                            else if(strstr(recv_msg, "gamemenu") != NULL)
                            {
                                memset(chartmsg, 0, sizeof(chartmsg));
                                int k = 0;
                                for(k = 9; k < strlen(recv_msg); k++)
                                {
                                    chartmsg[k - 9] = recv_msg[k];
                                }
                            }
                            //注册
                            else if(strstr(recv_msg, "register") != NULL)
                            {
                                int k, j = 0;
                                int split = 0;
                                char username[105] = {0};
                                char password[105] = {0};
                                for(k = 9; k < strlen(recv_msg); k++)
                                {
                                    if(recv_msg[k] != '+' && split == 0)
                                        username[j++] = recv_msg[k];
                                    else if(recv_msg[k] == '+')
                                    {
                                        split = 1;
                                        j = 0;
                                    }
                                    else
                                    {
                                        password[j++] = recv_msg[k];
                                    }
                                }
                                Add_User(username, password);
                                send(client_fds[i], "register+1", 10, 0);
                                
                            }
                            //登录
                            else if(strstr(recv_msg, "login") != NULL)
                            {
                                int k, j = 0;
                                int split = 0;
                                char username[105] = {0};
                                char password[105] = {0};
                                for(k = 6; k < strlen(recv_msg); k++)
                                {
                                    if(recv_msg[k] != '+' && split == 0)
                                        username[j++] = recv_msg[k];
                                    else if(recv_msg[k] == '+')
                                    {
                                        split = 1;
                                        j = 0;
                                    }
                                    else
                                    {
                                        password[j++] = recv_msg[k];
                                    }
                                }
                                printf("username:%s password:%s\n", username, password);
                                int flag = 0;
                                
                                flag = Check_User(username, password);
                                if(flag == 1)//登录成功
                                {
                                    send(client_fds[i], "login+1", 7, 0);
                                    if(gamerun[i] == 0) gamerun[i] = 1;
                                    //else if(gamerun >= 1) gamerun++;
                                    status[i] = 1;
                                    col[i] = 1;
                                }
                                else 
                                    send(client_fds[i], "login+0", 7, 0);
                            }
                        }
                        else if(byte_num < 0)
                        {
                            printf("从客户端(%d)接受消息出错.\n", i);
                        }
                        else
                        {
                            FD_CLR(client_fds[i], &server_fd_set);
                            client_fds[i] = 0;
                            printf("客户端(%d)退出了\n", i);
                            gamerun[i] = 0;
                            operate[i] = 0;
                            die[i] = 0;
                            status[i] = 0;
                        }  
                    }  
                }
            }  
        }
    }  
    return 0;  
}  