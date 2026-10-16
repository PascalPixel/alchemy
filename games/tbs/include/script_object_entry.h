#ifndef ALCHEMY_SCRIPT_OBJECT_ENTRY_H
#define ALCHEMY_SCRIPT_OBJECT_ENTRY_H

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

struct ScriptObjectEntry {
  void *data;
  u8 unk04[4];
  s32 values08[6];
  u16 value20;
  u8 unk22[0x37];
  u8 flags59;
  u8 unk5a[0x16];
};

STATIC_ASSERT(offsetof(struct ScriptObjectEntry, values08) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, value20) == 0x20);
STATIC_ASSERT(offsetof(struct ScriptObjectEntry, flags59) == 0x59);
STATIC_ASSERT(sizeof(struct ScriptObjectEntry) == 0x70);

#endif
