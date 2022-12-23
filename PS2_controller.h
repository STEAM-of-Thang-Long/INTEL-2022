#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class object

#define PS2_DAT 12 // MISO  19
#define PS2_CMD 13 // MOSI  23
#define PS2_SEL 15 // SS     5
#define PS2_CLK 14 // SLK   18

#define TOP_SPEED 4095
#define NORM_SPEED 1500
#define TURNING_FACTOR 1

#define SINGLE_HAND_DRIVING 0
#define TWO_HAND_DRIVING 1
int servo = 145;
int cuon = 0;
bool driving_mode = SINGLE_HAND_DRIVING;
void setupPS2controller()
{
  int err = -1;
  while (err != 0)
  {
    err = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
  }
}
bool PS2control()
{
  int speed = NORM_SPEED;
  if (ps2x.Button(PSB_R2))
    speed = TOP_SPEED;
   if (ps2x.ButtonPressed(PSB_SELECT))
    driving_mode =! driving_mode;

  int nJoyX = 128 - ps2x.Analog(PSS_RX); // read x-joystick
  int nJoyY = 128 - (driving_mode ? ps2x.Analog(PSS_LY) :ps2x.Analog(PSS_RY)); // read y-joystick
  int nMotMixL;                          // Motor (left) mixed output
  int nMotMixR;                          // Motor (right) mixed output

 
  bool temp = (nJoyY * nJoyX > 0);
  if (nJoyX) // Turning
  {
  nMotMixL = -nJoyX + (nJoyY * temp);
  nMotMixR = nJoyX + (nJoyY * !temp);
  }
  else // Forward or Reverse
  {
    nMotMixL = nJoyY;
    nMotMixR = nJoyY;
  }


  int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (nMotMixR > 0)
  {
    c3 = nMotMixR;
    c3 = map(c3, 0, 128, 0, speed);
  }

  else if (nMotMixR < 0)
  {
    c4 = abs(nMotMixR) + 1;
    c4 = map(c4, 0, 128, 0, speed);
  }

  if (nMotMixL > 0)
  {
    c1 = nMotMixL;
    c1 = map(c1, 0, 128, 0, speed);
  }
  else if (nMotMixL < 0)
  {
    c2 = abs(nMotMixL)+1;
    c2 = map(c2, 0, 128, 0, speed);
  }
  if ( ps2x.Button(PSB_PAD_LEFT)){
    servo = 80;

  }
  if ( ps2x.Button(PSB_PAD_RIGHT) ){
    servo = 300;
 
  }
  if ( ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_DOWN)  ){
    servo = 145;
 
  }
  if ( ps2x.Button(PSB_RED) ){
    cuon = 1024;

  }
  if ( ps2x.Button(PSB_PINK)){
    cuon = 2048;

  }
    if ( ps2x.Button(PSB_BLUE)){
    cuon = 0;

  }
  setPWMMotors(c1, c2, c3, c4);
  Serial.print ( "servo:" );
  quayservo(servo);
  roller(cuon);
//  Serial.println (servo);  
  return 1;
}
