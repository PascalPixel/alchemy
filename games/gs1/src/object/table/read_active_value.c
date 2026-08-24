#include "types.h"

struct ObjectValueSource {
    u8 unknown_000[40];
    const s16 *value;
};

struct ObjectTableEntry {
    u8 unknown_000[80];
    struct ObjectValueSource *value_source;
    u8 active;
};

struct ObjectTableState {
    u8 unknown_000[20];
    struct ObjectTableEntry *objects[4096];
};

#define OBJECT_08092BA8_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ObjectValueSource_value_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectValueSource, value) == 0x28
        ? 1 : -1
];
typedef char ObjectTableEntry_value_source_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableEntry, value_source) == 0x50 ? 1 : -1
];
typedef char ObjectTableEntry_active_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableEntry, active) == 0x54 ? 1 : -1
];
typedef char ObjectTableState_objects_offset[
    OBJECT_08092BA8_OFFSET(struct ObjectTableState, objects) == 0x14 ? 1 : -1
];
#undef OBJECT_08092BA8_OFFSET

extern struct ObjectTableState *Data_03001ebc;

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectTableEntry *entry =
        Data_03001ebc->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1) {
        result = *entry->value_source->value;
    }
    return result;
}
