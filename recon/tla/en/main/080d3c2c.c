#include "TYPES.H"
#include "RAM_BUFFER.H"

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

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct ObjectValueTable *state = Ram_HeapSlots->event_work;
    s32 result = -1;
    s32 index = 8;
    struct ObjectValueEntry *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->value_source->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 79) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->value_source->value != value)
                goto next;
            result = index;
        }
    }
    return result;
}
