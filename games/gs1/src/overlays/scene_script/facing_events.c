#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneData_GetTable8BB4 Func_02000030
#define SceneData_GetTable8dac Func_0200003c
#define SceneData_SelectTable8e00ByFlag Func_02000044
#define SceneData_SelectTable9310ByFlags Func_02000064
#define SceneState_SetWork1c0AndRun Func_0200009c
#define FieldScene_RunPrimarySequence Func_020000c0
#define FieldScene_RunScene3b6SequenceA Func_0200013c
#define FieldScene_RunActorsThirtyOneToThirtyThreeChoreography Func_02000328
#define Scene_InitFacingActors Func_020003dc
#define SceneDialogue_RunFacingPrompt Func_02000500
#define SceneDialogue_RunFacingActionPrompt Func_020005a8
#define SceneDialogue_RunFacingAction Func_0200066c
#define SceneDialogue_RunMessage239eStep Func_020006ec
#define SceneDialogue_RunActorLine23a1 Func_0200073c
#define SceneDialogue_RunMessage1FBBStep Func_02000760
#define FieldScene_RunScene3b6_020007b0 Func_020007b0
#define SceneDialogue_RunActor25FlaggedLine Func_0200085c
#define FieldScene_RunScene3b6_02000898 Func_02000898
#define SceneDialogue_RunMessage23acStep Func_020008cc
#define SceneDialogue_RunFacingMessage Func_0200091c

struct Ent { s32 a; u16 b; u16 c; };

extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];
extern struct Ent Data_02009dcc[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 Data_00001fd5[];
extern u8 Data_00002389[];
extern u8 Value_0000239e;
extern u8 Value_00001fbb;
extern u8 Data_00002399[];
extern u8 Value_000023ac;

s32 Func_02000a14(s32);
s32 Func_02000a34(s32);
s32 Func_02000a42(s32);
void Func_02000b16(void);
s32 Func_02000ab2();
s32 Func_02000ac6();
s32 Func_02000b0e();
void Func_02000b10();
void Func_02000b3c();
void Func_02000b78();
void Func_02000b8c();
s32 Func_02000afe();
void Func_02000b06();
void Func_02000b14();
void Func_02000b20();
void Func_02000b24();
void Func_02000b26();
void Func_02000b30();
void Func_02000b4c();
void Func_02000b5a();
void Func_02000b66();
void Func_02000b7e();
void Func_02000ba0();
void Func_02000ba8();
void Func_02000bb6();
void Func_02000bbe();
void Func_02000bd6();
void Func_02000be8();
void Func_02000bf2();
void Func_02000bf4();
void Func_02000bfa();
void Func_02000c08();
void Func_02000c0c();
void Func_02000c12();
void Func_02000c18();
void Func_02000c1a();
void Func_02000c28();
void Func_02000c38();
void Func_02000c46();
void Func_02000c48();
void Func_02000c54();
void Func_02000c62();
void Func_02000c6c();
void Func_02000c7e();
void Func_02000c88();
void Func_02000c8c();
void Func_02000c8c_a();
void Func_02000c92();
void Func_02000c9a();
void Func_02000c9a_a();
void Func_02000cb2();
void Func_02000cbe();
void Func_02000cbe_a();
void Func_02000cd8();
void Func_02000cdc();
void Func_02000ce0();
void Func_02000cf0();
void Func_02000cf0_a();
void Func_02000d12();
void Func_02000d14();
void Func_02000d14_a();
void Func_02000d1e();
void Func_02000d30();
void Func_02000d44_action();
void Func_02000d6a_action();
void Func_02000d0c(void);
void Func_02000d6a_a(s32, s32, s32);
void Func_02000d10(s32);
void Func_02000d74(s32, s32, s32);
void Func_02000d9c(s32, s32);
void Func_02000d32(s32);
void Func_02000dbe(s32, s32, s32);
void Func_02000d94(s32, s32);
void Func_02000d52(s32);
void Func_02000dca(s32, s32);
void Func_02000d60(s32);
void Func_02000db0(s32, s32);
void Func_02000d6e(s32);
void Func_02000de6(s32, s32);
void Func_02000d7c(s32);
void Func_02000ddc(s32, s32);
void Func_02000d8a(s32);
void Func_02000e02(s32, s32);
void Func_02000d98(s32);
void Func_02000de8(s32, s32);
void Func_02000da6(s32);
void Func_02000dba(void);
void Func_020005d4();
void Func_020007ea();
s32 Func_02000dc0();
void Func_02000dc8();
u8 *Func_02000e0e();
s32 Func_02000e1a();
u8 *Func_02000e32();
s32 Func_02000e4e();
void Func_02000e60();
void Func_02000e74();
s32 Func_02000e76();
void Func_02000e8a();
s32 Func_02000e90();
void Func_02000ec0();
void Func_02000ee6();
void Func_02000ef0();
void Func_02000efc();
void Func_02000f26();
u8 *Func_02000f00(s32 no);
s32 Func_02000ef8(s32 flag);
s32 Func_02000f10(s32 flag);
s32 Func_02000f64(s32 a, s32 b);
void Func_02000f8a(s32 msg);
void Func_02000f9c(s32 msg);
void Func_02000f9e(s32 action, s32 no);
void Func_02000fa2(s32 no, s32 mode);
s32 Func_02000fac(s32 no, s32 mode);
void Func_02000f56(s32 frames);
void Func_02000fbc(s32 msg);
void Func_02000fc4(s32 msg);
void Func_02000fdc(s32 no, s32 mode);
u8 *Func_02000fa8(s32 no);
s32 Func_02000fa6(s32 flag);
s32 Func_02000fde(s32 flag);
s32 Func_02000fea(s32 a, s32 b);
s32 Func_02001032(s32 no, s32 mode);
void Func_02001046_action(s32 action, s32 no);
void Func_02001046_msg(s32 msg);
void Func_02001022_msg(s32 msg);
void Func_02001022_wait(s32 frames);
void Func_02000fdc_wait(s32 frames);
void Func_02001058(s32 msg);
void Func_02001068(s32 msg);
void Func_02001070(s32 no, s32 mode);
void Func_02001080(s32 no, s32 mode);
void Func_0200109a(s32 no, s32 mode);
void Func_0200109c(s32 no, s32 action, s32 mode);
u8 *Func_0200106c(s32 no);
s32 Func_02001064(s32 flag);
s32 Func_0200107c(s32 flag);
void Func_020010f6(s32 msg);
void Func_02001106(s32 msg);
void Func_0200110e(s32 no, s32 mode);
void Func_0200110a(s32 action, s32 no);
void Func_0200111e(s32 no, s32 mode);
u8 *Func_0200131c(s32 no);
s32 Func_02001328(s32 flag);
void Func_020013a2(s32 msg);
void Func_020013b2(s32 msg);
void Func_020013ba(s32 no, s32 mode);
void Func_020013c0(s32 no);
void Func_020013ca(s32 no, s32 mode);
void Func_02001122(s32);
void Func_02001180(s32);
void Func_02001198(s32, s32);
void Func_0200113c(void);
s32 Func_02001184();
void Func_02001196();
void Func_020011ba();
s32 Func_020011c8();
s32 Func_020011c8_a();
void Func_020011da();
void Func_020011f6();
void Func_02001200();
void Func_02001204();
s32 Func_02001210();
void Func_02001220();
void Func_02001226();
void Func_02001228();
void Func_02001248();
void Func_02001256();
void Func_02001260();
void Func_0200126a();
void Func_02001282();
s32 Func_0200122e(s32);
void Func_020012a8(s32);
void Func_020012b0(s32);
void Func_020012c8(s32, s32);
void Func_0200126c(void);
void Func_0200127e();
void Func_020012a2();
void Func_020012dc();
void Func_020012fe();
void Func_02001306();

/* Contiguous unnamed leaf-owner run for resource_3b6. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */

/* Pool-forced message base: a SYMBOL_REF gcc cannot fold, so the two
 * sibling ids stay `adds r0, r5, #1` / `#2` off one pooled word. */

/* Old-style declarations: interfaces vary by call site across this overlay. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    return f(a0, a1, a2, a3);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void SceneInit_Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 SceneInit_Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void SceneInit_Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void SceneInit_Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Scene_Call1(void (*f)(s32), s32 a) { f(a); }

static __inline__ s32 Scene_Value2(s32 (*f)(s32,s32), s32 a, s32 b) { return f(a,b); }

static __inline__ void Scene_Call3(void (*f)(s32,s32,s32), s32 a, s32 b, s32 c) { f(a,b,c); }

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

void Func_020010d2(); /* begin step */

void Func_02001132(); /* present message by id */

s32 Func_02001142();  /* act on the subject, variant used before the branch */

s32 Func_020010fa();  /* branch predicate; nonzero selects the second arm */

void Func_020010ec(); /* wait / delay by count */

void Func_02001152(); /* present message, first arm */

void Func_0200115a(); /* present message, second arm */

void Func_02001172(); /* act on the subject */

void Func_02001116(); /* end step */

void Func_02001146(); /* begin step */

void Func_020011a6(); /* present message by id */

s32 Func_020011b6();  /* act on the subject, variant used before the branch */

s32 Func_0200116e();  /* branch predicate; nonzero selects the second arm */

void Func_02001160(); /* wait / delay by count */

void Func_020011c6(); /* present message, first arm */

void Func_020011ce(); /* present message, second arm */

void Func_020011e6(); /* act on the subject */

void Func_0200118a(); /* end step */

void Func_020012b2(); /* begin step */

void Func_02001312_message(); /* present message by id */

s32 Func_02001322();  /* act on the subject, variant used before the branch */

s32 Func_020012da();  /* branch predicate; nonzero selects the second arm */

void Func_020012cc(); /* wait / delay by count */

void Func_02001332(); /* present message, first arm */

void Func_0200133a(); /* present message, second arm */

void Func_02001352(); /* act on the subject */

void Func_020012f6(); /* end step */

u8 *SceneData_GetTable8BB4(void)
{
    return (u8 *)0x02008bb4;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return (u8 *)0x02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void) {
    if (Func_02000a14(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (Func_02000a34(0x950) != 0) {
        return Data_020099d0;
    }
    if (Func_02000a42(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}

void SceneState_SetWork1c0AndRun(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    Func_02000b16();
}

void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)Data_03001ebc;
    for (i = 8; i < 66; i++) {
        record = Value1(Func_02000ac6, i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Func_02000b78(158);
    Value3(Func_02000ab2,
           Data_02009dcc[n].a,
           Data_02009dcc[n].b,
           Data_02009dcc[n].c);
    Call3(Func_02000b10, 0, 0x8000, 0x4000);
    *(u8 *)(Func_02000b0e(0) + 85) = 0;
    Func_02000b3c(0, 2);
    Func_02000b8c(*(s16 *)p6);
}

void FieldScene_RunScene3b6SequenceA(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    Func_02000b20();
    Call1(Func_02000b7e, 0x2394);
    Func_02000b24(40);
    rec7 = Value4(Func_02000afe, 0x11c, 0x2580000, 0, 0x3380000);
    Func_02000b26(rec7, 0);
    Func_02000b06(rec7, 6);
    Func_02000b4c(10);
    Func_02000b14(rec7, 1);
    Func_02000b5a(40);
    Func_02000b30(rec7);
    Func_02000b66(2);
    Call3(Func_02000bf2, 25, 0x100, 50);
    Call3(Func_02000ba8, 25, 0x10000, 0x8000);
    Call3(Func_02000bbe, 25, 0x258, 0x350);
    Call3(Func_02000c12, 25, 0xc000, 0);
    Func_02000ba0(40);
    Func_02000c18(25, 0);
    Func_02000c08(25, 2);
    Func_02000bb6(30);
    Call3(Func_02000bf4, 25, 0x238, 0x350);
    Call3(Func_02000c48, 25, 0xc000, 0);
    Func_02000bd6(30);
    Call3(Func_02000c62, 25, 0x108, 50);
    Func_02000be8(20);
    Call3(Func_02000c8c, 0, 0, -16);
    Func_02000bfa(20);
    Call3(Func_02000c7e, 25, 0x3000, 0);
    Func_02000c0c(30);
    Func_02000c6c(25, 2);
    Func_02000c1a(20);
    Func_02000c92(25, 0);
    Func_02000c28(20);
    Call3(Func_02000cb2, 0, 0x101, 50);
    Func_02000c38(20);
    Func_02000c88(25, 4);
    Func_02000c46(20);
    Func_02000cbe(25, 0);
    Func_02000c54(30);
    Call3(Func_02000ce0, 25, 0x102, 50);
    Func_02000cd8(25, 0);
    Call3(Func_02000c9a, 25, 0x16666, 0xb333);
    Func_02000d14(25, 16, 0);
    Func_02000d1e(25, 0, 32);
    Func_02000c8c_a(20);
    Func_02000cdc(25, 3);
    Func_02000c9a_a(20);
    Func_02000d12(25, 0);
    Func_02000d44_action(0, 16, 0);
    Call3(Func_02000d30, 0, 0x8000, 0);
    Func_02000cbe_a(20);
    Call3(Func_02000cf0, 25, 0x1cccc, 0xe666);
    Func_02000d6a_action(25, 0, 48);
    Func_02000d14_a(25, 0, 0);
    Func_02000cf0_a();
}

void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Func_02000d44(s32);
    void Func_02000d6a(s32);

    Func_02000d0c();
    Func_02000d6a(0x23A4);
    Func_02000d10(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Func_02000d6a_a(31, 4, 13);
    Func_02000d74(31, 4, 30);
    Func_02000d9c(31, 0);
    Func_02000d32(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Func_02000dbe(32, 0x102, 50);
    Func_02000d44(10);
    Func_02000d94(32, 3);
    Func_02000d52(30);
    Func_02000dca(32, 0);
    Func_02000d60(10);
    Func_02000db0(33, 4);
    Func_02000d6e(20);
    Func_02000de6(33, 0);
    Func_02000d7c(10);
    Func_02000ddc(31, 2);
    Func_02000d8a(20);
    Func_02000e02(31, 0);
    Func_02000d98(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Func_02000de8(32, 3);
    Func_02000da6(30);
    Func_02000dba();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    extern u8 Data_03001ebc[];

    u8 *record;
    s32 none;

    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x209;
    if (SceneInit_Value1(Func_02000dc0, 0x950) != 0) {
        SceneInit_Call6(Func_02000dc8, 51, 47, 3, 1, 51, 45);
        record = Func_02000e0e(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Func_02000e32(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (SceneInit_Value1(Func_02000e1a, 0x8bc) != 0) {
            SceneInit_Call3(Func_02000e74, 25, 0x2300000, 0x2a80000);
            SceneInit_Call3(Func_02000ec0, 25, 0x8000, 0);
        }
        if (Data_02000240_t[225][0] == 19) {
            if (SceneInit_Value1(Func_02000e4e, 0x8bc) == 0) {
                SceneInit_Call1(Func_02000e60, 0x8bc);
                Func_02000efc();
                Func_020005d4();
            }
        }
        if (Data_02000240_t[225][0] == 16) {
            if (SceneInit_Value1(Func_02000e76, 0x300) == 0) {
                SceneInit_Call1(Func_02000e8a, 0x300);
                Func_02000f26();
                Func_020007ea();
            }
        }
        if (SceneInit_Value1(Func_02000e90, 0x8ab) != 0) {
            Func_02000ee6(35, 0, 0);
            Func_02000ef0(36, 0, 0);
        }
    }
    return 0;
}

void SceneDialogue_RunFacingPrompt(s32 no)
{

    u8 *actor = Func_02000f00(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0x8000) {
        Func_02000f9e(28, no);
    } else if (Func_02000ef8(0x950) != 0) {
        Scene_Call1(Func_02000f8a, 0x238d);
        Func_02000fa2(no, 0);
    } else if (Func_02000f10(0x962) != 0) {
        Scene_Call1(Func_02000f8a, 0x221b);
        Func_02000fa2(no, 0);
    } else {
        msg = (s32)Data_00001fd5;
        Func_02000f9c(msg);
        Scene_Value2(Func_02000fac, no, 0);
        if (Scene_Value2(Func_02000f64, 0, 0) == 0) {
            Func_02000f56(10);
            Func_02000fbc(msg + 1);
        } else {
            Func_02000fc4(msg + 2);
        }
        Func_02000fdc(no, 0);
    }
}

void SceneDialogue_RunFacingActionPrompt(s32 no)
{

    u8 *actor = Func_02000fa8(0);
    s32 msg;
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001046_action(26, no);
    } else if (Func_02000fa6(0x950) != 0) {
        msg = (s32)Data_00002389;
        Func_02001022_msg(msg);
        Scene_Value2(Func_02001032, no, 0);
        if (Scene_Value2(Func_02000fea, 0, 0) == 0) {
            Func_02000fdc_wait(10);
            Func_02001058(msg + 1);
        } else {
            Func_02001046_msg(msg + 2);
        }
        Func_02001070(no, 0);
    } else if (Func_02000fde(0x962) != 0) {
        Scene_Call1(Func_02001058, 0x2219);
        Func_02001070(no, 0);
    } else {
        Scene_Call1(Func_02001068, 0x1fd2);
        Func_02001080(no, 0);
        Scene_Call3(Func_0200109c, no, 0x106, 0);
        Func_02001022_wait(40);
        Func_0200109a(no, 0);
    }
}

void SceneDialogue_RunFacingAction(s32 no)
{

    u8 *actor = Func_0200106c(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Func_0200110a(27, no);
    } else {
        if (Func_02001064(0x950) != 0) {
            Scene_Call1(Func_020010f6, 0x238f);
            Func_0200110e(no, 0);
        } else if (Func_0200107c(0x962) != 0) {
            Scene_Call1(Func_020010f6, 0x221d);
            Func_0200110e(no, 0);
        } else {
            Func_02001106(0x1fd9);
            Func_0200111e(no, 0);
        }
    }
}

void SceneDialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Func_020010d2();

    message = (s32)&Value_0000239e;
    Func_02001132(message);
    Func_02001142(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Func_020010fa(0, 0) == 0) {
        Func_020010ec(10);
        Func_02001152(message + 1);
    } else {
        Func_0200115a(message + 2);
    }

    Func_02001172(subject, 0);
    Func_02001116();
}

void SceneDialogue_RunActorLine23a1(s32 no)
{
    Func_02001122(no);
    Func_02001180(0x23A1);
    Func_02001198(no, 0);
    Func_0200113c();
}

void SceneDialogue_RunMessage1FBBStep(s32 subject)
{
    s32 msg;

    Func_02001146();

    msg = (s32)&Value_00001fbb;
    Func_020011a6(msg);
    Func_020011b6(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Func_0200116e(0, 0) == 0) {
        Func_02001160(10);
        Func_020011c6(msg + 1);
    } else {
        Func_020011ce(msg + 2);
    }

    Func_020011e6(subject, 0);
    Func_0200118a();
}

void FieldScene_RunScene3b6_020007b0(s32 a0)
{
    void Func_02001240();

    u32 i;
    s32 record;
    s32 base5_2399;

    Func_02001196();
    if (Value1(Func_02001184, 0x8bd) == 0) {
        base5_2399 = (s32)Data_00002399;
        Func_02001200(base5_2399);
        Value2(Func_02001210, a0, 0);
        if (Value2(Func_020011c8, 0, 0) == 0) {
            Func_020011ba(10);
            Func_02001220((base5_2399 + 1));
        } else {
            Func_02001228((base5_2399 + 2));
        }
        Func_02001240(a0, 0);
    } else {
        if (Value1(Func_020011c8_a, 0x8be) == 0) {
            Call1(Func_020011da, 0x8be);
            Call1(Func_02001248, 0x239c);
            Func_02001260(a0, 0);
            Func_020011f6(10);
            Func_02001256(a0, 2);
            Func_02001204(20);
        }
        Call1(Func_0200126a, 0x239d);
        Func_02001282(a0, 0);
    }
    Func_02001226();
}

void SceneDialogue_RunActor25FlaggedLine(void)
{
    void Func_02001240_begin(void);

    Func_02001240_begin();
    if (Func_0200122e(0x8BE) == 0) {
        Func_020012a8(0x23B3);
    } else {
        Func_020012b0(0x23B4);
    }
    Func_020012c8(25, 0);
    Func_0200126c();
}

void FieldScene_RunScene3b6_02000898(s32 a0)
{

    u32 i;
    s32 record;

    Func_0200127e();
    Call1(Func_020012dc, 0x23a8);
    Call3(Func_02001306, 31, 0x103, 40);
    Func_020012fe(a0, 0);
    Func_020012a2();
}

void SceneDialogue_RunMessage23acStep(s32 subject)
{
    s32 message;

    Func_020012b2();

    message = (s32)&Value_000023ac;
    Func_02001312_message(message);
    Func_02001322(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Func_020012da(0, 0) == 0) {
        Func_020012cc(10);
        Func_02001332(message + 1);
    } else {
        Func_0200133a(message + 2);
    }

    Func_02001352(subject, 0);
    Func_020012f6();
}

void SceneDialogue_RunFacingMessage(s32 no)
{
    s32 Func_02001312(s32 flag);

    u8 *actor = Func_0200131c(0);
    if ((u16)((*(u16 *)(actor + 6) + 0x2000) & ~0x3fff) == 0xc000) {
        Func_020013c0(no);
    } else {
        if (Func_02001312(0x950) != 0) {
            Scene_Call1(Func_020013a2, 0x23bf);
            Func_020013ba(no, 0);
        } else if (Func_02001328(0x962) != 0) {
            Scene_Call1(Func_020013a2, 0x2231);
            Func_020013ba(no, 0);
        } else {
            Func_020013b2(0x1feb);
            Func_020013ca(no, 0);
        }
    }
}
