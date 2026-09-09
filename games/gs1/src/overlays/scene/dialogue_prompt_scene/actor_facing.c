#include "types.h"

#define SceneActor_StepActor24AnimationByFacing Func_020005bc
#define SceneActor_FaceActors24And25TowardActorZero Func_02002ba0
typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
struct Rec_383 {
    u8 pad00[6];
    u16 f6;                     /* +6  */
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

extern s32 Data_0200e4a8[];
extern s32 Data_0200e4c0[];

T *Func_0200533e(s32);
void Func_0200530c(void);
void Func_020053cc(s32, s32);
void Func_020053ea(s32);
void Func_02005402(s32, s32);
void Func_02005370(s32, s32, s32);
void Func_020053a2(s32, s32);
void Func_020053be(s32, s32);
void Func_020053de(s32, s32);
void Func_02005418(s32);
void Func_020053d4(void);
struct Rec_383 *Func_02007926();
struct Rec_383 *Func_0200792e();
struct Rec_383 *Func_02007936();
s32 Func_02007858();
s32 Func_0200786c();

void SceneActor_StepActor24AnimationByFacing(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Func_0200533e(24);
    Func_0200530c();
    Func_020053cc(24, 2);
    Func_020053ea(0x12ac);
    Func_02005402(24, 0);
    Func_02005370(24, 0x40000, 0x20000);
    if ((u32)((p->unk6 & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Func_020053a2(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Func_020053be(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Func_020053de(24, Data_0200e4c0[v]);
    n = *(u16 *)q - 1;
    *(u16 *)q = n;
clamp:
    if (*q > 5) {
        n = 0;
        *(u16 *)q = n;
    }
    if (*q < 0) {
        n = 5;
        *(u16 *)q = n;
    }
    Func_02005418(24);
    Func_020053d4();
}

void SceneActor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Func_02007926(0);
    struct Rec_383 *first = Func_0200792e(24);
    struct Rec_383 *second = Func_02007936(25);

    first->f6 = Func_02007858(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Func_0200786c(origin->f16 - second->f16, origin->f8 - second->f8);
}
