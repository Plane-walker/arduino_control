#include<Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial sserial(2,3);//RX,TX
SoftwareSerial nserial(4,5);//RX,TX
String info="";
int track[6]={6,7,8,9,10,11};
float P,I,D,error,previous_error=0;
bool autoc=false;

float Kp=10,Ki=0.5,Kd=0;

void settrackpin()
{
  for(int i=0;i<6;i++)
      pinMode(track[i],INPUT);
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

void automode()
{
  calc_pid();
}

void infocheck()
{
  if(info.length()>0)
      {
        if(info=="auto")
        {
          autoc=!autoc;
          info=="";
        }
        if(!autoc)
        {
        if(info=="pick")
        {
          for(int i=0;i<30;i++)
          {
          sserial.listen();
          sserial.println("#0 P500 #1 P500 #2 P500 T500");
          Serial.println("#0 P1000 #1 P1000");
          delay(1000);
          }
          sserial.end();
          info="";
        }
        if(info=="put")
        {
          sserial.listen();
          sserial.println("put");
          Serial.println("put");
          sserial.end();
          info="";
        }
        if(info=="gohead")
        {
          nserial.listen();
          nserial.println("gahead");
          nserial.end();
          info="";
        }
        if(info=="goback")
        {
          nserial.listen();
          nserial.println("gaback");
          nserial.end();
          info="";
        }
        if(info=="goleft")
        {
          nserial.listen();
          nserial.println("galeft");
          nserial.end();
          info="";
        }
        if(info=="goright")
        {
          nserial.listen();
          nserial.println("galeft");
          nserial.end();
          info="";
        }
      }
      }
      if(autoc)
      {
        automode(); 
      }
}

void setup() 
{
  Serial.begin(9600);
  sserial.begin(115200);
  nserial.begin(115200);
  settrackpin();
}

void loop() 
{
  while(Serial.available())
    {
      info+=char(Serial.read());
      delay(2);
    }
     if(info.length()!=0)
     {
      for(int i=0;i<30;i++)
          {
          sserial.listen();
          sserial.println(info);
          Serial.println(info);
          }
          sserial.end();
          info="";
     }
}
