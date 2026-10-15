#include "ov686.h"

void Func_020020fc(void)
{
    s32 z = Object_GetById(9)->z >> 20;

    if (Ov686_FlagCheck(0x9dd) != 0 && z == 24) {
        s32 found = Func_02002dcc(Func_02001f7c);

        if (found == -1) {
            Func_02002e44();
            Func_02002edc(0);
            Func_02002eec(207);
            Ov686_Call2e24(0x20000, 0x20000, 0x10000);
            Battle_WaitMode0(10);
            Motion_CamBounds(0xe80000, -1, 0x1880000, 1);
            Func_02002eb4();
            Scheduler_AddOrUpdateCallback(Func_02001f7c, 0x480);
            Battle_WaitMode0(40);
            Object_AttachWorkTargetToObject(gPartyState.current_owner, 1);
            Func_02002eb4();
            Ov686_Call2dfc(13, 30, 3, 2, 13, z);
            Ov686_Call2dfc(14, 87, 1, 1, 14, 88);
            Ov686_FlagSet(381);
            Ov686_FlagSet(353);
            Func_02002e4c();
        }
    }
}

void Func_020021d4(void)
{
    s32 z = Object_GetById(9)->z >> 20;

    if (Ov686_FlagCheck(0x9dd) != 0 && z == 24) {
        /* FAKEMATCH: the callback pinned to r5 leaves the z cell in r6; plain, the two swap (differ 12). */
        register void (*callback)(void) asm("r5") = Func_02001f7c;

        if (Func_02002dcc(callback) == -1) {
            Ov686_Call2e24(0x20000, 0x20000, 0x10000);
            Scheduler_AddOrUpdateCallback(callback, 0x480);
            Ov686_Call2dfc(13, 30, 3, 2, 13, z);
            Ov686_Call2dfc(14, 87, 1, 1, 14, 88);
            Ov686_FlagSet(381);
            Ov686_FlagSet(353);
            Func_02002e4c();
        }
    }
}
