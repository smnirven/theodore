/*!@addtogroup HiTechnic
 * @{
 * @defgroup htang Angle Sensor
 * HiTechnic Angle Sensor
 * @{
 */

/*
 * $Id: HTANG-driver.h 40 2011-01-03 09:37:09Z xander $
 */

#ifndef __HTANG_H__
#define __HTANG_H__
/** \file HTANG-driver.h
 * \brief HiTechnic Angle Sensor driver
 *
 * HTANG-driver.h provides an API for the HiTechnic Angle Sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 12 November 2009
 * \version 0.1
 * \example HTANG-test1.c
 * \example HTANG-SMUX-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTANG_I2C_ADDR         0x02      /*!< HTCS2 I2C device address */
#define HTANG_CMD_REG          0x41      /*!< Command register */
#define HTANG_OFFSET           0x42      /*!< Offset for data registers */

// Values contained by registers in active mode
#define HTANG_ANG2             0x00      /*!< Current angle (2 deg increments) */
#define HTANG_ANG1             0x01      /*!< Current angle (1 deg adder) */
#define HTANG_ACC_ANG_B4       0x02      /*!< 32 bit accumulated angle 4th byte */
#define HTANG_ACC_ANG_B3       0x03      /*!< 32 bit accumulated angle 3rd byte */
#define HTANG_ACC_ANG_B2       0x04      /*!< 32 bit accumulated angle 2nd byte */
#define HTANG_ACC_ANG_B1       0x05      /*!< 32 bit accumulated angle 1st byte */
#define HTANG_RPM_H            0x06      /*!< 16 bit rpms, high byte */
#define HTANG_RPM_L            0x07      /*!< 16 bit rpms, low byte */

// Various commands
#define HTANG_CMD_MEASURE      0x00      /*!< Normal angle measurement mode */
#define HTANG_CMD_RST_ANG      0x43      /*!< Resets 0 position to current shaft angle, non-volatile setting */
#define HTANG_CMD_RST_ACC_ANG  0x52      /*!< Resets the accumulated angle */

int HTANGreadAngle(tSensors link);
int HTANGreadAngle(tMUXSensor muxsensor);
long HTANGreadAccumulatedAngle(tSensors link);
long HTANGreadAccumulatedAngle(tMUXSensor muxsensor);
int HTANGreadRPM(tSensors link);
int HTANGreadRPM(tMUXSensor muxsensor);
bool HTANGresetAngle(tSensors link);
bool HTANGresetAccumulatedAngle(tSensors link);
bool _HTANGsendCommand(tSensors link, byte command);

tByteArray HTANG_I2CRequest;             /*!< Array to hold I2C command data */
tByteArray HTANG_I2CReply;               /*!< Array to hold I2C reply data */


/**
 * Return the current angle
 * @param link the HTANG port number
 * @return current angle or -1 if an error occurred.
 */
int HTANGreadAngle(tSensors link) {
  memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  HTANG_I2CRequest.arr[0] = 2;                         // Message size
  HTANG_I2CRequest.arr[1] = HTANG_I2C_ADDR;            // I2C Address
  HTANG_I2CRequest.arr[2] = HTANG_OFFSET + HTANG_ANG2; // Start Current angle

  if (!writeI2C(link, HTANG_I2CRequest, 2))
    return -1;

  if (!readI2C(link, HTANG_I2CReply, 2))
    return -1;

  return HTANG_I2CReply.arr[0] * 2 + HTANG_I2CReply.arr[1];
}


/**
 * Return the current angle
 * @param muxsensor the SMUX sensor port number
 * @return current angle or -1 if an error occurred.
 */
int HTANGreadAngle(tMUXSensor muxsensor) {
	memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXAngle)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTANG_I2CReply, 2, HTANG_ANG2)) {
    return -1;
  }

  return HTANG_I2CReply.arr[0] * 2 + HTANG_I2CReply.arr[1];
}


/**
 * Return the accumulated angle (signed 32 bit value)
 * @param link the HTANG port number
 * @return current angle or -1 if an error occurred.
 */
long HTANGreadAccumulatedAngle(tSensors link) {
  memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  HTANG_I2CRequest.arr[0] = 2;                                // Message size
  HTANG_I2CRequest.arr[1] = HTANG_I2C_ADDR;                   // I2C Address
  HTANG_I2CRequest.arr[2] = HTANG_OFFSET + HTANG_ACC_ANG_B4;  // Start accumulated angle

  if (!writeI2C(link, HTANG_I2CRequest, 4))
    return -1;

  if (!readI2C(link, HTANG_I2CReply, 4))
    return -1;

  return (HTANG_I2CReply.arr[0] << 24) +
         (HTANG_I2CReply.arr[1] << 16) +
         (HTANG_I2CReply.arr[2] <<  8) +
          HTANG_I2CReply.arr[3];
}


/**
 * Return the accumulated angle (signed 32 bit value)
 * @param muxsensor the SMUX sensor port number
 * @return current angle or -1 if an error occurred.
 */
long HTANGreadAccumulatedAngle(tMUXSensor muxsensor) {
	memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXAngle)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTANG_I2CReply, 4, HTANG_ACC_ANG_B4)) {
    return -1;
  }

  return (HTANG_I2CReply.arr[0] << 24) +
         (HTANG_I2CReply.arr[1] << 16) +
         (HTANG_I2CReply.arr[2] <<  8) +
          HTANG_I2CReply.arr[3];
}

/**
 * Return the rpm that the shaft is currently rotating at
 * @param link the HTANG port number
 * @return the current rpm of the shaft or -1 if an error occurred.
 */
int HTANGreadRPM(tSensors link) {
  memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  HTANG_I2CRequest.arr[0] = 2;                           // Message size
  HTANG_I2CRequest.arr[1] = HTANG_I2C_ADDR;              // I2C Address
  HTANG_I2CRequest.arr[2] = HTANG_OFFSET + HTANG_RPM_H;  // Start of rpm

  if (!writeI2C(link, HTANG_I2CRequest, 2))
    return -1;

  if (!readI2C(link, HTANG_I2CReply, 2))
    return -1;

  return (HTANG_I2CReply.arr[0] <<  8) +
          HTANG_I2CReply.arr[1];
}


/**
 * Return the rpm that the shaft is currently rotating at
 * @param muxsensor the SMUX sensor port number
 * @return the current rpm of the shaft or -1 if an error occurred.
 */
int HTANGreadRPM(tMUXSensor muxsensor) {
  memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  if (HTSMUXreadSensorType(muxsensor) != HTSMUXAngle)
    return -1;

  if (!HTSMUXreadPort(muxsensor, HTANG_I2CReply, 2, HTANG_RPM_H)) {
    return -1;
  }

  return HTANG_I2CReply.arr[0] * 2 + HTANG_I2CReply.arr[1];

  return (HTANG_I2CReply.arr[0] <<  8) +
          HTANG_I2CReply.arr[1];
}


/**
 * Reset the 0 position to the current shaft angle.<br>
 * Note: this will also reset the accumulated angle counter
 * @param link the HTANG port number
 * @return true if no error occured, false if it did
 */
bool HTANGresetAngle(tSensors link) {
  return _HTANGsendCommand(link, HTANG_CMD_RST_ANG);
}


/**
 * Reset the accumulated angle
 * @param link the HTANG port number
 * @return true if no error occured, false if it did
 */
bool HTANGresetAccumulatedAngle(tSensors link) {
  return _HTANGsendCommand(link, HTANG_CMD_RST_ACC_ANG);
}


/**
 * Send a command to the sensor
 *
 * Note: this is an internal function and should not be called directly.
 * @param link the HTANG port number
 * @param command the command to be sent to the sensor
 * @return true if no error occured, false if it did
 */
bool _HTANGsendCommand(tSensors link, byte command) {
  memset(HTANG_I2CRequest, 0, sizeof(tByteArray));

  HTANG_I2CRequest.arr[0] = 3;              // Message size
  HTANG_I2CRequest.arr[1] = HTANG_I2C_ADDR; // I2C Address
  HTANG_I2CRequest.arr[2] = HTANG_CMD_REG;  // Command register
  HTANG_I2CRequest.arr[3] = command;        // Command to be sent

  return writeI2C(link, HTANG_I2CRequest, 0);
}


#endif // __HTANG_H__

 /*
 * $Id: HTANG-driver.h 40 2011-01-03 09:37:09Z xander $
 */
/* @} */
/* @} */
