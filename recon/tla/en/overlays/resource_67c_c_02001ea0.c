/* Draft, differ 2: the listing loads the sprite attribute byte before lsls of 0x4000; with rotation stored first the zero for nextFrame comes from the pool. Also tried: every order of the three sprite stores (8 to 19), the sprite through an asm reading the rotation (4), the rotation through an asm (2), a one-pass block around the rotation (4). In sched2 the shift has priority 6 through the r3 reuse and the attribute load 5. Round 2: the rotation pinned to r3 (14), the attribute byte read into a local first, with or without an asm (17), a volatile priority bitfield in each store order (2 to 19); a 3-minute permute found nothing better. */
#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02001ea0(struct ObjectRuntime *object)
{
    s32 phase = gFrameCount & 15;

    if (phase == 0) {
        struct ObjectRuntime *child = Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

        if (child != 0) {
            struct FieldSprite *sprite = child->animation;

            Func_02003ccc(child, Data_020040dc);
            child->motionFlags = 0;
            child->terrainId = 1;
            if (sprite != 0) {
                Animation_ApplyChildArgument(sprite, 2);
                sprite->nextFrame = 0;
                sprite->rotation = 0x4000;
                sprite->priority = 2;
            }
        }
    }
}
