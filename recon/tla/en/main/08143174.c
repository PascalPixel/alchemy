/* Near miss, differ 10 of 106: at the three colour kernel calls the native loads the
   source (ldr r0, =gMapCellBuffer) after the destination and the size; here it loads
   first. The kernels' inline forwarders change nothing there. The rest matches with
   the devices of Func_08143114 (the IRQ_LOCK block, a barrier after each IME restore,
   the IWRAM forwarders) and a volatile read before the counter is set to 1, which is
   the native's dead load (ldr r2, [r3]; movs r2, #1; str r2, [r3]). */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 level, void *destination, s32 size);

static __inline__ void CopyWords_(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does. */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does. */
    Iwram_FillWords(destination, size, value);
}

static __inline__ void Halve_(void *source, void *destination, s32 size)
{
    /* FAKEMATCH: forwarding through the inline loads the source last, as the native does; called directly it loads first (differ 2 each). */
    ColorBuffer_Halve(source, destination, size);
}

static __inline__ void ScaleThreeQuarters_(void *source, void *destination, s32 size)
{
    /* FAKEMATCH: forwarding through the inline loads the source last, as the native does. */
    ColorBuffer_ScaleThreeQuarters(source, destination, size);
}

static __inline__ void Darken_(void *source, s32 level, void *destination, s32 size)
{
    /* FAKEMATCH: forwarding through the inline loads the source last, as the native does. */
    ColorBuffer_Darken(source, level, destination, size);
}

void Func_08143174(void)
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
    pending = (s32 *)(work + 0x78e8);
    if (*pending == 1) {
        *pending = 0;
        REG_IME = ime;
        /* FAKEMATCH: the native restores IME before it starts the next constant (differ 2 without the barrier). */
        __asm__ volatile("");
        switch (*(s32 *)(work + 0x7780)) {
        case 0:
            CopyWords_((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
            break;
        case 1:
            CopyWords_((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
            FillWords_(Ram_MapCellBuffer, 0x7800, *(u32 *)(work + 0x7784));
            break;
        case 2:
            if (*(s32 *)(work + 0x7784) == 50)
                Halve_(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
            else
                ScaleThreeQuarters_(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
            break;
        case 3:
            Darken_(Ram_MapCellBuffer, *(s32 *)(work + 0x7784), (void *)0x06008000, 0x7800);
            break;
        }
        *(volatile s32 *)(work + 0x78e4);
        *(volatile s32 *)(work + 0x78e4) = 1;
    } else {
        REG_IME = ime;
        /* FAKEMATCH: the native restores IME before it starts the next constant (differ 2 without the barrier). */
        __asm__ volatile("");
        (*(s32 *)(work + 0x78e4))++;
    }
}
