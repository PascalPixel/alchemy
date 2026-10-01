/* NONMATCHING: Scene_RunSoldierInterception, resource_64d, 2026-10-01.
 * Represent the native first dialogue operand as a proposed semantic PO import instead of an EN integer immediate.
 * Current EN native owner: 5,404 complete bytes, literal pools included.
 * EN diagnostic: 5396/5404 bytes, 3633 differing positions, first +0x41c; no matching source adoption.
 * All six ordinary TLA target objects compile; text extents in bytes:
 * JA 5396; EN 5396; DE 5396; ES 5396; FR 5396; IT 5396.
 * Other five editions have no complete linked proof for this attempt.
 * MsgVinasuIriguchiThereTheyAre is a proposed canonical role from actual EN PO05559 (There they are / Get them). It has not been integrated into any catalog. This diagnostic leaves its relocation unbound, emits a zero word and counts that word among differences. Neither message-word identity nor executable/link-ready correctness is claimed.
 * Current imports refer to physical maintained symbols. Inherited unchecked
 * generic call signatures, cast-based actor access and unused carrier locals
 * remain draft limitations; this is compilable reconstruction work, not a
 * claim of safe executable behavior or transitive raw-callee readiness.
 * No compiler options, production routing, output patch or credit changed.
 */
#include "TYPES.H"
/* Proposed canonical role from current EN PO05559 and native first message operand; catalog integration pending. */
extern const u8 MsgVinasuIriguchiThereTheyAre;

void Func_020023a4();
void Func_020026f4();
void Func_020027b0();
void GameFlag_SetBit();
void Battle_WaitMode0();
void Func_020028b4();
void Func_020028bc();
s32 Inventory_PromptAndSetObjectMode();
u8 *Object_GetById();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetPositionAndCommit();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Func_0200290c();
void Object_SetModeById();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();
void ObjectMotion_SetVariantCallback();
void Motion_SetVarCbAndRefresh();
void Func_0200293c();
s32 UiText_OpenMessageAtObject();
void Func_0200294c();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetActionVariant();
s32 Func_02002964();
void Func_0200296c();
void Object_AttachWorkTargetToObject();
void Motion_CamBounds();
void Func_0200298c();
void Func_02002994();
void Func_020029cc();
void Func_020029d4();
void ObjectMotion_CommitPositionAndActivate();
void Func_02002a34();
void Field_BeginPaletteTransition();
void Func_02002a44();
void Func_02002a4c();
void Func_02002a54();
void Func_02002a5c();
void Func_02002a64();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    /* FAKEMATCH: inherited argument-forwarding carrier; this draft remains nonmatching. */
    f(a0, a1, a2, a3);
}

void Scene_RunSoldierInterception(void)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 rec;
    u8 *record;
    s32 v5;
    s32 v2;

    Call1(GameFlag_SetBit, 0x890);
    Func_020028b4();
    Func_020029cc(0);
    Call1(Func_0200293c, (s32)&MsgVinasuIriguchiThereTheyAre);
    Call3(ObjectMotion_SetSpeedParameters, 12, 0x10000, 0x8000);
    Call3(ObjectMotion_SetSpeedParameters, 13, 0x10000, 0x8000);
    Func_0200294c(12, 0, 0);
    Call3(ObjectMotion_SetPositionAndReset, 5, 0x178, 0x118);
    Battle_WaitMode0(10);
    Value3(Func_02002964, 5, 0x101, 40);
    Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
    Battle_WaitMode0(30);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    Battle_WaitMode0(30);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Battle_WaitMode0(30);
    Motion_SetVarCbAndRefresh(5, 2);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 5, 0x178, 0x168);
    Battle_WaitMode0(30);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1a80000, 1);
    Func_0200298c();
    ObjectMotion_CommitCurrentPositionAndActivate(5);
    Battle_WaitMode0(10);
    Value3(Func_02002964, 12, 0x102, 40);
    Func_0200294c(12, 0, 10);
    Motion_SetVarCbAndRefresh(13, 2);
    Battle_WaitMode0(10);
    Func_0200294c(13, 0, 10);
    Value3(Func_02002964, 17, 0x107, 30);
    Func_0200294c(17, 0, 10);
    ObjectMotion_Launch(18, 6, 15);
    ObjectMotion_Launch(18, 6, 25);
    Func_0200294c(18, 0, 10);
    Call3(ObjectMotion_ArmCallback, 14, 0x8000, 0);
    Battle_WaitMode0(30);
    Call3(ObjectMotion_ArmCallback, 14, 0xa000, 0);
    Battle_WaitMode0(30);
    Call3(ObjectMotion_ArmCallback, 14, 0x8000, 0);
    Battle_WaitMode0(30);
    Call3(ObjectMotion_ArmCallback, 14, 0x2000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(14, 0, 10);
    Motion_SetVarCbAndRefresh(15, 2);
    Battle_WaitMode0(10);
    Func_0200294c(15, 0, 10);
    Call3(ObjectMotion_ArmCallback, 19, 0x5000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(19, 0, 10);
    Motion_SetModeAndWaitAnimation(20, 3);
    Battle_WaitMode0(10);
    Func_0200294c(20, 0, 10);
    Motion_SetModeAndWaitAnimation(19, 3);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 15, 0xe000, 0);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 16, 0x5000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(15, 0, 10);
    Motion_SetModeAndWaitAnimation(16, 3);
    Battle_WaitMode0(10);
    Func_0200294c(16, 0, 10);
    ObjectMotion_CommitPositionAndActivate(16, 64, 0);
    Func_0200290c(16, 0, 0);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 20, 0xa000, 0);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 21, 0x2000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(20, 0, 10);
    Motion_SetModeAndWaitAnimation(21, 3);
    Battle_WaitMode0(10);
    Func_0200294c(21, 0, 10);
    Call3(ObjectMotion_CommitPositionAndActivate, 21, -64, 64);
    Func_0200290c(21, 0, 0);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 14, 0xa000, 0);
    Call3(ObjectMotion_ArmCallback, 15, 0xa000, 0);
    Call3(ObjectMotion_ArmCallback, 19, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 20, 0xe000, 0);
    Battle_WaitMode0(30);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1680000, 1);
    Func_0200298c();
    Battle_WaitMode0(10);
    Call4(Func_020029d4, 8, -16, -16, 0x4000);
    Call4(Func_020029d4, 9, 16, -16, 0x4000);
    ObjectMotion_CommitCurrentPositionAndActivate(8);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x10000, 0x8000);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x10000, 0x8000);
    Battle_WaitMode0(20);
    ObjectMotion_CommitPositionAndActivate(8, 0, 16);
    Battle_WaitMode0(20);
    Motion_SetVarCbAndRefresh(8, 2);
    Battle_WaitMode0(20);
    ObjectMotion_ArmCallback(8, 0, 0);
    Battle_WaitMode0(20);
    Func_0200294c(8, 0, 10);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Battle_WaitMode0(40);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Battle_WaitMode0(30);
    Call3(Func_0200294c, 0x2005, 0, 10);
    Value3(Func_02002964, 9, 0x101, 30);
    Func_0200294c(9, 0, 0);
    ObjectMotion_CommitPositionAndActivate(9, 0, 16);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(9, 0, 10);
    ObjectMotion_ArmCallback(5, 0, 0);
    Battle_WaitMode0(20);
    Value3(Func_02002964, 5, 0x102, 60);
    Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
    Battle_WaitMode0(30);
    Motion_SetModeAndWaitAnimation(5, 3);
    Battle_WaitMode0(10);
    Func_0200294c(5, 0, 10);
    Motion_SetVarCbAndRefresh(9, 2);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x8000, 0x4000);
    Battle_WaitMode0(10);
    ObjectMotion_CommitPositionAndActivate(9, 0, 16);
    Call3(ObjectMotion_ArmCallback, 9, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x2000, 0);
    Battle_WaitMode0(20);
    Value3(Func_02002964, 8, 0x101, 30);
    Func_0200294c(8, 0, 10);
    Call3(ObjectMotion_ArmCallback, 9, 0xa000, 0);
    Battle_WaitMode0(20);
    ((void (*)())UiText_OpenMessageAtObject)(9, 0);
    Battle_WaitMode0(10);
    if (Value2(Inventory_PromptAndSetObjectMode, 5, 0) == 0) {
        Battle_WaitMode0(20);
        Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
        ObjectMotion_ArmCallback(8, 0, 0);
        Battle_WaitMode0(20);
        Motion_SetModeAndWaitAnimation(8, 3);
        Battle_WaitMode0(20);
        Motion_SetModeAndWaitAnimation(5, 3);
        Battle_WaitMode0(30);
        Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
        Call3(ObjectMotion_ArmCallback, 8, 0x2000, 0);
        Battle_WaitMode0(20);
        Func_0200294c(8, 0, 10);
        *(u16 *)((*(s32 *)0x0300006c + 0x1c4)) += 1;
    } else {
        Battle_WaitMode0(30);
        ObjectMotion_ArmCallback(8, 0, 0);
        Battle_WaitMode0(10);
        Motion_SetModeAndWaitAnimation(8, 4);
        *(u16 *)((*(s32 *)0x0300006c + 0x1c4)) += 1;
        Func_0200294c(8, 0, 10);
        Call3(ObjectMotion_ArmCallback, 8, 0x2000, 0);
    }
    Motion_SetModeAndWaitAnimation(9, 3);
    Battle_WaitMode0(20);
    Func_0200294c(9, 0, 10);
    Motion_SetVarCbAndRefresh(9, 2);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Object_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Battle_WaitMode0(20);
    Call4(Motion_CamBounds, 0x1380000, -1, 0x1a80000, 1);
    Func_0200298c();
    Battle_WaitMode0(20);
    Func_0200294c(9, 0, 10);
    Func_0200294c(5, 0, 10);
    Func_0200294c(8, 0, 10);
    Call3(ObjectMotion_ArmCallback, 17, 0x8000, 0);
    ObjectMotion_ArmCallback(18, 0, 0);
    Battle_WaitMode0(40);
    Call3(ObjectMotion_ArmCallback, 17, 0xe000, 0);
    Call3(ObjectMotion_ArmCallback, 18, 0xe000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(17, 0, 10);
    Value3(Func_02002964, 18, 0x107, 30);
    Func_0200294c(18, 0, 10);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1a80000, 1);
    Func_0200298c();
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
    Battle_WaitMode0(25);
    Call3(Func_0200294c, 0x1009, 0, 10);
    Call3(ObjectMotion_ArmCallback, 8, 0x2000, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0x2000, 0);
    Battle_WaitMode0(20);
    Func_0200294c(5, 0, 10);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1680000, 1);
    Func_0200298c();
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(9, 3);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Value3(Func_02002964, 5, 0x102, 40);
    Func_0200294c(5, 0, 10);
    Motion_SetVarCbAndRefresh(9, 2);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Object_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(9, 3);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1980000, 1);
    Func_0200298c();
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 9, 0x3000, 0);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x8000, 0x4000);
    ObjectMotion_CommitPositionAndActivate(9, 0, 16);
    Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
    Call3(ObjectMotion_ArmCallback, 17, 0xf000, 0);
    Call3(ObjectMotion_ArmCallback, 18, 0xf000, 0);
    Call3(ObjectMotion_ArmCallback, 19, 0xf000, 0);
    Call3(ObjectMotion_ArmCallback, 20, 0xf000, 0);
    Battle_WaitMode0(10);
    Motion_SetVarCbAndRefresh(12, 2);
    Battle_WaitMode0(10);
    Func_0200294c(12, 0, 10);
    Motion_SetModeAndWaitAnimation(9, 4);
    Call3(Func_0200294c, 0x1009, 0, 10);
    Motion_SetVarCbAndRefresh(13, 2);
    Battle_WaitMode0(10);
    Func_0200294c(13, 0, 10);
    Call3(ObjectMotion_ArmCallback, 9, 0x9000, 0);
    Battle_WaitMode0(25);
    Func_0200294c(9, 0, 10);
    Call3(ObjectMotion_ArmCallback, 9, 0x2000, 0);
    Battle_WaitMode0(30);
    ObjectMotion_ArmCallback(12, 0, 0);
    Call3(ObjectMotion_ArmCallback, 13, 0x8000, 0);
    Battle_WaitMode0(30);
    Motion_SetModeAndWaitAnimation(12, 3);
    Motion_SetModeAndWaitAnimation(13, 3);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 12, 0xa000, 0);
    Call3(ObjectMotion_ArmCallback, 13, 0xa000, 0);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_SetSpeedParameters, 12, 0x13333, 0x9999);
    Call3(ObjectMotion_SetSpeedParameters, 13, 0x13333, 0x9999);
    Call3(ObjectMotion_CommitPositionAndActivate, 12, -26, 8);
    Call3(ObjectMotion_ArmCallback, 12, 0xb000, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 13, -16, -12);
    Call3(ObjectMotion_ArmCallback, 13, 0x8000, 0);
    Battle_WaitMode0(10);
    Motion_SetVarCbAndRefresh(9, 2);
    Battle_WaitMode0(10);
    Call3(Func_0200294c, 0x1009, 0, 10);
    Battle_WaitMode0(10);
    Object_SetModeById(9, 8);
    Battle_WaitMode0(50);
    Func_02002a44();
    record = Object_GetById(9);
    Func_02002a5c((s32)record, 2);
    Func_02002a64(178);
    Battle_WaitMode0(20);
    Call2(Func_02002a34, 8, 0x4084);
    Battle_WaitMode0(60);
    Call1(Func_02002a64, 0x186);
    ObjectMotion_SetActionVariant(12, 0);
    Func_020023a4(12);
    Value3(Func_02002964, 17, 0x100, 0);
    Value3(Func_02002964, 18, 0x100, 0);
    Value3(Func_02002964, 19, 0x100, 0);
    Value3(Func_02002964, 20, 0x100, 0);
    Value3(Func_02002964, 13, 0x100, 0);
    Value3(Func_02002964, 14, 0x100, 0);
    Value3(Func_02002964, 15, 0x100, 50);
    ObjectMotion_SetActionVariant(13, 1);
    Func_020023a4(13);
    Battle_WaitMode0(30);
    record = Object_GetById(9);
    Func_02002a5c((s32)record, 0);
    Func_02002a54();
    Func_02002a4c();
    Field_BeginPaletteTransition(16);
    Battle_WaitMode0(10);
    Object_SetModeById(9, 9);
    Battle_WaitMode0(30);
    Value3(Func_02002964, 14, 0x102, 40);
    Func_0200294c(14, 0, 10);
    Motion_SetVarCbAndRefresh(15, 2);
    Battle_WaitMode0(10);
    Func_0200294c(15, 0, 10);
    ObjectMotion_SetVariantCallback(14, 3);
    Call2(Func_0200296c, 14, 0x102);
    ObjectMotion_SetVariantCallback(15, 3);
    Call2(Func_0200296c, 15, 0x102);
    Battle_WaitMode0(50);
    Call3(ObjectMotion_ArmCallback, 14, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 15, 0x8000, 0);
    ObjectMotion_Launch(14, 6, 15);
    ObjectMotion_Launch(14, 6, 25);
    Func_0200294c(14, 0, 10);
    Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x5000, 0);
    Battle_WaitMode0(20);
    Value3(Func_02002964, 9, 0x108, 30);
    Func_0200294c(9, 0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0xcccc, 0x6666);
    Call3(ObjectMotion_CommitPositionAndActivate, 9, -16, 16);
    Call3(ObjectMotion_ArmCallback, 9, 0x6000, 0);
    Call4(Motion_CamBounds, 0x1680000, -1, 0x1980000, 1);
    Func_0200298c();
    ObjectMotion_SetVariantCallback(17, 3);
    Call2(Func_0200296c, 17, 0x102);
    ObjectMotion_SetVariantCallback(18, 3);
    Call2(Func_0200296c, 18, 0x102);
    ObjectMotion_SetVariantCallback(19, 3);
    Call2(Func_0200296c, 19, 0x102);
    ObjectMotion_SetVariantCallback(20, 3);
    Call2(Func_0200296c, 20, 0x102);
    ObjectMotion_SetVariantCallback(21, 3);
    Call2(Func_0200296c, 21, 0x102);
    Battle_WaitMode0(50);
    Motion_SetModeAndWaitAnimation(17, 4);
    Func_0200294c(17, 0, 10);
    Value3(Func_02002964, 18, 0x102, 40);
    Func_0200294c(18, 0, 10);
    Motion_SetModeAndWaitAnimation(19, 4);
    Func_0200294c(19, 0, 10);
    ObjectMotion_SetVariantCallback(20, 3);
    Call2(Func_0200296c, 20, 0x102);
    Battle_WaitMode0(55);
    Call3(ObjectMotion_ArmCallback, 17, 0x8000, 0);
    ObjectMotion_ArmCallback(18, 0, 0);
    Call3(ObjectMotion_ArmCallback, 19, 0x5000, 0);
    Call3(ObjectMotion_ArmCallback, 20, 0xe000, 0);
    Battle_WaitMode0(30);
    Motion_SetVarCbAndRefresh(20, 2);
    Battle_WaitMode0(10);
    Func_0200294c(20, 0, 10);
    ObjectMotion_Launch(18, 6, 0);
    ObjectMotion_Launch(19, 6, 0);
    ObjectMotion_Launch(20, 6, 25);
    Func_020026f4();
    Call3(ObjectMotion_SetSpeedParameters, 20, 0x1cccc, 0xe666);
    Call3(ObjectMotion_SetSpeedParameters, 19, 0x1cccc, 0xe666);
    Call3(ObjectMotion_SetSpeedParameters, 18, 0x1cccc, 0xe666);
    Call3(ObjectMotion_SetSpeedParameters, 17, 0x1cccc, 0xe666);
    v5 = 0;
    rec = Object_GetById(20);
    *(u16 *)(rec + 100) = v5;
    *(s32 *)(rec + 108) = 0x200809d;
    rec = Value1(Object_GetById, 19);
    *(u16 *)(rec + 100) = v5;
    *(s32 *)(rec + 108) = 0x200809d;
    rec = Value1(Object_GetById, 18);
    *(u16 *)(rec + 100) = v5;
    *(s32 *)(rec + 108) = 0x200809d;
    Object_SetModeById(20, 10);
    Object_SetModeById(19, 10);
    Object_SetModeById(18, 10);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 20, -24, 24);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 19, -24, 24);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 18, -24, 24);
    ObjectMotion_CommitCurrentPositionAndActivate(18);
    Call3(ObjectMotion_ResetAndSetPosition, 20, 224, 0x1e0);
    Call3(ObjectMotion_ResetAndSetPosition, 19, 224, 0x1e0);
    Call3(ObjectMotion_SetPositionAndCommit, 18, 224, 0x1e0);
    rec = Value1(Object_GetById, 20);
    *(s32 *)(rec + 108) = v5;
    rec = Value1(Object_GetById, 19);
    *(s32 *)(rec + 108) = v5;
    rec = Value1(Object_GetById, 18);
    *(s32 *)(rec + 108) = v5;
    Battle_WaitMode0(1);
    ObjectMotion_Launch(17, 6, 15);
    ObjectMotion_Launch(17, 6, 25);
    rec = Object_GetById(17);
    p10 = rec + 100;
    *(u16 *)p10 = v5;
    *(s32 *)(rec + 108) = 0x200809d;
    Object_SetModeById(17, 10);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 17, -20, 20);
    ObjectMotion_CommitCurrentPositionAndActivate(17);
    ObjectMotion_Launch(17, 4, 0);
    Call3(ObjectMotion_OffsetPositionAndResetMotion, 17, -20, 20);
    ObjectMotion_CommitCurrentPositionAndActivate(17);
    Object_SetModeById(17, 1);
    Func_02002a64(133);
    *(s32 *)(rec + 8) += 0x80000;
    *(s32 *)(rec + 108) = v5;
    *(s32 *)(rec + 16) += -0x80000;
    {
        s32 shown = 0xc000;

        *(u16 *)(rec + 6) = shown;
    }
    {
        s32 target = *(s32 *)(rec + 80);
        s32 shown = 0xa000;

        *(u16 *)(target + 18) = shown;
    }
    Battle_WaitMode0(55);
    *(s32 *)(rec + 8) += -0x40000;
    *(s32 *)(rec + 16) += 0x40000;
    *(u16 *)(*(s32 *)(rec + 80) + 18) = v5;
    {
        s32 shown = 0x4000;

        *(u16 *)(rec + 6) = shown;
    }
    ObjectMotion_Launch(17, 6, 40);
    Object_SetModeById(17, 11);
    Func_02002a64(135);
    Battle_WaitMode0(14);
    Func_02002a64(135);
    Battle_WaitMode0(14);
    Func_02002a64(135);
    Battle_WaitMode0(14);
    Func_02002a64(135);
    Battle_WaitMode0(14);
    Battle_WaitMode0(20);
    *(u16 *)p10 = v5;
    *(s32 *)(rec + 108) = 0x200809d;
    Object_SetModeById(17, 10);
    Call3(ObjectMotion_SetPositionAndCommit, 17, 224, 0x1d8);
    Battle_WaitMode0(30);
    *(s32 *)(rec + 108) = v5;
    Func_0200290c(20, 0, 0);
    Func_0200290c(19, 0, 0);
    Func_0200290c(18, 0, 0);
    Func_0200290c(17, 0, 0);
    Func_020027b0();
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(9, 4);
    Battle_WaitMode0(10);
    Func_0200294c(9, 0, 10);
    Call4(Motion_CamBounds, 0x1780000, -1, 0x1980000, 1);
    Func_0200298c();
    Battle_WaitMode0(10);
    Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
    ObjectMotion_ArmCallback(5, 0x4000, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Battle_WaitMode0(15);
    Func_0200294c(9, 0, 10);
    Call3(ObjectMotion_ArmCallback, 14, 0x2000, 0);
    ObjectMotion_ArmCallback(15, 0xa000, 0);
    Battle_WaitMode0(40);
    Call3(ObjectMotion_ArmCallback, 14, 0x8000, 0);
    Call3(ObjectMotion_ArmCallback, 15, 0x8000, 0);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_SetSpeedParameters, 14, 0x18000, 0xc000);
    Call3(ObjectMotion_CommitPositionAndActivate, 14, -32, 0);
    record = Object_GetById(9);
    {
        s32 shown = 0x2000;

        *(u16 *)((s32)record + 6) = shown;
    }
    Call3(Func_0200294c, 0x1009, 0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0x18000, 0xc000);
    ObjectMotion_CommitPositionAndActivate(9, 0, 16);
    ObjectMotion_ArmCallback(9, 0, 0);
    Battle_WaitMode0(10);
    Motion_SetVarCbAndRefresh(14, 2);
    Battle_WaitMode0(10);
    ObjectMotion_SetActionVariant(14, 3);
    Call3(ObjectMotion_SetSpeedParameters, 14, 0x20000, 0x10000);
    *(u8 *)(Object_GetById(14) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 14, 0x1c8, 0x1a8);
    Battle_WaitMode0(1);
    *(u8 *)(Object_GetById(14) + 90) |= 1;
    Call3(ObjectMotion_ArmCallback, 14, 0x6000, 0);
    Call3(ObjectMotion_SetSpeedParameters, 14, 0x10000, 0x8000);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(9, 3);
    Battle_WaitMode0(10);
    Call3(Func_0200294c, 0x1009, 0, 10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0xb333, 0x5999);
    ObjectMotion_CommitPositionAndActivate(9, 48, 0);
    Call3(ObjectMotion_ArmCallback, 9, 0x1000, 0);
    ObjectMotion_ArmCallback(5, 0x2000, 0);
    ObjectMotion_ArmCallback(8, 0x2000, 0);
    Call3(ObjectMotion_SetSpeedParameters, 15, 0x10000, 0x8000);
    *(u8 *)(Object_GetById(15) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 15, 0x1c8, 0x1b8);
    Battle_WaitMode0(1);
    *(u8 *)(Object_GetById(15) + 90) |= 1;
    Battle_WaitMode0(10);
    Value3(Func_02002964, 9, 0x108, 30);
    Call3(Func_0200294c, 0x1009, 0, 10);
    Motion_SetVarCbAndRefresh(14, 2);
    Battle_WaitMode0(10);
    Func_0200294c(14, 0, 10);
    ObjectMotion_CommitPositionAndActivate(9, 16, 0);
    Battle_WaitMode0(10);
    Value3(Func_02002964, 9, 0x107, 30);
    Call3(Func_0200294c, 0x1009, 0, 10);
    ObjectMotion_SetVariantCallback(14, 2);
    Motion_SetVarCbAndRefresh(15, 2);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_SetSpeedParameters, 14, 0x8000, 0x4000);
    Call3(ObjectMotion_SetSpeedParameters, 15, 0x8000, 0x4000);
    *(u8 *)(Object_GetById(14) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 14, 0x1e8, 0x1a8);
    Battle_WaitMode0(1);
    *(u8 *)(Object_GetById(14) + 90) |= 1;
    *(u8 *)(Object_GetById(15) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 15, 0x1e8, 0x1b8);
    Battle_WaitMode0(1);
    *(u8 *)(Object_GetById(15) + 90) |= 1;
    Call3(ObjectMotion_SetPositionAndReset, 9, 0x1c8, 0x1b0);
    ObjectMotion_ArmCallback(9, 0, 0);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(9, 3);
    Battle_WaitMode0(10);
    Call3(Func_0200294c, 0x1009, 0, 10);
    ObjectMotion_SetVariantCallback(14, 3);
    Call2(Func_0200296c, 14, 0x102);
    ObjectMotion_SetVariantCallback(15, 3);
    Call2(Func_0200296c, 15, 0x102);
    Battle_WaitMode0(50);
    *(u8 *)(Object_GetById(14) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 14, 0x1f8, 0x1a8);
    Battle_WaitMode0(1);
    *(u8 *)(Object_GetById(14) + 90) |= 1;
    Func_0200290c(14, 0, 0);
    *(u8 *)(Object_GetById(15) + 90) &= 254;
    Call3(ObjectMotion_SetPositionAndReset, 15, 0x1f8, 0x1b8);
    Battle_WaitMode0(1);
    record = Object_GetById(15);
    v2 = (1 | *(u8 *)(record + 90));
    *(u8 *)(record + 90) = v2;
    p8 = v2;
    Func_0200290c(15, 0, 0);
    Battle_WaitMode0(10);
    Call3(ObjectMotion_SetSpeedParameters, 9, 0xcccc, 0x6666);
    ObjectMotion_CommitPositionAndActivate(9, 64, 0);
    Func_0200290c(9, 0, 0);
    Battle_WaitMode0(30);
    Func_02002994(5, 1);
    Func_0200298c();
    Battle_WaitMode0(10);
    Value3(Func_02002964, 8, 0x105, 30);
    Func_0200294c(8, 0, 10);
    Motion_SetModeAndWaitAnimation(5, 3);
    Battle_WaitMode0(30);
    Motion_SetVarCbAndRefresh(8, 2);
    Battle_WaitMode0(10);
    Func_0200294c(8, 0, 10);
    Battle_WaitMode0(10);
    ObjectMotion_Launch(8, 4, 0);
    ObjectMotion_ArmCallback(8, 0, 0);
    Battle_WaitMode0(20);
    Func_0200294c(8, 0, 10);
    Value3(Func_02002964, 5, 0x105, 50);
    Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
    Battle_WaitMode0(20);
    Call3(Func_0200294c, 0x2005, 0, 10);
    Value3(Func_02002964, 8, 0x101, 30);
    Func_0200294c(8, 0, 10);
    Call3(Func_02002964, 5, 0x109, 80);
    Battle_WaitMode0(10);
    Call3(Func_02002964, 8, 0x106, 40);
    Value2(UiText_OpenMessageAtObject, 8, 0);
    if (Value2(Inventory_PromptAndSetObjectMode, 5, 0) == 0) {
        Battle_WaitMode0(20);
        Func_0200294c(8, 0, 10);
        *(u16 *)((*(s32 *)0x0300006c + 0x1c4)) += 1;
    } else {
        Battle_WaitMode0(40);
        *(u16 *)((*(s32 *)0x0300006c + 0x1c4)) += 1;
        Func_0200294c(8, 0, 10);
    }
    Motion_SetModeAndWaitAnimation(5, 3);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Battle_WaitMode0(10);
    Func_0200294c(8, 0, 10);
    ObjectMotion_ArmCallback(8, 0, 0);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(5, 3);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Battle_WaitMode0(20);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x13333, 0x9999);
    Object_SetModeById(8, 2);
    record = Value1(Object_GetById, 5);
    if ((s32)record != 0) {
        ObjectMotion_ResetAndSetPosition(8, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(8);
    Func_0200290c(8, 0, 0);
    Object_AttachWorkTargetToObject(5, 1);
    Func_020028bc();
}
