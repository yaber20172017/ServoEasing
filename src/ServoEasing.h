/*
 * ServoEasing.h
 *
 *  Copyright (C) 2019  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of ServoEasing https://github.com/ArminJo/ServoEasing.
 *
 *  ServoEasing is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#ifndef SERVOEASING_H_
#define SERVOEASING_H_

/*  *****************************************************************************************************************************
 *  To access the library files from your sketch, you have to first use `Sketch/Show Sketch Folder (Ctrl+K)` in the Arduino IDE.
 *  Then navigate to the parallel `libraries` folder and select the library you want to access.
 *  The library files itself are located in the `src` sub-directory.
 *  If you did not yet store the example as your own sketch, then with Ctrl+K you are instantly in the right library folder.
 *  *****************************************************************************************************************************/
/*
 * For use with e.g. the Adafruit PCA9685 16-Channel Servo Driver aOffUnits.
 * To enable it, open the library file ServoEasing.h and comment out line 32.
 */
//#define USE_PCA9685_SERVO_EXPANDER

/*
 * If you have only one or two servos, then you can save program space by defining symbol `USE_LEIGHTWEIGHT_SERVO_LIB`.
 * This saves 742 bytes FLASH and 42 bytes RAM.
 * Using Lightweight Servo library (or PCA9685 servo expander) makes the servo pulse generating immune
 * to other libraries blocking interrupts for a longer time like SoftwareSerial, Adafruit_NeoPixel and DmxSimple.
 * If not using the Arduino IDE take care that Arduino Servo library sources are not compiled / included in the project.
 */
//#define USE_LEIGHTWEIGHT_SERVO_LIB
#if defined(USE_PCA9685_SERVO_EXPANDER) && defined(USE_LEIGHTWEIGHT_SERVO_LIB)
#error "Please define only one of the symbols USE_PCA9685_SERVO_EXPANDER or USE_LEIGHTWEIGHT_SERVO_LIB"
#endif

#if defined(USE_PCA9685_SERVO_EXPANDER) || defined(USE_LEIGHTWEIGHT_SERVO_LIB)
#define REFRESH_INTERVAL 20000

#if defined(USE_PCA9685_SERVO_EXPANDER)
#include <Wire.h>
#ifndef MAX_SERVOS
#define MAX_SERVOS 16 // one PCA9685 has 16 outputs.
#endif

#elif defined(USE_LEIGHTWEIGHT_SERVO_LIB)
#include "LightweightServo.h"
#ifndef MAX_SERVOS
#define MAX_SERVOS 2 // default value for UNO etc.
#endif
#endif

#else  // defined(USE_PCA9685_SERVO_EXPANDER) || defined(USE_LEIGHTWEIGHT_SERVO_LIB)
#include <Servo.h>
#ifndef MAX_SERVOS
#define MAX_SERVOS 12 // default value for UNO etc.
#endif
#endif // defined(USE_PCA9685_SERVO_EXPANDER) || defined(USE_LEIGHTWEIGHT_SERVO_LIB)

/*
 * Define `KEEP_LIBRARY_SMALL` if space (2100 Bytes) matters.
 * The saving comes mainly from avoiding the sin() cos() sqrt() and pow() library functions in this code.
 * If you need only one complex easing function and want to save space, you can specify it any time as a user functions. See AsymmetricEasing example line 58.
 */
//#define KEEP_LIBRARY_SMALL
//
// Enable this if you want to measure timing by toggling pin12 on an arduino
//#define MEASURE_TIMING
/*
 * Enable this to see information on each call.
 * Since there should be no library which uses Serial, enable TRACE only for development purposes.
 */
//#define TRACE
#define DEBUG
// Propagate debug level
#ifdef TRACE
#define DEBUG
#endif
#ifdef DEBUG
#define INFO
#endif
#ifdef INFO
#define WARN
#endif
#ifdef WARN
#define ERROR
#endif

#define DEFAULT_MICROSECONDS_FOR_0_DEGREE 544
#define DEFAULT_MICROSECONDS_FOR_180_DEGREE 2400

#define DEFAULT_PCA9685_UNITS_FOR_0_DEGREE  111 // 111.411 = 544 us
#define DEFAULT_PCA9685_UNITS_FOR_180_DEGREE 491 // 491.52 = 2400 us
/*
 * The different easing functions:
 *
 * In order to reuse the IN functions for OUT and IN_OUT functions, the following call and result conversions are used internally.
 * 1. Using IN function direct: Call with PercentageOfCompletion | 0.0 to 1.0. Result is from 0.0 to 1.0
 * 2. Using IN function to generate OUT function: Call with (1 - PercentageOfCompletion) | 1.0 to 0.0. Result = (1 - result)
 * 3. Using IN function to generate IN_OUT function:
 *      In the first half, call with (2 * PercentageOfCompletion) | 0.0 to 1.0. Result = (0.5 * result)
 *      In the second half, call with (2 - (2 * PercentageOfCompletion)) | 1.0 to 0.0. Result = ( 1- (0.5 * result))
 * 4. Using IN function to generate bouncing_OUT_IN / mirrored_OUT function, which return to start point (like the upper half of a sine):
 *      In the first half, call with (1 - (2 * PercentageOfCompletion)) | 1.0 to 0.0. Result = (1 - result) -> call OUT function 2 times faster.
 *      In the second half, call with ((2 * PercentageOfCompletion) - 1) | 0.0 to 1.0. Result = (1- result) -> call OUT function 2 times faster and backwards.
 */
#define CALL_STYLE_DIRECT           0x00 // == IN
#define CALL_STYLE_OUT              0x20
#define CALL_STYLE_IN_OUT           0x40
#define CALL_STYLE_BOUNCING_OUT_IN  0x60
#define CALL_STYLE_MASK             0xE0 // for future extensions

/*
 * Values for provided EaseTypes
 * The call style is coded in the upper 3 bits
 */
#define EASE_TYPE_MASK          0x0F

#define EASE_LINEAR             0x00

#define EASE_QUADRATIC_IN       0x01
#define EASE_QUADRATIC_OUT      0x21
#define EASE_QUADRATIC_IN_OUT   0x41
#define EASE_QUADRATIC_BOUNCING 0x61

#define EASE_CUBIC_IN           0x02
#define EASE_CUBIC_OUT          0x22
#define EASE_CUBIC_IN_OUT       0x42
#define EASE_CUBIC_BOUNCING     0x62

#define EASE_QUARTIC_IN         0x03
#define EASE_QUARTIC_OUT        0x23
#define EASE_QUARTIC_IN_OUT     0x43
#define EASE_QUARTIC_BOUNCING   0x63

#ifndef KEEP_LIBRARY_SMALL
#define EASE_SINE_IN            0x08
#define EASE_SINE_OUT           0x28
#define EASE_SINE_IN_OUT        0x48
#define EASE_SINE_BOUNCING      0x68

#define EASE_CIRCULAR_IN        0x09
#define EASE_CIRCULAR_OUT       0x29
#define EASE_CIRCULAR_IN_OUT    0x49
#define EASE_CIRCULAR_BOUNCING  0x69

#define EASE_BACK_IN            0x0A
#define EASE_BACK_OUT           0x2A
#define EASE_BACK_IN_OUT        0x4A
#define EASE_BACK_BOUNCING      0x6A

#define EASE_ELASTIC_IN         0x0B
#define EASE_ELASTIC_OUT        0x2B
#define EASE_ELASTIC_IN_OUT     0x4B
#define EASE_ELASTIC_BOUNCING   0x6B

// the coded function is an OUT function
#define EASE_BOUNCE_IN          0x2C // call OUT function inverse
#define EASE_BOUNCE_OUT         0x0C // call OUT function direct
#endif

#define EASE_USER_DIRECT        0x0F
#define EASE_USER_OUT           0x2F
#define EASE_USER_IN_OUT        0x4F
#define EASE_USER_BOUNCING      0x6F

// some PCA9685 specific constants
#define PCA9685_GENERAL_CALL_ADDRESS 0x00
#define PCA9685_SOFTWARE_RESET      6
#define PCA9685_DEFAULT_ADDRESS     0x40
#define PCA9685_MAX_CHANNELS        16 // 16 PWM channels on each PCA9685 expansion module
#define PCA9685_MODE1_REGISTER      0x0
#define PCA9685_AUTOINCREMENT       5
#define PCA9685_SLEEP               4
#define PCA9685_FIRST_PWM_REGISTER  0x06
#define PCA9685_PRESCALE_REGISTER   0xFE

#define PCA9685_PRESCALER_FOR_20_MS ((25000000L /(4096L * 50))-1) // = 121 / 0x79 at 50 Hz

class ServoEasing
#if not defined(USE_LEIGHTWEIGHT_SERVO_LIB) && not defined(USE_PCA9685_SERVO_EXPANDER)
: public Servo
#endif
{
public:

#if defined(USE_PCA9685_SERVO_EXPANDER)
    ServoEasing(uint8_t aPCA9685I2CAddress = PCA9685_DEFAULT_ADDRESS, TwoWire *aI2CClass = &Wire);
    void PCA9685Init();
    void I2CWriteByte(uint8_t aAddress, uint8_t aData);
    void setPWM(uint16_t aOff);
    // main mapping function for us to PCA9685 Units (20000/4096 = 4.88 us)
    uint16_t MicrosecondsToPCA9685Units(uint16_t aMicroseconds);
#else
    ServoEasing();
#endif
    uint8_t attach(int aPin);
    // Here no units accepted, only microseconds!
    uint8_t attach(int aPin, int aMicrosecondsForServo0Degree, int aMicrosecondsForServo180Degree);
    void setReverseOperation(bool aOperateServoReverse); // you want to call it before using setTrim

    void setTrim(int8_t aTrim);
    void setTrimMicrosecondsOrUnits(int16_t aTrimMicrosecondsOrUnits);
    void setEasingType(uint8_t aEasingType);
    uint8_t getEasingType();

    void registerUserEaseInFunction(float (*aUserEaseInFunction)(float aPercentageOfCompletion));

    void write(int aValue); // write value direct to servo
    void writeMicrosecondsOrUnits(int aValue);

    void easeTo(uint8_t aDegree, uint16_t aDegreesPerSecond); // blocking move to new position using speed
    void easeToD(uint8_t aDegree, uint16_t aMillisForMove); // blocking move to new position using duration

    bool setEaseTo(uint8_t aDegree, uint16_t aDegreesPerSecond); // shortcut for startEaseTo(..,..,false)
    bool startEaseTo(uint8_t aDegree, uint16_t aDegreesPerSecond, bool aStartUpdateByInterrupt = true);
    bool setEaseToD(uint8_t aDegree, uint16_t aDegreesPerSecond); // shortcut for startEaseToD(..,..,false)
    bool startEaseToD(uint8_t aDegree, uint16_t aMillisForMove, bool aStartUpdateByInterrupt = true);
    bool update();
    float callEasingFunction(float aPercentageOfCompletion); // used in update()

    uint8_t getCurrentAngle();
    uint16_t getMillisForCompleteMove();
    bool isMoving();

    uint8_t MicrosecondsOrUnitsToDegree(uint16_t aMicrosecondsOrUnits);
    uint16_t DegreeToMicrosecondsOrUnits(uint8_t aDegree);

    void synchronizeServosAndStartInterrupt(bool doUpdateByInterrupt);

    void print(Stream * aSerial); // Print dynamic and static info
    void printDynamic(Stream * aSerial, bool doExtendedOutput = false);
    void printStatic(Stream * aSerial);

    /*
     * Internally only microseconds if using Servo library or units (= 4.88 us) if using PCA9685 expander are used to speed up things.
     * Other expander or libraries can therefore easily be added.
     */
    volatile uint16_t mCurrentMicrosecondsOrUnits; // set by write() and writeMicrosecondsOrUnits(). Needed as start for next move and to avoid unnecessary writes.
    uint16_t mStartMicrosecondsOrUnits;  // used with millisAtStartMove to compute currentMicrosecondsOrUnits
    uint16_t mEndMicrosecondsOrUnits;    // used once as last value just if movement was finished
    int16_t mDeltaMicrosecondsOrUnits;   // end - start

    uint8_t mEasingType; // EASE_LINEAR, EASE_QUADRATIC_IN_OUT, EASE_CUBIC_IN_OUT, EASE_QUARTIC_IN_OUT

    volatile bool mServoMoves;

#if defined(USE_PCA9685_SERVO_EXPANDER)
    uint8_t mPCA9685I2CAddress;
    TwoWire * mI2CClass;
#endif
    uint8_t mServoPin; // pin number - at least needed for Lightweight Servo Lib

    uint8_t mServoIndex; // Index in sServoArray

    float (*mUserEaseInFunction)(float aPercentageOfCompletion);

    uint32_t mMillisAtStartMove;
    uint16_t mMillisForCompleteMove;

    bool mOperateServoReverse; // true -> direction is reversed by internal swapping the values of mServo0DegreeMicrosecondsOrUnits and mServo180DegreeMicrosecondsOrUnits
    int16_t mTrimMicrosecondsOrUnits; // This value will be added only at writeMicrosecondsOrUnits()

    uint16_t mServo0DegreeMicrosecondsOrUnits;
    uint16_t mServo180DegreeMicrosecondsOrUnits;
};

/*
 * List of all servos to enable synchronized movings
 * Servos are inserted in the order, in which they are declared
 */
extern uint8_t sServoCounter;
extern ServoEasing * sServoArray[MAX_SERVOS];
extern uint8_t sServoNextPositionArray[MAX_SERVOS];

/*
 * Functions working on all servos in the list
 */
bool setEaseToForAllServos(uint16_t aDegreesPerSecond);
void setEaseToForAllServosSynchronizeAndStartInterrupt(uint16_t aDegreesPerSecond);
void synchronizeAndEaseToArrayPositions(uint16_t aDegreesPerSecond);
void printArrayPositions(Stream * aSerial);
void setEasingTypeForAllServos(uint8_t aEasingType);
bool isOneServoMoving();
void stopAllServos();
bool updateAllServos();
void synchronizeAllServosAndStartInterrupt(bool aStartUpdateByInterrupt = true);

// blocking wait functions
void updateAndWaitForAllServosToStop();
void synchronizeAllServosStartAndWaitForAllServosToStop();

void enableServoEasingInterrupt();
void disableServoEasingInterrupt();

/*
 * Included easing functions
 */

float QuadraticEaseIn(float aPercentageOfCompletion);
float CubicEaseIn(float aPercentageOfCompletion);
float QuarticEaseIn(float aPercentageOfCompletion);

float SineEaseIn(float aPercentageOfCompletion);
float CircularEaseIn(float aPercentageOfCompletion);
float BackEaseIn(float aPercentageOfCompletion);
float ElasticEaseIn(float aPercentageOfCompletion);

// Non symmetric functions
float EaseOutBounce(float aPercentageOfCompletion);

extern float (*sEaseFunctionArray[])(float aPercentageOfCompletion);

#endif /* SERVOEASING_H_ */