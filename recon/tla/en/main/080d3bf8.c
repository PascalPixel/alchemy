#include "TYPES.H"

struct ObjectValueSource {
    u8 unknown_00[0x28];
    const s16 *value;
};

struct ObjectValueEntry {
    u8 unknown_00[0x50];
    struct ObjectValueSource *value_source;
    u8 active;
};

struct ObjectValueTable {
    u8 unknown_00[0x14];
    struct ObjectValueEntry *objects[4096];
};

extern struct ObjectValueTable *gEventWork;

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectValueEntry *entry =
        gEventWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1)
        result = *entry->value_source->value;
    return result;
}
