#include "types.h"

struct EffectSprite {
    u8 pad[0x1e];
    u16 h1e;
};

struct EffectWork {
    u8 pad0[8];
    s32 x;      /* 0x08 */
    s32 y;      /* 0x0c */
    s32 z;      /* 0x10 */
    s32 pad14;
    s32 a18;    /* 0x18 */
    s32 a1c;    /* 0x1c */
    u8 pad20[0x10];
    s32 v18;    /* 0x30 */
    s32 v1c;    /* 0x34 */
    u8 pad38[0xc];
    s32 vx;     /* 0x44 */
    s32 vy;     /* 0x48 */
    s32 vz;     /* 0x4c */
    struct EffectSprite *sprite; /* 0x50 */
    u8 pad54[0x10];
    u16 h64;    /* 0x64 */
};

#define IntegrateEffect Func_02000104

void IntegrateEffect(struct EffectWork *work)
{
    struct EffectSprite *sprite;

    work->x += work->vx;
    work->y += work->vy;
    work->z += work->vz;
    work->a18 += work->v18;
    *(s32 *)((u32)work + 0x1c) = *(s32 *)((u32)work + 0x1c) + work->v1c;
    sprite = (struct EffectSprite *)*(s32 *)((u32)work + 0x50);
    sprite->h1e += work->h64;
}
