#ifndef MOTORCT_H_INCLUDE
#define MOTORCT_H_INCLUDE
class motorct
{
  private:
  int lspeed,rspeed;
  int (*wheel)[2];
  int (*wn)[2];
  int *EN;
  void setwheelpin()
  {
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
        pinMode(wheel[i][j],OUTPUT);
    pinMode(EN[0],OUTPUT);
    pinMode(EN[1],OUTPUT);
  }
  void dirc()
  {
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
      digitalWrite(wheel[i][j],wn[i][j]);
  }
  public:
  motorct()
  {
    wheel=new int[4][2];
    int temp1[4][2]={52,53,30,31,50,51,32,33};
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    wheel[i][j]=temp1[i][j];
    wn=new int[4][2];
    int temp2[4][2]={0,0,0,0,0,0,0,0};
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    wn[i][j]=temp2[i][j];
    EN=new int[2]{2,3};
    lspeed=128;
    rspeed=128;
    setwheelpin();
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    digitalWrite(wheel[i][j],0);
  }
  ~motorct()
  {
    delete[] wheel;
    delete[] wn;
    delete[] EN;
  }
  void goahead()
  {
    for(int i=0;i<4;i++)
       {
         wn[i][0]=1;
         wn[i][1]=!wn[i][0];
       }
       dirc();
  }
  void goback()
  {
    for(int i=0;i<4;i++)
       {
         wn[i][0]=0;
         wn[i][1]=!wn[i][0];
       }
       dirc();
  }
  void gonewdir(compass& comp,double dirchange)
  {
    double newdir=comp.getdir()+dirchange;
    if(newdir<0)
    newdir+=360;
    else if(newdir>360)
    newdir-=360;
    comp.updatedir(newdir);
    if(dirchange<0)
    turnL();
    else
    turnR();
  }
  void goleft(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==1||i==2);
      wn[i][1]=!wn[i][0];
      }
      dirc();
  }
  void goright(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==0||i==3);
      wn[i][1]=!wn[i][0];
      }
      dirc();
  }
  void goAL(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==1||i==2);
      wn[i][1]=0;
      }
      dirc();
  }
  void goAR(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==0||i==3);
      wn[i][1]=0;
      }
      dirc();
  }
  void goBL(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=0;
      wn[i][1]=(i==0||i==3);
      }
      dirc();
  }
  void goBR(compass & comp)
  {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=0;
      wn[i][1]=(i==1||i==2);
      }
      dirc();
  }
  void turnL()
  {
    for(int i=0;i<4;i++)
    {
       wn[i][0]=(i%2!=0);
       wn[i][1]=(i%2==0);
    }
    dirc();
  }
  void turnR()
  {
    for(int i=0;i<4;i++)
    {
       wn[i][0]=(i%2==0);
       wn[i][1]=(i%2!=0);
    }
    dirc();
  }
  void hardstop()
  {
    for(int i=0;i<4;i++)
    {
      wn[i][0]=0;
      wn[i][1]=0;
    }
    dirc();
  }
  void writespeed(int ls,int rs)
  {
    lspeed=ls;
    rspeed=rs;
    if(lspeed<0)
    lspeed=0;
    if(rspeed<0)
    rspeed=0;
    if(lspeed>255)
    lspeed=255;
    if(rspeed>255)
    rspeed=255;
    analogWrite(EN[0],lspeed);
    analogWrite(EN[1],rspeed);
  }
  int readlspeed()
  {
    return lspeed;
  }
  int readrspeed()
  {
    return rspeed;
  }
};
#endif // MOTORCT_H_INCLUDE
