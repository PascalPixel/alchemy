#include "TYPES.H"
#include "FIELD_EVENT.H"

#define Audio_PlayCue_1(a0) Value1(Engine_AudioPlayCue, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02006af6, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02006b04, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Value4(Func_02006b12, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Engine_ActorWaitForMove, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Engine_EventWait, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_4(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_5(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_6(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_7(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleEvent_RunActionAndWait_8(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_16(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_9(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_10(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEvent_RunActionAndWait_12(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_2(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_14(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_15(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_24(a0) Value1(Engine_EventWait, a0)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_26(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_16(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_27(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_17(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleEvent_RunActionAndWait_18(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_29(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_30(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_31(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_20(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_32(a0) Value1(Engine_EventWait, a0)
#define SceneWork_SetStepValue_2(a0) Value1(Engine_EventSetMessage, a0)
#define BattleRuntime_WaitIfModeZero_33(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CommitPositionAndActivate_1(a0, a1, a2) Value3(Engine_ActorWalkByAndWait, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_21(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Engine_EventSetMessage, a0)
#define BattleRuntime_WaitIfModeZero_35(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CommitPositionAndActivate_2(a0, a1, a2) Value3(Engine_ActorWalkByAndWait, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_37(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_22(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define SceneWork_SetStepValue_4(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_CommitPositionAndActivate_3(a0, a1, a2) Value3(Engine_ActorWalkByAndWait, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_39(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_40(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetAngleToward_6(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_41(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_24(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_CommitPositionAndActivate_4(a0, a1, a2) Value3(Engine_ActorWalkByAndWait, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_43(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_6(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_44(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetAngleToward_7(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_45(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_25(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_46(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_47(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CommitPositionAndActivate_5(a0, a1, a2) Value3(Engine_ActorWalkByAndWait, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_48(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define SceneWork_SetStepValue_5(a0) Value1(Engine_EventSetMessage, a0)
#define BattleRuntime_WaitIfModeZero_49(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_26(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_50(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_9(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_51(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_27(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleEvent_RunActionAndWait_28(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_53(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_10(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_54(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_29(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_55(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_11(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_56(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_30(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_57(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_58(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_31(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_59(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_32(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_60(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_33(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_62(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_34(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_35(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_65(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_66(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_36(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_67(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_37(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_69(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_38(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleEvent_RunActionAndWait_39(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_71(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_40(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_72(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_73(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_12(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_75(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_42(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_13(a0, a1) Value2(Engine_ActorSetAnimationAndWait, a0, a1)
#define BattleRuntime_WaitIfModeZero_77(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetAngleToward_10(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_78(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_79(a0) Value1(Engine_EventWait, a0)
#define Audio_PlayCue_2(a0) Value1(Engine_AudioPlayCue, a0)
#define Object_SetModeById_2(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_020070ea, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_3(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_0200711a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_4(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define Scene_GetRecord_3(a0) Value1(Func_0200714a_a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Engine_ActorWaitForMove, a0)
#define Audio_PlayCueForPartyMember_1() Call0(Func_02007298)
#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097
#define PrimaryOverlayData Data_0200c1b0
#define SecondaryOverlayData Data_0200c270
#define TertiaryOverlayData Data_0200c318
#define DefaultOverlayData Data_0200c198
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

typedef struct {
    s32 a, b, c, d, e, f;
} S6;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ent_02000d58;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec;

extern u8 Data_0200bf70[];
extern u8 Data_0200c138[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 Data_0200c688[];
extern u8 Data_0200c724[];
extern u8 Data_0200c76c[];
extern u8 Data_0200c808[];
extern u8 Data_0200c850[];
extern u8 Data_0200c5e0[];
extern u8 Data_02001004;
extern u8 Value_0000217f;
extern u16 *Data_0200bf6c;
extern s8 Data_02001002;
extern u8 Data_02008ee1;

void Func_02004794(s32, s32);
void Func_02006a42();
void Func_02006af6();
void Func_02006b04();
void Func_02006b12();
u8 *Func_020070ea();
u8 *Func_0200711a();
u8 *Func_0200714a_a();
void Func_02007298();
void Func_02001528(void);
void Func_02001a48(void);
void Func_020014da(S6);
void Func_0200490a(s32);
void Func_020015ce_a(void);
Ent *Func_02004a2c(s32);
s32 Func_020049ce(Ent *, Vec *);
u8 *Func_02004a64(s32);
u8 *Func_02004bfa(s32);
void Func_02004b68(s32, s32, s32 *);
u8 *Func_02004cea(s32);
void Func_02007684(s32 first, s32 second, s32 *position);
s32 Func_02007898(void *object);
s32 Func_020078b0(void *object);
void Func_020078da(void *object);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/*
 * Primary cutscene script -- resource_3bd. A no-argument, void, almost-linear
 * sequence whose one branch rejoins after both arms.
 */

/*
 * Each alias is the per-site call word the overlay image holds -- one word can
 * serve two sites with different targets -- and the macro names the engine
 * function the site reaches, keeping the site's own calling form. Names
 * without a binding elsewhere in the tree are provisional.
 */

/* Signed coordinate halfwords in a placed-actor record, named by offset. */
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void FieldScene_SetActor13Value41(void)
{
    Func_02004794(13, 0x41);
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */
u8 *SceneData_GetTablebf70(void)
{
    return Data_0200bf70;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    return Data_0200c138;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR_BODY.INC"
}

void SceneState_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Func_02001528();
    }
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */
void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetByte1004AndRunWhenIdle(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetByte1004AndRunWhenIdle(1);
}

void FieldScene_RunIndexedStep2(void)
{
    SceneState_SetByte1004AndRunWhenIdle(2);
}

void FieldScene_RunIndexedStep3(void)
{
    SceneState_SetByte1004AndRunWhenIdle(3);
}

void FieldScene_RunIndexedStep4(void)
{
    SceneState_SetByte1004AndRunWhenIdle(4);
}

void FieldScene_RunIndexedStep5(void)
{
    SceneState_SetByte1004AndRunWhenIdle(5);
}

void SceneEffect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Task_Wait(1);
    Func_0200490a(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (GameFlag_IsSet(0x201) != 0) {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x1d1d;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 0x3f;
            *slot = c;
        }
        Func_020015ce_a();
        return;
    } else {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x3f3f;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 31;
            *slot = c;
        }
    }
    {
        s32 a = 0x3f42;
        *(u16 *)0x4000050 = a;
    }
    {
        s32 b = 0xc04;
        *(u16 *)0x4000052 = b;
    }
}

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{

    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (GameFlag_IsSet(0x200) != 0) {
        SceneEffect_SetupBlendByFlag201();
        work[0x34] = 1;
    }
    tbl = Data_02000240;
    if (tbl[0xe0] == (s32)&Value_00000097) {
        Actor_SetChildValue(16, 6);
        Actor_SetChildValue(17, 6);
        Actor_SetChildValue(18, 6);
        Actor_SetChildValue(19, 6);
        Actor_SetChildValue(20, 6);
    }
}

void FieldScene_RunTwoCallSequence(void)
{
    StagedActor_AdvancePair();
    Func_02001a48();
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneState_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    Ent_02000d58 *p = Func_02004a2c(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Func_020049ce(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void FieldScene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Func_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Map_CopyCellAttributes(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        GameFlag_Set(0x211);
    }
}

void FieldScene_RunGuardedSixWordStep(void)
{
    S6 s;

    Event_Begin();
    if (StagedActor_FindClearPosition(&s) != 0) {
        Func_020014da(s);
    }
    Event_End();
}

void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = Func_02004bfa(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Func_02004b68(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneActor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}

void SceneEffect_UpdateStateMachine(void)
{
    u16 *p = Data_0200bf6c;
    s32 flag = 1;
    s32 state = *(s16 *)p;

    if (state == 0) {
        s32 t = p[4] + 16;
        p[4] = t;
        if ((u16)t > 0xbff) {
            p[0] = p[0] + 1;
            p[1] = state;
        }
    } else if (state == 1) {
        if ((s16)p[1] == 30) {
            p[0] = p[0] + 1;
        }
    } else if (state == 2) {
        s32 t = p[4] + 0xfff8;
        p[4] = t;
        if ((u16)t <= 0x2ff) {
            p[0] = p[0] + 1;
        }
    } else if (state == 3) {
        s32 v = Data_02001002;
        s32 r = Math_Divide(v << 16, 5);
        if ((unsigned int)(((p[3] - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
            u8 *o;
            s32 nv = r + 0x4000;
            p[3] = nv;
            {
                s32 z = 0;
                s32 k = 0x63;
                p[0] = k;
                p[4] = z;
            }
            o = Func_02004cea(v + 11);
            *(s32 *)(o + 0x6c) = (s32)&Data_02008ee1;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        Data_0200bf6c[3] += Data_0200bf6c[4];
        SceneActor_PlaceFiveActorsInRow(Data_0200bf6c[3]);
        q2 = Data_0200bf6c;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Audio_PlayCue(0x87);
            }
        }
    }
    {
        u16 *q = Data_0200bf6c;
        q[1]++;
    }
}

/*
 * Every call site is written out separately and repeated calls must not be
 * folded: the sequence of distinct call words is what reproduces the
 * reference. The three record lookups near the end are null checked before
 * their stored coordinates are forwarded.
 */
void FieldScene_RunBranchingCutsceneSequence(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set(0x960);
    Audio_PlayCue_1(24);
    Event_Begin();
    Value0(Func_02006a42); /* main:0808a460 */
    Event_SetMessage(0x214f);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleEvent_RunActionAndWait_1(8, 0);
    BattleRuntime_WaitIfModeZero_2(10);
    Camera_MoveTo(0xf80000, -1, 0xb80000, 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 248, 192);
    Actor_FaceDirection(0, 0xc000, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -16, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_2(3, 0, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_3(2, 16, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x102, 0);
    BattleRuntime_WaitIfModeZero_4(40);
    Event_ShowMessage(2, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(3, 2);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleEvent_RunActionAndWait_3(3, 0);
    BattleRuntime_WaitIfModeZero_6(10);
    Actor_ShowEmote(8, 0x100, 40);
    BattleEvent_RunActionAndWait_4(8, 0);
    BattleRuntime_WaitIfModeZero_7(10);
    Actor_FaceEachOther(1, 2, 50);
    Actor_FaceDirection(1, 0xc000, 0);
    ObjectMotion_ArmCallback_3(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_8(30);
    BattleEvent_RunActionAndWait_5(1, 0);
    BattleRuntime_WaitIfModeZero_9(10);
    Actor_SetAttachedEffect(8, 0x102); /* main:0808a1f0 */
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_11(30);
    BattleEvent_RunActionAndWait_6(8, 0);
    BattleRuntime_WaitIfModeZero_12(20);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(3, 0xc000, 0);
    ObjectMotion_ArmCallback_5(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_13(30);
    BattleEvent_RunActionAndWait_7(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(8, 0x100, 40);
    BattleEvent_RunActionAndWait_8(8, 0);
    BattleRuntime_WaitIfModeZero_15(10);
    Actor_SetAnimationAndWait(3, 4);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleEvent_RunActionAndWait_9(3, 0);
    BattleRuntime_WaitIfModeZero_17(10);
    Actor_ShowEmote(8, 0x105, 40);
    BattleEvent_RunActionAndWait_10(8, 0);
    Event_Wait(10);
    Actor_FaceActor(1, 0, 30);
    Actor_ShowEmote(1, 0x101, 40);
    Event_ShowMessage(1, 0);
    ObjectMotion_ArmCallback_6(1, 0xc000, 0);
    Event_Wait(20);
    BattleEvent_RunActionAndWait_12(1, 0);
    BattleRuntime_WaitIfModeZero_20(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(8, 2);
    BattleRuntime_WaitIfModeZero_21(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
    BattleRuntime_WaitIfModeZero_22(20);
    BattleEvent_RunActionAndWait_14(3, 0);
    BattleRuntime_WaitIfModeZero_23(30);
    Actor_ShowEmote(8, 0x106, 40);
    BattleEvent_RunActionAndWait_15(8, 0);
    BattleRuntime_WaitIfModeZero_24(10);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_11(2, 0x101, 0);
    Event_Wait(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(2, 2);
    BattleRuntime_WaitIfModeZero_26(20);
    BattleEvent_RunActionAndWait_16(2, 0);
    BattleRuntime_WaitIfModeZero_27(20);
    BattleEvent_RunActionAndWait_17(8, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x102, 40);
    BattleEvent_RunActionAndWait_18(1, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 2);
    BattleRuntime_WaitIfModeZero_30(20);
    Event_ShowMessage(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(3, 2);
    BattleRuntime_WaitIfModeZero_31(20);
    BattleEvent_RunActionAndWait_20(3, 0);
    BattleRuntime_WaitIfModeZero_32(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_OpenMessage(8, 0); /* main:0808a178 */
    Actor_FaceActor(1, 0, 0);
    Actor_FaceActor(2, 0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        SceneWork_SetStepValue_2(0x2164);
        BattleRuntime_WaitIfModeZero_33(20);
        Actor_ShowEmote(1, 0x100, 40);
        Actor_SetSpeed(1, 0x20000, 0x10000);
        ObjectMotion_CommitPositionAndActivate_1(1, 0, -16);
        BattleRuntime_WaitIfModeZero_34(10);
        Actor_FaceEachOther(1, 0, 30);
        BattleEvent_RunActionAndWait_21(1, 0);
    } else {
        SceneWork_SetStepValue_3(0x2168);
        BattleRuntime_WaitIfModeZero_35(10);
        Actor_SetSpeed(1, 0x10000, 0x8000);
        ObjectMotion_CommitPositionAndActivate_2(1, 0, -16);
        BattleRuntime_WaitIfModeZero_36(10);
        Actor_FaceEachOther(1, 0, 30);
        Actor_SetAnimationAndWait(1, 3);
        BattleRuntime_WaitIfModeZero_37(20);
        BattleEvent_RunActionAndWait_22(1, 0);
    }

    line = 0x2165;
    SceneWork_SetStepValue_4(line);
    Actor_ShowEmote(2, 0x103, 40);
    Actor_SetSpeed(2, 0x20000, 0x10000);
    ObjectMotion_CommitPositionAndActivate_3(2, 0, -16);
    BattleRuntime_WaitIfModeZero_38(10);
    Actor_FaceEachOther(2, 0, 30);
    Event_ShowMessage(2, 0);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_39(10);
    ObjectMotion_CallThenWaitForAnimationChange_5(3, 3);
    BattleRuntime_WaitIfModeZero_40(20);
    Actor_FaceActor(0, 3, 0);
    Actor_FaceActor(1, 3, 0);
    ObjectMotion_SetAngleToward_6(2, 3, 0);
    BattleRuntime_WaitIfModeZero_41(20);
    BattleEvent_RunActionAndWait_24(3, 0);
    Event_Wait(20);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_4(1, 0, 16);
    BattleRuntime_WaitIfModeZero_43(30);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 4);
    BattleRuntime_WaitIfModeZero_44(30);
    ObjectMotion_SetAngleToward_7(1, 0, 0);
    BattleRuntime_WaitIfModeZero_45(20);
    BattleEvent_RunActionAndWait_25(1, 0);
    BattleRuntime_WaitIfModeZero_46(30);
    Actor_FaceEachOther(3, 2, 30);
    Actor_SetAnimationAndWait(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    BattleRuntime_WaitIfModeZero_47(30);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_5(2, 0, 16);
    BattleRuntime_WaitIfModeZero_48(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    ObjectMotion_ArmCallback_10(2, 0xc000, 0);
    line += 7;
    SceneWork_SetStepValue_5(line);
    BattleRuntime_WaitIfModeZero_49(30);
    Actor_ShowEmote(8, 0x100, 40);
    BattleEvent_RunActionAndWait_26(8, 0);
    BattleRuntime_WaitIfModeZero_50(10);
    ObjectMotion_CallThenWaitForAnimationChange_9(2, 3);
    BattleRuntime_WaitIfModeZero_51(30);
    BattleEvent_RunActionAndWait_27(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(8, 0x108, 40);
    BattleEvent_RunActionAndWait_28(8, 0);
    BattleRuntime_WaitIfModeZero_53(10);
    ObjectMotion_CallThenWaitForAnimationChange_10(2, 3);
    BattleRuntime_WaitIfModeZero_54(30);
    BattleEvent_RunActionAndWait_29(8, 0);
    BattleRuntime_WaitIfModeZero_55(10);
    ObjectMotion_CallThenWaitForAnimationChange_11(3, 3);
    BattleRuntime_WaitIfModeZero_56(30);
    BattleEvent_RunActionAndWait_30(8, 0);
    BattleRuntime_WaitIfModeZero_57(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    ObjectMotion_ArmCallback_12(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_58(30);
    BattleEvent_RunActionAndWait_31(8, 0);
    BattleRuntime_WaitIfModeZero_59(10);
    Actor_ShowEmote(3, 0x101, 40);
    BattleEvent_RunActionAndWait_32(3, 0);
    BattleRuntime_WaitIfModeZero_60(30);
    BattleEvent_RunActionAndWait_33(8, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(0, 0xc000, 0);
    ObjectMotion_ArmCallback_14(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_62(30);
    BattleEvent_RunActionAndWait_34(8, 0);
    Event_Wait(10);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(3, 0xc000, 0);
    ObjectMotion_ArmCallback_16(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_64(30);
    BattleEvent_RunActionAndWait_35(8, 0);
    BattleRuntime_WaitIfModeZero_65(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(1, 2);
    BattleRuntime_WaitIfModeZero_66(20);
    BattleEvent_RunActionAndWait_36(1, 0);
    BattleRuntime_WaitIfModeZero_67(30);
    BattleEvent_RunActionAndWait_37(8, 0);
    Event_Wait(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(2, 2);
    BattleRuntime_WaitIfModeZero_69(20);
    BattleEvent_RunActionAndWait_38(2, 0);
    Event_Wait(20);
    Actor_ShowEmote(8, 0x108, 50);
    BattleEvent_RunActionAndWait_39(8, 0);
    BattleRuntime_WaitIfModeZero_71(10);
    Actor_ShowEmote(1, 0x101, 40);
    BattleEvent_RunActionAndWait_40(1, 0);
    BattleRuntime_WaitIfModeZero_72(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(8, 2);
    BattleRuntime_WaitIfModeZero_73(20);
    Event_Wait(10);
    ObjectMotion_CallThenWaitForAnimationChange_12(3, 4);
    BattleRuntime_WaitIfModeZero_75(20);
    BattleEvent_RunActionAndWait_42(3, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 3, 40);
    ObjectMotion_CallThenWaitForAnimationChange_13(0, 3);
    BattleRuntime_WaitIfModeZero_77(30);
    Actor_FaceActor(1, 3, 0);
    ObjectMotion_SetAngleToward_10(2, 3, 0);
    BattleRuntime_WaitIfModeZero_78(20);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(2, 3);
    BattleRuntime_WaitIfModeZero_79(30);
    Audio_PlayCue_2(17);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Object_SetModeById_2(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    Actor_SetPosition(1, 0, 0);
    Object_SetModeById_3(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_3(2);
    Actor_SetPosition(2, 0, 0);
    Object_SetModeById_4(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_4(3);
    Actor_SetPosition(3, 0, 0);
    Audio_PlayCueForPartyMember_1();
    Event_End();
}

void FieldScene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Event_Begin();
    val = (s32)&Value_0000217f;
    Event_SetMessage(val);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_SetMessage(val + 1);
        Event_ShowMessage(8, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(val + 2);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}

void FieldScene_RunFlagGatedActorEightDialogue(void)
{
    if (GameFlag_IsSet(0x960) == 0)
        return;
    if (GameFlag_IsSet(0x962) != 0)
        return;

    GameFlag_Set(0x961);
    Event_Begin();
    Event_SetMessage(0x217d);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(30);
    Actor_FaceActor(0, 8, 0);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Event_End();
}

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}

void OverlayObject_UpdateThreeStateMotion(void *obj)
{
    s32 position[3];
    s32 x;
    s32 z;
    u8 *p;
    s32 state;

    p = (u8 *)obj + 0x40;
    state = *(s8 *)p;
    if (state == 0) {
        z = FIELD(obj, s32, 0x18);
        x = FIELD(obj, s32, 0x14);
        FIELD(obj, s32, 8) = z;
        position[2] = z;
        FIELD(obj, s32, 4) = x;
        position[0] = x;
        Func_02007684(0x780000, Random_Next(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Audio_PlayCue(0x86);
    } else if (state == 1) {
        if (Func_02007898(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (Func_020078b0(obj) == 0)
            Func_020078da(obj);
    }
}
