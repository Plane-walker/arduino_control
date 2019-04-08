#include<math.h>
#include"servoct.h"
#include"motorct.h"
#include"automode.h"
#include"sensorct.h"

#define RELEASE

servoct servos(1400,1000,1800,2100,2500);
motorct motors(0.6);
pathfind track[3]={pathfind(8),pathfind(7),pathfind(18)};
ultrasonic ultr(4,19);//echo,t
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
 * 27------null
 * 28------detectdis
*/

void infocheck(String info)
{
  String order="";
  order+=info[0];
  order+=info[1];
  int lspeed,rspeed;
  int aimh,distance;
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
  }
  if(order.toInt()==24)
  {
    String temp="";
    for(int i=2;i<4;i++)
    temp+=info[i];
    distance=temp.toInt();
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
          motors.goahead();
        }
        break;
        case 2:
        {
          motors.goback();
        }
        break;
        case 3:
        {
          motors.goleft(spec);
        }
        break;
        case 4:
        {
          motors.goright(spec);
        }
        break;
        case 5:
        {
          motors.goAR(spec);
        }
        break;
        case 6:
        {
          motors.goAL(spec);
        }
        break;
        case 8:
        {
          motors.goBR(spec);
        }
        break;
        case 7:
        {
          motors.goBL(spec);
        }
        break;
        case 0:
        {
          motors.hardstop();
        }
        break;
        case 9:
        {
          motors.turnL();
        }
        break;
        case 10:
        {
          motors.turnR();
        }
        break;
//----------------------------------------------------------------
        case 28:
       {
        Serial.println(ultr.detective());
       }
       break;
       }
      }
}

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  while(Serial.available())
    {
      info+=char(Serial.read());
      delay(2);
    }
    if(!Serial.available()&&info.length()>1)
    {
      motors.closeclock();
      infocheck(info);
    }
    info="";
    motors.clockcome();
    if(atmode.ab())
    {
      if(!atmode.autop())
      atmode.autowork_s(track,motors);
      else
      atmode.auto_pick(ultr,servos);
    }
}
