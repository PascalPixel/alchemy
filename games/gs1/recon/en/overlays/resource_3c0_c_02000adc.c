#include "types.h"

s32 Func_02001d2e(s32);
void Func_02001d40(s32);
void Func_02001d9e(s32, const void *);
void Func_02001dc6(s32, s32, s32);

void Func_02000adc(void)
{
    if (Func_02001d2e(2487) == 0) {
        s32 x = 240;
        s32 y = 206;
        s32 actor = 12;

        Func_02001d40(526);
        x <<= 15;
        y <<= 18;
        Func_02001dc6(actor, x, y);
        Func_02001d9e(12, (const void *)0x020097a8);
    }
}
