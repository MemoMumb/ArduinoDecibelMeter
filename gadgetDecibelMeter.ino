#include <TMRpcm.h>
#include <SPI.h>
#include <SD.h>

const int MIC = 0; //the microphone amplifier output is connected to pin A0
const int sampleRate = 16000;
int adc;
int dB, PdB; //the variable that will hold the value read from the microphone each time
int seconds;
int readCount = 0;

char filePrefixName[50] = "sound";
char fileSuffixName[15] = "";
char header[10] = ".wav";
bool isRecording = false;

#define SD_CSPin 10

File myFile;
TMRpcm audio;

void setup() {
  Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
  pinMode(MIC, INPUT);

  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occured while mounting SD");
  }

  while(1) {
    Serial.println("initialization done.");
  }

  myFile = SD.open("data.txt", FILE_WRITE);
}

void loop() {
  PdB = dB; //Store the previous of dB here
  
  adc= analogRead(MIC); //Read the ADC value from amplifer 
  //Serial.println (adc); //Print ADC for initial calculation 
  dB = (adc+83.2073) / 11.003; //Default values, calibrate using linear regression
  readCount++;
  seconds = millis()/1000;
  //Serial.println(dB);
  if (readCount = 10) {
    myFile.println(dB, seconds);
    readCount = 0;
  }
  
  
  if (dB <= 75) {
    delay(100);
  }
  
  else {
    isRecording = true;
    seconds = round(millis()/1000);
    itoa(seconds, fileSuffixName, 10);

    audio.startRecording(strcat(strcat(filePrefixName, fileSuffixName), header), sampleRate, MIC);
    while (isRecording) {
      PdB = dB;
      adc = analogRead(MIC);
      dB = (adc+83.2073) / 11.003; // Do regression values
      readCount++;
      
      seconds = round(millis()/1000);
      if (readCount = 10) { 
        myFile.println(dB, seconds); 
        readCount = 0;
      }
      
      if (dB > 75) {
        isRecording = false;
        break;
      }
    }

    audio.stopRecording(strcat(strcat(filePrefixName, fileSuffixName), header));
    delay(100);
  }
}
