// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>

void setup()
{
    Serial.begin(9600);   // Debugging only
    Serial.println("setup");
    pinMode(13, OUTPUT);
    // Initialise the IO and ISR
    //vw_set_tx_pin(1);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec

    pinMode(11, OUTPUT);
}

int state = 0;

void loop()
{
  char msg[5];
  msg[0] = 'x';
  msg[1] = '/';
  msg[3] = '/';
  msg[4] = 'x';

  state = digitalRead(11);

  Serial.println(state);

  if(state == 1){
    msg[2] = '1';
  }
  else{
    msg[2] = '0';
  }

  digitalWrite(13, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 7);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, LOW);
  delay(1000);
}
