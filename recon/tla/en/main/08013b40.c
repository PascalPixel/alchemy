#include "types.h"
#include "system.h"
#include "frame.h"
#include "io_reg.h"

static inline void Reg_Write16(u32 value, volatile u16 *reg)
{
    /* FAKEMATCH: through the inline the value stays a word, built by movs, lsls, adds as the native builds 0xc3ff; a halfword constant store loads it from the pool (three rows). */
    *reg = value;
}

void Input_HandleKeyInterrupt(void)
{
    if (gSleepActive == 0) {
        volatile u16 *keycnt = &REG_KEYCNT;

        /* FAKEMATCH: the native loads the register address before it builds the value (one reordered row). */
        __asm__ volatile("" : "+l"(keycnt));
        Reg_Write16(0xc3ff, keycnt);
        gResetRequested = 1;
    }
}
