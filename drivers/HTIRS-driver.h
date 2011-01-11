/*!@addtogroup HiTechnic
 * @{
 * @defgroup htirs IR Seeker V1
 * HiTechnic Color IR Seeker V1
 * @{
 */

/*
 * $Id: HTIRS-driver.h 29 2010-06-25 12:55:41Z xander $
 */

#ifndef __HTIRS_H__
#define __HTIRS_H__
/** \file HTIRS-driver.h
 * \brief HiTechnic IR Seeker driver
 *
 * HTIRS-driver.h provides an API for the HiTechnic IR Seeker.
 *
 * NOTE: THIS DRIVER WILL NOT WORK WITH THE IR SEEKER V2!<br>
 * Use the HTDIR driver for the V2 sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Changed comments on HTIRReadDir()
 * - 0.3: Reduced MAX_BUFSIZE to 5 to reduce overhead
 * - 0.4: Major rewrite of code, uses common.h for most functions
 * - 0.5: Fixed incorrect register numbers for signal strength registers
 * - 0.6: Added SMUX functions
 * - 0.7: HTIRSreadAllStrength() is now pass by reference to reduce memory<br>
 *        SMUX tByteArray removed, reuses HTIRS_I2CReply
 * - 0.8: Use new calls in common.h that don't require SPORT/MPORT macros
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 * - JamesD for testing and finding bugs.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 25 November 2009
 * \version 0.8
 * \example HTIRS-test1.c
 * \example HTIRS-SMUX-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTIRS_I2C_ADDR 0x02      /*!< IR Seeker I2C device address */
#define HTIRS_OFFSET   0x42      /*!< Offset for data registers */
#define HTIRS_DIR      0x00      /*!< Address of Direction data */
#define HTIRS_SSTR1    0x01      /*!< Address of Sensor 0 signal strength */
#define HTIRS_SSTR2    0x02      /*!< Address of Sensor 1 signal strength */
#define HTIRS_SSTR3    0x03      /*!< Address of Sensor 2 signal strength */
#define HTIRS_SSTR4    0x04      /*!< Address of Sensor 3 signal strength */
#define HTIRS_SSTR5    0x05      /*!< Address of Sensor 4 signal strength */

int HTIRSreadDir(tSensors link);
int HTIRSreadDir(tMUXSensor muxsensor);
int HTIRSreadStrength(tSensors link, byte sensorNr);
int HTIRSreadStrength(tMUXSensor muxsensor, byte sensorNr);
bool HTIRSreadAllStrength(tSensors link, int &dcS1, int &dcS2, int &dcS3, int &dcS4, int &dcS5);
bool HTIRSreadAllStrength(tMUXSensor muxsensor, int &dcS1, int &dcS2, int &dcS3, int &dcS4, int &dcS5);

tByteArray HTIRS_I2CRequest;    /*!< Array to hold I2C command data */
tByteArray HTIRS_I2CReply;      /*!< Array to hold I2C reply data */

/**
 * Read the value of the Direction data register and return it.
 * @param link the HTIRS port number
 * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
 */
int HTIRSreadDir(tSensors link) {
  memset(HTIRS_I2CRequest, 0, sizeof(tByteArray));

  HTIRS_I2CRequest.arr[0] = 2;                        // Message size
  HTIRS_I2CRequest.arr[1] = HTIRS_I2C_ADDR;           // I2C Address
  HTIRS_I2CRequest.arr[2] = HTIRS_OFFSET + HTIRS_DIR; // Start direction register

  if (!writeI2C(link, HTIRS_I2CRequest, 1))
    return -1;

  if (!readI2C(link, HTIRS_I2CReply, 1))
    return -1;

  return ubyteToInt(HTIRS_I2CReply.arr[0]);
}


/**
 * Read the value of the Direction data register and return it.
 * @param muxsensor the SMUX sensor port number
 * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
 */
int HTIRSreadDir(tMUXSensor muxsensor) {
	memset(HTIRS_I2CReply, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXIRSeeker)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTIRS_I2CReply, 1, HTIRS_DIR))
    return -1;

  return ubyteToInt(HTIRS_I2CReply.arr[0]);
}


/**
 * Read the value of the specified internal sensor, numbered 0-5 and return it.
 * @param link the HTIRS port number
 * @param sensorNr the internal sensor to read
 * @return the signal strength value of the specified sensor or -1 if an error occurred.
 */
int HTIRSreadStrength(tSensors link, byte sensorNr) {
  memset(HTIRS_I2CRequest, 0, sizeof(tByteArray));

  HTIRS_I2CRequest.arr[0] = 2;                                     // Message size
  HTIRS_I2CRequest.arr[1] = HTIRS_I2C_ADDR;                        // I2C Address
  HTIRS_I2CRequest.arr[2] = HTIRS_OFFSET + HTIRS_SSTR1 + sensorNr; // Address of Sensor <sensorNr> signal strength

  if (!writeI2C(link, HTIRS_I2CRequest, 1))
    return -1;

  if (!readI2C(link, HTIRS_I2CReply, 1))
    return -1;

  return ubyteToInt(HTIRS_I2CReply.arr[0]);
}


/**
 * Read the value of the specified internal sensor, numbered 0-5 and return it.
 * @param muxsensor the SMUX sensor port number
 * @param sensorNr the internal sensor to read
 * @return the signal strength value of the specified sensor or -1 if an error occurred.
 */
int HTIRSreadStrength(tMUXSensor muxsensor, byte sensorNr) {
  memset(HTIRS_I2CReply, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXIRSeeker)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTIRS_I2CReply, 1, HTIRS_SSTR1 + sensorNr))
    return -1;

  return ubyteToInt(HTIRS_I2CReply.arr[0]);
}


/**
 * Read the value of the all of the internal sensors.
 * @param link the HTIRS port number
 * @param dcS1 data from internal sensor nr 1
 * @param dcS2 data from internal sensor nr 2
 * @param dcS3 data from internal sensor nr 3
 * @param dcS4 data from internal sensor nr 4
 * @param dcS5 data from internal sensor nr 5
 * @return true if no error occured, false if it did
 */
bool HTIRSreadAllStrength(tSensors link, int &dcS1, int &dcS2, int &dcS3, int &dcS4, int &dcS5) {
  memset(HTIRS_I2CRequest, 0, sizeof(tByteArray));

  HTIRS_I2CRequest.arr[0] = 2;                          // Message size
  HTIRS_I2CRequest.arr[1] = HTIRS_I2C_ADDR;             // I2C Address
  HTIRS_I2CRequest.arr[2] = HTIRS_OFFSET + HTIRS_SSTR1; // Address of Sensor 0 signal strength

  if (!writeI2C(link, HTIRS_I2CRequest, 5))
    return false;

  if (!readI2C(link, HTIRS_I2CReply, 5))
    return false;

  dcS1 = ubyteToInt(HTIRS_I2CReply.arr[0]);
  dcS2 = ubyteToInt(HTIRS_I2CReply.arr[1]);
  dcS3 = ubyteToInt(HTIRS_I2CReply.arr[2]);
  dcS4 = ubyteToInt(HTIRS_I2CReply.arr[3]);
  dcS5 = ubyteToInt(HTIRS_I2CReply.arr[4]);

  return true;
}


/**
 * Read the value of the all of the internal sensors.
 * @param muxsensor the SMUX sensor port number
 * @param dcS1 data from internal sensor nr 1
 * @param dcS2 data from internal sensor nr 2
 * @param dcS3 data from internal sensor nr 3
 * @param dcS4 data from internal sensor nr 4
 * @param dcS5 data from internal sensor nr 5
 * @return true if no error occured, false if it did
 */
bool HTIRSreadAllStrength(tMUXSensor muxsensor, int &dcS1, int &dcS2, int &dcS3, int &dcS4, int &dcS5) {
  memset(HTIRS_I2CReply, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXIRSeeker)
    return false;

  if (!HTSMUXreadPort(muxsensor, HTIRS_I2CReply, 5, HTIRS_SSTR1))
    return false;

  dcS1 = ubyteToInt(HTIRS_I2CReply.arr[0]);
  dcS2 = ubyteToInt(HTIRS_I2CReply.arr[1]);
  dcS3 = ubyteToInt(HTIRS_I2CReply.arr[2]);
  dcS4 = ubyteToInt(HTIRS_I2CReply.arr[3]);
  dcS5 = ubyteToInt(HTIRS_I2CReply.arr[4]);

  return true;
}
#endif // __HTIRS_H__

/*
 * $Id: HTIRS-driver.h 29 2010-06-25 12:55:41Z xander $
 */
/* @} */
/* @} */
