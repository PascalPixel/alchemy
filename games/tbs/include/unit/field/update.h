#ifndef ALCHEMY_UNIT_FIELD_UPDATE_H
#define ALCHEMY_UNIT_FIELD_UPDATE_H

#include "types.h"

/// The axis a moving object watches to know it has arrived at its target: the
/// one along which it has the furthest to go.
enum ArrivalAxis {
  ARRIVAL_AXIS_NONE = 0,
  ARRIVAL_AXIS_X = 16,
  ARRIVAL_AXIS_Y = 17,
  ARRIVAL_AXIS_Z = 18
};

void Object_UpdateAll(void);

#endif
