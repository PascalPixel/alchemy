#ifndef ALCHEMY_HANGING_VINE_H
#define ALCHEMY_HANGING_VINE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "field/sprite.h"

/// One link of a hanging vine: a point on the vine and its sprite.
struct HangingVineLink {
  struct HangingVineLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct AnimationSpritePart sprite;
};

/// One of the eight hanging vines: the actor it hangs from and its swing.
struct HangingVineSlot {
  s16 id; ///< the actor the chain hangs from
  u8 unk02[2];
  s32 swing;   ///< the arc's width
  s32 spacing; ///< between links, in 16.16
  u8 unk0c[4];
  s32 phase; ///< where the swing is in its arc, an angle
  s32 active;
  struct HangingVineLink
      *root; ///< the first link HangingVine_Hang hangs; NULL if free
};

struct HangingVineWork {
  s32 vramEntry; ///< the links' picture
  struct HangingVineSlot slots[8];
  struct HangingVineLink links[128];
};

#define HANGING_VINE_WORK ((struct HangingVineWork *)Ram_HeapSlots->unk84[8])

#endif
