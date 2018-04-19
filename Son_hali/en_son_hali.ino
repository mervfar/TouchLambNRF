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
int control=0;

int PIN_RED = 6;//red rgb
int PIN_GREEN = 5;//green rgb
int PIN_BLUE = 3;//blue rgb
int counter = 0;
int numColors = 255;
int animationDelay = 10;
void setup()                    
{
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
 pinMode(PIN_RED, OUTPUT);
 pinMode(PIN_BLUE, OUTPUT);
 pinMode(PIN_GREEN, OUTPUT);
   ///radyo ayarları

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
////rgb///////////////
   float colorNumber = counter > numColors ? counter - numColors: counter;
 float saturation = 1; // Between 0 and 1 (0 = gray, 1 = full color)
 float brightness = .5; // Between 0 and 1 (0 = dark, 1 is full brightness)
 float hue = (colorNumber / float(numColors)) * 360; // Number between 0 and 360
 long color = HSBtoRGB(hue, saturation, brightness); 
 
 // Get the red, blue and green parts from generated color
 int red = color >> 16 & 255;
 int green = color >> 8 & 255;
 int blue = color & 255;
setColor(red, green, blue);
//Triggered();

counter = (counter + 1) % (numColors * 2);

 delay(animationDelay);
 
   ///////////////
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
    while (nrf24.waitAvailableTimeout(10) && nrf24.recv(buf, &len))
    {   
        //something was received, turn the right LED on
       
       Swap();
       Triggered();//control=1;//kontrol değişkeni 1
       // delay(10);
    }      
    
   void Swap();//control=0;//kontrol değişkeni 0
  }
  delay(10);
}


void setColor (unsigned char red, unsigned char green, unsigned char blue) 
{        if(control==0){
   analogWrite(PIN_RED, red);
   analogWrite(PIN_GREEN, green);
   analogWrite(PIN_BLUE, blue);
} }

long HSBtoRGB(float _hue, float _sat, float _brightness) {
   float red = 0.0;
   float green = 0.0;
   float blue = 0.0;
   
   if (_sat == 0.0) {
       red = _brightness;
       green = _brightness;
       blue = _brightness;
   } else {
       if (_hue == 360.0) {
           _hue = 0;
       }

       int slice = _hue / 60.0;
       float hue_frac = (_hue / 60.0) - slice;

       float aa = _brightness * (1.0 - _sat);
       float bb = _brightness * (1.0 - _sat * hue_frac);
       float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));
       
       switch(slice) {
           case 0:
               red = _brightness;
               green = cc;
               blue = aa;
               break;
           case 1:
               red = bb;
               green = _brightness;
               blue = aa;
               break;
           case 2:
               red = aa;
               green = _brightness;
               blue = cc;
               break;
           case 3:
               red = aa;
               green = bb;
               blue = _brightness;
               break;
           case 4:
               red = cc;
               green = aa;
               blue = _brightness;
               break;
           case 5:
               red = _brightness;
               green = aa;
               blue = bb;
               break;
           default:
               red = 0.0;
               green = 0.0;
               blue = 0.0;
               break;
       }
   }

   long ired = red * 255.0;
   long igreen = green * 255.0;
   long iblue = blue * 255.0;
   
   return long((ired << 16) | (igreen << 8) | (iblue));
}
//////////////////////////

void Triggered() {
 if(control==1){
  Trig_setColor(255, 0, 0); // Red Color
  delay(250);
  Trig_setColor(0, 255, 0); // Green Color
  delay(250);
  Trig_setColor(0, 0, 255); // Blue Color
  delay(250);
  Trig_setColor(255, 255, 255); // White Color
  delay(250);
  Trig_setColor(170, 0, 255); // Purple Color
  delay(250);
  control=0;
}
}
void Trig_setColor(int redValue, int greenValue, int blueValue) {
  
  analogWrite(PIN_RED, redValue);
  analogWrite(PIN_GREEN, greenValue);
  analogWrite(PIN_BLUE, blueValue);

}
void Swap(){
  if(control==0){
    control=1;
  }else{
    control=0;
  }
}

