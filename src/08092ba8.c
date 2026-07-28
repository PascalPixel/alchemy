#include "types.h"

struct ValueSource_08092ba8 {
    u8 unknown_000[40];
    const s16 *value;
};

struct Object_08092ba8 {
    u8 unknown_000[80];
    struct ValueSource_08092ba8 *inner;
    u8 active;
};

struct State_08092ba8 {
    u8 unknown_000[20];
    struct Object_08092ba8 *objects[4096];
};

#define OBJECT_08092BA8_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ValueSource_08092ba8_value_offset[
    OBJECT_08092BA8_OFFSET(struct ValueSource_08092ba8, value) == 0x28
        ? 1 : -1
];
typedef char Object_08092ba8_inner_offset[
    OBJECT_08092BA8_OFFSET(struct Object_08092ba8, inner) == 0x50 ? 1 : -1
];
typedef char Object_08092ba8_active_offset[
    OBJECT_08092BA8_OFFSET(struct Object_08092ba8, active) == 0x54 ? 1 : -1
];
typedef char State_08092ba8_objects_offset[
    OBJECT_08092BA8_OFFSET(struct State_08092ba8, objects) == 0x14 ? 1 : -1
];
#undef OBJECT_08092BA8_OFFSET

extern struct State_08092ba8 *Data_03001ebc;

s32 Func_08092ba8(s32 key)
{
    s32 result = -1;
    struct Object_08092ba8 *object =
        Data_03001ebc->objects[(u32)key & 0x0fff];

    if (object != 0 && object->active == 1) {
        result = *object->inner->value;
    }
    return result;
}
