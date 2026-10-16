#ifndef ALCHEMY_SCRIPT_OBJECT_ENTRY_H
#define ALCHEMY_SCRIPT_OBJECT_ENTRY_H

#include "unit/field/field.h"
#include "types.h"
#include "layout_guard.h"

struct ScriptObjectEntry {
  void *data;
  u8 unk04[4];
  s32 values08[6];
  u16 value20;
  u8 unk22[0x37];
  u8 flags59;
  u8 unk5a[0x16];
};

LAYOUT_OFFSET_GUARD(ScriptObjectEntry_values_08_offset,
                    struct ScriptObjectEntry, values08, 0x08);
LAYOUT_OFFSET_GUARD(ScriptObjectEntry_value_20_offset, struct ScriptObjectEntry,
                    value20, 0x20);
LAYOUT_OFFSET_GUARD(ScriptObjectEntry_flags_59_offset, struct ScriptObjectEntry,
                    flags59, 0x59);
LAYOUT_SIZE_GUARD(ScriptObjectEntry_size, struct ScriptObjectEntry, 0x70);

#endif
