#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_ApplyValues12And2And3 Func_02000030
#define SceneState_ClearWord24WhenFlag200 Func_0200158c
#define SceneState_ClearRecordZeroBit0At35 Func_020015f8
#define SceneState_CheckFlags941And940 Func_020017e8
#define SceneState_ApplyFlagGatedActorEightSetup Func_02001858
#define SceneState_SetWork448AndRunFlag915Step Func_020018a4
#define SceneState_SetValues27Through34 Func_02001e6c

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern s16 Data_02000240[];

void Func_02003d96(s32, s32, s32);
s32 Func_02005168();
void Func_0200518c();
s32 Func_0200521a(s32);
s32 Func_020053c2(s32);
void Func_020053d4(s32);
void Func_020053da(s32);
void Func_020053e0(s32);
s32 Func_020053e6(s32);
s32 Func_020053e6_a(s32);
void Func_020053f8(s32);
s32 Func_02005406(s32);
void Func_02003842(void);
s32 Func_02005432(s32);
void Func_020052a4(s32);
s32 Func_02005442(s32);
struct Obj *Func_02005494(s32);
void Func_02003f50(void);
s32 Func_0200548e(s32);
void Func_0200546e(s32, s32, s32, s32, s32, s32);
void Func_0200548a(s32, s32, s32, s32, s32, s32);
void Func_02005494_a(s32, s32, s32, s32, s32, s32);
void Func_02005478(void);
void Func_02005416(s32);
void Func_0200567c(s32);
void Func_02003d16(s32);
void Func_02003d1c(s32);
void Func_02003d22(s32);
void Func_02003d28(s32);
void Func_02003d2e(s32);
void Func_02003d34(s32);
void Func_02003d3a(s32);
void Func_02003d40(s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void SceneState_ApplyValues12And2And3(void)
{
    Func_02003d96(0xC, 2, 3);
}

void SceneState_ClearWord24WhenFlag200(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_02005168, 0x200) != 0) {
        *(volatile s32 *)(*(volatile s32 *)0x03001ee0 + 24) = 0;
        Call1(Func_0200518c, 0x200);
    }
}

void SceneState_ClearRecordZeroBit0At35(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_0200521a(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}

void SceneState_CheckFlags941And940(void)
{
    if (Func_020053c2(0x941) != 0) {
        Func_020053d4(0x321);
        Func_020053da(0x913);
        Func_020053e0(0x912);
        Func_020053e6(0x915);
    }
    if (Func_020053e6_a(0x940) != 0) {
        Func_020053f8(0x321);
    }
    if (Data_02000240[225] != 0) {
        if (Func_02005406(0x912) == 0) {
            Func_02003842();
        }
    }
}

void SceneState_ApplyFlagGatedActorEightSetup(void)
{
    struct Obj *o;

    if (Func_02005432(0xfd6) == 0) {
        Func_020052a4(12);
    }
    if (Func_02005442(0x915) != 0) {
        o = Func_02005494(8);
        o->f06 = 0;
    }
    if (Data_02000240[225] == 10) {
        Func_02003f50();
    }
}

void SceneState_SetWork448AndRunFlag915Step(void)
{
    extern u8 *Data_03001ebc;

    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_0200548e(0x915) != 0) {
        s32 k = 2;
        Func_0200546e(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Func_0200548a(8, 11, 2, 1, a, b); }
        Func_02005494_a(8, 12, 8, 11, k, 1);
        Func_02005478();
        Func_02005416(1);
    }
    if (Data_02000240[225] <= 3) {
        Func_0200567c(170);
    }
}

void SceneState_SetValues27Through34(void)
{
    Func_02003d16(27);
    Func_02003d1c(28);
    Func_02003d22(29);
    Func_02003d28(30);
    Func_02003d2e(32);
    Func_02003d34(31);
    Func_02003d3a(33);
    Func_02003d40(34);
}
