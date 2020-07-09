/* Sweep 4 channels RC Servo (time) Demo - XMC_Servo
 Controlling Four servo positions by programme sweep in opposite directions
 Copyright (c) 2020 Paul Carpenter - PC Services.  All right reserved.

 Update new values at 10 Hz (10 times a second)
 One counter is used, and four Servos controlled anti-phase by degrees control
 Sweeps counter from min to max, then max to min and repeats
 ServoA is driven direct value mapped from counter 600 - 2400
 ServoB is driven as reversed angle 2400 - 600
 ServoC is same as Servo A with limited range so will have smaller physical steps
 ServoD is same as Servo B with limited range so will have smaller physical steps
 PWM output will continue between updates

 LED1 the Built-in LED is used when Pot is at centre position (90&deg;) the LED
 is turned ON.

 LED2 on-board LED to indicate Servo C and Servo D in operating range and
 values not being clipped

 If both LED1 and LED2 are permanently ON then an invalid pin for PWM has been
 attempted to be attached to a Servo class

 Wiring
    Default PWM pins are 3 and 4

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

// 30 micro second steps at 10 Hz, gives 6 seconds one sweep in one direction
#define STEP    30

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

int val;                // the counter value
int direction;          // 1 = count up, 0 = count down


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

val = MIN_TIME;             // start at minimum pulse width
direction = 1;              // counting up
}


void loop( )
{
ServoA.writeMicroseconds( val );                // sets servo position to value
ServoB.writeMicroseconds( MAX_TIME - val + MIN_TIME ); // sets servo position to Reversed value
ServoC.writeMicroseconds( val );                // sets servo position to value
ServoD.writeMicroseconds( MAX_TIME - val + MIN_TIME ); // sets servo position to Reversed value

// When Mid position light LED1
if( val >= 1490 && val <= 1510 )
  Led.On( LED1 );
else
  Led.Off( LED1 );
// When ServoC and ServoD in operating range light LED2
if( val >= LIMIT_LOW && val <= LIMIT_HIGH )
  Led.On( LED2 );
else
  Led.Off( LED2 );

// Check for end of sweep direction
if( direction )
  if( val >= MAX_TIME )
    direction = 0;
  else
    val += STEP;
else
  if( val <= MIN_TIME )
    direction = 1;
  else
    val -= STEP;

delay( 100 );
}
