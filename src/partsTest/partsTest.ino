#include <TMRpcm.h>
#include <SPI.h>
#include <SD.h>

const int MIC = 0; //the microphone amplifier output is connected to pin A0
const int sampleRate = 16000;
int adc = 0;
int dB, PdB = 0; //the variable that will hold the value read from the microphone each time
int seconds = 0;

File myFile;
TMRpcm audio;

void setup() {
  Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
  while (!Serial) {
    ;
  }
  
  pinMode(MIC, INPUT);
   if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  
  Serial.println("initialization done.");
  myFile = SD.open("data.txt", FILE_WRITE);
}

void loop() {
  measure();
  myFile.println(dB);
  if (seconds > 20) {
    myFile.close();
    Serial.println("Exiting!");
    exit(0);
  }
  delay(100);
}

void measure() {
  PdB = dB; //Store the previous of dB here
  
  adc= analogRead(MIC); //Read the ADC value from amplifer 
  Serial.println (adc); 
  dB = (adc+83.2073) / 11.003; //Default values, calibrate using linear regression
  seconds = millis()/1000;
  //Serial.println(dB);
}
