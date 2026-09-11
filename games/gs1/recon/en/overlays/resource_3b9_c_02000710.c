#include "types.h"

#define Scene_RunBranchingActorPresentation Func_02000710

/* Scene_RunBranchingActorPresentation - resource_3b9:02000710, 1724 bytes.
 *
 * One field-scene presentation script guarded by a single flag query. When the
 * flag is clear the routine returns at once. Otherwise it resets the battle
 * runtime, builds the four presentation records 17..20 (tint, timing, layer and
 * motion fields), then places actors 0..3, taking each optional party record's
 * position pair at +8 and +16. It then runs one of three closing sequences
 * chosen by two further queries. Every closing sequence installs its text pair,
 * runs the same fade/hold pattern and stores 40 into the scene work field at
 * +0x1c8; the two later sequences bump the halfword step counter at +0x1d8
 * first. All three paths leave through the common tail at the end.
 *
 * Retained oddity: the fourth presentation record is prepared as 20 throughout
 * but handed to the record-4 call at owner offset 0x42e as 19. The reference
 * does this, so the draft does too.
 *
 * Uncertain: the roles of the record indices, the layout of the party record
 * behind +8/+16, and the layout of the scene work block reached through the
 * pointer at 0x03001ebc. Only the offsets this owner touches are spelled out
 * and no struct is invented for them. The argument roles of the unnamed
 * veneers below are unknown; their argument counts follow the reference.
 *
 * Residual: candidate=1724 reference=1724 topology=equal,
 * differing_halfwords=7, wrong_instructions=14, class=allocation-uncovered.
 * Every call target, branch, loop-free store and side effect matches. The one
 * residual is local register allocation in the FIRST step-counter bump at
 * owner offset 0x3de: the reference holds 236 in r3 and the work pointer in
 * r2, the candidate holds them the other way round. Instruction order, widths
 * and offsets are identical; it is a pure register rename over seven
 * halfwords. The SECOND, textually identical bump compiles to the reference
 * allocation exactly, so the difference is driven by surrounding context, not
 * by how the statement is spelled. Four spellings were tried - address literal,
 * extern data symbol, reversed addition operands, and an explicit u16 * local
 * in a nested block - and all four measure 7. Parked as an allocator floor,
 * the same class the sibling resource_3b9:02001cd4 draft parks.
 */

extern u8 Data_0200ad3c[];
extern u8 Data_0200ad74[];
extern u8 Data_0200ade4[];

/* Shared scene work record, reached through the pointer at 0x03001ebc. The two
 * field offsets are the ones the adopted sibling
 * games/gs1/src/overlays/scene/script/run_scene_3b9_conditional_scene_setup.c
 * already spells. */

extern u8 Data_03001ebc[];
#define SCENE_WORK (*(u8 **)Data_03001ebc)
static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u16 *step = (u16 *)(*(u8 **)Data_03001ebc + 0x1d8);

    *step = *step + amount;
}
#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))

/* Overlay call bindings.
 *
 * The loader rewrites every BL-shaped halfword pair, so the word an overlay
 * image stores for a call site folds in that site's own position: the legacy
 * Func_<address> alias a disassembler prints is per site, not per callee. The
 * 177 call sites in this owner reach only 32 distinct runtime targets. Each
 * declaration below is one target, spelled once with an alias the reference
 * binds to that target and to no other, so every site calling it can use the
 * same name. Its comment gives the overlay veneer and the main-image entry
 * that veneer reaches.
 *
 * Three targets - 0x0200abc4, 0x0200ac94 and 0x0200acec - have no unambiguous
 * alias anywhere in this owner: each of their aliases also names a second
 * target. They are spelled with a Data_<runtime address> name, which reaches
 * an overlay-space address directly instead of going through the per-site
 * alias table, and the emitted call word matches the reference. The sibling
 * draft resource_3b9:02001cd4 uses the same device. If this owner is ever
 * given a translation-unit entry, that entry should declare the runtime
 * symbols and these three can go back to the ordinary Func_ form.
 *
 * Readable names are reading aids, not recovered spellings. Ten come from the
 * adopted source
 * games/gs1/src/overlays/scene/script/run_scene_3b9_conditional_scene_setup.c,
 * which reaches the same veneer table; twelve more come from the sibling draft
 * games/gs1/recon/en/overlays/resource_3b9_c_02001cd4.c, which documents the
 * same veneer-to-main pairs but is itself unadopted; and the two overlay-local
 * targets carry the names alchemy inspect prints for them. The veneer at
 * 0x0200ac14 is left unnamed on purpose: that sibling draft calls it
 * Motion_SetSpeed, which the adopted source already uses for a
 * different main entry (0x0808a370), so one of the two spellings is wrong and
 * this owner does not pick a side. Eight targets nobody has named keep their
 * alias.
 */

/* overlay 0x02001c48, runtime 0x02009c48, 18 sites */
void Func_02002410();
#define Scene_CallPairWith10 Func_02002410
/* overlay 0x02001c5c, runtime 0x02009c5c, 8 sites */
void Func_0200244a();
#define State_ForwardMaskedHalfwordWith10 Func_0200244a
/* veneer 0x0200abbc -> main 0x080770c0, 1 site */
s32 Func_020032d2();
#define GameFlag_IsSet Func_020032d2
/* veneer 0x0200abc4 -> main 0x080770c8, 1 site */
void Data_0200abc4();
/* veneer 0x0200abec -> main 0x0808a010, 17 sites */
void Func_020033a2();
#define Battle_WaitMode0 Func_020033a2
/* veneer 0x0200abf4 -> main 0x0808a018, 1 site */
void Func_02003314();
#define Battle_Reset Func_02003314
/* veneer 0x0200abfc -> main 0x0808a020, 1 site */
void Func_020039a2();
#define Battle_SchedShoulder Func_020039a2
/* veneer 0x0200ac04 -> main 0x0808a070, 2 sites */
s32 Func_02003634();
#define UiWork_WaitThenFinalizeCapacity Func_02003634
/* veneer 0x0200ac0c -> main 0x0808a080, 3 sites */
s32 Func_02003548();
/* veneer 0x0200ac14 -> main 0x0808a090, 11 sites */
void Func_0200351e();
/* veneer 0x0200ac1c -> main 0x0808a098, 11 sites */
void Func_02003668();
#define Motion_EnableActCb Func_02003668
/* veneer 0x0200ac2c -> main 0x0808a0b0, 4 sites */
void Func_02003688();
#define Motion_MarkActiveAndSetActionCallback Func_02003688
/* veneer 0x0200ac44 -> main 0x0808a0c8, 2 sites */
void Func_020035ca();
#define Motion_ResetPosMode2 Func_020035ca
/* veneer 0x0200ac4c -> main 0x0808a0d0, 4 sites */
void Func_0200354c();
#define Motion_SetPosReset Func_0200354c
/* veneer 0x0200ac54 -> main 0x0808a0f0, 3 sites */
void Func_020035b2();
#define Motion_SetHPosTerrain Func_020035b2
/* veneer 0x0200ac5c -> main 0x0808a100, 10 sites */
void Func_02003510();
#define Object_SetModeById Func_02003510
/* veneer 0x0200ac6c -> main 0x0808a110, 11 sites */
void Func_0200343c();
#define Motion_CallWaitAnim Func_0200343c
/* veneer 0x0200ac74 -> main 0x0808a130, 6 sites */
void Func_020033d8();
#define Motion_SetVarCb Func_020033d8
/* veneer 0x0200ac7c -> main 0x0808a138, 8 sites */
void Func_020033a4();
#define Motion_SetVarCbObj Func_020033a4
/* veneer 0x0200ac94 -> main 0x0808a170, 1 site */
void Data_0200ac94();
#define SceneWork_SetStepValue Data_0200ac94
/* veneer 0x0200ac9c -> main 0x0808a178, 2 sites */
void Func_020036ac();
/* veneer 0x0200acac -> main 0x0808a188, 5 sites */
void Func_020034c4();
/* veneer 0x0200acbc -> main 0x0808a1b8, 21 sites */
void Func_020033fa();
#define Motion_ArmCb Func_020033fa
/* veneer 0x0200acc4 -> main 0x0808a1e8, 10 sites */
void Func_02003486();
#define BattleFx_SpawnLinked Func_02003486
/* veneer 0x0200accc -> main 0x0808a1f0, 1 site */
void Func_020034ac();
/* veneer 0x0200acdc -> main 0x0808a208, 4 sites */
void Func_02003476();
#define Motion_SetSpeedLim Func_02003476
/* veneer 0x0200ace4 -> main 0x0808a210, 4 sites */
void Func_0200375a();
#define Motion_CamBounds Func_0200375a
/* veneer 0x0200acec -> main 0x0808a218, 1 site */
void Data_0200acec();
#define Object_CommitPositionThenWaitIfModeZero Data_0200acec
/* veneer 0x0200acf4 -> main 0x0808a228, 1 site */
u8 *Func_02003480();
#define RuntimeBlock_GetOffset1e0Pointer Func_02003480
/* veneer 0x0200acfc -> main 0x0808a248, 1 site */
void Func_02003a98();
/* veneer 0x0200ad24 -> main 0x0808a368, 2 sites */
void Func_020039be();
/* veneer 0x0200ad2c -> main 0x0808a370, 2 sites */
void Func_020039ca();
#define Motion_SetSpeed Func_020039ca

/* A call site spelled through one of these wrappers passes its constants
 * straight into the argument registers; a direct call lets the compiler
 * precompute a constant into a pseudo and share it with later uses in the same
 * block. Both shapes occur here, and the choice at each site is the one the
 * reference's argument setup shows. The adopted sibling
 * run_scene_3b9_conditional_scene_setup.c uses the same device. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_RunBranchingActorPresentation(void)
{
    s32 record;
    s32 value;
    u8 *slot;

    if (Value1(GameFlag_IsSet, 0x962) == 0) {
        return;
    }

    Battle_Reset();
    Motion_SetVarCbObj(17, 1);
    Call3(Motion_ArmCb, 17, 0x3000, 20);
    value = 192;
    Motion_ArmCb(17, 0, 60);
    Call3(BattleFx_SpawnLinked, 17, 0x100, 40);
    State_ForwardMaskedHalfwordWith10(17, (value << 6));
    Call1(SceneWork_SetStepValue, 0x2267);
    Motion_SetVarCb(17, 2);
    Scene_CallPairWith10(17);
    Motion_ArmCb(18, (value << 6), 0);
    Motion_ArmCb(19, (value << 6), 0);
    Value3(Motion_ArmCb, 20, (value << 6), 0);
    slot = RuntimeBlock_GetOffset1e0Pointer();
    *(u8 *)(slot + 85) = 0;
    Call2(Motion_SetSpeedLim, 0x19999, 0x3333);
    Call4(Motion_CamBounds, 0x1000000, -1, 0xac0000, 1);
    Object_CommitPositionThenWaitIfModeZero();
    Battle_WaitMode0(20);
    Call3(BattleFx_SpawnLinked, 18, 0x102, 40);
    Scene_CallPairWith10(18);
    Motion_CallWaitAnim(17, 3);
    Scene_CallPairWith10(17);
    Call2(Func_020034ac, 19, 0x102);
    Battle_WaitMode0(40);
    State_ForwardMaskedHalfwordWith10(19, 0);
    Scene_CallPairWith10(19);
    Call3(BattleFx_SpawnLinked, 20, 0x103, 40);
    State_ForwardMaskedHalfwordWith10(20, 0);
    Motion_SetVarCb(20, 2);
    Func_020034c4(20, 0, 20);
    Motion_SetVarCbObj(17, 1);
    Battle_WaitMode0(10);
    Call3(Motion_ArmCb, 17, 0xb000, 20);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(18, 1);
    Motion_CallWaitAnim(18, 4);
    Battle_WaitMode0(20);
    State_ForwardMaskedHalfwordWith10(17, 0x8000);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(19, 1);
    Battle_WaitMode0(40);
    Motion_CallWaitAnim(19, 4);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(20, 2);
    Battle_WaitMode0(40);
    Motion_CallWaitAnim(20, 3);
    Battle_WaitMode0(20);
    Object_SetModeById(17, 3);
    Object_SetModeById(18, 3);
    Object_SetModeById(19, 3);
    Motion_CallWaitAnim(20, 3);
    Motion_ArmCb(17, (value << 6), 0);
    Motion_ArmCb(18, (value << 6), 0);
    Motion_ArmCb(19, (value << 6), 0);
    State_ForwardMaskedHalfwordWith10(20, (value << 6));
    Call3(Func_0200351e, 17, 0x9999, 0x4ccc);
    Call3(Motion_SetPosReset, 17, 0x102, 172);
    Call3(Func_0200351e, 0, 0xcccc, 0x6666);
    Call3(Motion_SetPosReset, 0, 0x106, 188);
    Call3(Motion_ArmCb, 0, 0xc000, 0);
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_0200351e, 1, 0xcccc, 0x6666);
    Call3(Func_0200351e, 2, 0xcccc, 0x6666);
    Call3(Func_0200351e, 3, 0xcccc, 0x6666);
    Motion_ResetPosMode2(1, 246, 200);
    Call3(Motion_ResetPosMode2, 2, 0x106, 200);
    Call3(Motion_SetPosReset, 3, 0x116, 200);
    Object_SetModeById(2, 1);
    Object_SetModeById(1, 1);
    Call3(Motion_ArmCb, 1, 0xc000, 0);
    Call3(Motion_ArmCb, 2, 0xc000, 0);
    Call3(Motion_ArmCb, 3, 0xc000, 40);
    Motion_SetVarCbObj(17, 1);
    Scene_CallPairWith10(17);
    Motion_SetVarCb(0, 2);
    Motion_SetVarCb(1, 2);
    Motion_SetVarCb(2, 2);
    Motion_SetVarCbObj(3, 2);
    Motion_CallWaitAnim(17, 3);
    Value2(Func_020036ac, 17, 0);
    Call3(Motion_ArmCb, 1, 0xe000, 0);
    Call3(Motion_ArmCb, 3, 0xa000, 0);

    if (Value2(UiWork_WaitThenFinalizeCapacity, 0, 0) == 0) {
        Motion_CallWaitAnim(17, 3);
        Scene_CallPairWith10(17);
        Motion_EnableActCb(1, (s32)Data_0200ad3c);
        Value2(Motion_EnableActCb, 2, (s32)Data_0200ad3c);
        Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad3c);
        Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
        Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
        Call3(Func_0200351e, 17, 0x10000, 0x8000);
        Motion_EnableActCb(17, (s32)Data_0200ade4);
        Battle_WaitMode0(10);
        Call3(Func_0200351e, 0, 0x10000, 0x8000);
        Value2(Motion_EnableActCb, 0, (s32)Data_0200ade4);
        Battle_WaitMode0(80);
        SCENE_FIELD_1C8 = 40;
        Func_020039be();
        Motion_SetSpeed();
    } else {
        bump_step(1);
        Call3(BattleFx_SpawnLinked, 17, 0x102, 40);
        Scene_CallPairWith10(17);
        Call3(BattleFx_SpawnLinked, 18, 0x102, 40);
        Scene_CallPairWith10(18);
        Call3(BattleFx_SpawnLinked, 19, 0x102, 40);
        Scene_CallPairWith10(19);
        Call3(BattleFx_SpawnLinked, 20, 0x102, 40);
        Scene_CallPairWith10(19);
        Motion_CallWaitAnim(17, 4);
        Scene_CallPairWith10(17);
        Motion_SetVarCbObj(2, 1);
        Battle_WaitMode0(20);
        Scene_CallPairWith10(2);
        Call3(BattleFx_SpawnLinked, 3, 0x102, 40);
        Scene_CallPairWith10(3);
        Call3(Motion_ArmCb, 17, 0xb000, 0);
        Motion_ArmCb(19, 0x8000, 0);
        Motion_ArmCb(20, 0, 60);
        Motion_ArmCb(17, (value << 6), 0);
        Motion_ArmCb(19, (value << 6), 0);
        Motion_ArmCb(20, (value << 6), 20);
        Object_SetModeById(17, 3);
        Object_SetModeById(18, 3);
        Object_SetModeById(19, 3);
        Motion_CallWaitAnim(20, 3);
        Call3(BattleFx_SpawnLinked, 3, 0x100, 60);
        Call2(State_ForwardMaskedHalfwordWith10, 3, 0xa000);
        Scene_CallPairWith10(3);
        Object_SetModeById(1, 4);
        Battle_WaitMode0(20);
        Value2(Func_020036ac, 1, 0);

        if (Value2(UiWork_WaitThenFinalizeCapacity, 0, 0) == 0) {
            Motion_CallWaitAnim(17, 3);
            Scene_CallPairWith10(17);
            Motion_EnableActCb(1, (s32)Data_0200ad3c);
            Value2(Motion_EnableActCb, 2, (s32)Data_0200ad3c);
            Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad3c);
            Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
            Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
            Call3(Func_0200351e, 17, 0x10000, 0x8000);
            Motion_EnableActCb(17, (s32)Data_0200ade4);
            Battle_WaitMode0(10);
            Call3(Func_0200351e, 0, 0x10000, 0x8000);
            Value2(Motion_EnableActCb, 0, (s32)Data_0200ade4);
            Battle_WaitMode0(80);
            SCENE_FIELD_1C8 = 40;
            Func_020039be();
            Motion_SetSpeed();
        } else {
            bump_step(1);
            Battle_WaitMode0(20);
            Motion_SetVarCb(1, 2);
            Func_020034c4(1, 0, 20);
            Call3(BattleFx_SpawnLinked, 2, 0x102, 60);
            Scene_CallPairWith10(2);
            State_ForwardMaskedHalfwordWith10(3, 0x8000);
            Object_SetModeById(3, 3);
            Scene_CallPairWith10(3);
            Motion_EnableActCb(2, (s32)Data_0200ad74);
            Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad74);
            Battle_WaitMode0(20);
            Value2(Motion_MarkActiveAndSetActionCallback, 0, (s32)Data_0200ad74);
            Call3(Motion_SetPosReset, 1, 0x106, 188);
            Value2(State_ForwardMaskedHalfwordWith10, 1, 0xc000);
            Motion_CallWaitAnim(1, 3);
            Scene_CallPairWith10(1);
            Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
            Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
            Call3(Func_0200351e, 17, 0x10000, 0x8000);
            Motion_EnableActCb(17, (s32)Data_0200ade4);
            Battle_WaitMode0(10);
            Call3(Func_0200351e, 1, 0x10000, 0x8000);
            Value2(Motion_EnableActCb, 1, (s32)Data_0200ade4);
            Battle_WaitMode0(80);
            SCENE_FIELD_1C8 = 40;
            Func_020039be();
            Motion_SetSpeed();
        }
    }

    Func_02003a98(2);
    Call1(Data_0200abc4, 0x93f);
    Battle_SchedShoulder();
}
