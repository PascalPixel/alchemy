#include "local.h"

void Func_02003ae4(s32 id, s32 base);

void Func_02003b2c(void)
{
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 0x100;
    if (GameFlag_Test(0x842)) {
        Func_02005230(8, 0, 0);
        Func_02005178_(0, 0, 1, 1, 5, 13);
    } else {
        Func_020052c0(8);
    }
    if (GameFlag_Test(0x849)) {
        Func_02005178_(2, 0, 2, 3, 17, 24);
        Func_02005230(9, 0, 0);
        Func_02005230(10, 0, 0);
        Func_02005238_(11, 0x1760000, 0x11a0000, 0);
        {
            /* FAKEMATCH: a temporary builds the angle ahead of the other arguments (three reordered rows as a plain argument). */
            s32 angle = 0xd000;

            Func_02005238_(12, 0x1100000, 0x1280000, angle);
        }
        {
            /* FAKEMATCH: a temporary builds the angle ahead of the other arguments (three reordered rows as a plain argument). */
            s32 angle = 0x3000;

            Func_02005238_(13, 0xd80000, 0x1080000, angle);
        }
    } else {
        Func_02003ae4(11, 0);
        Func_02003ae4(12, 240);
        Func_02003ae4(13, 480);
    }
    if (GameFlag_Test(0x212)) {
        Func_02005170(77, 33, 77, 24, 2, 2);
        Func_02005178_(32, 30, 2, 1, 17, 25);
    }
#if !defined(TLA_EDITION_JA) /* 日本版にはない */
    if (gPartyState.entrance == 3 || gPartyState.entrance == 4)
        GameFlag_ClearBit(0x12f);
#endif
}
