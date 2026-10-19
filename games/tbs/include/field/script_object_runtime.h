#ifndef ALCHEMY_SCRIPT_OBJECT_RUNTIME_H
#define ALCHEMY_SCRIPT_OBJECT_RUNTIME_H

/// @file
/// ScriptObjectRuntime, the work of an object a script moves.

#include "types.h"
#include "static_assert.h"

/// A field object as its object script commands see it: the script and its
/// place, position, motion toward a target, and the home it wanders around.
struct ScriptObjectRuntime {
  const s32 *script;
  u16 scriptCursor;
  u16 scriptValue;
  s32 x;             ///< 16.16 fixed-point pixels
  s32 y;             ///< 16.16 fixed-point pixels
  s32 z;             ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  u8 unk18[0x0a];
  u8 terrainId;
  u8 unk23;
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  u8 unk44[0x0c];
  void *animation;
  u8 animationKind;
  u8 flags;
  u8 unk56[0x03];
  u8 flags59;
  u8 actionFlags;
  u8 movementState;
  u8 unk5c[0x02];
  u16 turnedBack;
  void *callback;
  s16 homeX; ///< pixels
  s16 homeZ; ///< pixels
  struct ScriptObjectRuntime *linkedObject;
  u8 unk6c[0x04];
};

STATIC_ASSERT(sizeof(struct ScriptObjectRuntime) == 0x70);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptValue) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, x) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, speedLimit) == 0x30);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, linkedObject) == 0x68);

#endif
