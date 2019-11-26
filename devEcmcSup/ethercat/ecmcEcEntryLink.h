/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*
*  ecmcEcEntryLink.h
*
*  Created on: May 23, 2016
*      Author: anderssandstrom
*
\*************************************************************************/

/**\file
* \defgroup ecmc
* \author Anders Sandström
* \contact anders.sandstrom@esss.se
*/

#ifndef ECMCECENTRYLINK_H_
#define ECMCECENTRYLINK_H_

#include "ecmcEcEntry.h"

#define MaxEcEntryLinks 20

struct entryInfo{
  ecmcEcEntry *entry;
  int          bitNumber;
};

class ecmcEcEntryLink : public ecmcError {
 public:
  ecmcEcEntryLink();
  ~ecmcEcEntryLink();
  int  setEntryAtIndex(ecmcEcEntry *entry,
                       int          index,
                       int          bitIndex);
  int  validateEntry(int index);
  int  readEcEntryValue(int       entryIndex,
                        uint64_t *value);
  int  writeEcEntryValue(int      entryIndex,
                         uint64_t value);
  bool checkEntryExist(int entryIndex);

 protected:
  int  validateEntryBit(int index);
  int  getEntryBitCount(int  index,
                        int *bitCount);
  int  getEntryStartBit(int  index,
                        int *startBit);

 private:
  entryInfo entryInfoArray_[MaxEcEntryLinks];
};

#endif  /* ECMCECENTRYLINK_H_ */
