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
#endif // SENSORCT_H_INCLUDE
