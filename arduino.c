#include "LedControl.h"
 
LedControl lc=LedControl(12,11,10,2);  // Pins: DIN,CLK,CS, # of Display connected
 
//unsigned long delayTime=50;  // Delay between Frames
 
byte getFrame(int frame,int level){
  if(frame == 3){
     if(level == 3 || level == 4)
       return B11111111;
     else
        return B00000000;
  }
   else if(frame == 2)
        return B00000001 <<  level;
   else if(frame == 4)
       return B10000000 >> level;
}
 
void setup()
{
  Serial.begin(9600);
 
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,5);  // Set intensity levels
  lc.setIntensity(1,5);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
}
 
 
//  Take values in Arrays and Display them
void frameShow(int frame)
{
  for (int i = 0; i < 8; i++)  
  {
    if(frame == 1)
      lc.setRow(0,i,B00011000);
    if(frame == 2)
      lc.setRow(0,i,getFrame(frame,i));
    if(frame == 3)
      lc.setRow(0,i,getFrame(frame,i));;
    if(frame == 4)
      lc.setRow(0,i,getFrame(frame,i));
  }
}
 
void rotate(int time,boolean dir){
  for(byte i=1;i<=4;++i){
    if(dir)
      frameShow(i);
    else
      frameShow(5-i);  
    delay(time);
  }
}
 
 
 
void loop()
{
  // Put #1 frame on both Display
  start:
  boolean direction = true;
  boolean speed=true;
  unsigned int delayTime = 1000;
  byte actualSpeed =2;
  int maxCounter = 0;
  while(true){
    rotate((11-actualSpeed)*delayTime/10,direction);
 
    if(actualSpeed > 1 && actualSpeed < 10){
      if(speed)
        ++actualSpeed;
      else
        --actualSpeed;
    }
    else if(actualSpeed == 10){
      if(maxCounter < 20) ++maxCounter;
      else if(maxCounter == 20){
        maxCounter = 0;
        --actualSpeed;
        speed = !speed;
      }
    }
    else if(actualSpeed == 1){
 
      frameShow(1);
      delay(500);
      ++actualSpeed;
      speed = !speed;
      direction = !direction;
    }
 
     
      if(Serial.available() > 0){
        char a[6];
        int i = 0;
        char tmp;
        /*while(a[i] != -1){
          if(i < 6 ){
             a[i] = Serial.read();
              ++i;
          }
         
        }*/
        do {
          tmp = Serial.read();
          if(i < 6 ){
             a[i] = tmp;
              ++i;
          }
        }while(tmp != -1);
       
        switch(a[0]){
        case 'R':
          Serial.print("R");
          break;
        case 'P':
          Serial.print("P");
          break;
        case 'S':
          goto start;
          break;
        case 'N':
          Serial.print("N");
          break;
        default:
          Serial.print("none");
 
        }
      }
    }  
 
}