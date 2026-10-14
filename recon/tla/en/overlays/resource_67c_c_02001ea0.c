/* Draft, differ 2: the listing loads the sprite attribute byte before lsls of 0x4000; with rotation stored first the zero for next_frame comes from the pool. */
#include "../../../../games/tla/src/field/overlay_67c/local.h"

void Func_02001ea0(struct ObjectRuntime *object)
{
    s32 phase = gFrameCount & 15;

    if (phase == 0) {
        struct ObjectRuntime *child = Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

        if (child != 0) {
            struct FieldSprite *sprite = child->animation;

            Func_02003ccc(child, Data_020040dc);
            child->motion_flags = 0;
            child->terrain_id = 1;
            if (sprite != 0) {
                Animation_ApplyChildArgument(sprite, 2);
                sprite->next_frame = 0;
                sprite->rotation = 0x4000;
                sprite->priority = 2;
            }
        }
    }
}
