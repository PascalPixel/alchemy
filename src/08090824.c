#include "types.h"

struct DmaTransfer_08090824 {
    const void *source;
    void *destination;
    u32 control;
};

struct EffectState_08090824 {
    u8 data[0x528];
    s16 value;
    s16 timer;
};

void *Func_080048f4(s32, s32);
void Func_080907b0(s32);
void Func_080041d8(void (*)(void), s32);
void Func_080030f8(u32);
void Func_08090658(void);

void Func_08090824(s32 value)
{
    struct EffectState_08090824 *state;
    struct DmaTransfer_08090824 *dma;
    u32 *source;
    u32 zero;

    state = Func_080048f4(0x1f, 0x540);
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_08090824 *)0x040000d4;
    dma->source = source;
    dma->destination = state;
    dma->control = 0x85000150;
    Func_080907b0(0);
    state->value = value;
    state->timer = 0;
    Func_080041d8(Func_08090658, 0xc80);
    Func_080030f8(0x78);
}
