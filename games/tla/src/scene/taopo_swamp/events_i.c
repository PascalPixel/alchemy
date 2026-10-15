#include "local.h"

s32 Func_0200166c(struct ObjectRuntime *object)
{
    if (object->y + object->velocityY < -0x300000)
        return 0;
    return 1;
}

void Func_02001684(void)
{
    if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {
        s32 offset = Random16Far() << 5;
        s32 x = offset + 0x1800000;
        s32 z = 0x1900000 - offset;
        s32 y = 0xa00000;
        struct ObjectRuntime *object = Func_02002808(0x12d, x, y, z);
        s32 scale = 0x547a;

        if (object != NULL) {
            object->scaleX = scale;
            object->scaleY = scale;
            object->motionFlags = 2;
            object->terrainHeight = 0xffc00000;
            object->gravity = 0x7333;
            Func_02002800(object, gData_02002a6c);
            ObjectDispatch_SetSingleChildFlags(object, 0);
        }
    }
}

void Func_020016fc(void)
{
    if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {
        s32 offset = Random16Far() << 5;
        s32 x = offset + 0x1a00000;
        s32 z = 0x1600000 - offset;
        s32 y = 0xa00000;
        struct ObjectRuntime *object = Func_02002808(0x12d, x, y, z);
        s32 scale = 0x547a;

        if (object != NULL) {
            object->scaleX = scale;
            object->scaleY = scale;
            object->motionFlags = 2;
            object->terrainHeight = 0xffc00000;
            object->gravity = 0x7333;
            Func_02002800(object, gData_02002a6c);
            ObjectDispatch_SetSingleChildFlags(object, 0);
        }
    }
}
