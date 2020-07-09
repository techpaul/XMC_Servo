/* Test 3 channels RC Servo (time) - XMC_Servo
 Controlling THREE servo positions using a Fixed position
 Copyright (c) 2020 Paul Carpenter - PC Services.  All right reserved.

 Set up THREE servo outputs to fixed position for accurate measurements of
 Min, Max and Mid positions, from Servo library defines for these limits

 If both LED1 and LED2 are permanently ON then an invalid pin for PWM has been
 attempted to be attached to a Servo class

 Wiring
    Default PWM pins are 3, 4 and 6

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
//#define SERVO_A_PIN 3
//#define SERVO_B_PIN 4
//#define SERVO_C_PIN 6
// XMC1300 Boot Kit Wiring
#define SERVO_A_PIN 31
#define SERVO_B_PIN 26
#define SERVO_C_PIN 32


// Servo objects
Servo ServoA;
Servo ServoB;
Servo ServoC;

LED Led;        /* Create an LED object */

int val;


void setup( )
{
Led.Add( LED1 );            // Configure the LEDs
Led.Add( LED2 );
Led.Off( LED1 );            // Set default state of LEDs
Led.Off( LED2 );

// attaches the PWM pins to the servo objects and check for errors
val = 0;
if( (ServoA.attach( SERVO_A_PIN ) ) == INVALID_SERVO )
  val++;
else
  if( (ServoB.attach( SERVO_B_PIN ) ) == INVALID_SERVO )
    val++;
  else
    if( (ServoC.attach( SERVO_C_PIN ) ) == INVALID_SERVO )
      val++;

if( val )           // Invalid pin trap for not a PWM pin
  {
  Led.On( LED1 );
  Led.On( LED2 );
  // while( 1 );       // Stick in error loop
  }
else
  {  
  ServoA.writeMicroseconds( DEFAULT_PULSE_WIDTH );
  ServoB.writeMicroseconds( MIN_PULSE_WIDTH );
  ServoC.writeMicroseconds( MAX_PULSE_WIDTH );
  }
}


void loop( )
{
// Do nothing loop
}
