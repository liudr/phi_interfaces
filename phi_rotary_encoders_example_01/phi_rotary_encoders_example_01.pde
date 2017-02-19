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

#define Encoder1ChnA 2
#define Encoder1ChnB 3
#define Encoder2ChnA 4
#define Encoder2ChnB 5
#define Encoder3ChnA 6
#define Encoder3ChnB 7
#define EncoderDetent 12

char mapping1[]={'U','D'}; // This is a rotary encoder so it returns U for up and D for down on the dial.
char mapping2[]={'L','R'}; // This is a rotary encoder so it returns L for left and R for right on the dial.
char mapping3[]={'A','B'}; // This is a rotary encoder so it returns A for acceleration and B for brake on the dial.
phi_rotary_encoders my_encoder1(mapping1, Encoder1ChnA, Encoder1ChnB, EncoderDetent);
phi_rotary_encoders my_encoder2(mapping2, Encoder2ChnA, Encoder2ChnB, EncoderDetent);
phi_rotary_encoders my_encoder3(mapping3, Encoder3ChnA, Encoder3ChnB, EncoderDetent);
multiple_button_input* dial1=&my_encoder1;
multiple_button_input* dial2=&my_encoder2;
multiple_button_input* dial3=&my_encoder3;

void setup()
{
  Serial.begin(9600);
  Serial.println("Phi_interfaces library rotary encoder test code");
}

void loop()
{
  char temp;
//Rotary encoder 1:  
//  temp=my_encoder1.getKey(); // Use phi_keypads object to access the keypad
  temp=dial1->getKey(); // Use the phi_interfaces to access the same keypad
  if (temp!=NO_KEY) Serial.println(temp);

//Rotary encoder 2:  
//  temp=my_encoder2.getKey(); // Use phi_keypads object to access the keypad
  temp=dial2->getKey(); // Use the phi_interfaces to access the same keypad
  if (temp!=NO_KEY) Serial.println(temp);

//Rotary encoder 3:  
  temp=my_encoder3.getKey(); // Use phi_keypads object to access the keypad
//  temp=dial3->getKey(); // Use the phi_interfaces to access the same keypad
  if (temp!=NO_KEY) Serial.println(temp);
}

