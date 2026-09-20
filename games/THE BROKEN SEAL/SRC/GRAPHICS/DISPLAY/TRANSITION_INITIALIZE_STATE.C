#include "DMA.H"

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
};

void *Func_080048f4(s32, s32);
void Func_080907b0(s32);
void Func_080041d8(void (*)(void), s32);
void Func_080030f8(u32);
void Func_08090658(void);

#define DisplayTransition_InitializeState Func_08090824

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Func_080048f4(0x1f, 0x540);
    zero = 0;
    Dma_Set(&zero, state, 0x85000150, (volatile u32 *)0x040000d4);
    Func_080907b0(0);
    state->value = value;
    state->timer = 0;
    Func_080041d8(Func_08090658, 0xc80);
    Func_080030f8(0x78);
}
