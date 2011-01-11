/*!@addtogroup HiTechnic
 * @{
 * @defgroup hteopd EOPD Sensor
 * HiTechnic EOPD Sensor
 * @{
 */

/*
 * $Id: HTEOPD-driver.h 29 2010-06-25 12:55:41Z xander $
 */

#ifndef __HTEOPD_H__
#define __HTEOPD_H__
/** \file HTEOPD-driver.h
 * \brief HiTechnic EOPD Sensor driver
 *
 * HTEOPD-driver.h provides an API for the HiTechnic EOPD sensor.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Removed HTEOPDsetRange() and HTEOPDgetRange(), not really necessary
 *        Changed the way raw value is calculated due to sensor type change
 * - 0.3: Renamed HTEOPDgetRaw to HTEOPDreadRaw
 *        Renamed HTEOPDgetProcessed to HTEOPDreadProcessed
 *        Added SMUX functions
 * - 0.4: Added No Wait versions of HTEOPDsetShortRange and HTEOPDsetLongRange for non-SMUX functions
 *        Changed the underlying sensor types for RobotC 1.57A and higher.
 * - 0.5: Now only supports ROBOTC 2.00<br>
 *        Make use of the new analogue sensor calls for SMUX sensors in common.h
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 29 November 2009
 * \version 0.5
 * \example HTEOPD-test1.c
 * \example HTEOPD-SMUX-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

// This ensures the correct sensor types are used.
TSensorTypes LRType = sensorAnalogActive;
TSensorTypes SRType = sensorAnalogInactive;

int HTEOPDreadRaw(tSensors link);
int HTEOPDreadRaw(tMUXSensor muxsensor);
int HTEOPDreadProcessed(tSensors link);
int HTEOPDreadProcessed(tMUXSensor muxsensor);
void HTEOPDsetShortRange(tSensors link);
void HTEOPDsetShortRangeNW(tSensors link);
void HTEOPDsetShortRange(tMUXSensor muxsensor);
void HTEOPDsetLongRange(tSensors link);
void HTEOPDsetLongRangeNW(tSensors link);
void HTEOPDsetLongRange(tMUXSensor muxsensor);

/**
 * Get the raw value from the sensor
 * @param link the HTEOPD port number
 * @return raw value of the sensor
 */
int HTEOPDreadRaw(tSensors link) {
  return 1023 - SensorRaw[link];
}


/**
 * Get the raw value from the sensor
 * @param muxsensor the SMUX sensor port number
 * @return raw value of the sensor
 */
int HTEOPDreadRaw(tMUXSensor muxsensor) {
  return 1023 - HTSMUXreadAnalogue(muxsensor);
}


/**
 * Get the processed value from the sensor. This is obtained by using sqrt(raw value * 10)
 * @param link the HTEOPD port number
 * @return processed value of the sensor
 */
int HTEOPDreadProcessed(tSensors link) {
  int _val = sqrt(HTEOPDreadRaw(link) * 10);
  return _val;
}


/**
 * Get the processed value from the sensor. This is obtained by using sqrt(raw value * 10)
 * @param muxsensor the SMUX sensor port number
 * @return processed value of the sensor
 */
int HTEOPDreadProcessed(tMUXSensor muxsensor) {
  int _val = sqrt((long)HTEOPDreadRaw(muxsensor) * (long)10);
  return _val;
}


/**
 * Set the range of the sensor to short range, this is done
 * by configuring the sensor as sensorRawValue
 * @param link the HTEOPD port number
 */
void HTEOPDsetShortRange(tSensors link) {
  SetSensorType(link, SRType);
  wait1Msec(10);
}


/**
 * Set the range of the sensor to short range, this is done
 * by configuring the sensor as sensorRawValue.<br>
 * This version does not wait (NW) 10ms after setting the range.
 * @param link the HTEOPD port number
 */
void HTEOPDsetShortRangeNW(tSensors link) {
  SetSensorType(link, SRType);
}


/**
 * Set the range of the sensor to short range, this is done
 * by switching off dig0
 * @param muxsensor the SMUX sensor port number
 */
void HTEOPDsetShortRange(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueInactive(muxsensor);
}


/**
 * Set the range of the sensor to long range, this is done
 * by configuring the sensor as sensorLightActive and setting
 * it to modeRaw
 * @param link the HTEOPD port number
 */
void HTEOPDsetLongRange(tSensors link) {
  SetSensorType(link, LRType);
  wait1Msec(10);
}


/**
 * Set the range of the sensor to long range, this is done
 * by configuring the sensor as sensorLightActive and setting
 * it to modeRaw.<br>
 * This version does not wait (NW) 10ms after setting the range.
 * @param link the HTEOPD port number
 */
void HTEOPDsetLongRangeNW(tSensors link) {
  SetSensorType(link, LRType);
}


/**
 * Set the range of the sensor to long range, this is done
 * by setting dig0 high (1).
 * @param muxsensor the SMUX sensor port number
 */
void HTEOPDsetLongRange(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueActive(muxsensor);
}

#endif // __HTEOPD_H__

/*
 * $Id: HTEOPD-driver.h 29 2010-06-25 12:55:41Z xander $
 */
/* @} */
/* @} */
