#include <PID.h>
#include <encoder1.h>
#include <Servo.h>

#define spindle 13
#define motorX1 3
#define motorX2 5
#define motorY1 6
#define motorY2 9
#define motorZ1 10
#define motorZ2 11

encoder myencoderX(A0,A1);
encoder myencoderY(A2,A3);
encoder myencoderZ(A4,A5);

PID myPIDX(127, 0.1, 0); // KP, KI, KD
PID myPIDY(127, 0.1, 0); // KP, KI, KD
PID myPIDZ(127, 0.1, 0); // KP, KI, KD

byte c;

double setpointX = 0,
       setpointY = 0,
       setpointZ = 0,
       lastsetpointX = 0,
       lastsetpointY = 0,
       lastsetpointZ = 0,
       inputX = 0,
       inputY = 0,
       inputZ = 0,
       pwmX,
       pwmY,
       pwmZ;

void setup(){
  
  Serial.begin(115200);
  pinMode(spindle, OUTPUT);
  pinMode(motorX1, OUTPUT);
  pinMode(motorX2, OUTPUT);
  pinMode(motorY1, OUTPUT);
  pinMode(motorY2, OUTPUT);
  pinMode(motorZ1, OUTPUT);
  pinMode(motorZ2, OUTPUT);
  
  myPIDX.LimitP(127, -127); //Pmax , Pmin
  myPIDX.LimitI(127, -127); //Imax , Imin
  
  myPIDY.LimitP(127, -127); //Pmax , Pmin
  myPIDY.LimitI(127, -127); //Imax , Imin
  
  myPIDZ.LimitP(127, -127); //Pmax , Pmin
  myPIDZ.LimitI(127, -127); //Imax , Imin
  
  delay(1000);
  Serial.write('R'); //send R to computer. Arduino Ready....
  
}

void loop(){
  
  cekSerial();
  
  inputX = myencoderX.baca();
  inputY = myencoderY.baca();
  inputZ = myencoderZ.baca();

  pwmX = myPIDX.Calculate(setpointX, inputX);
  pwmY = myPIDY.Calculate(setpointY, inputY);
  pwmZ = myPIDZ.Calculate(setpointZ, inputZ);
  
  if(pwmX<0){
    digitalWrite(motorX1, LOW);
    analogWrite(motorX2, pwmX * -1);
  }else{
    analogWrite(motorX1, pwmX);
    digitalWrite(motorX2, LOW);
  }
  
  if(pwmY<0){
    digitalWrite(motorY1, LOW);
    analogWrite(motorY2, pwmY * -1);
  }else{
    analogWrite(motorY1, pwmY);
    digitalWrite(motorY2, LOW);
  }
  
  if(pwmZ<0){
    digitalWrite(motorZ1, LOW);
    analogWrite(motorZ2, pwmZ * -1);
  }else{
    analogWrite(motorZ1, pwmZ);
    digitalWrite(motorZ2, LOW);
  }
  /*
  Serial.print("Setpoint: ");
  Serial.print(setpointX);
  Serial.print(" Input: ");
  Serial.print(inputX);
  Serial.print(" Pwm: ");
  Serial.println(pwmX);
  */
}

void cekSerial(){
  
  if(Serial.available()>0){
    c = Serial.read();
    
    if(c == 1){ //if Computer send 1 spindle ON
      digitalWrite(spindle, HIGH);
      Serial.write('1');
    }//end if c = 1
    
    if(c == 2){ //if Computer send 2 spindle OFF
      digitalWrite(spindle, LOW);
      Serial.write('2');
    }//end if c = 2
    
    if(c == 4){ //if Computer send 4 SetpointX + 1;
      setpointX++;
    }//end if c = 3
    
    if(c == 8){ //if Computer send 8 SetpointX - 1;
      setpointX--;
    }//end if c = 4
    
    if(c == 16){ //if Computer send 16 SetpointY + 1;
      setpointY++;
    }//end if c = 5
    
    if(c == 32){ //if Computer send 32 SetpointY - 1;
      setpointY--;
    }//end if c = 6
    
    if(c == 64){ //if Computer send 64 SetpointZ + 1;
      setpointZ++;
    }//end if c = 7
    
    if(c == 128){ //if Computer send 128 SetpointZ - 1;
      setpointZ--;
    }//end if c = 8
    
  }//end serial available
}
