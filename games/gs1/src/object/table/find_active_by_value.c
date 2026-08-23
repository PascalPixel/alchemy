#include "types.h"

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
 * This owner's view of Data_03001ebc. games/gs1/include/battle_effect_runtime.h declares
 * the same global as `struct BattleRuntime` with a different layout; both are
 * per-owner views of one object and only the fields each owner reads are
 * evidence. Named for the owner so the two cannot be mistaken for one type.
 */
struct Work_08092be0 {
    u8 unknown_000[20];
    struct Object_08092be0 *objects[4096];
};

extern struct Work_08092be0 *Data_03001ebc;

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct Work_08092be0 *state = Data_03001ebc;
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
