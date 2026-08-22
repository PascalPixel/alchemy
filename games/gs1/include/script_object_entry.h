#ifndef ALCHEMY_SCRIPT_OBJECT_ENTRY_H
#define ALCHEMY_SCRIPT_OBJECT_ENTRY_H

#include "types.h"

/* スクリプト衝突判定が走査する0x70バイトのオブジェクト表。 */
struct ScriptObjectEntry {
    void *data;
    u8 unknown_04[4];
    s32 values_08[6];
    u16 value_20;
    u8 unknown_22[0x37];
    u8 flags_59;
    u8 unknown_5a[0x16];
};

#define SCRIPT_OBJECT_ENTRY_OFFSET(field) \
    ((u32)&(((struct ScriptObjectEntry *)0)->field))

typedef char ScriptObjectEntry_values_08_offset[
    SCRIPT_OBJECT_ENTRY_OFFSET(values_08) == 0x08 ? 1 : -1
];
typedef char ScriptObjectEntry_value_20_offset[
    SCRIPT_OBJECT_ENTRY_OFFSET(value_20) == 0x20 ? 1 : -1
];
typedef char ScriptObjectEntry_flags_59_offset[
    SCRIPT_OBJECT_ENTRY_OFFSET(flags_59) == 0x59 ? 1 : -1
];
typedef char ScriptObjectEntry_size[
    sizeof(struct ScriptObjectEntry) == 0x70 ? 1 : -1
];

#undef SCRIPT_OBJECT_ENTRY_OFFSET

#endif
