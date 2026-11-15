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
/// A flame thrower's flags (the flame thrower tables): the flame's reach in the
/// low four bits, and which flame it sends, or none.
enum FlameThrowerFlags {
  FLAME_FORWARD = 0x00,          ///< Ride_SpawnForward
  FLAME_LANDING_FORWARD = 0x10,  ///< Ride_SpawnLandingForward
  FLAME_LANDING_BACKWARD = 0x20, ///< Ride_SpawnLandingBackward
  FLAME_BACKWARD = 0x30,         ///< Ride_SpawnBackward
  FLAME_THROWER_DRY = 0x40,      ///< nothing comes out of it
  FLAME_SPARK = 0x80             ///< Ride_SpawnSpark
};

struct RideEntry {
  s16 active;
  s16 timer; ///< frames to the next puff, 8 after each
  /// enum FlameThrowerFlags in the high nibble (the Ride_Spawn routine each
  /// picks), the reach in the low.
  s16 flags;
  s16 offScreen; ///< puffs off screen and keeps the flames' sprites
  struct Actor *actor;
  s32 reach; ///< 16.16, the low nibble of flags << 16
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
