#define outputA 2
#define outputB 3
#define in1 7
#define in2 33

float  kp=0.0001;
float kd=0.0000;
float ki=0;

int pos,dist=0;
float previousMillis = 0;
float currentMillis = 0;
int setpoint=800;
int error;
float  p;
float errord,errori=0.0000;
int eprev;
int pwm;




//int a1;

void setup()
{
  Serial.begin(9600);
  pinMode(outputA,INPUT_PULLUP);
   pinMode(outputB,INPUT_PULLUP);
   pinMode(in1,OUTPUT);
   pinMode(in2,OUTPUT);
   
   
   attachInterrupt(digitalPinToInterrupt(outputA),ReadEncoder,CHANGE);
  attachInterrupt(digitalPinToInterrupt(outputB),ReadEncoderB,CHANGE);
 
}
void loop()
{

 
   
 
 
   error=setpoint-pos;
 
 errord=(error-eprev);
  
   errori=errori+error;

 p=(kp*error)+(kd*errord)+(ki*errori);  
   
   eprev=error;     
  
   

 



   Serial.print(" p value: ");
   Serial.print(p);
  Serial.print("  error:  ");
   Serial.print(error);
   Serial.print(" position:  ");
   Serial.println(pos);
   
   
 int pwm=(int)(220*p);


 
if(error>0)
{
   
   digitalWrite(in2,HIGH);
   // digitalWrite(in1,LOW);
   //analogWrite(in1,pwm); 
    
}
 else if(error<0)
  {
   

    
     digitalWrite(in2,LOW);
 
  }
else 
{

 
  digitalWrite(in1,LOW);
   digitalWrite(in2,LOW);

}
 
if(pwm>220){
 

 analogWrite(in1,220);}
 else if(pwm>0)
{
  analogWrite(in1,pwm);
}
else if(pwm<=0)
{
  analogWrite(in1,pwm);
}





}

void ReadEncoder()
{
  if(digitalRead(outputA)==HIGH)
  {
    if(digitalRead(outputB)==HIGH)
    {
      pos++;
    }
    else
    {
      pos--;
    }
  }

  else if(digitalRead(outputA)== LOW)
  {
    if(digitalRead(outputB)==HIGH)
    {
      pos--;
    }
    else
    {
      pos++;
    }
  }
} 

void ReadEncoderB()
{
  if(digitalRead(outputB)==HIGH)
  {
    if(digitalRead(outputA)==HIGH)
    {
      pos--;
    }
    else
    {
      pos++;
    }
  }

  else if(digitalRead(outputB)==LOW)
  {
    if(digitalRead(outputA)==HIGH)
    {
      pos++;  
    }
    else
    {
      pos--;
    }
  }
}
