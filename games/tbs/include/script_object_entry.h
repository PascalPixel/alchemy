#ifndef ALCHEMY_SCRIPT_OBJECT_ENTRY_H
#define ALCHEMY_SCRIPT_OBJECT_ENTRY_H

/// @file
/// ScriptObjectEntry, an entry in the script objects' table.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// A field object as the script commands' overlap checks see it.
struct ScriptObjectEntry {
  void *data;       ///< Object.script; null for a free object
  u16 scriptCursor; ///< Object's step; not named through this struct
  u16 facing;       ///< Object's angle; not named through this struct
  s32 values08[6];  ///< x, y, z, terrainHeight, scaleX and scaleY
  u16 value20;      ///< Object.radius, in pixels
  /// ObjectEvent's terrainId to snapToTarget; not named through this struct.
  u8 terrainToSnap[0x37];
  u8 flags59; ///< Object.collisionFlags: bit 0 solid
  /// ObjectEvent's steeringFlags to its update hook; not named through this
  /// struct.
  u8 steeringToUpdate[0x16];
};

STATIC_ASSERT(offsetof(struct ScriptObjectEntry, values08) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, value20) == 0x20);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, flags59) == 0x59);
STATIC_ASSERT(sizeof(struct ScriptObjectEntry) == 0x70);

#endif
