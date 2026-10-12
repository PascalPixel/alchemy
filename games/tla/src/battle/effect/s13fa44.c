#include "types.h"
#include "ram_buffer.h"

struct BattleFxFadeWork {
    s32 unknown_00;
    s32 halve_pending;
    s32 frames_left;
    s32 step;
};

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale, s32 count);

void Func_0813fa44(void)
{
    struct BattleFxFadeWork *work = (struct BattleFxFadeWork *)Ram_HeapSlots->battle_fx_work[0];
    u8 *battle = (u8 *)Ram_HeapSlots->battle_work;

    if (work->frames_left > 0) {
        work->step++;
        ColorBuffer_ScaleFar(battle + 0x56c, (void *)0x050000c0, 0x10000 - work->step * 1092, 128);
        work->frames_left--;
    }
}
