/* NONMATCHING: 120 bytes, 11 differing halfwords.
 * WALL: reload assigns the 0x528 offset to r1 instead of r2.
 * Typed state fields preserve behavior but leave the same allocation. */
#include "TYPES.H"
#include "DMA.H"

struct DisplayTransitionState {
 u8 data[0x528];
 s16 mode;
 s16 timer;
 u8 gap[8];
 s16 mask;
 s16 active;
};

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
    ((struct DisplayTransitionState *)pool)->mode = mode;
    ((struct DisplayTransitionState *)pool)->timer = 0;
    value = 0x3f3f;
    ((struct DisplayTransitionState *)pool)->mask = value;
    one = 1;
    ((struct DisplayTransitionState *)pool)->active = one;
    Func_080041d8((void *)0x0808f52d, 0xc80);
    Func_080041d8((void *)0x0808f499, 0x480);
}
