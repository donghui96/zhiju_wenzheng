/**
************************************************************
* @file         Gizwits + ArduinoUnoR3 Library 
* @brief        Datapoints handle , Gizwits Process
* @author       Gizwits
* @date         2017-08-01
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#include <Gizwits.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2, A3); // A2 -> RX, A3 -> TX

Gizwits myGizwits;

/**
* Serial Init , Gizwits Init  
* @param none
* @return none
*/
void setup() {
  // put your setup code here, to run once:

  mySerial.begin(115200);

  myGizwits.begin();

  mySerial.println("GoKit init  OK \n");

}

/**
* Arduino loop 
* @param none
* @return none
*/
void loop() {  
  
  //Configure network
  //if(XXX) //Trigger Condition
  //myGizwits.setBindMode(0x02);  //0x01:Enter AP Mode;0x02:Enter Airlink Mode
    /*
  unsigned long varW_wendu = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_wendu, varW_wendu);
  unsigned long varW_shidu = 0;//Add Sensor Data Collection
  myGizwits.write(VALUE_shidu, varW_shidu);


  bool varR_door = 0;
  if(myGizwits.hasBeenSet(EVENT_door))
  {
    myGizwits.read(EVENT_door,&varR_door);//Address for storing data
    mySerial.println(F("EVENT_door"));
    mySerial.println(varR_door,DEC);

  }
  bool varR_chuang = 0;
  if(myGizwits.hasBeenSet(EVENT_chuang))
  {
    myGizwits.read(EVENT_chuang,&varR_chuang);//Address for storing data
    mySerial.println(F("EVENT_chuang"));
    mySerial.println(varR_chuang,DEC);

  }
  bool varR_led_1 = 0;
  if(myGizwits.hasBeenSet(EVENT_led_1))
  {
    myGizwits.read(EVENT_led_1,&varR_led_1);//Address for storing data
    mySerial.println(F("EVENT_led_1"));
    mySerial.println(varR_led_1,DEC);

  }
  bool varR_led_2 = 0;
  if(myGizwits.hasBeenSet(EVENT_led_2))
  {
    myGizwits.read(EVENT_led_2,&varR_led_2);//Address for storing data
    mySerial.println(F("EVENT_led_2"));
    mySerial.println(varR_led_2,DEC);

  }
  bool varR_reshuiqi = 0;
  if(myGizwits.hasBeenSet(EVENT_reshuiqi))
  {
    myGizwits.read(EVENT_reshuiqi,&varR_reshuiqi);//Address for storing data
    mySerial.println(F("EVENT_reshuiqi"));
    mySerial.println(varR_reshuiqi,DEC);

  }
  bool varR_jinghuaqi = 0;
  if(myGizwits.hasBeenSet(EVENT_jinghuaqi))
  {
    myGizwits.read(EVENT_jinghuaqi,&varR_jinghuaqi);//Address for storing data
    mySerial.println(F("EVENT_jinghuaqi"));
    mySerial.println(varR_jinghuaqi,DEC);

  }
  unsigned long varR_tiaoguang = 0;
  if(myGizwits.hasBeenSet(EVENT_tiaoguang))
  {
    myGizwits.read(EVENT_tiaoguang,&varR_tiaoguang);//Address for storing data
    mySerial.println(F("EVENT_tiaoguang"));
    mySerial.println(varR_tiaoguang,DEC);

  }


  //binary datapoint handle
  */

  myGizwits.process();
}