#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"

void BattleFx_PackTargetBuffer(void)
{
    struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
    u16 *packed = work->palette;
    s32 blue = 0x7c00;
    u16 *current;
    s32 i;

    i = 0x1c0;
    current = work->target;
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | ((current[1] >> 5) & 0x3e0) | ((current[2] >> 10) & 0x1f);
        current += 3;
    }
}
