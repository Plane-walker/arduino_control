#ifndef SENSORCT_H_INCLUDE
#define SENSORCT_H_INCLUDE
class sensorct
{
  protected:
  int signin;
  int signout;
  public:
  sensorct()
  {
    
  }
  virtual ~sensorct()
  {
    
  }
  virtual int writesign()=0;
  virtual int readsign()=0;
};

class ultrasonic:public sensorct
{
  int writesign()
  {
    
  }
  int readsign()
  {
    
  }
  public:
  ultrasonic(int signin,int signout)
  {
    this->signin=signin;
    this->signout=signout;
    pinMode(signin,INPUT);
    pinMode(signout,OUTPUT);
  }
  ~ultrasonic()
  {
    
  }
  double detective()
  {
    digitalWrite(signout,LOW);
    delay(2);
    digitalWrite(signout,HIGH);
    delay(10);
    digitalWrite(signout,LOW);
    double distance=pulseIn(signin,HIGH)/58.0;
    return distance;
  }
};

class pathfind:public sensorct
{
  int writesign()
  {
    
  }
  public:
  pathfind(int signin)
  {
    this->signin=signin;
    pinMode(signin,INPUT); 
  }
  ~pathfind()
  {
    
  }
  int readsign()
  {
    return digitalRead(signin);
  }
};

class compass:public sensorct
{
  private:
  double aimdir;
  int writesign()
  {
    
  }
  int readsign()
  {
    
  }
  public:
  compass()
  {
    Serial2.begin(9600);
    aimdir=detective();
  }
  ~compass()
  {
    
  }
  double detective()
  {
    Serial2.write(0x31);
    String info="";
      while(Serial2.available()||info.length()==0)
    {
      char temp;
      temp=char(Serial2.read());
      if((temp>='0'&&temp<='9')||temp=='.')
      info+=temp;
      delay(2);
    }
    return info.toDouble();
  }
  void updatedir(double newdir)
  {
    aimdir=newdir;
  }
  double getdir()
  {
    return aimdir;
  }
};
#endif // SENSORCT_H_INCLUDE
