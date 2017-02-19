#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <phi_interfaces.h>

// Static member variable initialization
unsigned int multiple_button_input::buttons_hold_time=buttons_hold_time_def;
unsigned int multiple_button_input::buttons_debounce_time=buttons_debounce_time_def;
unsigned int multiple_button_input::buttons_dash_threshold=buttons_dash_threshold_def;
unsigned int multiple_button_input::buttons_repeat_time=buttons_repeat_time_def;
unsigned int multiple_button_input::buttons_dash_time=buttons_dash_time_def;

unsigned long multiple_button_input::t_last_action=0;

//Rotary encoder class member functions:
/*
.______        ______   .___________.    ___      .______     ____    ____ 
|   _  \      /  __  \  |           |   /   \     |   _  \    \   \  /   / 
|  |_)  |    |  |  |  | `---|  |----`  /  ^  \    |  |_)  |    \   \/   /  
|      /     |  |  |  |     |  |      /  /_\  \   |      /      \_    _/   
|  |\  \----.|  `--'  |     |  |     /  _____  \  |  |\  \----.   |  |     
| _| `._____| \______/      |__|    /__/     \__\ | _| `._____|   |__|     
*/
/**
 * \details Constructor for rotary encoder. Provide the names of up and down actions such as 1, and 2, or 'U' and 'D', arduino pins for channels A and B, and number of detent per rotation. Please define the shaft click as a regular phi_buttons or phi_button_arrays object.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to the rotary encoder dial up and down.
 * \param ChnA This is the arduino pin connected to the encoder channel A.
 * \param ChnB This is the arduino pin connected to the encoder channel B.
 * \param det This is the number of detent per rotation.

 * Example:
 
char mapping[]={'U','D'}; // This is a rotary encoder that returns U for up and D for down rotation on the dial.

phi_rotary_encoders my_encoder(mapping, Encoder1ChnA, Encoder1ChnB, EncoderDetent); // Replace Encoder1ChnA, Encoder1ChnB, EncoderDetent with actual numbers.
 */
phi_rotary_encoders::phi_rotary_encoders(char *na, byte ChnA, byte ChnB, byte det)
{
  device_type=Rotary_encoder;
  key_names=na; // Translated names of the keys, such as '0'.
  EncoderChnA=ChnA;
  EncoderChnB=ChnB;
  
  pinMode(EncoderChnA, INPUT);
  digitalWrite(EncoderChnA, HIGH);
  pinMode(EncoderChnB, INPUT);
  digitalWrite(EncoderChnB, HIGH);

  detent=det;
  stat_seq_ptr=4; // Center the status of the encoder
  counter=0;
}

/**
 * \details This actually performs the encoder read and returns up or down dials with the translation done by key_names.
 * If you are not very interested in the inner working of this library, this is the only function you need to call to get a response on the rotary encoder.
 * It assumes the channels are off when the knob is in a groove. To assume the channels are on when the knob is in a groove, read the code on stat_deq.
 * To properly sense the encoder, call this function inside of a loop.
 * \return It returns the named keys defined by the constructor such as 'U' and 'D' for up and down dial rotations.
 */
byte phi_rotary_encoders::getKey()
{
  static const byte stat_seq[]={3,2,0,1,3,2,0,1,3}; // For always on switches use {0,1,3,2,0,1,3,2,0}; For the sake of simple coding, please don't mix always-on encoders with always-off encoders.
  byte stat_int=(digitalRead(EncoderChnB)<<1) | digitalRead(EncoderChnA);
  if (stat_int==stat_seq[stat_seq_ptr+1])
  {
    stat_seq_ptr++;
    if (stat_seq_ptr==8)
    {
      stat_seq_ptr=4;
      counter++;
      return key_names[0];
    }
  }
  else if (stat_int==stat_seq[stat_seq_ptr-1])
  {
    stat_seq_ptr--;
    if (stat_seq_ptr==0)
    {
      stat_seq_ptr=4;
      counter--;
      return key_names[1];
    }
  }
  return NO_KEY;
}

/**
 * \details This always returns buttons_up due to the fact that rotary encoders can't assume other status.
 * \return This function is defined only to be compatible with the parent class and always returns buttons_up. 
 */
byte phi_rotary_encoders::get_status()
{
  return buttons_up;
}

/**
 * \details This always returns NO_KEY due to the nature of rotary encoders.
 * \return This function is defined only to be compatible with the parent class and always returns NO_KEY. 
 */
byte phi_rotary_encoders::get_sensed()
{
  return NO_KEY;
}

/**
 * \details Get the angle or orientation of the rotary encoder between 0 and detent-1. This function calls getKey to update the angle.
 * If you call getKey BEFORE get_angle, you get the dial up/down from getKey and the correct angle from get_angle. If you call get_angle BEFORM getKey, the dial up/down is read and lost but you get the correct angle.
 * So make your decision. Do you want just dial up/down actions? Then only call getKey. Do you want just angle? Then only call get_angle. Chances of you need them both is very slim but as mentioned you should call getKey first.
 * To properly update the angle, you need to call this function inside of a loop.
 * \return It returns a value between 0 and detent-1. You can calculate angle with it return.
 */
byte phi_rotary_encoders::get_angle()
{
  getKey();
  return (((int)counter)%detent+detent)%detent;
}

/*
______ _____ _____ ___  ________   __    ______ 
| ___ \  _  |_   _/ _ \ | ___ \ \ / /    |  _  \
| |_/ / | | | | |/ /_\ \| |_/ /\ V /     | | | |
|    /| | | | | ||  _  ||    /  \ /      | | | |
| |\ \\ \_/ / | || | | || |\ \  | |      | |/ / 
\_| \_|\___/  \_/\_| |_/\_| \_| \_/      |___/  
*/
/**
 * \details Constructor for rotary encoder. Provide the names of up and down actions such as 1, and 2, or 'U' and 'D', arduino pins for channels A and B, and number of detent per rotation. Please define the shaft click as a regular phi_buttons or phi_button_arrays object.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to the rotary encoder dial up and down.
 * \param ChnA This is the arduino pin connected to the encoder channel A.
 * \param ChnB This is the arduino pin connected to the encoder channel B.
 * \param det This is the number of detent per rotation.
 * \param en_type This is the type of rotary encoder, EncoderType_NO, EncoderType_NC. EncoderType_OC is not yet supported. See #defines in the header file.

 * Example:
 
char mapping[]={'U','D'}; // This is a rotary encoder that returns U for up and D for down rotation on the dial.

phi_rotary_encoders my_encoder(mapping, Encoder1ChnA, Encoder1ChnB, EncoderDetent, EncoderType); // Replace Encoder1ChnA, Encoder1ChnB, EncoderDetent, EncoderType with actual numbers.
 */
phi_rotary_encoders_d::phi_rotary_encoders_d(char *na, byte ChnA, byte ChnB, byte det, byte en_type)
{
	device_type=Rotary_encoder;
	key_names=na; // Translated names of the keys, such as '0'.
	EncoderChnA=ChnA;
	EncoderChnB=ChnB;
	EncoderType=en_type;

	pinMode(EncoderChnA, INPUT);
	digitalWrite(EncoderChnA, HIGH);
	pinMode(EncoderChnB, INPUT);
	digitalWrite(EncoderChnB, HIGH);

	detent=det;
	counter=0;
	stat_seq_ptr=4; // Center the status of the encoder
}

/**
 * \details This function does the actual sensing of the encoder and returns a 2-bit state, with channel A at 1th bit and channel B at 0th bit. It inverts the results from an NC type encoder so matching sequence will be easier to do.
 * \return It returns the key from mapping stored in key_names[].
 */
byte phi_rotary_encoders_d::get_encoder_state()
{
	if (EncoderType==EncoderType_NC)
	return ((!digitalRead(EncoderChnA))<<1) | (!digitalRead(EncoderChnB));
	else return (digitalRead(EncoderChnA)<<1) | digitalRead(EncoderChnB);
}

/**
 * \details This actually performs the encoder read and returns up or down dials with the translation done by key_names.
 * If you are not very interested in the inner working of this library, this is the only function you need to call to get a response on the rotary encoder.
 * It assumes the channels are off when the knob is in a groove. To assume the channels are on when the knob is in a groove, read the code on stat_deq.
 * To properly sense the encoder, call this function inside of a loop.
 * \return It returns the named keys defined by the constructor such as 'U' and 'D' for up and down dial rotations.
 */
byte phi_rotary_encoders_d::getKey()
{
	static const byte stat_seq[]={3,2,0,1,3,2,0,1,3}; // For always on switches use {0,1,3,2,0,1,3,2,0}; For the sake of simple coding, please don't mix always-on encoders with always-off encoders.
	byte stat_int=get_encoder_state();// This layer separates the actual sensing of either analog or digital signal from the logic layer.
	if (stat_int==stat_seq[stat_seq_ptr+1])
	{
		stat_seq_ptr++;
		if (stat_seq_ptr==8)
		{
			stat_seq_ptr=4;
			counter++;
			return key_names[0];
		}
	}
	else if (stat_int==stat_seq[stat_seq_ptr-1])
	{
		stat_seq_ptr--;
		if (stat_seq_ptr==0)
		{
			stat_seq_ptr=4;
			counter--;
			return key_names[1];
		}
	}
	return NO_KEY;
}

/**
 * \details This always returns buttons_up due to the fact that rotary encoders can't assume other status.
 * \return This function is defined only to be compatible with the parent class and always returns buttons_up. 
 */
byte phi_rotary_encoders_d::get_status()
{
  return buttons_up;
}

/**
 * \details This always returns NO_KEY due to the nature of rotary encoders.
 * \return This function is defined only to be compatible with the parent class and always returns NO_KEY. 
 */
byte phi_rotary_encoders_d::get_sensed()
{
  return NO_KEY;
}

/**
 * \details Get the angle or orientation of the rotary encoder between 0 and detent-1. This function calls getKey to update the angle.
 * If you call getKey BEFORE get_angle, you get the dial up/down from getKey and the correct angle from get_angle. If you call get_angle BEFORM getKey, the dial up/down is read and lost but you get the correct angle.
 * So make your decision. Do you want just dial up/down actions? Then only call getKey. Do you want just angle? Then only call get_angle. Chances of you need them both is very slim but as mentioned you should call getKey first.
 * To properly update the angle, you need to call this function inside of a loop.
 * \return It returns a value between 0 and detent-1. You can calculate angle with it return.
 */
byte phi_rotary_encoders_d::get_angle()
{
  getKey();
  return (((int)counter)%detent+detent)%detent;
}




/*
______ _____ _____ ___  ________   __      ___  
| ___ \  _  |_   _/ _ \ | ___ \ \ / /     / _ \ 
| |_/ / | | | | |/ /_\ \| |_/ /\ V /     / /_\ \
|    /| | | | | ||  _  ||    /  \ /      |  _  |
| |\ \\ \_/ / | || | | || |\ \  | |      | | | |
\_| \_|\___/  \_/\_| |_/\_| \_| \_/      \_| |_/
*/
/**
 * \details Constructor for rotary encoder. Provide the names of up and down actions such as 1, and 2, or 'U' and 'D', arduino pins for channels A and B, and number of detent per rotation. Please define the shaft click as a regular phi_buttons or phi_button_arrays object.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to the rotary encoder dial up and down.
 * \param ChnA This is the arduino analog pin connected to the encoders.
 * \param vals This stores the values of the analog channel readings for conversion to states. This stores the analog values of the encoder when the various encoder states: [0]=A&B open, [1]=A closed, B open, [2]=A&B closed, [3]=A open, B closed, [4]=A&B open. Being byte arrays, they only store 1/4 the actual analogRead values, since the four values are far enough apart. Example: analog_values[]={152,128,0,80};
 * \param det This is the number of detent per rotation.
 * \param en_type This is the type of encoder, EncoderType_NO, EncoderType_NC, EncoderType_OC. See #defines in the header file.

 * Example:
 
char mapping[]={'U','D'}; // This is a rotary encoder that returns U for up and D for down rotation on the dial.
byte analog_vals[]={151,128,0,80};

phi_rotary_encoders_a my_encoder(mapping, EncoderAnalogChn, analog_vals, EncoderDetent, EncoderType_NO); // The most common normally open encoder (NO). Replace EncoderAnalogChn, EncoderDetent with actual numbers.
 */
phi_rotary_encoders_a::phi_rotary_encoders_a(char *na, byte ChnA, byte *vals, byte det, byte en_type)
{
	device_type=Rotary_encoder;
	key_names=na; // Translated names of the keys, such as '0'.
	ChnAnalog=ChnA;
	EncoderType=en_type;
	detent=det;
	analog_values=vals;
	counter=0;
	stat_seq_ptr=4; // Center the status of the encoder
}

/**
 * \details This function does the actual sensing of the encoder and returns a 2-bit state, with channel A at 1th bit and channel B at 0th bit. It inverts the results from an NC type encoder so matching sequence will be easier to do.
 * \param prev_state This is the previous encoder state supplied by the caller. In case the reading is a stray value, this is returned.
 * \return It returns the key from mapping stored in key_names[].
 */
byte phi_rotary_encoders_a::get_encoder_state(byte prev_state)
{
	int analog_in=0;
	byte ret_val=B11;
	byte found_val=0; // Sometimes analog value strays away from the expected values and we may find no value.
	//analogRead(ChnAnalog); // Read and discard.
	analog_in=analogRead(ChnAnalog)/4; // Read once and discard.
	
	/*for (byte i=0;i<4;i++)
	{
		analog_in+=analogRead(ChnAnalog);
	}
	analog_in/=16;*/
	
	if (abs(analog_in-analog_values[0])<=analog_difference/2) {ret_val=B11; found_val=1;} //Both open
	else if (abs(analog_in-analog_values[1])<=analog_difference/2) {ret_val=1; found_val=1;} //A close and B open
	else if (abs(analog_in-analog_values[2])<=analog_difference/2) {ret_val=0; found_val=1;} //Both close
	else if (abs(analog_in-analog_values[3])<=analog_difference/2) {ret_val=B10; found_val=1;} //A open B close
	
	if (!found_val) return prev_state; // In case the analog value is stray value away from expected, just return previous state supplied by the caller.
	if (EncoderType==EncoderType_NC)
	return ((~ret_val)&B11);
	else return ret_val;
}

/**
 * \details This actually performs the encoder read and returns up or down dials with the translation done by key_names.
 * If you are not very interested in the inner working of this library, this is the only function you need to call to get a response on the rotary encoder.
 * It assumes the channels are off when the knob is in a groove. To assume the channels are on when the knob is in a groove, read the code on stat_deq.
 * To properly sense the encoder, call this function inside of a loop.
 * \return It returns the named keys defined by the constructor such as 'U' and 'D' for up and down dial rotations.
 */
byte phi_rotary_encoders_a::getKey()
{
  static const byte stat_seq[]={3,2,0,1,3,2,0,1,3}; // For always on switches use {0,1,3,2,0,1,3,2,0}; For the sake of simple coding, please don't mix always-on encoders with always-off encoders.
  byte stat_int=get_encoder_state(stat_seq[stat_seq_ptr]);// This layer separates the actual sensing of either analog or digital signal from the logic layer. Due to analog nature sometimes stray value is read so previous state is supplied.
  if (stat_int==stat_seq[stat_seq_ptr+1])
  {
    stat_seq_ptr++;
    if (stat_seq_ptr==8)
    {
      stat_seq_ptr=4;
      counter++;
      return key_names[0];
    }
  }
  else if (stat_int==stat_seq[stat_seq_ptr-1])
  {
    stat_seq_ptr--;
    if (stat_seq_ptr==0)
    {
      stat_seq_ptr=4;
      counter--;
      return key_names[1];
    }
  }
  return NO_KEY;
}

/**
 * \details This always returns buttons_up due to the fact that rotary encoders can't assume other status.
 * \return This function is defined only to be compatible with the parent class and always returns buttons_up. 
 */
byte phi_rotary_encoders_a::get_status()
{
  return buttons_up;
}

/**
 * \details This always returns NO_KEY due to the nature of rotary encoders.
 * \return This function is defined only to be compatible with the parent class and always returns NO_KEY. 
 */
byte phi_rotary_encoders_a::get_sensed()
{
  return NO_KEY;
}

/**
 * \details Get the angle or orientation of the rotary encoder between 0 and detent-1. This function calls getKey to update the angle.
 * If you call getKey BEFORE get_angle, you get the dial up/down from getKey and the correct angle from get_angle. If you call get_angle BEFORM getKey, the dial up/down is read and lost but you get the correct angle.
 * So make your decision. Do you want just dial up/down actions? Then only call getKey. Do you want just angle? Then only call get_angle. Chances of you need them both is very slim but as mentioned you should call getKey first.
 * To properly update the angle, you need to call this function inside of a loop.
 * \return It returns a value between 0 and detent-1. You can calculate angle with it return.
 */
byte phi_rotary_encoders_a::get_angle()
{
  getKey();
  return (((int)counter)%detent+detent)%detent;
}

//Serials class member functions:
/*
     _______. _______ .______       __       ___       __
    /       ||   ____||   _  \     |  |     /   \     |  |
   |   (----`|  |__   |  |_)  |    |  |    /  ^  \    |  |
    \   \    |   __|  |      /     |  |   /  /_\  \   |  |
.----)   |   |  |____ |  |\  \----.|  |  /  _____  \  |  `----.
|_______/    |_______|| _| `._____||__| /__/     \__\ |_______|
*/
/**
 * \details This is the constructor of phi_serial_keypads class. This class is intended for <a href="http://liudr.wordpress.com/phi-panel/">phi-panel serial LCD keypad</a> or debugging and prototyping. You can create a keypad with this class and use serial keypads or type in the serial monitor as key presses.
 * Later once you decide what actual keypad to use, you can just replace this class with the proper class.
 * \param ser This is the address of your serial object, which you already used begin() on, such as &SERIAL or &NSS.
 * \param bau This is the baud rate in unsigned long integer. At the moment it is not utilized but just reserved for future code.
 */
phi_serial_keypads::phi_serial_keypads(Stream *ser, unsigned long bau)
{
  device_type=Serial_keypad;
  ser_baud=bau;
  ser_port=ser;
}

/**
 * \details This acquires one character from a serial port as a key press. If the port is empty then it returns NO_KEY.
 * If you are not very interested in the inner working of this library, this is the only function you need to call to get a response on the rotary encoder.
 * \return It returns the serial port content in byte data type or NO_KEY.
 */
byte phi_serial_keypads::getKey()
{
  if (ser_port->available()) return ser_port->read();
  else return NO_KEY;
}  

//Keypad class member functions:
/*
 __  ___  ___________    ____ .______      ___       _______  
|  |/  / |   ____\   \  /   / |   _  \    /   \     |       \ 
|  '  /  |  |__   \   \/   /  |  |_)  |  /  ^  \    |  .--.  |
|    <   |   __|   \_    _/   |   ___/  /  /_\  \   |  |  |  |
|  .  \  |  |____    |  |     |  |     /  _____  \  |  '--'  |
|__|\__\ |_______|   |__|     | _|    /__/     \__\ |_______/ 
*/
/**
 * \details Outputs the name of the last sensed key or NO_KEY. If all you want is to  sense a key press, use getKey instead. You can use this in conjunction with get_status to sense if a key is held.
 * \return it returns the name of the last sensed key or NO_KEY. This key may not be currently pressed.
 */ 
byte phi_keypads::get_sensed()
{
  if (button_sensed==NO_KEYs) return NO_KEY;
  else return key_names[button_sensed];
}

/**
 * \details Return status of the sensed key. If there is no sensed key, this return should not be used.
 * \return It returns status of the sensed key.
 */
byte phi_keypads::get_status()
{
  return button_status;
}

/**
 * \details This is the public method to get a key press from the keypad. The key press is translated into key_names or NO_KEY.
 * This function is inherited from multiple_button_inputs. All Keypad subclasses such as phi_matrix_keypads and phi_analog_keypads share this code.
 * Since all multiple_button_inputs devices have this method, you can treat all of them as multiple_button_inputs and call this method to get a key press.
 * This is the function you should call to sense key presses. 
 * It is only few lines of code and is generic enough for all phi_keypads.
 * \return It returns the name of the key that is pressed down.
 */
byte phi_keypads::getKey()
{
  byte key=scanKeypad();
  if (key==NO_KEYs) key=NO_KEY;
  else key=key_names[key];
  return key;
}

/**
 * \details This routine uses senseAll to scan the keypad, use debouncing to update button_sensed and button_status.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * The getKey calls this function and translates keypress from scan code (0 to max_key-1) into characters or key names.
 * \return This function only returns scan code (0 to max_key-1).
 */
byte phi_keypads::scanKeypad()
{
  byte button_pressed=sense_all();
  
  switch (button_status)
  {
    case buttons_up:
    if (button_pressed!=NO_KEYs)
    {
      button_sensed==button_pressed;
      button_status_t=millis();
      button_status=buttons_debounce;
    }
    else button_sensed=NO_KEYs;
    break;
    
    case buttons_debounce:
    if (button_pressed!=NO_KEYs)
    {
      if (button_sensed==button_pressed)
      {
        if (millis()-button_status_t>buttons_debounce_time)
        {
          button_status=buttons_pressed;
          button_status_t=millis();
          t_last_action=button_status_t;
          return button_sensed;
        }
      }
      else
      {
        button_status_t=millis();
        button_sensed=button_pressed;
      }
    }
    else
    {
      button_status=buttons_up;
      button_sensed=NO_KEYs;
    }
    break;
    
    case buttons_pressed:
    if (button_pressed!=NO_KEYs)
    {
      if (button_sensed==button_pressed) button_status=buttons_down;
      else button_status=buttons_debounce;
      button_status=buttons_down;
    }
    else button_status=buttons_released;
    button_status_t=millis();
    break;
    
    case buttons_down:
    if (button_sensed==button_pressed)
    {
      if (millis()-button_status_t>buttons_hold_time)
      {
        button_status=buttons_held;
        button_status_t=millis();
      }
    }
    else
    {
      button_status=buttons_released;
      button_status_t=millis();
    }
    break;
    
    case buttons_released:
    if (button_pressed==NO_KEYs) button_status=buttons_up;
    else
    {
      button_status=buttons_debounce;
      button_sensed=button_pressed;
      button_status_t=millis();
    }
    break;
    
    case buttons_held:
    if (button_sensed!=button_pressed)
    {
      button_status=buttons_released;
      button_status_t=millis();
      return button_sensed;
    }
    else if (millis()-button_status_t>buttons_repeat_time)
    {
      button_status_t=millis();
      return button_sensed;
    }
    break;
  }
  return NO_KEYs;
}

//Joystick class member functions
/*
       __    ______   ____    ____  _______.___________. __    ______  __  ___
      |  |  /  __  \  \   \  /   / /       |           ||  |  /      ||  |/  /
      |  | |  |  |  |  \   \/   / |   (----`---|  |----`|  | |  ,----'|  '  /
.--.  |  | |  |  |  |   \_    _/   \   \       |  |     |  | |  |     |    <
|  `--'  | |  `--'  |     |  | .----)   |      |  |     |  | |  `----.|  .  \
 \______/   \______/      |__| |_______/       |__|     |__|  \______||__|\__\
*/
/**
 * \details A joystick has two potentiometers and provides the x and y axis locations with the wiper pins.
 * If the shaft is clickable, declare the button in a phi_button_groups object.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each directional key press. The array starts with the top left direction and iterates through top, top right, etc. The middle position is filled with NO_KEYs, not NO_KEY.
 * \param sp This is the name of (or pointer to) a byte array that stores all analog pins used by the joystick.
 * \param dp This is the name of (or pointer to) an integer array that stores the analog values of each button press (stick push). The array must be sorted from small to big then from one axis to the next.
 * \param th This stores the threshold of matching the joystick with a directional key.
 */
phi_joysticks::phi_joysticks(char *na, byte *sp, int * dp, int th)
{
  device_type=Joy_stick;
  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins
  values=dp; // Points to divider value array.
  rows=2;
  columns=3;
  threshold=th;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<rows;j++) // Setting sensing rows to input and disabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],LOW);
  }
}

/**
 * \details This is the most physical layer of the phi_joysticks. Senses all input pins for a valid status.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_joysticks::sense_all()
{
  byte diff[2];
  diff[0]=NO_KEYs;
  diff[1]=NO_KEYs;
  for (byte i=0;i<rows;i++)
  {
    axis_vals[i]=analogRead(mySensorPins[i]);
    delay(5);
  }
  for (byte j=0;j<rows;j++)
  {
    for (byte i=0;i<columns;i++)
    {
      if(abs(values[j*columns+i]-axis_vals[j])<threshold) diff[j]=i; // Find the difference between analog read and stored values.
    }
  }
  if ((diff[0]==NO_KEYs)||(diff[1]==NO_KEYs)||((diff[0]==columns/2)&&(diff[1]==columns/2))) return NO_KEYs; // returns the button pressed if neither axis is in the middle.
  else return (diff[0]*columns+diff[1]);
}

//Analog keys class member functions
/*
     ___      .__   __.      ___       __        ______     _______ 
    /   \     |  \ |  |     /   \     |  |      /  __  \   /  _____|
   /  ^  \    |   \|  |    /  ^  \    |  |     |  |  |  | |  |  __  
  /  /_\  \   |  . `  |   /  /_\  \   |  |     |  |  |  | |  | |_ | 
 /  _____  \  |  |\   |  /  _____  \  |  `----.|  `--'  | |  |__| | 
/__/     \__\ |__| \__| /__/     \__\ |_______| \______/   \______| 
*/
/**
 * \details Analog keypads are made up of several analog pins. Each pin is connected to several buttons and resistors. Find diagram in my blog.
 * All analog pins need to be connected to the same resistor network. If you don't need as many buttons, you can omit some buttons but never omit any resistors.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each key press.
 * \param sp This is the name of (or pointer to) a byte array that stores all analog pins used by the keypad. Unlike the original analogbutton, you can use multiple pins, with each pin connected to a number of buttons to form a keypad.
 * \param dp This is the name of (or pointer to) an integer array that stores the analog values of each button press. The array must be sorted from small to big. If you have 5 buttons, this array should have 5 elements.
 * \param r This is the number of analog pins or "rows" of the analog keypad.
 * \param c This is the number of buttons attached to each analog pin or "columns" of the analog keypad. All analog pins should connect to identical button/resistor configurations.
 *  If you don't need that many buttons for one particular pin, don't forget to connect all the resistors so that the analog values will be the same.
 */
phi_analog_keypads::phi_analog_keypads(char *na, byte *sp, int * dp, byte r, byte c)
{
  device_type=Analog_keypad;
  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins
  values=dp; // Points to divider value array.
  rows=r;
  columns=c;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<rows;j++) // Setting sensing rows to input and disabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],LOW); // To use phi-3 shield with internal pullup, do digitalWrite(A0,HIGH) after simple_setup_phi_3() or after this constructor.
  }
}

/**
 * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_analog_keypads::sense_all()
{

  for (byte j=0;j<rows;j++)
  {
    int temp=analogRead(mySensorPins[j]);
    for (byte i=0;i<columns;i++)
    {
      int diff=abs(values[i]-temp); // Find the difference between analog read and stored values.
      if (diff<analog_difference) 
      {
        return (i+j*columns); // returns the button pressed
      }
    }
  }
  return NO_KEYs;
}

//Matrix keypads class member functions
/*
.___  ___.      ___   .___________..______       __  ___   ___ 
|   \/   |     /   \  |           ||   _  \     |  | \  \ /  / 
|  \  /  |    /  ^  \ `---|  |----`|  |_)  |    |  |  \  V  /  
|  |\/|  |   /  /_\  \    |  |     |      /     |  |   >   <   
|  |  |  |  /  _____  \   |  |     |  |\  \----.|  |  /  .  \  
|__|  |__| /__/     \__\  |__|     | _| `._____||__| /__/ \__\ 
*/
/**
 * \details Matrix keypad has column pins and row pins.
 * Column pins are pulled LOW when that column is addressed and left in tristate with pull-up resistor when those columns are not addressed.
 * Row pins are always inputs with pull-up resistors and will be read when that row is addressed.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each key press.
 * \param sp This is the name of (or pointer to) a byte array that stores all pins used by the keypad, with row pins and then column pins.
 * \param r This is the number of rows of the matrix keypad.
 * \param c This is the number of columns of the matrix keypad.
 */
phi_matrix_keypads::phi_matrix_keypads(char *na, byte * sp, byte r, byte c) // Matrix keypads constructor
{
  if ((r==4)&&(c==3)) device_type=Matrix3X4;
  if ((r==4)&&(c==4)) device_type=Matrix4X4;

  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins are followed by column pins
  rows=r;
  columns=c;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<rows;j++) // Setting sensing rows to input and enabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],HIGH);
  }

  for (int j=rows;j<columns+rows;j++) // Setting columns to HIGH.
  {
    pinMode(mySensorPins[j],OUTPUT);
    digitalWrite(mySensorPins[j],HIGH);
  }
}

/**
 * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status. The scanKeypad calls this function and interprets the return into status of the key.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_matrix_keypads::sense_all()
{

  for (byte j=0;j<rows;j++)
  {
    for (byte i=0;i<columns;i++)
    {
      digitalWrite(mySensorPins[rows+i],LOW);
      buttonBits=digitalRead(mySensorPins[j]); // Sensing button, using buttonBits as a temp variable.
      digitalWrite(mySensorPins[rows+i],HIGH); 
      if (buttonBits==LOW)
      {
        return (i+j*columns); // returns the button pressed
      }
    }
  }
  return NO_KEYs; // no buttons pressed
}

//Button arrays class member functions
/*
.______    __    __  .___________.___________.  ______   .__   __. 
|   _  \  |  |  |  | |           |           | /  __  \  |  \ |  | 
|  |_)  | |  |  |  | `---|  |----`---|  |----`|  |  |  | |   \|  | 
|   _  <  |  |  |  |     |  |        |  |     |  |  |  | |  . `  | 
|  |_)  | |  `--'  |     |  |        |  |     |  `--'  | |  |\   | 
|______/   \______/      |__|        |__|      \______/  |__| \__| 
                                                                   
  _______ .______        ______    __    __  .______        _______.
 /  _____||   _  \      /  __  \  |  |  |  | |   _  \      /       |
|  |  __  |  |_)  |    |  |  |  | |  |  |  | |  |_)  |    |   (----`
|  | |_ | |      /     |  |  |  | |  |  |  | |   ___/      \   \    
|  |__| | |  |\  \----.|  `--'  | |  `--'  | |  |      .----)   |   
 \______| | _| `._____| \______/   \______/  | _|      |_______/    
*/
/**
 * \details Button group is a class that senses a group of push buttons. This class is preferred compared with phi_buttons class.
 * You can assign names to each button so when the button is pressed, the name is returned such as '1', or 'A'.
 * All buttons should connect arduino pins to GND. Internal pull-up resistors are automatically enabled.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each key press.
 * \param sp This is the name of (or pointer to) a byte array that stores all arduino pins used by each button. If you have 4 buttons, the array has 4 elements.
 * \param r This is the number of buttons in this group.
 */
phi_button_groups::phi_button_groups(char *na, byte * sp, byte r) // button_groups constructor
{
  device_type=Button_group;

  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins
  rows=r;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<rows;j++) // Setting sensing rows to input and enabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],HIGH);
  }
}

/**
 * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status. The scanKeypad calls this function and interprets the return into status of the key.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_button_groups::sense_all()
{
  for (byte j=0;j<rows;j++)
  {
    buttonBits=digitalRead(mySensorPins[j]); // Sensing button, using buttonBits as a temp variable.
    if (buttonBits==LOW)
    {
      return (j); // returns the button pressed
    }
  }
  return NO_KEYs; // no buttons pressed
}

//Liudr shift register keypads class member functions
/*
 __       __   __    __   _______  .______      
|  |     |  | |  |  |  | |       \ |   _  \     
|  |     |  | |  |  |  | |  .--.  ||  |_)  |    
|  |     |  | |  |  |  | |  |  |  ||      /     
|  `----.|  | |  `--'  | |  '--'  ||  |\  \----.
|_______||__|  \______/  |_______/ | _| `._____|
*/
/**
 * \details Liudr keypad has column pins and row pins.
 * Column pins are on shift register 0 with 8 total column pins. They are pulled LOW when that column is addressed and left in HIGH when those columns are not addressed so you should not press two buttons together.
 * Row pins are always inputs with pull-up resistors and will be read when that row is addressed. By default there are 2 row pins.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each key press.
 * \param sp This is the name of (or pointer to) a byte array that stores all row pins used by the keypad.
 * \param cp This is the arduino pin for shift register clock.
 * \param dp This is the arduino pin for shift register data.
 * \param lp This is the arduino pin for shift register latch.
 * \param r This is the number of rows of the liudr keypad.
 * \param c This is the number of columns of the liudr keypad. By default it is 8.
 */
phi_liudr_keypads::phi_liudr_keypads(char *na, byte * sp, byte cp, byte dp, byte lp, byte r, byte c)
{
  device_type=Liudr_shift_register_pad;
  ledStatusBits=0;
  buttonBits=255;
  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins
  clockPin=cp;
  dataPin=dp;
  latchPin=lp;
  rows=r;
  columns=c;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<rows;j++) // Setting sensing rows to input and enabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],HIGH);
  }

  pinMode(dataPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);

  updateShiftRegister(ledStatusBits,buttonBits);
}

void phi_liudr_keypads::updateShiftRegister(byte first8, byte next8)
{
  digitalWrite(latchPin, LOW);  // Disable update to the output buffers.
  shiftOut(dataPin, clockPin, MSBFIRST, first8);//MSBFIRST when flat LSBFIRST when standing.
  shiftOut(dataPin, clockPin, LSBFIRST, next8);//MSBFIRST when flat LSBFIRST when standing.
  digitalWrite(latchPin, HIGH);  // Enable update to the output buffers.
}

/**
 * \details You may connect a second shift register and connect up to 8 LEDs to this register. This function can set the status of each of these 8 LEDs.
 * \param led This is the LED number to be set. 0-7.
 * \param on_off This is the status you want to set the LED to, either LOW or HIGH.
 */
void phi_liudr_keypads::setLed(byte led, byte on_off)
{
  bitWrite(ledStatusBits,led,on_off);
  updateShiftRegister(ledStatusBits,buttonBits);
}


/**
 * \details You may connect a second shift register and connect up to 8 LEDs to this register. This function can set the status of each of these 8 LEDs.
 * \param led This is the binary status of all 8 LEDs. If you decide to turn on all LEDs, use 255.
 */
void phi_liudr_keypads::setLedByte(byte led)
{
  ledStatusBits=led;
  updateShiftRegister(ledStatusBits,buttonBits);
}

/**
 * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status. The scanKeypad calls this function and interprets the return into status of the key.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_liudr_keypads::sense_all()
{

  for (byte j=0;j<rows;j++)
  {
    for (byte i=0;i<columns;i++)
    {
        buttonBits=255;
        bitClear(buttonBits,i);
        updateShiftRegister(ledStatusBits,buttonBits);

      buttonBits=digitalRead(mySensorPins[j]); // Sensing button, using buttonBits as a temp variable.

      if (buttonBits==LOW)
      {
        return (i+j*columns); // returns the button pressed
      }
    }
  }
  return NO_KEYs; // no buttons pressed
}

//Liudr analog digital keypads class member functions
/*
  _     _           _     ____  
 | |   (_)_   _  __| |_ _|___ \ 
 | |   | | | | |/ _` | '__|__) |
 | |___| | |_| | (_| | |  / __/ 
 |_____|_|\__,_|\__,_|_| |_____|
 */
 /**
 * \details Analog keypads are made up of several analog pins. Each pin is connected to several buttons and resistors. Find diagram in my blog.
 * All analog pins need to be connected to the same resistor network. If you don't need as many buttons, you can omit some buttons but never omit any resistors.
 * \param na This is the name of (or pointer to) a char array that stores the names corresponding to each key press.
 * \param sp This is the name of (or pointer to) a byte array that stores all analog pins used by the keypad. Unlike the original analogbutton, you can use multiple pins, with each pin connected to a number of buttons to form a keypad.
 * \param dp This is the name of (or pointer to) an integer array that stores the analog values of each button press. The array must be sorted from small to big. If you have 5 buttons, this array should have 5 elements.
 * \param r This is the number of analog pins or "rows" of the analog keypad.
 * \param c This is the number of buttons attached to each analog pin or "columns" of the analog keypad. All analog pins should connect to identical button/resistor configurations.
 *  If you don't need that many buttons for one particular pin, don't forget to connect all the resistors so that the analog values will be the same.
 */
phi_liudr_keypads_2::phi_liudr_keypads_2(char *na, byte * sp, byte asp, byte r, byte c, int * dp)
{
  device_type=Liudr_AD_matrix_pad;
  key_names=na; // Translated names of the keys, such as '0'.
  mySensorPins=sp; // Row pins
  analog_sensing_pin=asp;	// Analog sensing pin
  values=dp; // Points to divider value array.
  rows=r;
  columns=c;
  button_sensed=NO_KEYs; // This indicates which button is sensed or 255 if no button is sensed.
  button_status=buttons_up; // This indicates the status of the button if button_sensed is not 255.
  button_status_t=millis(); // This is the time stamp of the sensed button first in the status stored in button_status.
  
  for (int j=0;j<columns+4;j++) // Setting sensing rows and 4 LED pins to input and disabling internal pull-up resistors.
  {
    pinMode(mySensorPins[j],INPUT);
    digitalWrite(mySensorPins[j],LOW); // To use phi-3 shield with internal pullup, do digitalWrite(A0,HIGH) after simple_setup_phi_3() or after this constructor.
  }
  pinMode(analog_sensing_pin,INPUT);
  digitalWrite(analog_sensing_pin,HIGH);	// Enable internal pullup
}

/**
 * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status.
 * This function is not intended to be call by arduino code but called within the library instead.
 * If all you want is a key press, call getKey.
 * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
 */
byte phi_liudr_keypads_2::sense_all()
{
int temp, diff;

	for (byte k=0;k<columns;k++)
	{
		for (byte j=0;j<columns;j++) // Set all digital sense pins to tri-state
		{
			pinMode(mySensorPins[j],INPUT);
			digitalWrite(mySensorPins[j],LOW);
		}
		pinMode(mySensorPins[k],OUTPUT); // Only set the pin being scanned to output LOW
		digitalWrite(mySensorPins[k],LOW);
		
		temp=analogRead(analog_sensing_pin);
		for (byte i=0;i<rows;i++)
		{
			diff=abs(values[i]-temp); // Find the difference between analog read and stored values.
			if (diff<analog_difference_2) 
			{
				return (i+k*rows); // returns the button pressed
			}
		}
		if (abs(1023-temp)<analog_difference_2) return rows*columns; // The 5V button is pressed.
	}
	return NO_KEYs;
}

/**
 * \details You may connect a second shift register and connect up to 8 LEDs to this register. This function can set the status of each of these 8 LEDs.
 * \param led This is the LED number to be set. 0-7.
 * \param on_off This is the status you want to set the LED to, either LOW or HIGH.
 */
void phi_liudr_keypads_2::setLed(byte led, byte on_off)
{
  pinMode(mySensorPins[columns+led],OUTPUT);
  digitalWrite(mySensorPins[columns+led],on_off);
}


/**
 * \details You may connect a second shift register and connect up to 8 LEDs to this register. This function can set the status of each of these 8 LEDs.
 * \param led This is the binary status of all 8 LEDs. If you decide to turn on all LEDs, use 255.
 */
void phi_liudr_keypads_2::setLedByte(byte led)
{
	for (byte i=0;i<4;i++)
	{
		pinMode(mySensorPins[columns+i],OUTPUT);
		digitalWrite(mySensorPins[columns+i],(led&1));
		led=led>>1;
	}
}
