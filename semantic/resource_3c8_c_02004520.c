#include "types.h"

struct ParticleSpec_02004520 {
    u32 kind;
    u32 variant;
    s32 spread_x;
    s32 spread_y;
    u8 padding10[8];
    u16 field18;
    u8 padding1a[2];
    const void *payload;
    u16 padding20;
    u16 field22;
};

/*
 * Frame-gated split-pattern particle owner at 0x02004520, complete 208-byte
 * span through its sole return and three-word pool before 0x020045f0.  Both
 * random branches and their complete eight-argument spawn calls are explicit.
 */

extern void Func_02000118(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_02004520 *);
extern s32 Func_03000380(s32 value, s32 divisor);
extern u32 Func_080000f8(void);

void Func_02004520(s32 base, s32 lane)
{
    struct ParticleSpec_02004520 spec;
    u32 frame = *(u32 *)0x03001e40;
    u32 branch;

    spec.spread_y = 0xb333;
    spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);
    spec.spread_x = 0xb333;

    if ((frame & 3) != 0)
        return;

    branch = (Func_080000f8() << 1) >> 16;
    if (branch != 0) {
        u32 horizontal = Func_080000f8();
        s32 x = (base + (s32)(((horizontal << 1) >> 16) << 4)) << 16;
        s32 vertical = (s32)(((Func_080000f8() * 5u) >> 16) << 16) +
            0x00070000;
        s32 divided = Func_03000380(vertical, 10);

        Func_02000118(x, 0, lane << 19, 0,
                      0, divided, 0x00880000, &spec);
    } else {
        s32 offset = (s32)((Func_080000f8() * 17u) >> 16);
        s32 x = (base + offset) << 16;
        s32 z = (base << 19) - 0x00040000;

        Func_02000118(x, 0, z, 0,
                      0, 0, 0x00880000, &spec);
    }
}
