#include "types.h"
#include "scene.h"
#include "abi/item_menu/pos_category.h"
#include "layout_guard.h"

struct Object_080a9cbc {
    u8 padding[6];
    s16 value1;
    s16 value2;
};

struct State_080a9cbc {
    u8 padding[72];
    struct Object_080a9cbc *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9cbc_value1_offset, struct Object_080a9cbc, value1, 6);
LAYOUT_OFFSET_GUARD(
    State_080a9cbc_objects_offset, struct State_080a9cbc, objects, 72);
LAYOUT_SIZE_GUARD(State_080a9cbc_size, struct State_080a9cbc, 200);

extern struct State_080a9cbc *gIw;

void ItemMenu_PosCategory(void)
{
    struct State_080a9cbc *state = gIw;
    s32 value1 = 248;
    struct Object_080a9cbc **entry = state->objects;
    s32 value2 = 168;
    s32 remaining = 31;

    do {
        struct Object_080a9cbc *object = *entry++;

        if (object != 0) {
            object->value1 = value1;
            object->value2 = value2;
            Sys_Do(object);
        }
        remaining--;
    } while (remaining >= 0);
}
