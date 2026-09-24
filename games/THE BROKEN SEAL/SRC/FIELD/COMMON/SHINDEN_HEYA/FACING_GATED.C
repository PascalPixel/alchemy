#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 *Data_03001ebc;

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

#define SCENE_REQUEST (*(u32 *)(Data_03001ebc + 0x1c0))
#define SCENE_SETUP_WORD (*(u32 *)(Data_03001ebc + 0x1c8))
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 0x1d8))

void Func_0200187c();
void Func_020025c4();
void Func_02002660();
void Func_02002750();
void Func_0200290c();
u8 *Func_0808a080();
void Func_0808a118();
void Func_080f9078();

/* The sibling actor-update script passes repeated large constants through
 * these inline call forms, keeping each call's argument evaluation local. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_scene_primary_script(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

static __inline__ void bump_step(void)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + 1);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#include "TYPES.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02003588
#define ResolveFacingObject Func_0200364e

#include "FACING_OBJECT.H"

s16 Func_02003588(s32, s32);
struct FacingObject *Func_0200364e(s16);

#include "TYPES.H"

/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

#include "TYPES.H"

/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */
extern u8 *Func_02005c76();

/* Publish the scene's upper prompt and lower dialogue panel. */

#include "TYPES.H"

/*
 * Ellipse orbit step for resource_378.  The object is offset from its anchor
 * along two axes and its angle advanced once per call.
 */

/*
 * The anchor at +104 supplies the centre; the result is published to the
 * object's +8/+16 and to its +56/+64 shadow pair, with +56 taken from a fresh
 * read of +8 rather than from x.  The two imports take the same angle and form
 * a cosine/sine pair; which is which is not settled.  The radii 14 and 10 and
 * the +100/+102 displacements are built from immediates.  The angle at +100 and
 * its step at +102 are separate halfwords, not one 32-bit field.
 */

#include "TYPES.H"

#define GetActorState Func_020068be

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

ActorState *Func_020068be(s32 actor_id);

#include "TYPES.H"

/* Close through scene 8 when facing inward; otherwise select the story line. */
extern s32 Func_0200657c(void);

#include "TYPES.H"

/* Close scene 8 when facing inward; otherwise choose its story line. */
extern s32 Func_02006708(void);
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
extern s32 Func_02006754(void);
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */
extern s32 Func_0200679c(void);

#include "TYPES.H"

#define BattleRuntime_WaitIfModeZero_1(a0, a1) Call2_scene_primary_script(Engine_ActorSetAttachedEffect, a0, a1)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02005050(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(a0, a1) Call2_scene_primary_script(Engine_ColorBufferApplyTarget, a0, a1)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2_scene_primary_script(Engine_CameraSetSpeed, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectGroup_ConfigureChildValue_1(a0, a1) Call2_scene_primary_script(Engine_ActorSetChildValue, a0, a1)
#define Scene_GetRecord_1(args...) Func_020051f0(args)
#define Scene_GetRecord_2(args...) Func_020051fc(args)
#define BattleRuntime_WaitIfModeZero_30(a0, a1) Call2_scene_primary_script(Engine_ColorBufferApplyTarget, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_22(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_23(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_24(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_25(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_26(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3_scene_primary_script(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_27(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_28(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_29(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_30(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_31(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_32(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_33(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_34(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_35(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3_scene_primary_script(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3_scene_primary_script(Engine_ActorWalkToAndWait, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_36(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_37(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_38(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_39(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_40(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_41(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_42(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_43(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define Scene_GetRecord_1_0200290c(args...) Func_02005f7e(args)
#define Scene_GetRecord_2_0200290c(args...) Func_02005f9e(args)
#define Scene_GetRecord_3(args...) Func_02005fbc_a(args)
#define Scene_GetRecord_4(args...) Func_02005fda(args)
#define Scene_GetRecord_5(args...) Func_02005fe8_b(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define ObjectMotion_ArmCallback_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_8_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_9_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_10_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define Scene_GetRecord_6(args...) Func_02006284(args)
#define Scene_GetRecord_7(args...) Func_02006290(args)
#define Scene_GetRecord_8(args...) Func_0200629a(args)
#define Scene_GetRecord_9(args...) Func_020062a4(args)
#define ObjectMotion_SetPositionAndReset_1_0200290c(a0, a1, a2) Value3(Engine_ActorWalkToAndWait, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define Scene_GetRecord_10(args...) Func_0200632a(args)
#define Scene_GetRecord_11(args...) Func_02006334(args)
#define ObjectMotion_SetVariantCallback_1_0200290c(a0, a1) Value2(Engine_ActorStartRepeatedMotion, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_46_0200290c(args...) ((void (*)())Engine_EventWait)(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Call2_scene_primary_script(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LookupAndStep_1(args...) Func_020064f4(args)
#define Object_LookupAndStep_2(args...) Func_020064fa(args)
#define ObjectMotion_SetSpeedParameters_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3_scene_primary_script(Engine_ActorFaceDirection, a0, a1, a2)
#define Engine_ActorFaceDirection(args...) Actor_FaceDirection(args)
#define ObjectMotion_SetVariantCallback_5_0200290c(a0, a1) Call2_scene_primary_script((void (*)())Func_02005fe8, a0, a1)
#define ObjectMotion_SetPositionAndReset_10(a0, a1, a2) Call3_scene_primary_script((void (*)())Func_02005fe8_a, a0, a1, a2)

extern s16 Data_02000240[];

void Func_02001d28();
struct FacingObject *Func_020039e2();
struct FacingObject *Func_020039f2_b();
struct FacingObject *Func_02003a20();
struct FacingObject *Func_02003a40();
void Func_02003af4();
void Func_02003c02();
void Func_020040c0();
void Func_020042a8();
void Func_02004d8c();
struct FacingObject *Func_02005f7e();
u8 *Func_020051f0();
u8 *Func_020051fc();
s32 Func_02005bc6();
struct FacingObject *Func_02005fbc_a();
struct FacingObject *Func_02005fda();
struct FacingObject *Func_02006284();
u8 *Func_02006290();
struct FacingObject *Func_0200629a();
u8 *Func_020062a4();
u8 *Func_0200632a();
void Func_020064f4();
void Func_020064fa();
u8 *Func_02005fe8();
u8 *Func_02005fe8_a();
struct FacingObject *Func_02005fe8_b();
u8 *Func_02005fe8_c();
u8 *Func_0200632a_a();
u8 *Func_02006334();
s32 Func_020065e0();
void Func_02003790();

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, while a direct call precomputes a costly
 * constant into a local that later uses in the block share. A call that
 * returns a value sets r0 last of its arguments; the Value wrappers spell
 * those sites, and the result is sometimes unused.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

static __inline__ void Call2_scene_primary_script(void (*f)(), s32 a0, s32 a1)
{
    void Func_02005050();
    u8 *Func_02005f9e();

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Func_02005050();
    u8 *Func_02005f9e();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    u8 *Func_02005050();
    s32 Func_02005f9e();

    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    u8 *Func_02005050();
    s32 Func_02005f9e();

    return f(a0, a1, a2, a3);
}

/*
 * Runs actor nine's flag-branched dialogue. The 112-byte owner includes its
 * five pool words. The scene selector is the signed halfword at
 * Data_02000240 + 450, reached as index 225. The last two calls must stay
 * after the selector test: the epilogue pops the return address into r0, so
 * the second call's result is discarded there.
 */

/*
 * The sibling path to the dialogue above, reading the same selector
 * halfword. The 116-byte owner includes its five pool words.
 */

/*
 * Steps actor ten and, when the check passes, increments the same workspace
 * +472 halfword the preceding owner writes. The 88-byte owner includes its
 * two pool words.
 */

/*
 * Steps a fixed sequence of actor position, pose and timing calls over
 * slots 0, 1, 8, 9, 10, 11, 12 and 13, including one loop that nudges a
 * pair of per-actor record fields down 32 times.
 */

/*
 * Dispatches on the scene selector Data_02000240[225] over the range 10 to
 * 50, through a 41-entry jump table. The epilogue pops the return address
 * into r0, so no result survives it and the owner is void; the 296-byte
 * owner covers dispatcher, table, case bodies and literal pool. The default
 * arm doubles as the shared tail, so the arms that fall into it break while
 * the 20/21/50 arm returns instead.
 */

#include "TYPES.H"

extern u8 Data_0200baa8;
extern u8 Data_0200bbc8;
extern u8 Data_0200bbf4[];   /* Empty table: place nothing. */
extern u8 Data_0200bc0c[];
extern u8 Data_0200bccc[];
extern u8 Data_0200bd2c[];
extern u8 Data_0200be04[];
extern u8 Data_0200be34[];
extern u8 Data_0200be70[];
extern u8 Data_0200bec4[];
extern u8 Data_0200bf0c[];
extern u8 Data_0200bf78[];
extern u8 Data_0200bfd8[];
extern u8 Data_0200c038[];
extern u8 Data_0200c080[];
extern u8 Data_0200c0ec[];

/* One symbol per call site, named at the site's decoded address. */

/*
 * Select this scene's placement script from its stored sub-state.  The
 * 220-byte owner at 0x02000070 includes the 35-entry jump table at
 * 0x0200008c-0x02000117 and the literal pool at 0x02000130-0x0200014b.  The
 * selector is the signed halfword at offset 450 of the scene-record block, and
 * its address is built rather than folded: the `s32 off = 450;` local in its
 * own block is what forces that.  The out-of-range arm is also the arm for
 * most of the in-range entries, so it is a genuine default.
 */

/*
 * In-image script tables: runs of 24-byte records terminated by 0x0000ffff, in
 * the shape {0xffff0000 | selector, count, x, 0, z, value} with coordinates in
 * 16.16 fixed point.  The layout is read off the data, so the fields at +12
 * and +20 are named by position only, and the selector's return type stays an
 * opaque pointer.  The tables live in EWRAM, which is writable and used as
 * save state, so they are deliberately not const.
 */

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

/*
 * Select a table from the scene id and two flags.  The 304-byte owner at
 * 0x0200014c decomposes as dispatcher, the 41-entry jump table at
 * 0x02000168-0x0200020b, the case bodies, an alignment halfword at 0x02000246
 * and the literal pool.  Case-arm order follows the table -- entries grouped
 * by value, distinct values ascending -- so the 20/21/50 arm comes third and
 * the 29 arm after the 32 arm, not in selector order.
 */

s32 UpdateFacingFromResolvedObject(struct FacingObject *object)
{
    struct FacingObject *target;

    target = ResolveFacingObject(object->unknown_64);
    object->facing = CalculateFacingAngle(
        target->position_z - object->position_z,
        target->position_x - object->position_x
    );
    return 0;
}

void *SceneData_GetTableBaa8(void)
{
    return &Data_0200baa8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableBbc8(void)
{
    return &Data_0200bbc8;
}

void *SceneData_SelectPlacementTableBySubstate(void)
{

    s16 sub;

    {
        s32 off = 450;

        sub = *(s16 *)((u8 *)Data_02000240 + off);
    }
    switch ((s32)sub) {
    case 1:
    case 2:
        return Data_0200bc0c;

    case 10:
    case 11:
    case 12:
    case 35:
        return Data_0200bccc;

    case 20:
    case 21:
        return Data_0200bd2c;

    case 29:
    case 32:
        return Data_0200be04;

    default:
        return Data_0200bbf4;
    }
}

u8 *SceneData_SelectTableBySceneIdAndFlags(void)
{
    extern s16 Data_02000240[];

    s16 *tbl = Data_02000240;
    s32 no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        return Data_0200be70;
    case 11:
        return Data_0200bec4;
    case 20:
    case 21:
    case 50:
        return Data_0200bf0c;
    case 32:
        return Data_0200c0ec;
    case 29:
        return Data_0200c038;
    case 35:
        return Data_0200c080;
    default:
        break;
    }

    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200bfd8;
    }
    if (GameFlag_IsSet(0x815) != 0) {
        return Data_0200bf78;
    }
    return Data_0200be34;
}

void FieldScene_RunActorNineFlagDialogueA(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x855) != 0) {
        Event_SetMessage(0x1377);
    } else {
        Event_SetMessage(0x1289);
    }

    if (gGameState.entrance == 11) {
        Event_SetMessage(0x1ce9);
    }

    Actor_SetAnimation(9, 1);
    Actor_FaceEachOther(9, 0, 0);
    Event_Wait(2);
    Event_ShowMessage(9, 0);
    Event_End();
}

void FieldScene_RunActorNineFlagDialogueB(void)
{
    void Actor_Stop(s32 id);
    void Func_020039f2(s32 id, s32 arg1);

    Event_Begin();

    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(0x128b);
    } else {
        Event_SetMessage(0x1379);
    }

    if (gGameState.entrance == 11) {
        Event_SetMessage(0x1ceb);
    }

    Actor_Stop(9);
    Actor_SetAnimation(9, 1);
    Event_Wait(2);
    Func_020039f2(9, 0);
    Actor_EnableActionCallback(9, 2);
    Event_End();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    Event_Begin();
    Event_SetMessage(0x1164);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(8, 3);
        Event_Wait(20);
    } else {
        Event_Wait(20);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(20);
            Event_OpenMessage(8, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_Wait(20);
                if (Func_020039e2(8)->facing >= 0xa000 && Func_020039f2_b(8)->facing <= 0xe000) {
                    Call3_scene_primary_script(Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
                    Actor_FaceDirection(8, 0, 0);
                    Event_Wait(10);
                    Func_02003a20(8)->facing_flags &= ~1;
                    Actor_WalkToAndWait(8, 152, 120);
                    Event_Wait(1);
                    Func_02003a40(8)->facing_flags |= 1;
                    Event_Wait(20);
                    Actor_SetAnimationAndWait(8, 3);
                    Event_Wait(20);
                    Actor_WalkToAndWait(0, 168, 120);
                    Actor_WalkTo(0, 192, 168);
                    Event_Wait(20);
                    Actor_WalkToAndWait(8, 168, 120);
                    Call3_scene_primary_script(Engine_ActorFaceDirection, 8, 0x3000, 0);
                    Actor_WaitForMove(0);
                } else {
                    Actor_WalkTo(0, 192, 168);
                    Event_Wait(20);
                    Call3_scene_primary_script(Engine_ActorFaceDirection, 8, 0x3000, 0);
                    Actor_WaitForMove(0);
                }
                Func_02001d28();
                ColorBuffer_ApplyTarget(0, 0);
                ColorBuffer_Interpolate(120);
                Event_Wait(120);
                Audio_PlayCue(86);
                Func_02003c02();
                GameFlag_Set(0x9f0);
                Event_RequestExit(30);
            }
        }
    }
    Event_End();
}

void FieldScene_RunScene378SequenceB(void)
{
    void Actor_Stop();
    u8 *Func_020039f2();

    u32 i;
    s32 record;

    Event_Begin();
    Call1(Func_02003af4, 0x200bc9c);
    Call1((void (*)())Engine_TaskWait, 1);
    Event_SetMessage(0x1bfd);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(9, 0);
    } else {
        bump_step();
        Call11(Engine_EventShowTwoMessagesAndWait, 2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Event_ShowMessage(9, 0);
    }
    Event_End();
}

void FieldScene_RunActorTenCountStep(void)
{

    Event_Begin();
    Actor_FaceActor(10, 0, 0);
    Event_Wait(10);
    Event_SetMessage(0x119f);
    Event_OpenMessage(10, 0);

    if (Event_ChooseYesNo(0, 0) == 1) {
        (gEventWork->message)++;
    }

    Event_ShowMessage(10, 0);
    Event_End();
}

void FieldScene_RunActorEightResetSequence(void)
{
    Event_Begin();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveToActor(1, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_SetMessage(0x116c);
    Event_ShowMessage(8, 0);
    GameFlag_Set(0x200);
    Event_End();
}

void FieldScene_RunScriptedSceneSequence(void)
{
    u32 i;

    Event_Begin();
    if (GameFlag_IsSet(0x201) != 0) {
        Actor_FaceEachOther(0, 8, 0);
        Actor_FaceActor(1, 0, 0);
        Actor_FaceActor(0xb, 0, 0);
        Actor_FaceActor(0xc, 0, 0);
        Actor_FaceActor(9, 0, 0);
        Actor_FaceActor(0xa, 0, 0);
        Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
        Camera_WaitForMove();
        SCENE_REQUEST = 0x100;
        SCENE_SETUP_WORD = 0x40;
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(0x78);
        goto dialogue;
    }

    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(1);
    Event_Wait(1);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    SCENE_REQUEST = 0x209;
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_0200290c();
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(0x3c);
    Event_Wait(0x64);
    Actor_SetAnimation(0, 1);
    Actor_SetAnimation(1, 1);
    Event_Wait(0x1e);
    Actor_FaceActor(0, 0xc, 0);
    Actor_FaceActor(1, 0xc, 0);
    Event_Wait(0x14);
    Actor_EnableActionCallback(8, 1);
    Actor_EnableActionCallback(0xc, 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(0xb, 0xcccc, 0x6666);
    Actor_SetSpeed(0xc, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(0xa, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(0xc, 2);
    Event_SetMessage(0x1138);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0xa);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(0x1e);
    Actor_SetAnimationAndWait(0xc, 3);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Actor_FaceActor(0, 0xb, 0);
    Actor_FaceActor(1, 0xb, 0);
    Event_Wait(0x14);
    Event_OpenMessage(0xb, 0);
    /* Each arm advances once, on its own side of the object-state call. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(0xb, 3);
        Event_Wait(0x14);
        Event_ShowMessage(0xb, 0);
        SKIP_BEATS++;
    } else {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(0xb, 4);
        Event_Wait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(0xb, 0);
    }
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(0x14);
    Actor_FaceActor(0, 9, 0);
    Actor_FaceActor(1, 9, 0);
    Actor_FaceActor(0xb, 9, 0);
    Event_Wait(0x14);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(0x14);
        Event_ShowMessage(9, 0);
        SKIP_BEATS++;
    } else {
        Event_Wait(0x14);
        Actor_SetAnimationAndWait(9, 4);
        Event_Wait(0x14);
        SKIP_BEATS++;
        Event_ShowMessage(9, 0);
    }
    Event_Wait(0x14);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(0xc, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(0xa, 3);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x101, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Actor_WalkToAndWait(0xc, 0xe0, 0x78);
    Event_Wait(0xa);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0xa);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0xa);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(8, 3);
    Actor_ShowEmote(8, 0x100, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(8, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(0xc, 3);
    Event_Wait(0xa);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0xa);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Actor_FaceDirection(8, 0x3000, 0);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x32);
    Actor_ShowEmote(0xc, 0x101, 0);
    Event_Wait(0x28);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(0xb, 0x100, 0);
    Actor_ShowEmote(0xc, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_ShowEmote(0xa, 0x100, 0);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(0xa);
    Event_ShowMessage(1, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(0xb, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(0xa, 1);
    Event_Wait(0xa);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_FaceActor(0xc, 0, 0);
    Actor_FaceActor(0xb, 0, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(0xc, 2);
    Event_Wait(0x14);
    Actor_FaceActor(0xc, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x1e);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Event_Wait(0x28);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    Func_020025c4();
    Audio_PlayCue(0xc4);
    Func_02002750(8, 0x1200);
    Event_Wait(0x20);
    Func_02002750(8, 0x1200);
    Event_Wait(0x10);
    for (i = 0; i < 6; i++) {
        Func_02002750(8, 0x1200);
        Event_Wait(8);
    }
    Event_Wait(8);
    Func_02002750(8, 0x1200);
    Event_Wait(0x20);
    Func_02002750(8, 0x1200);
    Event_Wait(0x60);
    Event_Wait(0x20);
    Actor_SetChildValue(8, 0);
    Event_Wait(0x1e);
    Func_02002660();
    Actor_SetAnimation(8, 1);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(0xa, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xa, 0);
    Event_Wait(0x14);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_FaceEachOther(9, 0xa, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(0xa, 3);
    Func_0808a118(0xa);
    Event_Wait(0x1e);
    Actor_FaceEachOther(0, 0xb, 0);
    Actor_FaceEachOther(1, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(0xc, 3);
    Func_0808a118(0xc);
    Event_Wait(0x3c);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    Func_020025c4();
    Audio_PlayCue(0xc4);
    Func_02002750(8, 0x1200);
    Event_Wait(0x20);
    Func_02002750(8, 0x1200);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x10);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(0xb, 1);
    Actor_StartRepeatedMotion(0xc, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(0xa, 1);
    for (i = 0; i < 6; i++) {
        Func_02002750(8, 0x1200);
        Event_Wait(8);
    }
    Event_Wait(8);
    Func_02002750(8, 0x1200);
    Event_Wait(0x20);
    Func_02002750(8, 0x1200);
    Event_Wait(0x80);
    Actor_SetChildValue(8, 0);
    Event_Wait(0x1e);
    Func_02002660();
    Actor_SetAnimation(8, 1);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x105, 0);
    Event_Wait(0x14);
    Actor_FaceEachOther(9, 0xa, 0);
    Event_Wait(0x28);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 2);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_FaceEachOther(0, 1, 0);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(0xa, 1);
    Actor_FaceEachOther(9, 0xa, 0);
    Actor_SetAttachedEffect(0xc, 0x102);
    Event_Wait(0x3c);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x14);
    Event_ShowMessage(1, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(0xb, 3);
    Actor_SetAnimation(0xc, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(0xa, 3);
    Func_0808a118(0xa);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xc, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_FaceActor(8, 0xc, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x32);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(0xb, 0x102, 0);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(0xa, 0x102, 0);
    Event_Wait(0x50);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Event_Wait(0x50);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(0xc, 4);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x28);
    Actor_FaceEachOther(0, 1, 0);
    Actor_FaceEachOther(9, 0xa, 0);
    Actor_FaceActor(0xc, 0, 0);
    Event_Wait(0x3c);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_Wait(0x14);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x3c);
    Actor_WalkToAndWait(8, 0xc8, 0x88);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(0xc, 0, 0);
    Actor_FaceEachOther(8, 1, 0);
    Event_Wait(0x28);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x32);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(0x28);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x32);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Event_Wait(0x3c);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_RunRepeatedMotion(0xb, 1);
    Event_ShowMessage(0xb, 0);
    Event_Wait(0x1e);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0x1e);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_StartRepeatedMotion(0xc, 1);
    Event_Wait(0x14);
    Event_ShowMessage(0xc, 0);
    Event_Wait(0x1e);
    Actor_WalkToAndWait(8, 0xa8, 0x78);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceDirection(8, 0xd000, 0);
    Event_Wait(0x14);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x1e);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Event_Wait(0x3c);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(0xa);
    Event_ShowMessage(8, 0);
    Event_Wait(0xa);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(0x14);
    Event_ShowMessage(8, 0);
    Event_Wait(0x14);
    Actor_FaceActor(0xc, 0, 0);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_RunRepeatedMotion(1, 2);
    Actor_FaceActor(1, 0, 0);
    Event_Wait(0x14);
    Event_ShowMessage(1, 0);
    Event_Wait(0x1e);
dialogue:
    Event_SetMessage(0x1162);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(0x14);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Func_0200187c();
        ColorBuffer_ApplyTarget(0, 0);
        ColorBuffer_Interpolate(0x78);
        Event_Wait(0x78);
        Audio_PlayCue(0x56);
        Func_080f9078();
        GameFlag_Set(0x9f0);
        Event_RequestExit(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Event_Wait(0x1e);
        Actor_RunRepeatedMotion(0xc, 1);
        Actor_SetAnimationAndWait(0xc, 4);
        Event_Wait(0x14);
        Event_ShowMessage(0xc, 0);
        Event_Wait(0x14);
        actor = Func_0808a080(0xa);
        actor += 0x23;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = Func_0808a080(0xa);
        *(u32 *)(actor + 0x6c) = 0x02008849;
    }
    Event_End();
}

void FieldScene_RunActorUpdateSequence(void)
{
    void Func_02005050();
    u8 *Func_02005f9e();

    u32 i;
    u8 *record;

    BattleRuntime_WaitIfModeZero_1(1, 0x102);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(11, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_4(12, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_6(10, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_7(8, 0xcccc, 0x6666);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    Actor_FaceActor(1, 0, 0);
    Actor_FaceActor(12, 0, 0);
    Actor_FaceActor(9, 0, 0);
    Actor_FaceActor(10, 0, 0);
    Actor_FaceEachOther(0, 11, 0);
    Event_Wait(10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(60);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(50);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_SetMessage(0x1171);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_FaceActor(12, 1, 0);
    ObjectMotion_SetVariantCallback_1(12, 2);
    BattleEffect_SpawnLinkedResourceObject_1(12, 0x103, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(40);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    ObjectMotion_ArmCallback_1(8, 0xd000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(80);
    Audio_PlayCue(17);
    BattleRuntime_WaitIfModeZero_19(0x10005, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x6666, 0xccc);
    Camera_MoveTo(0xc00000, -1, 0x680000, 1);
    Event_Wait(120);
    Audio_PlayCue(21);
    Audio_PlayCue(0x134);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(13, 0xc80000, 0x80000);
    ObjectMotion_SetSpeedParameters_8(13, 0x6666, 0x3333);
    Actor_MoveToAndWait(13, 200, 72);
    Audio_PlayCue(0x120);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Actor_SetAnimation(8, 0);
    Func_020040c0();
    Actor_FaceActor(0, 13, 0);
    Actor_FaceActor(1, 13, 0);
    Actor_FaceActor(11, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Actor_FaceActor(9, 13, 0);
    Actor_FaceActor(10, 13, 0);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(10, 2);
    Event_Wait(40);
    Event_ShowMessage(13, 0);
    Event_Wait(20);
    Event_Wait(40);
    Event_ShowMessage(13, 0);
    Event_Wait(60);
    ObjectGroup_ConfigureChildValue_1(13, 0x100);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    /*
     * 32 repeats: step actor 13's animation, then subtract a fixed amount
     * from the record fields at +24 and +28.
     */
    for (i = 0; i < 32; i++) {
        Func_02004d8c(13);
        Event_Wait(4);
        record = Scene_GetRecord_1(13);
        *(s32 *)(record + 24) += -0x28f;
        record = Scene_GetRecord_2(13);
        *(s32 *)(record + 28) += -0x28f;
    }
    Audio_PlayCue(0x120);
    Actor_SetChildValue(13, 0);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(40);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Camera_WaitForMove();
    BattleRuntime_WaitIfModeZero_30(0x10000, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(120);
    Func_020042a8();
    Actor_SetAnimation(8, 1);
    Audio_PlayCue(2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(30);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(30);
    Actor_FaceActor(12, 8, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_FaceActor(8, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_4(11, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_5(12, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_6(9, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_7(10, 0x102, 0);
    Event_Wait(30);
    BattleEffect_SpawnLinkedResourceObject_8(8, 0x102, 0);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(180);
    BattleEffect_SpawnLinkedResourceObject_9(12, 0x105, 0);
    Event_Wait(60);
    Actor_FaceEachOther(0, 1, 0);
    Actor_FaceEachOther(9, 10, 0);
    Event_Wait(40);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(30);
    BattleEffect_SpawnLinkedResourceObject_10(8, 0x101, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_11(8, 0x101, 0);
    Event_Wait(80);
    BattleEffect_SpawnLinkedResourceObject_12(12, 0x102, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(30);
    BattleEffect_SpawnLinkedResourceObject_13(8, 0x105, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(8, 1);
    BattleEffect_SpawnLinkedResourceObject_14(8, 0x106, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(0, 1);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(9, 1);
    Actor_RunRepeatedMotion(10, 1);
    Event_Wait(40);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(30);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(120);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_15(0, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_16(1, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_17(11, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_18(12, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_19(9, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_20(10, 0x105, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(12, 1);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_FaceActor(8, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_21(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_22(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_23(11, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_24(12, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_25(9, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_26(10, 0x100, 0);
    Event_Wait(60);
    Actor_FaceActor(8, 0, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(8, 0);
    Event_Wait(40);
    Actor_WalkToAndWait(8, 168, 176);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Actor_WalkToAndWait(8, 200, 200);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceDirection(11, 0, 0);
    ObjectMotion_ArmCallback_3(9, 0x8000, 0);
    ObjectMotion_ArmCallback_4(10, 0x8000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_1(8, 200, 0x110);
    Event_Wait(40);
    ObjectMotion_ArmCallback_5(11, 0x3000, 0);
    ObjectMotion_ArmCallback_6(9, 0x5000, 0);
    ObjectMotion_ArmCallback_7(10, 0x5000, 0);
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Event_Wait(60);
    BattleEffect_SpawnLinkedResourceObject_27(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_28(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_29(11, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_30(12, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_31(9, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_32(10, 0x102, 0);
    Event_Wait(60);
    Event_Wait(120);
    Actor_FaceActor(0, 9, 0);
    Actor_FaceActor(1, 10, 0);
    Actor_FaceActor(11, 9, 0);
    Actor_FaceActor(12, 9, 0);
    Event_Wait(120);
    BattleEffect_SpawnLinkedResourceObject_33(9, 0x105, 0);
    Event_Wait(60);
    Actor_FaceActor(9, 0, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(10);
    ObjectMotion_ArmCallback_8(9, 0x5000, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_34(9, 0x102, 0);
    Event_Wait(80);
    Actor_FaceActor(9, 10, 0);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    ObjectMotion_ArmCallback_9(10, 0x5000, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_35(10, 0x101, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_FaceActor(10, 0, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(30);
    ObjectMotion_ArmCallback_10(10, 0x5000, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(20);
    ObjectMotion_ResetAndSetPositionInMode2_2(9, 200, 0x110);
    ObjectMotion_SetPositionAndReset_3(10, 200, 0x110);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    BattleEffect_SpawnLinkedResourceObject_36(12, 0x105, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(12, 200, 136);
    Actor_FaceEachOther(0, 12, 0);
    Actor_FaceActor(1, 12, 0);
    Actor_FaceActor(11, 12, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(12, 4);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(30);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_WalkToAndWait(11, 168, 168);
    Actor_FaceActor(11, 12, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 11, 0);
    Actor_FaceActor(1, 11, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(11, 4);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_37(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_38(1, 0x102, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(12, 4);
    Event_Wait(20);
    Actor_FaceActor(0, 12, 0);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    BattleEffect_SpawnLinkedResourceObject_39(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_40(1, 0x102, 0);
    Event_Wait(60);
    BattleEffect_SpawnLinkedResourceObject_41(12, 0x102, 0);
    Event_Wait(60);
    Event_ShowMessage(12, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    BattleEffect_SpawnLinkedResourceObject_42(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_43(1, 0x100, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 11, 0);
    Event_Wait(30);
    Actor_SetAnimation(11, 3);
    Event_Wait(30);
    Actor_SetAnimation(12, 3);
}

void SceneState_ResetObject14Word108(void)
{
    u8 *state = Func_02005c76(14);
    *(s32 *)(state + 108) = 0;
    Actor_SetPosition(14, 0, 0);
}

void SceneEffect_StepEllipseOrbit(u8 *obj)
{
    u8 *anchor = *(u8 **)(obj + 104);
    u16 *angle = (u16 *)(obj + 100);
    u16 theta = *angle;
    s32 x;
    s32 z;
    s32 tmp;

    x = *(s32 *)(anchor + 8) + Math_Cos(theta)* 14;
    *(s32 *)(obj + 8) = x;

    z = *(s32 *)(anchor + 16) + Math_Sin(theta)* 10;
    tmp = *(s32 *)(obj + 8);
    *(s32 *)(obj + 16) = z;
    *(s32 *)(obj + 64) = z;
    *(s32 *)(obj + 56) = tmp;

    *angle = (u16)(*angle + *(u16 *)(obj + 102));
}

void FieldScene_RunPairedActorChoreography(void)
{
    u8 *Func_02005050();
    struct FacingObject *Func_02005f9e();

    ObjectMotion_SetSpeedParameters_1_0200290c(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_2_0200290c(1, 0x18000, 0xc000);
    Call2_scene_primary_script((void (*)())Engine_ActorRunRepeatedMotion, 12, 2); /* main:0808a138 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3); /* main:0808a110 */
    Event_Wait(15);
    Actor_FaceActor(0, 1, 0);
    ObjectMotion_SetVariantCallback_5_0200290c(0, 1); /* object 0, variant 1 */
    Scene_GetRecord_1_0200290c(0)->facing_flags &= ~1;
    Actor_WalkTo(0, 184, 168);
    Scene_GetRecord_2_0200290c(1)->facing_flags &= ~1;
    ObjectMotion_SetPositionAndReset_10(1, 200, 168);
    Event_Wait(1);
    Scene_GetRecord_3(1)->facing_flags |= 1;
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    Scene_GetRecord_4(0)->facing_flags |= 1;
    Scene_GetRecord_5(1)->facing_flags |= 1;
    Actor_Jump(1, 2, 0);
    Event_Wait(15);
    Actor_FaceActor(1, 8, 0);
    Call1((void (*)())Func_02005fe8_c, 5); /* main:0808a080 */
    Actor_Jump(1, 2, 0);
    Event_Wait(25);
    Actor_RunRepeatedMotion(1, 2);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(5);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3); /* main:0808a110 */
    Event_Wait(5);
    Actor_SetAnimationAndWait(0, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceActor(1, 0, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(15);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimationAndWait(10, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceActor(0, 12, 0);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(10); /* main:0808a138 */
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceActor(0, 11, 0);
    Actor_FaceActor(1, 11, 0);
    Event_Wait(10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_FaceDirection(0, 0, 0);
    Event_Wait(15);
    Actor_RunRepeatedMotion(0, 2); /* main:0808a138 */
    Event_Wait(10);
    Value4(Func_02005bc6, 222, 0xb80000, 0x1b0000, 0xa80000);
    Actor_FaceActor(1, 0, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 1);
    Event_Wait(10);
    Actor_Jump(1, 4, 0); /* main:0808a138 */
    ObjectMotion_ArmCallback_2_0200290c(1, 0xd000, 0);
    Event_Wait(15);
    ObjectMotion_ArmCallback_3_0200290c(1, 0xb000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_4_0200290c(1, 0xd000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_5_0200290c(1, 0xb000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_6_0200290c(1, 0xd000, 0);
    Event_Wait(30);
    Actor_Jump(1, 4, 0);
    ObjectMotion_ArmCallback_7_0200290c(1, 0x3000, 0);
    Event_Wait(15);
    ObjectMotion_ArmCallback_8_0200290c(1, 0x5000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_9_0200290c(1, 0x3000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_10_0200290c(1, 0x5000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_11(1, 0x3000, 0);
    Event_Wait(30);
    ObjectMotion_ArmCallback_12(0, 0xc000, 0);
    ObjectMotion_ArmCallback_13(1, 0xc000, 0);
    Event_Wait(10);
    BattleEffect_SpawnLinkedResourceObject_1_0200290c(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_2_0200290c(1, 0x102, 0);
    Event_Wait(60);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(10);
    Scene_GetRecord_6(8)->unknown_64 = 1;
    *(s32 *)(Scene_GetRecord_7(8) + 108) = (s32)UpdateFacingFromResolvedObject;
    Scene_GetRecord_8(12)->unknown_64 = 1;
    *(s32 *)(Scene_GetRecord_9(12) + 108) = (s32)UpdateFacingFromResolvedObject;
    ObjectMotion_SetPositionAndReset_1_0200290c(1, 196, 180);
    Actor_WalkToAndWait(1, 184, 184);
    Actor_WalkToAndWait(1, 180, 180);
    Actor_WalkToAndWait(1, 168, 168);
    Actor_WalkToAndWait(1, 180, 156);
    Actor_WalkTo(1, 200, 104);
    Actor_WalkToAndWait(0, 192, 168);
    ObjectMotion_ArmCallback_14(0, 0xc000, 0);
    Actor_WaitForMove(1);
    Event_Wait(30);
    Actor_RunRepeatedMotion(1, 1); /* main:0808a138 */
    Event_Wait(10);
    ObjectMotion_ArmCallback_15(1, 0x5000, 0);
    Event_Wait(15);
    *(s32 *)(Scene_GetRecord_10(12) + 108) = 0;
    *(s32 *)(Scene_GetRecord_11(8) + 108) = 0;
    ObjectMotion_SetVariantCallback_1_0200290c(8, 2);
    BattleEffect_SpawnLinkedResourceObject_3_0200290c(8, 0x100, 0);
    Call1((void (*)())Func_0200632a_a, 60); /* main:0808a080 */
    Actor_SetAnimation(8, 0);
    BattleEffect_SpawnLinkedResourceObject_4_0200290c(0, 0x102, 0);
    Event_Wait(60);
    Actor_FaceEachOther(0, 11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(11, 3); /* main:0808a110 */
    Event_Wait(10);
    ObjectMotion_ArmCallback_16(0, 0xc000, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(10);
    Actor_Jump(0, 2, 0); /* main:0808a138 */
    Event_Wait(20);
    Actor_Jump(0, 2, 0);
    BattleRuntime_WaitIfModeZero_46_0200290c(20);
    Event_Wait(15);
    Actor_FaceEachOther(0, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(60);
    Actor_WalkToAndWait(1, 208, 168);
    Actor_FaceActor(0, 11, 0);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(10);
    Actor_SetAnimation(0, 4);
    Actor_SetAnimationAndWait(1, 4); /* main:0808a110 */
    Event_Wait(10);
    Actor_StartRepeatedMotion(0, 1);
    Actor_RunRepeatedMotion(1, 1); /* main:0808a138 */
    Event_Wait(10);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(10);
    ObjectMotion_EnableActionAndSetCallback_1(0, 0x200b740);
    ObjectMotion_EnableActionAndSetCallback_2(1, 0x200b81c);
    Object_LookupAndStep_1(0); /* main:0808a0a0 */
    Object_LookupAndStep_2(1); /* main:0808a0a0 */
    ObjectMotion_SetSpeedParameters_3_0200290c(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_4_0200290c(1, 0x18000, 0xc000);
    Actor_Jump(0, 6, 0);
    Actor_Jump(1, 6, 0);
    Actor_FaceActor(0, 9, 0);
    Actor_FaceActor(1, 8, 0);
    Event_Wait(1);
    Actor_FaceActor(0, 12, 0);
    Actor_FaceActor(1, 11, 0);
    Event_Wait(1);
    Actor_FaceActor(0, 8, 0);
    Actor_FaceActor(1, 9, 0);
    Event_Wait(1); /* main:0808a138 */
    Actor_WalkTo(0, 192, 168);
    Actor_WalkToAndWait(1, 208, 168);
    Actor_WaitForMove(0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_20(0, 0x3000, 0);
    ObjectMotion_ArmCallback_21(1, 0xd000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_22(0, 0x5000, 0);
    ObjectMotion_ArmCallback_23(1, 0xb000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_24(0, 0x3000, 0);
    ObjectMotion_ArmCallback_25(1, 0xd000, 0);
    Event_Wait(10);
    ObjectMotion_ArmCallback_26(0, 0x5000, 0);
    ObjectMotion_ArmCallback_27(1, 0xb000, 0);
    Event_Wait(20);
    Actor_FaceActor(0, 11, 0);
    Actor_FaceActor(1, 12, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimationAndWait(12, 3); /* main:0808a110 */
    Event_Wait(30);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3); /* main:0808a110 */
    Event_Wait(10);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(10);
    Actor_SetAnimation(0, 2);
    Actor_SetAnimation(1, 2);
    Event_Wait(60);
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Map_CopyCellsTo(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Map_CopyCellAttributes(11, 10, 3, 2, a5, a6);
    }
    Task_Wait(1);
}

s32 IsActorFacingInward(void)
{
    ActorState *actor = GetActorState(0);

    if ((u32)((actor->angle + 0x5fff) << 16) <= 0x3ffe0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorEightFacingDialogue(void)
{
    if (Func_0200657c() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0)
        Event_SetMessage(0x1bfc);
    else if (GameFlag_IsSet(0x815) != 0)
        Event_SetMessage(0x119d);
    else
        Event_SetMessage(0x1035);
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_DispatchBySceneId(void)
{
    s16 *tbl;
    s32 no;

    if (Func_020065e0() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();

    tbl = Data_02000240;
    no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        if (GameFlag_IsSet(0x855) != 0) {
            Event_SetMessage(0x1376);
        } else {
            Event_SetMessage(0x1288);
        }
        break;
    case 11:
        Event_SetMessage(0x1ce8);
        break;
    case 20:
    case 21:
    case 50:
        Event_End();
        Func_02003790();
        return;
    default:
        break;
    }

    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (Func_02006708() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0)
        Event_SetMessage(0x171c);
    else
        Event_SetMessage(0x1408);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (Func_02006754() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    Event_SetMessage(0x190a);
    if (GameFlag_IsSet(0x909) != 0)
        Event_SetMessage(0x1951);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (Func_0200679c() != 0) {
        Sanctum_Open(8);
        return;
    }

    Event_Begin();
    Event_SetMessage(0x1823);
    Event_ShowMessage(8, 0);
    Event_End();
}
