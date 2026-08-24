#include "types.h"

extern u8 *Data_03001f2c;

s32 Func_08077148(void);
void Func_08009008(void *, s32 *, s32 *, s32);

void Func_080a19a0(void)
{
    u8 *objectSlot;
    s32 position[2];
    s32 transform[4];
    s32 *positionCursor;
    volatile s32 *transformCursor;
    s16 *heightCursor;
    s32 index;
    s32 objectCount;

    objectSlot = Data_03001f2c;
    objectCount = (u16)Func_08077148();
    index = 0;
    if (index < objectCount) {
        positionCursor = position;
        transformCursor = transform;
        heightCursor = (s16 *)(objectSlot + 308);
        objectSlot += 276;
        do {
            void *object;
            s32 top;

            top = 0x01e20000 - (heightCursor[8] << 16);
            object = *(void **)objectSlot;
            if (object != 0) {
                *((s8 *)object + 9) &= -13;
                position[0] = *(s32 *)(objectSlot + 64);
                positionCursor[1] = *(s32 *)(objectSlot + 64);
                transformCursor[1] = top;
                transformCursor[0] = heightCursor[0] << 16;
                transformCursor[2] = (heightCursor[8] << 16) + top;
                transformCursor[3] = 0;
                Func_08009008(object, (s32 *)transformCursor, positionCursor, 0x4000);
            }
            index++;
            heightCursor++;
            objectSlot += 4;
        } while (index < objectCount);
    }
}
