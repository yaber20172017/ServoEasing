# ServoEasing - move your servo more natural
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/ServoEasing.svg?)](https://www.ardu-badge.com/ServoEasing)
[![Commits since latest](https://img.shields.io/github/commits-since/ArminJo/ServoEasing/latest)](https://github.com/ArminJo/ServoEasing/commits/master)
[![Build status](https://travis-ci.org/ArminJo/ServoEasing.svg?branch=master)](https://travis-ci.org/ArminJo/ServoEasing)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FServoEasing)](https://github.com/brentvollebregt/hit-counter)

YouTube video of ServoEasing in action

[![Demonstration of different servo easings](https://i.ytimg.com/vi/fC9uxdOBhfA/hqdefault.jpg)](https://www.youtube.com/watch?v=fC9uxdOBhfA)

## Servo easing library for Arduino
This is a library for smooth servo movements. It uses the standard Arduino Servo library and therefore has its restrictions regarding pins and platform support.<br/>
As an alternative to the Arduino Servo library, ServoEasing can be used with a [PCA9685 servo expander](https://learn.adafruit.com/16-channel-pwm-servo-driver?view=all) using the Arduino Wire library or a compatible one (and their restrictions).<br/>
For **ESP32** you need to install the Arduino ESP32Servo library.<br/><br/>
For **AVR**, if you need only one or two servos, you may want to use the included [Lightweight Servo library](https://github.com/ArminJo/LightweightServo) instead of the Arduino Servo library because it uses only the internal Timer1 with no software overhead and has no problems with interrupt blocking libraries like SoftwareSerial, Adafruit_NeoPixel and DmxSimple.<br/>
For instructions how to enable these alternatives see [Modifying library properties](https://github.com/ArminJo/ServoEasing#modifying-library-properties)

### Features
- **Linear** and 9 other ease movements are provided.
- All servos can move **synchronized** or **independently**.
- **Non blocking** movements are enabled by using **startEaseTo\* functions** by reusing the interrupts of the servo timer Timer1 or using a dedicated timer on other platforms. This function is not available for all platforms.
- Trim value for each servo may be set.
- Reverse operation of servo is possible eg. if it is mounted head down.
- Allow to specify an arbitrary mapping between degreen and microseconds by 'attach(int aPin, int aMicrosecondsForServoLowDegree, int aMicrosecondsForServoHighDegree, int aServoLowDegree, int aServoHighDegree)' 

## Usage
Just call **myServo.startEaseTo()** instead of **myServo.write()** and you are done. Or if you want to wait (blocking) until servo has arrived, use **myServo.easeTo()**. Speed of movement can be set by **myServo.setSpeed()**.<br/>
Do not forget to **set the start position** for the Servo by simply calling **myServo.write()**, since the library has **no knowledge about your servos initial position** and therefore starts at **0 degree** at the first move, which may be undesirable.

### Includes the following **easing functions**:
- Linear
- Quadratic
- Cubic
- Quartic
- Sine (similar to Quadratic)
- Circular
- Back
- Elastic
- Bounce
- User defined

### All easing functions can be used in the following **variants**:
- In
- Out
- InOut
- Bouncing (mirrored Out) e.g. Bouncing of the Sine function results in the upper (positive) half of the sine.

## Useful resources
- [Easings Cheat Sheet](https://easings.net/)
- [Robert Penner](http://www.robertpenner.com/easing/)
- [C functions on Github](https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c)
- [Interactive cubic-bezier](http://cubic-bezier.com)

# Internals
Internally only microseconds (or units (= 4.88 us) if using PCA9685 expander) and not degree are used to speed up things. Other expander or libraries can therefore easily be added.<br/>
On **AVR** Timer1 is used for the Arduino Servo library. To have non blocking easing functions its unused **Channel B** is used to generate an interrupt 100 us before the end of the 20 ms Arduino Servo refresh period. This interrupt then updates all servo values for the next refresh period.

# Modifying library properties
To access the Arduino library files from a sketch, you have to first use `Sketch/Show Sketch Folder (Ctrl+K)` in the Arduino IDE.<br/>
Then navigate to the parallel `libraries` folder and select the library you want to access.<br/>
The library files itself are located in the `src` sub-directory.<br/>
If you did not yet store the example as your own sketch, then with Ctrl+K you are instantly in the right library folder.

## Using PCA9685 16-Channel Servo Expander
To enable the use of the expander, open the library file ServoEasing.h and comment out line 37 or define global symbol `USE_PCA9685_SERVO_EXPANDER` which is not yet possible in Arduino IDE:-(.<br/>
Timer1 is then only needed for the startEaseTo* functions.

## Using the included [Lightweight Servo library](https://github.com/ArminJo/LightweightServo)
Using the **Lightweight Servo Library** reduces sketch size and makes the servo pulse generating immune to other libraries blocking interrupts for a longer time like SoftwareSerial, Adafruit_NeoPixel and DmxSimple.<br/>
Up to 2 servos are supported by this library and they must be attached to pin 9 and/or 10.<br/>
To enable it, open the library file ServoEasing.h and comment out line 46 or define global symbol `USE_LEIGHTWEIGHT_SERVO_LIB` which is not yet possible in Arduino IDE:-(.<br/>
If not using the Arduino IDE, take care that Arduino Servo library sources are not compiled / included in the project.

## Reducing library size
If you have only one or two servos, then you can save program space by using Lightweight Servo library .
This saves 742 bytes FLASH and 42 bytes RAM.<br/>
If you do not need the more complex easing functions like `Sine` etc., which in turn need sin(), cos(), sqrt() and pow(), you can shrink library size by approximately 1850 bytes by commenting out line 97 in ServoEasing.h or define global symbol `KEEP_LIBRARY_SMALL` which is not yet possible in Arduino IDE:-(.<br/>

# [Examples](https://github.com/ArminJo/ServoEasing/tree/master/examples)
All examples with up to 2 Servos can be used without modifications with the [Lightweight Servo library](https://github.com/ArminJo/LightweightServo) for AVR by by commenting out line 46 in the file ServoEasing.h (see above).

## Simple example
This example does not use interrupts and should therefore run on any platform where the Arduino Servo library is available.<br/><br/>
**Arduino Serial Plotter** result of Simple example if `#define TRACE_FOR_SERIAL_PLOTTER` in `ServoEasing.cpp` is enabled.
![Arduino plot](https://github.com/ArminJo/ServoEasing/blob/master/extras/ServoEasing-Linear-Cubic-Circular.png)

## OneServo example
This example moves one Servo with different speeds and using blocking and interrupt commands. The internal LED blinks when using interrupt based commands

## TwoServo and ThreeServo examples
This example shows how to move 2 or 3 servos synchronized or independently.

## SymmetricEasing example
This example shows symmetric (end movement is mirror of start movement) linear, quadratic and cubic movements for 3 servos synchronously.

## AsymmetricEasing example
This example shows asymmetric (end movement is different from start movement) partially **user defined** (line 134) non linear movements for 3 servos synchronously.

## CatMover example
Demo of using two servos in a pan tilt housing to move a laser pointer.

## QuadrupedControl example
Control 8 servos to move a Quadruped robot.<br/>
The full example with IR remote control, NeoPixel and US distance sensor support is available [here](https://github.com/ArminJo/QuadrupedControl).

### YouTube Video
[![mePed V2 in actions](https://i.ytimg.com/vi/cLgj_sr7f1o/hqdefault.jpg)](https://youtu.be/cLgj_sr7f1o)

## RobotArmControl example
Program for controlling a robot arm with 4 servos using 4 potentiometers and/or an IR Remote.

## PCA9685_Expander example
The OneServo example modified for using a PCA9685 expander board and the standard Arduino Wire library.

## EndPositionsTest example
This example helps you determine the right end values for your servo.<br/>
These values are needed for the `attach()` function, if your servo does not comply to the standard values.
E.g. some of my SG90 servos have a 0 degree period of 620 us instead of the standard 544.<br/>
This example does not use the ServoEasing functions.

## SpeedTest example
This example gives you a feeling how fast your servo can move, what the end position values are and which refresh rate they accept.<br/>
This example does not use the ServoEasing functions.

# Supported Platforms
AVR, ESP8266, ESP32, STMF1
Every platform with a Servo library will work in blocking mode. If timer support is available for a platform the library can be ported by adding code for the Timer20ms like is was done for ESP and STM.
Non blocking behavior can always be achieved manually by calling update() in a loop - see last movement in [Simple example](https://github.com/ArminJo/ServoEasing/blob/master/examples/Simple/Simple.ino).

# Revision History
### Version 1.4.0
- setTrim has additional parameter 'doWrite' which is default 'false' in contrast to older versions, where a write was always performed.
- **New attach**( aPin,  aMicrosecondsForServoLowDegree,  aMicrosecondsForServoHighDegree,  aServoLowDegree,  aServoHighDegree) function for arbitrary mapping of servo degree to servo pulse width.
- Order of Servos in 'sServoArray[]' now depends from order of calling attach() and not from order of declaration.
### Version 1.3.1
- Added detach() function.
### Version 1.3.0
- Added ESP32 support by using ESP32Servo.h and Ticker.h instead of Servo.h timer interrupts.
- Changed degree parameter and values from uint8_t to integer to support operating a servo from -90 to + 90 degree with 90 degree trim.
- RobotArmControl + QuadrupedControl examples refactored.
- Changed "while" to "for" loops to avoid a gcc 7.3.0 atmel6.3.1 bug.
- Extended SpeedTest example. Now also able to change the width of the refresh period.

### Version 1.2
- Added ESP8266 support by using Ticker instead of timer interrupts for ESP.
- AsymetricEasing example overhauled.

### Version 1.1.0
- Corrected sine, circular, back and elastic IN functions.
- easeTo() and write() store their degree parameter now also in sServoNextPositionArray.
- added setSpeed(), getSpeed(), setSpeedForAllServos() and added ease* functions without speed parameter.
- added getEndMicrosecondsOrUnits(), getDeltaMicrosecondsOrUnits().
- added setDegreeForAllServos(uint8_t aNumberOfValues, va_list * aDegreeValues),setDegreeForAllServos(uint8_t aNumberOfValues, ...).
- added compile switch PROVIDE_ONLY_LINEAR_MOVEMENT to save additional 1500 bytes FLASH if enabled.
- added convenience function clipDegreeSpecial().

### Version 1.0.0
Initial Arduino library version

# Travis CI
The ServoEasing library examples are built on Travis CI for the following boards:

- Arduino Uno
- Arduino Leonardo
- Arduino cplayClassic
- Arduino Mega 2560
- ESP8266 boards (tested with LOLIN D1 R2 board)
- ESP32   boards (tested with ESP32 DEVKITV1 board)

## Requests for modifications / extensions
Please write me a PM including your motivation/problem if you need a modification or an extension e.g. a callback functionality after move has finished.

#### If you find this library useful, please give it a star.
