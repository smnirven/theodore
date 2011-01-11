/*!@addtogroup mindsensors
 * @{
 * @defgroup nxtservo Servo Controller
 * Servo Controller
 * @{
 */

/*
 * $Id: NXTServo-driver.h 29 2010-06-25 12:55:41Z xander $
 */

#ifndef __NXTSERVO_H__
#define __NXTSERVO_H__
/** \file NXTServo-driver.h
 * \brief Mindsensors NXTServo Sensor Driver
 *
 * NXTServo-driver.h provides an API for the Mindsensors NXTServo Sensor
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Mindsensors for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 30 September 2009
 * \version 0.1
 * \example NXTServo-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define NXTSERVO_I2C_ADDR     0xB0      /*!< NXT Servo I2C device address */

// Command register, can be written for commands and read for battery voltage level
#define NXTSERVO_CMD          0x41      /*!< Servo command register
*/
// Position registers, can be read and written
#define NXTSERVO_POS_CHAN1    0x42      /*!< Servo channel 1 position (low byte) */
#define NXTSERVO_POS_CHAN2    0x44      /*!< Servo channel 2 position (low byte) */
#define NXTSERVO_POS_CHAN3    0x46      /*!< Servo channel 3 position (low byte) */
#define NXTSERVO_POS_CHAN4    0x48      /*!< Servo channel 4 position (low byte) */
#define NXTSERVO_POS_CHAN5    0x4A      /*!< Servo channel 5 position (low byte) */
#define NXTSERVO_POS_CHAN6    0x4C      /*!< Servo channel 6 position (low byte) */
#define NXTSERVO_POS_CHAN7    0x4E      /*!< Servo channel 7 position (low byte) */
#define NXTSERVO_POS_CHAN8    0x50      /*!< Servo channel 8 position (low byte) */

// Speed registers, can be read and written
#define NXTSERVO_SPEED_CHAN1  0x52      /*!< Servo channel 1 speed */
#define NXTSERVO_SPEED_CHAN2  0x53      /*!< Servo channel 2 speed */
#define NXTSERVO_SPEED_CHAN3  0x54      /*!< Servo channel 3 speed */
#define NXTSERVO_SPEED_CHAN4  0x55      /*!< Servo channel 4 speed */
#define NXTSERVO_SPEED_CHAN5  0x56      /*!< Servo channel 5 speed */
#define NXTSERVO_SPEED_CHAN6  0x57      /*!< Servo channel 6 speed */
#define NXTSERVO_SPEED_CHAN7  0x58      /*!< Servo channel 7 speed */
#define NXTSERVO_SPEED_CHAN8  0x59      /*!< Servo channel 8 speed */

// Quick position register, can only be written
#define NXTSERVO_QPOS_CHAN1   0x42      /*!< Servo channel 1 quick position */
#define NXTSERVO_QPOS_CHAN2   0x44      /*!< Servo channel 2 quick position */
#define NXTSERVO_QPOS_CHAN3   0x46      /*!< Servo channel 3 quick position */
#define NXTSERVO_QPOS_CHAN4   0x48      /*!< Servo channel 4 quick position */
#define NXTSERVO_QPOS_CHAN5   0x4A      /*!< Servo channel 5 quick position */
#define NXTSERVO_QPOS_CHAN6   0x4C      /*!< Servo channel 6 quick position */
#define NXTSERVO_QPOS_CHAN7   0x4E      /*!< Servo channel 7 quick position */
#define NXTSERVO_QPOS_CHAN8   0x50      /*!< Servo channel 8 quick position */

/*! Some other defines. */
#define NXTSERVO_MIN_POS      500       /*!< Servo minimum pulse width in uS */
#define NXTSERVO_MAX_POS     2500       /*!< Servo maximum pulse width in uS */
#define NXTSERVO_MID_POS     1500       /*!< Servo centered pulse width in uS */

/*
<function prototypes>
*/
bool NXTServoSetPos(tSensors link, ubyte servochan, int position, ubyte speed);
bool NXTServoSetPos(tSensors link, ubyte servochan, int position);
bool NXTServoSetSpeed(tSensors link, ubyte servochan, ubyte speed);
bool NXTServoQSetPos(tSensors link, ubyte servochan, ubyte position, byte speed);
bool NXTServoQSetPos(tSensors link, ubyte servochan, ubyte position);
int NXTServoReadPos(tSensors link, ubyte servochan);
int NXTServoReadVoltage(tSensors link);

tByteArray NXTSERVO_I2CRequest;         /*!< Array to hold I2C command data */
tByteArray NXTSERVO_I2CReply;           /*!< Array to hold I2C reply data */


/**
 * Tell the servo to move to the specified position using the specified speed.
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @param position the position to move the servo to
 * @param speed the amount to increase the position by every 24ms
 * @return true if no error occured, false if it did
 */
bool NXTServoSetPos(tSensors link, ubyte servochan, int position, ubyte speed) {
  if (!NXTServoSetSpeed(link, servochan, speed))
    return false;

  return NXTServoSetPos(link, servochan, position);
}


/**
 * Tell the servo to move to the specified position.  It will use whatever
 * speed is already set in the speed register.  Use NXTServoSetSpeed() to change
 * this setting or use NXTServoSetPos() to set both at once.
 *
 * The position argument is clipped at 500 2500 to prevent damage to the servo.
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @param position the position to move the servo to
 * @return true if no error occured, false if it did
 */
bool NXTServoSetPos(tSensors link, ubyte servochan, int position) {
  if (position < 500) position = 500;
  if (position > 2500) position = 2500;


  // set the position register and tell NXTServo to move the servo
  memset(NXTSERVO_I2CRequest, 0, sizeof(tByteArray));
  NXTSERVO_I2CRequest.arr[0] = 4;
  NXTSERVO_I2CRequest.arr[1] = NXTSERVO_I2C_ADDR;
  NXTSERVO_I2CRequest.arr[2] = NXTSERVO_POS_CHAN1 + ((servochan - 1) * 2);
  NXTSERVO_I2CRequest.arr[3] = position & 0x00FF;
  NXTSERVO_I2CRequest.arr[4] = (position >> 8) & 0x00FF;

  return writeI2C(link, NXTSERVO_I2CRequest, 0);
}


/**
 * Set the speed register for the specified servo.  This is the amount to increase
 * the current position by every 24ms when the servo position is changed.
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @param speed the amount to increase the position by every 24ms
 * @return true if no error occured, false if it did
 */
bool NXTServoSetSpeed(tSensors link, ubyte servochan, ubyte speed) {
  memset(NXTSERVO_I2CRequest, 0, sizeof(tByteArray));
  NXTSERVO_I2CRequest.arr[0] = 3;
  NXTSERVO_I2CRequest.arr[1] = NXTSERVO_I2C_ADDR;
  NXTSERVO_I2CRequest.arr[2] = NXTSERVO_SPEED_CHAN1 + (servochan - 1);
  NXTSERVO_I2CRequest.arr[3] = speed;

  return writeI2C(link, NXTSERVO_I2CRequest, 0);
}


/**
 * Tell the servo to move to the specified position using the specified speed.
 * This uses the Quick Position register which is less accurate.  It only accepts
 * values from 50 to 250
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @param position the position to move the servo to
 * @param speed the amount to increase the position by every 24ms
 * @return true if no error occured, false if it did
 */
bool NXTServoQSetPos(tSensors link, ubyte servochan, ubyte position, byte speed) {
  if (position < 50) position = 50;
  if (position > 250) position = 250;

  if (!NXTServoSetSpeed(link, servochan, speed))
    return false;

  return NXTServoQSetPos(link, servochan, position);
}


/**
 * Tell the servo to move to the specified position using the specified speed.
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @param position the position to move the servo to
 * @return true if no error occured, false if it did
 */
bool NXTServoQSetPos(tSensors link, ubyte servochan, ubyte position) {
  // set the position register and tell NXTServo to move the servo
  memset(NXTSERVO_I2CRequest, 0, sizeof(tByteArray));
  NXTSERVO_I2CRequest.arr[0] = 3;
  NXTSERVO_I2CRequest.arr[1] = NXTSERVO_I2C_ADDR;
  NXTSERVO_I2CRequest.arr[2] = NXTSERVO_QPOS_CHAN1 + (servochan - 1);
  NXTSERVO_I2CRequest.arr[3] = position;

  return writeI2C(link, NXTSERVO_I2CRequest, 0);
}


/**
 * Tell the servo to move to the specified position using the specified speed.
 * @param link the NXTServo port number
 * @param servochan the servo channel to use
 * @return true if no error occured, false if it did
 */
int NXTServoReadPos(tSensors link, ubyte servochan) {
  int position = 0;

  memset(NXTSERVO_I2CRequest, 0, sizeof(tByteArray));

  NXTSERVO_I2CRequest.arr[0] = 2;                 // Message size
  NXTSERVO_I2CRequest.arr[1] = NXTSERVO_I2C_ADDR; // I2C Address
  NXTSERVO_I2CRequest.arr[2] = NXTSERVO_POS_CHAN1 + ((servochan - 1) * 2);

  if (!writeI2C(link, NXTSERVO_I2CRequest, 2))
    return false;

  if (!readI2C(link, NXTSERVO_I2CReply, 2))
    return false;

  position = NXTSERVO_I2CReply.arr[1] * 256 + NXTSERVO_I2CReply.arr[0];

  return position;
}


/**
 * Get the voltage level of the battery pack.
 * @param link the NXTServo port number
 * @return the voltage of the battery pack or -1 if an error occurred
 */
int NXTServoReadVoltage(tSensors link) {
  long mvs = 0;

  memset(NXTSERVO_I2CRequest, 0, sizeof(tByteArray));

  NXTSERVO_I2CRequest.arr[0] = 2;                 // Message size
  NXTSERVO_I2CRequest.arr[1] = NXTSERVO_I2C_ADDR; // I2C Address
  NXTSERVO_I2CRequest.arr[2] = NXTSERVO_CMD;      // Start red sensor value

  if (!writeI2C(link, NXTSERVO_I2CRequest, 1))
    return -1;

  if (!readI2C(link, NXTSERVO_I2CReply, 1))
    return -1;

  mvs = ((long)ubyteToInt(NXTSERVO_I2CReply.arr[0]) * 3886) / 100;

  return mvs;
}

#endif // __NXTSERVO_H__
/*
 * $Id: NXTServo-driver.h 29 2010-06-25 12:55:41Z xander $
 */
/* @} */
/* @} */
