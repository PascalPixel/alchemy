#include "TYPES.H"
#include "DMA.H"

#define DisplayTransition_InitializeState Func_0808fe38

void DisplayTransition_InitializeState(s16 mode)
{
    u8 *pool = Func_080048f4(31, 0x540);
    s32 zero = 0;

    Dma_Set((const void *)&zero, pool, 0x85000150,
            (volatile u32 *)0x040000d4);
    *(s16 *)(pool + 0x528) = mode;
    *(s16 *)(pool + 0x52a) = 0;
    *(s16 *)(pool + 0x534) = 0x3f3f;
    *(s16 *)(pool + 0x536) = 1;
    Func_080041d8((void *)0x0808f52d, 0xc80);
    Func_080041d8((void *)0x0808f499, 0x480);
}
