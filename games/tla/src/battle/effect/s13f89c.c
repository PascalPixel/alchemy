#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"

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
    volatile u16 *ime_reg;
    u32 ime;

    /* FAKEMATCH: the block that runs once is meaningless, as in IRQ_LOCK: a constant 0x208 store loads it from the literal pool where the ROM stores the address register (strh r2, [r2]). */
    do {
        ime_reg = &REG_IME;
        ime = *ime_reg;
    } while (0);
    *ime_reg = (u16)(u32)ime_reg;
    if (work->halve_pending == 1) {
        work->halve_pending = 0;
        /* FAKEMATCH: the native clears the flag before it restores IME and then loads the destination (differ 6 without the barrier). */
        __asm__("");
        REG_IME = ime;
        {
            void *destination = (void *)0x06004000;

            /* FAKEMATCH: the native loads the destination before it starts the size (differ 2 without the barrier). */
            __asm__ volatile("" : "+r"(destination));
            ColorBuffer_Halve(buffer, destination, 0x4000);
        }
    } else {
        REG_IME = ime;
    }
}
