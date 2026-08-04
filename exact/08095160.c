#include "types.h"

struct DmaTransfer_08095160 {
    const void *source;
    void *destination;
    u32 control;
};

struct EffectState_08095160 {
    u8 data[0x1f80];
    u16 timer;
    u16 enabled;
};

extern u8 *Data_03001ed0;

void *Func_080048f4(s32 kind, s32 size);
s32 Func_08090a5c(s32 kind, s32 source, s32 destination, s32 enabled);
void Func_0809088c(s16 *first, s16 *second, s16 *output, s32 count);
s32 Func_080041d8(void (*callback)(void), s32 period);
void Func_080949a8(void);

void Func_08095160(void)
{
    struct EffectState_08095160 *effect;
    u8 *display;
    u32 zero;
    u32 *source;
    void *destination;
    struct DmaTransfer_08095160 *dma;

    destination = Func_080048f4(30, 0x1f88);
    effect = destination;
    display = Data_03001ed0;

    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_08095160 *)0x040000d4;
    dma->source = source;
    dma->destination = destination;
    dma->control = 0x850007e2;

    Func_08090a5c(0x10003, (s32)display, (s32)effect, 1);
    Func_08090a5c(0x10005, (s32)display, (s32)effect + 0xa80, 1);
    Func_0809088c((s16 *)((u8 *)effect + 0xa80),
                  (s16 *)effect,
                  (s16 *)((u8 *)effect + 0x1500),
                  12);
    Func_08090a5c((s32)effect, 0, (s32)display + 0xe00, 1);

    effect->timer = 0x258;
    effect->enabled = 1;
    Func_080041d8(Func_080949a8, 0xc80);
}
