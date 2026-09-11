#include "types.h"
#include "scene.h"

/* object/table/read_active_value.c */
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

extern struct ObjectTableState *gWork;

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectTableEntry *entry =
        gWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->active == 1) {
        result = *entry->value_source->value;
    }
    return result;
}

/* object/table/find_active_by_value.c */
struct Inner {
    u8 unknown_000[40];
    s16 *value;
};

struct Object_08092be0 {
    u8 unknown_000[80];
    struct Inner *inner;
    u8 active;
};

/*
 * This owner's view of gWork. games/gs1/include/battle_effect_runtime.h declares
 * the same global as `struct BattleRuntime` with a different layout; both are
 * per-owner views of one object and only the fields each owner reads are
 * evidence. Named for the owner so the two cannot be mistaken for one type.
 */
struct Work_08092be0 {
    u8 unknown_000[20];
    struct Object_08092be0 *objects[4096];
};

extern struct Work_08092be0 *gWork;

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct Work_08092be0 *state = gWork;
    s32 result = -1;
    s32 index = 8;
    struct Object_08092be0 *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->inner->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->inner->value != value) {
                goto next;
            }
            result = index;
        }
    }
    return result;
}
