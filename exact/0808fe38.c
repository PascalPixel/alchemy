#include "types.h"

struct State_0808fe38 {
    u8 padding000[0x528];
    u16 value528;
    u16 value52a;
    u8 padding52c[8];
    u16 value534;
    u16 value536;
    u8 padding538[8];
};

struct DmaChannel_0808fe38 {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32 kind, s32 size);
void Func_080041d8(void (*callback)(void), s32 period);
void Func_0808f52c(void);
void Func_0808f498(void);

void Func_0808fe38(s32 value)
{
    struct State_0808fe38 *state;
    u32 zero;
    u32 *zeroPtr;

    state = Func_080048f4(31, sizeof(*state));
    {
        volatile struct DmaChannel_0808fe38 *dma;

        zeroPtr = &zero;
        *zeroPtr = 0;
        dma = (volatile struct DmaChannel_0808fe38 *)0x040000d4;
        dma->source = zeroPtr;
        dma->destination = state;
        dma->control = 0x85000150;
    }

    state->value528 = value;
    state->value52a = 0;
    state->value534 = 0x3f3f;
    state->value536 = 1;

    Func_080041d8(Func_0808f52c, 3200);
    Func_080041d8(Func_0808f498, 1152);
}
