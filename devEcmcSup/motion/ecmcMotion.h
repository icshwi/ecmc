/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*
*  ecmcMotion.h
*
*  Created on: Jan 10, 2019
*      Author: anderssandstrom
*
\*************************************************************************/

/**\file
* \defgroup ecmc
* \author Anders Sandström
* \contact anders.sandstrom@esss.se
*/

#ifndef ECMC_MOTION_H_
#define ECMC_MOTION_H_

#include "../com/ecmcOctetIF.h"        // Log Macros
#include "../main/ecmcErrorsList.h"
#include "../main/ecmcDefinitions.h"

# ifdef __cplusplus
extern "C" {
# endif  // ifdef __cplusplus


#define CHECK_AXIS_RETURN_IF_ERROR(axisIndex)                                 \
  {                                                                           \
    if (axisIndex >= ECMC_MAX_AXES || axisIndex <= 0) {                       \
      LOGERR("ERROR: Axis index out of range.\n");                            \
      return ERROR_MAIN_AXIS_INDEX_OUT_OF_RANGE;                              \
    }                                                                         \
    if (axes[axisIndex] == NULL) {                                            \
      LOGERR("ERROR: Axis object NULL\n");                                    \
      return ERROR_MAIN_AXIS_OBJECT_NULL;                                     \
    }                                                                         \
  }                                                                           \

#define CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)                   \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getBlockExtCom()) {                                    \
    LOGERR("ERROR: External Commands Disabled\n");                            \
    return ERROR_MAIN_AXIS_EXTERNAL_COM_DISABLED;                             \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex)                         \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->                                                       \
    getEnc() == NULL) {                                                       \
    LOGERR("ERROR: Encoder object NULL.\n");                                  \
    return ERROR_MAIN_ENCODER_OBJECT_NULL;                                    \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex)                      \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getCntrl() ==NULL) {                                   \
    LOGERR("ERROR: Controller object NULL.\n");                               \
    return ERROR_MAIN_CONTROLLER_OBJECT_NULL;                                 \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex)                           \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getDrv() == NULL) {                                    \
    LOGERR("ERROR: Drive object NULL.\n");                                    \
    return ERROR_MAIN_DRIVE_OBJECT_NULL;                                      \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)                            \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getTraj() == NULL) {                                   \
    LOGERR("ERROR: Trajectory object NULL.\n");                               \
    return ERROR_MAIN_TRAJECTORY_OBJECT_NULL;                                 \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)                             \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getSeq()== NULL) {                                     \
    LOGERR("ERROR: Sequence object NULL.\n");                                 \
    return ERROR_MAIN_SEQUENCE_OBJECT_NULL;                                   \
  }                                                                           \
}                                                                             \

#define CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)                             \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getMon() == NULL) {                                    \
    LOGERR("ERROR: Monitor object NULL.\n");                                  \
    return ERROR_MAIN_MONITOR_OBJECT_NULL;                                    \
  }                                                                           \
}                                                                             \

/*#define CHECK_AXIS_TRAJ_TRANSFORM_RETURN_IF_ERROR(axisIndex)                  \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getExternalTrajIF()->getExtInputTransform() == NULL) { \
    LOGERR("ERROR: Trajectory transform object NULL.\n");                     \
    return ERROR_MAIN_TRAJ_TRANSFORM_OBJECT_NULL;                             \
  }                                                                           \
} */                                                                            \

/*#define CHECK_AXIS_ENC_TRANSFORM_RETURN_IF_ERROR(axisIndex)                   \
{                                                                             \
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);                                      \
  if (axes[axisIndex]->getExternalEncIF()->getExtInputTransform() == NULL) {  \
    LOGERR("ERROR: Encoder transform object NULL.\n");                        \
    return ERROR_MAIN_ENC_TRANSFORM_OBJECT_NULL;                              \
  }                                                                           \
}*/

/** \brief Move axis to an absolute position.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in] positionSet Position setpoint.\n
 * \param[in] velocitySet Velocity setpoint.\n
 * \param[in] accelerationSet Acceleration setpoint.\n
 * \param[in] decelerationSet Deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Move axis 5 to position 1234 with a velocity of 10, acceleration of 100, and deceleration of 200.\n
 * "MoveAbsolutePosition(5,1234,10,100,200)" //Command string to ecmcCmdParser.c\n
 */
int moveAbsolutePosition(
  int    axisIndex,
  double positionSet,
  double velocitySet,
  double accelerationSet,
  double decelerationSet);

/** \brief Move axis to a relative position.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in] positionSet Relative position setpoint.\n
 * \param[in] velocitySet Velocity setpoint.\n
 * \param[in] accelerationSet Acceleration setpoint.\n
 * \param[in] decelerationSet Deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Move axis 5 to 123 relative to current position with a velocity of 10,
 * acceleration of 100, and deceleration of 200.\n
 * "MoveRelativePosition(5,1234,10,100,200)" //Command string to ecmcCmdParser.c\n
 */
int moveRelativePosition(
  int    axisIndex,
  double positionSet,
  double velocitySet,
  double accelerationSet,
  double decelerationSet);

/** \brief Move axis in a constant velocity.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in] velocitySet Velocity setpoint.\n
 * \param[in] accelerationSet Acceleration setpoint.\n
 * \param[in] decelerationSet Deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Move axis 5  at a constant velocity of 10, acceleration of
 * 100, and deceleration of 200.\n
 * "MoveVelocity(5,10,100,200)" //Command string to ecmcCmdParser.c\n
 */
int moveVelocity(int    axisIndex,
                 double velocitySet,
                 double accelerationSet,
                 double decelerationSet);

/** \brief Stop axis.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in] killAmplifier Disable amplifier.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Stop motion and kill amplifier of axis 2.\n
 * "StopMotion(2,1)" //Command string to ecmcCmdParser.c\n
 */
int stopMotion(int axisIndex,
               int killAmplifier);

/** \brief Get axis error state.\n
 *
 * \param[in] axisIndex Axis index.\n
 *
 * \return axis error state.\n
 *
 * \note Example: Get error code of axis 3.\n
 * "Main.M3.bError?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int         getAxisError(int axisIndex);

/** \brief Get axis error code.\n
 *
 * \param[in] axisIndex Axis index.\n
 *
 * \return axis error code.\n
 *
 * \note Example: Get error code of axis 3.\n
 * "Main.M3.nErrorId?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int         getAxisErrorID(int axisIndex);

/** \brief Get axis execution cycle counter.\n
 * Can be used for checking that logic for an axis object is
 * executing.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[out] counter Execution cycle counter.\n
 *
 * \return  error code.\n
 * \note The counter can overflow.
 * \note Example: Get cycle counter of axis 3.\n
 * "GetAxisCycleCounter(3)" //Command string to ecmcCmdParser.c.\n
  */
int         getAxisCycleCounter(int  axisIndex,
                                int *counter);

/** \brief Get axis debug information string.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in,out] buffer Pointer to char output data buffer.\n
 * \param[in] bufferByteSize Size of data buffer.\n
 *
 * \return error code.\n
 *
 * \note Example: Get information of axis 3.\n
 * "GetAxisDebugInfoData(3)" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisDebugInfoData(int   axisIndex,
                         char *buffer,
                         int   bufferByteSize);

/** \brief Get axis status structure V2.\n
 *
 * \param[in] axisIndex Axis index.\n
 * \param[in,out] buffer Pointer to char output data buffer.\n
 * \param[in] bufferByteSize Size of data buffer.\n
 *
 * \return error code.\n
 *
 * \note Example: Get information of axis 3.\n
 * "Main.M3.stAxisStatusV2?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisStatusStructV2(int   axisIndex,
                          char *buffer,
                          int   bufferByteSize);

/** \brief Get axis execute bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis execute bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get execute bit for axis 3.\n
 * "Main.M3.bExecute?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisExecute(int  axisIndex,
                   int *value);

/** \brief Get axis command word.\n
 *
 * The command word defines different modes of operation for and axis. See
 * fbDriveVirtual manual for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  axis command word.\n
 *  value = 0: Jog.\n
 *  value = 1: Constant Velocity.\n
 *  value = 2: Relative Positioning.\n
 *  value = 3: Absolute Positioning.\n
 *  value = 10: Homing.\n
 *  See manual for fbDriveVirtual for more information.
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get command word for axis 3.\n
 * "Main.M3.nCommand?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisCommand(int  axisIndex,
                   int *value);

/** \brief Get axis command data word.\n
 *
 * The command data word is an argument linked to the axis command. See
 * fbDriveVirtual manual for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  axis command data word.\n
 *
 * Command word = 10 (homing):\n
 *   value 1..6: Different homing sequences.\n
 *
 * Command word = 4 (absolute positioning): \n
 *   value 1: Go to start position of trajectory generator external
 *   transformation expressions. This is useful to avoid jumps  during
 *   start phase when absolute synchronizing.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get command data word for axis 3.\n
 * "Main.M3.nCmdData?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisCmdData(int  axisIndex,
                   int *value);

/** \brief Get axis reset bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis reset bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get reset bit for axis 3.\n
 * "Main.M3.bReset?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisReset(int  axisIndex,
                 int *value);

/** \brief Get axis amplifier state bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis amplifier.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get amplifier actual state for axis 3.\n
 * "Main.M3.bEnabled?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEnabled(int  cntrl_no,
                   int *value);

/** \brief Get axis amplifier command bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis amplifier command bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get amplifier enable command bit for axis 3.\n
 * "Main.M3.bEnable?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEnable(int  axisIndex,
                  int *value);

/** \brief Get axis block external com.\n
 *   If true the axis will not take any active commands via cmd\n
 *   parser (statuses can still be read).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] block  Axis blocked.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get axis block for axis 3.\n
 * "GetAxisBlockCom(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisBlockCom(int  axisIndex,
                    int *block);

/** \brief Get axis busy bit.\n
 *
 * The axis busy bit is high while an command is executed or while synchronizing to other axes.
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis busy bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get busy state for axis 3.\n
 * "Main.M3.bBusy?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisBusy(int  axisIndex,
                int *value);

/** \brief Get axis index.\n
 *
 * This function is only implemented for compatibility reasons with the
 * ESS MCAG TwinCAT implementation. (An TwinCAT plc axis can be linked to
 * different NC-axis).
 *
 * If the axis at axisIndex exists, axisIndex will be "returned" in the value parameter.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  axis index of chosen axis.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get busy state for axis 3.\n
 * "Main.M3.nMotionAxisID?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisID(int  axisIndex,
              int *value);

/** \brief Get enable alarms at limits bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value state of enable alarm at limits bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get state of enable alarm at limits bit for axis 3.\n
 * "GetAxisEnableAlarmAtHardLimits(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisEnableAlarmAtHardLimits(int  axisIndex,
                                   int *value);

/** \brief Get backward soft-limit position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value soft-limit position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get soft-limit backward position for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#D,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisSoftLimitPosBwd(int     axisIndex,
                           double *value);

/** \brief Get forward soft-limit position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Soft-limit position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get soft-limit forward position for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#E,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisSoftLimitPosFwd(int     axisIndex,
                           double *value);

/** \brief Get backward soft-limit enabled state of an axis.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value soft-limit enabled.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get soft-limit backward enabled state for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#B,2,2?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEnableSoftLimitBwd(int  axisIndex,
                              int *value);

/** \brief Get forward soft-limit enabled state of an axis.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value soft-limit enabled.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get soft-limit forward enabled state for axis 3.\n
 * "ADSPORT=501/.ADR.16#5001,16#C,2,2?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEnableSoftLimitFwd(int  axisIndex,
                              int *value);

/** \brief Get axis operation mode.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value current operation mode of axis.\n
 * value = 0 Axis is in automatic mode (controller enabled).\n
 * value = 1 Axis is in manual mode (controller and some "safety features
 * disabled). A direct output to the drive can be set in this mode.
 * Only useful in commissioning.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get current operation mode of axis 3.\n
 * "GetAxisOpMode(3)"  //Command string to ecmcCmdParser.c.\n
 */
int getAxisOpMode(int  axisIndex,
                  int *value);

/** \brief Get axis acceleration setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value acceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get acceleration setpoint for axis 3.\n
 * "Main.M3.fAcceleration?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisAcceleration(int     axisIndex,
                        double *value);

/** \brief Get axis deceleration setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get deceleration setpoint for axis 3.\n
 * "Main.M3.fDeceleration?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisDeceleration(int     axisIndex,
                        double *value);

/** \brief Get axis target position setpoint.\n
 *
 * The target position is the desired end setpoint of a motion.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value target position setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get target position setpoint for axis 3.\n
 * "Main.M3.fPosition?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisTargetPos(int     axisIndex,
                     double *value);

/** \brief Get axis target velocity setpoint.\n
 *
 * The target velocity is the desired velocity of a motion.\n
 * Note: The actual velocity can be higher than this setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value target velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get target velocity setpoint for axis 3.\n
 * "Main.M3.fVelocity?"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisTargetVel(int     axisIndex,
                     double *value);

/** \brief Get axis done bit.\n
 *
 * The axis done bit is high when an axis is ready to take a new command
 * (inverse of busy bit).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of axis done bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get done state for axis 3.\n
 * "Main.M3.bDone?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisDone(int axisIndex,
                int *value);

/** \brief Get gear ration setting.\n
 *
 * Note: The gear ratio is only valid during synchronized motions.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  gear ratio.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get gear ratio setting for axis 3.\n
 * "ReadAxisGearRatio(3)" //Command string to ecmcCmdParser.c.\n
 */
/*int getAxisGearRatio(int     axisIndex,
                     double *value);*/

/** \brief Get state of forward hard limit.\n
 *
 * Checks state of forward hard limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of forward limit switch.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get state of forward limit switch for axis 3.\n
 * "Main.M3.bLimitFwd?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisAtHardFwd(int  axisIndex,
                     int *value);

/** \brief Get state of backward hard limit.\n
 *
 * Checks state of backward hard limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of backward limit switch.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get state of backward limit switch for axis 3.\n
 * "Main.M3.bLimitBwd?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisAtHardBwd(int  axisIndex,
                     int *value);

/** \brief Get encoder homed bit.\n
 *
 * Checks if encoder has been homed.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of encoder homed bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Check if encoder of axis 3 has been homed.\n
 * "Main.M3.bHomed?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEncHomed(int  axisIndex,
                    int *value);

/** \brief Get actual encoder position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  encoder actual position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get actual encoder position for axis 3.\n
 * "Main.M3.fActPosition?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEncPosAct(int     axisIndex,
                     double *value);

/** \brief Get actual encoder velocity.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  encoder actual velocity.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get actual encoder velocity for axis 3.\n
 * "Main.M3.fActVelocity?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEncVelAct(int     axisIndex,
                     double *value);

/** \brief Get state of reference/home switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  state of reference/home switch.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get state of reference/home switch for axis 3.\n
 * "Main.M3.stAxisStatus?" //The command returns a complete structure of
 * information including the state of the reference switch
 * //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisAtHome(int  axisIndex,
                  int *value);

/** \brief Get actual pid controller error.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value actual error of pid controller.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get pid controller error for axis 3.\n
 * "Main.M3.stAxisStatus?" //The command returns a complete structure of
 * information including the state of the pid controller error *
 * //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisCntrlError(int     axisIndex,
                      double *value);

/** \brief Get off cam referencing/homing velocity setpoint.\n
 *
 * This velocity setpoint is only used during homing sequence when the
 * reference switch already have been found. See command
 * "getAxisHomeVelTwordsCam()" for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get off cam referencing/homing velocity setpoint for axes 3.\n
 * "ADSPORT=501/.ADR.16#4003,16#7,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisHomeVelOffCam(int     axisIndex,
                         double *value);

/** \brief Get twords cam referencing/homing velocity setpoint.\n
 *
 * This velocity setpoint is only used during homing sequence when finding
 * the reference switch. See command "getAxisHomeVelTwordsCam()" for more
 * information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get twords cam referencing/homing velocity setpoint for axes 3.\n
 * "ADSPORT=501/.ADR.16#4003,16#6,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisHomeVelTwordsCam(int     axisIndex,
                            double *value);

/** \brief Get the numerator part of the encoder scale.\n
 *
 * The encoder scale factor is divided into one numerator and one denominator
 * part. This function reads the numerator part.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Encoder scale numerator part.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get encoder scale numerator for axes 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#23,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEncScaleNum(int     axisIndex,
                       double *value);

/** \brief Get the denominator part of the encoder scale.\n
 *
 * The encoder scale factor is divided into one numerator and one denominator
 * part. This function reads the denominator part.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Encoder scale denominator part.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get encoder scale denominator for axes 8.\n
 * "ADSPORT=501/.ADR.16#5008,16#24,8,5?" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int getAxisEncScaleDenom(int     axisIndex,
                         double *value);

/** \brief Get raw unscaled encoder value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Raw encoder value.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get raw encoder value for axis 3.\n
 * "GetAxisEncPosRaw(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisEncPosRaw(int      axisIndex,
                     int64_t *value);

/** \brief Get PID-controller proportional output part.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value PID-controller proportional output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisCntrlOutPpart(int     axisIndex,
                         double *value);

/** \brief Get PID-controller integral output part.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value PID-controller integral output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisCntrlOutIpart(int     axisIndex,
                         double *value);

/** \brief Get PID-controller differential output part.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value PID-controller differential output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisCntrlOutDpart(int     axisIndex,
                         double *value);

/** \brief Get PID-controller feed forward output part.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value PID-controller feed forward output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisCntrlOutFFpart(int     axisIndex,
                          double *value);

/** \brief Get PID-controllegetAxisAtHomer total output part.\n
 *
 * The current total output from the PID-controller (the sum of the P,I,D and
 * FF part).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value PID-controller total output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisCntrlOutput(int     axisIndex,
                       double *value);

// int getAxisCntrlVelSet(int axisIndex,double *value);
// int getAxisCntrlRate(int axisIndex,double *value);

/** \brief Get the drive output scale factor.\n
 *
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Drive output scale factor.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisDrvScale(int     axisIndex,
                    double *value);

/** \brief Get enable state of drive.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Drive enabled.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisDrvEnable(int  axisIndex,
                     int *value);

/** \brief Get at target.\n
 *
 * Checks if axis is within a certain tolerance from target position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value At target.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note No command string implemented in the ecmcCmdParser.c parser.\n
 */
int getAxisMonAtTarget(int  axisIndex,
                       int *value);

/** \brief Get axis type.\n
 *
 * An axis can be of the following types:
 *   type = 1 : Normal axis (drive, encoder, monitor,pid-controller,trajectory).\n
 *   type = 2 : Virtual axis (encoder, monitor, trajectory).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Axis type.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get encoder scale denominator for axes 8.\n
 * "GetAxisType(8)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisType(int  axisIndex,
                int *value);

/** \brief Get axis trajectory transformation expression.\n
 *
 * The axis transformation expression is used for synchronization of axes. The
 * expression is a mathematical expression describing relation ship between
 * different axes.\n
 *
 * Example: "out:=sin(traj1+enc5)/500;".\n
 *   traj1 = trajectory setpoint of axis 1.\n
 *   enc5  = actual encoder position of axis 5.\n
 *   out   = the trajectory setpoint for the current axis (axes[axisIndex]).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] error Error code.\n
 * error = 0: No error.\n
 * error > 0: Error.\n
 *
 * \return pointer to transformation expression.\n
 *
 * \note Example: Get trajectory transformation expression for axes 5.\n
 * "GetAxisTrajTransExpr(5)" //Command string to ecmcCmdParser.c.\n
 */
const char* getAxisTrajTransExpr(int  axisIndex,
                                 int *error);

/** \brief Get axis encoder transformation expression.\n
 *
 * The axis transformation expression is used for synchronization of axes. The
 * expression is a mathematical expression describing relation ship between
 * different axes.\n
 *
 * Example: "out:=sin(traj1+enc5)/500;".\n
 *   traj1 = trajectory setpoint of axis 1.\n
 *   enc5  = actual encoder position of axis 5.\n
 *   out   = the encoder setpoint for the current axis (axes[axisIndex]).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] error Error code.\n
 * error = 0: No error.\n
 * error > 0: Error.\n
 *
 * \return pointer to transformation expression.\n
 *
 * \note Example: Get encoder transformation expression for axes 5.\n
 * "GetAxisEncTransExpr(5)" //Command string to ecmcCmdParser.c.\n
 */
const char* getAxisEncTransExpr(int  axisIndex,
                                int *error);

/** \brief Get axis sync. PLC expression.\n
 *
 * The axis sync PLC expression is used for enabling and executing of\n
 * axes based on mathematical expressions. This is useful when synchronizing\n
 * axes i.e. a slave axis could recive an custom trajatory, other enabled based\
 * on other axes or ethercat data in the form of mathematical expressions.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] error Error code.\n
 * error = 0: No error.\n
 * error > 0: Error.\n
 *
 * \return pointer to transformation expression.\n
 *
 * \note Example: Get axis sync. PLC expression for axes 5.\n
 * "getAxisPLCExpr(5)" //Command string to ecmcCmdParser.c.\n
 */
const char* getAxisPLCExpr(int  axisIndex,
                           int *error);

/** \brief Get axis trajectory data source.\n
 *
 * An axis trajectory generator can get position setpoints from different
 * sources:\n
 *   source = 0 : Internal (normal trajectory generation).\n
 *   source = 1 : External from transformation expression (synchronization).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Source type.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get trajectory generator data source type for axis 3.\n
 * "GetAxisTrajSourceType(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisTrajSource(int  axisIndex,
                      int *value);

/** \brief Get axis encoder data source.\n
 *
 * An axis encoder can get actual position from different sources:\n
 *   source = 0 : Internal (EtherCAT entry).\n
 *   source = 1 : External from transformation expression (synchronization).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Source type.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get encoder generator data source type for axis 3.\n
 * "GetAxisEncSourceType(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisEncSource(int  axisIndex,
                     int *value);

/** \brief Get axis allow command from PLC.\n
 *
 * An axis can receive commands from  PLCs (expressions).\n
 * However, the axis must be allow to receive these commands, see
 * command getAxisPLCExpr() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get axis enable command from other axis for axis 3.\n
 * "getAxisAllowCommandsFromPLC(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisAllowCommandsFromPLC(int  axisIndex,
                                 int *value);

/** \brief Get axis enable for axis sync. PLC.\n
 *
 * The axis sync. PLC  expression  can be enabled/disabled.
 * see command getAxisPLCExpr() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get axis enable command transform for axis 5.\n
 * "getAxisPLCEnable(5)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisPLCEnable(int  axisIndex,
                                   int *value);

/** \brief Set axis execute bit.\n
 *
 * An positive edge of the execute bit triggers a new command.\n
 * Motion is interlocked when the execute bit is low.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Execute bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set execute bit for axis 3.\n
 * "Main.M3.bExecute=1" //Command string to ecmcCmdParser.c.\n
 *
 * Example: Stop motion on axis 3.\n
 * "Main.M3.bExecute=0" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisExecute(int axisIndex,
                   int value);

/** \brief Set axis command word.\n
 *
 * The command word defines different modes of operation for and axis. See
 * fbDriveVirtual manual for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value  axis command word.\n
 *  value = 0: Jog.\n
 *  value = 1: Constant Velocity.\n
 *  value = 2: Relative Positioning.\n
 *  value = 3: Absolute Positioning.\n
 *  value = 10: Homing.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set command word to absolute positioning for axis 4.\n
 * "Main.M4.nCommand=3" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisCommand(int axisIndex,
                   int value);

/** \brief Set axis command data word.\n
 *
 * The command data word is an argument linked to the axis command. See
 * fbDriveVirtual manual for more information.\n
 * A new command will be triggered with a positive edge on the bExecute
 * flag.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value  Axis command data word.\n
 *
 * The command data word have different meaning depending on the command
 * word.\n
 *
 * Command word = 1 (Move velocity): \n
 *   Command data 0: Move Velocity\n
 *
 * Command word = 2 (Relative positioning): \n
 *   Command data 0: Move relative\n
 *
 * Command word = 3 (absolute positioning): \n
 *   Command data 0: Default absolute positioning\n
 *   Command Data 1: Go to start position of trajectory generator external
 *   transformation expressions. This is useful to avoid jumps  during
 *   start phase when absolute synchronizing.\n
 *
 * Command word = 10 (Referencing to fHomePosition):\n
 *   Command Data 0: Simplest homing sequence. The actual position of the
 *                   encoder will be set to fHomePosition.
 *   Command Data 1: Ref. on low limit switch.\n
 *   Command Data 2: Ref. on high limit switch.\n
 *   Command Data 3: Ref. on home sensor via low limit switch.\n
 *   Command Data 4: Ref. on home sensor via high limit switch.\n
 *   Command Data 5: Ref. on center of home sensor via low limit switch.\n
 *   Command Data 6: Ref. on center of home sensor via high limit switch.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Prepare homing sequence 3 at next positive edge of execute
 * for axis 3 (two command needed).\n
 * "Main.M3.nCommand=10" //Set homing. Command string to ecmcCmdParser.c.\n
 * "Main.M3.nCmdData=3"  //Set homing sequence. Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisCmdData(int axisIndex,
                   int value);

/** \brief Set axis amplifier enable command bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value  State of axis amplifier command bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set amplifier enable command bit for axis 3.\n
 * "Main.M3.bEnable=1" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisEnable(int axisIndex,
                  int value);

/** \brief Set enable alarms at limits bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable alarm at limits bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable alarm at limits bit for axis 3.\n
 * "Cfg.SetAxisEnableAlarmAtHardLimits(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEnableAlarmAtHardLimits(int axisIndex,
                                   int value);

/** \brief Block/unblock communicatiom from via cmd parser.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] block      block or unblock com.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Block Com for axis 3.\n
 * "Cfg.SetAxisBlockCom(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisBlockCom(int axisIndex,
                    int block);

/** \brief Set enable backward soft-limit of an axis.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Soft-limit enable command.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Disable backward soft-limit for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#B,2,2=0"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisEnableSoftLimitBwd(int axisIndex,
                              int value);

/** \brief Set enable forward soft-limit of an axis.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Soft-limit enable command.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable forward soft-limit for axis 3.\n
 * "ADSPORT=501/.ADR.16#5001,16#C,2,2=1"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisEnableSoftLimitFwd(int axisIndex,
                              int value);

/** \brief Set backward soft-limit position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Soft-limit position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set backward soft-limit position to -100 for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#E,8,5=-100" //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisSoftLimitPosBwd(int    axisIndex,
                           double value);

/** \brief Set forward soft-limit position.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Soft-limit position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set forward soft-limit position to 123 for axis 3.\n
 * "ADSPORT=501/.ADR.16#5003,16#E,8,5=123" //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisSoftLimitPosFwd(int    axisIndex,
                           double value);

/** \brief Set axis acceleration setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Acceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set acceleration setpoint for axis 3 to 500.\n
 * "Main.M3.fAcceleration=500"  //Command string to ecmcCmdParser.c.\n
 *
 * \note Example: Set deceleration setpoint for axis 3 to 500.\n
 * "Cfg.SetAxisAcc(3,500)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisAcceleration(int    axisIndex,
                        double value);

/** \brief Set axis deceleration setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set deceleration setpoint for axis 3 to 500.\n
 * "Main.M3.fDeceleration=500"  //Command string to ecmcCmdParser.c.\n
 *
 * \note Example: Set deceleration setpoint for axis 3 to 500.\n
 * "Cfg.SetAxisDec(3,500)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisDeceleration(int    axisIndex,
                        double value);

/** \brief Set axis emergency deceleration setpoint.\n
 *
 * The emergency deceleration setpoint is used:\n
 * - limit switch is engaged.\n
 * - both limit switches are engaged.\n
 * - ...
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Emergency deceleration setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set emergency deceleration setpoint for axis 3 to 54321.\n
 * "Cfg.SetAxisEmergDeceleration(3,54321)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisEmergDeceleration(int    axisIndex,
                             double value);

/** \brief Set axis maximum jerk setpoint. NOT USED!\n
 *
 * \note Currently not used!!!!!.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Jerk setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: No command string implemented in the ecmcCmdParser.c parser.\n
 */
int setAxisJerk(int    axisIndex,
                double value);

/** \brief Set axis target position setpoint.\n
 *
 * The target position is the desired end setpoint of a motion.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Target position setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set target position setpoint for axis 3 to 111.\n
 * "Main.M3.fPosition=111"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisTargetPos(int    axisIndex,
                     double value);

/** \brief Set axis target velocity setpoint.\n
 *
 * The target velocity is the desired velocity of a motion.\n
 * Note: The actual velocity can be higher than this setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Target velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set target velocity setpoint for axis 3 to 55.\n
 * "Cfg.SetAxisVel(3,55)"  //Command string to ecmcCmdParser.c.\n
 *
 * \note Example: Set target velocity setpoint for axis 3 to 55 in\n
 * "TwinCAT" syntax.\n
 * "Main.M3.fVelocity=55"  //Command string to ecmcCmdParser.c.\n
 *
 * \note The "CfgSetAxisVelAccDecTime()" Command can be used to set\n
 * velocity, acceleration and decelartion simultaneously\n
 * Acceleration and deceleration are defined by time to reach target velocity\n
 * Syntax: "Cfg.SetAxisVelAccDecTime(axisIndex,targetVel,timeToVel)"
 *
 * \note Example: Set target velocity setpoint for axis 3 to 55 and
 * time to reach target velocity to 2s (acc and dec will be set to the same\n
 * value).\n
 * "Cfg.SetAxisVelAccDecTime(3,55,2)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisTargetVel(int    axisIndex,
                     double value);

/** \brief Set axis jog velocity setpoint.\n
 *
 * The jog velocity is the desired velocity of a motion when executing a jog
 * command (command=10).\n
 * Note: The actual velocity can be higher than this setpoint.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Jog velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set target velocity setpoint for axis 3 to 55.\n
 * "Cfg.SetAxisJogVel(3,55)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisJogVel(int    axisIndex,
                  double value);

/** \brief Set axis operation mode.\n
 *
 * \note This command can disable certain safety functionality and
 * should only be used with high caution and only during commissioning.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Current operation mode of axis.\n
 * value = 0 Axis is in automatic mode (controller enabled).\n
 * value = 1 Axis is in manual mode (controller and some "safety features
 * disabled). A direct output to the drive can be set in this mode.
 * Only useful in commissioning.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set current operation mode of axis 3 to manual.\n
 * "Cfg.SetAxisOpMode(3,1)"  //Command string to ecmcCmdParser.c.\n
 */
int setAxisOpMode(int axisIndex,
                  int value);

/** \brief Set the denominator part of the encoder scale.\n
 *
 * The encoder scale factor is divided into one numerator and one denominator
 * part. This function reads the denominator part.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Encoder scale denominator part.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder scale denominator for axes 8 to 4096.\n
 * "ADSPORT=501/.ADR.16#5008,16#24,8,5=4096" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisEncScaleDenom(int    axisIndex,
                         double value);

/** \brief Set the numerator part of the encoder scale.\n
 *
 * The encoder scale factor is divided into one numerator and one denominator
 * part. This function reads the numerator part.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Encoder scale numerator part.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder scale numerator for axes 3 to 360.\n
 * "ADSPORT=501/.ADR.16#5003,16#23,8,5=360" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisEncScaleNum(int    axisIndex,
                       double value);

/** \brief Set axis home reference position setpoint.\n
 *
 * The home reference position setpoint is only used during referencing/homing
 * sequence. The final referencing will be done to this position
 * (the reference switch edge will have this positon after referenceing).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Home position setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set home position setpoint for axis 3 to 111.\n
 * "Main.M3.fHomePosition=111"  //Command string to ecmcCmdParser.c.\n
 *
 * \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisHomePos(int    axisIndex,
                   double value);

/** \brief Set home index pulse count offset.\n
 *
 * Sets number of latches before homing is made.\n
 *
 *  \note Only valid for some homing sequences when\
 *  homing on hardware latched position (encoder index or external hw latch).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] count Number of latches before homing.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set home latch count to 1 for axis 10.\n
 * "Cfg.SetAxisHomeLatchCountOffset(10,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisHomeLatchCountOffset(int axisIndex,
                                int count);

/** \brief Set twords cam referencing/homing velocity setpoint.\n
 *
 * This velocity setpoint is only used during homing sequence when finding
 * the reference switch. See command "getAxisHomeVelTwordsCam()" for more
 * information.\n
 * Normally the off cam velocity setpoint is lower than the twords cam setpoint
 * since the accuracy of homing is depending on how accurate the signal edge
 * of the reference switch can be identified.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set twords cam referencing/homing velocity setpoint for axes
 * 3 to 10.\n
 * "ADSPORT=501/.ADR.16#4003,16#6,8,5=10" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisHomeVelTwordsCam(int    axisIndex,
                            double dVel);

/** \brief Set off cam referencing/homing velocity setpoint.\n
 *
 * This velocity setpoint is only used during homing sequence when the
 * reference switch already have been found. See command
 * "getAxisHomeVelTwordsCam()" for more information.\n
 * Normally the off cam velocity setpoint is lower than the twords cam setpoint
 * since the accuracy of homing is depending on how accurate the signal edge
 * of the reference switch can be identified.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set off cam referencing/homing velocity setpoint for axes
 * 3 to 7.5.\n
 * "ADSPORT=501/.ADR.16#4003,16#7,8,5=7.5" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int setAxisHomeVelOffCam(int    axisIndex,
                         double dVel);

/** \brief Set gear ration setting.\n
 *
 * Note: The gear ratio is only valid during synchronized motions.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] ratioNum  Gear ratio factor numerator.\n
 * \param[in] ratioDenom  Gear ratio factor denominator.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set gear ratio setting for axis 3 to 1/7.\n
 * "Cfg.SetAxisGearRatio(3,1,7)" //Command string to ecmcCmdParser.c.\n
 */
/*int setAxisGearRatio(int    axisIndex,
                     double ratioNum,
                     double ratioDenom);*/

/** \brief Set axis reset bit.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value  State of axis reset bit.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set reset bit for axis 3.\n
 * "Main.M3.bReset=1" //Command string to ecmcCmdParser.c.\n
 *
 *  \todo  "TwinCAT syntax. Needs to be changed.\n
 */
int axisErrorReset(int axisIndex,
                   int value);

/** \brief Enables/disables velocity filter of external setpoint.\n
 *
 *  This filter is needed in order to have a smoth feedforward value when\n
 *  reciving setpoints form an PLC. If filter is disabled the velocity will\n
 *  be calculated based on the last two postion values recived from PLC. In many\n
 *  cases this will result in a "unstable signal" depending onresoltions of the\n
 *  values involved in the calculation. Using a filter will smoth the velocity\n
 *  feed forward value and will thereby result in a smoother motion.\n
 *
 * \note: This filter is only enabled when the axis recives setpoin ts from an PLC.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] enable enable filter.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable velocity filter for external setpoint position for axis 5.
 * "Cfg.SetAxisPLCTrajVelFilterEnable(5,1) //Command string to ecmcCmdParser.c.\n
 */
int setAxisPLCTrajVelFilterEnable(int axisIndex,
                                  int enable);

/** \brief Set size of external trajectory velocity filter.\n
 *
 *  Sets the size of the filter for velocity from "external" PLC code.\n
 *  This filter is needed in order to have a smoth feedforward value when\n
 *  reciving setpoints form an PLC. If filter is disabled the velocity will\n
 *  be calculated based on the last two postion values recived from PLC. In many\n
 *  cases this will result in a "unstable signal" depending onresoltions of the\n
 *  values involved in the calculation. Using a filter will smoth the velocity\n
 *  feed forward value and will thereby rresult in a smoother motion.\n
 *
 * \note: This filter is only enabled when the axis recives setpoin ts from an PLC.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] size       Size of filter (default 100).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set filter size to 10 for for axis 7.\n
 * "Cfg.SetAxisPLCTrajVelFilterSize(7,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisPLCTrajVelFilterSize(int axisIndex,
                                int size);

/** \brief Enables/disables velocity filter of external actual value.\n
 *
 * NOTE: This filter is currentlly not used.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] enable enable filter.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable velocity filter for external actual position for axis 5.
 * "Cfg.SetAxisPLCEncVelFilterEnable(5,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisPLCEncVelFilterEnable(int axisIndex,
                                 int enable);

/** \brief Set size of external encoder velocity filter.\n
 *
 * NOTE: This filter is currentlly not used.\n
 *
 *  Sets the size of the filter for velocity from "external" PLC code.\n
 * \param[in] axisIndex  Axis index.\n
 * \param[in] size       Size of filter (default 100).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set filter size to 10 for for axis 7.\n
 * "Cfg.SetAxisPLCEncVelFilterSize(7,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisPLCEncVelFilterSize(int axisIndex,
                               int size);

/** \brief Set size of encoder velocity filter.\n
 *
 *  Sets the size of the low pass filter for velocity.\n
 *  Needed when resolution of encoder is low compared to\n
 *  sample rate and speed.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] size       Size of filter (default 100), needs to be >0.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set filter size to 10 for for axis 7.\n
 * "Cfg.SetAxisEncVelFilterSize(7,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncVelFilterSize(int axisIndex,
                            int size);

/** \brief Set axis trajectory data source.\n
 *
 * An axis trajectory generator can get position setpoints from different
 * sources:\n
 *   source = 0 : Internal (normal trajectory generation).\n
 *   source = 1 : External from transformation expression (synchronization).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Source type.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set trajectory generator data source type for axis 3 to
 * internal.\n
 * "Cfg.SetAxisTrajSourceType(3,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisTrajSource(int axisIndex,
                      int value);

/** \brief Set axis encoder data source.\n
 *
 * An axis encoder can get actual position from different sources:\n
 *   source = 0 : Internal (EtherCAT entry).\n
 *   source = 1 : External from transformation expression (synchronization).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Source type.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder data source type for axis 3 to
 * external.\n
 * "Cfg.SetAxisEncSourceType(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncSource(int axisIndex,
                     int value);

/** \brief Set axis trajectory start position. NOTE: NOT USED!!.\n
 *
 * \note This function is not used currently.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Start position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set trajectory start position to 100 for axis 3.\n
 * "Cfg.SetAxisTrajStartPos(3,100)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisTrajStartPos(int    axisIndex,
                        double value);

/** \brief Set encoder offset value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Offset position.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder offset value to 100 for axis 3.\n
 * "Cfg.SetAxisEncOffset(3,100)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncOffset(int    axisIndex,
                     double value);

/** \brief Set encoder type.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Encoder type.\n
 *  value = 0: Incremental encoder.\n
 *  value = 1: Absolute encoder.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder type to absolute for axis 3.\n
 * "Cfg.SetAxisEncType(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncType(int axisIndex,
                   int value);

/** \brief Set encoder register bit count.\n
 *
 * The bit count is used to handle over/under flow.\n
 *
 *\note: The bits will be considered to be the least significant\n
 * of the encoder data area. The raw encoder mask will therefore\n
 * be set to 2^bits-1 (see setAxisEncRawMask() for more info). If\n
 * bits need to be filtered away then use setAxisEncRawMask() command\n
 * instead.
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] bits Encoder register bit count.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder bit count to 16 for axis 3.\n
 * "Cfg.SetAxisEncBits(3,16)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncBits(int axisIndex,
                   int bits);

/** \brief Set encoder register bit count for absolute data.\n
 *
 * This setting is used for homing of partly absolute encoders\n
 * like resolvers (single turn absolute). The data is always\n
 * considered to be located at the least significant bits\n
 * of the encoder data (masked). See setAxisEncBits() and\n
 * setAxisEncRawMask() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] bits Encoder register bit count for absolute data.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder absolute bit count to 10 for axis 3.\n
 * "Cfg.SetAxisEncAbsBits(3,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncAbsBits(int axisIndex,
                      int bits);

/** \brief Set encoder raw data mask.\n
 *
 * Mask to filter Encoder data from encoder 64bit data\n
 * Also calculates bit-count of encoder data. If this function \n
 * is used then the setAxisEncBits() should NOT be used since\n.
 * setAxisEncBits() also calculates and sets the a raw-mask\n
 * (but based on all data located on the least significant bits).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] rawMask Encoder raw mask.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set encoder mask to 0xFFFF for axis 3.\n
 * "Cfg.SetAxisEncRawMask(3,0xFFFF)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisEncRawMask(int      axisIndex,
                      uint64_t rawMask);

/** \brief Set PID-controller proportional gain.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Proportional gain.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller proportional gain to 1.5 for axis 3.\n
 * "Cfg.SetAxisCntrlKp(3,1.5)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlKp(int    axisIndex,
                   double value);

/** \brief Set PID-controller integral gain.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Integral gain.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller integral gain to 0.1 for axis 3.\n
 * "Cfg.SetAxisCntrlKi(3,0.1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlKi(int    axisIndex,
                   double value);

/** \brief Set PID-controller differential gain.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Differential gain.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller differential gain to 1.1 for axis 3.\n
 * "Cfg.SetAxisCntrlKd(3,1.1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlKd(int    axisIndex,
                   double value);

/** \brief Set PID-controller feed forward gain.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Feed forward gain.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller feed forward gain to 4.1 for axis 3.\n
 * "Cfg.SetAxisCntrlKff(3,4.1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlKff(int    axisIndex,
                    double value);

/** \brief Set PID-controller maximum output value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Max value.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller max value to 1000 for axis 3.\n
 * "Cfg.SetAxisCntrlOutHL(3,1000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlOutHL(int    axisIndex,
                      double value);

/** \brief Set PID-controller minimum output value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Min value.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller min value to -1000 for axis 3.\n
 * "Cfg.SetAxisCntrlOutLL(3,-1000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlOutLL(int    axisIndex,
                      double value);

/** \brief Set PID-controller minimum integral part output value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Min integral value.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller minimum integral part output value to -700 for axis 3.\n
 * "Cfg.SetAxisCntrlIPartLL(3,-700)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlIpartLL(int    axisIndex,
                        double value);

/** \brief Set PID-controller maximum integral part output value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Max integral value.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PID-controller maximum integral part output value to 700 for axis 3.\n
 * "Cfg.SetAxisCntrlIPartHL(3,700)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisCntrlIpartHL(int    axisIndex,
                        double value);

/** \brief Get drive output scale numerator.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value Scale numerator.\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisDrvScaleNum(int     axisIndex,
                       double *value);

/** \brief Set drive output scale numerator.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Scale numerator.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set drive output scale numerator to 10000 for axis 3.\n
 * "Cfg.SetAxisDrvScaleNum(3,10000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvScaleNum(int    axisIndex,
                       double value);

/** \brief Set drive output scale denominator.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Scale denominator.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set drive output scale denominator to 32000 for axis 3.\n
 * "Cfg.SetAxisDrvScaleDenom(3,32000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvScaleDenom(int    axisIndex,
                         double value);

/** \brief Set drive amplifier enable. NOTE: Only used in manual mode!!!\n
 *
 * \note This function is only used in manual mode otherwise use
 * setAxisEnable() instead.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable amplifier.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable amplifier for axis 3.\n
 * "Cfg.SetAxisDrvEnable(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvEnable(int axisIndex,
                     int value);

/** \brief Set drive velocity setpoint. NOTE: Only used in manual mode!!!\n
 *
 * \note This function is only used in manual mode otherwise use.\n
 *
 * If in manual mode and amplifier enabled the velocity setpoint will be
 * rescaled and output to the drive.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Drive velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set drive velocity setpoint to 10 for axis 3.\n
 * "Cfg.SetAxisDrvVelSet(3,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvVelSet(int    axisIndex,
                     double value);

/** \brief Set drive raw velocity setpoint. NOTE: Only used in manual mode!!!\n
 *
 * \note This function is only used in manual mode otherwise use.\n
 *
 * If in manual mode and amplifier enabled the raw velocity setpoint will be
 * directly output to the drive (without being rescaled).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Raw drive velocity setpoint.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set raw drive velocity setpoint to 1000 for axis 3.\n
 * "Cfg.SetAxisDrvVelSetRaw(3,1000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvVelSetRaw(int axisIndex,
                        int value);

/** \brief Set enable of brake.\n
 *
 *  The brake output will follow the amplifier enable state of the drive. The
 *  brake is high when amplifier enable is high and low when amplifier enable
 *  is low.\n
 *
 *  Note: Delays are can be configured with:\n
 *        -setAxisDrvBrakeOpenDelayTime() \n
 *        -setAxisDrvBrakeCloseAheadTime() \n
 *
 *  Note: A valid EtherCAT entry must be linked to the drive objects brake
 *  entry in order to work like intended.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Brake enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable brake for axis 3.\n
 * "Cfg.SetAxisDrvBrakeEnable(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvBrakeEnable(int axisIndex,
                          int enable);

/** \brief Set brake open delay time .\n
 *
 *  The release of the brake will be delayed for an additional
 *  delay time when enabling the amplifier.
 *
 *  Note: A valid EtherCAT entry must be linked to the drive objects brake
 *  entry in order to work like intended.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] delayTime Delay time in cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set brake open delay time for axis 3 to 100 cycles.\n
 * "Cfg.SetAxisDrvBrakeOpenDelayTime(3,100)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvBrakeOpenDelayTime(int axisIndex,
                                 int delayTime);

/** \brief Set brake close ahead time .\n
 *
 *  Activation of the brake will be made prior to disabling the amplifier.\n
 *
 *  Note: A valid EtherCAT entry must be linked to the drive objects brake
 *  entry in order to work like intended.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] aheadTime Ahead time in cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set brake close ahead time for axis 3 to 100 cycles.\n
 * "Cfg.SetAxisDrvBrakeCloseAheadTime(3,100)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvBrakeCloseAheadTime(int axisIndex,
                                  int aheadTime);

/** \brief Set enable of reduce torque functionality.\n
 *
 *  The reduce torque output will go high when the axis is atTarget. The
 *  "atTarget" monitoring functionality needs to be configured in order to
 *  reduce torque at the correct time.\n
 *
 *  Note: A valid EtherCAT entry must be linked to the drive objects reduce torque
 *  entry in order to work like intended.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Reduce torque enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable reduce torque for axis 3.\n
 * "Cfg.SetAxisDrvReduceTorqueEnable(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisDrvReduceTorqueEnable(int axisIndex,
                                 int enable);

/** \brief Set drive type.\n
 *  OBSOLETE COMMAND. USE CREATEAXIS(id,type,drvtype).
 *
 * \note  ALL SETTINGS MADE TO THE DRIVE WILL BE OVERWRITTEN.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] type Drive type.\n
 *                 0 = Stepper drive.\n
 *                 1 = DS402 drive.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set axis 3 drive type to stepper.\n
 * "Cfg.SetAxisDrvType(3,0)" //Command string to ecmcCmdParser.c.\n
 */
/*int setAxisDrvType(int axisIndex,
                   int type);*/

/** \brief Get "at target" monitoring tolerance.\n
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value At target tolerance.\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonAtTargetTol(int     axisIndex,
                          double *value);

/** \brief Set "at target" monitoring tolerance.\n
 *
 *  The motion will be considered to have reached the target position
 *  when the difference between the actual position and target position
 *  is within this tolerance for a certain number of cycles, see command
 *  setAxisMonAtTargetTime() for more information.\n
 *
 *  Note: The "at target" functionality needs to be enabled by the command
 *  setAxisMonEnableAtTargetMon().\n
 *
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value At target tolerance.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set at target tolerance to 0.1 for axis 7.\n
 * "Cfg.SetAxisMonAtTargetTol(7,0.1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonAtTargetTol(int    axisIndex,
                          double value);

/** \brief Get "at target" monitoring time (cycles).\n
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value At target time (cycles) .\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonAtTargetTime(int  axisIndex,
                           int *value);

/** \brief Set "at target" monitoring time (cycles).\n
 *
 *  The motion will be considered to have reached the target position
 *  when the difference between the actual position and target position
 *  is within a certain tolerance for a this number of cycles, see command
 *  setAxisMonAtTargetTol() for more information.\n
 *
 *  Note: The "at target" functionality needs to be enabled by the command
 *  setAxisMonEnableAtTargetMon().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value At target time (cycles) .\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set at target tolerance time (cycles) to 10 for axis 7.\n
 * "Cfg.SetAxisMonAtTargetTime(7,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonAtTargetTime(int axisIndex,
                           int value);

/** \brief Get enable "at target" monitoring.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value Enable monitoring .\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonEnableAtTargetMon(int  axisIndex,
                                int *value);

/** \brief Enable "at target" monitoring.\n
 *
 *  The motion will be considered to have reached the target position
 *  when the difference between the actual position and target position
 *  is within a certain tolerance for a this number of cycles, see command
 *  setAxisMonAtTargetTol() and setAxisMonAtTargetTime() for more information.
 *  \n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable monitoring .\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable at target monitoring for axis 7.\n
 * "Cfg.SetAxisMonEnableAtTargetMon(7,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonEnableAtTargetMon(int axisIndex,
                                int value);

/** \brief Get position lag maximum monitoring tolerance.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value Position lag maximum tolerance.\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonPosLagTol(int     axisIndex,
                        double *value);

/** \brief Set position lag maximum monitoring tolerance.\n
 *
 *  The position lag monitoring functionality monitors the difference between
 *  the actual position and trajectory generated setpoint position. The motion
 *  can be interlocked if the difference exceeds this tolerance for a certain
 *  number of cycles, see command setAxisMonPosLagTime() for more information.
 *  \n
 *  A big position lag error during a motion could be generated by bad tuning or
 *  mechanical issues.\n
 *
 *  Note: The position lag functionality needs to be enabled with the command
 *  setAxisMonEnableLagMon().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Position lag maximum tolerance.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set maximum position lag tolerance to 0.2 for axis 7.\n
 * "Cfg.SetAxisMonPosLagTol(7,0.2)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonPosLagTol(int    axisIndex,
                        double value);

/** \brief Get position lag monitoring time (cycles).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pointer to return value Position lag time (cycles).\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonPosLagTime(int  axisIndex,
                         int *value);

/** \brief Set position lag monitoring time (cycles).\n
 *
  *  The position lag monitoring functionality monitors the difference between
 *  the actual position and trajectory generated setpoint position. The motion
 *  can be interlocked if the difference exceeds a certain tolerance for a this
 *  number of cycles, see command setAxisMonPosLagTol() for more information.
 *  \n
 *  A big position lag error during a motion could be generated by bad tuning or
 *  mechanical issues.\n
 *
 *  Note: The position lag functionality needs to be enabled with the command
 *  setAxisMonEnableLagMon().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Position lag time (cycles).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set position lag time (cycles) to 10 for axis 7.\n
 * "Cfg.SetAxisMonPosLagTime(7,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonPosLagTime(int axisIndex,
                         int value);

/** \brief Get position lag monitoring enable.\n
 *
  *  The position lag monitoring functionality monitors the difference between
 *  the actual position and trajectory generated setpoint position. The motion
 *  can be interlocked if the difference exceeds a certain tolerance for a certain
 *  number of cycles, see command setAxisMonPosLagTime() and setAxisMonPosLagTol()
 *  for more information.\n
 *  A big position lag error during a motion could be generated by bad tuning or
 *  mechanical issues.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable monitoring.\n.\n
 *
 * \return 0 if success or otherwise an error code.\n
 */
int getAxisMonEnableLagMon(int  axisIndex,
                           int *value);

/** \brief Enable position lag monitoring.\n
*
*  The position lag monitoring functionality monitors the difference between
*  the actual position and trajectory generated setpoint position. The motion
*  can be interlocked if the difference exceeds a certain tolerance for a certain
*  number of cycles, see command setAxisMonPosLagTime() and setAxisMonPosLagTol()
*  for more information.\n
*  A big position lag error during a motion could be generated by bad tuning or
*  mechanical issues.\n
*
* \param[in] axisIndex  Axis index.\n
* \param[in] value monitoring enabled.\n.\n
*
* \return 0 if success or otherwise an error code.\n
*
* \note Example: Disable position lag monitoring for axis 2.\n
* "Cfg.SetAxisMonEnableLagMon(2,0)" //Command string to ecmcCmdParser.c.\n
*/
int setAxisMonEnableLagMon(int axisIndex,
                           int value);

/** \brief Get maximum allowed velocity.\n
 *
 * The motion will be interlocked if the actual velocity or the velocity
 * setpoint exceeds this value.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Maximum velocity.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 */
int getAxisMonMaxVel(int     axisIndex,
                     double *value);

/** \brief Set maximum allowed velocity.\n
 *
 * The motion will be interlocked if the actual velocity or the velocity
 * setpoint exceeds this value.\n
 *
 *  Note: The maximum velocity monitoring needs to be enabled with the command
 *  setAxisMonEnableMaxVel().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Maximum velocity.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set maximum velocoity for axis 3 to 20.\n
 * "Cfg.SetAxisMonMaxVel(3,20)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonMaxVel(int    axisIndex,
                     double value);

/** \brief Enable maximum velocity monitoring (over speed).\n
 *
 * The motion will be interlocked if the actual velocity or the velocity
 * setpoint exceeds the limit value set by the command setAxisMonMaxVel().\n
 *
  * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable monitoring.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable over speed monitoring for axis 3.\n
 * "Cfg.SetAxisMonEnableMaxVel(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonEnableMaxVel(int axisIndex,
                           int value);

/** \brief Get enable maximum velocity monitoring (over speed).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Enable monitoring.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 */
int getAxisMonEnableMaxVel(int  axisIndex,
                           int *value);

/** \brief Set velocity monitoring interlock delay for drive.\n
 *
 * The over speed interlock for the drive can be delayed in
 * order ensure that encoder noise reulsting in a high velocity will not
 * interlock motion. If the overspeed interlock is still active after this
 * amount of cycles the drive will be interlocked and the amplifier will
 * be turned off.\n
 *
 *  Note: The maximum velocity monitoring needs to be enabled with the command
 *  setAxisMonEnableMaxVel().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Delay cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set drive over speed interlock delay to 10 cycles for axis 4.\n
 * "Cfg.SetAxisMonMaxVelDriveILDelay(4,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonMaxVelDriveILDelay(int axisIndex,
                                 int value);

/** \brief Set velocity monitoring interlock delay for trajectory.\n
 *
 * The over speed interlock for the trajectory generator can be delayed in
 * order ensure that encoder noise reulsting in a high velocity will not
 * interlock motion. If the overspeed interlock is still active after this
 * amount of cycles the trajectory generator will be interlocked  and velocity
 * will be ramped down to stand still.\n
 *
 *  Note: The maximum velocity monitoring needs to be enabled with the command
 *  setAxisMonEnableMaxVel().\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Delay cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set trajectory generator over speed interlock delay to 10 cycles
 * for axis 4.\n
 * "Cfg.SetAxisMonMaxVelTrajILDelay(4,10)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonMaxVelTrajILDelay(int axisIndex,
                                int value);

/** \brief Set sequence timeout time in seconds.\n
 *
 * The motion sequences (mainly related to homing) can be aborted if not
 * finalized within a certain time. This is to prevent that the motion
 * control hangs in one sequence. This could happen if the reference switch
 * for some reason is not encountered.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Timeout time.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set sequence timeout value for axis 2 to 1 minute.\n
 * "Cfg.SetAxisSeqTimeout(2,60)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisSeqTimeout(int axisIndex,
                      int value);

/** \brief Enable controller output high limit monitoring.\n
 *
 *  The controller output high limit monitoring functionality monitors
 *  the controller output. The motion can be interlocked if the controller
 *  output exceeds a certain tolerance, see command setAxisMonCntrlOutHL()
 *  for more information.\n
 *  A high controller output during a motion could be generated by bad tuning or
 *  mechanical issues.\n
 *
 *  \note WARNING: If an error is detected, the enable (power) will be removed
 *  from the axis. Please always ensure that the power less state is the
 *  safe state.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable monitoring.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Disable controller output high limit monitoring for axis 2.\n
 * "Cfg.SetAxisMonEnableCntrlOutHLMon(2,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonEnableCntrlOutHLMon(int axisIndex,
                                  int value);

/** \brief Set monitoring controller output high limit.\n
 *
 * Set maximum allowed controller output for an axis.\n
 *  A high controller output during a motion could be generated by bad tuning
 *  or mechanical issues, see command setAxisMonEnableCntrlOutHLMon()for more
 *  information.\n
 *
 *  \note WARNING: If an error is detected, the enable (power) will be removed
 *  from the axis. Please always ensure that the power less state is the
 *  safe state.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Maximum allowed controller output.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set maximum allowed output to 2000 for for axis 3.\n
 * "Cfg.SetAxisMonCntrlOutHL(3,2000)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonCntrlOutHL(int    axisIndex,
                         double value);

/** \brief Enable monitoring of velocity difference.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable monitoring.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable monitoring for axis 2.\n
 * "Cfg.SetAxisMonEnableVelocityDiff(2,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonEnableVelocityDiff(int axisIndex,
                                 int value);

/** \brief Set trajectory interlock filter time in cycles for velocity
 * difference monitoring.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Time in cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set filter time to 100 cycles for axis 2.\n
 * "Cfg.SetAxisMonVelDiffTrajILDelay(2,100)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonVelDiffTrajILDelay(int axisIndex,
                                 int value);

/** \brief Set drive interlock filter time in cycles for velocity
 * difference monitoring.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Time in cycles.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set filter time to 500 cycles for axis 2.\n
 * "Cfg.SetAxisMonVelDiffDriveILDelay(2,500)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonVelDiffDriveILDelay(int axisIndex,
                                  int value);

/** \brief Set maximum allowed difference between setpoint speed and
 * actual speed±n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value velocity difference.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set maximum difference 0.5 for axis 2.\n
 * "Cfg.SetAxisMonVelDiffTol(2,0.5)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonVelDiffTol(int    axisIndex,
                         double value);

/** \brief Enable motion axis interlock from EtherCAT entry.\n
 *
 *Enable
 * The motion can be interlocked based on an EtherCAT entry. See command
 * linkEcEntryToAxisMon() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable external interlock.\n
 *
 * \note WARNING: If an error is detected, the enable (power) will be removed
 *  from the axis. Please always ensure that the power less state is the
 *  safe state.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable external interlock for axis 7.\n
 * "Cfg.SetAxisMonEnableExtHWInterlock(7,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonEnableExternalInterlock(int axisIndex,
                                      int value);

/** \brief Set polarity of motion axis interlock from EtherCAT entry.\n
 *
 *
 * The motion can be interlocked based on an EtherCAT entry. See command
 * linkEcEntryToAxisMon() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Polarity external interlock.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \note WARNING: If an error is detected, the enable (power) will be removed
 *  from the axis. Please always ensure that the power less state is the
 *  safe state.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set external interlock to NC for axis 7.\n
 * "Cfg.SetAxisMonExtHWInterlockPolarity(7,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonExtHWInterlockPolarity(int axisIndex,
                                     int value);

/** \brief Get polarity of motion axis interlock from EtherCAT entry.\n
 *
 *
 * The motion can be interlocked based on an EtherCAT entry. See command
 * linkEcEntryToAxisMon() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value Polarity external interlock.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \note WARNING: If an error is detected, the enable (power) will be removed
 *  from the axis. Please always ensure that the power less state is the
 *  safe state.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get external interlock polarity for axis 7.\n
 * "GetAxisMonExtHWInterlockPolarity(7)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisMonExtHWInterlockPolarity(int axisIndex, int *pol);

/** \brief Set polarity of hard low limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Polarity.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set low limit polarity to NC for axis 7.\n
 * "Cfg.SetAxisMonLimitBwdPolarity(7,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonLimitBwdPolarity(int axisIndex, int value);

/** \brief Get polarity of hard low limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pol Polarity.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get low limit polarity for axis 7.\n
 * "GetAxisMonLimitBwdPolarity(7)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisMonLimitBwdPolarity(int axisIndex, int *pol);

/** \brief Set polarity of hard high limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Polarity.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set high limit polarity to NC for axis 7.\n
 * "Cfg.SetAxisMonLimitFwdPolarity(7,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonLimitFwdPolarity(int axisIndex, int value);

/** \brief Get polarity of hard high limit switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pol Polarity.\n
 *                0 = NC (High is OK) Default.\n
 *                1 = NO (Low is OK).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get high limit polarity for axis 7.\n
 * "GetAxisMonLimitFwdPolarity(7)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisMonLimitFwdPolarity(int axisIndex, int *pol);

/** \brief Set polarity of home switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Polarity.\n
 *                0 = NC (High is 1) Default.\n
 *                1 = NO (Low is 1).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set home switch polarity to NC for axis 7.\n
 * "Cfg.SetAxisMonHomeSwitchPolarity(7,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisMonHomeSwitchPolarity(int axisIndex, int value);

/** \brief Get polarity of home switch.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] pol Polarity.\n
 *                0 = NC (High is 1) Default.\n
 *                1 = NO (Low is 1).\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get home switch polarity for axis 7.\n
 * "GetAxisMonHomeSwitchPolarity(7)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisMonHomeSwitchPolarity(int axisIndex, int *pol);

/** \brief Allow commands from PLCs.\n
 *
 * An axis can receive commands from PLCs (see PLC syntax).
 * However, the axis must be allow to receive these commands, see
 * command setAxisPLCExpr() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable command from PLC for axis 3.\n
 * "Cfg.setAxisAllowCommandsFromPLC(3,1)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisAllowCommandsFromPLC(int axisIndex,
                                       int value);

/** \brief Enable axis sync PLC expression.\n
 *
 * The axis sync PLC expression for an axis can be enabled/disabled.
 * see command setAxisPLCExpr() for more information.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value Enable.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Disable command transformation expression axis 5.\n
 * "Cfg.SetAxisPLCEnable(5,0)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisPLCEnable(int axisIndex,
                     int value);

/** \brief Append axis sync. PLC expression.\n
 *
 * Add one line to the axis PLC code.\n
 * The axis PLC expression is used for enabling and executing of
 * axes based on mathematical expressions. This is useful when synchronizing
 * axes i.e. a slave axis could be enabled and executed at the same time as
 * the master axis.\n
 *
 * Example: Enable of axis 2 is related to the enable command of axis 1 and 5.
 * The execute command is related to an expression including the
 * execute command for axis 2 and 7.\n
 * "ax2.drv.enable:=ax1.drv.enable or ax5.drv.enable|".\n
 *
 * For more syntax help plese view PLC syntax (setPLCExpr()).\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] expr PLC expression.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set PLC expression for axes 5 to
 * ax2.drv.enable:=ax1.drv.enable or ax5.drv.enable|.\n
 * "Cfg.setAxisPLCExpr(5)=ax2.drv.enable:=ax1.drv.enable or ax5.drv.enable|"
 * //Command string to ecmcCmdParser.c.\n
 */
int appendAxisPLCExpr(int   axisIndex,
                      char *expr);

/** \brief Compile Axis PLC code\n
 *
 * For more syntax help plese view PLC syntax (setPLCExpr()).\n
 *
 * \param[in] axisIndex  Axis index.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Compile code for the PLC of axis 5.\n
 * "Cfg.CompileAxisPLC(5)" //Command string to ecmcCmdParser.c.\n
 */
int compileAxisPLCExpr(int   axisIndex);

/** \brief Creates an axis object at index axisIndex.
 *
 * \param[in] axisIndex Index of axis to address.\n
 * \param[in] axisType Type of axis.\n
 *   type = 1 : Normal axis (drive, encoder, monitor,pid-controller,trajectory).\n
 *   type = 2 : Virtual axis (encoder, monitor, trajectory).\n
 * \param[in] drvType Type of axis.\n
 *   type = 0 : Simple drive (stepper).\n
 *   type = 1 : DS402 drive.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Create a normal axis with stepper drive at axisIndex 1.\n
 *  "Cfg.CreateAxis(1)" //Command string to ecmcCmdParser.c\n
 *
 * \note Example: Create a virtual axis at axisIndex 1 (no drive).\n
 *  "Cfg.CreateAxis(1,2)" //Command string to ecmcCmdParser.c\n
 *
 * \note Example: Create a normal axis with DS402 drive at axisIndex 1.\n
 *  "Cfg.CreateAxis(1,2,1)" //Command string to ecmcCmdParser.c\n
 */
int createAxis(int axisIndex,
               int axisType,
               int drvType);

/** \brief Links an EtherCAT entry to the encoder object of the axis at axisIndex.
   *
   *
   *  \param[in] slaveBusPosition Position of the EtherCAT slave on the bus.\n
   *    slaveBusPosition = -1: Used to address the simulation slave. Only two
   *                         entries are configured, "ZERO" with default
   *                         value 0 and "ONE" with default value 1.\n
   *    slaveBusPosition = 0..65535: Addressing of normal EtherCAT slaves\n
   *  \param[in] entryIdString String for addressing purpose (see command
   *                      "Cfg.EcAddEntryComplete() for more information").\n
   *  \param[in] axisIndex Index of axis to link to.\n
   *  \param[in] encoderEntryIndex Index of encoder objects entry list.\n
   *    encoderEntryIndex = 0: Actual position (input).\n
   *  \param[in] entryBitIndex Bit index of EtherCAT entry to use.\n
   *    entryBitIndex = -1: All bits of the entry will be used.\n
   *    entryBitIndex = 0..64: Only the selected bit will be used.\n
   *
   *  \return 0 if success or otherwise an error code.\n
   *
   *  \note Example: Link an EtherCAT entry configured as "POSITION_ACT" in slave 1
   *  as actual position for the encoder of axis 5.\n
   *  "Cfg.LinkEcEntryToAxisEncoder(1,POSITION_ACT,5,0,-1)" //Command string
   *  to ecmcCmdParser.c\n
   */
int linkEcEntryToAxisEnc(int   slaveBusPosition,
                         char *entryIdString,
                         int   axisIndex,
                         int   encoderEntryIndex,
                         int   entryBitIndex);

/** \brief Links an EtherCAT entry to the drive object of the axis at axisIndex.
   *
   *
   *  \param[in] slaveBusPosition Position of the EtherCAT slave on the bus.\n
   *    slaveBusPosition = -1: Used to address the simulation slave. Only two
   *                         entries are configured, "ZERO" with default
   *                         value 0 and "ONE" with default value 1.\n
   *    slaveBusPosition = 0..65535: Addressing of normal EtherCAT slaves\n
   *  \param[in] entryIdString String for addressing purpose (see command
   *                      "Cfg.EcAddEntryComplete() for more information").
   *                      If left blank driveEntryIndex == 3 or4, the corresponding
   *                      functionality will be disabled.\n
   *  \param[in] axisIndex Index of axis to link to.\n
   *  \param[in] driveEntryIndex Index of drive objects entry list.\n
   *    driveEntryIndex = 0: Amplifier enable (output).\n
   *    driveEntryIndex = 1: Velocity setpoint (output).\n
   *    driveEntryIndex = 2: Amplifier enabled (input).\n
   *    driveEntryIndex = 3: Brake (output).\n
    *                        Brake func will be enabled.\n
   *    driveEntryIndex = 4: Reduce torque (output).
   *                         Reduce torque func will be enabled.\n
   *  \param[in] entryBitIndex Bit index of EtherCAT entry to use.\n
   *    entryBitIndex = -1: All bits of the entry will be used.\n
   *    entryBitIndex = 0..64: Only the selected bit will be used.\n
   *
   * \return 0 if success or otherwise an error code.\n
   *
   *  \note Example 1: Link an EtherCAT entry configured as "VELOCITY_SET" in slave 3
   *  as velocity setpoint entry for the drive object of axis 5.\n
   *   "Cfg.LinkEcEntryToAxisDrive(3,VELOCITY_SET,5,1,-1)" //Command string to ecmcCmdParser.c\n
   *
   *  Example 2: Link bit 0 of an EtherCAT entry configured as "STM_CONTROL" in
   *  slave 3 as enable amplifier output entry for the drive object of axis 5.\n
   *   "Cfg.LinkEcEntryToAxisDrive(3,STM_CONTROL,5,0,0)" //Command string to ecmcCmdParser.c\n
   *
   *  Example 3: If a drive have no feedback for enable, a simulation slave and
   *  entry can be used. The simulation EtherCAT slave can be addressed with a
   *  slaveBusIndex of -1.
   *  The simulation slave contains two entries, "ZERO" with default value zero
   *  and "ONE" with default value set to 1.\n
   *   "Cfg.LinkEcEntryToAxisDrive(-1,ONE,5,0,0)" //Command string to ecmcCmdParser.c\n
   */
int linkEcEntryToAxisDrv(int   slaveBusPosition,
                         char *entryIdString,
                         int   axisIndex,
                         int   driveEntryIndex,
                         int   entryBitIndex);

/** \brief Links an EtherCAT entry to the monitor object of the axis at axisIndex\n
 *
 *  \param[in] slaveBusPosition Position of the EtherCAT slave on the bus.\n
 *    slaveBusPosition = -1: Used to address the simulation slave. Only two
 *                           entries are configured, "ZERO" with default
 *                           value 0 and "ONE" with default value 1.\n
 *    slaveBusPosition = 0..65535: Addressing of normal EtherCAT slaves.\n
 *  \param[in] entryIdString String for addressing purpose (see command
 *                      "Cfg.EcAddEntryComplete() for more information").\n
 *  \param[in] axisIndex Index of axis to link to.\n
 *  \param[in] monitorEntryIndex Index of monitor objects entry list.\n
 *    monitorEntryIndex = 0: Limit switch backward direction.\n
 *    monitorEntryIndex = 1: Limit switch forward direction.\n
 *    monitorEntryIndex = 2: Reference switch (homing).\n
 *    monitorEntryIndex = 3: External interlock input (optional).\n
 *  \param[in] entryBitIndex Bit index of EtherCAT entry to use.\n
 *    entryBitIndex = -1: All bits of the entry will be used.\n
 *    entryBitIndex = 0..64: Only the selected bit will be used.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 *  \note Example 1: Link an EtherCAT entry configured as "INPUT_0" in slave 1 as
 *  forward limit switch entry for the monitor object of axis 5.\n
 *   "Cfg.LinkEcEntryToAxisMonitor(1,"INPUT_0",5,1,0)" //Command string to ecmcCmdParser.c\n
 *
 *  Example 2: If an axis is not equipped with limit switches the entries for
 *  limit switches needs to be linked to the simulation entries. The
 *  simulation slave contains two entries, "ZERO" with default value
 *  zero and "ONE" with default value set to 1.\n
 *   "Cfg.LinkEcEntryToAxisMonitor(-1,ONE,5,1,0)" //Command string to ecmcCmdParser.c\n
 */
int linkEcEntryToAxisMon(int   slaveBusPosition,
                         char *entryIdString,
                         int   axisIndex,
                         int   monitorEntryIndex,
                         int   entryBitIndex);

/** \brief Links an EtherCAT entry to the an axis object for
 *   status output\n
 *
 *  The output will be high when the axis object is without error code and
 *  otherwise zero.
 *
 *  \param[in] slaveIndex Position of the EtherCAT slave on the bus.\n
 *    slaveIndex = -1: Used to address the simulation slave. Only two
 *                           entries are configured, "ZERO" with default
 *                           value 0 and "ONE" with default value 1.\n
 *    slaveIndex = 0..65535: Addressing of normal EtherCAT slaves.\n
 *  \param[in] entryIdString String for addressing purpose (see command
 *                      "Cfg.EcAddEntryComplete() for more information").\n
 *  \param[in] axisIndex Index of axis.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 *  \note Example 1: Link an EtherCAT entry configured as "OUTPUT_0" in slave 1 as
 *  status output for axis with index 2.\n
 *   "Cfg.LinkEcEntryToAxisStatusOutput(1,"OUTPUT_0",2)" //Command string to ecmcCmdParser.c\n
 */

int linkEcEntryToAxisStatusOutput(int   slaveIndex,
                                  char *entryIDString,
                                  int   axisIndex);



/** \brief Set axis index for detailed motion diagnostics.\n
 *
 * \param[in] axisIndex Index of axis.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Choose detailed motion diagnostics for axis 3.\n
 *  "Cfg.SetDiagAxisIndex(3)" //Command string to ecmcCmdParser.c\n
 */
int setDiagAxisIndex(int axisIndex);

/** \brief Set axis frequency of detailed motion diagnostics printouts.\n
 *
 * \param[in] freq Printout frequency.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set printout frequency to 10.\n
 *  "Cfg.SetDiagAxisFreq(10)" //Command string to ecmcCmdParser.c\n
 */
int setDiagAxisFreq(int freq);

/** \brief Enable detailed motion diagnostics printouts.\n
 *
 * The detailed motion diagnostics prints valuable motion related information
 * for one axis.\n
 * See setDiagAxisIndex() and setDiagAxisFreq() for more information.\n
 *
 * \param[in] enable Enable printouts.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Enable printout of detailed motion diagnostics.\n
 *  "Cfg.SetDiagAxisEnable(1)" //Command string to ecmcCmdParser.c\n
 */
int setDiagAxisEnable(int enable);

/** \brief Get axis modulo range.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] value  modulo range.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get modulo range for axis 3.\n
 * "GetAxisModRange(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisModRange(int  axisIndex,
                    double *range);

/** \brief Set axis modulo range.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] value  modulo range.\n
 *
 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set modulo range for axis 3 to 360.\n
 * "Cfg.SetAxisModRange(3,360)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisModRange(int  axisIndex,
                    double range);

/** \brief Set axis modulo motion type.\n
 *
 * Used for positioning if modulo range is set (setAxisModRange())\n
 * to a value greater than 0.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[in] type Modulo type.\n
 *    type = 0 : ECMC_MOD_MOTION_NORMAL\n
             1:  ECMC_MOD_MOTION_FWD (Always forward)\n
             2:  ECMC_MOD_MOTION_BWD (always backward)\n
             3:  ECMC_MOD_MOTION_CLOSEST (closest path)\n

 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Set modulo type to "ECMC_MOD_MOTION_BWD"\n
 * for axis 3.\n
 * "Cfg.SetAxisModType(3,2)" //Command string to ecmcCmdParser.c.\n
 */
int setAxisModType(int axisIndex,
                   int type);

/** \brief Set axis modulo motion type.\n
 *
 * \param[in] axisIndex  Axis index.\n
 * \param[out] type Modulo type.\n
 *    type = 0 : ECMC_MOD_MOTION_NORMAL\n
             1:  ECMC_MOD_MOTION_FWD (Always forward)\n
             2:  ECMC_MOD_MOTION_BWD (always backward)\n
             3:  ECMC_MOD_MOTION_CLOSEST (closest path)\n

 * \return 0 if success or otherwise an error code.\n
 *
 * \note Example: Get modulo type for axis 3.\n
 * "GetAxisModType(3)" //Command string to ecmcCmdParser.c.\n
 */
int getAxisModType(int  axisIndex,
                   int *value);

# ifdef __cplusplus
}
# endif  // ifdef __cplusplus

#endif  /* ECMC_MOTION_H_ */
