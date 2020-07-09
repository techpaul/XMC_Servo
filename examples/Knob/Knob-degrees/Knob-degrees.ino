/* Knob to 2 channels RC Servo (degrees) Demo - XMC_Servo
 Controlling TWO servo positions using a potentiometer (variable resistor)
 Copyright (c) 2020 Paul Carpenter - PC Services.  All right reserved.

 Repeat at 10 Hz (10 times a second)
 One pot is read, and two Servos controlled anti-phase by degrees control
 ServoA is driven direct value mapped from pot 0-180
 ServoB is driven as reversed angle 180-0
 PWM output will continue between updates

 LED1 the Built-in LED is used when Pot is at centre position (90&deg;) the LED
 is turned ON.

 If both LED1 and LED2 are permanently ON then an invalid pin for PWM has been
 attempted to be attached to a Servo class

 Wiring
    Pot is connected to analogue input A1
            (XMC1300 Boot Kit has on-board pot on this pin)
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
#define POT_PIN     A1

Servo ServoA;
Servo ServoB;

/* Create an LED object */
LED Led;

int val;            // the analogue pin value


void setup( )
{
Led.Add( LED1 );            // Configure the LEDs
Led.Off( LED1 );            // Set default state of LEDs
Led.Add( LED2 );
Led.Off( LED2 );

// attaches the PWM pins to the servo object
val = 0;
if( (ServoA.attach( SERVO_A_PIN ) ) == INVALID_SERVO )
  val++;
else
  if( (ServoB.attach( SERVO_B_PIN ) ) == INVALID_SERVO )
    val++;

if( val )           // Invalid pin trap
  {
  Led.On( LED1 );
  Led.On( LED2 );
  while( 1 );       // Stick in error loop
  }
}


void loop( )
{
val = analogRead( POT_PIN );         // reads potentiometer (value 0 - 1023)
val = map( val, 0, getAnalogReadMaximum(), 0, 180 ); // scale to angle (value between 0 - 180)

ServoA.write( val );                 // sets servo position to scaled value
ServoB.write( 180 - val );           // sets servo position to Reversed scaled value

if( val == DEFAULT_ANGLE )           // When 90 degrees light LED
  Led.On( LED1 );
else
  Led.Off( LED1 );

delay( 100 );
}
