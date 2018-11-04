#include <Arduino.h>
#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <U8x8lib.h>
#include <DFPlayer_Mini_Mp3.h>//MP3头文件名
#include <SoftwareSerial.h>
int buttonpin=2;
int v;
int G=0;
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
SoftwareSerial mySerial(A2,A3); // A2 -> RX, A3 -> TX
Gizwits myGizwits;
#define   KEY1              6
#define   KEY2              7
#define   KEY1_SHORT_PRESS  1
#define   KEY1_LONG_PRESS   2
#define   KEY2_SHORT_PRESS  4
#define   KEY2_LONG_PRESS   8
#define   NO_KEY            0
#define   KEY_LONG_TIMER    3
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 
Servo myservo; 
int flag =0;
int pos;
unsigned long Last_KeyTime = 0;
unsigned long gokit_time_s(void)
{
  return millis() / 1000;
}
char gokit_key1down(void)//按键函数，不用管
{
  unsigned long keep_time = 0;
  if (digitalRead(KEY1) == LOW)
  {
    delay(100);
    if (digitalRead(KEY1) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY1) == LOW)
      {
        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY1_LONG_PRESS;
        }
      } //until open the key
      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY1_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}
char gokit_key2down(void)//按键函数不用管。
{
  int unsigned long keep_time = 0;
  if (digitalRead(KEY2) == LOW)
  {
    delay(100);
    if (digitalRead(KEY2) == LOW)
    {
      keep_time = gokit_time_s();
      while (digitalRead(KEY2) == LOW) //until open the key
      {
        if ((gokit_time_s() - keep_time) > KEY_LONG_TIMER)
        {
          Last_KeyTime = gokit_time_s();
          return KEY2_LONG_PRESS;
        }
      }
      if ((gokit_time_s() - Last_KeyTime) > KEY_LONG_TIMER)
      {
        return KEY2_SHORT_PRESS;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}
char gokit_keydown(void)
{
  char ret = 0;
  ret |= gokit_key2down();
  ret |= gokit_key1down();
  return ret;
}
void KEY_Handle(void)//这里是检测按键的函数，不用管
{
  switch (gokit_keydown())
  {
    case KEY1_SHORT_PRESS:
      myGizwits.setBindMode(WIFI_PRODUCTION_TEST);
      break;
    case KEY1_LONG_PRESS:
      myGizwits.setBindMode(WIFI_RESET_MODE);
      break;
    case KEY2_SHORT_PRESS:
      myGizwits.setBindMode(WIFI_SOFTAP_MODE);
      break;
    case KEY2_LONG_PRESS:
      myGizwits.setBindMode(WIFI_AIRLINK_MODE);//这里我自己加了一个如果开启了配网功能，蜂鸣器就响一秒。很好用哦
    digitalWrite(8,HIGH);
    delay(1000);
   digitalWrite(8,LOW);
      break;
    default:
      break;
  }
}
void wifiStatusHandle()//这个函数我做了修改，因为没什么用。
{
  if(myGizwits.wifiHasBeenSet(WIFI_SOFTAP))
  {
  }  
  if(myGizwits.wifiHasBeenSet(WIFI_AIRLINK))
  {
  } 
}
////////////////////////////////////////////////wenshidu 
double Fahrenheit(double celsius) 
{
        return 1.8 * celsius + 32;
}    //摄氏温度度转化为华氏温度

double Kelvin(double celsius)
{
        return celsius + 273.15;
}     //摄氏温度转化为开氏温度

// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

// 快速计算露点，速度是5倍dewPoint()
// 参考: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}
#include <dht11.h>

dht11 DHT11;

#define DHT11PIN A0
///////////////////////////////////////////////wenshidu
void setup() {
  // put your setup code here, to run once:
  int G=0;
   pinMode(buttonpin,INPUT);//定义避障传感器为输出接口
mp3_set_serial (mySerial);//定义MP3串口为Arduino串口。
        mp3_set_volume (20);//设置音量。
  mySerial.begin(9600);
  pinMode(KEY1, INPUT_PULLUP);
  pinMode(KEY2, INPUT_PULLUP);
  ////////////////////////////////////代码部署/////////////////////////////////
  
  
  pinMode(3,OUTPUT);//开关1
  pinMode(4,OUTPUT);//开关2
  pinMode(5,OUTPUT);//开关3
  pinMode(8,OUTPUT);//beeWIFI_AIRLINK成功报警提示这里是配网提示
  pinMode(9,OUTPUT);//开关4woshiwoshi
  pinMode(10,OUTPUT);//开关5
  pinMode(11,OUTPUT);//脉冲输出///keting
  pinMode(12,OUTPUT);//指示灯
  pinMode(13,OUTPUT);//开关6
  pinMode(A0,INPUT);//温湿度传感器
  pinMode(A1,INPUT);//烟雾报警器
  myservo.attach(3);
   myservo.write(0);  
  digitalWrite(9,LOW);
  myGizwits.begin();
  begin();
}
void begin ()
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(5,1,"Zhi-Ju");
  u8x8.drawString(0,2,"wendu:");
  u8x8.drawString(0,3,"shidu:");
  u8x8.drawString(0,4,"yanwu:");
  u8x8.drawString(0,5,"led:");
  }
void loop() {  
  KEY_Handle();//key handle , network configure网络配置
  wifiStatusHandle();//WIFI Status Handle无线网络状态处理
 /////////////////////////////////////////////////////////////
 int chk = DHT11.read(DHT11PIN);//温湿度必要代码
 /////////////////////////////////////////////////////////
 v=digitalRead(buttonpin);//将数字接口3的值读取赋给val

 if ((v==0)&&(G==0))//当避障传感器检测有信号时，LED 灭
{
 mp3_play (16);
 delay(2000);
 
}
 int val=analogRead(A1);
u8x8.setCursor(6,4);//这里设置显示的坐标
u8x8.print(val);//输出显示内容
 ////////////////////////////////////////////////////////
 if((float)DHT11.temperature<5)
 {
   
  mp3_play (1);//播放第一首。天冷嫁衣
  delay(2000);
  
  
  }
  if((float)DHT11.temperature>35)
 {
  mp3_play (2);//播放第一首。天冷嫁衣
  delay(2000);
  }
  ///////////////////////////////
   if(val>600)
 {
 
  mp3_play (15);//播放第一首。天冷嫁衣
  delay(2000);
   
  }
 
 unsigned long varW_wendu = (float)DHT11.temperature;
 u8x8.setCursor(6,2);//这里设置显示的坐标
u8x8.print(varW_wendu);//输出显示内容
  myGizwits.write(VALUE_wendu, varW_wendu);
  unsigned long varW_shidu = (float)DHT11.humidity ;
   u8x8.setCursor(6,3);//这里设置显示的坐标
u8x8.print(varW_shidu);//输出显示内容
  myGizwits.write(VALUE_shidu, varW_shidu);
  bool varR_door = 0;
  if(myGizwits.hasBeenSet(EVENT_door))
  {
    myGizwits.read(EVENT_door,&varR_door);//Address for storing data
if(varR_door==1)   // 这是对应手机APP第一个开关的控制程序
    {
      G=1;               
    myservo.write(90); 
    mp3_play (); 
     mp3_play (3);//播放第一首。天冷嫁衣
  delay(2000);// tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  
      }
     else
     {
      myservo.write(0); 
  G=0;
  
     mp3_play (4);//播放第一首。天冷嫁衣
  delay(2000);// tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
     
  }
  }
  bool varR_chuang = 0;
  if(myGizwits.hasBeenSet(EVENT_chuang))
  {
    myGizwits.read(EVENT_chuang,&varR_chuang);//Address for storing data
 if(varR_chuang==1)  
    {
      digitalWrite(4,HIGH);
       mp3_play (5);//播放第一首。天冷嫁衣
  delay(2000);
      }
     else{
     digitalWrite(4,LOW);
      mp3_play (6);//播放第一首。天冷嫁衣
  delay(2000);
  }
  }
  bool varR_led_1 = 0;
  if(myGizwits.hasBeenSet(EVENT_led_1))
  {
    myGizwits.read(EVENT_led_1,&varR_led_1);//Address for storing data
     if(varR_led_1==1)  
    {
      digitalWrite(11,HIGH);
      digitalWrite(5,HIGH);
       mp3_play (7);//播放第一首。天冷嫁衣
  delay(2000);
      }
     else
     {
     digitalWrite(11,LOW);
      digitalWrite(5,LOW);
       mp3_play (8);//播放第一首。天冷嫁衣
  delay(2000);
     }
  }
  bool varR_led_2 = 0;
  if(myGizwits.hasBeenSet(EVENT_led_2))
  {
    myGizwits.read(EVENT_led_2,&varR_led_2);//Address for storing data
   if(varR_led_2==1)  
    {
      digitalWrite(9,HIGH);
       mp3_play (9);//播放第一首。天冷嫁衣
  delay(2000);
      }
     else{
     digitalWrite(9,LOW);
      mp3_play (10);//播放第一首。天冷嫁衣
  delay(2000);
  }
  }
  bool varR_reshuiqi = 0;
  if(myGizwits.hasBeenSet(EVENT_reshuiqi))
  {
    myGizwits.read(EVENT_reshuiqi,&varR_reshuiqi);//Address for storing data
     if(varR_reshuiqi==1)  
    {
      digitalWrite(10,HIGH);
       mp3_play (11);//播放第一首。天冷嫁衣
  delay(2000);
      }
     else{
     digitalWrite(10,LOW);
      mp3_play (12);//播放第一首。天冷嫁衣
  delay(2000);
  }

  }
  bool varR_jinghuaqi = 0;
  if(myGizwits.hasBeenSet(EVENT_jinghuaqi))
  {
    myGizwits.read(EVENT_jinghuaqi,&varR_jinghuaqi);//Address for storing data
    if(varR_jinghuaqi==1)  
    {
      digitalWrite(13,HIGH);
       mp3_play (13);//播放第一首。天冷嫁衣
  delay(2000);
      }
     else{
     digitalWrite(13,LOW);
      mp3_play (14);//播放第一首。天冷嫁衣
  delay(2000);
  }
  }
  unsigned long varR_tiaoguang = 0;
  if(myGizwits.hasBeenSet(EVENT_tiaoguang))
  {
    myGizwits.read(EVENT_tiaoguang,&varR_tiaoguang);//Address for storing data
   analogWrite(11,varR_tiaoguang); 
      u8x8.setCursor(6,5);//这里设置显示的坐标
u8x8.print(varR_tiaoguang);//输出显示内容
  }
  myGizwits.process();
}
