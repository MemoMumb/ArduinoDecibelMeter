#include <SPI.h>
#include <SD.h>

const int MIC = 0; //the microphone amplifier output is connected to pin A0
int adc = 0;
int seconds = 0;

double measureFor = 43200; //Overall sample time duration
int sampleDur = 200; //Set sampling duration as 200 ms
int maxADC = 0;
unsigned long start;
unsigned long lastOpen; 

bool writeTo = true;

File myFile;

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
  lastOpen = millis();
}

void loop() {
  start = millis();
  measure();
  if (writeTo) {
    myFile.print(maxADC);
    myFile.print(", time: ");
    myFile.println(millis());
  }

  maxADC = 0;
if (millis() - lastOpen >= 600000) { //Every 10 minutes, save to microSD card
    myFile.close();
    myFile = SD.open("data.txt", FILE_WRITE);
    lastOpen = millis();
    Serial.println("Clock reset!");
}
  
  if (seconds > measureFor) {
    myFile.close();
    Serial.println("Exiting!");
    exit(0);
  }
}

void measure() {

  while (start - millis() < sampleDur) {
    adc = analogRead(MIC) - 332; //Read the ADC value from amplifer
    if (abs(adc) > maxADC) {
      maxADC = abs(adc);
    }

  }
  
  seconds = millis()/1000;

  Serial.println(maxADC);
  delay(100);
}
