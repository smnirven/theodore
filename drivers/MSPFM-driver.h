/*!@addtogroup mindsensors
 * @{
 * @defgroup pfmate PFMate Sensor
 * PFMate Sensor
 * @{
 */

/*
 * $Id: MSPFM-driver.h 29 2010-06-25 12:55:41Z xander $
 */

/** \file MSPFM-driver.h
 * \brief Mindsensors PFMate Sensor driver
 *
 * MSPFM-driver.h provides an API for the Mindsensors PFMate Sensor driver
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
 * \date 22 July 2009
 * \version 0.1
 * \example MSPFM-test1.c
 */

#pragma systemFile

#ifndef __MSPFM_H__
#define __MSPFM_H__

#ifndef __COMMON_H__
#include "common.h"
#endif

#define MSPFM_I2C_ADDR    0x48      /*!< I2C address for sensor */
#define MSPFM_CMD         0x41      /*!< Command register */
#define MSPFM_IRCHAN      0x42      /*!< IR Channel selection register */
#define MSPFM_MSELECT     0x43      /*!< Motor selection register */
#define MSPFM_MOTOPA      0x44      /*!< Motor A operation register */
#define MSPFM_MOTSPA      0x45      /*!< Motor A speed register */
#define MSPFM_MOTOPB      0x46      /*!< Motor B operation register */
#define MSPFM_MOTSPB      0x47      /*!< Motor B speed register */

#define MSPFM_GOCMD       0x47      /*!< "Go" command to start sending IR signals to PF receiver */

#define MSPFM_MOTORAB     0x00
#define MSPFM_MOTORA      0x01
#define MSPFM_MOTORB      0x02

#define MSPFM_FLOAT       0x00
#define MSPFM_FORWARD     0x01
#define MSPFM_REVERSE     0x02
#define MSPFM_BRAKE       0x03

tByteArray MSPFM_I2CRequest;       /*!< Array to hold I2C command data */


/**
 * Send a direct command to the PFMate sensor
 *
 * Note: this is an internal command and should not be called directly.
 * @param link the PFMate port number
 * @param command the command to be sent
 * @return true if no error occured, false if it did
 */
bool _MSPFMsendCommand(tSensors link, byte command) {
  memset(MSPFM_I2CRequest, 0, sizeof(tByteArray));
  MSPFM_I2CRequest.arr[0] = 3;
  MSPFM_I2CRequest.arr[1] = MSPFM_I2C_ADDR;
  MSPFM_I2CRequest.arr[2] = MSPFM_CMD;
  MSPFM_I2CRequest.arr[3] = command;

  return writeI2C(link, MSPFM_I2CRequest, 0);
}


/**
 * Control motors A and B with the PFMate. Must be followed by a MSHID_GOCMD
 * command using _MSPFMsendCommand()
 *
 * Note: this is an internal command and should not be called directly.
 * @param link the PFMate port number
 * @param chan the channel of the IR receiver, value of 1-4
 * @param mselect select the motors to operate on, 0: both motors, 1: motor A, 2: motor B
 * @param motorA_op motor A operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motorA_sp the speed at which motor A should turn, value between 1-7
 * @param motorB_op motor B operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motorB_sp the speed at which motor B should turn, value between 1-7
 * @return true if no error occured, false if it did
 */
bool _MSPFMcontrolMotors(tSensors link, byte chan, byte mselect, byte motorA_op, byte motorA_sp, byte motorB_op, byte motorB_sp) {
  memset(MSPFM_I2CRequest, 0, sizeof(tByteArray));
  MSPFM_I2CRequest.arr[0] = 8;
  MSPFM_I2CRequest.arr[1] = MSPFM_I2C_ADDR;
  MSPFM_I2CRequest.arr[2] = MSPFM_IRCHAN;
  MSPFM_I2CRequest.arr[3] = chan;
  MSPFM_I2CRequest.arr[4] = mselect;
  MSPFM_I2CRequest.arr[5] = motorA_op;
  MSPFM_I2CRequest.arr[6] = motorA_sp;
  MSPFM_I2CRequest.arr[7] = motorB_op;
  MSPFM_I2CRequest.arr[8] = motorB_sp;

  return writeI2C(link, MSPFM_I2CRequest, 0);
}


/**
 * Control motor A with the PFMate.
 *
 * @param link the PFMate port number
 * @param chan the channel of the IR receiver, value of 1-4
 * @param motor_op motor A operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motor_speed the speed at which motor A should turn, value between 1-7
 * @return true if no error occured, false if it did
 */
bool MSPFMcontrolMotorA(tSensors link, byte chan, byte motor_op, byte motor_speed) {
  if (!_MSPFMcontrolMotors(link, chan, MSPFM_MOTORA, motor_op, motor_speed, 0, 0))
    return false;
  return _MSPFMsendCommand(link, MSPFM_GOCMD);
}


/**
 * Control motor B with the PFMate.
 *
 * @param link the PFMate port number
 * @param chan the channel of the IR receiver, value of 1-4
 * @param motor_op motor B operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motor_speed the speed at which motor B should turn, value between 1-7
 * @return true if no error occured, false if it did
 */
bool MSPFMcontrolMotorB(tSensors link, byte chan, byte motor_op, byte motor_speed) {
  if (!_MSPFMcontrolMotors(link, chan, MSPFM_MOTORB, 0, 0, motor_op, motor_speed))
    return false;
  return _MSPFMsendCommand(link, MSPFM_GOCMD);
}


/**
 * Control motors A and B with the PFMate.
 *
 * @param link the PFMate port number
 * @param chan the channel of the IR receiver, value of 1-4
 * @param motorA_op motor A operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motorA_speed the speed at which motor A should turn, value between 1-7
 * @param motorB_op motor B operation, 0: float, 1: forward, 2: reverse, 3: brake
 * @param motorB_speed the speed at which motor B should turn, value between 1-7
 * @return true if no error occured, false if it did
 */
bool MSPFMcontrolMotorAB(tSensors link, byte chan, byte motorA_op, byte motorA_speed, byte motorB_op, byte motorB_speed) {
  if (!_MSPFMcontrolMotors(link, chan, MSPFM_MOTORAB, motorA_op, motorA_speed, motorB_op, motorB_speed))
    return false;
  return _MSPFMsendCommand(link, MSPFM_GOCMD);
}

#endif // __MSPFM_H__

/*
 * $Id: MSPFM-driver.h 29 2010-06-25 12:55:41Z xander $
 */
/* @} */
/* @} */
