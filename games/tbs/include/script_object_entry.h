#ifndef ALCHEMY_SCRIPT_OBJECT_ENTRY_H
#define ALCHEMY_SCRIPT_OBJECT_ENTRY_H

/// @file
/// ScriptObjectEntry, an entry in the script objects' table.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

/// A field object as the script commands' overlap checks see it.
struct ScriptObjectEntry {
  void *data;      ///< ObjectRuntime.script; null for a free object
  u8 unk04[4];     ///< the script cursor and angle
  s32 values08[6]; ///< x, y, z, terrainHeight, scaleX and scaleY
  u16 value20;     ///< ObjectRuntime.radius, in pixels
  u8 unk22[0x37];  ///< not named through this struct
  u8 flags59;      ///< ObjectRuntime.collisionFlags: bit 0 solid
  u8 unk5a[0x16];  ///< not named through this struct
};

STATIC_ASSERT(offsetof(struct ScriptObjectEntry, values08) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, value20) == 0x20);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, flags59) == 0x59);
STATIC_ASSERT(sizeof(struct ScriptObjectEntry) == 0x70);

#endif
