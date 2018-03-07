#include <CapacitiveSensor.h>
#include <SPI.h>
#include <RH_NRF24.h>
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */
 /*######################################################################################
  * ARDUINO PRO MINI 5V 
  * 
  * Radio Setup Pin
  * CE:8
  * CSN:10
  * SCK:13
  * MOSI:11
  * MISO:12
  * 
  * Touch Sensor Pin
  * pin4-pin2 (pin2 is sensor pin)
  * 
  * RGB LED Pin
  * RED:
  * GREEN:
  * BLUE:
  * 
  * 
  * #####################################################################################
  */
RH_NRF24 nrf24;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
//CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
//CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
/////radyo kısmı


boolean state=false;
int redLed = 3;
int control=0;


void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
   pinMode(3,INPUT);
   ///radyo ayarları
 pinMode(redLed, OUTPUT);
  //pinMode(button, INPUT);

  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");    

   
}
void loop()
{
 long total1 =  cs_4_2.capacitiveSensor(80);
  Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
  
  
if(total1>35){
   // Send a message
    uint8_t data[] = "mesaj";
    nrf24.send(data, sizeof(data));
    nrf24.waitPacketSent();
   delay(10);
  } else {
    // Wait for a message
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    while (nrf24.waitAvailableTimeout(200) && nrf24.recv(buf, &len))
    {   
        //something was received, turn the right LED on
        digitalWrite(redLed, HIGH); 
        control=1;//kontrol değişkeni 1
        void rgb_func();   delay(100);  
    }      
    digitalWrite(redLed, LOW);
    control=0;//kontrol değişkeni 0
  }
  delay(10);
}

void rgb_func(){


Serial.print("rgb fonksiyon geldi");


}



