#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2,3);
LiquidCrystal_I2C lcd(0x27,16,2);

int buzzer = 13;
int btn = 7;

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

int cnt_m = 1;
char mode[10];

int gp_tone[] = {
  392, 523, 494, 523, 587, 523, 587, 659, 698, 659
, 440, 587, 587, 523, 523, 523, 494, 440, 494, 523
, 392, 523, 494, 523, 587, 523, 587, 659, 698, 659
, 440, 587, 587, 523, 523, 523, 494, 440, 494, 523
, 523, 659, 784, 659, 587, 523, 494, 523, 587, 523
, 494, 440, 392, 523, 659, 784, 659, 587, 523, 494
, 523, 587, 392, 392, 523, 587, 659, 659, 698, 659
, 440, 587, 587, 523, 523, 523, 494, 440, 494, 523
};

int gp_duration[]={
  2, 2, 1, 1, 2, 1, 1, 2, 1, 1
, 2, 1, 1, 2, 1, 1, 2, 1, 1, 4
, 2, 2, 1, 1, 2, 1, 1, 2, 1, 1
, 2, 1, 1, 2, 1, 1, 2, 1, 1, 4
, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1
, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1
, 1, 4, 1, 1, 4, 2, 1, 1, 1, 1
, 2, 1, 1, 2, 1, 1, 2, 1, 1, 4
};

void gp_Melody(){
  for(int i = 0; i < 80; i++){
     int mTime = 200*gp_duration[i];
     tone(buzzer ,gp_tone[i], mTime);
     delay(mTime);
     if(digitalRead(btn) == false){
        break;
     }
  }
}

int s_tone[] ={
  392, 392, 440, 440, 392, 392, 330, 392, 392, 330
, 330, 294, 392, 392, 440, 440, 392, 392, 330, 392
, 330, 294, 330, 262
};

int s_duration[] = {
  1, 1, 1, 1, 1, 1, 2, 1, 1, 1
, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1
, 1, 1, 1, 4  
};


void s_Melody(){
  for(int i = 0; i < 24; i++){
     int mTime = 200*s_duration[i];
     tone(buzzer ,s_tone[i], mTime);
     delay(mTime);
     if(digitalRead(btn) == false){
        break;
     }
  }
}

int w_tone[]{
  262, 330, 392, 262, 330, 392, 440, 440, 440, 392
, 349, 349, 349, 330, 330, 330, 294, 294, 294, 262
};

int w_duration[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2
, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2
};

void w_Melody(){
  for(int i = 0; i < 20; i++){
     int mTime = 200*w_duration[i];
     tone(buzzer ,w_tone[i], mTime);
     delay(mTime);
     if(digitalRead(btn) == false){
        break;
     }
  }
}

int play[] = {
  262, 294, 330, 349, 392, 440 ,494, 523, 587, 659
};

void basic(){
  lcd.setCursor(2,0);
  lcd.print("WELCOME!! ");
  
  lcd.createChar(0, smiley);
  lcd.setCursor(12,0);
  lcd.write(byte(0));
}

void setup() {
  BT_Serial.begin(9600);
  pinMode(btn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();

  basic();
}

void loop() {  
  if(BT_Serial.available()){
    char ch = BT_Serial.read();

    if(ch == 'm'){
      cnt_m *= -1;
    }
    
    if(cnt_m == -1){ 
      if(ch == '1'){
        gp_Melody();
      }else if( ch == '2'){
        s_Melody();
      }else if( ch == '3'){
        w_Melody();
      }
    }else if(cnt_m == 1){
      if(ch == '1'){
        tone(buzzer, play[0], 250);
        delay(250);
      }else if(ch =='2'){
        tone(buzzer, play[1], 250);
        delay(250);
      }
      else if(ch =='3'){
        tone(buzzer, play[2], 250);
        delay(250);
      }
      else if(ch =='4'){
        tone(buzzer, play[3], 250);
        delay(250);
      }
      else if(ch =='5'){
        tone(buzzer, play[4], 250);
        delay(250);
      }
      else if(ch =='6'){
        tone(buzzer, play[5], 250);
        delay(250);
      }
      else if(ch =='7'){
        tone(buzzer, play[6], 250);
        delay(250);
      }
      else if(ch =='8'){
        tone(buzzer, play[7], 250);
        delay(250);
      }
      else if(ch =='9'){
        tone(buzzer, play[8], 250);
        delay(250);
      }
      else if(ch =='0'){
        tone(buzzer, play[9], 250);
        delay(250);
      }
    }
  }
  
  lcd.setCursor(2,1);
  lcd.print("MODE : "); 
  if(cnt_m == -1){
    strcpy(mode, "PLAY");
  }else if(cnt_m == 1){
    strcpy(mode, "FREE");
  }
  lcd.setCursor(9,1);
  lcd.print(mode);
}
  
