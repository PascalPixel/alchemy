#include "types.h"

#define SceneState_SetFlags92bAnd94b Func_020002c0
#define SceneState_SetFlags929And949 Func_020002e8
#define SceneState_SetValue123Mode11 Func_02001990
#define SceneActor_UpdateAnimationOnStateMatch Func_020026e4
#define SceneState_SetFlagByActorPosition Func_0200271c
#define SceneState_SetWord1c0To209AndRun Func_020045cc
#define SceneState_SetValue2ThenFinish Func_02004684
void Func_02005008(void);
void Func_02004fc0(s32, s32);
void Func_02004fc8(s32, s32);
void Func_02005024(void);
void Func_02005030(void);
void Func_02004fe8(s32, s32);
void Func_02004ff0(s32, s32);
void Func_0200504c(void);
void Func_020068ba(s32);
void Func_02006808(s32);
/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */
extern u8 *Func_02007472(s32 actor);
void Func_02007496(s32 actor, const u8 *desc);
u8 *Func_0200749e(s32 actor);
u8 *Func_020074a6(s32 actor);
void Func_02007462(s32 flag);
void Func_02007474(s32 flag);
void Func_0200947a(void);
void Func_0200948e(void);
void Func_02009324(s32);
void Func_02009556(s32);
void Func_02009572(void);
void Func_0200957e(void);

void SceneState_SetFlags92bAnd94b(void)
{
    Func_02005008();
    Func_02004fc0(0x92b, 1);
    Func_02004fc8(0x94b, 1);
    Func_02005024();
}

void SceneState_SetFlags929And949(void)
{
    Func_02005030();
    Func_02004fe8(0x929, 1);
    Func_02004ff0(0x949, 1);
    Func_0200504c();
}

void SceneState_SetValue123Mode11(void)
{
    Func_020068ba(123);
    Func_02006808(11);
}

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = Func_02007472(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Func_02007496(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void SceneState_SetFlagByActorPosition(void)
{
    u8 *p0 = Func_0200749e(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = Func_020074a6(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Func_02007462(148 << 2);
    } else {
        Func_02007474(148 << 2);
    }
}

void SceneState_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Func_0200947a();
    Func_0200948e();
    Func_02009324(1);
}

void SceneState_SetValue2ThenFinish(void)
{
    Func_02009556(2);
    Func_02009572();
    Func_0200957e();
}
