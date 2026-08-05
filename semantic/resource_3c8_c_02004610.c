#include "types.h"

struct ParticleSpec_02004610 {
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

typedef void (*Task_02004610)(void);

/*
 * First particle-wave scene owner at 0x02004610, 432 bytes through its sole
 * return and five-word pool before 0x020047c0.  The nested 13-wave/four-spawn
 * choreography, four repeats at wave three and all 27 ordered call sites are
 * explicit ordinary C.
 */

extern void Func_02000118(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_02004610 *);
extern void Func_020045f0(void);
extern void Func_080000c0(s32 frames);
extern void Func_080000d0(Task_02004610 task, s32 priority);
extern u32 Func_080000f8(void);
extern void Func_08009180();
extern void Func_080091e0(void *record, s32 mode);
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a158(s32 actor, s32 value);
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218(void);
extern void Func_0808a248(s32 actor);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a5e0(s32 cue);
extern void Func_080f9010(s32 cue);

void Func_02004610(void)
{
    struct ParticleSpec_02004610 spec;
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
    Func_0808a208(0x8000, 0x1000);
    Func_0808a210(0x01b80000, -1, 0x01680000, 1);

    for (wave = 0; wave <= 12; wave++) {
        u32 inner;

        spec.spread_x =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.spread_y =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);

repeat_wave:
        for (inner = 0; inner < 4; inner++) {
            s32 x = 0x01a00000 +
                (s32)(((Func_080000f8() * 7u) >> 16) << 19);
            s32 z = 0x00c00000 + wave * 0x00400000 + inner * 0x00040000;

            Func_02000118(x, 0, z, 0, 0, 0, 0x00880000, &spec);
        }
        Func_080000c0(3);

        if (wave == 3 && repeat <= 2) {
            repeat++;
            goto repeat_wave;
        }
        if (wave == 3 && repeat == 3)
            Func_080000d0(Func_020045f0, 200 << 4);

        Func_08009180(53, wave + 12, 26, wave + 12, 3, 1);
    }

    Func_08009180(81, 41, 89, 14, 9, 2);
    Func_0808a218();
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a010(60);
    Func_080770c8(0x306);
    Func_0808a248(19);
    Func_0808a020();
}
