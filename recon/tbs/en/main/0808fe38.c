/* NONMATCHING: 11 differing halfwords. The halfword constants pass through
 * ints so they load as words; the remaining difference is which low
 * register reload gives the 0x528 offset (r1 here, r2 in the ROM), which
 * turns the 0x52a and 0x534 offsets around. Pinning the 0x3f3f temporary
 * to r3 with a register hint reaches 8. */
#include "TYPES.H"
#include "DMA.H"

#define DisplayTransition_InitializeState Func_0808fe38

void DisplayTransition_InitializeState(s32 mode)
{
    u8 *pool = Func_080048f4(31, 0x540);
    volatile s32 zero;
    s32 value;
    s32 one;

    zero = 0;
    Dma_Set((const void *)&zero, pool, 0x85000150,
            (volatile u32 *)0x040000d4);
    *(s16 *)(pool + 0x528) = mode;
    *(s16 *)(pool + 0x52a) = 0;
    value = 0x3f3f;
    *(s16 *)(pool + 0x534) = value;
    one = 1;
    *(s16 *)(pool + 0x536) = one;
    Func_080041d8((void *)0x0808f52d, 0xc80);
    Func_080041d8((void *)0x0808f499, 0x480);
}
