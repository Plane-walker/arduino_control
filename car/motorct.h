  #ifndef MOTORCT_H_INCLUDE
#define MOTORCT_H_INCLUDE
#include"sensorct.h"
class motorct
{
  private:
  int motorder;
  int lspeed,rspeed;
  int (*wheel)[2];
  int (*wn)[2];
  int *EN;
  
  double P,I,D,error,last_error,previous_error;
  double Kp,Ki,Kd;
  bool stassist;
  
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
  int calc_pid()
  {
    P=Kp*(error-last_error);
    I=Ki*error;
    D=Kd*(error-last_error*2+previous_error);
    double PID_value =P+I+D;
    last_error=error;
    previous_error=last_error;
    return (int)PID_value;
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
    motorder=0;
    setwheelpin();
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    digitalWrite(wheel[i][j],0);

    P=0;
    I=0;
    D=0;
    error=0;
    previous_error=0;
    last_error=0;
    Kp=200;
    Ki=20;
    Kd=100;
    stassist=true;
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
  }
  /*void goleft(compass & comp)
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
  }*/
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
  void changemor(int ord)
  {
    motorder=ord;
  }
  int getmor()
  {
    return motorder;
  }
  void autofixdir(compass &comp)
  {
    if(getmor()!=0&&getmor()!=9&&getmor()!=10)
    {
      error=comp.getdir()-comp.detective();
      if(error<-180)
      error+=360;
      if(error>180)
      error-=360;
      if((getmor()==1||getmor()==2))
      {
      if(assist())
      {
      int fixnum=calc_pid();
      int Lspeed,Rspeed;
      if(getmor()==1)
      {
        Lspeed=readlspeed()+(int)fixnum;
        Rspeed=readrspeed()-(int)fixnum;
        goahead();
      }
      else
      {
        Lspeed=readlspeed()-(int)fixnum;
        Rspeed=readrspeed()+(int)fixnum;
        goback();
      }
      if(Lspeed<0)
        Lspeed=0;
      if(Rspeed<0)
        Rspeed=0;
      if(Lspeed>255)
        Lspeed=255;
      if(Rspeed>255)
        Rspeed=255;
      writespeed(abs(Lspeed),abs(Rspeed));
      }
      }
      else
      {
        if(error>5)
        turnR();
        else if(error<-5)
        turnL();
        else
        {
          goahead();
          changemor(1);
        }
      }
    }
  }
   void changepid(int P,int I,int D)
  {
    Kp=P;
    Ki=I;
    Kd=D;
    error=0;
    last_error=0;
    previous_error=0;
  }
  bool assist()
  {
    return stassist;
  }
  void changeassist()
  {
    stassist=!stassist;
  }
};
#endif // MOTORCT_H_INCLUDE
