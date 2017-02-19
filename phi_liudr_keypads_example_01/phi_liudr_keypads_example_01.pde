/** \file
 *  \brief     This is the first official release of the phi_interfaces library.
 *  \details   This library unites buttons, rotary encoders and several types of keypads libraries under one library, the phi_interfaces library, for easy of use. This is the first official release. All currently supported input devices are buttons, matrix keypads, rotary encoders, analog buttons, and liudr pads. User is encouraged to obtain compatible hardware from liudr or is solely responsible for converting it to work on other shields or configurations.
 *  \author    Dr. John Liu
 *  \version   1.0
 *  \date      01/24/2012
 *  \pre       Compatible with Arduino IDE 1.0 and 0022.
 *  \bug       Not tested on, Arduino IDE 0023 or arduino MEGA hardware!
 *  \warning   PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING! No warranty!
 *  \copyright Dr. John Liu. Free software for educational and personal uses. Commercial use without authorization is prohibited.
 *  \par Contact
 * Obtain the documentation or find details of the phi_interfaces, phi_prompt TUI library, Phi-2 shield, and Phi-panel hardware or contact Dr. Liu at:
 *
 * <a href="http://liudr.wordpress.com/phi_interfaces/">http://liudr.wordpress.com/phi_interfaces/</a>
 *
 * <a href="http://liudr.wordpress.com/phi-panel/">http://liudr.wordpress.com/phi-panel/</a>
 *
 * <a href="http://liudr.wordpress.com/phi_prompt/">http://liudr.wordpress.com/phi_prompt/</a>
 *
 * <a href="http://liudr.wordpress.com/phi-2-shield/">http://liudr.wordpress.com/phi-2-shield/</a>
*/

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <phi_interfaces.h>

#define buttons_per_column 8
#define buttons_per_row 2
#define myRowPin0 8 // This is the sensor for POV
#define myRowPin1 9 // This is the sensor for POV
#define myDataPin 15  // The data pin for the shift registers
#define myLatchPin 16  // The latch pin for the shift registers
#define myClockPin 17  // The clock pin for the shift registers

byte keypad_type=Liudr_shift_register_pad;
const char key_mapping_liudr[]={'1','6','2','7','3','8','4','9','5','0','U','D','L','R','B','A'}; // This is a modified version that puts 1-5 on top row and 6-0 on bottom row, more natural, for Liudr pad.
const byte row_pins_liudr[]={myRowPin0,myRowPin1}; // Liudr's keypad
phi_liudr_keypads panel_keypad((char*)key_mapping_liudr, (byte*)row_pins_liudr, myClockPin, myDataPin, myLatchPin, buttons_per_row, buttons_per_column);
multiple_button_input* pad1=&panel_keypad;

void setup()
{
  Serial.begin(9600);
  Serial.println("Phi_interfaces library liudr keypad test code");
}

void loop()
{
  char temp;
//  temp=panel_keypad.getKey(); // Use phi_keypads object to access the keypad
  temp=pad1->getKey(); // Use the phi_interfaces to access the same keypad
  if (temp!=NO_KEY) Serial.write(temp);
}

