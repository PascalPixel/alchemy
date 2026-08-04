typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct ParticleSpec_020047c0 {
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

typedef void (*Task_020047c0)(void);

/*
 * Second particle-wave scene owner at 0x020047c0, 424 bytes through its sole
 * return and five-word pool before 0x02004968.  It shares the reviewed control
 * shape of 0x4610 while preserving its distinct launch base, per-wave task,
 * dialogue coordinates, flag and final actor.
 */

extern void Func_02000118(s32, s32, s32, s32, s32, s32, s32,
                          struct ParticleSpec_020047c0 *);
extern void Func_02004600(void);
extern void Func_080000c0(s32 frames);
extern void Func_080000d0(Task_020047c0 task, s32 priority);
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

void Func_020047c0(void)
{
    struct ParticleSpec_020047c0 spec;
    u8 *state = *(u8 **)0x03001ebc;
    s32 wave;
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
    Func_0808a210(0x02380000, -1, 0x01680000, 1);

    for (wave = 0; wave <= 12; wave++) {
        s32 inner;

        spec.spread_x =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.spread_y =
            (s32)(((Func_080000f8() << 1) >> 16) * 0x4ccc) + 0x17ffc;
        spec.field22 = (u16)(((Func_080000f8() << 12) >> 16) + 0xf800);

repeat_wave:
        for (inner = 0; inner < 4; inner++) {
            s32 x = 0x02200000 +
                (s32)(((Func_080000f8() * 7u) >> 16) << 19);
            s32 z = 0x00c00000 + wave * 0x00400000 + inner * 0x00040000;

            Func_02000118(x, 0, z, 0, 0, 0, 0x00880000, &spec);
        }
        Func_080000c0(3);

        if (wave == 3 && repeat <= 2) {
            repeat++;
            goto repeat_wave;
        }
        Func_080000d0(Func_02004600, 200 << 4);
        Func_08009180(58, wave + 12, 34, wave + 12, 3, 1);
    }

    Func_08009180(86, 41, 97, 14, 5, 2);
    Func_0808a218();
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a010(60);
    Func_080770c8(0x307);
    Func_0808a248(20);
    Func_0808a020();
}
