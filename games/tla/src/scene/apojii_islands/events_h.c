#include "local.h"

void Func_02001140(struct ObjectRuntime *object)
{
    s32 phase = gFrameCount & 15;

    if (phase == 0) {
        struct ObjectRuntime *child = Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

        if (child != 0) {
            struct FieldSprite *sprite = child->animation;

            Func_02001c74(child, gData_02001ef4);
            child->motionFlags = 0;
            child->y = -0x78000;
            child->terrainId = 1;
            if (sprite != 0) {
                Animation_ApplyChildArgument(sprite, 2);
                sprite->nextFrame = 0;
                sprite->priority = 2;
            }
        }
    }
}
