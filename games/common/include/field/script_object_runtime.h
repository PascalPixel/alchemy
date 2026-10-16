#ifndef ALCHEMY_SCRIPT_OBJECT_RUNTIME_H
#define ALCHEMY_SCRIPT_OBJECT_RUNTIME_H

#include "types.h"
#include "layout_guard.h"

struct ScriptObjectRuntime {
  const s32 *script;
  u16 scriptCursor;
  u16 scriptValue;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x0a];
  u8 terrainId;
  u8 unk23;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
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
  s16 homeX;
  s16 homeZ;
  struct ScriptObjectRuntime *linkedObject;
  u8 unk6c[0x04];
};

LAYOUT_SIZE_GUARD(ScriptObjectRuntime_size, struct ScriptObjectRuntime, 0x70);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_script_offset,
                    struct ScriptObjectRuntime, script, 0x00);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_script_cursor_offset,
                    struct ScriptObjectRuntime, scriptCursor, 0x04);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_script_value_offset,
                    struct ScriptObjectRuntime, scriptValue, 0x06);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_x_offset, struct ScriptObjectRuntime, x,
                    0x08);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_speed_limit_offset,
                    struct ScriptObjectRuntime, speedLimit, 0x30);
LAYOUT_OFFSET_GUARD(ScriptObjectRuntime_linked_object_offset,
                    struct ScriptObjectRuntime, linkedObject, 0x68);

#endif
