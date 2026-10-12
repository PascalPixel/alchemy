#include "types.h"
#include "input.h"
#include "object_runtime.h"
#include "party_state.h"

extern s32 Data_080f3944;

void Func_080d8fa8(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080d9978(struct ObjectRuntime *object)
{
    u8 *parts = object->animation;

    parts[22] = 253;
    parts[23] = 12;
    if (gInput.repeat & 0x200)
        Data_080f3944 = 1;
    if (Data_080f3944 != 0)
        Func_080d8fa8(0, 12, 13, 0);
}

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_080d99b8(s32 first_id, s32 second_id)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *first = Object_GetById(first_id);
    struct ObjectRuntime *second = Object_GetById(second_id);
    s32 x = owner->x >> 20;
    s32 y = owner->y >> 20;
    s32 z = owner->z >> 20;
    s32 first_x = first->x >> 20;
    s32 first_y = first->y >> 20;
    s32 first_z = first->z >> 20;
    s32 second_x = second->x >> 20;
    s32 second_y = second->y >> 20;
    s32 second_z = second->z >> 20;

    if (y == first_y) {
        if (x == first_x) {
            if (z == first_z - 1 || z == first_z + 1 || z == first_z)
                return 1;
        } else if (z == first_z) {
            if (x == first_x - 1 || x == first_x + 1 || x == first_x)
                return 1;
        }
    }
    if (y == second_y) {
        if (x == second_x) {
            if (z == second_z - 1 || z == second_z + 1 || z == second_z)
                return 1;
        } else if (z == second_z) {
            if (x == second_x - 1 || x == second_x + 1 || x == second_x)
                return 1;
        }
    }
    return 0;
}
