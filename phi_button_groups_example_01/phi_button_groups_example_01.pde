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

//The following button pins apply to phi-1 and phi-2 shields. Please make appropriate modification for your own setup.
// For phi-1 shield btn_r is 3. For phi-2 shield btn_r is 4
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 4
#define btn_b 14
#define btn_a 15

#define total_buttons 6

char mapping[]={'U','D','L','R','B','A'}; // This is a list of names for each button.
byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a}; // The digital pins connected to the 6 buttons.
phi_button_groups my_btns(mapping, pins, total_buttons);
multiple_button_input* pad1=&my_btns;

void setup(){
  Serial.begin(9600);
  Serial.println("Phi_interfaces library button groups test code");
  my_btns.set_repeat(100); // You don't have to set this since it has a default value of 200. This sets how often a held key is repeated, 0.1s. So the held key is repeated 10 times a second.
  my_btns.set_hold(2000); // You don't have to set this since it has a default value of 2000. This sets how long you need to hold a key before it is repeated, 2s. So the held key starts repeating after being held for 2 seconds.
}

void loop(){
  char temp;
  temp=my_btns.getKey(); // Use phi_button_groups object to access the group of buttons
//  temp=pad1->getKey(); // Use the generic multiple_button_interface to access the same hardware
  if (temp!=NO_KEY) Serial.write(temp);
}

