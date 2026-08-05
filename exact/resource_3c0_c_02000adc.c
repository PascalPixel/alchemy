#include "types.h"






extern s32 Func_02001d2e(s32 flagId);
extern void Func_02001d40(s32 flagId);
extern void Func_02001dc6(s32 actorId, s32 x, s32 y);
extern void Func_02001d9e(s32 actorId, void *descriptor);
void Func_02000adc(void)
{
    if (Func_02001d2e(0x9b7) != 0)
        return;
    Func_02001d40(0x20e);
    Func_02001dc6(12, 0x780000, 0x3380000);
    Func_02001d9e(12, (void *)0x020097a8);
}
