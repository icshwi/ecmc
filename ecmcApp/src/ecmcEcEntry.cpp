/*
 * ecmcEcEntry.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: anderssandstrom
 */

#include "ecmcEcEntry.h"

ecmcEcEntry::ecmcEcEntry(uint16_t entryIndex,uint8_t  entrySubIndex, uint8_t bits, ec_direction_t direction, std::string id)
{
  initVars();
  entryIndex_=entryIndex;
  entrySubIndex_=entrySubIndex;
  bitLength_= bits;
  direction_=direction;
  sim_=false;
  idString_=id;
}

ecmcEcEntry::ecmcEcEntry(uint8_t bits,uint8_t *domainAdr, std::string id)
{
  initVars();
  domainAdr_=domainAdr;
  bitLength_= bits;
  sim_=true;
  idString_=id;
}

void ecmcEcEntry::initVars()
{
  errorReset();
  domainAdr_=NULL;
  adrOffset_=0;
  bitLength_=0;
  bitOffset_=0;
  byteOffset_=0;
  entryIndex_=0;
  entrySubIndex_=0;
  bitLength_= 0;
  value_=0;
  direction_=EC_DIR_INVALID;
  sim_=false;
  idString_="";
}

ecmcEcEntry::~ecmcEcEntry()
{
  ;
}

void ecmcEcEntry::setDomainAdr(uint8_t *domainAdr)
{
  domainAdr_=domainAdr;
}

uint16_t ecmcEcEntry::getEntryIndex()
{
  return entryIndex_;
}

uint8_t ecmcEcEntry::getEntrySubIndex()
{
  return entrySubIndex_;
}

int ecmcEcEntry::getEntryInfo(ec_pdo_entry_info_t *info)
{
  if(info==NULL){
    LOGERR("%s/%s:%d: ERROR: output parameter pointer NULL (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_DATA_POINTER_NULL);
    return setErrorID(ERROR_EC_ENTRY_DATA_POINTER_NULL);
  }
  info->bit_length=bitLength_;
  info->index=entryIndex_;
  info->subindex=entrySubIndex_;
  return 0;
}

int ecmcEcEntry::getBits()
{
  return bitLength_;
}

void ecmcEcEntry::setAdrOffsets(int byteOffset,int bitOffset)
{
  byteOffset_=byteOffset;
  bitOffset_=bitOffset;
}

int ecmcEcEntry::writeValue(uint64_t value)
{
  value_=value;
  return 0;
}

int ecmcEcEntry::writeBit(int bitNumber, uint64_t value)
{
  if(value){
    BIT_SET(value_,bitNumber);
  }
  else{
    BIT_CLEAR(value_,bitNumber);
  }

  return 0;
}

int ecmcEcEntry::readValue(uint64_t *value)
{
  *value=value_;
  return 0;
}

int ecmcEcEntry::readBit(int bitNumber, uint64_t* value)
{
  *value=BIT_CHECK(value_,bitNumber);
  //*value=value_ & (int)pow(2,bitNumber);
  return 0;
}

int ecmcEcEntry::updateInputProcessImage()
{
  if(direction_!=EC_DIR_INPUT && !sim_){
    return 0;
  }

  if(byteOffset_<0 ){
    LOGERR("%s/%s:%d: ERROR: Invalid data offset (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_OFFSET);
    return setErrorID(ERROR_EC_ENTRY_INVALID_OFFSET);
  }

  if(domainAdr_<0 || domainAdr_==NULL){
    LOGERR("%s/%s:%d: ERROR: Invalid domain address (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_DOMAIN_ADR);
    return setErrorID(ERROR_EC_ENTRY_INVALID_DOMAIN_ADR);
  }

  switch( bitLength_){
    case 1:
      value_=(uint64_t)EC_READ_BIT(domainAdr_+byteOffset_, bitOffset_);
      break;
    case 8:
      value_=(uint64_t)EC_READ_U8(domainAdr_+byteOffset_);
      break;
    case 16:
      value_=(uint64_t)EC_READ_U16(domainAdr_+byteOffset_);
      break;
    case 32:
      value_=(uint64_t)EC_READ_U32(domainAdr_+byteOffset_);
      break;
    case 64:
      value_=(uint64_t)EC_READ_U64(domainAdr_+byteOffset_);
      break;
    default:
      LOGERR("%s/%s:%d: ERROR: Invalid bit length (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_BIT_LENGTH);
      return setErrorID(ERROR_EC_ENTRY_INVALID_BIT_LENGTH);
      break;
  }

  return 0;
}

int ecmcEcEntry::updateOutProcessImage()
{
  if(direction_!=EC_DIR_OUTPUT && !sim_){
    return 0;
  }

  if(byteOffset_<0 ){
    LOGERR("%s/%s:%d: ERROR: Invalid data offset (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_OFFSET);
    return setErrorID(ERROR_EC_ENTRY_INVALID_OFFSET);
  }

  if(domainAdr_<0 || domainAdr_==NULL){
    LOGERR("%s/%s:%d: ERROR: Invalid domain address (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_DOMAIN_ADR);
    return setErrorID(ERROR_EC_ENTRY_INVALID_DOMAIN_ADR);
  }

  switch( bitLength_){
    case 1:
      EC_WRITE_BIT(domainAdr_+byteOffset_, bitOffset_,value_);
      break;
    case 8:
      EC_WRITE_U8(domainAdr_+byteOffset_,value_);
      break;
    case 16:
      EC_WRITE_U16(domainAdr_+byteOffset_,value_);
      break;
    case 32:
      EC_WRITE_U32(domainAdr_+byteOffset_,value_);
      break;
    case 64:
      EC_WRITE_U64(domainAdr_+byteOffset_,value_);
      break;
    default:
      LOGERR("%s/%s:%d: ERROR: Invalid bit length (0x%x).\n",__FILE__, __FUNCTION__, __LINE__,ERROR_EC_ENTRY_INVALID_BIT_LENGTH);
      return setErrorID(ERROR_EC_ENTRY_INVALID_BIT_LENGTH);
      break;
  }
  return 0;
}

std::string ecmcEcEntry::getIdentificationName()
{
  return idString_;
}
