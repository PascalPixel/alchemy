/* Near miss: differ 12 of 58, registers only: the native keeps the callback's
 * address in r5 and the z cell in r6; the draft has them the other way round.
 * Tried: a local for the callback. */
#include "../../../../games/tla/src/field/overlay_686/ov686.h"

void Func_020021d4(void)
{
    s32 z = Object_GetById(9)->z >> 20;

    if (Ov686_FlagChk(0x9dd) != 0 && z == 24) {
        if (Func_02002dcc(Func_02001f7c) == -1) {
            Ov686_Call2e24(0x20000, 0x20000, 0x10000);
            Scheduler_AddOrUpdateCallback(Func_02001f7c, 0x480);
            Ov686_Call2dfc(13, 30, 3, 2, 13, z);
            Ov686_Call2dfc(14, 87, 1, 1, 14, 88);
            Ov686_FlagSet(381);
            Ov686_FlagSet(353);
            Func_02002e4c();
        }
    }
}
