#include "EVENT_RUNTIME.H"
#include "TYPES.H"

extern struct EventRuntime *Data_03001ebc;

struct EventRuntime1d8 {
    u8 unknown_000[0x1d8];
    s16 value;
};

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

void Event_SetValue1d8(s16 value)
{
    ((struct EventRuntime1d8 *)Data_03001ebc)->value = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectValueEntry *entry =
        gEventWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1)
        result = *entry->value_source->value;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct ObjectValueTable *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectValueEntry *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->value_source->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->value_source->value != value)
                goto next;
            result = index;
        }
    }
    return result;
}
