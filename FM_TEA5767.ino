
#ifdef __AVR_ATtiny85__
#include <TinyWireM.h> 
#else
#include <Wire.h>
#endif

const int potPin = 3;
const int numChannel=23;

double channel_freq[]={89.3,89.7,90.1,90.5,90.9,
                    91.3,91.7,92.1,93.1,93.7,
                    94.3,96.3,97.3,98.1,98.9,
                    99.7,100.7,101.7,103.3,104.1,
                    104.9,106.5,107.7};

unsigned char frequencyH = 0;
unsigned char frequencyL = 0;

unsigned int frequencyB;
double frequency = 0;
double pre_frequency=0;

int pre_channel=0;
int channel=0;


int pre_reading=0;
int reading_thres=20;



void setup()
{
  
#ifdef __AVR_ATtiny85__//
  TinyWireM.begin();
#else
  Wire.begin();
#endif
  pre_frequency=frequency = 83.4; //starting frequency
  setFrequency();
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
#endif
}

void loop()
{
  int reading = analogRead(potPin);
#ifndef __AVR_ATtiny85__
  //Serial.println(reading);
#endif
  if(abs(pre_reading-reading)>reading_thres){ 
 
//    frequency = ((double)reading * (108.0 - 83.4)) / 1024.0 + 83.4;
//    frequency = ((int)(frequency * 10)) / 10.0;
    channel=reading/(1024.0/numChannel);
    frequency=channel_freq[channel];

    if(pre_channel!=channel){
    //if(pre_frequency!=frequency){
      setFrequency();      
    }
     pre_reading=reading;
  } 
 
  
}
/////////////
void setFrequency(){

  digitalWrite(4,HIGH);
  pre_frequency=frequency;
  pre_channel=channel;
  
  frequencyB = 4 * (frequency * 1000000 + 225000) / 32768;
  frequencyH = frequencyB >> 8;
  frequencyL = frequencyB & 0XFF;
  delay(100);
  
#ifdef __AVR_ATtiny85__
  TinyWireM.beginTransmission(0x60);
  TinyWireM.send(frequencyH);
  TinyWireM.send(frequencyL);
  TinyWireM.send(0xB0);
  TinyWireM.send(0x10);
  TinyWireM.send((byte)0x00);
  TinyWireM.endTransmission();
#else
  Serial.println(frequency);
  Wire.beginTransmission(0x60);
  Wire.write(frequencyH);
  Wire.write(frequencyL);
  Wire.write(0xB0);
  Wire.write(0x10);
  Wire.write((byte)0x00);
  Wire.endTransmission();
#endif
  delay(100); 
  digitalWrite(4,LOW);
  
  
}
