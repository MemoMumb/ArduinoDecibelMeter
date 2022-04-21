#include <TMRpcm.h>
#include <SPI.h>
#include <SD.h>

const int MIC = 0; //the microphone amplifier output is connected to pin A0
const int sampleRate = 16000;
int adc;
int dB, PdB; //the variable that will hold the value read from the microphone each time
int seconds;
int readCount = 0;

//measure for this amount of seconds
int measureFor = 30; 

char filePrefixName[50] = "sound";
char fileSuffixName[15] = "";
char header[10] = ".wav";
bool isRecording = false;
bool doRecording = false

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
  
  measure()
  if (dB <= 75) {
    delay(100);
  }
  
  if (dB > 75) { //Record from microphone if the decibel value is above 75
    if (doRecording) { 
      isRecording = true; 
      seconds = round(millis()/1000);
      itoa(seconds, fileSuffixName, 10);

      audio.startRecording(strcat(strcat(filePrefixName, fileSuffixName), header), sampleRate, MIC); //Start recording, the file name will be "Sound[seconds].wav"
      while (isRecording) { //constantly check if the decibel values are above 75
        measure()
        if (dB > 75) { //if decibel values are no longer above 75, stop recording
          isRecording = false;
          break;
        }
      }

      audio.stopRecording(strcat(strcat(filePrefixName, fileSuffixName), header));
      myFile.close()
      delay(100);
    }

    else {
      delay(100);
    }
  }

  if (seconds > measureFor) {
    myFile.close();
    Serial.println("Exiting!");
    exit(0)
  }
}

void measure() {
  PdB = dB;
  adc = analogRead(MIC);
  dB = (adc+83.2073) / 11.003; // Do regression values
  readCount++;
      
  seconds = round(millis()/1000);
  if (readCount = 10) { 
    myFile.println(dB); 
    readCount = 0;
}
