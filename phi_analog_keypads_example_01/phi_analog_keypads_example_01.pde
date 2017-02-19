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

#define buttons_per_column 5 // Each analog pin has five buttons with resistors.
#define buttons_per_row 1 // There are two analog pins in use.

byte keypad_type=Analog_keypad;
char mapping[]={'1','2','3','4','5'}; // This is an analog keypad.
byte pins[]={0}; // The pin numbers are analog pin numbers.
int values[]={0, 146, 342, 513, 744}; //These numbers need to increase monotonically.
phi_analog_keypads panel_keypad(mapping, pins, values, buttons_per_row, buttons_per_column);
multiple_button_input* pad1=&panel_keypad;

void setup()
{
  Serial.begin(9600);
  Serial.println("Phi_interfaces library analog keypad test code");
}

void loop()
{
  byte temp=panel_keypad.getKey(); // Use phi_keypads object to access the keypad
//  byte temp=pad1->getKey(); // Use the generic multiple_button_interface to access the same keypad
  if (temp!=NO_KEY) Serial.write(temp);
}

