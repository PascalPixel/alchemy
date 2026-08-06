#include "types.h"

struct ParticleSpec_02004968 {
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
 * Frame-gated particle owner at 0x02004968, complete 196-byte span through
 * its sole return and two-word pool before 0x02004a2c.  The four independent
 * random draws, signed position spreads and eight-argument spawn are explicit.
 */

extern const u8 Data_020052cc[];

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly's five `bl`s to the
 * random/cue helpers are direct `bl sub_0200XXXX` relocations against
 * absolute .set symbols (verified via `arm-none-eabi-objdump -dr -M
 * force-thumb` on the assembled .o) -- lifted verbatim, not the
 * veneer-math final target names (Func_080000f8/Func_080f9010) this file
 * used before. Func_02000118 (a genuine same-overlay direct call, no
 * relocation) is unchanged.
 */
extern u32 Func_0200975c(void);
extern u32 Func_02009774(void);
extern u32 Func_0200978a(void);
extern u32 Func_020097a0(void);
extern void Func_020099a0(s32 cue);

extern void Func_02004b2a(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_02004968 *);
s32 Func_02004968(const s32 *position)
{
    struct ParticleSpec_02004968 spec;
    u32 frame = *(u32 *)0x03001e40;
    s32 x;
    s32 y;
    s32 z;
    s32 scatter;

    spec.kind = 1;
    spec.variant = 5;
    spec.field18 = 0x11e;
    spec.payload = Data_020052cc;

    if ((frame & 3) != 0)
        return 0;
    if ((frame & 7) == 0)
        Func_020099a0(0xf6);

    x = position[2] +
        ((s32)(((Func_0200975c() * 49u) >> 16) - 24) << 16);
    y = position[3] +
        ((s32)(((Func_02009774() * 49u) >> 16) - 24) << 16);
    z = position[4] +
        ((s32)(((Func_0200978a() * 49u) >> 16) - 24) << 16);
    scatter = 0x8000 + (s32)(((Func_020097a0() << 2) >> 16) << 15);

    Func_02004b2a(x, y, z, 0, scatter, 0, 0x00330000, &spec);
    return 0;
}
