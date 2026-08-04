#include "types.h"

typedef void (*DecodeResource_080c08ec)(
    const void *source,
    void *destination,
    void *workspace);

typedef void (*FillOrCopy_080c08ec)(
    void *destination,
    u32 size,
    u32 source_or_fill);

struct Dma_080c08ec {
    const void *source;
    void *destination;
    u32 control;
};

struct RuntimeCallbacks_080c08ec {
    void *state;
    u8 unknown_04[16];
    DecodeResource_080c08ec decode_resource;
};

struct EffectState_080c08ec {
    u8 unknown_00[8];
    s32 mode;
};

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080041d8(const void *, u32);
void *Func_080048b0(s32, s32);
void Func_080c0098(void *);
void Func_080c00d8(void *);
void Func_080c1724(void *, void *, s32, s32);

void Func_080c08ec(s32 mode, s32 resource_index, s32 fade)
{
    struct RuntimeCallbacks_080c08ec *callbacks =
        (struct RuntimeCallbacks_080c08ec *)0x03001f00;
    struct EffectState_080c08ec *state =
        (struct EffectState_080c08ec *)callbacks->state;
    u8 *base = *(u8 **)0x03001e74;
    volatile struct Dma_080c08ec *dma =
        (volatile struct Dma_080c08ec *)0x040000d4;
    FillOrCopy_080c08ec fill_or_copy =
        (FillOrCopy_080c08ec)0x03000164;
    u8 *resource = Func_08002f40(resource_index);
    void *allocation = Func_080048b0(49, 0x230);

    dma->source = (const void *)0x080b5138;
    dma->destination = allocation;
    dma->control = 0x8400008c;

    callbacks->decode_resource(
        resource + 0x100,
        (void *)0x06008000,
        callbacks);
    Func_08002dd8(49);

    dma->source = resource;
    dma->destination = base + 0x544;
    dma->control = 0x84000040;

    if (fade >= 0) {
        s32 scale = 0x10000 - fade * 0x444;

        *(s32 *)(base + 0x644) = scale;
        Func_080c1724(base + 0x544, (void *)0x050000c0, scale, 128);
    }

    dma->source = (const void *)0x05000200;
    dma->destination = (void *)0x050000a0;
    dma->control = 0x80000010;
    *(volatile u16 *)0x050000bc = *(volatile u16 *)0x050001e8;

    Func_080c0098((void *)0x06003800);
    Func_080c00d8((void *)0x0600f800);
    fill_or_copy((void *)0x0600ffc0, 0x40, 0x00020002);

    if (state->mode == 0)
        Func_080041d8((const void *)0x080c0131, 0x4ff);
    state->mode = mode;
    if (mode == 1)
        *(volatile u16 *)0x0400000a = 0x1f83;
}
