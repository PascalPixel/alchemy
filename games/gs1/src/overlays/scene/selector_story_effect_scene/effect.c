#include "types.h"

#define SceneEffect_SpawnParticleRowsByMode Func_02002030
#define SceneEffect_SpawnParticleRowsAndDrawTiles Func_02003860
#define SceneEffect_SpawnParticleEveryFourthFrame Func_02003a64
#define SceneEffect_SpawnRandomEveryFourFrames Func_02003b24
#define SceneEffect_SpawnWithRandomOffset Func_02003bd4
#define SceneEffect_AdvanceAnchoredRiseFrame Func_020051b0
/*
 * resource_39c owner at 0x020051b0, 64 bytes.
 * Advances a frame counter and, while it is still within 31, reseeds the two
 * accumulators from the counter and re-anchors the position on the src
 * record. Returns whether the step ran.
 */
struct Src_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct Src_39c *f104;       /* +104 */
};

extern u32 Data_03001e40;

void Func_02007a56(s32, s32, s32, s32, s32, s32);
void Func_02007a66(s32, s32, s32, s32, s32, s32);
void Func_02007a76_a(s32, s32, s32, s32, s32, s32);
u32 Func_02007a76_b(void);
void Func_02007cdc(s32);
u32 Func_02007a3e(void);
u32 Func_02007aac(void);
void Func_02002214(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_0200224e(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_02002288(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_02007bfe(s32);
void Func_02007b74(s32, s32, s32, s32, s32, s32);
void Func_02007b8c(s32, s32, s32, s32, s32, s32);
void Func_02007ba2(s32, s32, s32, s32, s32, s32);
void Func_02009280(s32, s32, s32, s32, s32, s32);
u32 Func_0200923a(void);
u32 Func_0200924e(void);
void Func_02003a32(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_020093a8(s32);
void Func_0200931a(s32, s32, s32, s32, s32, s32);
void Func_02009330(s32, s32, s32, s32, s32, s32);
s32 *Func_02009548(s32);
u32 Func_02009426(void);
u32 Func_02009440(void);
u32 Func_02009452(void);
u32 Func_02009466(void);
void Func_02003c40(s32, s32, s32, s32, s32, s32, s32, s32 *);
u32 Func_020094e8(void);
u32 Func_02009502(void);
u32 Func_02009516(void);
void Func_02003cee(s32, s32, s32, s32, s32, s32, s32, s32 *);
u32 Func_0200958a(void);
u32 Func_0200959a(void);
void Func_02003d68(s32, s32, s32, s32, s32, s32, s32, s32 *);
s32 Func_0200ab74();

void SceneEffect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Func_02007a56(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Func_02007a66(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Func_02007a76_a(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Func_02007cdc(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Func_02002214((0x319 - ((Func_02007a3e() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Func_0200224e((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Func_02007a76_b)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Func_02002288(0x3380000 - (i << 17) - (j << 19), 0,
                                  (((Func_02007aac() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Func_02007bfe(1);
            }
        }
        if (mode == 0)
            Func_02007b74(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Func_02007b8c(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Func_02007ba2(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}

void SceneEffect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Func_02009280(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = ((Func_0200923a() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = ((Func_0200924e() << 3) >> 16) * 0x3333 + 0xffff3334;

                Func_02003a32(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Func_020093a8(1);
            }
        }
        Func_0200931a(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Func_02009330(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}

void SceneEffect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Func_02009548(0);
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_02009426() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + (((Func_02009440() << 2) >> 16) << 16);
            s32 a = ((Func_02009452() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009466() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003c40(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_020094e8() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = ((Func_02009502() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009516() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003cee(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + (((Func_0200958a() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + (((Func_0200959a() << 3) >> 16) << 16) + 0xfffc0000;

        Func_02003d68(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
{
    struct Src_39c *src = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Func_0200ab74(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = src->f8;
    work->f12 += 0x10000;
    work->f16 = src->f16;
    return 1;
}
