#include "ecmcMonitor.hpp"

#include <stdio.h>

ecmcMonitor::ecmcMonitor()
{
  initVars();
}

ecmcMonitor::ecmcMonitor(bool enableAtTargetMon, bool enableLagMon)
{
  initVars();
  enableAtTargetMon_=enableAtTargetMon;
  enableLagMon_=enableLagMon;
}

void ecmcMonitor::initVars()
{
  errorReset();
  enable_=false;
  atTargetTol_=0;
  atTargetTime_=0;
  enableAtTargetMon_=true;
  posLagTol_=0;
  posLagTime_=0;
  enableLagMon_=true;
  actPos_=0;
  motionInterlock_=false;   //Not implemented
  atTarget_=false;
  atTargetCounter_=0;
  lagErrorTraj_=false;
  lagErrorDrive_=false;
  lagMonCounter_=0;
  hardBwd_=false;
  HardFwd_=false;
  homeSwitch_=false;
  targetPos_=0;
  lagError_=0;
  bothLimitsLowInterlock_=false;
  currSetPos_=0;
  targetVel_=0;
  actVel_=0;
  maxVel_=0;
  enableMaxVelMon_=true;
  velErrorTraj_=false;
  velErrorDrive_=false;
  maxVelCounterDrive_=0;
  maxVelCounterTraj_=0;
  maxVelDriveILDelay_=0;
  //for(int i=0;i<MaxMcuMonitorEntries;i++){
  //  entryArray_[i]=NULL;
  //}
  maxVelTrajILDelay_=200; //200 cycles
  maxVelDriveILDelay_=maxVelTrajILDelay_*2; //400 cycles default
}

ecmcMonitor::~ecmcMonitor()
{
  ;
}

void ecmcMonitor::setActPos(double pos)
{
  actPos_=pos;
}

void ecmcMonitor::execute()
{
  atTarget_=false;
  lagErrorTraj_=false;
  lagErrorDrive_=false;

  bothLimitsLowInterlock_=!hardBwd_ && !HardFwd_;

  if(!enable_){
    return;
  }

  //At target cMcuMonitoring
  if(enableAtTargetMon_){
    if(std::abs(targetPos_-actPos_)<atTargetTol_){
      if (atTargetCounter_<=atTargetTime_){
        atTargetCounter_++;
      }
      if(atTargetCounter_>atTargetTime_){
        atTarget_=true;
      }
    }
    else{
      atTargetCounter_=0;
    }
  }
  else{
    atTarget_=true;
  }

  //Lag error
  if(enableLagMon_ && !lagError_){
    lagError_=std::abs(actPos_-currSetPos_);

    if(lagError_>posLagTol_){
      if(lagMonCounter_<=posLagTime_*2){
        lagMonCounter_++;
      }
      if(lagMonCounter_>posLagTime_){
        lagErrorTraj_=true;
      }
      if(lagMonCounter_>posLagTime_*2){  //interlock the drive in twice the time..
        lagErrorDrive_=true;
      }
    }
    else{
      lagMonCounter_=0;
    }
  }

  //Max Vel error
  if((std::abs(actVel_)>maxVel_ || std::abs(targetVel_)>maxVel_) && enableMaxVelMon_){
    if(maxVelCounterTraj_ <= maxVelTrajILDelay_){
      maxVelCounterTraj_++;
    }
  }
  else{
    maxVelCounterTraj_=0;
  }

  if(!velErrorTraj_){
    velErrorTraj_=maxVelCounterTraj_>=maxVelTrajILDelay_;
  }

  if(velErrorTraj_ &&  maxVelCounterDrive_<= maxVelDriveILDelay_){
    maxVelCounterDrive_++;
  }
  else{
    maxVelCounterDrive_=0;
  }

  velErrorDrive_=velErrorTraj_ && maxVelCounterDrive_>=maxVelDriveILDelay_;

}

void ecmcMonitor::setTargetPos(double pos)
{
  targetPos_=pos;
}

double ecmcMonitor::getTargetPos()
{
  return targetPos_;
}

void ecmcMonitor::setCurrentPosSet(double pos)
{
  currSetPos_=pos;
}

double ecmcMonitor::getCurrentPosSet()
{
  return currSetPos_;
}


bool ecmcMonitor::getAtTarget()
{
  return atTarget_;
}

bool ecmcMonitor::getHardLimitFwd()
{
  return HardFwd_;
}

bool ecmcMonitor::getHardLimitBwd()
{
  return hardBwd_;
}

interlockTypes ecmcMonitor::getTrajInterlock()
{
  if(lagErrorTraj_){
    return ECMC_INTERLOCK_POSITION_LAG;
  }

  if(bothLimitsLowInterlock_){
    return ECMC_INTERLOCK_BOTH_LIMITS;
  }

  if(velErrorTraj_){
    return ECMC_INTERLOCK_MAX_SPEED;
  }

  return ECMC_INTERLOCK_NONE;
}

bool ecmcMonitor::getDriveInterlock()
{
  return lagErrorDrive_ || bothLimitsLowInterlock_ || velErrorDrive_;
}

void ecmcMonitor::setAtTargetTol(double tol)
{
  atTargetTol_=tol;
}

double ecmcMonitor::getAtTargetTol()
{
  return atTargetTol_;
}

void ecmcMonitor::setAtTargetTime(int time)
{
  atTargetTime_=time;
}

int ecmcMonitor::getAtTargetTime()
{
  return atTargetTime_;
}

void ecmcMonitor::setEnableAtTargetMon(bool enable)
{
  enableAtTargetMon_=enable;
}

bool ecmcMonitor::getEnableAtTargetMon()
{
  return enableAtTargetMon_;
}

void ecmcMonitor::setPosLagTol(double tol)
{
  posLagTol_=tol;
}

double ecmcMonitor::getPosLagTol()
{
  return posLagTol_;
}

void ecmcMonitor::setPosLagTime(int time){
  posLagTime_=time;
}

int ecmcMonitor::getPosLagTime()
{
  return posLagTime_;
}

void ecmcMonitor::setEnableLagMon(bool enable)
{
  enableLagMon_=enable;
}

bool ecmcMonitor::getEnableLagMon()
{
  return enableLagMon_;
}

void ecmcMonitor::setHomeSwitch(bool switchState)
{
  homeSwitch_=switchState;
}

bool ecmcMonitor::getHomeSwitch()
{
  return homeSwitch_;
}

/*int ecmcMonitor::setEntryAtIndex(ecmcEcEntry *entry,int index)
{
  if (entry!=NULL && index<MaxMcuMonitorEntries && index>=0) {
    entryArray_[index]=entry;
    return 0;
  } else {
    printf("Assigning entry to cMcuMonitor entry list failed. Index=%d \n",index);
    return setErrorID(ERROR_MON_ASSIGN_ENTRY_FAILED);
  }
}*/

void ecmcMonitor::readEntries(){
  uint64_t tempRaw=0;

  //Hard limit BWD
  if (readEcEntryValue(0,&tempRaw)) {
    setErrorID(ERROR_MON_ENTRY_READ_FAIL);
    return;
  }
  hardBwd_=tempRaw>0;

  //Hard limit FWD
  if(readEcEntryValue(1,&tempRaw)){
    setErrorID(ERROR_MON_ENTRY_READ_FAIL);
    return;
  }
  HardFwd_=tempRaw>0;

  //Home
  if(readEcEntryValue(2,&tempRaw)){
    setErrorID(ERROR_MON_ENTRY_READ_FAIL);
    return;
  }
  homeSwitch_=tempRaw>0;
}

void ecmcMonitor::setEnable(bool enable)
{
  enable_=enable;
}

bool ecmcMonitor::getEnable()
{
  return enable_;
}

int ecmcMonitor::validate()
{
  int error=validateEntryBit(0);
  if(error){ //Hard limit BWD
    return  setErrorID(ERROR_MON_ENTRY_HARD_BWD_NULL);
  }

  error=validateEntryBit(1);
  if(error){ //Hard limit FWD
    return  setErrorID(ERROR_MON_ENTRY_HARD_FWD_NULL);
  }

  error=validateEntryBit(2);
  if(error){ //Home
    return  setErrorID(ERROR_MON_ENTRY_HOME_NULL);
  }

  return 0;
}

int ecmcMonitor::setTargetVel(double vel)
{
  targetVel_=vel;
  return 0;
}

int ecmcMonitor::setActVel(double vel)
{
  actVel_=vel;
  return 0;
}

int ecmcMonitor::setMaxVel(double vel)
{
  maxVel_=vel;
  return 0;
}

int ecmcMonitor::setEnableMaxVelMon(bool enable)
{
  enableMaxVelMon_=enable;
  return 0;
}

bool ecmcMonitor::getEnableMaxVelMon()
{
  return enableMaxVelMon_;
}

int ecmcMonitor::setMaxVelDriveTime(int time)
{
  maxVelDriveILDelay_=time;
  return 0;
}

int ecmcMonitor::setMaxVelTrajTime(int time)
{
  maxVelTrajILDelay_=time;
  return 0;
}

int ecmcMonitor::reset()
{
  atTarget_=false;
  lagErrorTraj_=false;
  lagErrorDrive_=false;
  velErrorTraj_=false;
  velErrorDrive_=false;
  atTargetCounter_=0;
  lagMonCounter_=0;
  maxVelCounterDrive_=0;
  maxVelCounterTraj_=0;
  return 0;
}

