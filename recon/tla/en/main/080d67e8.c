/* Near miss, differ 4 of 110: The Broken Seal twin (BattleFx_UpdateStormFlash). After the second Random16
 * the native interleaves the two products as a*3, 100, <<3, muls, +a, <<4 (movs r2, #100 one step later
 * and muls one step later than here). In sched2 the multiply has cost 4, which gives the 100 priority 15,
 * level with the first shift; the native order is what a multiply cost of 3 would give. Signed operands
 * are the same differ 4; a barrier after a*25 changes the allocation of the offset constants (differ 58). */
#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"
#include "system.h"

void BattleFx_ApplyColorToTargetBuffer(void *buffer, s32 mode);

s32 GameFlag_Test(s32 flag);
void Audio_PlayCue(s32 cue);

// Lightning: selects the palette according to the remaining duration.
void BattleFx_UpdateStormFlash(void)
{
    struct FieldBlendWork *work = Ram_HeapSlots->unk70[2];
    struct BattleEffectBuffers *target = Ram_HeapSlots->block32;
    s16 *timer = &work->timer;

    if (*timer < 0)
        return;
    if (GameFlag_Test(358))
        work->timer = 128;
    switch ((*timer)--) {
    case 0:
        if (work->enabled != 0) {
            u32 a = Random16();
            u32 b = Random16();
            work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
            if (work->loud != 0)
                Audio_PlayCue(172);
            else
                Audio_PlayCue(171);
        }
    case 5:
    case 10:
        BattleFx_ApplyColorToTargetBuffer(work, 1);
        Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
        target->duration = 12;
        target->step = 0;
        break;
    case 1:
    case 6:
    case 11:
        BattleFx_ApplyColorToTargetBuffer(work->to, 1);
        target->duration = 1;
        target->step = 0;
        break;
    }
}
