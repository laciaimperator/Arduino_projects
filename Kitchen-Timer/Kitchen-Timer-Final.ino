
#include <LiquidCrystal_I2C.h>

#define buzzer 7
#define select 3
#define set 4
#define start 5
#define stop 6

LiquidCrystal_I2C lcd(0x26, 16, 2);
char bufor[8 + 1];
int sign_not_select = ' ';  //timer has not started and isn't selected
int sign_select = '>';      //timer has not started and is selected
int sign_working = '!';     //timer has started and isn't selected
int sign_stop = '?';        //timer has started but is selected
int which = 0;              //selected timer
int pos_x[4] = {0, 0, 1, 1};
int pos_y[4] = {0, 8, 0, 8};
int has_started[4] = {0, 0, 0, 0};
unsigned long time[4] = { 0, 0, 0, 0 };  //millis() at the start of the timer
unsigned long last_tick[4] = {0,0,0,0};
int cur_sign[4] = {sign_select, sign_not_select, sign_not_select, sign_not_select};

void print_time(int i) {
  lcd.setCursor(pos_y[i], pos_x[i]);
  int mins = time[i]/60;
  int secs = time[i]%60;
  sprintf(bufor, "%c%02d:%02d ", cur_sign[i], mins, secs);
  lcd.print(bufor);
}

void buzz() {
  tone(buzzer, 440);
  delay(400);
  tone(buzzer, 440 + 220);
  delay(400);
  tone(buzzer, 440 + 440);
  delay(400);
  tone(buzzer, 440 + 440 + 220);
  delay(600);
  noTone(buzzer);
}

void setup() {
  pinMode(select, INPUT_PULLUP);
  pinMode(set, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);
  pinMode(stop, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" 00:00");
  lcd.setCursor(8, 0);
  lcd.print(" 00:00");
  lcd.setCursor(0, 1);
  lcd.print(" 00:00");
  lcd.setCursor(8, 1);
  lcd.print(" 00:00");
  Serial.begin(9600);
  //select first timer
  print_time(0);
}

void loop() {
  unsigned long now = millis();
  for (int i = 0; i < 4; i++) {
	if(has_started[i] && time[i] > 0) {
      if(now - last_tick[i] >= 1000) {
        last_tick[i] += 1000;
        time[i]--;

        if(time[i] == 0) {
          has_started[i] = 0;
          buzz();
        }
      }
    }


    //update all timer outputs on LCD
    if (i == which) {
      if (has_started[i] == 1) {
        cur_sign[i] = sign_stop;
      } else {
        cur_sign[i] = sign_select;
      }
    } else {
      if (has_started[i] == 1) {
        cur_sign[i] = sign_working;
      } else {
        cur_sign[i] = sign_not_select;
      }
    }
    print_time(i);
  }

  //select button action on click
  if(digitalRead(select) == LOW){
  delay(20);
  which = (which + 1)%4;
  while(digitalRead(select)==LOW);
  delay(20);
  }

  //set button action on click
  if(digitalRead(set) == LOW) {
    if(has_started[which] == 0) {
      time[which] += 60;
    }
    while(digitalRead(set)==LOW);
    delay(20);
  }

  //start button action on click
  if(digitalRead(start) == LOW) {
    if(!has_started[which] && time[which] > 0){
      has_started[which] = 1;
      last_tick[which] = now;
    }
    while(digitalRead(start)==LOW);
    delay(20);
  }

  //stop button action on click
  if(digitalRead(stop) == LOW) {
  	has_started[which] = 0;   // tylko pauza
    while(digitalRead(stop)==LOW);
    delay(20);
  }
}