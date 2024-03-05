#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup(){}
void loop(){}
#else

#define DOSERIAL true

#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

const long DEBOUNCE_MILLIS = 5;
const uint8_t inputKeys[] {
  /*  0 */      0x00, // NOT USED - BOOT button
  /*  1 */      0x00,
  /*  2 */      0x00,
  /*  3 */      0x00,
  /*  4 */      0x00,
  /*  5 */      0x00,
  /*  6 */      0x00,
  /*  7 */      0x00,
  /*  8 */      0x00,
  /*  9 */      0x00,
  /* 10 */      0x00,
  /* 11 */      0x00,
  /* 12 */      0x00,
  /* 13 */      0x00,
  /* 14 */      0x00,
  /* 15 */      0x00,
  /* 16 */      0x00,
  /* 17 */      0x00,
  /* 18 */      0x00,
  /* 19 */      0x00, // NOT USED - USB
  /* 20 */      0x00, // NOT USED - USB
  /* 21 */      0x00,
  /* 22 - 34 */ 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // NOT USED - Not Connected
  /* 35 */      0x00,
  /* 36 */      0x00,
  /* 37 */      0x00,
  /* 38 */      0x00,
  /* 39 */      0x00,
  /* 40 */      0x00,
  /* 41 */      0x00,
  /* 42 */      0x00,
  /* 43 */      0x00,
  /* 44 */      0x00,
  /* 45 */      0x00,
  /* 46 */      0x00,
  /* 47 */      0x00,
  /* 48 */      0x00,
};

bool currentStates[48];
long lastChangeState[48];
bool tmp;
long milliseconds;
int i;

void setup() {
  #if DOSERIAL
    Serial.begin( 115200 );
  #endif

  Keyboard.begin();
  USB.begin();

  i = 1;
  while ( i < 48 ) {
    pinMode( i, INPUT_PULLUP );

    i = ( i == 18 ) ? 21 : ( ( i == 21 ) ? 35 : i + 1 );
  }
}

void loop() {
  // Keyboard inputs
  milliseconds = millis();
  i = 1;
  while ( i < 48 ) {
    tmp = digitalRead( i );
    if ( currentStates[i] != tmp && ( milliseconds - lastChangeState[i] ) > DEBOUNCE_MILLIS ) {
      currentStates[i] = tmp;
      lastChangeState[i] = milliseconds;

      #if DOSERIAL
        Serial.print( "Key " );
        Serial.print( i );
        Serial.print( " " );
        Serial.println( tmp );
      #endif

      if ( !tmp ) {
        Keyboard.press( inputKeys[i] );
      } else {
        Keyboard.release( inputKeys[i] );
      }
    }

    i = ( i == 18 ) ? 21 : ( ( i == 21 ) ? 35 : i + 1 );
  }
}

#endif
