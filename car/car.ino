#include<SoftwareSerial.h>

#define DEBUG

SoftwareSerial sserial(2,3);//RX,TX
String info="";
int wheel[4][2]={7,8,9,10,11,12,14,15};
int wn[4][2]={};
int track[3]={16,17,18};
int EN[2]={5,6};
float P,I,D,error,previous_error=0;
bool autoc=false;
int Lspeed=128,Rspeed=128;
int degree[3]={1400,2200,2200};

float Kp=10,Ki=0.5,Kd=0;

void setwheelpin()
{
  for(int i=0;i<4;i++)
  for(int j=0;j<2;j++)
      pinMode(wheel[i][j],OUTPUT);
}

void dirc(int (*wn)[2])
{
  for(int i=0;i<4;i++)
  for(int j=0;j<2;j++)
    {digitalWrite(wheel[i][j],wn[i][j]);
    #ifdef DEBUG
    Serial.println(wn[i][j]);
    #endif
    }
}

void speedset(int L,int R)
{
  if(L<0)
  L=0;
  if(R<0)
  R=0;
  if(L>255)
  L=255;
  if(R>255)
  R=255;
    analogWrite(EN[0],L);
    analogWrite(EN[1],R);  
}

float calc_pid()
{
  P=error;
  I=I+error;
  D=error-previous_error;
  float PID_value =(Kp*P)+(Ki*I)+(Kd*D);
  previous_error=error;
  return PID_value;
}

void errorupdate()
{
  int trackn[3]={};
  for(int i=0;i<3;i++)
    trackn[i]=digitalRead(track[i]);
    if(trackn[0]==1&&trackn[1]==0&&trackn[2]==1)
    error=0;
    else if(trackn[0]==0&&trackn[1]==1&&trackn[2]==1)
    error=-1;
    else if(trackn[0]==1&&trackn[1]==1&&trackn[2]==0)
    error=1;
}
void automode()
{
  errorupdate();
  int fixnum=calc_pid();
  Lspeed+=(int)fixnum;
  Rspeed-=(int)fixnum;
  if(Lspeed<0)
  Lspeed=0;
  if(Rspeed<0)
  Rspeed=0;
  if(Lspeed>255)
  Lspeed=255;
  if(Rspeed>255)
  Rspeed=255;
  #ifdef DEBUG
  Serial.println(Lspeed);
  Serial.println(Rspeed);
  #endif
  speedset(Lspeed,Rspeed);
}

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
*/

void infocheck(String info)
{
  String order="";
  order+=info[0];
  order+=info[1];
  int lspeed,rspeed;
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
  }
  if(order.toInt()==24)
  {
    String temp="";
    for(int i=2;i<5;i++)
    temp+=info[i];
    degree[1]-=temp.toInt();  
    degree[2]-=temp.toInt();
    if(degree[1]<1200)
    degree[1]=1200;  
    if(degree[2]<1200)
    degree[2]=1200;  
  }
  if(order.toInt()==25)
  {
    String temp="";
    for(int i=2;i<5;i++)
    temp+=info[i];
    degree[1]+=temp.toInt();  
    degree[2]+=temp.toInt();  
    if(degree[1]>2200)
    degree[1]=2200;  
    if(degree[2]>2200)
    degree[2]=2200;
  }
  #ifdef DEBUG
  Serial.println(order);
  Serial.println(lspeed);
  Serial.println(rspeed);
  #endif
        if(order=="23")
        {
          autoc=!autoc;
        }
        else if(!autoc)
        {

//----------------------------------------------------------------
//armcontrol
        switch(order.toInt())
        {
          case 21:
        {
          for(int i=0;i<10;i++)
          {
          sserial.listen();
          sserial.println("#0 P1500 T500");
          #ifdef DEBUG
          if(i==0)
          Serial.println("pick");
          #endif
          }
          sserial.end();
        }
        break;
        case 22:
        {
          for(int i=0;i<10;i++)
          {
          sserial.listen();
          sserial.println("#0 P900 T500");
          #ifdef DEBUG
          if(i==0)
          Serial.println("put");
          #endif
          }
          sserial.end();
        }
        break;
        case 24:
        {
          String aorder="#1 p";
          aorder.concat(degree[1]);
          aorder+=" T";
          aorder.concat(degree[2]);
          aorder+=" #2 p";
          aorder.concat(degree[1]);
          aorder+=" T";
          aorder.concat(degree[2]);
          for(int i=0;i<10;i++)
          {
          sserial.listen();
          sserial.println(aorder);
          #ifdef DEBUG
          if(i==0)
          Serial.println(aorder);
          #endif
          }
          sserial.end();
        }
        break;
        case 25:
        {
          String aorder="#1 p";
          aorder.concat(degree[1]);
          aorder+=" T";
          aorder.concat(degree[2]);
          aorder+=" #2 p";
          aorder.concat(degree[1]);
          aorder+=" T";
          aorder.concat(degree[2]);
          for(int i=0;i<10;i++)
          {
          sserial.listen();
          sserial.println(aorder);
          #ifdef DEBUG
          if(i==0)
          Serial.println(aorder);
          #endif
          }
          sserial.end();
        }
        break;
//----------------------------------------------------------------

//----------------------------------------------------------------
//wheelcontrol
        case 1:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=0;
            wn[i][1]=!wn[i][0];
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 2:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=1;
            wn[i][1]=!wn[i][0];
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 3:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i==0||i==3);
            wn[i][1]=!wn[i][0];
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 4:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i==1||i==2);
            wn[i][1]=!wn[i][0];
          }
         speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 5:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=0;
            wn[i][1]=(i==0||i==3);
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 6:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=0;
            wn[i][1]=(i==1||i==2);
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 8:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i==1||i==2);
            wn[i][1]=0;
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 7:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i==0||i==3);
            wn[i][1]=0;
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 0:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=0;
            wn[i][1]=0;
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 10:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i%2!=0);
            wn[i][1]=(i%2==0);
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 9:
        {
          for(int i=0;i<4;i++)
          {
            wn[i][0]=(i%2==0);
            wn[i][1]=(i%2!=0);
          }
          speedset(lspeed,rspeed);
          dirc(wn);
        }
        break;
        case 11:
        {
          speedset(lspeed,rspeed);
        }
        break;
        }
//----------------------------------------------------------------
      }
}

void setup() 
{
  Serial.begin(9600);
  sserial.begin(115200);
  setwheelpin();
  for(int i=0;i<3;i++)
    pinMode(track[i],INPUT); 
    for(int i=0;i<4;i++)
    for(int j=0;j<2;j++)
    digitalWrite(wheel[i][j],0);
    pinMode(EN[0],OUTPUT);
    pinMode(EN[1],OUTPUT);
    speedset(128,128);
}

void loop() 
{
  while(Serial.available())
    {
      info+=char(Serial.read());
       delay(2);
    }
    if(!Serial.available()&&info.length()>1)
    infocheck(info);
    info="";
    if(autoc)
    automode();
}
