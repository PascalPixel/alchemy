#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "irqlock.h"

struct BattleFxFadeWork {
    s32 unknown_00;
    s32 halve_pending;
    s32 frames_left;
    s32 step;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);

void Func_0813f89c(void)
{
    struct BattleFxFadeWork *work = (struct BattleFxFadeWork *)Ram_HeapSlots->battle_fx_work[0];
    void *buffer = Ram_HeapSlots->battle_fx_work[1];
    u32 ime;

    IRQ_LOCK(ime);
    if (work->halve_pending == 1) {
        work->halve_pending = 0;
        IRQ_UNLOCK(ime);
        {
            void *destination = (void *)0x06004000;

            /* FAKEMATCH: the native loads the destination before it starts the size (differ 2 without the barrier). */
            __asm__ volatile("" : "+r"(destination));
            ColorBuffer_Halve(buffer, destination, 0x4000);
        }
    } else {
        IRQ_UNLOCK(ime);
    }
}
