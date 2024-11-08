#include <SharpIR.h>

#define IRF A0
#define IRL A4
#define IRR A2
#define modelF 215
#define modelL 215
#define modelR 215

#define MOT_L1_IN1 10
#define MOT_L2_IN2 9
#define MOT_R1_IN3 3
#define MOT_R2_IN4 5

bool frontWall, leftWall, rightWall;
int speedL = 43, speedR = speedL + 2;
int distF, distL, distR, time = 50, time1 = 2, limitDistF = 5;
int distWall = (17 - 9)/2;
int k, min;

SharpIR SharpIRF(IRF, modelF);
SharpIR SharpIRL(IRL, modelL);
SharpIR SharpIRR(IRR, modelR);



void setup() {

  Serial.begin(9600);
  // Set all the motor control inputs to OUTPUT
  pinMode(MOT_L1_IN1, OUTPUT);
  pinMode(MOT_L2_IN2, OUTPUT);
  pinMode(MOT_R1_IN3, OUTPUT);
  pinMode(MOT_R2_IN4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(MOT_L1_IN1, LOW);
  digitalWrite(MOT_L2_IN2, LOW);
  digitalWrite(MOT_R1_IN3, LOW);
  digitalWrite(MOT_R2_IN4, LOW);

}

void loop() {

  walls();

  // Liber doar stanga. 
  if (frontWall == true && rightWall == true && leftWall == false)
  {
    walls();
    while (distF > limitDistF && rightWall == true)
    {
      keepStraightLineR();
      walls();
    }
    if(rightWall == true)
    {
      while (leftWall == false)
    {
      left();
      walls();
    }
    while (leftWall == true)
    {
      left();
      walls();
    }
    k = distR;
    min = distR;
    while(k < min)
    {
      left();
      walls();
      min = k;
      k = distR;
    }
    while(k > min)
    {
      right();
      walls();
      min = k;
      k = distR;
    }
    while(leftWall == false && rightWall == true)
    {
      keepStraightLineR();
      walls();
    }
    }
    
  }
  // Liber doar dreapta.
  if (frontWall == true && rightWall == false && leftWall == true)
  {
    walls();
        while (distF > limitDistF && leftWall == true)
      {
        keepStraightLineL();
        walls();
      }
      if(leftWall == true)
      {
        while (rightWall == false)
      {
        right();
        walls();
      }
        while (rightWall == true)
      {
        right();
        walls();
      }
      k = distL;
      min = distL;
        while(k < min)
      {
        right();
        walls();
        min = k;
        k = distL;
      }
      while(k > min)
      {
      left();
      walls();
      min = k;
      k = distR;
      }
        while(rightWall == false)
      {
        keepStraightLineL();
        walls();
      }
      }
  }
  //Liber doar fata.
 else if (frontWall == false && rightWall == true && leftWall == true)
  {
    walls();
    while ((distL > distWall) && leftWall == true && frontWall == false)
      {
        left2();
        delay(time);
        right2();
        delay(time/time1);
        walls();
      }
    while ((distL <= distWall) && leftWall == true && frontWall == false)
      {
        right2();
        delay(time);
        left2();
        delay(time/time1);
        walls();
      }
  }
  //Fundatura
  else if (frontWall == true && rightWall == true && leftWall == true)
  {
    walls();
    while (distF > limitDistF && rightWall == true && leftWall == true)
    {
      keepStraightLineR();
      walls();
    }
    if (rightWall == true && leftWall == true)
    {
        while (frontWall == true)
      {
        turnAround();
        walls();
      }
    }
    
  }
  //Liber fata si dreapta.
  else if (frontWall == false && rightWall == false && leftWall == true)
  {
    walls();
      while ((distL > distWall) && leftWall == true && rightWall == false)
      {
        left2();
        delay(time);
        right2();
        delay(time/time1);
        walls();
      }
    while ((distL <= distWall) && leftWall == true && rightWall == false)
      {
        right2();
        delay(time);
        left2();
        delay(time/time1);
        walls();
      }
  }
  //Liber fata si stanga.
  else if (frontWall == false && rightWall == true && leftWall == false)
  {
    delay(400);
    walls();
    while (leftWall == false) 
    {
      left();
      walls();
    }
    while (leftWall == true) 
    {
      left();
      walls();
    }
    while (frontWall == true) 
    {
      left();
      walls();
    }
    while (leftWall == false) 
    {
      left2();
      delay(300);
      right2();
      delay(300);
      walls();
    }
  }
  //Liber stanga si dreapta.
  else if (frontWall == true && rightWall == false && leftWall == false)
  {
    walls();
    while(distF > limitDistF)
    {
      forward();
      walls();
    }
    while (leftWall == false) 
    {
      left();
      walls();
    }
    while (leftWall == true) 
    {
      left();
      walls();
    }
    while (leftWall == false)
    {
      keepStraightLineR();
      walls();
    }
  }
  //Liber toate directiile.
  else if (frontWall == false && rightWall == false && leftWall == false)
  {
    walls();
    while (leftWall == false)
    {
      left();
      walls();
    }
    if(frontWall == false)
    {
        while (leftWall == true)
      {
        left();
        walls();
      }
        while (leftWall == false)
      {
        left2();
        delay(50);
        right2();
        delay(50);
        walls();
      }
    }
    
  }
}
  
void walls ()
{
   distF = SharpIRF.distance(); 
   distL = SharpIRL.distance(); 
   distR = SharpIRR.distance();

  if (distF < 16)
  {
    frontWall = true;
  } else if (distF >= 16)
  {
    frontWall = false;
  }

  if (distL < 14)
  {
    leftWall = true;
  } else if (distL >= 14)
  {
    leftWall = false;
  }

  if (distR < 14)
  {
    rightWall = true;
  } else if (distR >= 14)
  {
    rightWall = false;
  }
}

void keepStraightLineL()
{
  walls();
  if (distL > distWall)
      {
        left2();
        delay(time);
        right2();
        delay(time/time1);
        walls();
      }
  else if (distL <= distWall)
      {
        right2();
        delay(time);
        left2();
        delay(time/time1);
        walls();
      }
}

void keepStraightLineR()
{
  walls();
  if (distR > distWall)
      {
        right2();
        delay(time);
        left2();
        delay(time/time1);
        walls();
      }
  else if (distR <= distWall)
      {
        left2();
        delay(time);
        right2();
        delay(time/time1);
        walls();
      }
}

void turnAround()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, speedL);
  analogWrite(MOT_R1_IN3, 0);
  analogWrite(MOT_R2_IN4, speedR + 8);
}

void forward()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, speedL);
  analogWrite(MOT_R1_IN3, speedR);
  analogWrite(MOT_R2_IN4, 0);
}

void right()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, speedL);
  analogWrite(MOT_R1_IN3, 0);
  analogWrite(MOT_R2_IN4, speedR + 8);
}

void right2()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, speedL);
  analogWrite(MOT_R1_IN3, speedR-12);
  analogWrite(MOT_R2_IN4, 0);
}

void left()
{
  analogWrite(MOT_L1_IN1, speedL);
  analogWrite(MOT_L2_IN2, 0);
  analogWrite(MOT_R1_IN3, speedR + 8);
  analogWrite(MOT_R2_IN4, 0);
}

void left2()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, speedL - 12);
  analogWrite(MOT_R1_IN3, speedR);
  analogWrite(MOT_R2_IN4, 0);
}

void stop()
{
  analogWrite(MOT_L1_IN1, 0);
  analogWrite(MOT_L2_IN2, 0);
  analogWrite(MOT_R1_IN3, 0);
  analogWrite(MOT_R2_IN4, 0);
} 


  
  
    

   
