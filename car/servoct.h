#ifndef SERVOCT_H_INCLUDE
#define SERVOCT_H_INCLUDE
#include<SoftwareSerial.h>
#include"sensorct.h"
class servoct
{
  private:
  SoftwareSerial *sserial;
  int *pos;
  int *originpos;
  int *posc;
  double handl,frontl,r1,r2,carh;
  double dispath,aimh;
  double *orde;
  double calcde(double a,double b,double c)
  {
    return acos((c*c+b*b-a*a)/2/c/b)*180/3.14159;
  }
  void setde()
  {
    double subline=pow(dispath*dispath+(carh-aimh)*(carh-aimh),0.5);
    double de2=calcde(subline,r1,r2)*3.14159/180;
    double de3=270-(calcde(r1,subline,r2)-calcde(dispath,subline,carh-aimh));
    double de1=calcde(r2,subline,r1)+calcde(carh-aimh,subline,dispath)+90;
    posc[2]=(-de1+orde[0])/180*2000;
    changepos(2);
    posc[3]=(-de2+orde[1])/180*2000;
    changepos(3);
    posc[4]=(de3-orde[2])/180*2000;
    changepos(4);
  }
  void changepos(int i)
  {
    pos[i]+=posc[i];
    if(pos[i]>2500)
    {
      posc[i]-=pos[i]-2500;
      pos[i]=2500;
    }
    if(pos[i]<500)
    {
      posc[i]+=500-pos[i];
      pos[i]=500;
    }
  }
  void writepos()
  {
    String order="";
    for(int i=0;i<5;i++)
    {
      order+="#";
      order.concat(i);
      order+=" P";
      order.concat(pos[i]);
      order+=" T";
      order.concat((int)(abs(posc[i])/1.5));
      order+=" ";
      posc[i]=0;
    }
    sserial->listen();
    //for(int i=0;i<10;i++)
    sserial->println(order);
    delay(2);
    sserial->end();
  }
  public:
  servoct(int o0,int o1,int o2,int o3,int o4)
  {
    sserial=new SoftwareSerial(2,3);//RX,TX
    sserial->begin(115200);
    handl=11.408;
    frontl=18.978;
    r1=15.145;
    r2=12.850;
    carh=14;
    dispath=9;
    aimh=10;
    orde=new double[3]{95.98,62.11,62.11};
    originpos=new int[5]{o0,o1,o2,o3,o4};
    pos=new int[5]{o0,o1,o2,o3,o4};
    posc=new int[5]{0,0,0,0,0};
    String order="";
    for(int i=0;i<5;i++)
    {
        order+="#";
        order.concat(i);
        order+=" P";
        order.concat(originpos[i]);
        order+=" ";
    }
  }
  ~servoct()
  {
    delete sserial;
    delete[] originpos;
    delete[] pos;
    delete[] posc;
  }
  void openhand()
  {
    posc[0]=1500-pos[0];
    pos[0]=1500;
    writepos();
  }
  void closehand()
  {
    posc[0]=900-pos[0];
    pos[0]=900;
    writepos();
  }
  void pushahead(ultrasonic &ultr,double ah=10)
  {
    aimh=ah;
    dispath=ultr.detective()+10;
    setde();
    writepos();
  }
  void pullback()
  {
    for(int i=1;i<5;i++)
    {
      posc[i]=originpos[i]-pos[i];
      pos[i]=originpos[i];
    }
    writepos();
  }
};
#endif // SERVOCT_H_INCLUDE
