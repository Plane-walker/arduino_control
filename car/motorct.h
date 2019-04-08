#ifndef MOTORCT_H_INCLUDE
#define MOTORCT_H_INCLUDE
class motorct
{
  private:
  int lspeed,rspeed;
  int (*wheel)[2];
  int (*wn)[2];
  int *EN;
  bool clocked;
  unsigned long timedelay;
  long turntime;
  double dek;
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
  void timecost(double de)
  {
    double n=(double)lspeed/255.0*500.0;
    timedelay=2500*de/(13*3.14*dek*n)*1000;
    turntime=millis();
  }
  public:
  motorct(double dek=0.6):dek(dek)
  {
    wheel=new int[4][2];
    int temp1[4][2]={14,15,12,11,16,17,10,9};
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    wheel[i][j]=temp1[i][j];
    wn=new int[4][2];
    int temp2[4][2]={0,0,0,0,0,0,0,0};
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    wn[i][j]=temp2[i][j];
    EN=new int[2]{5,6};
    clocked=false;
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
  void goleft(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/2;
      timecost(sde);
      turnL();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==1||i==2);
      wn[i][1]=!wn[i][0];
      }
      dirc();
    }
  }
  void goright(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/2;
      timecost(sde);
      turnR();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==0||i==3);
      wn[i][1]=!wn[i][0];
      }
      dirc();
    }
  }
  void goAL(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/4;
      timecost(sde);
      turnL();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==1||i==2);
      wn[i][1]=0;
      }
      dirc();
    }
  }
  void goAR(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/4;
      timecost(sde);
      turnR();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=(i==0||i==3);
      wn[i][1]=0;
      }
      dirc();
    }
  }
  void goBL(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/4*3;
      timecost(sde);
      turnL();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=0;
      wn[i][1]=(i==0||i==3);
      }
      dirc();
    }
  }
  void goBR(int spec)
  {
    if(spec==0)
    {
      clocked=true;
      double sde=3.14/4*3;
      timecost(sde);
      turnR();
    }
    else
    {
      for(int i=0;i<4;i++)
      {
      wn[i][0]=0;
      wn[i][1]=(i==1||i==2);
      }
      dirc();
    }
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
  void clockcome()
  {
    if(clocked&&(millis()-turntime)>=timedelay)
    {
      clocked=false;
      goahead();
    }
  }
  void closeclock()
  {
    clocked=false;
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