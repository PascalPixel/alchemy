#ifndef ALCHEMY_RIDE_H
#define ALCHEMY_RIDE_H

/// @file
/// The state of the Mars Lighthouse rooms' flame throwers and of the fireballs
/// that knock the leader back, or carry him along on a ride, when they hit him;
/// it is kept in the inventory snapshot's spare space.

#include "fixed_math.h"
#include "types.h"
#include "game/game.h"

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
/// A flame thrower's packed state: the flame's reach in the low four bits,
/// enum FlameThrowerFlags in the next four.
#define FLAME_THROWER_REACH_BITS 0xf
#define FLAME_THROWER_FLAG_BITS 0xf0

/// One of the state's four flame throwers.
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

/// The way a ride last moved along x and z, 1 or -1; the other axis is 0.
struct RideDirection {
  u16 x;
  u16 z;
};

/// An actor the flames check against as they move: a flame stops beside it
/// unless it is marked blocked, which sets the state's own blocked flag.
struct RideSlot {
  u16 id; ///< the actor's id; 0 ends the list
  u16 blocked;
};

/// The flame throwers' state: the hit counter, what a flame met, the flame
/// throwers and the actors they stop at, and the fireball carrying the leader.
struct FlameHitState {
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

/// The flame-hit state, in the inventory snapshot's spare space.
#define RIDE_STATE                                                             \
  ((struct FlameHitState *)(gInventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

/// The knock-back (shared/leader_knock_back.c) as the four Mars Lighthouse
/// rooms that keep a copy name it, and what it calls: the fall damage the
/// leader takes, the update hook that blinks him while he is thrown, his
/// animation, the terrain of his cell, and the collision check a step ahead.
#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision

#endif
