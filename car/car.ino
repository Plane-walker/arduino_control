#include<math.h>
#include"servoct.h"
#include"motorct.h"
#include"automode.h"
#include"sensorct.h"

#define RELEASE

servoct servos(1400,1000,1800,2100,2500);
motorct motors;
pathfind track[5]={pathfind(37),pathfind(38),pathfind(39),pathfind(40),pathfind(41)};
ultrasonic ultr(34,35);//echo,t
compass comp;
automode atmode;
String info="";

/*
 * encipher_list
 * 00------stop
 * 01------goahead
 * 02------goback
 * 03------goleft
 * 04------goright
 * 05------goAR
 * 06------goAL
 * 07------goBL
 * 08------goBR
 * 09------turnL
 * 10------turnR
 * 11------speedchange
 * 12------null
 * 13------null
 * 14------null
 * 15------null
 * 16------null
 * 17------null
 * 18------null
 * 19------null
 * 20------null
 * 21------pick
 * 22------put
 * 23------auto
 * 24------pullahead
 * 25------pullback
 * 26------null
 * 27------detectde
 * 28------detectdis
*/

void infocheck(String info)
{
  String order="";
  order+=info[0];
  order+=info[1];
  int lspeed,rspeed;
  int aimh;
  double distance;
  int spec;
  if(order.toInt()<21)
  {
    String temp="";
    for(int i=2;i<5;i++)
          temp+=info[i];
          lspeed=temp.toInt();
          temp="";
          for(int i=5;i<8;i++)
          temp+=info[i];
          rspeed=temp.toInt();
          motors.writespeed(lspeed,rspeed);
          if(order.toInt()<9&&order.toInt()>2)
          {
            temp=info[8];
            spec=temp.toInt();
          }
          if(order=="11")
          return;
          if(order.toInt()>=0&&order.toInt()<=10)
          motors.changemor(order.toInt());
  }
  if(order.toInt()==24)
  {
    String temp="";
    for(int i=2;i<4;i++)
    temp+=info[i];
    distance=static_cast<double>(temp.toInt());
    temp="";
    for(int i=4;i<6;i++)
    temp+=info[i];
    aimh=temp.toInt();
  }
        if(order=="23")
        {
          atmode.changeab();
        }
        else if(!atmode.ab())
        {

//----------------------------------------------------------------
//armcontrol
        switch(order.toInt())
        {
          case 21:
        {
          servos.openhand();
        }
        break;
        case 22:
        {
         servos.closehand();
        }
        break;
        case 24:
        {
          servos.pushahead(ultr,distance,aimh);
        }
        break;
        case 25:
        {
          servos.pullback();
        }
        break;
//----------------------------------------------------------------

//----------------------------------------------------------------
//wheelcontrol
        case 1:
        {
          comp.updatedir(comp.detective());
          motors.goahead();
        }
        break;
        case 2:
        {
          comp.updatedir(comp.detective());
          motors.goback();
        }
        break;
        case 3:
        {
          motors.gonewdir(comp,-90);
        }
        break;
        case 4:
        {
          motors.gonewdir(comp,90);
        }
        break;
        case 5:
        {
          motors.gonewdir(comp,45);
        }
        break;
        case 6:
        {
          motors.gonewdir(comp,-45);
        }
        break;
        case 8:
        {
          motors.gonewdir(comp,135);
        }
        break;
        case 7:
        {
          motors.gonewdir(comp,-135);
        }
        break;
        case 0:
        {
          motors.hardstop();
        }
        break;
        case 9:
        {
          motors.gonewdir(comp,-180);
        }
        break;
        case 10:
        {
          motors.gonewdir(comp,180);
        }
        break;
//----------------------------------------------------------------
        case 28:
       {
        Serial3.println(ultr.detective());
       }
       break;
       case 27:
       {
        Serial3.println(comp.detective());
       }
       break;
       }
      }
}

void setup() 
{
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  comp.updatedir(comp.detective());
}

void loop() 
{
  while(Serial3.available())
    {
      info+=char(Serial3.read());
      delay(2);
    }
    if(!Serial3.available()&&info.length()>1)
      infocheck(info);
    info="";
    if(motor.getmor()!=0)
    {
      double fixrg=comp.getdir()-comp.detective();
      if(fixrg<-180)
      fixrg+=360;
      if(fixrg>180)
      fixrg-=360;
      if((fixrg>3&&motors.getmor()!=9&&motors.getmor()!=10)||(abs(fixrg)>3&&motors.getmor()==10))
      {
        motors.turnR();
      }
      else if((fixrg<-3&&motors.getmor()!=9&&motors.getmor()!=10)||(abs(fixrg)>3&&motors.getmor()==9))
      {
        motors.turnL();
      }
      else if(motors.getmor()==2)
      {
        motors.goback();
      }
      else
      {
        motors.goahead();
        motors.changemor(1);
      }
    }
    if(atmode.ab())
    {
      if(!atmode.autop())
      atmode.autowork_s(track,motors);
      else
      atmode.auto_pick(ultr,servos);
    }
}
