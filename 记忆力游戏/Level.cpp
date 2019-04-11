// Level.cpp: implementation of the CLevel class.
//
//////////////////////////////////////////////////////////////////////

#include "Level.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLevel::CLevel()
{
	SetScore();
}

CLevel::~CLevel()
{

}

void CLevel::Init()
{
	kind=2;
	score=0;
	number=0;
	memset(vis,0,sizeof vis);
	memset(mpt,0,sizeof mpt);
	Rand();
	cx=-1;cy=-1;
	f_cx=cx;f_cy=cy;
}

void CLevel::Running()
{
	Drawtoback();
	Drawblock();
	
	cvShowImage("level1",img_background);
	while(true)
	{
		push=0;
		Click();
		cvShowImage("level1",img_background);
		if(push==0)cvWaitKey(100);
		if(number>=width*height)break;
	}
}

void CLevel::Drawtoback()
{
	cvNamedWindow("level1",1);
	cvShowImage("level1",img_background);
}

void CLevel::Drawblock()
{
	int i,j;
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			int px=i*60;
			int py=j*60;
			for(int x=0;x<img_back->height;x++)
			{
				for(int y=0;y<img_back->width;y++)
				{
					int r,g,b;
					r=CV_IMAGE_ELEM(img_back,uchar,y,3*x);
					g=CV_IMAGE_ELEM(img_back,uchar,y,3*x+1);
					b=CV_IMAGE_ELEM(img_back,uchar,y,3*x+2);

					CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px))=r;
					CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+1)=g;
					CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+2)=b;
		
				}
			}
		}
	}
}

//?ص???????Ӧ???¼?
void OnMouse1(int event, int x, int y, int flags, void* param)
{
	
	CLevel *level=(CLevel*)param;
	if (event==CV_EVENT_LBUTTONDOWN)//??????????
	{
		int i=0,j=0;
		for( i=0;i<level->width;i++)
		{
			for(j=0;j<level->height;j++)
			{
				int px=i*60;
				int py=j*60;
				int tx=(i+1)*60;
				int ty=(j+1)*60;
				if(x>=px&&y>=py&&x<=tx&&y<=ty)
				{
					level->cx=i;
					level->cy=j;
					level->push=1;
					return;
				}
			}
		}
		return;
	}
}

void CLevel::Click()
{
	cvSetMouseCallback("level1",OnMouse1,this);
	if(cx==-1&&cy==-1)return;
	if(cx==f_cx&&cy==f_cy)return;
	
	int px=cx*60;
	int py=cy*60;
	int x,y;
	IplImage *img_temp=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\???jpg");
	if(mpt[cx][cy]==1)
	{
		cvCopy(img_p1,img_temp,NULL);
	}
	if(mpt[cx][cy]==2)
	{
		cvCopy(img_p2,img_temp,NULL);
	}
	if(mpt[cx][cy]==3)
	{
		cvCopy(img_p3,img_temp,NULL);
	}
	if(mpt[cx][cy]==4)
	{
		cvCopy(img_p4,img_temp,NULL);
	}
	if(mpt[cx][cy]==5)
	{
		cvCopy(img_p5,img_temp,NULL);
	}
	if(mpt[cx][cy]==6)
	{
		cvCopy(img_p6,img_temp,NULL);
	}
	if(mpt[cx][cy]==7)
	{
		cvCopy(img_p7,img_temp,NULL);
	}
	for(x=0;x<img_temp->height;x++)
	{
		for(y=0;y<img_temp->width;y++)
		{
			int r,g,b;
			r=CV_IMAGE_ELEM(img_temp,uchar,y,3*x);
			g=CV_IMAGE_ELEM(img_temp,uchar,y,3*x+1);
			b=CV_IMAGE_ELEM(img_temp,uchar,y,3*x+2);

			CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px))=r;
			CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+1)=g;
			CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+2)=b;
		}
	}
	Judge();
}

void CLevel::Judge()
{
	if(vis[cx][cy]==0)
	{
		vis[cx][cy]=1;
		number++;
		if(number%2==0)
		{
			if(mpt[cx][cy]==mpt[f_cx][f_cy])
			{
				score+=add;
			}
			else
			{
				cvShowImage("level1",img_background);
				cvWaitKey(200);
				number-=2;
				score-=sub;
				vis[cx][cy]=0;
				vis[f_cx][f_cy]=0;

				int x,y;
				int px;	
				int py;

				px=cx*60;
				py=cy*60;
				for(x=0;x<img_back->height;x++)
				{
					for(y=0;y<img_back->width;y++)
					{
						int r,g,b;
						r=CV_IMAGE_ELEM(img_back,uchar,y,3*x);
						g=CV_IMAGE_ELEM(img_back,uchar,y,3*x+1);
						b=CV_IMAGE_ELEM(img_back,uchar,y,3*x+2);

						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px))=r;
						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+1)=g;
						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+2)=b;
					}
				}


				px=f_cx*60;
				py=f_cy*60;

				for(x=0;x<img_back->height;x++)
				{
					for(y=0;y<img_back->width;y++)
					{
						int r,g,b;
						r=CV_IMAGE_ELEM(img_back,uchar,y,3*x);
						g=CV_IMAGE_ELEM(img_back,uchar,y,3*x+1);
						b=CV_IMAGE_ELEM(img_back,uchar,y,3*x+2);

						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px))=r;
						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+1)=g;
						CV_IMAGE_ELEM(img_background,uchar,(y+py),3*(x+px)+2)=b;
					}
				}
				cvShowImage("level1",img_background);
			}
		}
		f_cx=cx;
		f_cy=cy;
	}
}

void CLevel::Rand()
{
	int flag=0;
	while(true)
	{
		srand(time(NULL));
		flag=0;
		int i,j;
		for(i=0;i<width;i++)
		{
			for(j=0;j<height;j++)
			{
				int x=rand()%kind+1;
				mpt[i][j]=x;
			}
		}
		int num=1;
		while(num<=kind)
		{
			int cnt=0;
			for(i=0;i<width;i++)
			{
				for(j=0;j<height;j++)
				{
					if(mpt[i][j]==num)cnt++;
				}
			}
			num++;
			if(cnt>0&&cnt%2==0)continue;
			else
			{
				flag=1;break;
			}
		}
		if(flag==0)break;
	}
}

int CLevel::GetSocre()
{
	return score;
}

void CLevel::Init(int w, int h, int num, IplImage *bg, IplImage *p1, IplImage *p2)
{
	width=w;
	height=h;
	kind=num;
	score=0;
	number=0;
	memset(vis,0,sizeof vis);
	memset(mpt,0,sizeof mpt);
	Rand();
	cx=-1;cy=-1;
	f_cx=cx;f_cy=cy;
	img_p1=p1;
	img_p2=p2;
	img_background=bg;
	img_back=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\back.jpg");
}

void CLevel::Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4)
{
	width=w;
	height=h;
	kind=num;
	score=0;
	number=0;
	memset(vis,0,sizeof vis);
	memset(mpt,0,sizeof mpt);
	Rand();
	cx=-1;cy=-1;
	f_cx=cx;f_cy=cy;
	img_p1=p1;
	img_p2=p2;
	img_p3=p3;
	img_p4=p4;
	img_background=bg;
	img_back=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\back.jpg");
}

void CLevel::Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4,IplImage *p5,IplImage *p6)
{
	width=w;
	height=h;
	kind=num;
	score=0;
	number=0;
	memset(vis,0,sizeof vis);
	memset(mpt,0,sizeof mpt);
	Rand();
	cx=-1;cy=-1;
	f_cx=cx;f_cy=cy;
	img_p1=p1;
	img_p2=p2;
	img_p3=p3;
	img_p4=p4;
	img_p5=p5;
	img_p6=p6;

	img_background=bg;
	img_back=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\back.jpg");
}

void CLevel::Init(int w,int h,int num,IplImage *bg,IplImage *p1,IplImage *p2,IplImage *p3,IplImage *p4,IplImage *p5,IplImage *p6,IplImage *p7)
{
	width=w;
	height=h;
	kind=num;
	score=0;
	number=0;
	memset(vis,0,sizeof vis);
	memset(mpt,0,sizeof mpt);
	Rand();
	cx=-1;cy=-1;
	f_cx=cx;f_cy=cy;
	img_p1=p1;
	img_p2=p2;
	img_p3=p3;
	img_p4=p4;
	img_p5=p5;
	img_p6=p6;
	img_p7=p7;

	img_background=bg;
	img_back=cvLoadImage("C:\\Users\\lx_Zz\\Desktop\\back.jpg");
}
void CLevel::SetScore()
{
	add=10;
	sub=1;
}
