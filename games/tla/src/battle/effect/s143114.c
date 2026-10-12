#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"

static __inline__ void CopyWords_(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does: called directly, CSE shares one 0x1000 in r6 across the first call (differ 16). */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does. */
    Iwram_FillWords(destination, size, value);
}

void Func_08143114(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    volatile u16 *ime_reg;
    u32 ime;
    s32 *pending;

    /* FAKEMATCH: the block that runs once is meaningless, as in IRQ_LOCK: a constant 0x208 store loads it from the literal pool where the ROM stores the address register (strh r1, [r1]). */
    do {
        ime_reg = &REG_IME;
        ime = *ime_reg;
    } while (0);
    *ime_reg = (u16)(u32)ime_reg;
    pending = (s32 *)(work + 0x78ec);
    if (*pending == 1) {
        *pending = 0;
        REG_IME = ime;
        /* FAKEMATCH: the native restores IME before it builds the buffer's offset (differ 2 without the barrier). */
        __asm__ volatile("");
        CopyWords_(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
        FillWords_(work + 0x6780, 0x1000, 0);
    } else {
        REG_IME = ime;
    }
}
