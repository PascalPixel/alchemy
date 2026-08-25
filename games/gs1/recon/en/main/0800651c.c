#include "types.h"

#define BattleLink_ResetTransferState Func_0800651c

void BattleLink_ResetTransferState(void)
{
    u16 interrupt_enable;

    interrupt_enable = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x04000208;
    *((s8 *)0x02002220 + 1) = 0x80;
    *(s32 *)0x02002080 = 0;
    *(s16 *)0x02002008 = 0;
    *(s32 *)0x020023ac = 0;
    *((s8 *)0x02002220 + 3) = 0;
    *((s8 *)0x02002220 + 2) = 0;
    *(s16 *)0x02002238 = 0;
    *(u16 *)0x04000208 = interrupt_enable;
}
