/* Sweep 4 channels RC Servo (degrees) Demo - XMC_Servo
 Controlling FOUR servo positions by programme sweep in opposite directions
 Copyright (c) 2020 Paul Carpenter - PC Services.  All right reserved.

 Update new values at 10 Hz (10 times a second)
 One counter is used, and four Servos controlled anti-phase by degrees control
 Sweeps counter from min to max, then max to min and repeats
 ServoA is driven direct value mapped from counter 0-180
 ServoB is driven as reversed angle 180-0
 ServoC is same as Servo A with limited range so will have smaller physical steps
 ServoD is same as Servo B with limited range so will have smaller physical steps
 PWM output will continue between updates

 LED1 the Built-in LED is used when Pot is at centre position (90&deg;) the LED
 is turned ON.

 If both LED1 and LED2 are permanently ON then an invalid pin for PWM has been
 attempted to be attached to a Servo class

 Wiring
    Default PWM pins are 3, 4, 6, and 9

    Some Board variant PWM Pins
    XMC1100 Boot Kit    3, 4, 6, 9
    XMC1100 XMC2Go      8, 3, 2, 1
    XMC1300 Boot Kit    31, 26, 32, 33
    XMC4700 Relax       3, 10, 11, 95, 72, 96, 63, 36, 78, 90, 91,
                        5,  6, 9, 53, 39, 64, 38, 68, 79, 83, 82, 81
 */
#include <Servo.h>
#include <LED.h>

// Adjust the following defines to match your setup
// Pins for Servos and pot
#define SERVO_A_PIN 3
#define SERVO_B_PIN 4
#define SERVO_C_PIN 6
#define SERVO_D_PIN 9

// 3 degree steps at 10 Hz, gives 6 seconds one sweep in one direction
#define STEP    3

#define MIN_TIME    600
#define MAX_TIME    2400
#define LIMIT_LOW   1000
#define LIMIT_HIGH  2000

Servo ServoA;
Servo ServoB;
Servo ServoC;
Servo ServoD;

/* Create an LED object */
LED Led;

int val = 0;            // the counter value
int direction = 1;      // 1 = count up, 0 = count down


void setup( )
{
Led.Add( LED1 );            // Configure the LEDs
Led.Off( LED1 );            // Set default state of LEDs
Led.Add( LED2 );
Led.Off( LED2 );

// attaches the PWM pins to the servo objects
val = 0;
if( (ServoA.attach( SERVO_A_PIN ) ) == INVALID_SERVO )
  val++;
else
 if( (ServoB.attach( SERVO_B_PIN ) ) == INVALID_SERVO )
   val++;
 else   // attach PWMs with limited range
   if( (ServoC.attach( SERVO_C_PIN, LIMIT_LOW, LIMIT_HIGH ) ) == INVALID_SERVO )
     val++;
   else
     if( (ServoD.attach( SERVO_D_PIN, LIMIT_LOW, LIMIT_HIGH ) ) == INVALID_SERVO )
       val++;

if( val )           // Invalid pin trap
  {
  Led.On( LED1 );
  Led.On( LED2 );
  while( 1 );       // Stick in error loop
  }

val = 0;                    // start at zero degrees
direction = 1;              // counting up
}


void loop( )
{
ServoA.write( val );                // sets servo position to value
ServoB.write( 180 - val );          // sets servo position to Reversed value
ServoC.write( val );                // sets servo position to scaled value
ServoD.write( 180 - val );          // sets servo position to Reversed value

// When Mid position light LED1
if( val == DEFAULT_ANGLE )
  Led.On( LED1 );
else
  Led.Off( LED1 );
// Check for end of sweep direction
if( direction )
  if( val >= 180 )
    direction = 0;
  else
    val += STEP;
else
  if( val <= 0 )
    direction = 1;
  else
    val -= STEP;

delay( 100 );
}
