#include "Game.h"
#include "Tcp.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
using namespace cv;

CTcp m_tcp;

CGame::CGame()
{

}

CGame::~CGame()
{

}

//回调函数
void OnMouse(int event, int x, int y, int flags, void* param)
{
    CGame *game = (CGame*)param;
    if (event == CV_EVENT_LBUTTONDOWN)//鼠标左键点击事件
    {
        if (x > 242 && x < 343 && y > 556 && y < 588)//游戏注册
        {
            cvDestroyWindow("login");
            game->Register();
        }
        else if (x > 463 && x < 565 && y > 557 && y < 587)//退出游戏
        {
            exit(0);
        }
        else return;
    }
}

//回调函数2
void OnMouse_2(int event, int x, int y, int flags, void* param)
{
    CGame *game = (CGame*)param;
    if (event == CV_EVENT_LBUTTONDOWN)//鼠标左键点击事件
    {
        if (x > 494 && x < 530 && y > 306 - 25 && y < 343 - 25)//点击红色
        {
            game->img_my_people = cvLoadImage(".\\img\\people_red.png");
            //MessageBoxA(NULL, "红色", "发送消息", MB_OK);
            game->SendMsg("red");
        }
        else if (x > 638 && x < 670 && y > 307 - 25 && y < 342 - 25)//点击蓝色
        {
            game->img_my_people = cvLoadImage(".\\img\\people_blue.png");
            //MessageBoxA(NULL, "蓝色", "发送消息", MB_OK);
            game->SendMsg("blue");
        }
        else if (x > 518 && x < 709 && y > 520 - 25 && y < 574 - 25)//点击准备
        {
            //MessageBoxA(NULL, "准备", "发送消息", MB_OK);
            game->SendMsg("ready");
        }
        else if (x > 651 && x < 775 && y > 452 - 25 && y < 501 - 25)//点击取消准备
        {
            //MessageBoxA(NULL, "取消准备", "发送消息", MB_OK);
            game->SendMsg("cancel");
        }
        else if (x > 754 && x < 787 && y > 596 - 25 && y < 622 - 25)//exit
        {
            //MessageBoxA(NULL, "退出", "发送消息", MB_OK);
            exit(0);
        }
        else return;
    }
}

void CGame::SendMsg(char* msg)
{
    m_tcp.Send_Tcp_Msg(msg);
}

void CGame::Init()
{
    img_page = cvLoadImage(".\\img\\enter.bmp");
    img_login = cvLoadImage(".\\img\\page.jpg");
    img_temp_login = cvLoadImage(".\\img\\page.jpg");
    img_register = cvLoadImage(".\\img\\register.jpg");
    img_temp_register = cvLoadImage(".\\img\\register.jpg");

    img_background = cvLoadImage(".\\img\\background.bmp");
    img_gameover = cvLoadImage(".\\img\\gameover.jpg");
    img_gamewin = cvLoadImage(".\\img\\gamewin.jpg");
    img_gamelose = cvLoadImage(".\\img\\gamelose.jpg");
    //img_gameintro = cvLoadImage(".\\img\\gameintro.jpg");

    img_bubble = cvLoadImage(".\\img\\bubble.png");
    img_people = cvLoadImage(".\\img\\people_red.png");
    img_my_people = cvLoadImage(".\\img\\people_red.png");
    img_people_red = cvLoadImage(".\\img\\people_red.png");
    img_people_blue = cvLoadImage(".\\img\\people_blue.png");
    img_tool_bubble = cvLoadImage(".\\img\\泡泡道具.png");
    img_box = cvLoadImage(".\\img\\小镇块黄色.bmp");
    img_tool_shoes = cvLoadImage(".\\img\\鞋子道具.png");
    img_tool_magic = cvLoadImage(".\\img\\威力道具.png");
    img_obstacle = cvLoadImage(".\\img\\房子.png");
    img_boom = cvLoadImage(".\\img\\boom.png");
    img_die = cvLoadImage(".\\img\\die.png");
}


DWORD WINAPI Tcp_Fun(LPVOID lpParameter)
{
    CTcp *tcp = (CTcp*)lpParameter;
    tcp->Connect();
    return 0;
}

void CGame::GameRun()
{
    //CTcp m_tcp;

    HANDLE hThread_tcp = CreateThread(NULL, 0, Tcp_Fun, &m_tcp, 0, NULL);
    if (hThread_tcp == NULL)
    {
        printf("create hThread_tcp failed!\n");
    }
    memset(sendmsg, 0, sizeof(sendmsg));
    char sendbuf[4096] = { 0 };

    //显示界面
    cvNamedWindow("page", 1);
    cvShowImage("page", img_page);
    cvWaitKey(2000);
    cvDestroyWindow("page");

    while (1)
    {
        Login();
        //cvWaitKey(1000);//等待服务器返回消息
        m_tcp.Send_Tcp_Msg(sendmsg);
        MessageBoxA(NULL, sendmsg, "发送消息", MB_OK);
        
        
        if (strstr(sendmsg, "login") != NULL)
        {
            if (m_tcp.login == 1)
            {
                MessageBoxA(NULL, "登录成功！", "登录信息", MB_OK);
                break;
            }
            else MessageBoxA(NULL, "登录失败！", "登录信息", MB_OK);
        }
        else
        {
            if (m_tcp.reg == 1)
            {
                MessageBoxA(NULL, "注册成功！", "注册信息", MB_OK);
            }
            else MessageBoxA(NULL, "注册失败！", "注册信息", MB_OK);
        }
        
        //break;
    }
    //cvWaitKey(0);

    //等待游戏开始
    //cvWaitKey(6000);
    cvDestroyWindow("login");

    overflag = 0;

restart:

    //进入游戏大厅
    cvNamedWindow("gamemenu", 1);

restart_2:
    img_gamemenu = cvLoadImage(".\\img\\gamemenu.jpg");
    //cvShowImage("gamememu", img_gamemenu);
    
    while (1)
    {   
        img_gamemenu = cvLoadImage(".\\img\\gamemenu.jpg");
        int status[8] = { 0 };
        //获取大厅内人物的状态  颜色和准备
        //0表示没有人
        //1表示红色未准备
        //2表示红色准备
        //3表示蓝色未准备
        //4表示蓝色准备
        m_tcp.GetStatus(status);
        
        //地图偏移
        int remove_x = 50;
        int remove_y = 85;
        //贴图偏移
        int pic_x = 0;
        int pic_y = 0;
        for (int i = 0; i < 8; i++)
        {
            if (status[i] == 0)
            {
                continue;
            }
            else if (status[i] == 1)
            {
                img_people = cvLoadImage(".\\img\\people_red.png");
                //pic_x = (img_people->width - 40) / 2;
                //pic_y = img_people->height - 40;
                int posx = 0;
                if(i < 4)
                    posx = i * 105 + remove_x - pic_x;
                else
                    posx = (i - 4) * 105 + remove_x - pic_x;
                float j = 0;
                if (i >= 0 && i < 4) j = 1;
                else j = 3 + 1.5;
                int posy = j * 40 + remove_y - pic_y;
                for (int x = 0; x<img_people->width; x++)
                {
                    for (int y = 0; y<img_people->height; y++)
                    {
                        int r, g, b;
                        r = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x);
                        g = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 1);
                        b = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 2);
                        if (r < 35 && g < 35 && b < 35)continue;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx)) = r;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                    }
                }

            }
            else if (status[i] == 2)
            {
                img_people = cvLoadImage(".\\img\\people_red.png");
                //pic_x = (img_people->width - 40) / 2;
                //pic_y = img_people->height - 40;
                int posx = 0;
                if (i < 4)
                    posx = i * 105 + remove_x - pic_x;
                else
                    posx = (i - 4) * 105 + remove_x - pic_x;
                float j = 0;
                if (i >= 0 && i < 4) j = 1;
                else j = 3 + 1.5;
                int posy = j * 40 + remove_y - pic_y;
                for (int x = 0; x<img_people->width; x++)
                {
                    for (int y = 0; y<img_people->height; y++)
                    {
                        int r, g, b;
                        r = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x);
                        g = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 1);
                        b = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 2);
                        if (r < 35 && g < 35 && b < 35)continue;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx)) = r;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                    }
                }
                //写上上准备两个字
                char *text = "ready";
                CvPoint p;
                CvScalar p_color;
                CvFont font;
                cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
                p_color.val[0] = 255;
                p_color.val[1] = 0;
                p_color.val[2] = 255;
                p.x = posx;
                p.y = posy + 80;
                cvPutText(img_gamemenu, text, p, &font, p_color);

                text = NULL;
            }
            else if (status[i] == 3)
            {
                img_people = cvLoadImage(".\\img\\people_blue.png");
                //pic_x = (img_people->width - 40) / 2;
                //pic_y = img_people->height - 40;
                int posx = 0;
                if (i < 4)
                    posx = i * 105 + remove_x - pic_x;
                else
                    posx = (i - 4) * 105 + remove_x - pic_x;
                float j = 0;
                if (i >= 0 && i < 4) j = 1;
                else j = 3 + 1.5;
                int posy = j * 40 + remove_y - pic_y;
                for (int x = 0; x<img_people->width; x++)
                {
                    for (int y = 0; y<img_people->height; y++)
                    {
                        int r, g, b;
                        r = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x);
                        g = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 1);
                        b = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 2);
                        if (r < 35 && g < 35 && b < 35)continue;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx)) = r;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                    }
                }

            }
            else if (status[i] == 4)
            {
                img_people = cvLoadImage(".\\img\\people_blue.png");
                //pic_x = (img_people->width - 40) / 2;
                //pic_y = img_people->height - 40;
                int posx = 0;
                if (i < 4)
                    posx = i * 105 + remove_x - pic_x;
                else
                    posx = (i - 4) * 105 + remove_x - pic_x;
                float j = 0;
                if (i >= 0 && i < 4) j = 1;
                else j = 3 + 1.5;
                int posy = j * 40 + remove_y - pic_y;
                for (int x = 0; x<img_people->width; x++)
                {
                    for (int y = 0; y<img_people->height; y++)
                    {
                        int r, g, b;
                        r = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x);
                        g = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 1);
                        b = CV_IMAGE_ELEM(img_people, uchar, y, 3 * x + 2);
                        if (r < 35 && g < 35 && b < 35)continue;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx)) = r;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                        CV_IMAGE_ELEM(img_gamemenu, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                    }
                }
                //写上上准备两个字
                char *text = "ready";
                CvPoint p;
                CvScalar p_color;
                CvFont font;
                cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
                p_color.val[0] = 255;
                p_color.val[1] = 0;
                p_color.val[2] = 255;
                p.x = posx;
                p.y = posy + 80;
                cvPutText(img_gamemenu, text, p, &font, p_color);
                cvShowImage("gamemenu", img_gamemenu);
                text = NULL;
            }
        }
        char ch_msg[4096] = { 0 };
        m_tcp.GetMsg(ch_msg);
        if (strlen(ch_msg) > 0 && ((ch_msg[0] >= 'a' && ch_msg[0] <= 'z') || (ch_msg[0] >= 'a' && ch_msg[0] <= 'z') || (ch_msg[0] >= '0' && ch_msg[0] <= '9')))
        {
            char *text = ch_msg;
            CvPoint p;
            CvScalar p_color;
            CvFont font;
            cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
            p_color.val[0] = 255;
            p_color.val[1] = 0;
            p_color.val[2] = 255;
            p.x = 29;
            p.y = 456 + 30;
            cvPutText(img_gamemenu, text, p, &font, p_color);
            cvShowImage("gamemenu", img_gamemenu);
            text = NULL;
            
        }
        Menu();
        if (strlen(sendmsg) > 0)
        {
            m_tcp.Send_Tcp_Msg(sendmsg);
            //MessageBoxA(NULL, sendmsg, "发送消息", MB_OK);
        }

        //判断是否开始游戏
        int ret = m_tcp.IsStart();
        if (ret == 1)
        {
            //MessageBoxA(NULL, "游戏开始", "游戏", MB_OK);
            break;
        }
        //cvWaitKey(0);
    }

    if (overflag == 1) goto restart_2;

    cvNamedWindow("gamerun", 1);
    cvShowImage("gamerun", img_background);
    printf("Game Start");
    //
    cvDestroyWindow("gamemenu");


    while (1)
    {
        img_background = cvLoadImage(".\\img\\background.bmp");//每次用新地图绘制

                                                               //绘图
                                                               //0表示空
                                                               //1表示墙
                                                               //2表示人物
                                                               //3表示箱子
                                                               //4表示泡泡数量道具
                                                               //5表示泡泡威力道具
                                                               //6表示人物速度道具
                                                               //7表示泡泡爆炸
                                                               //8表示人物被泡泡困住 die
                                                               //9表示放了泡泡
        int mpt[25][25] = { 0 };
        m_tcp.GetMap(mpt);
        int posx = 0;
        int posy = 0;
        //mpt[0][0] = 1;
        //mpt[1][1] = 9;
        //mpt[1][2] = 2;
        //mpt[1][3] = 3;

        

        //转换方向
        
        int temp_mpt[25][25] = { 0 };
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                temp_mpt[i][j] = mpt[i][j];
            }
        }
        for (int i = 0; i < 13; i++)
        {
            for (int j = 0; j < 18; j++)
            {
                mpt[j][i] = temp_mpt[i][j];
            }
        }

        FILE *fp_mpt;
        fp_mpt = fopen("mpt.txt", "w+");
        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                fprintf(fp_mpt, "%d ", mpt[i][j]);
            }
            fprintf(fp_mpt, "\n");
        }
        fflush(fp_mpt);
        fclose(fp_mpt);
        
        //地图偏移
        int remove_x = 40;
        int remove_y = 40;
        //贴图偏移
        int pic_x = 0;
        int pic_y = 0;
        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                if (mpt[i][j] == 0) continue;
                if (mpt[i][j] == 9)
                {
                    pic_x = (img_bubble->width - 40) / 2;
                    pic_y = img_bubble->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_bubble->width; x++)
                    {
                        for (int y = 0; y<img_bubble->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 1)
                {
                    pic_x = (img_obstacle->width - 40) / 2;
                    pic_y = img_obstacle->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_obstacle->width; x++)
                    {
                        for (int y = 0; y<img_obstacle->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_obstacle, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_obstacle, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_obstacle, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 2)
                {
                    pic_x = (img_people_red->width - 40) / 2;
                    pic_y = img_people_red->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_people_red->width; x++)
                    {
                        for (int y = 0; y<img_people_red->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_people_red, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_people_red, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_people_red, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 12)
                {
                    pic_x = (img_people_blue->width - 40) / 2;
                    pic_y = img_people_blue->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_people_blue->width; x++)
                    {
                        for (int y = 0; y<img_people_blue->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_people_blue, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_people_blue, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_people_blue, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 3)
                {
                    pic_x = (img_box->width - 40) / 2;
                    pic_y = img_box->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_box->width; x++)
                    {
                        for (int y = 0; y<img_box->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_box, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_box, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_box, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 4)
                {
                    pic_x = (img_tool_bubble->width - 40) / 2;
                    pic_y = img_tool_bubble->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_tool_bubble->width; x++)
                    {
                        for (int y = 0; y<img_tool_bubble->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_tool_bubble, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_tool_bubble, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_tool_bubble, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 5)
                {
                    pic_x = (img_tool_bubble->width - 40) / 2;
                    pic_y = img_tool_bubble->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_tool_magic->width; x++)
                    {
                        for (int y = 0; y<img_tool_magic->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_tool_magic, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_tool_magic, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_tool_magic, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 6)
                {
                    pic_x = (img_tool_shoes->width - 40) / 2;
                    pic_y = img_tool_shoes->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_tool_shoes->width; x++)
                    {
                        for (int y = 0; y<img_tool_shoes->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_tool_shoes, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_tool_shoes, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_tool_shoes, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 7)
                {
                    pic_x = (img_boom->width - 40) / 2;
                    pic_y = img_boom->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_boom->width; x++)
                    {
                        for (int y = 0; y<img_boom->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_boom, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_boom, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_boom, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
                if (mpt[i][j] == 8)
                {
                    pic_x = (img_die->width - 40) / 2;
                    pic_y = img_die->height - 40;
                    posx = i * 40 + remove_x - pic_x;
                    posy = j * 40 + remove_y - pic_y;
                    for (int x = 0; x<img_die->width; x++)
                    {
                        for (int y = 0; y<img_die->height; y++)
                        {
                            int r, g, b;
                            r = CV_IMAGE_ELEM(img_die, uchar, y, 3 * x);
                            g = CV_IMAGE_ELEM(img_die, uchar, y, 3 * x + 1);
                            b = CV_IMAGE_ELEM(img_die, uchar, y, 3 * x + 2);
                            if (r < 35 && g < 35 && b < 35)continue;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
                            CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
                        }
                    }
                }
            }
        }
        /*
        for (int x = 0; x<img_bubble->width; x++)
        {
        for (int y = 0; y<img_bubble->height; y++)
        {
        int r, g, b;
        r = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x);
        g = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x + 1);
        b = CV_IMAGE_ELEM(img_bubble, uchar, y, 3 * x + 2);
        //if (r>220 && g>220 && b>220)continue;
        CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx)) = r;
        CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 1) = g;
        CV_IMAGE_ELEM(img_background, uchar, (y + posy), 3 * (x + posx) + 2) = b;
        }
        }
        */
        cvShowImage("gamerun", img_background);

        //判断游戏是否结束 1表示win 2表示lose
        if (m_tcp.IsGameOver() == 1)
        {
            cvDestroyWindow("gamerun");
            cvNamedWindow("gameover", 1);
            cvShowImage("gameover", img_gamewin);
            SendMsg("cancel");
            cvWaitKey(3000);
            cvDestroyWindow("gameover");
            overflag = 1;
            goto restart;
            exit(0);
            break;
        }

        if (m_tcp.IsGameOver() == 2)
        {
            cvDestroyWindow("gamerun");
            cvNamedWindow("gameover", 1);
            cvShowImage("gameover", img_gamelose);
            SendMsg("cancel");
            cvWaitKey(3000);
            cvDestroyWindow("gameover");
            overflag = 1;
            goto restart;
            exit(0);
            break;
        }

        memset(sendbuf, 0, sizeof(sendbuf));
        //scanf("%c", sendbuf);
        //fgets(sendbuf, strlen(sendbuf), stdin);
        //tcp.Send_Tcp_Msg(sendbuf);
        int key = cvWaitKey(200);
        //printf("key:%d\n", key);
        if (key == 32)
        {
            strcpy(sendbuf, "space");
        }
        else if (key == 37 || key == 2424832)
        {
            strcpy(sendbuf, "left");
        }
        else if (key == 38 || key == 2490368)
        {
            strcpy(sendbuf, "up");
        }
        else if (key == 39 || key == 2555904)
        {
            strcpy(sendbuf, "right");
        }
        else if (key == 40 || key == 2621440)
        {
            strcpy(sendbuf, "down");
        }
        else
        {
            //printf("other operation!\n");
        }
        //printf("sendbuf:%s\n", sendbuf);
        m_tcp.Send_Tcp_Msg(sendbuf);
    }
    cvWaitKey(0);
}

void CGame::Register()
{
    cvNamedWindow("register", 1);
    img_register = cvLoadImage(".\\img\\register.jpg");
    img_temp_register = cvLoadImage(".\\img\\register.jpg");
    cvShowImage("register", img_register);

    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(impassword, '\0', sizeof(impassword));
    int ui = 0;
    int pi = 0;
    while (true)//
    {
        int key = cvWaitKey(0);
        if (key == 13)break;
        if (key == 8)
        {
            if (ui>0)ui--;
            username[ui] = '\0';
            cvCopy(img_register, img_temp_register, NULL);
        }
        else
        {
            char temp = key;
            if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
                username[ui++] = temp;
        }
        char *text = username;
        CvPoint p;
        CvScalar p_color;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
        p_color.val[0] = 255;
        p_color.val[1] = 0;
        p_color.val[2] = 255;
        p.x = 400;
        p.y = 490;
        cvPutText(img_temp_register, text, p, &font, p_color);
        cvShowImage("register", img_temp_register);
        text = NULL;
    }
    cvShowImage("register", img_temp_register);


    cvCopy(img_temp_register, img_register, NULL);
    while (true)//
    {
        int key = cvWaitKey(0);
        if (key == 13)break;
        if (key == 8)
        {
            if (pi>0)pi--;
            password[pi] = '\0';
            cvCopy(img_register, img_temp_register, NULL);
        }
        else
        {
            char temp = key;
            password[pi++] = temp;
        }
        int i;
        memset(impassword, '\0', sizeof impassword);
        for (i = 0; i<pi; i++)
        {
            if (password[i] != '\0')impassword[i] = '*';
            else break;
        }
        char *text = impassword;
        CvPoint p;
        CvScalar p_color;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
        p_color.val[0] = 255;
        p_color.val[1] = 255;
        p_color.val[2] = 0;
        p.x = 400;
        p.y = 517;
        cvPutText(img_temp_register, text, p, &font, p_color);
        cvShowImage("register", img_temp_register);
        text = NULL;
    }
    cvShowImage("register", img_temp_register);
    //img_register = cvLoadImage(".\\img\\register.jpg");
    //img_temp_register = cvLoadImage(".\\img\\register.jpg");
    cvDestroyWindow("register");
    memset(sendmsg, 0, sizeof(sendmsg));
    sprintf(sendmsg, "register+%s+%s", username, password);
    m_tcp.Send_Tcp_Msg(sendmsg);

    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(impassword, '\0', sizeof(impassword));
}

void CGame::Menu()
{
    //cvNamedWindow("gamemenu", 1);
    //img_gamemenu = cvLoadImage(".\\img\\gamemenu.jpg");
    img_temp_gamemenu = cvLoadImage(".\\img\\gamemenu.jpg");
    
    cvShowImage("gamemenu", img_gamemenu);
    cvSetMouseCallback("gamemenu", OnMouse_2, this);


    cvCopy(img_gamemenu, img_temp_gamemenu, NULL);
    memset(username, '\0', sizeof(username));
    int ui = 0;
    int pi = 0;
    int cnt_num = 10;
    while (cnt_num > 0)//
    {
        cnt_num--;
        int key = cvWaitKey(200);
        if (key == 13)break;
        if (key == 8)
        {
            if (ui>0)ui--;
            username[ui] = '\0';
            cvCopy(img_gamemenu, img_temp_gamemenu, NULL);
        }
        else
        {
            char temp = key;
            if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
                username[ui++] = temp;
        }
        char *text = username;
        CvPoint p;
        CvScalar p_color;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
        p_color.val[0] = 255;
        p_color.val[1] = 0;
        p_color.val[2] = 255;
        p.x = 144;
        p.y = 528 + 5;
        cvPutText(img_temp_gamemenu, text, p, &font, p_color);
        cvShowImage("gamemenu", img_temp_gamemenu);
        text = NULL;
    }
    cvShowImage("gamemenu", img_gamemenu);

    //img_login = cvLoadImage(".\\img\\page.jpg");
    //img_temp_login = cvLoadImage(".\\img\\page.jpg");
    //int ans = FindUser(username, password);
    memset(sendmsg, 0, sizeof(sendmsg));
    if(strlen(username) > 0)
        sprintf(sendmsg, "gamemenu+%s", username);
}

void CGame::Login()
{
    cvNamedWindow("login", 1);
    img_login = cvLoadImage(".\\img\\page.jpg");
    img_temp_login = cvLoadImage(".\\img\\page.jpg");
    cvShowImage("login", img_login);
    cvSetMouseCallback("login", OnMouse, this);
    //cvWaitKey(0);
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(impassword, '\0', sizeof(impassword));
    int ui = 0;
    int pi = 0;
    while (true)//
    {
        int key = cvWaitKey(0);
        if (key == 13)break;
        if (key == 8)
        {
            if (ui>0)ui--;
            username[ui] = '\0';
            cvCopy(img_login, img_temp_login, NULL);
        }
        else
        {
            char temp = key;
            if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
                username[ui++] = temp;
        }
        char *text = username;
        CvPoint p;
        CvScalar p_color;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
        p_color.val[0] = 255;
        p_color.val[1] = 0;
        p_color.val[2] = 255;
        p.x = 400;
        p.y = 490;
        cvPutText(img_temp_login, text, p, &font, p_color);
        cvShowImage("login", img_temp_login);
        text = NULL;
    }
    cvShowImage("login", img_temp_login);


    cvCopy(img_temp_login, img_login, NULL);
    while (true)//
    {
        int key = cvWaitKey(0);
        if (key == 13)break;
        if (key == 8)
        {
            if (pi>0)pi--;
            password[pi] = '\0';
            cvCopy(img_login, img_temp_login, NULL);
        }
        else
        {
            char temp = key;
            password[pi++] = temp;
        }
        int i;
        memset(impassword, '\0', sizeof impassword);
        for (i = 0; i<pi; i++)
        {
            if (password[i] != '\0')impassword[i] = '*';
            else break;
        }
        char *text = impassword;
        CvPoint p;
        CvScalar p_color;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.5f, 0.5f, 0, 1, CV_AA);
        p_color.val[0] = 255;
        p_color.val[1] = 255;
        p_color.val[2] = 0;
        p.x = 400;
        p.y = 517;
        cvPutText(img_temp_login, text, p, &font, p_color);
        cvShowImage("login", img_temp_login);
        text = NULL;
    }
    cvShowImage("login", img_temp_login);
    //img_login = cvLoadImage(".\\img\\page.jpg");
    //img_temp_login = cvLoadImage(".\\img\\page.jpg");
    //int ans = FindUser(username, password);
    cvDestroyWindow("login");
    memset(sendmsg, 0, sizeof(sendmsg));
    sprintf(sendmsg, "login+%s+%s", username, password);

    //free(img_login);
    //free(img_temp_login);
    memset(username, '\0', sizeof(username));
    memset(password, '\0', sizeof(password));
    memset(impassword, '\0', sizeof(impassword));
}

void CGame::GameIntro()
{
    cvNamedWindow("gameintro", 1);
    cvShowImage("gameintro", img_gameintro);
    cvWaitKey(0);
    cvDestroyWindow("gameintro");
}

void CGame::ShowWindow()
{
    //
}