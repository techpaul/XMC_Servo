/* Servo.h - Hardware PWM driven Servo library for Infineon XMC series using 16 bit timers
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
/*
A servo is activated by creating an instance of the Servo class.
Passing the desired pin to the attach() method (with default or user limits).
The servos are pulsed by Hardware PWM using the value most recently
written using the write() method.

Note The limit of how many servos can be assigned depends on the number of PWM
     pins available on your board

Class
  Servo - Class for manipulating servo motors connected to Arduino pins.

The methods are:
  attach( pin )         - Attach a servo motor to a PWM pin.
                            default min is 544, max is 2400
  attach( pin, min )    - Attach to a pin setting min value in microseconds max = 2400
  attach( pin, min, max ) - Attach to a pin setting min and max values in microseconds
  detach()              - Stops an attached servos and frees resources (H/W)
  write( value )        - Sets the servo angle in degrees. when value < 500
                          when value >= 500 set pulse width in microseconds
                          (value clipped to 0 to 180 degrees or min/max microseconds)
  writeMicroseconds( value ) - Sets the servo pulse width in microseconds
                          (value clipped to min/max microseconds)
  read()                - Gets the last written servo pulse width as angle 0 to 180.
  readMicroseconds()    - Gets the last written servo pulse width in microseconds.
*/

#ifndef _XMC_Servo_h
#define _XMC_Servo_h

// Architecture specific check
#if not (defined(ARDUINO_ARCH_ARM) || defined(ARDUINO_ARCH_XMC) )
#error "This library only supports boards with an Infineon XMC Arm processor."
#endif

// the shortest pulse sent to a servo
#define MIN_PULSE_WIDTH       544
// the longest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400
// default pulse width and angle when servo is attached
#define DEFAULT_PULSE_WIDTH  1500
#define DEFAULT_ANGLE          90
#define REFRESH_FREQUENCY      50u
// in microseconds refresh time
#define REFRESH_TIME        ( 1000000u / REFRESH_FREQUENCY )
// flag indicating an invalid servo index
#define INVALID_SERVO         255


class Servo
{
public:
  Servo();
  uint8_t attach( int );            // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach( int, int );       // as above but also sets min value for writes.
  uint8_t attach( int, int, int  ); // as above but also sets min and max values for writes.
  void detach( );
  void write( int );                // if value is < 500 its treated as an angle, otherwise as pulse width in microseconds
  void writeMicroseconds( int );    // Write pulse width in microseconds
  int read( );                      // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds( );          // returns current pulse width in microseconds for this servo (was read_us() in first release)
  bool attached();
private:
   int16_t      _updatePWM( uint8_t, int );
   uint8_t      _servoIndex;        // index into the channel data for this servo
   uint8_t      _deg;               // last write as degrees
   uint16_t     _width;             // last write as time
   uint16_t     _minimum;           // minimum is this value or MIN_PULSE_WIDTH
   uint16_t     _maximum;           // maximum is this value or MAX_PULSE_WIDTH
};

#endif
