// Include IR Remote Library 
#include <IRremote.h>

// Define sensor pin as D5
const int RECV_PIN = D5;

// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;

int flag = HIGH;// set variable named flag as HIGH

void setup(){
  // Serial Monitor @ 9600 baudrate
  Serial.begin(9600);
  // Enable the IR Receiver
  irrecv.enableIRIn();
  pinMode(LED_BUILTIN, OUTPUT);// set LED_BUILTIN pin as output
  digitalWrite(LED_BUILTIN, flag); // initially turn off the led
}

void loop(){
  
  if (irrecv.decode(&results))
  {
    // Print Code in HEX
    
        Serial.print((char)results.value);
        irrecv.resume();// resume IR receiving

      switch(results.value)
      {
        case 0x1FE48B7:// recieved hex code is equals to "0x1FE48B7" then on the LED
          digitalWrite(LED_BUILTIN, flag);
          flag = !flag; // complement the value of Flag variable
          break; // discontinue loop from here
      }
  }
}
