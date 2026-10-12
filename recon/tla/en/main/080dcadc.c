/* Draft, differ 2 of 33: after the last palette store the native loads the
 * party state's address and then shifts the first glow offset (ldr r3;
 * lsls r1, r1, #1); this shifts first. In the sched2 dump both are ready
 * with equal priority and the load, which reuses the store's r3, ranks
 * below. Measured: two scalar fields in place of the array elements are
 * the same differ 2; a pointer to the party state behind an empty asm
 * barrier cannot work, as the native starts the offset (movs r1, #147)
 * four instructions before the load; four minutes of permuter found
 * nothing better. Twin of The Broken Seal's
 * Ui_SetBank15PaletteAndClearRenderMode. */
#include "types.h"
#include "callback_scheduler.h"
#include "party_state.h"
#include "ram_buffer.h"

#define BG_PLTT_COLOR(bank, color) (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))

void Func_080dcdc8(void);
void Func_08038348(s32 first, s32 second);

// 走査線スクロールを止め、BG パレット 15 を戻す
void Func_080dcadc(void)
{
    u8 *work = Ram_HeapSlots->window_tiles;

    Scheduler_RemoveCallback((SchedulerCallback)Func_080dcdc8);
    BG_PLTT_COLOR(15, 1) = 0x7fff;  // 白
    BG_PLTT_COLOR(15, 3) = 0;       // 黒
    BG_PLTT_COLOR(15, 11) = 0x294a; // 灰 (10)
    BG_PLTT_COLOR(15, 12) = 0x5294; // 灰 (20)
    Func_08038348(gPartyState.unknown_220[5], gPartyState.unknown_220[6]);
    work[4] = 0;
}
