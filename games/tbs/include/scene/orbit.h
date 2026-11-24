#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// A floating Nut's actor and sprite, seen through the fields its scenes set
/// (shared/actor_orbit.c and kalay.c): the actor's fields where the Nut keeps
/// the spot it sways round and how far through the sway it is.

#include "fixed_math.h"
#include "types.h"

/// A floating Nut's sprite, seen as the attribute bits, palette and state its
/// scenes set: struct Sprite read a byte at a time.
typedef struct OrbitingMapActorSprite {
  u8 link[4]; ///< Sprite's queue link
  u8 y;       ///< attribute 0's y
  /// Sprite's attribute 0 high byte.
  /// bits: 7..6 shape | 5 full colour | 4 mosaic | 3..2 sprite mode |
  /// 1..0 affine
  ///@{
  u8 affineModeMosaic : 5; ///< the scenes leave these
  u8 fullColor : 1;        ///< the Nut's scenes clear it
  u8 shape : 2;            ///< the scenes leave it
  ///@}
  u8 attr1[2];
  u8 tileLow; ///< attribute 2's low byte
  /// Sprite's attribute 2 high byte.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  ///@{
  u8 tileHigh : 2;      ///< tile number bits 9..8; the scenes leave them
  u8 priority : 2;      ///< the Nut's scenes set 1
  u8 paletteNumber : 4; ///< the Nut's scenes set 0
  ///@}
  u8 attr3[2];
  u8 shadow[12]; ///< Sprite's shadow part
  u8 scale[4];
  /// Sprite's slot: the VRAM cache entry the Nut's icon is loaded
  /// into.
  u8 palette;
  u8 cacheFlags; ///< retainCacheEntry and useOwnScale
  u8 rotation[2];
  u8 width;
  u8 height;
  u8 offsetX;
  u8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 state; ///< Sprite's count of layers; the Nut's scenes zero it
} OrbitingMapActorSprite;

/// A floating Nut's actor: its position, the spot it sways around and
/// how far through the sway it is, its sprite and its update routine.
typedef struct OrbitingMapActor {
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
  s32 groundHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 layer;
  /// Actor's priorityFlags; the Nut's scenes clear bit 0,
  /// ACTOR_PRIORITY_AUTOMATIC.
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  /// Actor.speedLimit: 1/65536 turns; advanced by TRIG_TURN / 64 plus two
  /// random steps each frame.
  s32 orbitAngle;
  s32 acceleration;
  FIXED orbitCenterX; ///< Actor.targetX: 16.16 fixed-point pixels
  FIXED orbitCenterY; ///< Actor.targetY: 16.16 fixed-point pixels
  FIXED orbitCenterZ; ///< Actor.targetZ: 16.16 fixed-point pixels
  s32 bounce;
  s32 gravity;
  s32 sceneWord;
  OrbitingMapActorSprite *sprite;
  u8 spriteStorageKind;
  u8 mode; ///< Actor's motionFlags; the Nut's scenes zero it
  /// Actor.arrivalAxis, the axis watched for arrival; Scene_FloatNut
  /// zeroes it.
  u8 state;
  u8 conditionResult;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  /// Actor.retainSpriteCache: the Nut's scenes keep its sprite's VRAM
  /// cache entry.
  u8 active;
  u8 repeatCount;
  s16 wait;
  u8 refused;
  /// Actor's frozen, which skips steering, terrain, gravity and floating;
  /// the Nut's scenes set it.
  u8 visible;
  u8 sceneCounter;
  u8 sceneByte;
  s16 timer;
  u16 userValue;
  struct Actor *watchedActor;
  /// Actor's update routine; Scene_FloatNut sets
  /// Scene_UpdateFloatingNut.
  u32 callback;
} OrbitingMapActor;

#endif
