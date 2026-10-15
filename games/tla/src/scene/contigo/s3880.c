#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Func_020017f4(void);
void Func_02001c44(s32 a);
void Func_0200400c(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY);
void Func_020040d4(s32 actor, s32 x, s32 z);

static __inline__ void Func_0200400c_(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200400c(srcX, srcY, width, height, destX, destY);
}

void Func_02003880(void)
{
    Func_02001c44(18);
    if (GameFlag_Test(0xa21) != 0) {
        Func_0200400c_(44, 40, 8, 3, 44, 36);
        Func_020040d4(19, 0, 0);
        Func_020040d4(20, 0, 0);
        Func_020040d4(21, 0, 0);
        Func_020040d4(22, 0, 0);
        Func_020040d4(23, 0, 0);
        Func_020040d4(24, 0, 0);
        Func_020040d4(25, 0, 0);
        Func_020040d4(26, 0, 0);
        Func_020040d4(27, 0, 0);
        Func_020040d4(28, 0, 0);
        Func_020040d4(29, 0, 0);
        Func_020040d4(30, 0, 0);
        Func_020040d4(31, 0, 0);
        Func_020040d4(32, 0, 0);
        Func_020040d4(33, 0, 0);
        Func_020040d4(8, 0, 0);
        Func_020040d4(9, 0, 0);
        Func_020040d4(11, 0, 0);
        Func_020040d4(12, 0, 0);
        Func_020040d4(13, 0, 0);
        if (GameFlag_Test(0x9b2) == 0) {
            Func_020040d4(10, 0, 0);
            Func_020040d4(14, 0, 0);
            Func_020040d4(15, 0, 0);
            Func_020040d4(16, 0, 0);
            Func_020040d4(17, 0, 0);
        } else {
            Object_GetById(10)->angle = 0xd000;
            WaitFrames(1);
        }
    } else {
        Func_020017f4();
    }
}
