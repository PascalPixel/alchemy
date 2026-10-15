#include "local.h"

void Func_02001140(struct ObjectRuntime *object)
{
    s32 phase = gFrameCount & 15;

    if (phase == 0) {
        struct ObjectRuntime *child = Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

        if (child != 0) {
            struct FieldSprite *sprite = child->animation;

            Func_02001c74(child, Data_02001ef4);
            child->motion_flags = 0;
            child->y = -0x78000;
            child->terrain_id = 1;
            if (sprite != 0) {
                Animation_ApplyChildArgument(sprite, 2);
                sprite->next_frame = 0;
                sprite->priority = 2;
            }
        }
    }
}
