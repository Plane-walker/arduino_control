#ifndef AUTOMODE_H_INCLUDE
#define AUTOMODE_H_INCLUDE
#include"sensorct.h"
#include"servoct.h"
#include"motorct.h"
class automode
{
  double P,I,D,error,previous_error;
  double Kp,Ki,Kd;
  unsigned long timecheck;
  bool auto_p;
  int timeset;
  int disset;
  bool auto_begin;
  double calc_pid()
  {
    P=error;
    I=I+error;
    D=error-previous_error;
    double PID_value =(Kp*P)+(Ki*I)+(Kd*D);
    previous_error=error;
    return PID_value;
  }
  void errorupdate(pathfind *pf)
  {
    int trackn[3]={};
    for(int i=0;i<3;i++)
      trackn[i]=pf[i].readsign();
      if(trackn[0]==1&&trackn[1]==0&&trackn[2]==1)
      {
        error=0;
        timecheck=0;
      }
      else if(trackn[0]==0&&trackn[1]==1&&trackn[2]==1)
      {
        error=-1;
        timecheck=0;
      }
      else if(trackn[0]==1&&trackn[1]==1&&trackn[2]==0)
      {
        error=1;
        timecheck=0;
      }
      else if(trackn[0]==1&&trackn[1]==1&&trackn[2]==1&&previous_error==0)
      {
        if(timecheck==0)
        timecheck=millis();
        else if(millis()-timecheck>=timeset)
        auto_p=true;
      }
  }
  public:
  automode()
  {
    P=0;
    I=0;
    D=0;
    previous_error=0;
    Kp=10;
    Ki=0.5;
    Kd=0;
    timecheck=0;
    auto_p=false;
    timeset=1000;
    disset=10;
    auto_begin=false;
  }
  ~automode()
  {
    
  }
  void autowork(pathfind *pf,motorct &motors)
{
  errorupdate(pf);
  int fixnum=calc_pid();
  int Lspeed=motors.readlspeed()+(int)fixnum;
  int Rspeed=motors.readrspeed()-(int)fixnum;
  if(Lspeed<0)
  Lspeed=0;
  if(Rspeed<0)
  Rspeed=0;
  if(Lspeed>255)
  Lspeed=255;
  if(Rspeed>255)
  Rspeed=255;
  motors.writespeed(Lspeed,Rspeed);
}
  void auto_pick(ultrasonic &ultr,servoct &servos)
  {
    double distance=ultr.detective();
    if(distance<=disset)
    {
      servos.pushahead(ultr);
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
  }
};
#endif // AUTOMODE_H_INCLUDE
