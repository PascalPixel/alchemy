#include "types.h"
#include "party_state.h"

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_020003e8(void);
void Func_02000f98(void);
void Func_02001500(void);
void Func_02001dec(s32 value);
void Func_02001dfc(void);
void Func_02001e04(s32 a, s32 b, s32 c, s32 d);

void Func_02001054(void)
{
    /* FAKEMATCH: one-pass blocks between the entrance tests keep each a load of its own, as in overlay 657's s0b84.c (without them gcse shares one halfword load and compares it shifted, differ 37). */
    Scheduler_AddOrUpdateCallback(Func_020003e8, 0x480);
    Func_02001dfc();
    Func_02001e04(0, 0x200, 8, 9);
    Func_02001dec(170);
    Func_02000f98();
    if (gPartyState.entrance == 3)
        Func_02001500();
    do {
    } while (0);
    if (gPartyState.entrance == 4)
        Func_02001500();
    do {
    } while (0);
    if (gPartyState.entrance == 5)
        Func_02001500();
}
