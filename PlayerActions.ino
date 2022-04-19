/*
The circuit:

01 - GND GND
02 - VCC +5V
03 - Contrast GND
04 - RS Pin35 (P6_7)
05 - R/W GND
06 - EN Pin34 (P2_3)
07 - DB0 GND
08 - DB1 GND
09 - DB2 GND
10 - DB3 GND
11 - DB4 Pin39 (P2_6)
12 - DB5 Pin38 (P2_4)
13 - DB6 Pin37 (P5_6)
14 - DB7 Pin36 (P6_6)
15 - BL+ +5V
16 - BL- GND

 */
#include <LiquidCrystal.h>
#include "OneMsTaskTimer.h"
#define LED RED_LED;
int jumpFlag = 0;                                                                   //creating variables
int forwardsFlag;
const int rightThresh = 700;                                                        //creating threshold values for joystick
int backwardsFlag;
const int leftThresh = 200;
const int selectPin = P2_5;                                                         //maping pin
int Xout;
int count = 0;
OneMsTaskTimer_t timerTask = {250, playerActionTimerISR, 0, 0};
int timerFlag;
enum GameStates {start,gameStart, wait, forwards, backwards, jump};                 //creating state table names
GameStates state;

typedef struct xy_struct{                                                           //creating struct
  int x;
  int y;
}XY;

XY HeroLocation;                                                                    //struct type for herolocation
LiquidCrystal lcd(P6_7, P2_3, P2_6, P2_4, P5_6, P6_6);

void setupPlayerActions(){                                                          // setup player actions
  Serial.begin(9600);                                                               //for serail monitor
  OneMsTaskTimer::add(&timerTask);                                                  //delay setup
  OneMsTaskTimer::start();
  pinMode(selectPin, INPUT_PULLUP);                                                 //for button press
  attachInterrupt(digitalPinToInterrupt(selectPin), jumpISR, RISING);
}

void tickFunc(){
  //State transitions
  switch(state){
    case start:
      state = gameStart;                                                            //goes to gamestart state
      break;
      
    case gameStart:
      state = wait;                                                                 //goes to wait state
      break;
      
    case wait:
      if(forwardsFlag == 1 && HeroLocation.x < 15){                                 //goes to fowards state if forwards flag is one as well if hero location is less then 15
        state = forwards;
      }
      else if(backwardsFlag == 1 && HeroLocation.x > 0){                            //goes to backwards state if backwards flag is one as well if herolocation is greater then 0
        state = backwards;
      }
      else if(jumpFlag == 1){                                                       //goes to jumo state if jumpflag is one
        count = 0;                                                                  //sets count to zero
        state = jump;
      }
      break;
      
    case forwards:                                          
      state = wait;                                                                 //goes back wait state
      break;
      
    case backwards:
      state = wait;                                                                 //goes back to wait state
      break;
      
    case jump:
      if(count < 2){                                                                //goes back into jump state if count is less than 2 to simulate two clock ticks
        state = jump;
      }
      else if(count >= 2){                                                          //goes to wait state if count is greater then or equal to two
        state = wait;
        HeroLocation.y = 1;                                                         //sets HeroLocation y back to 1 after jumping
      }
      break;
  }

  //State Actions
  switch(state){
    case start:                                                                     //break from case
      break;
      
    case gameStart:                                                                 //sets HeroLocation to (15,1) and prints to serial monitor
      HeroLocation.x = 0;
      HeroLocation.y = 1;
      printCord();
      break;
      
    case wait:                                                                    //breaks from state
      break;
     
    case forwards:                                                                //adds one to HeroLocation.x and sets forwards flag to zero and prints cord to serial monitor
      HeroLocation.x++;
      forwardsFlag = 0;
      printCord();
      break;
      
    case backwards:
      HeroLocation.x--;                                                           //subs one to HeroLocation.x and sets backwards flag to zero and prints cord to serial monitor
      backwardsFlag = 0;
      printCord();
      break;
      
    case jump:
      HeroLocation.y = 0;                                                        //sets HeroLocation.y to zero for jumping
      count++;                                                                   //count++ 
      jumpFlag = 0;                                                              //sets jumpFlag to zero
      printCord();                                                               //prints cord to serial monitor
      break;
  }
}

void loopPlayerActions(){                                                        //loop player actions 
  while(timerFlag == 0){ delay(10);}                                             //runs until timer flag is 1 and has a delay of 10ms
  timerFlag = 0;                                                                 //sets timerflag to zero
  delay(10);                                                                     //extra delay of 10ms
  tickFunc();                                                                    //function with state transitions and actions
  MoveISR();                                                                     //dected joystick movement 
  printCord();                                                                   //prints cordanites to serial monitor 
  
}

void jumpISR(){                                                                  //function to set jumpflag to one
  Serial.println("Jump Detected!");
  jumpFlag = 1;
}

void MoveISR(){                                                                  //function to detect joystick movement
  Xout = analogRead(P6_0);                                                       //set Xout to the read pin of 6.0
  if(Xout < leftThresh){                                                         //runs if Xout is less then left thresh
    Serial.println("Move Backwards");                                            //prints to serial monitor
    backwardsFlag = 1;                                                           //sets backwards flag to 1
  }
  else if(Xout < rightThresh && Xout > leftThresh){                              //runs if Xout is in between the ranges of left thresh and right thresh
    Serial.println("Move Forwards");                                             //prints to serial monitor
    forwardsFlag = 1;                                                            //sets forward flag to 1
  }
}

void playerActionTimerISR(){                                                     // sets timerflag to one
  timerFlag = 1;
}

void printCord(){                                                                //prints hero location to serial monitior
  Serial.print("Hero Location: ");
  Serial.print(HeroLocation.x);
  Serial.print(" ");
  Serial.println(HeroLocation.y);
}
