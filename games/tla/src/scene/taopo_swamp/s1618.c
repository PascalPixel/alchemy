#include "local.h"

void Func_02001618(void)
{
    if ((gFrameCount & 63) == 0) {
        s32 x = 0x1980000;
        s32 z = 0x1b80000;
        s32 y = 0xa00000;
        struct ObjectRuntime *object = Func_02002808(0x12d, x, y, z);
        s32 scale = 0x547a;

        if (object != NULL) {
            object->scaleX = scale;
            object->scaleY = scale;
            object->gravity = 0x7333;
            Func_02002800(object, Data_02002a60);
            ObjectDispatch_SetSingleChildFlags(object, 0);
        }
    }
}
