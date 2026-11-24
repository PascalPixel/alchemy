#ifndef ALCHEMY_HANGING_VINE_H
#define ALCHEMY_HANGING_VINE_H

/// @file
/// The hanging vines the leader swings from: their links and their swing.

#include "fixed_math.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/sprite.h"

/// One link of a hanging vine: a point on the vine and its sprite.
struct HangingVineLink {
  struct HangingVineLink *next;
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y;
  FIXED z;
  u16 angle;
  s8 kind;            ///< 0 or 1: not drawn; else its picture, from 2
  u8 unusedAfterKind; ///< no code names it
  struct AnimationSpritePart sprite;
};

/// One of the eight hanging vines: the actor it hangs from and its swing.
struct HangingVineSlot {
  s16 id;                   ///< the actor the chain hangs from
  u8 unusedAfterId[2];      ///< no code names it
  s32 swing;                ///< the arc's width
  FIXED spacing;            ///< between links, in 16.16
  u8 unusedAfterSpacing[4]; ///< no code names it
  s32 phase;                ///< where the swing is in its arc, an angle
  s32 active;
  void *root; ///< the first link HangingVine_Hang hangs; NULL if free
};

/// The hanging vine's state (Ram_HeapSlots->hangingVine): the links' picture,
/// eight vine slots and up to 128 links.
struct HangingVineState {
  s32 vramEntry; ///< the links' picture
  struct HangingVineSlot slots[ROPE_SLOT_COUNT];
  struct HangingVineLink links[ROPE_NODE_COUNT];
};

#define HANGING_VINE_STATE                                                     \
  ((struct HangingVineState *)Ram_HeapSlots->hangingVine)

#endif
