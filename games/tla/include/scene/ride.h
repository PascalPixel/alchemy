#ifndef ALCHEMY_RIDE_H
#define ALCHEMY_RIDE_H

#include "fixed_math.h"
#include "types.h"

/// @file
/// The work of the Mars Lighthouse rooms' flame throwers and the fireballs that
/// knock the leader back or carry him along when they hit him, kept in the
/// inventory snapshot's spare space. The Ride_ names are older than that
/// reading; the struct and the routines that move a flame have taken it.

struct Actor;

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

/// One of the work's four flame throwers.
struct RideEntry {
  s16 active;
  s16 timer; ///< frames to the next puff, 8 after each
  /// enum FlameThrowerFlags in the high nibble (the Ride_Spawn routine each
  /// picks), the reach in the low.
  s16 flags;
  s16 offScreen; ///< puffs off screen and keeps the flames' sprites
  struct Actor *actor;
  FIXED reach; ///< 16.16, the low nibble of flags << 16
};

/// The way a ride last moved along x and z, 1 or -1 (0xffff); the other axis
/// is 0.
struct RideDirection {
  u16 x;
  u16 z;
};

/// An actor the flames check against as they move: a flame stops beside it
/// unless it is marked blocked, which sets the work's own blocked flag.
struct RideSlot {
  u16 id; ///< the actor's id; 0 ends the list
  u16 blocked;
};

/// The flame throwers' work: the hit counter, what a flame met, the flame
/// throwers and the actors they stop at, and the fireball carrying the leader.
struct FlameHitWork {
  s32 counter; ///< Ride_AdvanceCounter's count
  s16 active;  ///< set once a flame or fireball has hit the leader
  /// Set when a flame met a blocked slot's actor, and called back once for.
  u16 blocked;
  s16 wasBlocked;          ///< blocked as it was the frame before
  u16 hitTrigger;          ///< the step trigger raised when the leader is hit
  u16 flameVramBlock;      ///< the kept flame's VRAM cache entry (Ride_Create)
  s16 runsDuringPsynergy;  ///< Ride_SetMode: the flames are not paused by
                           ///< Psynergy
  void (*onBlocked)(void); ///< called once a flame meets its stop
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unusedAfterSlots[4]; ///< no code names it
  struct RideDirection direction;
  struct Actor *car; ///< the fireball carrying the hurt leader
  void *handle;
};

/// The step triggers the flame throwers raise: a flame that hits the leader
/// raises the trigger FlameThrowers_Start is given, a rolling fireball that
/// reaches him two more.
enum {
  STEP_TRIGGER_FLAME_HIT = 88,
  STEP_TRIGGER_FIREBALL_HIT = STEP_TRIGGER_FLAME_HIT + 2
};

extern u8 gInventorySnapshot[];

/// The flame-hit work, in the inventory snapshot's spare space.
#define RIDE_WORK ((struct FlameHitWork *)(gInventorySnapshot + 0x188))

#endif
