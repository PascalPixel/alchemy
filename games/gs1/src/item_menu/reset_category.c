#include "types.h"
#include "scene.h"
#include "abi/item_menu/reset_category.h"
#include "layout_guard.h"

struct Object_080a9d84 {
    u8 padding[6];
    s16 value1;
    s16 value2;
    u8 padding2[5];
    u8 flag;
};

struct State_080a9d84 {
    u8 padding[200];
    struct Object_080a9d84 *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9d84_flag_offset, struct Object_080a9d84, flag, 15);
LAYOUT_OFFSET_GUARD(
    State_080a9d84_objects_offset, struct State_080a9d84, objects, 200);
LAYOUT_SIZE_GUARD(State_080a9d84_size, struct State_080a9d84, 328);

extern struct State_080a9d84 *gIw;

void ItemMenu_ResetCategory(void)
{
    struct State_080a9d84 *state = gIw;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct Object_080a9d84 *object = state->objects[index];

        if (object != 0) {
            object->value1 = 248;
            object->value2 = 168;
            object->flag = 240;
            Sys_Do(object);
        }
    }
}
