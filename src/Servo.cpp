/* Servo.cpp - Hardware PWM driven Servo library for Infineon XMC series using 16 bit timers
Copyright (c) 2020 Paul Carpenter - PC Services.  All right reserved.

This library is free software; you can redistribute it and/or  modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2.1 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA  02110-1301,  USA. */
#if not (defined(ARDUINO_ARCH_ARM) || defined(ARDUINO_ARCH_XMC) )
#error "This library only supports boards with an Infineon XMC Arm processor."
#endif

#include <Arduino.h>
#include "Servo.h"

// Get board variant number of PWMs
#define MAX_SERVOS  NUM_PWM
// PWM resolution in bits
#define SERVO_RES   16
// threshold for write treat as degrees or time
#define DEGREE_THRESHOLD      500
// Class instantiated not assigned
#define DETACHED_SERVO        254

static uint8_t _servos[ MAX_SERVOS ];   // static array of servo pins in use
uint8_t     _ServoCount = 0;            // the total number of attached servos
uint16_t    _max_duty = 0;              // value of max resolution for 100% duty


Servo::Servo()
{
if( _ServoCount < MAX_SERVOS )
  {
  _servoIndex = _ServoCount++;     // assign a servo index to this instance
  _width = DEFAULT_PULSE_WIDTH;    // store default values
  _deg = DEFAULT_ANGLE;
  _minimum = MIN_PULSE_WIDTH;
  _maximum = MAX_PULSE_WIDTH;
  _servos[ _servoIndex ] = DETACHED_SERVO;
  }
else
  _servoIndex = INVALID_SERVO ;      // too many servos
}


/* _updatePWM us to duty cycle to PWM
    Pass in integer microseconds
    Save current width
    Send to PWM value as duty cycle of max value of resolution
    Assumes valid pin and setup
*/
int16_t Servo::_updatePWM( uint8_t pin, int val )
{
_width = val;
val = map( val, 0, REFRESH_TIME, 0, _max_duty );
return analogWrite( pin, val );
}


uint8_t Servo::attach( int pin )
{
return attach( pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH );
}


uint8_t Servo::attach( int pin, int min )
{
return attach( pin, min, MAX_PULSE_WIDTH );
}


/* Attach common method
Checks parameters (error exit if not valid)
Limits clipped if beyond absolute limits
Set valid pin low and update pin array
Updates limits
Initialise PWM on pin at default values (centred)

Returns index into pin array if successful
        INVALID_SERVO for errors
*/
uint8_t Servo::attach( int pin, int new_min, int new_max )
{
if( _servoIndex < MAX_SERVOS )
  {  // Check detached servo to attach to
  if( _servos[ _servoIndex ] != DETACHED_SERVO )
    return INVALID_SERVO;
  if( new_min >= new_max )            // Check valid ranges
    return INVALID_SERVO;
  if( new_min < MIN_PULSE_WIDTH )     // Clip Limits
    new_min = MIN_PULSE_WIDTH;
  if( new_max > MAX_PULSE_WIDTH )
    new_max > MAX_PULSE_WIDTH;
  // Check PWM pin not already used
  for( int i = 0; i < MAX_SERVOS; i++ )
     if( _servos[ i ] == pin )
       return INVALID_SERVO;
  if( ( setAnalogWriteFrequency( pin, REFRESH_FREQUENCY ) < 0 ) )
    return INVALID_SERVO;              // Invalid PWM pin or frequency

  // Valid details to use now
  if( _max_duty == 0 )
    {
    analogWriteResolution( SERVO_RES );
    _max_duty = getAnalogWriteMaximum( );
    }
  // Set going using saved default
  _servos[ _servoIndex ] = pin;
  _minimum  = new_min;
  _maximum  = new_max;
  _updatePWM( pin, _width );
  }
return _servoIndex;
}


/* detach return servo to initial state and pin to 0
  This allows servo to be reattached to same or different pin
*/
void Servo::detach( )
{
_width = DEFAULT_PULSE_WIDTH;               // Set width to default in case reattached
_deg = DEFAULT_ANGLE;
if( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO )
  {
  _updatePWM( _servos[ _servoIndex ], 0 );
  _servos[ _servoIndex ] = DETACHED_SERVO;  // set detached servo in pin array
  }
}


// Set PWM to position from degrees (0 to 180)
void Servo::write( int value )
{
if( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO )
  {
  if( value < DEGREE_THRESHOLD )
    {  // treat values < Threshold as angles in degrees
    constrain( value, 0, 180 );
    _deg = value;
    // Quadrant safe map degrees to time (for large imbalance min, centre, max)
    if( _deg == 90 )            // force 90 to default timing
      value = DEFAULT_PULSE_WIDTH;
    else    // < 90 scale to minimum to default range
      if( _deg < 90 )
        value = map( value, 0, 90, _minimum, DEFAULT_PULSE_WIDTH );
      else  // > 90 scale to default to maximum range
        value = map( value, 90, 180, DEFAULT_PULSE_WIDTH, _maximum );
    _updatePWM( _servos[ _servoIndex ], value );
    }
  else  // value >= 500 process as microseconds call
    writeMicroseconds( value );
  }
}


// Set PWM to value from microseconds value (min to max for channel)
// Calculate and store the values for the given channel
// Call common update function to set new PWM
void Servo::writeMicroseconds( int value )
{
// ensure channel is valid
if( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO )
  {
  constrain( value, _minimum, _maximum );      // ensure pulse width is valid
  // save as converted degrees for any read
  if( value == DEFAULT_PULSE_WIDTH )
    _deg = 90;
  else    // < centre time scale to minimum to default range
    if( value < DEFAULT_PULSE_WIDTH )
      _deg = map( value, _minimum, DEFAULT_PULSE_WIDTH, 0, 90 );
    else  // > centre time scale to default to maximum range
      _deg = map( value, DEFAULT_PULSE_WIDTH, _maximum, 90, 180 );
  _updatePWM( _servos[ _servoIndex ], value );
  }
}


int Servo::read()   // return last written value as degrees
{
if( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO )
  return _deg;
return 0;
}


int Servo::readMicroseconds()   // return last written value as microseconds
{
if( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO )
  return _width;
return 0;
}


bool Servo::attached()
{
return ( _servoIndex < MAX_SERVOS && _servos[ _servoIndex ] != DETACHED_SERVO ) ? true :  false;
}
