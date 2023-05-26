#include <Keypad.h>
#include<LiquidCrystal.h>  
#include<EEPROM.h>    
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);   
char password[4];  
char pass[4], pass1[4];  
int i=0;  
int t;
int unlockMode = 0;
int isPasswordSet = 0;
const unsigned long HOLD_TIME = 2000;
bool buttonHeld = false;
unsigned long holdStartTime;

char customKey = 0;  
const byte ROWS = 4;   
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = 
{  
  {'1','2','3','A'},  
  {'4','5','6','B'},  
  {'7','8','9','C'},  
  {'*','0','#','D'}  
};  
byte rowPins[ROWS] = {2, 3, 4, 5};  
byte colPins[COLS] = {6, 7, 8, 9};   
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);   

void de()
{
  pinMode(13, OUTPUT);
  int ll;
  ll = analogRead(A6);
  if(ll>=600)
    {digitalWrite(13, HIGH);}
  else
    {digitalWrite(13, LOW);}
}
void delow()
{
  pinMode(13, LOW);
  int ll;
  ll = analogRead(A6);
  if(ll>=600)
    {digitalWrite(13, LOW);}
  else
    {digitalWrite(13, LOW);}
    setup();
}
void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);  
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(12, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(0, LOW);
  lcd.print("PLEASE WAIT");  
  lcd.setCursor(0, 1);   
  delay(1000);  
  lcd.clear();  
  lcd.print("INITIAL PASSKEY:");  
  lcd.setCursor(0, 1);
  int t=0;  
  while(t<4)  
  {
    char key = customKeypad.getKey();  
    if(key)  
    { 
      pass[t] = key;  
      lcd.print(key);  
      EEPROM.write(t, key);  
      t++;   
    }  
  }
  lcd.clear();
  lcd.print("PASSWORD SET");
  beep();  
  delay(1000);
  lcd.clear();  
  lcd.print("ENTER PASSKEY:");  
  lcd.setCursor(0, 1);  
  customKey=0;
  }
void loop(){
  customKey = customKeypad.getKey();  
  if (unlockMode){
  de();
  }
  if(!isPasswordSet){
    if(customKey)  
    {  
      password[i++] = customKey;  
      lcd.print(customKey);    
    }  
    if(i==4)  
    { 
        for(int j=0; j<4; j++)  
        pass[j]=EEPROM.read(j);  
        if(!(strncmp(password, pass, 4)))  
      {   
        unlockMode = 1;
        beepCorrectPass();  
        lcd.clear();  
        lcd.print("ACCEPTED, OPENED");  
        lcd.setCursor(0, 1);  
        lcd.print("D.CHANGE PASSKEY"); 
        isPasswordSet = 1;  
      }
        else {
        unlockMode = 0;
        beepWrongPass();  
        lcd.clear();  
        lcd.print("DENIED");  
        lcd.setCursor(0, 1);  
        lcd.print("WRONG PASSWORD");
        delay(1000);
        lcd.clear();  
        lcd.print("ENTER PASSKEY:");  
        lcd.setCursor(0, 1);
        i=0;
        }
    } 
  }
    if(customKey == 'D')  
  {  
    buttonHeld = true;
    holdStartTime = millis();
    
  } 
    if(customKey == 'A')  
  {  
    unlockMode =0;
    delow();
  } 
    if (buttonHeld && millis() - holdStartTime >= HOLD_TIME) {
    lcd.setCursor(0, 1);
    lcd.print("CHANGE PASSKEY");
    delay(1000);
    changePassword();
    buttonHeld = false;
  }
}


void changePassword()  
{
  lcd.clear();
  lcd.print("ACCESS");
  lcd.setCursor(0, 1);  
  lcd.print("CHANGE PASSKEY");
  delay(1000);
  int j=0;  
  lcd.clear();  
  lcd.print("CURRENT PASSKEY:");  
  lcd.setCursor(0, 1);  
  while(j<4)  
  {  
    de();
    char key = customKeypad.getKey();  
    if(key)  
    {  
      pass1[j++]=key;  
      lcd.print(key);  
    }  
    key=0;  
  }
  if((strncmp(pass1, pass, 4)))  
  {  
    de();
    lcd.clear();  
    lcd.print("WRONG PASSKEY");  
    lcd.setCursor(0, 1);  
    lcd.print("TRY AGAIN"); 
    beepWrongPass();
  }  
  else  
  {  
    lcd.print(" OK");
    j=0;
    beepCorrectPass(); 
    delay(1000); 
    lcd.clear();  
    lcd.print("NEW PASSKEY:");  
    lcd.setCursor(0, 1);  
    while(j<4)  
    {
      char key = customKeypad.getKey();  
      if(key)  
      {  
        pass[j] = key;  
        lcd.print(key);  
        EEPROM.write(j, key);  
        j++;   
      }  
    }
    lcd.print(" DONE");
    beep();  
    delay(1000);  
  }
  lcd.clear();  
  lcd.print("ENTER PASSKEY:");  
  lcd.setCursor(0, 1);  
  customKey=0;  
  isPasswordSet = 0;
  for (int i = 0; i < 4; i++)
  {
    password[i] = 0;
  }
}  


void beep()  
{  
  digitalWrite(10, HIGH);  
  delay(200);  
  digitalWrite(10, LOW);  
} 

void beepWrongPass()  
{  
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW);
  delay(100);   
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW); 
  delay(100);   
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW);
  delay(100);   
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW);
}

 void beepCorrectPass()  
{  
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW);
  delay(100);   
  digitalWrite(10, HIGH);  
  delay(100);  
  digitalWrite(10, LOW); 
} 


