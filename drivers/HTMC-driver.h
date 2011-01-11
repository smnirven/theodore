/*!@addtogroup HiTechnic
 * @{
 * @defgroup htmc Compass Sensor
 * HiTechnic Compass Sensor
 * @{
 */

/*
 * $Id: HTMC-driver.h 40 2011-01-03 09:37:09Z xander $
 */

/** \file HTMC-driver.h
 * \brief HiTechnic Magnetic Compass Sensor Driver
 *
 * HTMC-driver.h provides an API for the HiTechnic Magnetic Compass Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Added SMUX functions
 * - 0.3: Removed HTMC_SMUXData, reuses HTMC_I2CReply to save memory
 * - 0.4: Replaced hex values in calibration functions with #define's
 * - 0.5: Replaced functions requiring SPORT/MPORT macros
 * - 0.6: simplified relative heading calculations - Thanks Gus!
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 27 February 2010
 * \version 0.6
 * \example HTMC-test1.c
 * \example HTMC-test2.c
 * \example HTMC-SMUX-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

// I2C address + registers
#define HTMC_I2C_ADDR       0x02  /*!< HTMC I2C device address */
#define HTMC_MODE           0x41  /*!< HTMC Mode control */
#define HTMC_HEAD_U         0x42  /*!< HTMC Heading Upper bits */
#define HTMC_HEAD_L         0x43  /*!< HTMC Heading Lower bit */

// I2C commands
#define HTMC_MEASURE_CMD    0x00  /*!< HTMC measurement mode command */
#define HTMC_CALIBRATE_CMD  0x43 /*!< HTMC calibrate mode command */

bool HTMCstartCal(tSensors link);
bool HTMCstopCal(tSensors link);
int HTMCreadHeading(tSensors link);
int HTMCreadHeading(tMUXSensor muxsensor);
int HTMCreadRelativeHeading(tSensors link);
int HTMCreadRelativeHeading(tMUXSensor muxsensor);
int HTMCsetTarget(tSensors link);
int HTMCsetTarget(tMUXSensor muxsensor);
int HTMCsetTarget(tSensors link, int offset);
int HTMCsetTarget(tMUXSensor muxsensor, int offset);

tByteArray HTMC_I2CRequest;       /*!< Array to hold I2C command data */
tByteArray HTMC_I2CReply;         /*!< Array to hold I2C reply data */

int target[][] = {{0, 0, 0, 0},   /*!< Offsets for the compass sensor relative readings */
                  {0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}};


/**
 * Start the calibration. The sensor should be rotated a little more than 360 along the
 * horizontal plane in no less than 20 seconds.  After the sensor has been rotated,
 * call HTMCstopCal() to set the sensor back in measurement mode and save the
 * calibration data.  This calibration data is stored in the sensor until the
 * next calibration.
 * @param link the HTMC port number
 * @return true if no error occured, false if it did
 */
bool HTMCstartCal(tSensors link) {
  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

  HTMC_I2CRequest.arr[0] = 3;                   // Number of bytes in I2C command
  HTMC_I2CRequest.arr[1] = HTMC_I2C_ADDR;       // I2C address of compass sensor
  HTMC_I2CRequest.arr[2] = HTMC_MODE;           // Set write address to sensor mode register
  HTMC_I2CRequest.arr[3] = HTMC_CALIBRATE_CMD;  // The calibration mode command

  // Start the calibration
  return writeI2C(link, HTMC_I2CRequest, 0);
}


/**
 * Stop the calibration. This should be called no less than 20 seconds after
 * HTMCstartCal() and only if the sensor has been rotated more than 360 degrees
 * @param link the HTMC port number
 * @return true if no error occured, false if it did
 */
bool HTMCstopCal(tSensors link) {
  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

  HTMC_I2CRequest.arr[0] = 3;                 // Number of bytes in I2C command
  HTMC_I2CRequest.arr[1] = HTMC_I2C_ADDR;     // I2C address of compass sensor
  HTMC_I2CRequest.arr[2] = HTMC_MODE;         // Set write address to sensor mode register
  HTMC_I2CRequest.arr[3] = HTMC_MEASURE_CMD;  // The measurement mode command

  // Stop the calibration by setting the mode register back to measurement.
  if (!writeI2C(link, HTMC_I2CRequest, 1))
    return false;

  // Read back the register value to check if an error has occurred.
  if (!readI2C(link, HTMC_I2CReply, 1))
    return false;

  // The register is equal to 2 if the calibration has failed.
  if (HTMC_I2CReply.arr[0] == 2)
    return false;

  return true;
}


/**
 * Return the current absolute heading
 * @param link the HTMC port number
 * @return heading in degrees (0 - 359) or -1 if an error occurred.
 */
int HTMCreadHeading(tSensors link) {
  memset(HTMC_I2CRequest, 0, sizeof(tByteArray));

  HTMC_I2CRequest.arr[0] = 2;               // Number of bytes in I2C command
  HTMC_I2CRequest.arr[1] = HTMC_I2C_ADDR;   // I2C address of compass sensor
  HTMC_I2CRequest.arr[2] = HTMC_HEAD_U;     // Set write address to sensor mode register

  if (!writeI2C(link, HTMC_I2CRequest, 2))
    return -1;

  if (!readI2C(link, HTMC_I2CReply, 2))
    return -1;

  // Result is made up of two bytes.  Reassemble for final heading.
  return HTMC_I2CReply.arr[0] * 2 + HTMC_I2CReply.arr[1];
}


/**
 * Return the current absolute heading
 * @param muxsensor the SMUX sensor port number
 * @return heading in degrees (0 - 359) or -1 if an error occurred.
 */
int HTMCreadHeading(tMUXSensor muxsensor) {
  memset(HTMC_I2CReply, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXCompass)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTMC_I2CReply, 2)) {
    return -1;
  }

  // Result is made up of two bytes.  Reassemble for final heading.
  return HTMC_I2CReply.arr[0] * 2 + HTMC_I2CReply.arr[1];
}


/**
 * Return the current relative heading, value between -179 and 180
 * @param link the HTMC port number
 * @return relative heading in degrees or -255 if an error occurred.
 */
int HTMCreadRelativeHeading(tSensors link) {

  // The black voodoo magic code below is courtsey of Gus from HiTechnic.
  int _tmpHeading = HTMCreadHeading(link) - target[link][0] + 180;
  return (_tmpHeading >= 0 ? _tmpHeading % 360 : 359 - (-1 - _tmpHeading)%360) - 180;
}


/**
 * Return the current relative heading, value between -179 and 180
 * @param muxsensor the SMUX sensor port number
 * @return relative heading in degrees or -255 if an error occurred.
 */
int HTMCreadRelativeHeading(tMUXSensor muxsensor) {

  return ((HTMCreadHeading(muxsensor) - target[SPORT(muxsensor)][MPORT(muxsensor)] + 540) % 360 - 180);

  /*
  int _tmpheading = 0;

  _tmpheading = HTMCreadHeading(muxsensor);
  if (_tmpheading < 0)
    return -255;

  // Compensate for offset and correct if result is less than 0
  _tmpheading = (_tmpheading - target[SPORT(muxsensor)][MPORT(muxsensor)]) % 360;
  if (_tmpheading < 0)
    _tmpheading += 360;

  // Return a result of -179 to 180 degrees from offset
  return (_tmpheading < 181) ? _tmpheading : (_tmpheading - 360);
  */
}


/**
 * Set the current heading as the value for the offset to be used as the new zero-point
 * for the relative heading returned by HTMCreadRelativeHeading()
 * @param link the HTMC port number
 */
int HTMCsetTarget(tSensors link) {
  target[link][0] = HTMCreadHeading(link);
  return target[link][0];
}


/**
 * Set the current heading as the value for the offset to be used as the new zero-point
 * for the relative heading returned by HTMCreadRelativeHeading()
 * @param muxsensor the SMUX sensor port number
 * @return the current target heading
 */
int HTMCsetTarget(tMUXSensor muxsensor) {
  target[SPORT(muxsensor)][MPORT(muxsensor)] = HTMCreadHeading(muxsensor);
  return target[SPORT(muxsensor)][MPORT(muxsensor)];
}


/**
 * Set the value for the offset to be used as the new zero-point
 * for the relative heading returned by HTMCreadRelativeHeading()
 * @param link the HTMC port number
 * @param offset to be used to calculate relative heading (0-360 degrees)
 * @return the current target heading
 */
int HTMCsetTarget(tSensors link, int offset) {
  target[link][0] = offset;
  return target[link][0];
}


/**
 * Set the value for the offset to be used as the new zero-point
 * for the relative heading returned by HTMCreadRelativeHeading()
 * @param muxsensor the SMUX sensor port number
 * @param offset to be used to calculate relative heading (0-360 degrees)
 */
int HTMCsetTarget(tMUXSensor muxsensor, int offset) {
  target[SPORT(muxsensor)][MPORT(muxsensor)] = offset;
  return target[SPORT(muxsensor)][MPORT(muxsensor)];
}

/*
 * $Id: HTMC-driver.h 40 2011-01-03 09:37:09Z xander $
 */
/* @} */
/* @} */
