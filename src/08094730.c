#include "types.h"

struct DmaTransfer_08094730 {
    const void *source;
    void *destination;
    u32 control;
};

struct State_08094730 {
    u8 padding000[0xf01];
    u8 mode;
    u8 paddingf02[6];
    s32 first;
    s32 fourth;
    s32 second;
    s32 fifth;
    s32 third;
    s32 sixth;
};

void *Func_080048f4(s32 id, s32 size);
s32 Func_080041d8(void (*callback)(void), s32 priority);
void Func_08094544(void);
void Func_080944ec(void);

void Func_08094730(s32 mode, s32 first, s32 second, s32 third,
                   s32 fourth, s32 fifth, s32 sixth)
{
    struct DmaTransfer_08094730 *dma;
    volatile struct DmaTransfer_08094730 *dmaStatus;
    struct State_08094730 *state;
    u32 *source;
    u32 zero;
    s32 priority;

    state = Func_080048f4(0x22, 0xf20);

    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_08094730 *)0x040000d4;
    dma->source = source;
    dma->destination = state;
    dma->control = 0x850003c8;

    dmaStatus = (volatile struct DmaTransfer_08094730 *)0x040000d4;
    while ((dmaStatus->control & 0x80000000) != 0) {
    }

    state->mode = mode;
    state->first = first;
    state->fourth = fourth;
    state->third = third;
    state->sixth = sixth;
    state->second = second;
    priority = 200;
    state->fifth = fifth;
    priority <<= 4;
    Func_080041d8(Func_08094544, priority);
    priority = 144;
    priority <<= 3;
    Func_080041d8(Func_080944ec, priority);
}
