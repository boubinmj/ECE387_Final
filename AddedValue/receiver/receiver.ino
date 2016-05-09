#include <SPI.h>
#include <SD.h>
#include <VirtualWire.h>

const int LED_PIN = 13;
const int YELLOW_PIN = 2;
const int RED_PIN = 3;

File recordFile;
int incomingByte = 0;
int doorState;
char sensorMsg[3];
int count = 0;
int doorCount = 0;

void decodeMsg(){
    char** sensorData_parsed;
    char* sensorData;  
    const char delim[2] = "/";

    
    
    int j = 1;
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    
    {
        int i;
        
        //Set LED to HIGH 
        //Indicates message received
        digitalWrite(LED_PIN, HIGH); 
        // Message with a good checksum received, dump it.
        
        for (i = 0; i < buflen; i++)
        {
            sensorMsg[i]= char(buf[i]);
            
        }

        //Add null terminator to char string
        //String must be terminated to enable use of atoi function
        sensorMsg[buflen] = '\0';

        //Serial.print("Sensor Data 1: ");
        //Serial.print(sensorMsg);
        //Serial.println("");

        //Convert String to Int Value

        sensorData = strtok(sensorMsg, delim);
        sensorData_parsed[0] = sensorData;

        //Serial.println("SensorData: ");
        //Serial.println(sensorData);
        

        while(sensorData != NULL){
           sensorData = strtok(NULL, delim);
           //Serial.println(sensorData);
           sensorData_parsed[j] = sensorData;
           j++;
        }

       doorState = atoi(sensorData_parsed[1]);
        
        //Set LED to LOW
        //indicating end of message transmission
        digitalWrite(LED_PIN, LOW);
    }
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    //return;
  }
  Serial.println("initialization done.");

  Serial.begin(9600); // Debugging only
    //Serial.println("setup");
    pinMode(LED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    // Initialise the IO and ISR
    vw_set_rx_pin(8);  //Changes rx pin on board
    //Default rx pin is I/O pin 11
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

/*Writes to a file if code door was just open*/
void writeFileYellow(){
      if (recordFile) {
      recordFile.println(doorCount + " just open");
      // close the file:
      recordFile.close();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening file for door open");
      }

      digitalWrite(YELLOW_PIN, HIGH);
      delay(100);
      digitalWrite(YELLOW_PIN, LOW);
}

/*Writes to file for state where door has been open*/
void writeFileRed(){
      if (recordFile) {
      recordFile.println(doorCount + " danger level");
      // close the file:
      recordFile.close();
      Serial.println("done.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening file for door open");
      }

      digitalWrite(RED_PIN, HIGH);
      delay(100);
      digitalWrite(RED_PIN, LOW);
}

/*Controls LED states for bad or nonexistent transmission*/
void badTrans(){
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(RED_PIN, HIGH);
      delay(100);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
}

void loop() {

  decodeMsg();

  if(doorState == 1){  
    count++;
  }
  else{
    count = 0;
  }

  if(count > 60){
    doorCount = 2;
  } else if(count > 0){
    doorCount = 1;
  }
  else{
    doorCount = 0;
  }

  switch(doorCount){
    case 0:
      break;
    case 1:
      writeFileYellow();
      break;
    case 2:
      writeFileRed();
      break;
    default:
      badTrans();
      break;
  }

  
  
  delay(1000);
}
