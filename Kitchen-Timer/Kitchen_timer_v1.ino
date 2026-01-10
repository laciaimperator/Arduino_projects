#include <LiquidCrystal_I2C.h>

// C++ code
//

#define buzzer 7
#define select 3
#define set 4
#define start 5
#define stop 6

LiquidCrystal_I2C lcd(0x26,16,2);
int start_time[4] = {0,0,0,0};
char bufor[8+1];
int sign_not_select = ' ';
int sign_select = '>';
int sign_working = '!';
int which = 0;
int pos_x[4] = {0,0,1,1};
int pos_y[4] = {0,8,0,8};
int has_started[4] = {0,0,0,0};
int mins[4] = {0,0,0,0};

void print_time(int x, int y, int min, int sign){
  lcd.setCursor(y,x);
  sprintf(bufor, "%c%02d:00 ", sign, min);
  lcd.print(bufor);
}


void setup()
{
  pinMode(select, INPUT_PULLUP);
  pinMode(set, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);
  pinMode(stop, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" 00:00");
  lcd.setCursor(8,0);
  lcd.print(" 00:00");
  lcd.setCursor(0,1);
  lcd.print(" 00:00");
  lcd.setCursor(8,1);
  lcd.print(" 00:00");
  Serial.begin(9600);
  
  print_time(pos_x[0],pos_y[0],mins[0], sign_select);
  
}
void loop()
{
  if(digitalRead(select) == LOW){
  delay(20);
  print_time(pos_x[which],pos_y[which],mins[which], sign_not_select);
  which = (which + 1)%4;
    while(has_started[which] == 1){
      which = (which + 1)%4;
    }
  print_time(pos_x[which],pos_y[which],mins[which], sign_select);
  while(digitalRead(select)==LOW);
  delay(20);
  }
  
 
  if(digitalRead(set) == LOW){
  delay(20);
  mins[which] = mins[which] + 1;
  print_time(pos_x[which],pos_y[which],mins[which], sign_select);
  while(digitalRead(set)==LOW);
  delay(20);
  }
 
  if(digitalRead(start) == LOW){
    delay(20);
    if(has_started[which] == 0){
     has_started[which] = 1;
     start_time[which] = millis();
    }
    while(digitalRead(start)==LOW);
    delay(20);
  }
  
  if(digitalRead(stop) == LOW){
    delay(20);
    if(has_started[which] == 1){
     has_started[which] = 0;
     mins[which] = 0;
     print_time(pos_x[which], pos_y[which], mins[which], sign_not_select);
    }
    while(digitalRead(start)==LOW);
    delay(20);
  }
  
  for(int i=0; i<4; i++){
    if(has_started[i] == 1){
      if (millis() < start_time[i] + mins[i] * 60000) {
        int sec = (start_time[i] + mins[i] * 60000 - millis()) / 1000;
        int m = sec / 60;
        int s = sec % 60;

        sprintf(bufor, "%c%02d:%02d ", sign_working, m, s);
        lcd.setCursor(pos_y[i], pos_x[i]);
        lcd.print(bufor);
      }
    
      else{
        tone(buzzer, 1000);
        delay(600);
        noTone(buzzer);
        has_started[i] = 0;
        mins[i] = 0;
        print_time(pos_x[i], pos_y[i], mins[i], sign_not_select);
      }
    }
  }
  

  
}