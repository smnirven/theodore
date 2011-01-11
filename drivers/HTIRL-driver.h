/*!@addtogroup HiTechnic
 * @{
 * @defgroup htirl IR Link Sensor
 * HiTechnic IR Link Sensor driver
 * @{
 */

/*
 * $Id: HTIRL-driver.h 29 2010-06-25 12:55:41Z xander $
 */

#ifndef __HTIRL_H__
#define __HTIRL_H__
/** \file HTIRL-driver.h
 * \brief HiTechnic IR Link Sensor driver
 *
 * HTIRL-driver.h provides an API for the HiTechnic IR Link Sensor. Currently only
 * the Power Functions API has been implemented.
 *
 * Changelog:
 * - 1.0: Initial release
 * - 1.1: Minor changes
 * - 1.2: Rewrite to make use of the new common.h API
 * - 1.3: Clarified port numbering
 * - 1.4: Removed inline functions
 *
 * Credits:
 * - Big thanks to HiTechnic for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date 25 May 2010
 * \version 1.4
 * \example HTIRL-test1.c
 */

#pragma systemFile

// #define __DEBUG_DRIVER__    /*!< Compile the driver with debugging code */

#ifdef __DEBUG_DRIVER__
#warn "-----------------------------------------"
#warn "HTIRL DRIVER COMPILED WITH DEBUGGING CODE"
#warn "-----------------------------------------"
#endif

#ifndef __COMMON_H__
#include "common.h"
#endif

#define BUF_HEADSIZE    3   /*!< I2C buff size, address and register */
#define BUF_DATASIZE    11  /*!< max size of encoded buffer */
#define BUF_TAILSIZE    3   /*!< IR data length, IR Link mode and start transmission */
#define START_HEAD      0   /*!< index of start of header */
#define START_DATA      3   /*!< index of start of data payload */
#define START_TAIL      15  /*!< index of start of tail */

/*!< Combo PWM Mode commands */
typedef enum {
  CPM_MOTOR_FLOAT = 0,      /*!< Float the motor */
  CPM_MOTOR_FWD_PWM_1 = 1,  /*!< Forward speed 1 */
  CPM_MOTOR_FWD_PWM_2 = 2,  /*!< Forward speed 2 */
  CPM_MOTOR_FWD_PWM_3 = 3,  /*!< Forward speed 3 */
  CPM_MOTOR_FWD_PWM_4 = 4,  /*!< Forward speed 4 */
  CPM_MOTOR_FWD_PWM_5 = 5,  /*!< Forward speed 5 */
  CPM_MOTOR_FWD_PWM_6 = 6,  /*!< Forward speed 6 */
  CPM_MOTOR_FWD_PWM_7 = 7,  /*!< Forward speed 7 */
  CPM_MOTOR_BRAKE = 8,      /*!< Brake the motor */
  CPM_MOTOR_BAK_PWM_7 = 9,  /*!< Reverse speed 7 */
  CPM_MOTOR_BAK_PWM_6 = 10, /*!< Reverse speed 6 */
  CPM_MOTOR_BAK_PWM_5 = 11, /*!< Reverse speed 5 */
  CPM_MOTOR_BAK_PWM_4 = 12, /*!< Reverse speed 4 */
  CPM_MOTOR_BAK_PWM_3 = 13, /*!< Reverse speed 3 */
  CPM_MOTOR_BAK_PWM_2 = 14, /*!< Reverse speed 2 */
  CPM_MOTOR_BAK_PWM_1 = 15  /*!< Reverse speed 1 */
} eCPMMotorCommand;

/*!< Combo Direct Mode commands */
typedef enum {
  CDM_MOTOR_FLOAT = 0,      /*!< Float the motor */
  CDM_MOTOR_FWD = 1,        /*!< Forward */
  CDM_MOTOR_BAK = 2,        /*!< Reverse */
  CDM_MOTOR_BRAKE = 3       /*!< Brake the motor */
} eCDMMotorCommand;

// Function prototypes
// inline void addI2CHead(tByteArray &data);
// inline void addI2CTail(tByteArray &data);
void PFcomboDirectMode(tSensors link, int channel, eCDMMotorCommand _motorB, eCDMMotorCommand _motorA);
void PFcomboPwmMode(tSensors link, int channel, eCPMMotorCommand _motorB, eCPMMotorCommand _motorA);
void encodeBuffer(tByteArray &iBuffer, tByteArray &oBuffer);
void transmitIR(tSensors link, tByteArray &oBuffer, int channel, int resend);

#ifdef __DEBUG_DRIVER__
void decToBin(int number, int length, string &output);
void debugIR(tByteArray &data);

/**
 * Returns a binary representation in a string of an int with specified length
 *
 * Note: this function is only available when driver is compiled with __DEBUG_DRIVER__ defined.
 * @param number the number to be converted to a binary representation
 * @param length number of bits to convert
 * @param output the number converted to binary representation
 */
void decToBin(int number, int length, string &output) {
  memset(output, 0, sizeof(output));
  output = "";

  for (int i = 0; i < length; i++) {
    output += (number & (1<< (length - 1))) >> (length - 1);
    number = number << 1;
  }
}

/**
 * Print out the buffer in question to the screen using the following format:
 *
 * @<index@> @<binary reprentation@> @<hex representation@>
 *
 * 0 11001100 0xCC
 *
 * It pauses for 10 seconds between each screenful, accompanied by a beep.
 *
 * Note: this function is only available when driver is compiled with __DEBUG_DRIVER__ defined.
 * @param data the data to be displayed as binary/hex numbers
 */
void debugIR(tByteArray &data) {
  string _output;
  for (int i = 0; i < MAX_ARR_SIZE; i++) {
    if ((i != 0) && (i % 8 == 0)) {
      wait1Msec(10000);
      PlaySound(soundBlip);
      eraseDisplay();
    }
    decToBin(data.arr[i], 8, _output);
    StringFormat(_output, "%2d %s", i, _output);
    nxtDisplayTextLine(i % 8, "%s 0x%02x", _output, ubyteToInt(data.arr[i]));
  }
  wait1Msec(10000);
}
#endif // __DEBUG_DRIVER__


/**
 * Control two motors using the ComboDirectMode.  This mode does not allow for fine grained
 * speed control.
 * @param link the sensor port number
 * @param channel the channel of the receiver we wish to communicate with, numbered 0-3
 * @param _motorB the command to be sent to Motor B
 * @param _motorA the command to be sent to Motor A
 */
void PFcomboDirectMode(tSensors link, int channel, eCDMMotorCommand _motorB, eCDMMotorCommand _motorA) {
  tByteArray _iBuffer;
  tByteArray _oBuffer;

  // Clear the input buffer before we start filling it
  memset(_iBuffer, 0, sizeof(tByteArray));
  memset(_oBuffer, 0, sizeof(tByteArray));

  // This is the unencoded command for the IR receiver
  _iBuffer.arr[0] = (channel << 4) + 1;
  _iBuffer.arr[1] = (_motorB << 6) + (_motorA << 4);
  _iBuffer.arr[1] += 0xF ^ (_iBuffer.arr[0] >> 4) ^ (_iBuffer.arr[0] & 0xF) ^ (_iBuffer.arr[1] >> 4);

  // Setup the header of the I2C packet
  _oBuffer.arr[0] = 16;    // Total msg length
  _oBuffer.arr[1] = 0x02;  // I2C device address
  _oBuffer.arr[2] = 0x42;  // Internal register

  // Generate the data payload
  encodeBuffer(_iBuffer, (tByteArray)_oBuffer);                       // Encode PF command

  // Setup the tail end of the packet
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE] = 11;         // Total IR command length
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE + 1] = 0x02;   // IRLink mode 0x02 is PF motor
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE + 2] = 0x01;   // Start transmitting

  transmitIR(link, _oBuffer, channel, 4);
}

/*
  =============================================================================
  Combo Direct Mode
        | Nib 1 | Nib 2 | Nib 3 | Nib 4 |
  start  a 1 C C B B B B A A A A L L L L stop

 */
/**
 * Control two motors using the ComboPWMMode.  This mode allows for fine grained
 * speed control.
 * @param link the sensor port number
 * @param channel the channel of the receiver we wish to communicate with, numbered 0-3
 * @param _motorB the command to be sent to Motor B
 * @param _motorA the command to be sent to Motor A
 */
void PFcomboPwmMode(tSensors link, int channel, eCPMMotorCommand _motorB, eCPMMotorCommand _motorA) {
  tByteArray _iBuffer;
  tByteArray _oBuffer;

  // Clear the input buffer before we start filling it
  memset(_iBuffer, 0, sizeof(tByteArray));
  memset(_oBuffer, 0, sizeof(tByteArray));

  // This is the unencoded command for the IR receiver
  _iBuffer.arr[0] = (1 << 6) + (channel << 4) + _motorA;
  _iBuffer.arr[1] = (_motorB << 4);
  _iBuffer.arr[1] = (_motorB << 4) + (0xF ^ ((1 << 2) + channel) ^ _motorA ^ _motorB);

  // Setup the header of the I2C packet
  _oBuffer.arr[0] = 16;    // Total msg length
  _oBuffer.arr[1] = 0x02;  // I2C device address
  _oBuffer.arr[2] = 0x42;  // Internal register
  // Generate the data payload
  encodeBuffer(_iBuffer, _oBuffer);                       // Encode PF command

  // Setup the tail end of the packet
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE] = 11;         // Total IR command length
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE + 1] = 0x02;   // IRLink mode 0x02 is PF motor
  _oBuffer.arr[BUF_HEADSIZE + BUF_DATASIZE + 2] = 0x01;   // Start transmitting
  //_oBuffer[3] = 0x80;

  transmitIR(link, _oBuffer, channel, 4);
}

/**
 * Encode the input buffer into a special format for the IRLink.
 *
 * Note: this is an internal function and should not be called directly.
 * @param iBuffer the data that is be encoded
 * @param oBuffer output buffer for encoded data
 */
void encodeBuffer(tByteArray &iBuffer, tByteArray &oBuffer) {
  int _oByteIdx = 0;
  int _oBitIdx = 0;
  int _iIndex = 0;              // _iBUffer bit index
  int _oIndex = 0;              // _oBuffer bit index
  int _len = 0;

  // Calculate the size of the output bit index
  _oIndex = (8 * (MAX_ARR_SIZE - BUF_HEADSIZE)) - 1;

  // Start bit is a special case and is encoded as 0x80
  oBuffer.arr[START_DATA] = 0x80; // Start bit
  _oIndex -= 8;                   // move the index along 8 bits.

  // Bits in the input buffer are encoded as follows:
  // 1 is encoded as 10000
  // 0 is encoded as 100
  // The encoded bits are tacked onto the end of the output
  // buffer, byte boundaries are ignored.
  for (_iIndex = 0; _iIndex < (2 * 8); _iIndex++) {
    _len = (iBuffer.arr[_iIndex / 8] & 0x80) ? 5 : 3;
    _oByteIdx = (MAX_ARR_SIZE - 1) - (_oIndex / 8);
    _oBitIdx = _oIndex % 8;
    oBuffer.arr[_oByteIdx] += (1 << _oBitIdx);
    _oIndex -= _len;
    iBuffer.arr[_iIndex / 8] <<= 1;
  }

  // Finally, add the stop byte to the end of our command
  _oByteIdx = (MAX_ARR_SIZE - 1) - (_oIndex / 8);
  _oBitIdx = _oIndex % 8;
  oBuffer.arr[_oByteIdx] += (1 << _oBitIdx);

}

/**
 * Send the command to the IRLink Sensor for transmission.
 *
 * Note: this is an internal function and should not be called directly.
 * If the driver is compiled with __DEBUG_DRIVER__, this function will call
 * debugIR() prior to transmitting the data for debugging purposes.
 * @param link the sensor port number
 * @param oBuffer the data that is be transmitted
 * @param channel the channel number of the receiver
 * @param resend the number of times the command should be resent
 */
void transmitIR(tSensors link, tByteArray &oBuffer, int channel, int resend) {
#ifdef __DEBUG_DRIVER__
  debugIR(oBuffer);
#endif // __DEBUG_DRIVER__
  for (int i = 0; i < resend; i++)
  if (!writeI2C(link, oBuffer, 0)) {
    eraseDisplay();
    PlaySound(soundException);
    nxtDisplayTextLine(3, "ERROR!!");
    wait1Msec(2000);
    StopAllTasks();
  } else {
    wait1Msec(48);
  }
}

#endif // __HTIRL_H__

/*
 * $Id: HTIRL-driver.h 29 2010-06-25 12:55:41Z xander $
 */
/* @} */
/* @} */
