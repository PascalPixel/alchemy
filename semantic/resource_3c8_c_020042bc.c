#include "types.h"

struct ParticleSpec_020042bc {
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
 * Dual particle-wave scene owner at 0x020042bc, 612 bytes through its sole
 * return and four-word pool before 0x02004520.  Both ten-wave passes, the
 * fourfold repeat at wave three, all spawn parameters and 27 ordered calls
 * are expressed as ordinary C.
 */

extern void Func_02000118(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_020042bc *);
extern void Func_080000c0(s32 frames);
extern u32 Func_080000f8(void);
extern void Func_08009180();
extern void Func_080091e0(void *record, s32 mode);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a158(s32 actor, s32 value);
extern void Func_0808a248(s32 actor);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a5e0(s32 cue);
extern void Func_080f9010(s32 cue);

void Func_020042bc(void)
{
    struct ParticleSpec_020042bc spec;
    u8 *state = *(u8 **)0x03001ebc;
    u32 wave;
    s32 repeat = 0;

    *(s32 *)(state + 0x1c0) = 0x202;
    Func_0808a018();
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a158(0, 15);
    Func_0808a5e0(0xaa);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_080f9010(0xa2);

    for (wave = 0; wave <= 9; wave++) {
        u32 inner;

        spec.spread_x =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.spread_y =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);
repeat_first_wave:
        if (wave <= 7) {
            for (inner = 0; inner < 4; inner++) {
                s32 x = 0x03600000 +
                    (s32)(((Func_080000f8() * 7u) >> 16) << 19);
                s32 z = 0x00300000 + wave * 0x00400000 +
                    inner * 0x00040000;

                Func_02000118(x, 0, z, 0, 0, 0, 0x00880000, &spec);
            }
        }
        Func_080000c0(3);
        if (wave == 3 && repeat <= 2) {
            repeat++;
            goto repeat_first_wave;
        }
        Func_08009180(48, wave + 3, 54, wave + 3, 3, 1);
    }

    Func_08009180(111, 5, 117, 5, 5, 2);
    Func_08009180(111, 10, 117, 10, 5, 2);
    Func_08009180(111, 7, 111, 5, 5, 2);
    Func_08009180(111, 7, 111, 10, 5, 2);

    for (wave = 0; wave <= 9; wave++) {
        u32 inner;

        spec.spread_x =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.spread_y =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);
        if (wave <= 7) {
            for (inner = 0; inner < 4; inner++) {
                s32 x = 0x03000000 +
                    (s32)(((Func_080000f8() * 7u) >> 16) << 19);
                s32 z = 0x00300000 + wave * 0x00400000 +
                    inner * 0x00040000;

                Func_02000118(x, 0, z, 0, 0, 0, 0x00880000, &spec);
            }
        }
        Func_080000c0(3);
        Func_08009180(55, wave + 26, 48, wave + 3, 3, 1);
    }

    Func_080f9010(0x121);
    Func_0808a010(60);
    Func_0808a248(21);
    Func_0808a020();
}
