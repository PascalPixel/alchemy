#include "types.h"

struct DmaTransfer_08097540 {
    const void *source;
    void *destination;
    u32 control;
};

struct EffectState_08097540 {
    u8 data[0x28b];
    s8 color_r;
    s8 color_g;
    s8 color_b;
    u16 angle;
    u16 first;
    u16 second;
    u8 timer;
};

extern s32 Data_03001e40;

void *Func_080048f4(s32 kind, s32 size);
void Func_08097384(void);
s32 Func_08002304(s32 value, s32 modulus);
void Func_080978c4(void);
void Func_08091200(s32 value, s32 enabled);
void Func_08091254(s32 value);
void Func_08097a7c(void);
void Func_080041d8(const void *callback, s32 period);

void Func_08097540(s32 first, s32 second)
{
    struct EffectState_08097540 *effect;
    struct DmaTransfer_08097540 *dma;
    u32 zero;
    u32 *source;
    s32 color;

    effect = Func_080048f4(22, 166 << 2);
    Func_08097384();

    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_08097540 *)0x040000d4;
    dma->source = source;
    dma->destination = effect;
    dma->control = 0x850000a6;

    effect->angle = Func_08002304(Data_03001e40 << 1, 360);
    Func_080978c4();

    color = ((s32)effect->color_b << 10)
          | ((s32)effect->color_g << 5)
          | effect->color_r
          | 0x200000;
    Func_08091200(color, 1);
    Func_08091254(8);

    effect->first = first;
    effect->second = second;
    effect->timer = 8;
    Func_08097a7c();
    Func_080041d8((const void *)0x08097645, 0xc80);
}
