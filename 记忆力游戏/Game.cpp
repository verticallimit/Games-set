// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	
}

CGame::~CGame()
{
	
}

//加载用户信息
void CGame::LoadUserInfo()
{
	CUser *user;
	fstream userfile;
	userfile.open("C:\\Users\\lx_Zz\\Desktop\\user.txt");
	while(!userfile.eof())
	{
		user=new CUser;
		string str;
		userfile>>str;
		user->SetUsername(str);
		userfile>>str;
		user->SetPassword(str);
		int num;
		userfile>>num;
		user->SetScore(num);
		userfile>>num;
		user->SetLevel(num);
		userlist.push_back(user);
	}
	userlist.pop_back();//最后一个为空要拿出来
	userfile.close();
}

void CGame::GameIntro()
{
	img_gameintro=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\游戏说明.jpg");
	cvNamedWindow("gameintro",1);
	cvShowImage("gameintro",img_gameintro);
	cvWaitKey(0);
	cvDestroyWindow("gameintro");
}

//初始化
void CGame::Init()
{
	//加载用户信息和图片
	score=0;
	LoadUserInfo();
	img_gameload=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\gameload.jpg");
	img_register=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\register.jpg");
	img_temp_register=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\register.jpg");
	img_login=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\login.jpg");
	img_temp_login=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\login.jpg");
	img_p1=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\西瓜.jpg");
	img_p2=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\梨.jpg");
	img_p3=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\橙子.jpg");
	img_p4=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\菠萝.jpg");
	img_p5=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\香蕉.jpg");
	img_p6=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\樱桃.jpg");
	img_p7=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\草莓.jpg");
	img_background=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\background_level1.jpg");
}

//回调函数、响应鼠标事件
void OnMouse(int event, int x, int y, int flags, void* param)
{
	CGame *game=(CGame*)param;
	if (event==CV_EVENT_LBUTTONDOWN)//表示鼠标左键按下
	{
		if(x>255&&x<375&&y>209&&y<242)//新建用户
		{
			cvDestroyWindow("start");
			game->Register();
		}
		else if(x>255&&x<375&&y>273&&y<305)//载入用户
		{
			cvDestroyWindow("start");
			game->Choose();
		}
		else if(x>255&&x<345&&y>344&&y<376)//排行榜/游戏说明
		{
			game->GameIntro();
		}
		else if(x>251&&x<413&&y>317&&y<444)//退出
		{
			exit(0);
		}
		else return;
	}
}

//运行
void CGame::GameRun()
{
	while(true)
	{
		cvNamedWindow("start",1);
		cvShowImage("start",img_gameload);
		cvSetMouseCallback("start",OnMouse,this);
		cvWaitKey(0);
		if(find==0)continue;

		CLevel1 level1;
		level1.Init(6,6,2,img_background,img_p1,img_p2);
		level1.Running();
		score+=level1.GetSocre();
		
		GameOver();
		cvWaitKey(0);
		cvDestroyWindow("gameover");
	
		CLevel2 level2;
		level2.Init(6,6,4,img_background,img_p1,img_p2,img_p3,img_p4);
		level2.Running();
		score+=level2.GetSocre();

		CLevel3 level3;
		level3.Init(6,6,6,img_background,img_p1,img_p2,img_p3,img_p4,img_p5,img_p6);
		level3.Running();
		score+=level3.GetSocre();

		CLevel4 level4;
		level4.Init(6,6,7,img_background,img_p1,img_p2,img_p3,img_p4,img_p5,img_p6,img_p7);
		level4.Running();
		score+=level4.GetSocre();
	
		GameOver();
		cvWaitKey(0);
		cvDestroyWindow("gameover");
	}
}


//游戏结束
 void CGame::GameOver()
{
	cvNamedWindow("gameover",1);
	img_gameover=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\gameover.jpg");
	
	char str[10];
	itoa(score,str,10);
	char *text=str;
	CvPoint p;
	CvScalar p_color;
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
	p_color.val[0]=255;
	p_color.val[1]=0;
	p_color.val[2]=255;
	p.x=250;
	p.y=250;
	cvPutText(img_gameover,text,p,&font,p_color);
	cvShowImage("gameover",img_gameover);

	fstream userfile;
	userfile.open("C:\\Users\\lx_Zz\\Desktop\\user.txt");
	vector<CUser *>::iterator iter1;
	for(iter1=userlist.begin();iter1!=userlist.end();iter1++)
	{
		string username1=(* iter1)->GetUsername();
		string password1=(* iter1)->GetPassword();
		int temp_score=(* iter1)->GetScore();
		int temp_level=(* iter1)->GetLevel();
		userfile<<username1<<" ";
		userfile<<password1<<" ";
		userfile<<temp_score<<" ";
		userfile<<temp_level<<endl;
	}
	userfile.close();
}


void CGame::Register()
{
	cvNamedWindow("register",1);
	cvShowImage("register",img_register);
	memset(username,'\0',sizeof username);
	memset(password,'\0',sizeof password);
	memset(impassword,'\0',sizeof impassword);
	int ui=0;
	int pi=0;
	while(true)//读入用户名
	{
		int key=cvWaitKey(0);
		if(key==13)break;
		if(key==8)
		{
			if(ui>0)ui--;
			username[ui]='\0';
			cvCopy(img_register,img_temp_register,NULL);
		}
		else
		{
			char temp=key;
			username[ui++]=temp;
		}
		char *text=username;
		CvPoint p;
		CvScalar p_color;
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=0;
		p_color.val[2]=255;
		p.x=169;
		p.y=150;
		cvPutText(img_temp_register,text,p,&font,p_color);
		cvShowImage("register",img_temp_register);
	}
	cvShowImage("register",img_temp_register);


	cvCopy(img_temp_register,img_register,NULL);
	while(true)//读入密码
	{
		int key=cvWaitKey(0);
		if(key==13)break;
		if(key==8)
		{
			if(pi>0)pi--;
			password[pi]='\0';
			cvCopy(img_register,img_temp_register,NULL);
		}
		else
		{
			char temp=key;
			password[pi++]=temp;
		}
		int i;
		memset(impassword,'\0',sizeof impassword);
		for(i=0;i<pi;i++)
		{
			if(password[i]!='\0')impassword[i]='*';
			else break;
		}
		char *text=impassword;
		CvPoint p;
		CvScalar p_color;
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=255;
		p_color.val[2]=0;
		p.x=169;
		p.y=200;
		cvPutText(img_temp_register,text,p,&font,p_color);
		cvShowImage("register",img_temp_register);
	}
	cvShowImage("register",img_temp_register);
	img_register=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\register.jpg");
	img_temp_register=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\register.jpg");
	cvDestroyWindow("register");

	CUser *user1;
	user1=new CUser;
	user1->SetUsername(username);
	user1->SetPassword(password);
	user1->SetScore(0);
	user1->SetLevel(1);
	userlist.push_back(user1);
}

//选择用户
void CGame::Choose()
{
	cvNamedWindow("login",1);
	cvShowImage("login",img_login);

	memset(username,'\0',sizeof username);
	memset(password,'\0',sizeof password);
	memset(impassword,'\0',sizeof impassword);
	int ui=0;
	int pi=0;
	while(true)//读入用户名
	{
		int key=cvWaitKey(0);
		if(key==13)break;
		if(key==8)
		{
			if(ui>0)ui--;
			username[ui]='\0';
			cvCopy(img_login,img_temp_login,NULL);
		}
		else
		{
			char temp=key;
			username[ui++]=temp;
		}
		char *text=username;
		CvPoint p;
		CvScalar p_color;
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=0;
		p_color.val[2]=255;
		p.x=200;
		p.y=102;
		cvPutText(img_temp_login,text,p,&font,p_color);
		cvShowImage("login",img_temp_login);
	}
	cvShowImage("login",img_temp_login);


	cvCopy(img_temp_login,img_login,NULL);
	while(true)//读入密码
	{
		int key=cvWaitKey(0);
		if(key==13)break;
		if(key==8)
		{
			if(pi>0)pi--;
			password[pi]='\0';
			cvCopy(img_login,img_temp_login,NULL);
		}
		else
		{
			char temp=key;
			password[pi++]=temp;
		}
		int i;
		memset(impassword,'\0',sizeof impassword);
		for(i=0;i<pi;i++)
		{
			if(password[i]!='\0')impassword[i]='*';
			else break;
		}
		char *text=impassword;
		CvPoint p;
		CvScalar p_color;
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=255;
		p_color.val[2]=0;
		p.x=200;
		p.y=180;
		cvPutText(img_temp_login,text,p,&font,p_color);
		cvShowImage("login",img_temp_login);
	}
	cvShowImage("login",img_temp_login);
	img_login=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\login.jpg");
	img_temp_login=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\login.jpg");
	int ans=FindUser(username,password);
	cvDestroyWindow("login");
}

//查找用户是否存在
int CGame::FindUser(string username,string password)
{
	vector<CUser *>::iterator iter;
	find=0;
	for(iter=userlist.begin();iter!=userlist.end();iter++)
	{
		if((* iter)->GetUsername()==username&&(* iter)->GetPassword()==password)
		{
			find=1;
			printf("isok\n");
			break;
		}
	}
	if(find==0)//未找到用户
	{
		img_notfind=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\notfind.jpg");
		cvNamedWindow("notfind",1);
		cvShowImage("notfind",img_notfind);
		cvWaitKey(0);
		cvDestroyWindow("notfind");
	}
	else
	{
		img_userinfo=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\userinfo.jpg");
		cvNamedWindow("userinfo",1);
		string str=(* iter)->GetUsername();
		char name1[105];
		memset(name1,'\0',sizeof name1);
		int i;
		for(i=0;i<str.size();i++)
		{
			name1[i]=str[i];
		}
		char *text=name1;	
		CvPoint p;
		CvScalar p_color;
		CvFont font;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=0;
		p_color.val[2]=255;
		p.x=174;
		p.y=130;
		cvPutText(img_userinfo,text,p,&font,p_color);
		
		char name2[105];
		memset(name2,'\0',sizeof name2);
		int num=(* iter)->GetScore();
		Change(num,name2);
		char *text2=name2;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=0;
		p_color.val[2]=255;
		p.x=174;
		p.y=186;
		cvPutText(img_userinfo,text2,p,&font,p_color);

		char name3[105];
		memset(name3,'\0',sizeof name3);
		int num1=(* iter)->GetLevel();
		Change(num1,name3);
		char *text3=name3;
		cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,1.0f,1.0f,0,1,CV_AA);
		p_color.val[0]=255;
		p_color.val[1]=0;
		p_color.val[2]=255;
		p.x=174;
		p.y=240;
		cvPutText(img_userinfo,text3,p,&font,p_color);


		cvShowImage("userinfo",img_userinfo);
		cvWaitKey(0);
		cvDestroyWindow("userinfo");
	}
	return find;
}

void CGame::Change(int x,char *name2)
{
	char name[105];
	memset(name,'\0',sizeof name);
	int i=0;
	while(x>0)
	{
		char w=x%10+'0';
		x=x/10;
		name[i++]=w;
	}
	int k,j;
	for(k=0,j=i-1;j>=0;j--,k++)
	{
		name2[k]=name[j];
	}
}

