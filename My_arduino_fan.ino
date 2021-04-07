/////////////////////////////////////////////////////// 라이브러리 등 필요한 변수 선언

extern volatile unsigned long timer0_millis;

unsigned long c_time1 = 1000*30;
unsigned long c_time2 = 1000*60;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <IRremote.h>
int recvPin = A0;
IRrecv irrecv(recvPin);
decode_results results;

int mt = 3;
int mt_speed = 6;

int btn_power = 5;  // 전원 버튼
int pbtn_chk = -1;
int pbtn = 0;
int bf_pbtn = LOW;
char power[10];  

int btn_mode = 9;   // 모드 변경 버튼
int mbtn_chk = -1;
int mbtn = 0;
int bf_mbtn = LOW;
char mode[10];    

int btn_level = 10; // 바람 세기 버튼
int lbtn_chk = 1;
int lbtn = 0;
int bf_lbtn = LOW;
char level[10];  

int btn_time = 11;  // 타이머 버튼
int tbtn_chk = 1;
int tbtn = 0;
int bf_tbtn = LOW;
char timer_n[10];  

///////////////////////////////////////////////////////   전원 관련

void pbtn_chg(){    // 파워 버튼의 변화를 읽어와 상태를 변환한다.
  pbtn = digitalRead(btn_power);
  if((pbtn == HIGH && bf_pbtn == LOW)){
    pbtn_chk *= -1;
  }
  bf_pbtn = pbtn; 
}

///////////////////////////////////////////////////////  모드 관련

void mbtn_chg(){    // 모드 버튼의 변화를 읽어와 상태를 변환한다.
  mbtn = digitalRead(btn_mode);
  if(mbtn == HIGH && bf_mbtn == LOW){
    mbtn_chk *= -1;
  }
  bf_mbtn = mbtn; 
}

///////////////////////////////////////////////////////  바람 세기 관련

void level_chg_btn(){   // 레벨 버튼의 변화를 읽어와 상태를 변환한다.
  lbtn = digitalRead(btn_level);
  if(lbtn == LOW && bf_lbtn == HIGH){
    lbtn_chk += 1;
    if(lbtn_chk > 3){
       lbtn_chk = 1;
    }
  }
  bf_lbtn = lbtn; 
}

void level_cnt(){   // 레벨의 상태를 알려주고, 기능을 실행한다
  if(lbtn_chk == 1){
    analogWrite(mt_speed, 205);
    digitalWrite(mt, HIGH);
    strcpy(level, "LOW");
  }else if(lbtn_chk == 2){
    analogWrite(mt_speed, 230);
    digitalWrite(mt, HIGH);
    strcpy(level, "MID");
  }else if(lbtn_chk == 3){
    analogWrite(mt_speed, 255);
    digitalWrite(mt, HIGH);
    strcpy(level, "HIGH");
  }   
}

///////////////////////////////////////////////////////  타이머 관련

void timer_chg(){   // 타이머 버튼의 변화를 읽어와 상태를 변환한다.
  tbtn = digitalRead(btn_time);
  if(tbtn == LOW && bf_tbtn == HIGH){
    tbtn_chk += 1;
    if(tbtn_chk > 3){
       tbtn_chk = 1;
    }
  }
  bf_tbtn = tbtn; 
}

void time_off(){    // 타이머의 시간이 다되었을 때 초기화를 도와준다
  analogWrite(mt_speed, 0);
  digitalWrite(mt, LOW);
  pbtn_chk = 1;
  mbtn_chk = 1;
  timer0_millis = 0;
}

void timer_cnt(){   // 타이머의 상태를 알려주고, 기능을 실행한다
  if(tbtn_chk == 1){
    timer0_millis = 0;
    strcpy(timer_n, "INF");
  }else if(tbtn_chk == 2){
    if(millis() >= c_time1){
      time_off();
    }
    strcpy(timer_n, "30s");    
  }else if(tbtn_chk == 3){
    if(millis() >= c_time2){
      time_off();
    }
    strcpy(timer_n, "60s");
  }
}

/////////////////////////////////////////////////////// 리모컨 관련

void remote_chg(){ // 리모컨의 변화를 읽어와 상태를 변환한다.
  if(irrecv.decode(&results)){
    if(results.value == 0xFFA25D){ // 전원 버튼
        pbtn_chk *= -1;   
    }else if(results.value == 0xFF629D && pbtn_chk == -1){ // 모드 버튼
      mbtn_chk *= -1;  
    }else if(results.value == 0xFF906F && pbtn_chk == -1 && mbtn_chk == 1){ // + 버튼
      lbtn_chk += 1;
      if(lbtn_chk > 3){
       lbtn_chk = 3;
      }           
    }else if(results.value == 0xFFA857 && pbtn_chk == -1 && mbtn_chk == 1){ // - 버튼
      lbtn_chk -= 1;
      if(lbtn_chk < 1){
       lbtn_chk = 1;
      }      
    }else if(results.value == 0xFF30CF && pbtn_chk == -1){ // 1 버튼
      tbtn_chk = 1;
    }else if(results.value == 0xFF18E7 && pbtn_chk == -1){ // 2 버튼
      tbtn_chk = 2;
    }else if(results.value == 0xFF7A85 && pbtn_chk == -1){ // 3 버튼
      tbtn_chk = 3;
    }
    irrecv.resume();
  }
}

/////////////////////////////////////////////////////// 

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(mt, OUTPUT);
  pinMode(btn_power, INPUT_PULLUP);
  pinMode(btn_mode, INPUT_PULLUP);
  pinMode(btn_level, INPUT_PULLUP);
  pinMode(btn_time, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
}

void loop(){
  pbtn_chg();
  remote_chg();
    
  lcd.setCursor(1,0);
  lcd.print("P-");    // 파워
  if(pbtn_chk == 1){
    analogWrite(mt_speed, 0);
    digitalWrite(mt, LOW);
    strcpy(power, "OFF");
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("                ");
      
  }else if(pbtn_chk == -1){
    mbtn_chg();
    timer_chg();   
    
    Serial.println(tbtn_chk);
    strcpy(power, "ON");

    lcd.setCursor(1,1);
    lcd.print("M-");  // 모드

    lcd.setCursor(10,0);
    lcd.print("L-");  // 레벨

    lcd.setCursor(10,1);
    lcd.print("T-");  // 타이머

    timer_cnt();
    
    if(mbtn_chk == 1){
      level_chg_btn();
      strcpy(mode, "Control");
      
      level_cnt();
      
      lcd.setCursor(12, 0);
      lcd.print("    ");
      lcd.setCursor(12, 0);
      lcd.print(level);
          
    }else if(mbtn_chk == -1){  
      int i = 0;
      
      strcpy(mode, "Auto");
      strcpy(level, "Auto");
      
      for(i = 155; i < 255; i++){
        analogWrite(mt_speed, i);
        digitalWrite(mt, HIGH);
      }
      delay(100);
      for(i = 255; i > 155; i--){
        analogWrite(mt_speed, i);
        digitalWrite(mt, HIGH);
      }
      delay(100);
      
      timer_cnt();
      
      lcd.setCursor(12, 0);
      lcd.print("    ");
      lcd.setCursor(12, 0);
      lcd.print(level);
    }
    lcd.setCursor(3,1);
    lcd.print("      ");    
    lcd.setCursor(3,1);
    lcd.print(mode);

    lcd.setCursor(12,1);
    lcd.print(timer_n);
  }
  lcd.setCursor(3,0);
  lcd.print("    ");
  lcd.setCursor(3,0);
  lcd.print(power);   
}

// https://www.youtube.com/watch?v=kcLsvOhKnb0
