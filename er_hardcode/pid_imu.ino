#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define motor1 6              
#define motor2 10
#define motor3 9
#define motor4 5
#define dir1 35
#define dir2 27
#define dir3 37
#define dir4 29


#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

float  kp=0.009;
float kd=0.0001;
float ki=0;
int pos,dist=0;

int setpoint;
int target;
int error;
float  p;
float errord,errori=0.0000;
int eprev;
int pwm;

int ms=175;


//int a1;

void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);

  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
  
  
   
 
}
void loop()
{

 imu::Quaternion quat = bno.getQuat();

 double rm[3][3];

rm[1][1] = quat.w()*quat.w() + quat.x()*quat.x() - quat.y()*quat.y() - quat.z()*quat.z();   
rm[1][2] = 2*quat.x()*quat.y() - 2*quat.w()*quat.z();            
rm[1][3] = 2*quat.x()*quat.z() + 2*quat.w()*quat.y();
rm[2][1] = 2*quat.x()*quat.y() + 2*quat.w()*quat.z();       
rm[2][2] = quat.w()*quat.w() - quat.x()*quat.x() + quat.y()*quat.y() - quat.z()*quat.z();          
rm[2][3] = 2*quat.y()*quat.z() - 2*quat.w()*quat.x();     
rm[3][1] = 2*quat.x()*quat.z() - 2*quat.w()*quat.y();       
rm[3][2] = 2*quat.y()*quat.z() + 2*quat.w()*quat.x();            
rm[3][3] = quat.w()*quat.w() - quat.x()*quat.x() - quat.y()*quat.y() + quat.z()*quat.z();

/* Create Roll Pitch Yaw Angles from Quaternions */
double yy = quat.y() * quat.y(); // 2 Uses below

//double roll = atan2(2 * (quat.w() * quat.x() + quat.y() * quat.z()), 1 - 2*(quat.x() * quat.x() + yy));
//double pitch = asin(2 * quat.w() * quat.y() - quat.x() * quat.z());
double yaw = atan2(2 * (quat.w() * quat.z() + quat.x() * quat.y()), 1 - 2*(yy+quat.z() * quat.z()));

/*  Convert Radians to Degrees */
//float rollDeg  = 57.2958 * roll;
//float pitchDeg = 57.2958 * pitch;
float yawDeg   = 57.2958 * yaw;

Serial.print(" Radians \t"); Serial.print(yawDeg,2);   Serial.println(" Degrees");




  delay(BNO055_SAMPLERATE_DELAY_MS);
   
  int yaw_t=fabs(yawDeg);
 
// float rev=(pos/enc_coun_rev)*360;
// target=re
 
 
setpoint=yaw_t;
target=30;
  
 //Serial.println(pos);
 
 //pid

  
   error=target-setpoint;
 errord=(error-eprev);
   errori=errori+error;
// errord=error-eprev;
   // rpm_right = (pos* 60 / enc_coun_rev);
 p=(kp*error)+(kd*errord)+(ki*errori);  
   
   eprev=error;     
  
   

 /*if(error<0)
  {
    
   // targetnew=error-targetnew;
    setpoint=targetnew;
    error=setpoint-pos;
    //errord=error;
    errord=(error-eprev)/deltaT;
    Serial.print(" target new ");
    Serial.print( setpoint );
    
  }*/


 // pvalue();

   Serial.print(" p value: ");
   Serial.print(p);
  Serial.print("  error:  ");
   Serial.print(error);
   Serial.print(" position:  ");
   Serial.println(pos);
   
  
 
if(error>0)
{
   clockwise();
    
}
 else if(error<0)
  {
   
    anticlockwise();
  }
else 
{
  stopmotor();
}
 




}

void clockwise()
{
  //ms=map(rx, Buff1, 127, 0, fast);
  //frontleft
    digitalWrite(dir1,LOW);
    analogWrite(motor1,ms);
    //frontright
    digitalWrite(dir2,LOW);
     analogWrite(motor2,ms);
     //backleft
    digitalWrite(dir3,HIGH);
     analogWrite(motor3,ms);
     //backright
    digitalWrite(dir4,HIGH);
     analogWrite(motor4,ms);
}
void anticlockwise()
{
  //ms=map(rx, Buff2, -128, 0,fast);
    //frontleft
    digitalWrite(dir1,HIGH);
    analogWrite(motor1,ms);
    //frontright
    digitalWrite(dir2,HIGH);
     analogWrite(motor2,ms);
     //backleft
    digitalWrite(dir3,LOW);
     analogWrite(motor3,ms);
     //backright
    digitalWrite(dir4,LOW);
     analogWrite(motor4,ms);
}
void stopmotor()
{
  
  //frontleft
    //digitalWrite(motor1,LOW);
    analogWrite(motor1,0);
    //frontright
    //digitalWrite(motor2,LOW);
     analogWrite(motor2,0);
     //backleft
    //digitalWrite(motor3,LOW);
    analogWrite(motor3,0);
     //backright
    //digitalWrite(motor4,LOW);
     analogWrite(motor4,0);
  
}
