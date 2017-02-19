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

#include <phi_interfaces.h>

#define buttons_per_column 4
#define buttons_per_row 4

char mapping[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'}; // This is a matrix keypad.
byte pins[]={17, 16, 15, 13, 12, 11, 9, 8}; // The first four pins are rows, the next 4 are columns. If you have 4*3 pad, then the first 4 are rows and the next 3 are columns.
phi_matrix_keypads panel_keypad(mapping, pins, buttons_per_row, buttons_per_column);
multiple_button_input* pad1=&panel_keypad;

void setup()
{
  Serial.begin(9600);
  Serial.println("Phi_interfaces library matrix keypad test code");
  panel_keypad.set_repeat(100); // You don't have to set this since it has a default value of 200. This sets how often a held key is repeated, 0.1s. So the held key is repeated 10 times a second.
  panel_keypad.set_hold(2000); // You don't have to set this since it has a default value of 2000. This sets how long you need to hold a key before it is repeated, 2s. So the held key starts repeating after being held for 2 seconds.
  
}

void loop()
{
  char temp;
  temp=panel_keypad.getKey(); // Use phi_keypads object to access the keypad
//  temp=pad1->getKey(); // Use the generic multiple_button_interface to access the same keypad
  if (temp!=NO_KEY) Serial.write(temp);
}

