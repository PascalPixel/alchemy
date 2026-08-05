#include "types.h"

struct EffectParams {
    s32 shape;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 rate1;
    s32 rate2;
    s32 id;
    s32 selector;
    u32 step;
    void (*callback)(u8 *particle);
};

extern void Func_0808a018(void);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_08009128(void);
extern void Func_080000c0(s32 frames);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 cue);
extern void Func_0808a010(s32 actor);
extern s32 Func_08000120(s32 angle);
extern s32 Func_08000118(s32 angle);
extern void Func_0200013c(s32 x, s32 y, s32 z,
                          s32 velocityX, s32 velocityY, s32 velocityZ,
                          u32 flags, struct EffectParams *params);
extern void Func_0808a1f0(s32 actor, s32 pose);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern void Func_080091f8(void);
extern void Func_0808a020(void);
extern void Func_0200185c(u8 *particle);

/* Stage the actor entrance and emit a seventeen-particle orbit around it. */
void Func_020018b8(void)
{
    struct EffectParams params;
    u8 *actor;
    s32 radial[3];
    u32 step;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_08009128();
    Func_080000c0(1);

    actor = Func_0808a080(0);
    *(s32 *)(actor + 12) = 0x00820000;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 72) = 0x00008000;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 68) = 0;
    actor = Func_0808a080(0);
    actor[85] = 0;

    Func_0808a360();
    Func_0808a370();
    Func_080f9010(204);
    actor = Func_0808a080(0);
    actor[85] = 3;
    Func_0808a010(24);

    actor = Func_0808a080(0);
    params.mode = 7;
    params.color1 = 0xcccc;
    params.color2 = 0xcccc;
    params.callback = Func_0200185c;

    for (step = 0; step <= 16; step++) {
        radial[0] = Func_08000120(step << 12);
        radial[1] = 0;
        radial[2] = Func_08000118(step << 12);
        radial[0] += radial[0] / 2;

        Func_0200013c(*(s32 *)(actor + 8),
                      *(s32 *)(actor + 12),
                      *(s32 *)(actor + 16),
                      radial[0], radial[1], radial[2],
                      0x01090001, &params);
    }

    Func_080f9010(188);
    Func_0808a1f0(0, 0x101);
    Func_0808a100(0, 22);
    Func_080091f0(0x50000, 0x50000, 0x10000);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a1f0(0, 0x100);

    actor = Func_0808a080(0);
    *(s32 *)(actor + 72) = 0x10000;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 68) = 0x4000;
    Func_0808a020();
}
