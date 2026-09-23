/* Draft, not exact: 70 differing halfwords, 204-byte candidate for the
   208-byte owner (2026-09-23). Residual: two local differences in the remap
   loop: the reference compares a copy of the loaded map entry and, when the
   palette is full, emits the stored count rather than a reload. Every
   readable form that expresses both (u8 colour, entry re-read after the if)
   makes loop optimisation hoist 0x100 and 0x05000000 and spill.
   UiText_LoadRemappedGlyph: glyph palette map, resource 0xf1 glyphs, byte-LZ
   decode, 1024-byte remap, DMA to 0x06004000 + tile * 64. */

#include "DMA.H"
#include "types.h"

#define Function Func_08021cb8

extern u8 Data_000000f1[];
void Func_08002dd8();
void Func_08002df0();
s32 Func_08002f40();
s32 Func_080048b0();
s32 Func_08004938();
void Func_080053e8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void Function(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 rec7;
    s32 rec8;
    s32 record;
    u8 *v5;
    u8 *v6;
    s32 v12;
    s32 v2;
    u8 *p4;

    p8 = a2;
    rec8 = Value2(Func_080048b0, 17, 0x608);
    record = Func_08002f40((s32)Data_000000f1);
    *(s32 *)((rec8 + 0x604)) = (record + *(u16 *)((a1 << 1) + record));
    Func_080053e8((record + *(u16 *)((a1 << 1) + record)), rec8);
    rec7 = Value1(Func_08004938, 0x400);
    v5 = rec7;
    v6 = rec8;
    v12 = 0;
    do {
        p4 = v6[0];
        v2 = *(u8 *)(a0 + (s32)p4);
        v6 = ((s32)v6 + 1);
        if (*(u8 *)(a0 + (s32)p4) == 255) {
            *(u8 *)(a0 + (s32)p4) = *(s32 *)((a0 + 0x100));
            if (*(s32 *)((a0 + 0x100)) <= 63) {
                p10 = (0x5000000 + (*(s32 *)((a0 + 0x100)) << 1));
                *(u16 *)p10 = *(u16 *)(((s32)((s32)p4 << 1) + 0x5000200));
                *(s32 *)((a0 + 0x100)) += 1;
                v2 = *(u8 *)(a0 + (s32)p4);
            } else {
                v2 = *(s32 *)((a0 + 0x100));
            }
        }
        v5[0] = v2;
        v12 = (v12 + 1);
        v5 = ((s32)v5 + 1);
    } while (v12 < 0x400);
    Dma_Set((const void *)(rec7), (void *)(((s32)((s32)p8 << 6) + 0x6004000)), -0x7bffff00, (volatile u32 *)(0x40000d4));
    Func_08002df0(rec7);
    Func_08002dd8(17);
    p10b = (s32)v5;
}
