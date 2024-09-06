#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6sURy_38U"
#define BLYNK_TEMPLATE_NAME "doan"
#define BLYNK_AUTH_TOKEN "qZZoYhsEUiF1PX2NRotgnySjzZF8BIa1"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "qZZoYhsEUiF1PX2NRotgnySjzZF8BIa1";
char ssid[] = "iphoneXr";
char pass[] = "07082000";

#include <DHT.h>
#define DHTPIN 23     // DHT Pin
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

int   pir=34; //PIR Pin
int   lua=16; //Flame Sensor Pin
int   mq2=17; //CO sensor Pin

int   nn1=15; //living room button
int   nn2=5; //garage button
int   nn3=0; //kitchen button
int   nnq=4; //fan button
int   nnsv=18; //garage door button

int   loa=19; //buzz pin
int   quat=12; //fan pin

int   sv1=26; 
int   sv2=27;

int   led1=33; //living room light
int   led2=25; //garage light
int   led3=32; //kitchen light
int   ledvs=14; //restroom light

long    nd,da; //temp,humid
long    adc,i,gtpir; 

long  chay,k;

//device status
int   ttled1,ttled2,ttled3,ttsv,ttq; 
int   ttvs,chayvs; 
int   ttsim,chaysim;

//Blynk config
WidgetLED ttloa(V7);

BLYNK_CONNECTED() 
{
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  ttled1 = param.asInt();
}

BLYNK_WRITE(V1)
{
  ttled2 = param.asInt();
}

BLYNK_WRITE(V2)         // bep
{
  ttled3 = param.asInt();
}

BLYNK_WRITE(V3)
{
  ttsv = param.asInt();
  if(ttsv==1)  
  {
    mo_cua1();
    mo_cua2();
  }
  else
  {
    dong_cua1();
    dong_cua2();
  } 
}

BLYNK_WRITE(V4)
{
  if((digitalRead(mq2)==0)||(digitalRead(lua)==0))
  {
    
  }
  else
  {
    ttq = param.asInt();
  }
}

void setup() 
{
  Serial.begin (9600);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(ledvs,OUTPUT);
  pinMode(quat,OUTPUT);
  pinMode(sv1,OUTPUT);
  pinMode(sv2,OUTPUT);

  pinMode(loa,OUTPUT);

  pinMode(pir,INPUT);
  pinMode(lua,INPUT);
  pinMode(mq2,INPUT);

  pinMode(nn1,INPUT);
  pinMode(nn2,INPUT);
  pinMode(nn3,INPUT);
  pinMode(nnq,INPUT);
  pinMode(nnsv,INPUT);

  digitalWrite(led1,0);
  digitalWrite(led2,0);
  digitalWrite(led3,0);
  digitalWrite(ledvs,0);
  digitalWrite(quat,0);
  digitalWrite(loa,0);
  dong_cua1();
  dong_cua2();
  dht.begin();
  
  lcd.init();     // khoi tao lcd
  lcd.backlight();

  lcd.setCursor(0,0);       
  lcd.print("ID  : iphoneXr     "); 
  lcd.setCursor(0,1);       
  lcd.print("PASS: 07082000 "); 

  Blynk.begin(auth, ssid, pass);
  
  //Module sim config
  delay(10000);
  Serial.println("AT+CMGF=1\r\n");  //set text mode sms
  delay(100);
  Serial.println("AT+CNMI=2,2,0,0,0\r\n"); //set SMS noti mode
  delay(1000);
  Serial.println("AT+CMGS=\"+84932774361\"\r\n"); //send mess to phone number
  delay(1000);
  Serial.println("SYSTEM ACTIVATES");
  delay(1000);
  Serial.println((char)26);
  delay(1000);
  ttled1=0;
  ttled2=0;
  ttled3=0;
  ttsv=0;
  chay=0;
  ttvs=0;
  chayvs=0;
  ttsim=0;
  chaysim=0;
  ttq=0;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("HOME SYSTEM");
  Blynk.virtualWrite(V0,0); 
  Blynk.virtualWrite(V1,0); 
  Blynk.virtualWrite(V2,0); 
  Blynk.virtualWrite(V3,0); 
  Blynk.virtualWrite(V4,0); 
  ttloa.off();
}

void loop()
{
  Blynk.run();
  doc_pir();
  ktra_nn1();
  ktra_nn2();
  ktra_nn3();
  ktra_nnq();
  ktra_nnsv();
  kich_tb();
  chay++;
  //Serial.println(chay);
  if(chay>10)
  {
    chay=0;
    da = dht.readHumidity();
    nd = dht.readTemperature(); 
    Blynk.virtualWrite(V5,nd);  
    Blynk.virtualWrite(V6,da); 
    Blynk.virtualWrite(V4,ttq); 
    hienthi();
  }

  if(gtpir>500)
  {
    ttvs=1;
    chayvs=0;
  }

  if(ttvs==1)
  {
    chayvs++;
    if(chayvs<30)
    {
      digitalWrite(ledvs,1);   
    }
    else
    {
      chayvs=0;
      ttvs=0;
      digitalWrite(ledvs,0);  
    }
  }
  else
  {
    chayvs=0;
    digitalWrite(ledvs,0);  
  }

  if(ttsim==0)
  {
    if((digitalRead(mq2)==0)||(digitalRead(lua)==0))
    {
      ttsim=1;
      //ttq=1;
      chaysim=0;
      ttloa.on();
      digitalWrite(loa,1);
      //digitalWrite(quat,1);
      lcd.setCursor(0,1);
      lcd.print("FIRE ALARM");
      goi();
    }
    else
    {
      ktra_nnq();
      if(ttq==1)        digitalWrite(quat,1);
      else              digitalWrite(quat,0);
    }
  }
  else if(ttsim==1)
  {
    chaysim++;
    //Serial.println(chaysim);
    if(chaysim<100)
    {

    }
    else if(chaysim==100)
    {
      ket_thuc();
      //sendSMS();
    }
    else
    {
      chaysim=200;
      if((digitalRead(mq2)==0)||(digitalRead(lua)==0))
      {
        
      }
      else
      {
        ttsim=0;
        ttq=0;
        ttloa.off();
        digitalWrite(loa,0);
        lcd.setCursor(0,1);
        lcd.print("HOME SYSTEM");
      }
    }
  }

   if(nd>24)
   {
     ttq=0;
     digitalWrite(quat,1);
   }
   else
   {
     ktra_nnq();
     if(ttq==1)        digitalWrite(quat,1);
     else              digitalWrite(quat,0);
   }
} 

void mo_cua1()
{
  for(k=0;k<5;k++)
  {
    digitalWrite(sv1,1);     delay(1);
    digitalWrite(sv1,0);     delay(19);     
  }         
}

void dong_cua1()
{
  for(k=0;k<50;k++)
  {
    digitalWrite(sv1,1);     delay(2);
    digitalWrite(sv1,0);     delay(18);     
  }  
}

void mo_cua2()
{
  for(k=0;k<5;k++)
  {
    digitalWrite(sv2,1);     delay(1);
    digitalWrite(sv2,0);     delay(19);     
  }         
}

void dong_cua2()
{
  for(k=0;k<50;k++)
  {
    digitalWrite(sv2,1);     delay(2);
    digitalWrite(sv2,0);     delay(18);     
  }  
}

void sendSMS()
{
  Serial.println("AT+CMGF=1");
  delay(1000);
  Serial.println("AT+CMGS=\"+84932774361\"\r\n");        //Send message to phone number   
  delay(1000);
  Serial.println("FIRE ALARM");
  delay(1000);
  Serial.println((char)26);
  delay(1000);
}

void xoa_tin()
{
  Serial.print("AT+CMGF=1\r"); 
  delay(100);
  Serial.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);  
}

void goi()
{
  Serial.print("ATD+84932774361");// Call phone number
  Serial.println(";"); 
}

void ket_thuc()
{
  Serial.println("ATH");   // End call. 
}

void doc_pir()
{
  adc=0;
  for(i=0;i<100;i++)
  {
    adc=adc+analogRead(pir);
    delay(1);
  }
  adc=adc*5000/4095;
  gtpir=adc;
}

void kich_tb()
{
  if(ttled1==1)
  {
    digitalWrite(led1,1);    
  }
  else
  {
    digitalWrite(led1,0);    
  }

  if(ttled2==1)
  {
    digitalWrite(led2,1);    
  }
  else
  {
    digitalWrite(led2,0);    
  }

  if(ttled3==1)
  {
    digitalWrite(led3,1);    
  }
  else
  {
    digitalWrite(led3,0);    
  }
}

void hienthi()
{
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.write(nd/10%10+48);
  lcd.write(nd%10+0x30);
  lcd.write(0xdf);
  lcd.print("C   ");

  lcd.setCursor(10,0);
  lcd.print("H:");
  lcd.write(da/10%10+48);
  lcd.write(da%10+0x30);
  lcd.print("%"); 
}

void ktra_nn1()
{
  if(digitalRead(nn1)==0)
  {
    delay(20);
    if(digitalRead(nn1)==0)
    {
      ttled1++;
      if(ttled1>1)        ttled1=0;
      Blynk.virtualWrite(V0,ttled1);                 
      kich_tb();
      delay(1000);
      while(digitalRead(nn1)==0);
    }
  }
}

void ktra_nn2()
{
  if(digitalRead(nn2)==0)
  {
    delay(20);
    if(digitalRead(nn2)==0)
    {
      ttled2++;
      if(ttled2>1)        ttled2=0;
      Blynk.virtualWrite(V1,ttled2);  
      kich_tb();
      delay(1000);
      while(digitalRead(nn2)==0);
    }
  }
}

void ktra_nn3()
{
  if(digitalRead(nn3)==0)
  {
    delay(20);
    if(digitalRead(nn3)==0)
    {
      ttled3++;
      if(ttled3>1)        ttled3=0;
      Blynk.virtualWrite(V2,ttled3);  
      kich_tb();
      while(digitalRead(nn3)==0);
    }
  }
}

void ktra_nnq()
{
  if(digitalRead(nnq)==0)
  {
    delay(20);
    if(digitalRead(nnq)==0)
    {
      ttq++;
      if(ttq>1)        ttq=0;
      Blynk.virtualWrite(V4,ttq);  
      delay(1000);
      while(digitalRead(nn3)==0);
    }
  }
}

void ktra_nnsv()
{
  if(digitalRead(nnsv)==0)
  {
    delay(20);
    if(digitalRead(nnsv)==0)
    {
      ttsv++;
      if(ttsv>1)        ttsv=0;
      Blynk.virtualWrite(V4,ttsv);  
      kich_tb();
      if(ttsv==1)  
      {
        mo_cua1();
        mo_cua2();
      }
      else
      {
        dong_cua1();
        dong_cua2();
      } 
      delay(1000);
      while(digitalRead(nnsv)==0);
    }
  }
}