#include "types.h"

struct DmaTransfer_0801d014 {
    const void *source;
    void *destination;
    u32 control;
};

struct Settings_0801d014 {
    u8 padding000[0x205];
    u8 first;
    u8 second;
    u8 padding207[3];
    u8 fourth;
    u8 padding20b[1];
    u8 third;
    u8 padding20d[0x1d];
    u8 fifth;
};

struct State_0801d014 {
    u8 padding000[0x594];
    u8 values[5];
    u8 defaults[5];
};

void *Func_080048f4(s32 kind, s32 size);
void Func_080041d8(const void *callback, s32 period);

void Func_0801d014(void)
{
    struct State_0801d014 *state;
    u32 zero;
    u32 *source;
    volatile struct DmaTransfer_0801d014 *dma;

    state = Func_080048f4(20, 0x628);
    source = &zero;
    *source = 0;
    dma = (volatile struct DmaTransfer_0801d014 *)0x040000d4;
    dma->source = source;
    dma->destination = state;
    dma->control = 0x8500018a;

    state->values[0] =
        ((const struct Settings_0801d014 *)0x02000240)->first;
    state->defaults[0] = 24;
    state->values[1] =
        ((const struct Settings_0801d014 *)0x02000240)->second;
    state->defaults[1] = 15;
    state->values[2] =
        ((const struct Settings_0801d014 *)0x02000240)->third;
    state->defaults[2] = 3;
    state->values[3] =
        ((const struct Settings_0801d014 *)0x02000240)->fourth;
    state->defaults[3] = 2;
    state->values[4] =
        ((const struct Settings_0801d014 *)0x02000240)->fifth;
    state->defaults[4] = 2;

    Func_080041d8((const void *)0x0801cf49, 0xc80);
}
