#include "types.h"

struct S {
    u8 pad0[0xFC];
    u8 unk_FC;
    u8 pad1[3];
    s16 a;
    s16 b;
};

struct D {
    u32 source;
    u32 destination;
    u32 control;
};

s32 Func_080042c8(s32);
void Func_080030f8(u32);
void Func_08012388(s32, s32);

void Func_08011590(void)
{
    void **p = (void **)0x03001E6C;
    u8 *base = (u8 *)*p++;
    struct S *work = (struct S *)*p;
    s32 flag;

    work->unk_FC = 1;
    Func_080042c8(0x0801179D);
    {
        struct D *dma;
        u32 source;
        u32 destination;
        u32 control;

        dma = (struct D *)0x040000D4;
        source = 0x06004000;
        destination = 0x0201C000;
        control = 0x84000800;
        dma->source = source;
        dma->destination = destination;
        dma->control = control;
    }
    Func_080030f8(1);
    flag = *(s32 *)0x03001E40 & 1;
    Func_08012388((s32)(base + flag * 5120 + 3200), 0x02010000);
    work->a = 200;
    work->b = 255;
    *(s32 *)0x03001CFC = 0x08011569;
}

void Func_0801161c(void)
{
    u32 value = 0x0501;
    u16 *reg = (u16 *)0x0400000A;
    struct D *dma;
    u32 source;
    u32 destination;
    u32 control;

    *reg = value;
    reg += 101;
    source = 0x02038000;
    destination = 0x06008000;
    control = 0x84002000;
    dma = (struct D *)reg;
    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}
