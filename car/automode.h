#ifndef AUTOMODE_H_INCLUDE
#define AUTOMODE_H_INCLUDE
#include"sensorct.h"
#include"servoct.h"
#include"motorct.h"
class automode
{
  /*double P,I,D,last_error,previous_error;
  double Kp,Ki,Kd;*/
  int error;
  long timecheck;
  bool auto_p;
  bool stopcar;
  unsigned long timeset;
  int disset;
  bool auto_begin;
  /*int calc_pid()
  {
    P=Kp*(error-last_error);
    I=Ki*error;
    D=Kd*(error-last_error*2+previous_error);
    double PID_value =P+I+D;
    last_error=error;
    previous_error=last_error;
    return (int)PID_value;
  }*/
  void errorupdate(pathfind *pf)
  {
    int trackn[5]={};
    for(int i=0;i<5;i++)
      trackn[i]=pf[i].readsign();
      if(trackn[0]==0&&trackn[1]==0&&trackn[2]==1&&trackn[3]==0&&trackn[4]==0)
      {
        error=0;
        timecheck=0;
      }
      else if(trackn[0]==1)
      {
        error=2;
        timecheck=0;
      }
      else if(trackn[4]==1)
      {
        error=-2;
        timecheck=0;
      }
      else if(trackn[1]==1)
      {
        error=1;
        timecheck=0;
      }
      else if(trackn[3]==1)
      {
        error=-1;
        timecheck=0;
      }
      else
      {
        if(timecheck==0)  
        timecheck=millis();
        else if(millis()-timecheck>=timeset)
        {
          stopcar=true;
          timecheck=0;
        }
      }
  }
  public:
  automode()
  {
    /*P=0;
    I=0;
    D=0;
    error=0;
    previous_error=0;
    last_error=0;
    Kp=50;
    Ki=0;
    Kd=0;*/
    error=0;
    timecheck=0;
    auto_p=false;
    timeset=1500;
    disset=10;
    auto_begin=false;
    stopcar=false;
  }
  ~automode()
  {
    
  }
  /*void autowork(pathfind *pf,motorct &motors)
{
  if(stopcar)
{
  motors.hardstop();
  return;
}
  errorupdate(pf);
  int fixnum=calc_pid();
  int Lspeed=motors.readlspeed()+(int)fixnum;
  int Rspeed=motors.readrspeed()-(int)fixnum;
  if(Lspeed<0)
    motors.turnL();
  else
  if(Rspeed<0)
    motors.turnR();
  else
  motors.goahead();
  if(Lspeed>255)
  Lspeed=255;
  if(Rspeed>255)
  Rspeed=255;
  if(Lspeed<-255)
  Lspeed=-255;
  if(Rspeed<-255)
  Rspeed=-255;
  motors.writespeed(abs(Lspeed),abs(Rspeed));
}*/
void autowork_s(pathfind *pf,motorct &motors)
{
  if(stopcar)
{
  motors.hardstop();
  return;
}
  errorupdate(pf);
  switch(error)
  {
    case 0:
    {
      motors.writespeed(128,128);
      motors.goahead();
    }
    break;
    case 1:
    {
      motors.writespeed(128,128);
      motors.turnR();
    }
    break;
    case -1:
    {
      motors.writespeed(128,128);
      motors.turnL();
    }
    break;
    case 2:
    {
      motors.writespeed(255,255);
      motors.turnR();
    }
    break;
    case -2:
    {
      motors.writespeed(255,255);
      motors.turnL();
    }
    break;
  }
  //Serial3.println(error);
}
  void auto_pick(ultrasonic &ultr,servoct &servos)
  {
    double distance=ultr.detective();
    if(distance<=disset)
    {
      servos.pushahead(ultr,10,9);
      delay(500);
      servos.openhand();
      delay(500);
      servos.closehand();
      delay(500);
      servos.pullback();
      delay(500);
    }
  }
  bool ab()
  {
    return auto_begin;
  }
  bool autop()
  {
    return auto_p;
  }
  void changeab()
  {
    auto_begin=!auto_begin;
    stopcar=false;
    error=0;
  }
    /*void changepid(int P,int I,int D)
  {
    Kp=P;
    Ki=I;
    Kd=D;
  }*/
};
#endif // AUTOMODE_H_INCLUDE
