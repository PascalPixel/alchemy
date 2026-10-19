#ifndef ALCHEMY_BABI_PALACE_H
#define ALCHEMY_BABI_PALACE_H

#include "keystate.h"
#include "item_ids.h"
extern struct EventWork *gEventWork;

struct OwnerState;

union WorkCell {
  s32 w;
  u16 h;
};

static __inline__ void SetHalf(u16 *at, s32 value) {
  ((union WorkCell *)at)->h = value;
}

#endif
