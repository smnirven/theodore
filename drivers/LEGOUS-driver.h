/*!@addtogroup Lego
 * @{
 * @defgroup legous Ultrasonic Sensor
 * Ultrasonic Sensor
 * @{
 */

/*
 * $Id: LEGOUS-driver.h 43 2011-01-03 09:48:06Z xander $
 */

#ifndef __LEGOUS_H__
#define __LEGOUS_H__
/** \file LEGOUS-driver.h
 * \brief SMUX driver for the Lego US sensor.
 *
 * LEGOUS-driver.h provides an API for the Lego US driver.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Added support for additional commands
 *
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 10 December 2010
 * \version 0.2
 * \example LEGOUS-SMUX-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define LEGOUS_I2C_ADDR    0x10      /*!< Lego US I2C address */
#define LEGOUS_REG_CMD     0x41      /*!< Command register */
#define LEGOUS_REG_DATA    0x42      /*!< Start of measurement data registers */

#define LEGOUS_CMD_OFF    0x00      /*!< Command to switch US off */
#define LEGOUS_CMD_SSHOT  0x01      /*!< Command to turn on Single Shot mode */
#define LEGOUS_CMD_CONT   0x02      /*!< Command to turn on Continuous Mode */
#define LEGOUS_CMD_ECAPT  0x03      /*!< Command to turn on Event Capture Mode */
#define LEGOUS_CMD_RST    0x04      /*!< Command to request a warm reset */

// Prototypes
int USreadDist(tMUXSensor muxsensor);
int USreadDist(tSensors link);
bool USreadDistances(tSensors link, tByteArray &distances);
bool _USsendCmd(tSensors link, ubyte command);
bool USsetSingleMode(tSensors link);
bool USsetContinuousMode(tSensors link);
bool USsetOff(tSensors link);
bool USsetEventCapture(tSensors link);
bool USreset(tSensors link);

tByteArray LEGOUS_SMUXData;      /*!< Array to hold SMUX data */
tByteArray LEGOUS_I2CRequest;
tByteArray LEGOUS_I2CReply;


/**
 * Get the distance value from the sensor
 * @param muxsensor the SMUX sensor port number
 * @return distance from the sensor or 255 if no valid range has been specified.
 */
int USreadDist(tMUXSensor muxsensor) {
	memset(LEGOUS_SMUXData, 0, sizeof(tByteArray));

  if (smuxData[SPORT(muxsensor)].sensor[MPORT(muxsensor)] != HTSMUXLegoUS)
    return 255;

  if (!HTSMUXreadPort((tSensors)SPORT(muxsensor), MPORT(muxsensor), LEGOUS_SMUXData, 1, 0)) {
    return 255;
  }

  return ubyteToInt(LEGOUS_SMUXData.arr[0]);
}


/**
 * Get the distance values from the sensor
 * @param link the US port number
 * @return distance from the sensor or 255 if no valid range has been specified.
 */
int USreadDist(tSensors link) {
  memset(LEGOUS_I2CRequest, 0, sizeof(tByteArray));

  LEGOUS_I2CRequest.arr[0] = 2;                // Message size
  LEGOUS_I2CRequest.arr[1] = LEGOUS_I2C_ADDR;  // I2C Address
  LEGOUS_I2CRequest.arr[2] = LEGOUS_REG_DATA;  // Start direction register

  if (!writeI2C(link, LEGOUS_I2CRequest, 1))
    return -1;

  if (!readI2C(link, LEGOUS_I2CReply, 1))
    return -1;

  return LEGOUS_I2CReply.arr[0];
}


/**
 * Get the distance values from the sensor. The distances to the
 * 8 closest objects are returned.
 * @param link the US port number
 * @param distances array holding data on last 8 echos received
 * @return distance from the sensor or 255 if no valid range has been specified.
 */
bool USreadDistances(tSensors link, tByteArray &distances) {
  memset(LEGOUS_I2CRequest, 0, sizeof(tByteArray));

  LEGOUS_I2CRequest.arr[0] = 2;                // Message size
  LEGOUS_I2CRequest.arr[1] = LEGOUS_I2C_ADDR;  // I2C Address
  LEGOUS_I2CRequest.arr[2] = LEGOUS_REG_DATA;  // Start direction register

  if (!writeI2C(link, LEGOUS_I2CRequest, 8))
    return false;

  if (!readI2C(link, LEGOUS_I2CReply, 8))
    return false;

  memcpy(distances, LEGOUS_I2CReply, sizeof(tByteArray));
  return true;
}


/**
 * Send a command to the US Sensor
 *
 * Note: this is an internal function and should not be called directly.
 * @param link the US port number
 * @param command the command to be sent to the sensor
 * @return true if no error occured, false if it did
 */
bool _USsendCmd(tSensors link, ubyte command) {
  memset(LEGOUS_I2CRequest, 0, sizeof(tByteArray));

  LEGOUS_I2CRequest.arr[0] = 3;                // Message size
  LEGOUS_I2CRequest.arr[1] = LEGOUS_I2C_ADDR;  // I2C Address
  LEGOUS_I2CRequest.arr[2] = LEGOUS_REG_CMD;   // command register
  LEGOUS_I2CRequest.arr[3] = command;          // command

  return writeI2C(link, LEGOUS_I2CRequest, 0);
}


/**
 * Configure the US sensor for Single Shot mode
 * @param link the US port number
 * @return true if no error occured, false if it did
 */
bool USsetSingleMode(tSensors link) {
  return _USsendCmd(link, LEGOUS_CMD_SSHOT);
}


/**
 * Configure the US sensor for Continuous Mode.  This is the default.
 * @param link the US port number
 * @return distance from the sensor or 255 if no valid range has been specified.
 */
bool USsetContinuousMode(tSensors link) {
  return _USsendCmd(link, LEGOUS_CMD_CONT);
}


/**
 * Turn the sensor off.
 * @param link the US port number
 * @return true if no error occured, false if it did
 */
bool USsetOff(tSensors link){
  return _USsendCmd(link, LEGOUS_CMD_OFF);
}


/**
 * Configure the US sensor for Event Capture mode
 * @param link the US port number
 * @return true if no error occured, false if it did
 */
bool USsetEventCapture(tSensors link) {
  return _USsendCmd(link, LEGOUS_CMD_ECAPT);
}


/**
 * Request a warm reset of the sensor.
 * @param link the US port number
 * @return true if no error occured, false if it did
 */
bool USreset(tSensors link) {
  return _USsendCmd(link, LEGOUS_CMD_RST);
}

#endif // __LEGOSNR_H__

/*
 * $Id: LEGOUS-driver.h 43 2011-01-03 09:48:06Z xander $
 */
/* @} */
/* @} */
