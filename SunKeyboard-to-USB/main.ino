/*
 * USB Adapter for Sun Type 5 Keyboard
 * Ben Rockwood <benr@cuddletech.com> 1/1/17
 *
 * Developed on Arduino Micro
 * MiniDIN 8a Adapter P/N: MD-80PL100
 * Wiring:
 *    Keyboard Pin 2 (White): GND
 *    Keyboard Pin 8 (Red): +5V
 *    Keyboard Pin 6 (Yellow): Arduino Pin D10 (Serial RX)
 *    Keyboard Pin 5 (Green): Aruidno Pin D11 (Serial TX)
 */

#include <Keyboard.h>
#include <SoftwareSerial.h>
#include "keyboardSwitch.h"

//Software serial for Sun KBD
SoftwareSerial sunSerial(10, 11, true);

boolean NUM_LOCK_ON = false;  // led bitfield xxx1 (1)
boolean CAPS_LOCK_ON = false; // led bitfield 1xxx (8)

byte led_cmd[2] = { 0x0E, 0x01 };  // Used for sending the 2 byte commands to the keyboard

void setup() {
  Serial.begin(1200);	        // Normal serial for Serial Monitor Debugging
  sunSerial.begin(1200);	// Serial Connection to Sun Keyboard
 
  Keyboard.begin();             // Initialize USB Keyboard Interface
  
  sunSerial.write(led_cmd, 2);  // Enable Number Lock by Default
  sunSerial.write(CMD_DISABLE_BELL);      // Power Key to Disable Bell
}


void loop() {
  char c = sunSerial.read();
  
  if (c != 0xFFFFFFFF) {
   switch(c) {
//    case 45:  sunSerial.write(CMD_DISABLE_CLICK); break;     // Mute Key to Disable Click Sound
//    case 2:   sunSerial.write(CMD_ENABLE_CLICK); break;      // Decr Vol Key to Enable Click Sound
//    case 4:   sunSerial.write(CMD_ENABLE_BELL); break;       // Incr Vol to Enable Bell
//    case 48:  sunSerial.write(CMD_DISABLE_BELL); break;      // Power Key to Disable Bell 
    case 45:  key_vol_mute(); break;
    case  4:  key_vol_incr(); break;
    case  2:  key_vol_decr(); break;
    case 48:  key_power(); break;
    case 98:  break;                                         // Ignore NumLock
    case  1:  key_stop(); break;
    case 49:  key_front(); break;
    case 72:  key_open(); break;
    case 95:  key_find(); break;
    case 51:  key_copy(); break;
    case 73:  key_paste(); break;
    case 97:  key_cut(); break;
    case 26:  key_undo(); break;
    case  3:  key_again(); break;
    case 22:  key_print_screen(); break;
//    case 118:  key_help(); break;
    case 118:  if (!CAPS_LOCK_ON) {                           // Caps Lock Toggle. Cap key = 119, blank key = 15, Help key = 118
                CAPS_LOCK_ON = true;
                led_cmd[1] += 8;
                sunSerial.write(led_cmd, 2);
                Keyboard.write(KEY_CAPS_LOCK);
              } else {
                CAPS_LOCK_ON = false;
                led_cmd[1] -= 8;
                sunSerial.write(led_cmd, 2);                 
                Keyboard.write(KEY_CAPS_LOCK);
              }
    default: outputKey(c); break;
   }
  }
}

void outputKey(int key){

 if (key < 0) {	
  key += 128;
  Serial.print("Key Up: "); Serial.println(sun_to_ascii[key]);
  Keyboard.release(sun_to_ascii[key]);
 } else if (key == 127) {
  Serial.println("All Keys Released");
  Keyboard.releaseAll();
 } else {
  Serial.print("Key Down: "); Serial.println(sun_to_ascii[key]);
  Keyboard.press(sun_to_ascii[key]);
 }
}

void key_help() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.releaseAll();
}

void key_stop() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_F4);
  Keyboard.releaseAll();
}

void key_stop() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_DEL);
}

void key_front() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_TAB);
  Keyboard.releaseAll();
}

void key_open() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(111); // o
  Keyboard.releaseAll();
}

void key_find() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(102); // f
  Keyboard.releaseAll();
}

void key_copy() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(99); // c
  Keyboard.releaseAll();
}

void key_paste() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(118); // v
  Keyboard.releaseAll();
}

void key_cut() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(120); // x
  Keyboard.releaseAll();
}

void key_undo() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(122); // z
  Keyboard.releaseAll();
}

void key_again() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(121); // y
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(122); //z
  Keyboard.releaseAll();
}

void key_print_screen() {
  Keyboard.write(44);
}

void key_vol_decr() {
  // Browser back button
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.releaseAll();
}

void key_vol_incr() {
  // Browser forward button
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_RIGHT_ARROW);
  Keyboard.releaseAll();
}

void key_vol_mute() {
  // Windows virtual desktop left
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.releaseAll();  
}

void key_power() {
  // Windows virtual desktop right
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_RIGHT_ARROW);
  Keyboard.releaseAll();  
}

