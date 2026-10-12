#include "local.h"

u32 __umodsi3(u32 a, u32 b);

s32 Func_020002dc(struct ObjectRuntime *object)
{
    switch (object->action) {
    case 6:
        object->scale_x -= 0x4000;
        {
            s32 scale = object->scale_y;

            /* FAKEMATCH: a one-pass block loads the scale before the step is built (one reordered row). */
            do {
            } while (0);
            object->scale_y = scale + 0x2000;
        }
        break;
    case 4:
        object->scale_x += 0x2000;
        object->scale_y -= 0x1000;
        break;
    case 2:
        object->scale_x += 0x1000;
        object->scale_y -= 0x800;
        break;
    case 0:
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        object->action = Random16Far() % 90 + 60;
        break;
    }
    object->action--;
    return 1;
}

s32 Func_0200035c(struct ObjectRuntime *object)
{
    object->motion_flags = 0;
    object->scale_x = 0xcccc;
    object->scale_y = 0xcccc;
    /* FAKEMATCH: the loop note ends the scheduling block after the scale stores; without it the collision flags are addressed before them (one reordered row). */
    do {
    } while (0);
    object->collision_flags |= 8;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    return 0;
}
