
#define __STDC_FORMAT_MACROS  // for printf uint_64_t
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string>

#include "ecmcMotion.h"

#include "../ethercat/ecmcEthercat.h"
#include "../plc/ecmcPLC.h"
#include "ecmcAxisBase.h"      // Abstract class for all axis types
#include "ecmcAxisReal.h"      // Normal axis (cntr,drv, enc, traj, mon, seq)
#include "ecmcAxisVirt.h"      // Axis without drive and controller
#include "ecmcDriveBase.h"     // Abstract drive base class
#include "ecmcTrajectoryTrapetz.h"
#include "ecmcPIDController.h"
#include "ecmcEncoder.h"
#include "ecmcMonitor.h"
#include "ecmcEc.h"
#include "ecmcEcSlave.h"
#include "ecmcEcEntry.h"

// TODO: REMOVE GLOBALS
#include "../main/ecmcGlobalsExtern.h"


int moveAbsolutePosition(int    axisIndex,
                         double positionSet,
                         double velocitySet,
                         double accelerationSet,
                         double decelerationSet) {
  LOGINFO4(
    "%s/%s:%d axisIndex=%d, positionSet=%lf, velocitySet=%lf, accelerationSet=%lf, decelerationSet=%lf\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    axisIndex,
    positionSet,
    velocitySet,
    accelerationSet,
    decelerationSet);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex);
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex);

  int errorCode = axes[axisIndex]->getErrorID();

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setExecute(0);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCommand(ECMC_CMD_MOVEABS);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCmdData(0);

  if (errorCode) {
    return errorCode;
  }
  axes[axisIndex]->getSeq()->setTargetPos(positionSet);
  axes[axisIndex]->getSeq()->setTargetVel(velocitySet);
  axes[axisIndex]->getTraj()->setAcc(accelerationSet);
  axes[axisIndex]->getTraj()->setDec(decelerationSet);
  errorCode = axes[axisIndex]->setExecute(1);

  if (errorCode) {
    return errorCode;
  }
  return 0;
}

int moveRelativePosition(int    axisIndex,
                         double positionSet,
                         double velocitySet,
                         double accelerationSet,
                         double decelerationSet) {
  LOGINFO4(
    "%s/%s:%d axisIndex=%d, positionSet=%lf, velocitySet=%lf, accelerationSet=%lf, decelerationSet=%lf\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    axisIndex,
    positionSet,
    velocitySet,
    accelerationSet,
    decelerationSet);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex);
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex);

  int errorCode = axes[axisIndex]->getErrorID();

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setExecute(0);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCommand(ECMC_CMD_MOVEREL);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCmdData(0);

  if (errorCode) {
    return errorCode;
  }
  axes[axisIndex]->getSeq()->setTargetPos(positionSet);
  axes[axisIndex]->getSeq()->setTargetVel(velocitySet);
  axes[axisIndex]->getTraj()->setAcc(accelerationSet);
  axes[axisIndex]->getTraj()->setDec(decelerationSet);
  errorCode = axes[axisIndex]->setExecute(1);

  if (errorCode) {
    return errorCode;
  }
  return 0;
}

int moveVelocity(int    axisIndex,
                 double velocitySet,
                 double accelerationSet,
                 double decelerationSet) {
  LOGINFO4(
    "%s/%s:%d axisIndex=%d, velocitySet=%lf, accelerationSet=%lf, decelerationSet=%lf\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    axisIndex,
    velocitySet,
    accelerationSet,
    decelerationSet);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex);
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex);

  int errorCode = axes[axisIndex]->getErrorID();

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setExecute(0);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCommand(ECMC_CMD_MOVEVEL);

  if (errorCode) {
    return errorCode;
  }
  errorCode = axes[axisIndex]->setCmdData(0);

  if (errorCode) {
    return errorCode;
  }
  axes[axisIndex]->getSeq()->setTargetVel(velocitySet);
  axes[axisIndex]->getTraj()->setAcc(accelerationSet);
  axes[axisIndex]->getTraj()->setAcc(decelerationSet);
  errorCode = axes[axisIndex]->setExecute(1);

  if (errorCode) {
    return errorCode;
  }
  return 0;
}

int stopMotion(int axisIndex, int killAmplifier) {
  LOGINFO4("%s/%s:%d axisIndex=%d, killAmplifier=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           killAmplifier);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  if (killAmplifier) {
    int errorCode = axes[axisIndex]->setEnable(0);

    if (errorCode) {
      return errorCode;
    }
  }

  int errorCode = axes[axisIndex]->setExecute(0);

  if (errorCode) {
    return errorCode;
  }

  return 0;
}

int getAxisError(int axisIndex) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->getError();
}

int getAxisErrorID(int axisIndex) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getErrorID();
}

int getAxisCycleCounter(int axisIndex, int *counter) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);
  *counter = axes[axisIndex]->getCycleCounter();
  return 0;
}

int setAxisExecute(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  // Axis needs to be reset before new command is executed
  // (however allow execute=0)
  if (value && axes[axisIndex]->getError()) {
    return ERROR_MAIN_AXIS_ERROR_EXECUTE_INTERLOCKED;
  }

  return axes[axisIndex]->setExecute(value);
}

int setAxisCommand(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setCommand((motionCommandTypes)value);
  return 0;
}

int setAxisCmdData(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setCmdData(value);
  return 0;
}

int setAxisSeqTimeout(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setSequenceTimeout(value * MCU_FREQUENCY);
  return 0;
}

int getAxisCommand(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  =  static_cast<int>(axes[axisIndex]->getSeq()->getCommand());
  return 0;
}

int getAxisCmdData(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getSeq()->getCmdData();
  return 0;
}

int getAxisDebugInfoData(int axisIndex, char *buffer, int bufferByteSize) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  int bytesUsed = 0;
  return axes[axisIndex]->getAxisDebugInfoData(buffer,
                                               bufferByteSize,
                                               &bytesUsed);
}

int getAxisStatusStructV2(int axisIndex, char *buffer, int bufferByteSize) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  ecmcAxisStatusType data;
  int error = axes[axisIndex]->getDebugInfoData(&data);

  if (error) {
    return error;
  }

  // (Ax,PosSet,PosAct,PosErr,PosTarg,DistLeft,CntrOut,VelFFSet,VelAct,VelFFRaw,VelRaw,CycleCounter,
  // Error,Co,CD,St,IL,TS,ES,En,Ena,Ex,Bu,Ta,L-,L+,Ho");
  int ret = snprintf(buffer,
                     bufferByteSize,
                     "Main.M%d.stAxisStatusV2=%g,%g,%" PRId64 ",%g,%g,%g,%g,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     axisIndex,
                     data.onChangeData.positionTarget,
                     data.onChangeData.positionActual,
                     data.onChangeData.positionRaw,
                     // UINT64_t
                     data.onChangeData.velocitySetpoint,
                     data.onChangeData.velocityActual,
                     data.acceleration,
                     data.deceleration,
                     data.cycleCounter,
                     0,
                     // EtherCAT time low32 not available yet
                     0,
                     // EtherCAT time high32 not available yet
                     data.onChangeData.enable,
                     data.onChangeData.enabled,
                     data.onChangeData.execute,
                     data.onChangeData.command,
                     data.onChangeData.cmdData,
                     data.onChangeData.limitBwd,
                     data.onChangeData.limitFwd,
                     data.onChangeData.homeSwitch,
                     data.onChangeData.error > 0,
                     data.onChangeData.error,
                     data.reset,
                     data.onChangeData.homed,
                     data.onChangeData.busy,
                     data.onChangeData.atTarget,
                     data.moving,
                     data.stall);

  if ((ret >= bufferByteSize) || (ret <= 0)) {
    return ERROR_MAIN_PRINT_TO_BUFFER_FAIL;
  }

  return 0;
}

int setAxisEnable(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  if (!value) {
    axes[axisIndex]->setExecute(value);
  }

  return axes[axisIndex]->setEnable(value);
}

int getAxisEnable(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getEnable() > 0;
  return 0;
}

int setAxisEnableAlarmAtHardLimits(int axisIndex, int enable) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           enable);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  int error = axes[axisIndex]->getMon()->setEnableHardLimitBWDAlarm(enable);

  if (error) {
    return error;
  }

  error = axes[axisIndex]->getMon()->setEnableHardLimitFWDAlarm(enable);

  if (error) {
    return error;
  }
  return 0;
}

int setAxisBlockCom(int axisIndex, int block) {
  LOGINFO4("%s/%s:%d axisIndex=%d, block=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           block);
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->setBlockExtCom(block);
}

int getAxisBlockCom(int axisIndex, int *block) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * block = 0;
  *block  = axes[axisIndex]->getBlockExtCom();
  return 0;
}

int getAxisEnableAlarmAtHardLimits(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getMon()->getEnableAlarmAtHardLimit() > 0;
  return 0;
}

int getAxisEnabled(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getEnabled() > 0;
  return 0;
}

int getAxisTrajSource(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex);

  *value =  static_cast<int>(axes[axisIndex]->
                                 getTrajDataSourceType());
  return 0;
}

int getAxisEncSource(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  *value =  static_cast<int>(axes[axisIndex]->
                                 getEncDataSourceType());
  return 0;
}

int getAxisAllowCommandsFromPLC(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  =  static_cast<int>(axes[axisIndex]->getAllowCmdFromPLC() > 0);
  return 0;
}

int getAxisPLCEnable(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);
  CHECK_PLCS_RETURN_IF_ERROR();
  CHECK_PLC_RETURN_IF_ERROR(AXIS_PLC_ID_TO_PLC_ID(axisIndex));

  int enable = 0;
  int error = plcs->getEnable(AXIS_PLC_ID_TO_PLC_ID(axisIndex),&enable);
  if(error) {
    return error;
  }

  *value  = enable;
  return 0;
}

int getAxisType(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getAxisType();
  return 0;
}

int setAxisOpMode(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  return axes[axisIndex]->setOpMode((operationMode)value);
}

int getAxisOpMode(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getOpMode();
  return 0;
}

int setAxisEnableSoftLimitBwd(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);
  
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getMon()->setEnableSoftLimitBwd(value);
  return 0;
}

int setAxisEnableSoftLimitFwd(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getMon()->setEnableSoftLimitFwd(value);
  return 0;
}

int setAxisSoftLimitPosBwd(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getMon()->setSoftLimitBwd(value);
  return 0;
}

int setAxisSoftLimitPosFwd(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getMon()->setSoftLimitFwd(value);
  return 0;
}

int getAxisSoftLimitPosBwd(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);
  
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getMon()->getSoftLimitBwd();
  return 0;
}

int getAxisSoftLimitPosFwd(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getMon()->getSoftLimitFwd();
  return 0;
}

int getAxisEnableSoftLimitBwd(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getMon()->getEnableSoftLimitBwd() > 0;
  return 0;
}

int getAxisEnableSoftLimitFwd(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getMon()->getEnableSoftLimitFwd() > 0;
  return 0;
}

int getAxisBusy(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getBusy() > 0;
  return 0;
}

int setAxisAcceleration(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getTraj()->setAcc(value);
  return 0;
}

int setAxisDeceleration(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getTraj()->setDec(value);
  return 0;
}

int setAxisEmergDeceleration(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getTraj()->setEmergDec(value);
  return 0;
}

int setAxisJerk(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  // TODO not implemented in trajectory generator
  axes[axisIndex]->getTraj()->setJerk(value);
  return 0;
}

int setAxisTargetPos(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setTargetPos(value);
  return 0;
}

int setAxisTargetVel(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setTargetVel(value);

  return 0;
}

int setAxisJogVel(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setJogVel(value);
  return 0;
}

int setAxisHomeVelTwordsCam(int axisIndex, double dVel) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           dVel);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->getSeq()->setHomeVelTwordsCam(dVel);
}

int setAxisHomeVelOffCam(int axisIndex, double dVel) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           dVel);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->getSeq()->setHomeVelOffCam(dVel);
}

int setAxisHomePos(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setHomePosition(value);
  return 0;
}

int setAxisHomeLatchCountOffset(int axisIndex, int count) {
  LOGINFO4("%s/%s:%d axisIndex=%d count=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           count);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getSeq()->setHomeLatchCountOffset(count);
  return 0;
}

int setAxisAllowCommandsFromPLC(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  return axes[axisIndex]->setAllowCmdFromPLC(value);
}

int setAxisPLCEnable(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_PLCS_RETURN_IF_ERROR();
  CHECK_PLC_RETURN_IF_ERROR(AXIS_PLC_ID_TO_PLC_ID(axisIndex));

  return plcs->setEnable(AXIS_PLC_ID_TO_PLC_ID(axisIndex),value);
}

int axisErrorReset(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%i value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  axes[axisIndex]->setReset(value);
  return 0;
}

/*
int setAxisGearRatio(int axisIndex, double ratioNum, double ratioDenom) {
  LOGINFO4("%s/%s:%d axisIndex=%d num=%lf denom=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           ratioNum,
           ratioDenom);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  return axes[axisIndex]->getExternalTrajIF()->setGearRatio(ratioNum,
                                                            ratioDenom);
}*/

int setAxisEncScaleNum(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  double temp = 0;
  int errorCode = axes[axisIndex]->getEncScaleNum(&temp);

  if (errorCode) {
    return errorCode;
  }

  if (temp == value) {
    return 0;
  }

  // Change of encoder scale while axis enabled is not allowed
  if (axes[axisIndex]->getEnable()) {
    return ERROR_MAIN_ENC_SET_SCALE_FAIL_DRV_ENABLED;
  }
  axes[axisIndex]->setEncScaleNum(value);

  return 0;
}

int setAxisEncScaleDenom(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  double temp = 0;
  int errorCode = axes[axisIndex]->getEncScaleDenom(&temp);

  if (errorCode) {
    return errorCode;
  }

  if (temp == value) {
    return 0;
  }

  // Change of encoder scale while axis enabled is not allowed
  if (axes[axisIndex]->getEnable()) {
    return ERROR_MAIN_ENC_SET_SCALE_FAIL_DRV_ENABLED;
  }
  axes[axisIndex]->setEncScaleDenom(value);

  return 0;
}

int setAxisPLCExpr(int axisIndex, char *expr) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%s\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           expr);
  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_PLCS_RETURN_IF_ERROR();
  CHECK_PLC_RETURN_IF_ERROR(AXIS_PLC_ID_TO_PLC_ID(axisIndex));

  return plcs->appendExprLine(AXIS_PLC_ID_TO_PLC_ID(axisIndex),expr);
}

int setAxisTrajExtVelFilterEnable(int axisIndex, int enable) {
  LOGINFO4("%s/%s:%d axisIndex=%d enable=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           enable);
  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)  
  
  return axes[axisIndex]->setEnableExtTrajVeloFilter(enable);
}

int setAxisEncExtVelFilterEnable(int axisIndex, int enable) {
  LOGINFO4("%s/%s:%d axisIndex=%d enable=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           enable);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)

  return axes[axisIndex]->setEnableExtEncVeloFilter(enable);
}

const char* getAxisPLCExpr(int axisIndex, int *error) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  if ((axisIndex >= ECMC_MAX_AXES) || (axisIndex <= 0)) {
    LOGERR("ERROR: Axis index out of range.\n");
    *error = ERROR_MAIN_AXIS_INDEX_OUT_OF_RANGE;
    return "";
  }

  if (axes[axisIndex] == NULL) {
    LOGERR("ERROR: Axis object NULL\n");
    *error = ERROR_MAIN_AXIS_OBJECT_NULL;
    return "";
  }

  int plcIndex = ECMC_MAX_PLCS + axisIndex;

  if (plcIndex >= ECMC_MAX_PLCS + ECMC_MAX_AXES || plcIndex < ECMC_MAX_PLCS) {
    LOGERR("ERROR: PLC index out of range.\n");
    *error=ERROR_PLCS_INDEX_OUT_OF_RANGE;
    return "";
  }
    
  int errorLocal = 0;
  std::string *expr = plcs->getExpr(plcIndex,&errorLocal);
  if(errorLocal) {
    *error = errorLocal;
    return "";
  }

  return expr->c_str();
}

int setAxisTrajSource(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->setTrajDataSourceType((dataSource)value);
}

int setAxisEncSource(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->setEncDataSourceType((dataSource)value);
}

int setAxisTrajStartPos(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex)
  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  axes[axisIndex]->getTraj()->setStartPos(value);
  return 0;
}

// *****GET*********
int getAxisAcceleration(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getTraj()->getAcc();
  return 0;
}

int getAxisDeceleration(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getTraj()->getDec();
  return 0;
}

int getAxisTargetPos(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  return axes[axisIndex]->getPosSet(value);
}

int getAxisTargetVel(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getSeq()->getTargetVel();
  return 0;
}

int getAxisDone(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = !axes[axisIndex]->getSeq()->getBusy() > 0;
  return 0;
}

int getAxisPosSet(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->getPosSet(value);
}

int getAxisVelFF(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_TRAJ_RETURN_IF_ERROR(axisIndex)

  * value = axes[axisIndex]->getTraj()->getVel();
  return 0;
}

int getAxisExecute(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getExecute() > 0;
  return 0;
}

int getAxisReset(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getReset() > 0;
  return 0;
}

int getAxisID(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)
  * value = axes[axisIndex]->getAxisID();
  return 0;
}

/*int getAxisGearRatio(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%i\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  return axes[axisIndex]->getExternalTrajIF()->getGearRatio(value);
}*/

int getAxisAtHardFwd(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getMon()->getHardLimitFwd() > 0;
  return 0;
}

int getAxisAtHardBwd(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex)
  * value = 0;
  *value  = axes[axisIndex]->getMon()->getHardLimitBwd() > 0;
  return 0;
}

int getAxisEncHomed(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  bool tempHomed = 0;
  int errorCode = axes[axisIndex]->getAxisHomed(&tempHomed);

  if (errorCode) {
    return errorCode;
  }
  *value = 0;
  *value = tempHomed > 0;
  return 0;
}

int getAxisEncPosAct(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  if (int iRet = axes[axisIndex]->getPosAct(value)) {
    value = 0;
    return iRet;
  }
  return 0;
}

int getAxisEncVelAct(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  if (int iRet = axes[axisIndex]->getVelAct(value)) {
    *value = 0;
    return iRet;
  }

  return 0;
}

int getAxisAtHome(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  if (axes[axisIndex]->getMon() == NULL) {
    return ERROR_MAIN_MONITOR_OBJECT_NULL;
  }
  *value = 0;
  *value = axes[axisIndex]->getMon()->getHomeSwitch() > 0;
  return 0;
}

int getAxisCntrlError(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex)

  if (int iRet = axes[axisIndex]->getCntrlError(value)) {
    *value = 0;
    return iRet;
  }
  return 0;
}

int getAxisHomeVelOffCam(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getSeq()->getHomeVelOffCam();
  return 0;
}

int getAxisHomeVelTwordsCam(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);
  
  CHECK_AXIS_SEQ_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getSeq()->getHomeVelTwordsCam();
  return 0;
}

int getAxisEncScaleNum(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  if (int iRet = axes[axisIndex]->getEncScaleNum(value)) {
    value = 0;
    return iRet;
  }
  return 0;
}

int getAxisEncScaleDenom(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  if (int iRet = axes[axisIndex]->getEncScaleDenom(value)) {
    *value = 0;
    return iRet;
  }
  return 0;
}

int getAxisEncPosRaw(int axisIndex, int64_t *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  if (int iRet = axes[axisIndex]->getEncPosRaw(value)) {
    *value = 0;
    return iRet;
  }
  return 0;
}

/****************************************************************************/

int setAxisCntrlKp(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setKp(value);
  return 0;
}

int setAxisCntrlKi(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setKi(value);
  return 0;
}

int setAxisCntrlKd(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setKd(value);
  return 0;
}

int setAxisCntrlKff(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setKff(value);
  return 0;
}

int setAxisCntrlOutHL(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setOutMax(value);
  return 0;
}

int setAxisCntrlOutLL(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setOutMin(value);
  return 0;
}

int setAxisCntrlIpartHL(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setIOutMax(value);
  return 0;
}

int setAxisCntrlIpartLL(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getCntrl()->setIOutMin(value);
  return 0;
}

int getAxisCntrlOutPpart(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getCntrl()->getOutPPart();
  return 0;
}

int getAxisCntrlOutIpart(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getCntrl()->getOutIPart();
  return 0;
}

int getAxisCntrlOutDpart(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getCntrl()->getOutDPart();
  return 0;
}

int getAxisCntrlOutFFpart(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getCntrl()->getOutFFPart();
  return 0;
}

int getAxisCntrlOutput(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_CONTROLLER_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getCntrl()->getOutTot();
  return 0;
}

int setAxisEncOffset(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getEnc()->setOffset(value);
}

int setAxisEncBits(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getEnc()->setBits(value);
}

int setAxisEncAbsBits(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getEnc()->setAbsBits(value);
}

int setAxisEncRawMask(int axisIndex, uint64_t rawMask) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           (uint)rawMask);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getEnc()->setRawMask(rawMask);
}

int setAxisEncType(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getEnc()->setType((encoderType)value);
}

/****************************************************************************/

// Drv GET
int getAxisDrvScaleNum(int axisIndex, double *value) {  
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);
  *value = axes[axisIndex]->getDrv()->getScaleNum();
  return 0;
}

// Drv SET
int setAxisDrvScaleNum(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getDrv()->setScaleNum(value);
  return 0;
}

int setAxisDrvScaleDenom(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setScaleDenom(value);
}

int setAxisDrvEnable(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setEnable(value);
}

int setAxisDrvVelSet(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setVelSet(value);
}

int setAxisDrvVelSetRaw(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setVelSetRaw(value);
}

int setAxisDrvBrakeEnable(int axisIndex, int enable) {
  LOGINFO4("%s/%s:%d axisIndex=%d enable=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           enable);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setEnableBrake(enable);
}

int setAxisDrvBrakeOpenDelayTime(int axisIndex, int delayTime) {
  LOGINFO4("%s/%s:%d axisIndex=%d delayTime=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           delayTime);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setBrakeOpenDelayTime(delayTime);
}

int setAxisDrvBrakeCloseAheadTime(int axisIndex, int aheadTime) {
  LOGINFO4("%s/%s:%d axisIndex=%d aheadTime=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           aheadTime);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setBrakeCloseAheadTime(aheadTime);
}

int setAxisDrvReduceTorqueEnable(int axisIndex, int enable) {
  LOGINFO4("%s/%s:%d axisIndex=%d enable=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           enable);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getDrv()->setEnableReduceTorque(enable);
}

int setAxisDrvType(int axisIndex, int type) {
  LOGINFO4("%s/%s:%d axisIndex=%d type=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           type);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  try {
    return axes[axisIndex]->setDriveType((ecmcDriveTypes)type);
  }
  catch (std::exception& e) {
    LOGERR("%s/%s:%d: EXCEPTION %s WHEN SET DRIVE TYPE.\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           e.what());
    return ERROR_MAIN_EXCEPTION;
  }
  return 0;
}

// Drv GET
int getAxisDrvScale(int axisIndex, double *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getDrv()->getScale();
  return 0;
}

int getAxisDrvEnable(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);
  *value = 0;
  *value = axes[axisIndex]->getDrv()->getEnable() > 0;
  return 0;
}

// Mon GET
int getAxisMonAtTargetTol(int axisIndex, double *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getAtTargetTol();
  return 0;
}

// Mon SET
int setAxisMonAtTargetTol(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setAtTargetTol(value);
  return 0;
}

int getAxisMonAtTargetTime(int axisIndex, int *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getAtTargetTime();
  return 0;
}

int setAxisMonAtTargetTime(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setAtTargetTime(value);
  return 0;
}

int getAxisMonEnableAtTargetMon(int axisIndex, int *value) {
  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getEnableAtTargetMon();
  return 0;
}

int setAxisMonEnableAtTargetMon(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setEnableAtTargetMon(value);
  return 0;
}

int setAxisMonExtHWInterlockPolarity(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setHardwareInterlockPolarity(
                                        (externalHWInterlockPolarity)value);
  return 0;
}

int getAxisMonPosLagTol(int axisIndex, double *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getPosLagTol();
  return 0;
}

int setAxisMonPosLagTol(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%f\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setPosLagTol(value);
  return 0;
}

int getAxisMonPosLagTime(int axisIndex, int *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getPosLagTime();
  return 0;
}

int setAxisMonPosLagTime(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setPosLagTime(value);
  return 0;
}

int getAxisMonEnableLagMon(int axisIndex, int *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);
  *value = axes[axisIndex]->getMon()->getEnableLagMon();
  return 0;
}

int setAxisMonEnableLagMon(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  axes[axisIndex]->getMon()->setEnableLagMon(value);
  return 0;
}

int setAxisMonMaxVel(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setMaxVel(value);
}

int getAxisMonMaxVel(int axisIndex, double *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getMaxVel();
  return 0;
}

int setAxisMonEnableMaxVel(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setEnableMaxVelMon(value);
}

int getAxisMonEnableMaxVel(int axisIndex, int *value) {
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  *value = axes[axisIndex]->getMon()->getEnableMaxVelMon();
  return 0;
}

int setAxisMonMaxVelDriveILDelay(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setMaxVelDriveTime(value);
}

int setAxisMonMaxVelTrajILDelay(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setMaxVelTrajTime(value);
}

int setAxisMonEnableExternalInterlock(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setEnableHardwareInterlock(value);
}

int setAxisMonEnableCntrlOutHLMon(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setEnableCntrlHLMon(value);
}

int setAxisMonEnableVelocityDiff(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setEnableVelocityDiffMon(value);
}

int setAxisMonVelDiffTol(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setVelDiffMaxDifference(value);
}

int setAxisMonVelDiffTrajILDelay(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setVelDiffTimeTraj(value);
}

int setAxisMonVelDiffDriveILDelay(int axisIndex, int value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setVelDiffTimeDrive(value);
}

int setAxisMonCntrlOutHL(int axisIndex, double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d value=%lf\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->getMon()->setCntrlOutputHL(value);
}

// Mon GET
int getAxisMonAtTarget(int axisIndex, int *value) {
  LOGINFO4("%s/%s:%d axisIndex=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);
  *value = 0;
  *value = axes[axisIndex]->getMon()->getAtTarget() > 0;
  return 0;
}

/****************************************************************************/

// Configuration procedures

int createAxis(int index, int type) {
  LOGINFO4("%s/%s:%d axisIndex=%d type:%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           index,
           type);

  if ((index < 0) && (index >= ECMC_MAX_AXES)) {
    return ERROR_MAIN_AXIS_INDEX_OUT_OF_RANGE;
  }

  try {
    switch ((axisType)type) {
    case ECMC_AXIS_TYPE_REAL:

      if (axes[index] != NULL) {
        delete axes[index];
      }
      axes[index] = new ecmcAxisReal(asynPort, index, 1 / MCU_FREQUENCY);
      break;

    case ECMC_AXIS_TYPE_VIRTUAL:

      if (axes[index] != NULL) {
        delete axes[index];
      }
      axes[index] = new ecmcAxisVirt(asynPort, index, 1 / MCU_FREQUENCY);      
      break;

    default:
      return ERROR_MAIN_AXIS_TYPE_UNKNOWN;
    }
  }
  catch (std::exception& e) {
    delete axes[index];
    axes[index] = NULL;
    LOGERR("%s/%s:%d: EXCEPTION %s WHEN ALLOCATE MEMORY FOR AXIS OBJECT.\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           e.what());
    return ERROR_MAIN_EXCEPTION;
  }

  axisDiagIndex = index;  // Always printout last axis added
  
  int error = createPLC(AXIS_PLC_ID_TO_PLC_ID(index),1,1);
  if (error) {
    return error;
  }

  return axes[index]->getErrorID();
}

int linkEcEntryToAxisEnc(int   slaveIndex,
                         char *entryIDString,
                         int   axisIndex,
                         int   encoderEntryIndex,
                         int   bitIndex) {
  LOGINFO4(
    "%s/%s:%d slave_index=%d entry=%s encoder=%d encoder_entry=%d bit_index=%d\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    slaveIndex,
    entryIDString,
    axisIndex,
    encoderEntryIndex,
    bitIndex);

  if (!ec->getInitDone()) return ERROR_MAIN_EC_NOT_INITIALIZED;

  ecmcEcSlave *slave = NULL;

  if (slaveIndex >= 0) {
    slave = ec->findSlave(slaveIndex);
  } else {    // simulation slave
    slave = ec->getSlave(slaveIndex);
  }

  if (slave == NULL) return ERROR_MAIN_EC_SLAVE_NULL;

  std::string sEntryID = entryIDString;

  ecmcEcEntry *entry = slave->findEntry(sEntryID);

  if (entry == NULL) return ERROR_MAIN_EC_ENTRY_NULL;

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_ENCODER_RETURN_IF_ERROR(axisIndex);

  if ((encoderEntryIndex >= MaxEcEntryLinks) ||
      (encoderEntryIndex <
       0)) return ERROR_MAIN_ENCODER_ENTRY_INDEX_OUT_OF_RANGE;

  return axes[axisIndex]->getEnc()->setEntryAtIndex(entry,
                                                    encoderEntryIndex,
                                                    bitIndex);
}

int linkEcEntryToAxisDrv(int   slaveIndex,
                         char *entryIDString,
                         int   axisIndex,
                         int   driveEntryIndex,
                         int   bitIndex) {
  LOGINFO4(
    "%s/%s:%d slave_index=%d entry=%s drive=%d drive_entry=%d bit_index=%d\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    slaveIndex,
    entryIDString,
    axisIndex,
    driveEntryIndex,
    bitIndex);

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_DRIVE_RETURN_IF_ERROR(axisIndex);

  // Disable brake output with empty string
  if ((strlen(entryIDString) == 0) &&
      (driveEntryIndex == ECMC_DRIVEBASE_ENTRY_INDEX_BRAKE_OUTPUT)) {
    return axes[axisIndex]->getDrv()->setEnableBrake(0);
  }

  // Disable reduce torque output with empty string
  if ((strlen(entryIDString) == 0) &&
      (driveEntryIndex == ECMC_DRIVEBASE_ENTRY_INDEX_REDUCE_TORQUE_OUTPUT)) {
    return axes[axisIndex]->getDrv()->setEnableReduceTorque(0);
  }

  if (!ec->getInitDone()) return ERROR_MAIN_EC_NOT_INITIALIZED;

  ecmcEcSlave *slave = NULL;

  if (slaveIndex >= 0) {
    slave = ec->findSlave(slaveIndex);
  } else {
    slave = ec->getSlave(slaveIndex);
  }

  if (slave == NULL) return ERROR_MAIN_EC_SLAVE_NULL;

  std::string sEntryID = entryIDString;

  ecmcEcEntry *entry = slave->findEntry(sEntryID);

  if (entry == NULL) return ERROR_MAIN_EC_ENTRY_NULL;

  if ((driveEntryIndex >= MaxEcEntryLinks) || (driveEntryIndex < 0)) {
    return ERROR_MAIN_DRIVE_ENTRY_INDEX_OUT_OF_RANGE;
  }

  int ret = axes[axisIndex]->getDrv()->setEntryAtIndex(entry,
                                                       driveEntryIndex,
                                                       bitIndex);

  if (ret) {
    return ret;
  }

  // Auto enable break
  if (driveEntryIndex == ECMC_DRIVEBASE_ENTRY_INDEX_BRAKE_OUTPUT) {
    ret = axes[axisIndex]->getDrv()->setEnableBrake(1);

    if (ret) {
      return ret;
    }
  }

  // Auto enable reduce torque
  if (driveEntryIndex == ECMC_DRIVEBASE_ENTRY_INDEX_REDUCE_TORQUE_OUTPUT) {
    ret = axes[axisIndex]->getDrv()->setEnableReduceTorque(1);

    if (ret) {
      return ret;
    }
  }

  return 0;
}

int linkEcEntryToAxisMon(int   slaveIndex,
                         char *entryIDString,
                         int   axisIndex,
                         int   monitorEntryIndex,
                         int   bitIndex) {
  LOGINFO4(
    "%s/%s:%d slave_index=%d entry=%s monitor=%d monitor_entry=%d bit_index=%d\n",
    __FILE__,
    __FUNCTION__,
    __LINE__,
    slaveIndex,
    entryIDString,
    axisIndex,
    monitorEntryIndex,
    bitIndex);

  if (!ec->getInitDone()) return ERROR_MAIN_EC_NOT_INITIALIZED;

  ecmcEcSlave *slave = NULL;

  if (slaveIndex >= 0) {
    slave = ec->findSlave(slaveIndex);
  } else {
    slave = ec->getSlave(slaveIndex);
  }

  if (slave == NULL) return ERROR_MAIN_EC_SLAVE_NULL;

  std::string sEntryID = entryIDString;

  ecmcEcEntry *entry = slave->findEntry(sEntryID);

  if (entry == NULL) {
    return ERROR_MAIN_EC_ENTRY_NULL;
  }

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);
  CHECK_AXIS_MON_RETURN_IF_ERROR(axisIndex);

  if ((monitorEntryIndex >= MaxEcEntryLinks) ||
      (monitorEntryIndex <
       0)) return ERROR_MAIN_MONITOR_ENTRY_INDEX_OUT_OF_RANGE;

  int errorCode = axes[axisIndex]->getMon()->setEntryAtIndex(entry,
                                                         monitorEntryIndex,
                                                         bitIndex);

  if (errorCode) {
    return errorCode;
  }

  if (monitorEntryIndex == ECMC_MON_ENTRY_INDEX_EXTINTERLOCK) {
    return setAxisMonEnableExternalInterlock(axisIndex, 1);
  }

  return 0;
}

int linkEcEntryToAxisStatusOutput(int   slaveIndex,
                                  char *entryIDString,
                                  int   axisIndex) {
  LOGINFO4("%s/%s:%d slave_index=%d entry=%s, axisId=%d\n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           slaveIndex,
           entryIDString,
           axisIndex);

  if (!ec->getInitDone()) return ERROR_MAIN_EC_NOT_INITIALIZED;

  ecmcEcSlave *slave = NULL;

  if (slaveIndex >= 0) {
    slave = ec->findSlave(slaveIndex);
  } else {
    slave = ec->getSlave(slaveIndex);
  }

  if (slave == NULL) return ERROR_MAIN_EC_SLAVE_NULL;

  std::string sEntryID = entryIDString;

  ecmcEcEntry *entry = slave->findEntry(sEntryID);

  if (entry == NULL) return ERROR_MAIN_EC_ENTRY_NULL;

  CHECK_AXIS_RETURN_IF_ERROR_AND_BLOCK_COM(axisIndex);

  return axes[axisIndex]->setEcStatusOutputEntry(entry);
}

int setDiagAxisIndex(int axisIndex) {
  LOGINFO4("%s/%s:%d axisIndex=%d \n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  axisDiagIndex = axisIndex;
  return 0;
}

int setDiagAxisFreq(int value) {
  LOGINFO4("%s/%s:%d frequency=%d \n", __FILE__, __FUNCTION__, __LINE__,
           value);

  if ((value < 1) ||
      (value > 500)) return ERROR_MAIN_DIAG_AXIS_FREQ_OUT_OF_RANGE;

  axisDiagFreq = value;
  return 0;
}

int setDiagAxisEnable(int value) {
  LOGINFO4("%s/%s:%d enable=%d \n", __FILE__, __FUNCTION__, __LINE__, value);

  WRITE_DIAG_BIT(FUNCTION_HW_MOTOR_AXIS_DIAGNOSTICS_BIT, value);

  return 0;
}

int getAxisModRange(int  axisIndex,
                     double *value){
  LOGINFO4("%s/%s:%d axisIndex=%d \n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  *value=axes[axisIndex]->getModRange();
  return 0;
}

int setAxisModRange(int  axisIndex,
                     double value) {
  LOGINFO4("%s/%s:%d axisIndex=%d, ModRange=%lf \n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           value);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->setModRange(value);
}

int getAxisModType(int  axisIndex,
                   int *type){
  LOGINFO4("%s/%s:%d axisIndex=%d \n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  *type = axes[axisIndex]->getModType();
  return 0;
}

int setAxisModType(int axisIndex,
                   int type) {
  LOGINFO4("%s/%s:%d axisIndex=%d, mod type=%d \n",
           __FILE__,
           __FUNCTION__,
           __LINE__,
           axisIndex,
           type);

  CHECK_AXIS_RETURN_IF_ERROR(axisIndex);

  return axes[axisIndex]->setModType(type);
}
