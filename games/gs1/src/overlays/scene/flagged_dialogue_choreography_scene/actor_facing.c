#include "types.h"

#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_UpdateFacingTowardTarget Func_020000d8
#define SceneActor_SetActorZeroFacingC000AndRun Func_02001990
#define SceneActor_TurnTowardTableAngle Func_02001f24

/*
 * Prepare the scene service selected by index zero, set its halfword at +6,
 * and run the four follow-up services in the order present in the overlay.
 *
 * This owner starts at the saved-link prologue at 0x02001990 and returns at
 * 0x020019b8.  The next saved-link prologue is at 0x020019bc, so the two zero
 * bytes between them are alignment and are deliberately outside this source.
 */
struct SceneService_02001990 {
    u16 unknown00[3];
    u16 value06;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

extern s16 Data_0200a3f0[];

s32 Func_02002282(s32, s32);
void Func_02003bc4(void);
struct SceneService_02001990 *Func_02003be2(s32 index);
void Func_02003d8e(s32 value);
void Func_02003d32(void);
void Func_02003d3e(void);
void Func_02003d14(s32 value);
void Func_02004100(T *, s32);
void Func_02004128(T *, s32);
void Func_02004138(T *, s32);

s32 OverlayObject_UpdateFacingTowardTarget(void *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    void *target;
    target = FIELD(obj, void *, 0x68);
    if (target != NULL) {
        FIELD(obj, u8, 0x5A) = (u8)(0xFE & FIELD(obj, u8, 0x5A));
        angle = (u16)Func_02002282(FIELD(target, s32, 0x10) - FIELD(obj, s32, 0x10), FIELD(target, s32, 8) - FIELD(obj, s32, 8));
        old = FIELD(obj, u16, 6);
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) delta = 0x1000;
            if (delta < -0x1000) delta = -0x1000;
            FIELD(obj, u16, 6) = (u16)(old + delta);
        }
    }
    return 1;
}

void SceneActor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService_02001990 *work;

    Func_02003bc4();
    work = Func_02003be2(0);
    work->value06 = 0xc000;
    Func_02003d8e(123);
    Func_02003d32();
    Func_02003d3e();
    Func_02003d14(8);
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = Data_0200a3f0[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Func_02004100(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Func_02004128(o, 2);
    Func_02004138(o, 0x30);
}
