#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <VirtualWire.h>

SoftwareSerial portOne(8, 9);

File myFile;
int incomingByte = 0;

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
/*
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  */
  Serial.begin(9600); // Debugging only
    //Serial.println("setup");
    pinMode(13, OUTPUT);
    // Initialise the IO and ISR
    //vw_set_rx_pin(8);  //Changes rx pin on board
    //Default rx pin is I/O pin 11
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop() {
  myFile = SD.open("test.txt", FILE_WRITE);

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
  int i;

  digitalWrite(13, HIGH); // Flash a light to show received good message
  // Message with a good checksum received, print it.
  Serial.print("Got: ");
  
  for (i = 0; i < buflen; i++)
  {
      Serial.print(buf[i], HEX);
      Serial.print(' ');
  }
  Serial.println();
        digitalWrite(13, LOW);
    }
  
  delay(1000);
}
