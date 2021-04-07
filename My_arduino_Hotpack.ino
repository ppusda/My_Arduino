#include <Wire.h> 
 #include <LiquidCrystal_I2C.h>

 LiquidCrystal_I2C lcd(0x27, 16, 2);

 int ledpinRed = 9; 
 int ledpinGreen = 10;
 int ledpinBlue = 11;
 int btn = 12;
 int Temperature = A0;
 int Mosfet = 7 ;
 
 int bright;
 int cnt = 0;

 float temp;
 float val;

 void count(){
    if(digitalRead(btn) == HIGH){
      if(cnt >= 4){
        cnt = 0;
      }else{
        cnt ++;
      }    
    }
 }

 void cnt_option(int bright, float val){
  
   if(cnt == 0){   
      analogWrite(ledpinRed, LOW);
      analogWrite(ledpinGreen, LOW);
      analogWrite(ledpinBlue, LOW); 
      delay(100);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("State: OFF");
      lcd.setCursor(0,1);
      lcd.print("Degree: ");
      lcd.print(val);
      
      digitalWrite(Mosfet,LOW);
         
    }else if(cnt == 1){
      analogWrite(ledpinRed, LOW);
      analogWrite(ledpinGreen, LOW);
      analogWrite(ledpinBlue, bright);
      delay(100);

      if(val<40){
        digitalWrite(Mosfet,HIGH);
      }
      else{
        digitalWrite(Mosfet,LOW);
      }

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("State: LOW");
      lcd.setCursor(0,1);
      lcd.print("Degree: ");
      lcd.print(val);
      
    }else if(cnt == 2){
      analogWrite(ledpinRed, LOW);
      analogWrite(ledpinGreen, bright);
      analogWrite(ledpinBlue, LOW);
      delay(100);

      if(val<60){
        digitalWrite(Mosfet,HIGH);
      }
      else{
        digitalWrite(Mosfet,LOW);
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("State: MEDIUM");
      lcd.setCursor(0,1);
      lcd.print("Degree: ");
      lcd.print(val);
      
    }else if(cnt == 3){
      analogWrite(ledpinRed, bright);
      analogWrite(ledpinGreen, LOW);
      analogWrite(ledpinBlue, LOW);
      delay(100);


      if(val<80){
        digitalWrite(Mosfet,HIGH);
      }
      else{
        digitalWrite(Mosfet,LOW);
      }

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("State: HIGH");
      lcd.setCursor(0,1);
      lcd.print("Degree: ");
      lcd.print(val);
    }
 }

void setup() {
  pinMode(ledpinRed, OUTPUT);
  pinMode(ledpinGreen, OUTPUT);
  pinMode(ledpinBlue, OUTPUT); 
  pinMode(btn, INPUT_PULLUP);

  pinMode(Mosfet, OUTPUT);
  
  lcd.init();
  lcd.backlight();
}

void loop() {

  temp = (float)analogRead(Temperature)*5/1024;
  val = 100*(temp-0.5);

  for(bright = 0; bright <= 30; bright+=5){
    count();   
    cnt_option(bright,val);
  }
  
  for(bright = 30; bright >= 0; bright-=5){
    count();   
    cnt_option(bright, val);
  }
}
