#ifndef ALCHEMY_RIDE_H
#define ALCHEMY_RIDE_H

#include "types.h"

/// @file
/// The work of the Mars Lighthouse rooms' flame throwers and the fireballs that
/// knock the leader back or carry him along when they hit him, kept in the
/// inventory snapshot's spare space. The Ride_ names are older than that
/// reading; the struct and the routines that move a flame have taken it.

struct Actor;

/// One of the ride's four entries.
struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

/// An actor the ride checks against as it moves: a ride stops beside it
/// unless it is marked blocked, which sets the ride's own blocked flag.
/// The way a ride last moved along x and z, 1 or -1 (0xffff); the other axis
/// is 0.
struct RideDirection {
  u16 x;
  u16 z;
};

struct RideSlot {
  u16 id; ///< the actor's id; 0 ends the list
  u16 blocked;
};

struct FlameHitWork {
  s32 counter; ///< Ride_AdvanceCounter's count
  s16 active;  ///< set once a flame or fireball has hit the leader
  u16 blocked; ///< set when the ride met a blocked slot's actor
  u8 unk08[2];
  u16 value0a; ///< the step trigger raised when the leader is hit
  u16 value0c;
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[4];
  struct RideDirection direction;
  struct Actor *car; ///< the fireball carrying the hurt leader
  void *handle;
};

extern u8 gInventorySnapshot[];

/// The flame-hit work, in the inventory snapshot's spare space.
#define RIDE_WORK ((struct FlameHitWork *)(gInventorySnapshot + 0x188))

#endif
