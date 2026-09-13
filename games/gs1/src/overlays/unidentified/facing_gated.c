#include "types.h"
extern u8 *Data_03001ebc;

#define FieldScene_RunScriptedSceneSequence Func_0200088c

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
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a118();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
s32 Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a218();
void Func_0808a248();
void Func_0808a330();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_080f9010();
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

#define Func_0808a090(a0, a1, a2) Call3(Func_0808a090, a0, a1, a2)
#define Func_0808a1b8(a0, a1, a2) Call3(Func_0808a1b8, a0, a1, a2)
#define Func_0808a1e8(a0, a1, a2) Call3(Func_0808a1e8, a0, a1, a2)
#define Func_0808a1f0(a0, a1) Call2(Func_0808a1f0, a0, a1)

#include "types.h"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02003588
#define ResolveFacingObject Func_0200364e
#define UpdateFacingFromResolvedObject Func_02000030

#include "facing_object.h"

s16 Func_02003588(s32, s32);
struct FacingObject *Func_0200364e(s16);

#include "types.h"

#define FieldScene_RunActorEightResetSequence Func_020006e8
/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */
extern void Func_02003cdc();
void Func_02003dd8();
void Func_02003df8();
void Func_02003df4();
void Func_02003cf2();
void Func_02003da4();
void Func_02003d02();
void Func_02003d8a();
void Func_02003d10();
void Func_02003dd6();
void Func_02003dee();
void Func_02003d16();
void Func_02003d3a();

#include "types.h"

#define SceneState_ResetObject14Word108 Func_02002660
#define SceneState_ApplyTwoRects Func_02003270
/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */
extern u8 *Func_02005c76();
void Func_02005ccc();
void Func_020067fe(s32, s32, s32, s32, s32, s32);
void Func_0200681a(s32, s32, s32, s32, s32, s32);
void Func_020067c8(s32 frames);



/* Publish the scene's upper prompt and lower dialogue panel. */

#include "types.h"

#define SceneEffect_StepEllipseOrbit Func_020026f0
/*
 * Ellipse orbit step for resource_378.  The object is offset from its anchor
 * along two axes and its angle advanced once per call.
 */
s32 Func_02005c52();           /* Relocated call word, not a runtime address. */

s32 Func_02005c5e();           /* Relocated call word, not a runtime address. */

/*
 * The anchor at +104 supplies the centre; the result is published to the
 * object's +8/+16 and to its +56/+64 shadow pair, with +56 taken from a fresh
 * read of +8 rather than from x.  The two imports take the same angle and form
 * a cosine/sine pair; which is which is not settled.  The radii 14 and 10 and
 * the +100/+102 displacements are built from immediates.  The angle at +100 and
 * its step at +102 are separate halfwords, not one 32-bit field.
 */

#include "types.h"

#define GetActorState Func_020068be
#define IsActorFacingInward Func_020032a8

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

ActorState *Func_020068be(s32 actor_id);

#include "types.h"

#define FieldScene_RunActorEightFacingDialogue Func_020032d0
/* Close through scene 8 when facing inward; otherwise select the story line. */
extern s32 Func_0200657c(void);
void Func_02006a06(s32 scene);
void Func_020068d4(void);
s32 Func_020068ba(s32 flag);
void Func_0200699c(s32 message);
s32 Func_020068cc(s32 flag);
void Func_020069ae(s32 message);
void Func_020069b6(s32 message);
void Func_020069ce(s32 actor, s32 mode);
void Func_02006912(void);

#include "types.h"

#define SceneDialogue_RunActorEightFlaggedDialogue Func_0200345c
#define SceneDialogue_RunActorEightFollowupDialogue Func_020034a8
#define SceneDialogue_RunActorEightDialogue Func_020034f0
/* Close scene 8 when facing inward; otherwise choose its story line. */
extern s32 Func_02006708(void);
void Func_02006b92(s32 scene);
void Func_02006a60(void);
s32 Func_02006a46(s32 flag);
void Func_02006b28(s32 message);
void Func_02006b30(s32 message);
void Func_02006b48(s32 actor, s32 mode);
void Func_02006a8c(void);
/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
extern s32 Func_02006754(void);
void Func_02006bde(s32 scene);
void Func_02006aac(void);
void Func_02006b6a(s32 message);
s32 Func_02006a98(s32 flag);
void Func_02006b7a(s32 message);
void Func_02006b92_a(s32 actor, s32 mode);
void Func_02006ad6(void);
/* Close scene 8 when facing inward; otherwise emit its fixed story line. */
extern s32 Func_0200679c(void);
void Func_02006c26(s32 scene);
void Func_02006af4(void);
void Func_02006bb2(s32 message);
void Func_02006bca(s32 actor, s32 mode);
void Func_02006b0e(void);

#include "types.h"

#define BattleRuntime_WaitIfModeZero_1(a0, a1) Call2_scene_primary_script(Func_02004f5e, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02004f16(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02004e7c(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3_scene_primary_script(Func_02004eb6, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3_scene_primary_script(Func_02004ec0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3_scene_primary_script(Func_02004eca, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3_scene_primary_script(Func_02004ed4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3_scene_primary_script(Func_02004ede, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3_scene_primary_script(Func_02004ee8, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3_scene_primary_script(Func_02004ef2, a0, a1, a2)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02004fd4, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02004fe0(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02004f8a(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02004f94(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02004f9e(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02004fa8(args)
#define Object_LinkPair_1(args...) Func_02004fba(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02004f10(args)
#define Object_SetModeById_1(args...) Func_02004f90(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02004fa0(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02004f26(args)
#define Object_SetModeById_2(args...) Func_02004fa6(args)
#define Object_SetModeById_3(args...) Func_02004fae(args)
#define Object_SetModeById_4(args...) Func_02004fb6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02004fc6(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02004f4c(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_02004ffe(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_02005008(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_02005012(args)
#define ObjectMotion_SetAngleToward_8(args...) Func_0200501c(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_02005026(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02004f84(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_0200500c(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02004f92(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02005058, a0)
#define BattleEvent_RunActionAndWait_1(args...) Func_02005070(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02004fa6_a(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_02005058_a(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02005050(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3_scene_primary_script(Func_020050aa, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02004fc8(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_020050a0(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02004fd6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_0200507e(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02004fe4(args)
#define ObjectMotion_SetAngleToward_11(args...) Func_02005096(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02004ff4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_0200507c(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02005002(args)
#define ObjectMotion_SetAngleToward_12(args...) Func_020050b4(args)
#define ObjectMotion_SetAngleToward_13(args...) Func_020050be(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_0200501c_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_020050a4(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_0200502a(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3_scene_primary_script(Func_0200510e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_0200503c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020050c4(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_0200504a(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02005122(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02005058_b(args)
#define Audio_PlayCue_1(args...) Func_020051a6(args)
#define BattleRuntime_WaitIfModeZero_19(a0, a1) Call2_scene_primary_script(Func_02005186, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_02005194(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_02005072(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2_scene_primary_script(Func_02005172, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_0200518c, a0, a1, a2, a3)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_02005092(args)
#define Audio_PlayCue_2(args...) Func_020051e0(args)
#define Audio_PlayCue_3(a0) Call1(Func_020051e8, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3_scene_primary_script(Func_0200511e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3_scene_primary_script(Func_020050e8, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_1(args...) Func_02005112(args)
#define Audio_PlayCue_4(a0) Call1(Func_02005212, a0)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_020050d0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02005178(args)
#define Object_SetModeById_5(args...) Func_02005158(args)
#define ObjectMotion_SetAngleToward_14(args...) Func_02005196(args)
#define ObjectMotion_SetAngleToward_15(args...) Func_020051a0(args)
#define ObjectMotion_SetAngleToward_16(args...) Func_020051aa(args)
#define ObjectMotion_SetAngleToward_17(args...) Func_020051b4(args)
#define ObjectMotion_SetAngleToward_18(args...) Func_020051be(args)
#define ObjectMotion_SetAngleToward_19(args...) Func_020051c8(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_020051c0(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_020051c8_a(args)
#define ObjectMotion_SetVariantCallback_4(args...) Func_020051d0(args)
#define ObjectMotion_SetVariantCallback_5(args...) Func_020051d8(args)
#define ObjectMotion_SetVariantCallback_6(args...) Func_020051e0_a(args)
#define ObjectMotion_SetVariantCallback_7(args...) Func_020051e8_a(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_02005156(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_0200522e(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_02005164(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_0200516a(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02005242(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_02005178_a(args)
#define ObjectGroup_ConfigureChildValue_1(a0, a1) Call2_scene_primary_script(Func_0200523a, a0, a1)
#define Audio_PlayCue_5(args...) Func_020052d0(args)
#define Audio_PlayCue_6(a0) Call1(Func_020052d8, a0)
#define BattleRuntime_WaitIfModeZero_28(args...) Func_020051c2(args)
#define Scene_GetRecord_1(args...) Func_020051f0(args)
#define Scene_GetRecord_2(args...) Func_020051fc(args)
#define Audio_PlayCue_7(a0) Call1(Func_02005330, a0)
#define ObjectGroup_ConfigureChildValue_2(args...) Func_020052a8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_0200526a(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Func_02005200(args)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_02005312, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2(args...) Func_0200531e(args)
#define BattleRuntime_WaitIfModeZero_30(a0, a1) Call2_scene_primary_script(Func_02005340, a0, a1)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_0200534e(args)
#define BattleRuntime_WaitIfModeZero_32(args...) Func_0200522c(args)
#define Object_SetModeById_6(args...) Func_020052b0(args)
#define Audio_PlayCue_8(args...) Func_02005386(args)
#define BattleRuntime_WaitIfModeZero_33(args...) Func_02005244(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_020052ec(args)
#define BattleRuntime_WaitIfModeZero_34(args...) Func_02005252(args)
#define ObjectMotion_SetVariantCallback_8(args...) Func_020052f2(args)
#define ObjectMotion_SetVariantCallback_9(args...) Func_020052fa(args)
#define ObjectMotion_SetVariantCallback_10(args...) Func_02005302(args)
#define ObjectMotion_SetVariantCallback_11(args...) Func_0200530a(args)
#define ObjectMotion_SetVariantCallback_12(args...) Func_02005312_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_02005322(args)
#define BattleRuntime_WaitIfModeZero_35(args...) Func_02005288(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_02005360(args)
#define BattleRuntime_WaitIfModeZero_36(args...) Func_02005296(args)
#define ObjectMotion_SetAngleToward_20(args...) Func_02005348(args)
#define BattleRuntime_WaitIfModeZero_37(args...) Func_020052a6(args)
#define ObjectMotion_SetAngleToward_21(args...) Func_02005358(args)
#define BattleRuntime_WaitIfModeZero_38(args...) Func_020052b6(args)
#define ObjectMotion_SetAngleToward_22(args...) Func_02005368(args)
#define ObjectMotion_SetAngleToward_23(args...) Func_02005372(args)
#define ObjectMotion_SetAngleToward_24(args...) Func_0200537c(args)
#define ObjectMotion_SetAngleToward_25(args...) Func_02005386_a(args)
#define ObjectMotion_SetAngleToward_26(args...) Func_02005390(args)
#define BattleRuntime_WaitIfModeZero_39(args...) Func_020052ee(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_020053c6(args)
#define BattleRuntime_WaitIfModeZero_40(args...) Func_020052fc(args)
#define ObjectMotion_SetAngleToward_27(args...) Func_020053ae(args)
#define BattleRuntime_WaitIfModeZero_41(args...) Func_0200530c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02005394(args)
#define BattleRuntime_WaitIfModeZero_42(args...) Func_0200531a(args)
#define Object_SetModeById_7(args...) Func_0200539a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_020053aa(args)
#define BattleRuntime_WaitIfModeZero_43(args...) Func_02005330_a(args)
#define ObjectMotion_SetAngleToward_28(args...) Func_020053e2(args)
#define BattleRuntime_WaitIfModeZero_44(args...) Func_02005340_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_020053c8(args)
#define BattleRuntime_WaitIfModeZero_45(args...) Func_0200534e_a(args)
#define BattleEvent_RunActionAndWait_8(args...) Func_02005426(args)
#define BattleRuntime_WaitIfModeZero_46(args...) Func_0200535c(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3_scene_primary_script(Func_02005450, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3_scene_primary_script(Func_0200545c, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3_scene_primary_script(Func_02005468, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3_scene_primary_script(Func_02005474, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3_scene_primary_script(Func_02005480, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3_scene_primary_script(Func_0200548c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_47(args...) Func_020053aa_a(args)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3_scene_primary_script(Func_0200549e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_48(args...) Func_020053bc(args)
#define BattleEvent_RunActionAndWait_9(args...) Func_02005494(args)
#define BattleRuntime_WaitIfModeZero_49(args...) Func_020053ca(args)
#define Object_SetModeById_8(args...) Func_0200544a(args)
#define Object_SetModeById_9(args...) Func_02005452(args)
#define Object_SetModeById_10(args...) Func_0200545a(args)
#define Object_SetModeById_11(args...) Func_02005462(args)
#define Object_SetModeById_12(args...) Func_0200546a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200547a(args)
#define BattleRuntime_WaitIfModeZero_50(args...) Func_02005400(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_02005488(args)
#define BattleRuntime_WaitIfModeZero_51(args...) Func_0200540e(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3_scene_primary_script(Func_02005500, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_52(args...) Func_0200541e(args)
#define Object_LinkPair_2(args...) Func_020054d8(args)
#define Object_LinkPair_3(args...) Func_020054e2(args)
#define BattleRuntime_WaitIfModeZero_53(args...) Func_02005438(args)
#define ObjectMotion_SetAngleToward_29(args...) Func_020054ea(args)
#define ObjectMotion_SetAngleToward_30(args...) Func_020054f4(args)
#define ObjectMotion_SetAngleToward_31(args...) Func_020054fe(args)
#define ObjectMotion_SetAngleToward_32(args...) Func_02005508(args)
#define BattleRuntime_WaitIfModeZero_54(args...) Func_02005466(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_0200550e(args)
#define BattleRuntime_WaitIfModeZero_55(args...) Func_02005474_a(args)
#define BattleEvent_RunActionAndWait_10(args...) Func_0200554c(args)
#define BattleRuntime_WaitIfModeZero_56(args...) Func_02005482(args)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Call3_scene_primary_script(Func_02005574, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_57(args...) Func_02005492(args)
#define ObjectMotion_SetVariantCallback_13(args...) Func_02005532(args)
#define ObjectMotion_SetVariantCallback_14(args...) Func_0200553a(args)
#define ObjectMotion_SetVariantCallback_15(args...) Func_02005542(args)
#define ObjectMotion_SetVariantCallback_16(args...) Func_0200554a(args)
#define ObjectMotion_SetVariantCallback_17(args...) Func_02005552(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_02005562(args)
#define BattleRuntime_WaitIfModeZero_58(args...) Func_020054c8(args)
#define BattleEvent_RunActionAndWait_11(args...) Func_020055a0(args)
#define BattleRuntime_WaitIfModeZero_59(args...) Func_020054d6(args)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Call3_scene_primary_script(Func_020055c8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_60(args...) Func_020054e6(args)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3_scene_primary_script(Func_020055da, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_61(args...) Func_020054f8(args)
#define BattleEvent_RunActionAndWait_12(args...) Func_020055d0(args)
#define BattleRuntime_WaitIfModeZero_62(args...) Func_02005506(args)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3_scene_primary_script(Func_020055f8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_63(args...) Func_02005516(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_020055be(args)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3_scene_primary_script(Func_02005612, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64(args...) Func_02005530(args)
#define ObjectMotion_SetVariantCallback_18(args...) Func_020055d0_a(args)
#define ObjectMotion_SetVariantCallback_19(args...) Func_020055d8(args)
#define ObjectMotion_SetVariantCallback_20(args...) Func_020055e0(args)
#define ObjectMotion_SetVariantCallback_21(args...) Func_020055e8(args)
#define ObjectMotion_SetVariantCallback_22(args...) Func_020055f0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(args...) Func_02005600(args)
#define BattleRuntime_WaitIfModeZero_65(args...) Func_02005566(args)
#define BattleEvent_RunActionAndWait_13(args...) Func_0200563e(args)
#define BattleRuntime_WaitIfModeZero_66(args...) Func_02005574_a(args)
#define Object_SetModeById_13(args...) Func_020055f4(args)
#define Object_SetModeById_14(args...) Func_020055fc(args)
#define Object_SetModeById_15(args...) Func_02005604(args)
#define Object_SetModeById_16(args...) Func_0200560c(args)
#define Object_SetModeById_17(args...) Func_02005614(args)
#define ObjectMotion_CallThenWaitForAnimationChange_12(args...) Func_02005624(args)
#define BattleRuntime_WaitIfModeZero_67(args...) Func_020055aa(args)
#define BattleEvent_RunActionAndWait_14(args...) Func_02005682(args)
#define BattleRuntime_WaitIfModeZero_68(args...) Func_020055b8(args)
#define Object_SetModeById_18(args...) Func_02005638(args)
#define Object_SetModeById_19(args...) Func_02005640(args)
#define Object_SetModeById_20(args...) Func_02005648(args)
#define Object_SetModeById_21(args...) Func_02005650(args)
#define Object_SetModeById_22(args...) Func_02005658(args)
#define ObjectMotion_CallThenWaitForAnimationChange_13(args...) Func_02005668(args)
#define BattleRuntime_WaitIfModeZero_69(args...) Func_020055ee(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(args...) Func_02005696(args)
#define BattleRuntime_WaitIfModeZero_70(args...) Func_020055fc_a(args)
#define BattleEvent_RunActionAndWait_15(args...) Func_020056d4(args)
#define BattleRuntime_WaitIfModeZero_71(args...) Func_0200560a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_14(args...) Func_02005692(args)
#define BattleRuntime_WaitIfModeZero_72(args...) Func_02005618(args)
#define BattleEvent_RunActionAndWait_16(args...) Func_020056f0(args)
#define BattleRuntime_WaitIfModeZero_73(args...) Func_02005626(args)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Call3_scene_primary_script(Func_02005718, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Call3_scene_primary_script(Func_02005722, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3_scene_primary_script(Func_0200572c, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3_scene_primary_script(Func_02005738, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3_scene_primary_script(Func_02005742, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3_scene_primary_script(Func_0200574c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_74(args...) Func_0200566a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_11(args...) Func_02005712(args)
#define BattleEvent_RunActionAndWait_17(args...) Func_0200574a(args)
#define BattleRuntime_WaitIfModeZero_75(args...) Func_02005680(args)
#define ObjectMotion_SetAngleToward_33(args...) Func_02005732(args)
#define BattleRuntime_WaitIfModeZero_76(args...) Func_02005690(args)
#define ObjectMotion_CallThenWaitForAnimationChange_15(args...) Func_02005718_a(args)
#define BattleRuntime_WaitIfModeZero_77(args...) Func_0200569e(args)
#define BattleEvent_RunActionAndWait_18(args...) Func_02005776(args)
#define BattleRuntime_WaitIfModeZero_78(args...) Func_020056ac(args)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3_scene_primary_script(Func_020057a0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_22(a0, a1, a2) Call3_scene_primary_script(Func_020057ac, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_23(a0, a1, a2) Call3_scene_primary_script(Func_020057b8, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_24(a0, a1, a2) Call3_scene_primary_script(Func_020057c4, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_25(a0, a1, a2) Call3_scene_primary_script(Func_020057d0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_26(a0, a1, a2) Call3_scene_primary_script(Func_020057dc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_79(args...) Func_020056fa(args)
#define ObjectMotion_SetAngleToward_34(args...) Func_020057ac_a(args)
#define BattleRuntime_WaitIfModeZero_80(args...) Func_0200570a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_16(args...) Func_02005792(args)
#define BattleEvent_RunActionAndWait_19(args...) Func_020057ea(args)
#define BattleRuntime_WaitIfModeZero_81(args...) Func_02005720(args)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_0200578a(args)
#define ObjectMotion_SetAngleToward_35(args...) Func_020057dc_a(args)
#define ObjectMotion_SetAngleToward_36(args...) Func_020057e6(args)
#define ObjectMotion_SetAngleToward_37(args...) Func_020057f0(args)
#define ObjectMotion_SetAngleToward_38(args...) Func_020057fa(args)
#define ObjectMotion_SetAngleToward_39(args...) Func_02005804(args)
#define ObjectMotion_SetAngleToward_40(args...) Func_0200580e(args)
#define ObjectMotion_SetPositionAndReset_2(args...) Func_020057d0_a(args)
#define ObjectMotion_SetAngleToward_41(args...) Func_02005822(args)
#define ObjectMotion_SetAngleToward_42(args...) Func_0200582c(args)
#define ObjectMotion_SetAngleToward_43(args...) Func_02005836(args)
#define ObjectMotion_ArmCallback_2(args...) Func_02005870(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3_scene_primary_script(Func_0200587c, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3_scene_primary_script(Func_02005888, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3_scene_primary_script(Func_02005814, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_82(args...) Func_020057c2(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3_scene_primary_script(Func_020058a6, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3_scene_primary_script(Func_020058b2, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3_scene_primary_script(Func_020058be, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02005854(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02005866(args)
#define BattleRuntime_WaitIfModeZero_83(args...) Func_020057fc(args)
#define BattleEffect_SpawnLinkedResourceObject_27(a0, a1, a2) Call3_scene_primary_script(Func_020058f0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_28(a0, a1, a2) Call3_scene_primary_script(Func_02005906, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_29(a0, a1, a2) Call3_scene_primary_script(Func_02005912, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_30(a0, a1, a2) Call3_scene_primary_script(Func_0200591e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_31(a0, a1, a2) Call3_scene_primary_script(Func_0200592a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_32(a0, a1, a2) Call3_scene_primary_script(Func_02005936, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_84(args...) Func_02005854_a(args)
#define BattleRuntime_WaitIfModeZero_85(args...) Func_0200585a(args)
#define ObjectMotion_SetAngleToward_44(args...) Func_0200590c(args)
#define ObjectMotion_SetAngleToward_45(args...) Func_02005916(args)
#define ObjectMotion_SetAngleToward_46(args...) Func_02005920(args)
#define ObjectMotion_SetAngleToward_47(args...) Func_0200592a_a(args)
#define BattleRuntime_WaitIfModeZero_86(args...) Func_02005888_a(args)
#define BattleEffect_SpawnLinkedResourceObject_33(a0, a1, a2) Call3_scene_primary_script(Func_0200597a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_87(args...) Func_02005898(args)
#define ObjectMotion_SetAngleToward_48(args...) Func_0200594a(args)
#define BattleRuntime_WaitIfModeZero_88(args...) Func_020058a8(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_12(args...) Func_02005950(args)
#define BattleRuntime_WaitIfModeZero_89(args...) Func_020058b6(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3_scene_primary_script(Func_0200599a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_90(args...) Func_020058c8(args)
#define BattleEffect_SpawnLinkedResourceObject_34(a0, a1, a2) Call3_scene_primary_script(Func_020059bc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_91(args...) Func_020058da(args)
#define ObjectMotion_SetAngleToward_49(args...) Func_0200598c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_13(args...) Func_0200598c_a(args)
#define BattleRuntime_WaitIfModeZero_92(args...) Func_020058f2(args)
#define BattleEvent_RunActionAndWait_20(args...) Func_020059ca(args)
#define BattleRuntime_WaitIfModeZero_93(args...) Func_02005900(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3_scene_primary_script(Func_020059e4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_94(args...) Func_02005912_a(args)
#define BattleEffect_SpawnLinkedResourceObject_35(a0, a1, a2) Call3_scene_primary_script(Func_02005a04, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_95(args...) Func_02005922(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_14(args...) Func_020059ca_a(args)
#define BattleRuntime_WaitIfModeZero_96(args...) Func_02005930(args)
#define ObjectMotion_SetAngleToward_50(args...) Func_020059e2(args)
#define BattleRuntime_WaitIfModeZero_97(args...) Func_02005940(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_15(args...) Func_020059e8(args)
#define BattleRuntime_WaitIfModeZero_98(args...) Func_0200594e(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3_scene_primary_script(Func_02005a32, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_99(args...) Func_02005960(args)
#define BattleEvent_RunActionAndWait_21(args...) Func_02005a38(args)
#define BattleRuntime_WaitIfModeZero_100(args...) Func_0200596e(args)
#define Object_SetModeById_23(args...) Func_020059ee(args)
#define ObjectMotion_CallThenWaitForAnimationChange_17(args...) Func_020059fe(args)
#define BattleRuntime_WaitIfModeZero_101(args...) Func_02005984(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3_scene_primary_script(Func_020059e8_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3_scene_primary_script(Func_020059fc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02005a16(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02005a20(args)
#define BattleEffect_SpawnLinkedResourceObject_36(a0, a1, a2) Call3_scene_primary_script(Func_02005aa2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_102(args...) Func_020059c0(args)
#define ObjectMotion_SetPositionAndReset_4(args...) Func_02005a2a(args)
#define Object_LinkPair_4(args...) Func_02005a84(args)
#define ObjectMotion_SetAngleToward_51(args...) Func_02005a86(args)
#define ObjectMotion_SetAngleToward_52(args...) Func_02005a90(args)
#define BattleRuntime_WaitIfModeZero_103(args...) Func_020059ee_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_18(args...) Func_02005a76(args)
#define BattleRuntime_WaitIfModeZero_104(args...) Func_020059fc_a(args)
#define BattleEvent_RunActionAndWait_22(args...) Func_02005ad4(args)
#define BattleRuntime_WaitIfModeZero_105(args...) Func_02005a0a(args)
#define Object_SetModeById_24(args...) Func_02005a8a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_19(args...) Func_02005a9a(args)
#define BattleRuntime_WaitIfModeZero_106(args...) Func_02005a20_a(args)
#define BattleEvent_RunActionAndWait_23(args...) Func_02005af8(args)
#define BattleRuntime_WaitIfModeZero_107(args...) Func_02005a2e(args)
#define ObjectMotion_SetPositionAndReset_5(args...) Func_02005a98(args)
#define ObjectMotion_SetAngleToward_53(args...) Func_02005aea(args)
#define BattleRuntime_WaitIfModeZero_108(args...) Func_02005a48(args)
#define ObjectMotion_SetAngleToward_54(args...) Func_02005afa(args)
#define ObjectMotion_SetAngleToward_55(args...) Func_02005b04(args)
#define BattleRuntime_WaitIfModeZero_109(args...) Func_02005a62(args)
#define ObjectMotion_CallThenWaitForAnimationChange_20(args...) Func_02005aea_a(args)
#define BattleRuntime_WaitIfModeZero_110(args...) Func_02005a70(args)
#define BattleEvent_RunActionAndWait_24(args...) Func_02005b48(args)
#define BattleRuntime_WaitIfModeZero_111(args...) Func_02005a7e(args)
#define BattleEffect_SpawnLinkedResourceObject_37(a0, a1, a2) Call3_scene_primary_script(Func_02005b72, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_38(a0, a1, a2) Call3_scene_primary_script(Func_02005b7e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_112(args...) Func_02005a9c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_21(args...) Func_02005b24(args)
#define BattleRuntime_WaitIfModeZero_113(args...) Func_02005aaa(args)
#define ObjectMotion_SetAngleToward_56(args...) Func_02005b5c(args)
#define ObjectMotion_SetAngleToward_57(args...) Func_02005b66(args)
#define BattleRuntime_WaitIfModeZero_114(args...) Func_02005ac4(args)
#define BattleEvent_RunActionAndWait_25(args...) Func_02005b9c(args)
#define BattleRuntime_WaitIfModeZero_115(args...) Func_02005ad2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_16(args...) Func_02005b7a(args)
#define BattleRuntime_WaitIfModeZero_116(args...) Func_02005ae0(args)
#define BattleEvent_RunActionAndWait_26(args...) Func_02005bb8(args)
#define BattleRuntime_WaitIfModeZero_117(args...) Func_02005aee(args)
#define BattleEffect_SpawnLinkedResourceObject_39(a0, a1, a2) Call3_scene_primary_script(Func_02005be2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_40(a0, a1, a2) Call3_scene_primary_script(Func_02005bee, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_118(args...) Func_02005b0c(args)
#define BattleEffect_SpawnLinkedResourceObject_41(a0, a1, a2) Call3_scene_primary_script(Func_02005c00, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_119(args...) Func_02005b1e(args)
#define BattleEvent_RunActionAndWait_27(args...) Func_02005bf6(args)
#define BattleRuntime_WaitIfModeZero_120(args...) Func_02005b2c(args)
#define ObjectMotion_SetVariantCallback_23(args...) Func_02005bcc(args)
#define ObjectMotion_SetVariantCallback_24(args...) Func_02005bd4(args)
#define BattleEffect_SpawnLinkedResourceObject_42(a0, a1, a2) Call3_scene_primary_script(Func_02005c30, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_43(a0, a1, a2) Call3_scene_primary_script(Func_02005c3c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_121(args...) Func_02005b5a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_22(args...) Func_02005be2_a(args)
#define BattleRuntime_WaitIfModeZero_122(args...) Func_02005b68(args)
#define BattleEvent_RunActionAndWait_28(args...) Func_02005c40(args)
#define BattleRuntime_WaitIfModeZero_123(args...) Func_02005b76(args)
#define ObjectMotion_SetAngleToward_58(args...) Func_02005c28(args)
#define BattleRuntime_WaitIfModeZero_124(args...) Func_02005b86(args)
#define Object_SetModeById_25(args...) Func_02005c06(args)
#define BattleRuntime_WaitIfModeZero_125(args...) Func_02005b94(args)
#define Object_SetModeById_26(args...) Func_02005c14(args)
#define ObjectMotion_SetSpeedParameters_1_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02005f36, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02005f44, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_0200290c(args...) Func_02005f22(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_0200290c(args...) Func_02005faa(args)
#define BattleRuntime_WaitIfModeZero_2_0200290c(args...) Func_02005f30(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_0200290c(args...) Func_02005fb8(args)
#define BattleRuntime_WaitIfModeZero_3_0200290c(args...) Func_02005f3e(args)
#define ObjectMotion_SetAngleToward_1_0200290c(args...) Func_02005ff0(args)
#define Scene_GetRecord_1_0200290c(args...) Func_02005f7e(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200290c(args...) Func_02005fc8(args)
#define Scene_GetRecord_2_0200290c(args...) Func_02005f9e(args)
#define BattleRuntime_WaitIfModeZero_4_0200290c(args...) Func_02005f8e(args)
#define Scene_GetRecord_3(args...) Func_02005fbc_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_0200290c(args...) Func_0200600c(args)
#define Object_SetModeById_1_0200290c(args...) Func_02006024(args)
#define Scene_GetRecord_4(args...) Func_02005fda(args)
#define Scene_GetRecord_5(args...) Func_02005fe8_b(args)
#define ObjectMotion_Launch_1(args...) Func_02006062(args)
#define BattleRuntime_WaitIfModeZero_5_0200290c(args...) Func_02005fd8(args)
#define ObjectMotion_SetAngleToward_2_0200290c(args...) Func_0200608a(args)
#define ObjectMotion_Launch_2(args...) Func_02006082(args)
#define BattleRuntime_WaitIfModeZero_6_0200290c(args...) Func_02005ff8(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_0200290c(args...) Func_020060a0(args)
#define ObjectMotion_SetAngleToward_3_0200290c(args...) Func_020060b2(args)
#define BattleRuntime_WaitIfModeZero_7_0200290c(args...) Func_02006010(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_0200290c(args...) Func_02006098(args)
#define BattleRuntime_WaitIfModeZero_8_0200290c(args...) Func_0200601e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_0200290c(args...) Func_020060a6(args)
#define BattleRuntime_WaitIfModeZero_9_0200290c(args...) Func_0200602c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_0200290c(args...) Func_020060b4(args)
#define BattleRuntime_WaitIfModeZero_10_0200290c(args...) Func_0200603a(args)
#define ObjectMotion_SetAngleToward_4_0200290c(args...) Func_020060ec(args)
#define BattleRuntime_WaitIfModeZero_11_0200290c(args...) Func_0200604a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6_0200290c(args...) Func_020060d2(args)
#define BattleRuntime_WaitIfModeZero_12_0200290c(args...) Func_02006058(args)
#define Object_SetModeById_2_0200290c(args...) Func_020060d8(args)
#define Object_SetModeById_3_0200290c(args...) Func_020060e0(args)
#define Object_SetModeById_4_0200290c(args...) Func_020060e8(args)
#define Object_SetModeById_5_0200290c(args...) Func_020060f0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7_0200290c(args...) Func_02006100(args)
#define BattleRuntime_WaitIfModeZero_13_0200290c(args...) Func_02006086(args)
#define ObjectMotion_SetAngleToward_5_0200290c(args...) Func_02006138(args)
#define ObjectMotion_SetAngleToward_6_0200290c(args...) Func_02006142(args)
#define BattleRuntime_WaitIfModeZero_14_0200290c(args...) Func_020060a0_a(args)
#define Object_SetModeById_6_0200290c(args...) Func_02006120(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8_0200290c(args...) Func_02006130(args)
#define BattleRuntime_WaitIfModeZero_15_0200290c(args...) Func_020060b6(args)
#define ObjectMotion_SetAngleToward_7_0200290c(args...) Func_02006168(args)
#define ObjectMotion_SetAngleToward_8_0200290c(args...) Func_02006172(args)
#define BattleRuntime_WaitIfModeZero_16_0200290c(args...) Func_020060d0(args)
#define Object_SetModeById_7_0200290c(args...) Func_02006150(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9_0200290c(args...) Func_02006160(args)
#define BattleRuntime_WaitIfModeZero_17_0200290c(args...) Func_020060e6(args)
#define ObjectMotion_ArmCallback_1_0200290c(args...) Func_020061c8(args)
#define BattleRuntime_WaitIfModeZero_18_0200290c(args...) Func_020060f6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_0200290c(args...) Func_0200619e(args)
#define BattleRuntime_WaitIfModeZero_19_0200290c(args...) Func_02006104(args)
#define ObjectMotion_SetAngleToward_9_0200290c(args...) Func_020061c8_a(args)
#define BattleRuntime_WaitIfModeZero_20_0200290c(args...) Func_02006126(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_0200290c(args...) Func_020061ce(args)
#define BattleRuntime_WaitIfModeZero_21_0200290c(args...) Func_02006134(args)
#define ObjectMotion_Launch_3(args...) Func_020061ce_a(args)
#define ObjectMotion_ArmCallback_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006222, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_22_0200290c(args...) Func_02006150_a(args)
#define ObjectMotion_ArmCallback_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006234, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_23_0200290c(args...) Func_02006162(args)
#define ObjectMotion_ArmCallback_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006246, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_24_0200290c(args...) Func_02006174(args)
#define ObjectMotion_ArmCallback_5_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006258, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_25_0200290c(args...) Func_02006186(args)
#define ObjectMotion_ArmCallback_6_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_0200626a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_26_0200290c(args...) Func_02006198(args)
#define ObjectMotion_Launch_4(args...) Func_02006232(args)
#define ObjectMotion_ArmCallback_7_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006286, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_27_0200290c(args...) Func_020061b4(args)
#define ObjectMotion_ArmCallback_8_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006298, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_28_0200290c(args...) Func_020061c6(args)
#define ObjectMotion_ArmCallback_9_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_020062aa, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_29_0200290c(args...) Func_020061d8(args)
#define ObjectMotion_ArmCallback_10_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_020062bc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_30_0200290c(args...) Func_020061ea(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3_scene_primary_script(Func_020062ce, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_31_0200290c(args...) Func_020061fc(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3_scene_primary_script(Func_020062e0, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3_scene_primary_script(Func_020062ec, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_32_0200290c(args...) Func_0200621a(args)
#define BattleEffect_SpawnLinkedResourceObject_1_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_0200630e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_0200631a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_33_0200290c(args...) Func_02006238(args)
#define ObjectMotion_SetAngleToward_10_0200290c(args...) Func_020062ea(args)
#define BattleRuntime_WaitIfModeZero_34_0200290c(args...) Func_02006248(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10_0200290c(args...) Func_020062d0(args)
#define BattleRuntime_WaitIfModeZero_35_0200290c(args...) Func_02006256(args)
#define Scene_GetRecord_6(args...) Func_02006284(args)
#define Scene_GetRecord_7(args...) Func_02006290(args)
#define Scene_GetRecord_8(args...) Func_0200629a(args)
#define Scene_GetRecord_9(args...) Func_020062a4(args)
#define ObjectMotion_SetPositionAndReset_1_0200290c(a0, a1, a2) Value3(Func_020062e8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_0200290c(args...) Func_020062f2(args)
#define ObjectMotion_SetPositionAndReset_3_0200290c(args...) Func_020062fc(args)
#define ObjectMotion_SetPositionAndReset_4_0200290c(args...) Func_02006306(args)
#define ObjectMotion_SetPositionAndReset_5_0200290c(args...) Func_02006310(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2_0200290c(args...) Func_02006312(args)
#define ObjectMotion_SetPositionAndReset_6(args...) Func_02006324(args)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3_scene_primary_script(Func_020063a8, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_0200633e(args)
#define BattleRuntime_WaitIfModeZero_36_0200290c(args...) Func_020062dc(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4_0200290c(args...) Func_02006384(args)
#define BattleRuntime_WaitIfModeZero_37_0200290c(args...) Func_020062ea_a(args)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3_scene_primary_script(Func_020063ce, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38_0200290c(args...) Func_020062fc_a(args)
#define Scene_GetRecord_10(args...) Func_0200632a(args)
#define Scene_GetRecord_11(args...) Func_02006334(args)
#define ObjectMotion_SetVariantCallback_1_0200290c(a0, a1) Value2(Func_020063b0, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_0200640c, a0, a1, a2)
#define Object_SetModeById_8_0200290c(args...) Func_020063aa(args)
#define BattleEffect_SpawnLinkedResourceObject_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006426, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_39_0200290c(args...) Func_02006344(args)
#define Object_LinkPair_1_0200290c(args...) Func_020063fe(args)
#define BattleRuntime_WaitIfModeZero_40_0200290c(args...) Func_02006354(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11_0200290c(args...) Func_020063dc(args)
#define BattleRuntime_WaitIfModeZero_41_0200290c(args...) Func_02006362(args)
#define ObjectMotion_CallThenWaitForAnimationChange_12_0200290c(args...) Func_020063ea(args)
#define BattleRuntime_WaitIfModeZero_42_0200290c(args...) Func_02006370(args)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3_scene_primary_script(Func_02006454, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_43_0200290c(args...) Func_02006382(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5_0200290c(args...) Func_0200642a(args)
#define BattleRuntime_WaitIfModeZero_44_0200290c(args...) Func_02006390(args)
#define ObjectMotion_Launch_5(args...) Func_0200642a_a(args)
#define BattleRuntime_WaitIfModeZero_45_0200290c(args...) Func_020063a0(args)
#define ObjectMotion_Launch_6(args...) Func_0200643a(args)
#define BattleRuntime_WaitIfModeZero_46_0200290c(args...) ((void (*)())Func_020063b0_a)(args)
#define BattleRuntime_WaitIfModeZero_47_0200290c(args...) Func_020063b6(args)
#define Object_LinkPair_2_0200290c(args...) Func_02006470(args)
#define BattleRuntime_WaitIfModeZero_48_0200290c(args...) Func_020063c6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_13_0200290c(args...) Func_0200644e(args)
#define BattleRuntime_WaitIfModeZero_49_0200290c(args...) Func_020063d4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_14_0200290c(args...) Func_0200645c(args)
#define BattleRuntime_WaitIfModeZero_50_0200290c(args...) Func_020063e2(args)
#define ObjectMotion_SetPositionAndReset_7(args...) Func_0200644c(args)
#define ObjectMotion_SetAngleToward_11_0200290c(args...) Func_0200649e(args)
#define ObjectMotion_SetAngleToward_12_0200290c(args...) Func_020064a8(args)
#define BattleRuntime_WaitIfModeZero_51_0200290c(args...) Func_02006406(args)
#define Object_SetModeById_9_0200290c(args...) Func_02006486(args)
#define ObjectMotion_CallThenWaitForAnimationChange_15_0200290c(args...) Func_02006496(args)
#define BattleRuntime_WaitIfModeZero_52_0200290c(args...) Func_0200641c(args)
#define ObjectMotion_SetVariantCallback_2_0200290c(args...) Func_020064bc(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6_0200290c(args...) Func_020064cc(args)
#define BattleRuntime_WaitIfModeZero_53_0200290c(args...) Func_02006432(args)
#define ObjectMotion_ArmCallback_17(args...) Func_02006514(args)
#define BattleRuntime_WaitIfModeZero_54_0200290c(args...) Func_02006442(args)
#define ObjectMotion_CallThenWaitForAnimationChange_16_0200290c(args...) Func_020064ca(args)
#define BattleRuntime_WaitIfModeZero_55_0200290c(args...) Func_02006450(args)
#define ObjectMotion_SetAngleToward_13_0200290c(args...) Func_02006502(args)
#define BattleRuntime_WaitIfModeZero_56_0200290c(args...) Func_02006460(args)
#define ObjectMotion_CallThenWaitForAnimationChange_17_0200290c(args...) Func_020064e8(args)
#define BattleRuntime_WaitIfModeZero_57_0200290c(args...) Func_0200646e(args)
#define ObjectMotion_ArmCallback_18(args...) Func_02006550(args)
#define ObjectMotion_ArmCallback_19(args...) Func_0200655a(args)
#define BattleRuntime_WaitIfModeZero_58_0200290c(args...) Func_02006488(args)
#define ObjectMotion_SetVariantCallback_3_0200290c(args...) Func_02006528(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7_0200290c(args...) Func_02006538(args)
#define BattleRuntime_WaitIfModeZero_59_0200290c(args...) Func_0200649e_a(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020064de, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Call2_scene_primary_script(Func_020064e6, a0, a1)
#define Object_LookupAndStep_1(args...) Func_020064f4(args)
#define Object_LookupAndStep_2(args...) Func_020064fa(args)
#define ObjectMotion_SetSpeedParameters_3_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_020064f8, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_0200290c(a0, a1, a2) Call3_scene_primary_script(Func_02006506, a0, a1, a2)
#define ObjectMotion_Launch_7(args...) Func_02006570(args)
#define ObjectMotion_Launch_8(args...) Func_0200657a(args)
#define ObjectMotion_SetAngleToward_14_0200290c(args...) Func_0200659c(args)
#define ObjectMotion_SetAngleToward_15_0200290c(args...) Func_020065a6(args)
#define BattleRuntime_WaitIfModeZero_60_0200290c(args...) Func_02006504(args)
#define ObjectMotion_SetAngleToward_16_0200290c(args...) Func_020065b6(args)
#define ObjectMotion_SetAngleToward_17_0200290c(args...) Func_020065c0(args)
#define BattleRuntime_WaitIfModeZero_61_0200290c(args...) Func_0200651e(args)
#define ObjectMotion_SetAngleToward_18_0200290c(args...) Func_020065d0(args)
#define ObjectMotion_SetAngleToward_19_0200290c(args...) Func_020065da(args)
#define BattleRuntime_WaitIfModeZero_62_0200290c(args...) Func_02006538_a(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(args...) Func_0200659a(args)
#define ObjectMotion_SetPositionAndReset_8(args...) Func_020065ac(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_020065ba(args)
#define BattleRuntime_WaitIfModeZero_63_0200290c(args...) Func_02006558(args)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3_scene_primary_script(Func_0200663c, a0, a1, a2)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3_scene_primary_script(Func_02006648, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64_0200290c(args...) Func_02006576(args)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3_scene_primary_script(Func_0200665a, a0, a1, a2)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3_scene_primary_script(Func_02006666, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_65_0200290c(args...) Func_02006594(args)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3_scene_primary_script(Func_02006678, a0, a1, a2)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3_scene_primary_script(Func_02006684, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_66_0200290c(args...) Func_020065b2(args)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3_scene_primary_script(Func_02006696, a0, a1, a2)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3_scene_primary_script(Func_020066a2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_67_0200290c(args...) Func_020065d0_a(args)
#define ObjectMotion_SetAngleToward_20_0200290c(args...) Func_02006682(args)
#define ObjectMotion_SetAngleToward_21_0200290c(args...) Func_0200668c(args)
#define BattleRuntime_WaitIfModeZero_68_0200290c(args...) Func_020065ea(args)
#define Object_SetModeById_10_0200290c(args...) Func_0200666a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_18_0200290c(args...) Func_0200667a(args)
#define BattleRuntime_WaitIfModeZero_69_0200290c(args...) Func_02006600(args)
#define Object_SetModeById_11_0200290c(args...) Func_02006680(args)
#define ObjectMotion_CallThenWaitForAnimationChange_19_0200290c(args...) Func_02006690(args)
#define BattleRuntime_WaitIfModeZero_70_0200290c(args...) Func_02006616(args)
#define Object_LinkPair_3_0200290c(args...) Func_020066d0(args)
#define BattleRuntime_WaitIfModeZero_71_0200290c(args...) Func_02006626(args)
#define Object_SetModeById_12_0200290c(args...) Func_020066a6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_20_0200290c(args...) Func_020066b6(args)
#define BattleRuntime_WaitIfModeZero_72_0200290c(args...) Func_0200663c_a(args)
#define ObjectMotion_ArmCallback_28(args...) Func_0200671e(args)
#define BattleRuntime_WaitIfModeZero_73_0200290c(args...) Func_0200664c(args)
#define Object_SetModeById_13_0200290c(args...) Func_020066cc(args)
#define Object_SetModeById_14_0200290c(args...) Func_020066d4(args)
#define BattleRuntime_WaitIfModeZero_74_0200290c(args...) Func_02006662(args)
#define ObjectMotion_SetVariantCallback_5_0200290c(a0, a1) Call2_scene_primary_script((void (*)())Func_02005fe8, a0, a1)
#define ObjectMotion_SetPositionAndReset_10(a0, a1, a2) Call3_scene_primary_script((void (*)())Func_02005fe8_a, a0, a1, a2)
#define FieldScene_RunActorNineFlagDialogueA Func_0200027c
#define FieldScene_RunActorNineFlagDialogueB Func_020002ec
#define FieldScene_RunSupplementalSequenceOne Func_02000360
#define FieldScene_RunScene378SequenceB Func_020004e8
#define FieldScene_RunActorTenCountStep Func_02000574
#define FieldScene_RunActorUpdateSequence Func_0200187c
#define FieldScene_RunPairedActorChoreography Func_0200290c
#define FieldScene_DispatchBySceneId Func_02003334

extern s16 Data_02000240[];

void Func_02003870(void);
s32 Func_02003856(s32 flag);
void Func_02003938(s32 value);
void Func_02003940(s32 value);
void Func_02003956(s32 value);
void Func_02003916(s32 id, s32 enabled);
void Func_02003958(s32 id, s32 x, s32 y);
void Func_020038ae(s32 frames);
void Func_02003986(s32 id, s32 arg1);
void Func_020038ca(void);
void Func_020038e0(void);
s32 Func_020038c6(s32 flag);
void Func_020039a8(s32 value);
void Func_020039b0(s32 value);
void Func_020039c6(s32 value);
void Func_0200398c(s32 id, s32 enabled);
void Func_0200391a(s32 frames);
void Func_02003962(s32 id, s32 mode);
void Func_0200393e(void);
void Func_02001d28();
void Func_02003954_a();
void Func_02003974();
void Func_0200397c();
s32 Func_02003982();
void Func_02003998();
s32 Func_020039ac();
void Func_020039b4();
s32 Func_020039c8();
u8 *Func_020039e2();
void Func_020039ee();
u8 *Func_020039f2_b();
void Func_020039f2_a();
void Func_02003a12();
void Func_02003a12_a();
void Func_02003a12_b();
s32 Func_02003a20();
s32 Func_02003a22();
void Func_02003a28();
void Func_02003a36();
u8 *Func_02003a40();
s32 Func_02003a4c();
void Func_02003a50();
s32 Func_02003a68();
void Func_02003a6c();
void Func_02003a7e();
void Func_02003aa0();
void Func_02003aa2();
void Func_02003aa8();
void Func_02003aaa();
void Func_02003ab0();
void Func_02003aba();
void Func_02003ac4();
void Func_02003ad0();
void Func_02003ad4();
void Func_02003ad4_a();
void Func_02003ade();
void Func_02003af4();
void Func_02003af8();
s32 Func_02003b18();
void Func_02003b3e();
void Func_02003b56();
void Func_02003b62();
void Func_02003ba8();
s32 Func_02003bb8();
void Func_02003bbc();
void Func_02003bca();
void Func_02003bd4();
void Func_02003bd8();
void Func_02003bf6();
void Func_02003c02();
void Func_02003c12();
void Func_02003c1a();
s32 Func_02003a22_a();
void Func_02003b68();
void Func_02003c12_a();
void Func_02003b70();
void Func_02003c36();
s32 Func_02003c46();
s32 Func_02003ba6();
void Func_02003c72();
void Func_02003bb6();
void Func_020040c0();
void Func_020042a8();
void Func_02004d8c();
void Func_02004e7c();
void Func_02004eb6();
void Func_02004ec0();
void Func_02004eca();
void Func_02004ed4();
void Func_02004ede();
void Func_02004ee8();
void Func_02004ef2();
void Func_02004f10();
void Func_02004f16();
void Func_02004f26();
void Func_02004f4c();
void Func_02004f5e();
void Func_02004f84();
void Func_02004f8a();
void Func_02004f90();
void Func_02004f92();
void Func_02004f94();
void Func_02004f9e();
void Func_02004fa0();
void Func_02004fa6();
void Func_02004fa6_a();
void Func_02004fa8();
void Func_02004fae();
void Func_02004fb6();
void Func_02004fba();
void Func_02004fc6();
void Func_02004fc8();
void Func_02004fd4();
void Func_02004fd6();
void Func_02004fe0();
void Func_02004fe4();
void Func_02004ff4();
void Func_02004ffe();
void Func_02005002();
void Func_02005008();
void Func_0200500c();
void Func_02005012();
void Func_0200501c();
void Func_0200501c_a();
void Func_02005026();
void Func_0200502a();
void Func_0200503c();
void Func_0200504a();
void Func_02005058();
void Func_02005058_a();
void Func_02005058_b();
void Func_02005070();
void Func_02005072();
void Func_0200507c();
void Func_0200507e();
void Func_02005092();
void Func_02005096();
void Func_020050a0();
void Func_020050a4();
void Func_020050aa();
void Func_020050b4();
void Func_020050be();
void Func_020050c4();
void Func_020050d0();
void Func_020050e8();
void Func_0200510e();
void Func_02005112();
void Func_0200511e();
void Func_02005122();
void Func_02005156();
void Func_02005158();
void Func_02005164();
void Func_0200516a();
void Func_02005172();
void Func_02005178();
void Func_02005178_a();
void Func_02005186();
void Func_0200518c();
void Func_02005194();
void Func_02005196();
void Func_020051a0();
void Func_020051a6();
void Func_020051aa();
void Func_020051b4();
void Func_020051be();
void Func_020051c0();
void Func_020051c2();
void Func_020051c8();
void Func_020051c8_a();
void Func_020051d0();
void Func_020051d8();
void Func_020051e0();
void Func_020051e0_a();
void Func_020051e8();
void Func_020051e8_a();
u8 *Func_02005f7e();
void Func_02005200();
void Func_02005212();
void Func_0200522c();
void Func_0200522e();
void Func_0200523a();
void Func_02005242();
void Func_02005244();
void Func_02005252();
void Func_0200526a();
void Func_02005288();
void Func_02005296();
void Func_020052a6();
void Func_020052a8();
void Func_020052b0();
void Func_020052b6();
void Func_020052d0();
void Func_020052d8();
void Func_020052ec();
void Func_020052ee();
void Func_020052f2();
void Func_020052fa();
void Func_020052fc();
void Func_02005302();
void Func_0200530a();
void Func_0200530c();
void Func_02005312();
void Func_02005312_a();
void Func_0200531a();
void Func_0200531e();
void Func_02005322();
void Func_02005330();
void Func_02005330_a();
void Func_02005340();
void Func_02005340_a();
void Func_02005348();
void Func_0200534e();
void Func_0200534e_a();
void Func_02005358();
void Func_0200535c();
void Func_02005360();
void Func_02005368();
void Func_02005372();
void Func_0200537c();
void Func_02005386();
void Func_02005386_a();
void Func_02005390();
void Func_02005394();
void Func_0200539a();
void Func_020053aa();
void Func_020053aa_a();
void Func_020053ae();
void Func_020053bc();
void Func_020053c6();
void Func_020053c8();
void Func_020053ca();
void Func_020053e2();
void Func_02005400();
void Func_0200540e();
void Func_0200541e();
void Func_02005426();
void Func_02005438();
void Func_0200544a();
void Func_02005450();
void Func_02005452();
void Func_0200545a();
void Func_0200545c();
void Func_02005462();
void Func_02005466();
void Func_02005468();
void Func_0200546a();
void Func_02005474();
void Func_02005474_a();
void Func_0200547a();
void Func_02005480();
void Func_02005482();
void Func_02005488();
void Func_0200548c();
void Func_02005492();
void Func_02005494();
void Func_0200549e();
void Func_020054c8();
void Func_020054d6();
void Func_020054d8();
void Func_020054e2();
void Func_020054e6();
void Func_020054ea();
void Func_020054f4();
void Func_020054f8();
void Func_020054fe();
void Func_02005500();
void Func_02005506();
void Func_02005508();
void Func_0200550e();
void Func_02005516();
void Func_02005530();
void Func_02005532();
void Func_0200553a();
void Func_02005542();
void Func_0200554a();
void Func_0200554c();
void Func_02005552();
void Func_02005562();
void Func_02005566();
void Func_02005574();
void Func_02005574_a();
void Func_020055a0();
void Func_020055aa();
void Func_020055b8();
void Func_020055be();
void Func_020055c8();
void Func_020055d0();
void Func_020055d0_a();
void Func_020055d8();
void Func_020055da();
void Func_020055e0();
void Func_020055e8();
void Func_020055ee();
void Func_020055f0();
void Func_020055f4();
void Func_020055f8();
void Func_020055fc();
void Func_020055fc_a();
void Func_02005600();
void Func_02005604();
void Func_0200560a();
void Func_0200560c();
void Func_02005612();
void Func_02005614();
void Func_02005618();
void Func_02005624();
void Func_02005626();
void Func_02005638();
void Func_0200563e();
void Func_02005640();
void Func_02005648();
void Func_02005650();
void Func_02005658();
void Func_02005668();
void Func_0200566a();
void Func_02005680();
void Func_02005682();
void Func_02005690();
void Func_02005692();
void Func_02005696();
void Func_0200569e();
void Func_020056ac();
void Func_020056d4();
void Func_020056f0();
void Func_020056fa();
void Func_0200570a();
void Func_02005712();
void Func_02005718();
void Func_02005718_a();
void Func_02005720();
void Func_02005722();
void Func_0200572c();
void Func_02005732();
void Func_02005738();
void Func_02005742();
void Func_0200574a();
void Func_0200574c();
void Func_02005776();
void Func_0200578a();
void Func_02005792();
void Func_020057a0();
void Func_020057ac();
void Func_020057ac_a();
void Func_020057b8();
void Func_020057c2();
void Func_020057c4();
void Func_020057d0();
void Func_020057d0_a();
void Func_020057dc();
void Func_020057dc_a();
void Func_020057e6();
void Func_020057ea();
void Func_020057f0();
void Func_020057fa();
void Func_020057fc();
void Func_02005804();
void Func_0200580e();
void Func_02005814();
void Func_02005822();
void Func_0200582c();
void Func_02005836();
void Func_02005854();
void Func_02005854_a();
void Func_0200585a();
void Func_02005866();
void Func_02005870();
void Func_0200587c();
void Func_02005888();
void Func_02005888_a();
void Func_02005898();
void Func_020058a6();
void Func_020058a8();
void Func_020058b2();
void Func_020058b6();
void Func_020058be();
void Func_020058c8();
void Func_020058da();
void Func_020058f0();
void Func_020058f2();
void Func_02005900();
void Func_02005906();
void Func_0200590c();
void Func_02005912();
void Func_02005912_a();
void Func_02005916();
void Func_0200591e();
void Func_02005920();
void Func_02005922();
void Func_0200592a();
void Func_0200592a_a();
void Func_02005930();
void Func_02005936();
void Func_02005940();
void Func_0200594a();
void Func_0200594e();
void Func_02005950();
void Func_02005960();
void Func_0200596e();
void Func_0200597a();
void Func_02005984();
void Func_0200598c();
void Func_0200598c_a();
void Func_0200599a();
void Func_020059bc();
void Func_020059c0();
void Func_020059ca();
void Func_020059ca_a();
void Func_020059e2();
void Func_020059e4();
void Func_020059e8();
void Func_020059e8_a();
void Func_020059ee();
void Func_020059ee_a();
void Func_020059fc();
void Func_020059fc_a();
void Func_020059fe();
void Func_02005a04();
void Func_02005a0a();
void Func_02005a16();
void Func_02005a20();
void Func_02005a20_a();
void Func_02005a2a();
void Func_02005a2e();
void Func_02005a32();
void Func_02005a38();
void Func_02005a48();
void Func_02005a62();
void Func_02005a70();
void Func_02005a76();
void Func_02005a7e();
void Func_02005a84();
void Func_02005a86();
void Func_02005a8a();
void Func_02005a90();
void Func_02005a98();
void Func_02005a9a();
void Func_02005a9c();
void Func_02005aa2();
void Func_02005aaa();
void Func_02005ac4();
void Func_02005ad2();
void Func_02005ad4();
void Func_02005ae0();
void Func_02005aea();
void Func_02005aea_a();
void Func_02005aee();
void Func_02005af8();
void Func_02005afa();
void Func_02005b04();
void Func_02005b0c();
void Func_02005b1e();
void Func_02005b24();
void Func_02005b2c();
void Func_02005b48();
void Func_02005b5a();
void Func_02005b5c();
void Func_02005b66();
void Func_02005b68();
void Func_02005b72();
void Func_02005b76();
void Func_02005b7a();
void Func_02005b7e();
void Func_02005b86();
void Func_02005b94();
void Func_02005b9c();
void Func_02005bb8();
void Func_02005bcc();
void Func_02005bd4();
void Func_02005be2();
void Func_02005be2_a();
void Func_02005bee();
void Func_02005bf6();
void Func_02005c00();
void Func_02005c06();
void Func_02005c14();
void Func_02005c28();
void Func_02005c30();
void Func_02005c3c();
void Func_02005c40();
u8 *Func_020051f0();
u8 *Func_020051fc();
s32 Func_02005bc6();
void Func_02005f22();
void Func_02005f30();
void Func_02005f36();
void Func_02005f3e();
void Func_02005f44();
void Func_02005f8e();
void Func_02005faa();
void Func_02005fb8();
s32 Func_02005fbc();
s32 Func_02005fbc_a();
void Func_02005fc8();
void Func_02005fd8();
s32 Func_02005fda();
void Func_02005ff0();
void Func_02005ff8();
void Func_0200600c();
void Func_02006010();
void Func_0200601e();
void Func_02006024();
void Func_0200602c();
void Func_0200603a();
void Func_0200604a();
void Func_02006058();
void Func_02006062();
void Func_02006082();
void Func_02006086();
void Func_0200608a();
void Func_02006098();
void Func_020060a0();
void Func_020060a0_a();
void Func_020060a6();
void Func_020060b2();
void Func_020060b4();
void Func_020060b6();
void Func_020060d0();
void Func_020060d2();
void Func_020060d8();
void Func_020060e0();
void Func_020060e6();
void Func_020060e8();
void Func_020060ec();
void Func_020060f0();
void Func_020060f6();
void Func_02006100();
void Func_02006104();
void Func_02006120();
void Func_02006126();
void Func_02006130();
void Func_02006134();
void Func_02006138();
void Func_02006142();
void Func_02006150();
void Func_02006150_a();
void Func_02006160();
void Func_02006162();
void Func_02006168();
void Func_02006172();
void Func_02006174();
void Func_02006186();
void Func_02006198();
void Func_0200619e();
void Func_020061b4();
void Func_020061c6();
void Func_020061c8();
void Func_020061c8_a();
void Func_020061ce();
void Func_020061ce_a();
void Func_020061d8();
void Func_020061ea();
void Func_020061fc();
void Func_0200621a();
void Func_02006222();
void Func_02006232();
void Func_02006234();
void Func_02006238();
void Func_02006246();
void Func_02006248();
void Func_02006256();
void Func_02006258();
void Func_0200626a();
void Func_02006286();
u8 *Func_02006284();
void Func_02006298();
u8 *Func_02006290();
u8 *Func_0200629a();
void Func_020062aa();
void Func_020062bc();
void Func_020062ce();
void Func_020062d0();
void Func_020062dc();
void Func_020062e0();
s32 Func_020062e8();
void Func_020062ea();
void Func_020062ea_a();
void Func_020062ec();
void Func_020062f2();
void Func_020062fc();
void Func_020062fc_a();
void Func_02006306();
void Func_0200630e();
void Func_02006310();
void Func_02006312();
void Func_0200631a();
void Func_02006324();
u8 *Func_020062a4();
u8 *Func_0200632a();
void Func_0200633e();
void Func_02006344();
void Func_02006354();
void Func_02006362();
void Func_02006370();
void Func_02006382();
void Func_02006384();
void Func_02006390();
void Func_020063a0();
void Func_020063a8();
void Func_020063aa();
s32 Func_020063b0();
s32 Func_020063b0_a();
void Func_020063b6();
void Func_020063c6();
void Func_020063ce();
void Func_020063d4();
void Func_020063dc();
void Func_020063e2();
void Func_020063ea();
void Func_020063fe();
void Func_02006406();
void Func_0200640c();
void Func_0200641c();
void Func_02006426();
void Func_0200642a();
void Func_0200642a_a();
void Func_02006432();
void Func_0200643a();
void Func_02006442();
void Func_0200644c();
void Func_0200644e();
void Func_02006450();
void Func_02006454();
void Func_0200645c();
void Func_02006460();
void Func_0200646e();
void Func_02006470();
void Func_02006486();
void Func_02006488();
void Func_02006496();
void Func_0200649e();
void Func_0200649e_a();
void Func_020064a8();
void Func_020064bc();
void Func_020064ca();
void Func_020064cc();
s32 Func_020064de();
void Func_020064e6();
void Func_020064e8();
void Func_020064f4();
void Func_020064f8();
void Func_020064fa();
void Func_02006502();
void Func_02006504();
void Func_02006506();
void Func_02006514();
void Func_0200651e();
void Func_02006528();
void Func_02006538();
void Func_02006538_a();
void Func_02006550();
void Func_02006558();
void Func_0200655a();
void Func_02006570();
void Func_02006576();
void Func_0200657a();
void Func_02006594();
void Func_0200659a();
void Func_0200659c();
void Func_020065a6();
void Func_020065ac();
void Func_020065b2();
void Func_020065b6();
void Func_020065ba();
void Func_020065c0();
void Func_020065d0();
void Func_020065d0_a();
void Func_020065da();
void Func_020065ea();
void Func_02006600();
void Func_02006616();
void Func_02006626();
void Func_0200663c();
void Func_0200663c_a();
void Func_02006648();
void Func_0200664c();
void Func_0200665a();
void Func_02006662();
void Func_02006666();
void Func_0200666a();
void Func_02006678();
void Func_0200667a();
void Func_02006680();
void Func_02006682();
void Func_02006684();
void Func_0200668c();
void Func_02006690();
void Func_02006696();
void Func_020066a2();
void Func_020066a6();
void Func_020066b6();
void Func_020066cc();
void Func_020066d0();
void Func_020066d4();
void Func_0200671e();
u8 *Func_02005fe8();
u8 *Func_02005fe8_a();
u8 *Func_02005fe8_b();
u8 *Func_02005fe8_c();
u8 *Func_0200632a_a();
u8 *Func_02006334();
s32 Func_020065e0();
void Func_02006a6a();
void Func_02006938();
s32 Func_020069dc();
void Func_02006abe();
void Func_02006ac6();
void Func_02006ace();
void Func_02006a24();
void Func_02003790();
void Func_02006af2();
void Func_02006a36();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200187c(s32 amount)
{
    void Func_02005050();
    u8 *Func_02005f9e();

    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200290c(s32 amount)
{
    u8 *Func_02005050();
    s32 Func_02005f9e();

    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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

#include "types.h"

#define SceneData_GetTableBaa8 Func_0200005c
#define SceneData_ReturnZero Func_02000064
#define SceneData_GetTableBbc8 Func_02000068
#define SceneData_SelectPlacementTableBySubstate Func_02000070
#define SceneData_SelectTableBySceneIdAndFlags Func_0200014c

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

s32 Func_020037f8();
s32 Func_02003806();

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

    if (Func_020037f8(0x87a) != 0) {
        return Data_0200bfd8;
    }
    if (Func_02003806(0x815) != 0) {
        return Data_0200bf78;
    }
    return Data_0200be34;
}

void FieldScene_RunActorNineFlagDialogueA(void)
{
    Func_02003870();

    if (Func_02003856(0x855) != 0) {
        Func_02003938(0x1377);
    } else {
        Func_02003940(0x1289);
    }

    if (Data_02000240[225] == 11) {
        Func_02003956(0x1ce9);
    }

    Func_02003916(9, 1);
    Func_02003958(9, 0, 0);
    Func_020038ae(2);
    Func_02003986(9, 0);
    Func_020038ca();
}

void FieldScene_RunActorNineFlagDialogueB(void)
{
    void Func_02003954(s32 id);
    void Func_020039f2(s32 id, s32 arg1);

    Func_020038e0();

    if (Func_020038c6(0x855) == 0) {
        Func_020039a8(0x128b);
    } else {
        Func_020039b0(0x1379);
    }

    if (Data_02000240[225] == 11) {
        Func_020039c6(0x1ceb);
    }

    Func_02003954(9);
    Func_0200398c(9, 1);
    Func_0200391a(2);
    Func_020039f2(9, 0);
    Func_02003962(9, 2);
    Func_0200393e();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    u32 i;
    u8 *record;

    Func_02003954_a();
    Call1(Func_02003a12, 0x1164);
    Value2(Func_02003a22, 8, 0);
    if (Value2(Func_02003982, 0, 0) == 0) {
        Func_020039ee(8, 3);
        Func_02003974(20);
    } else {
        Func_0200397c(20);
        Value2(Func_02003a4c, 8, 0);
        if (Value2(Func_020039ac, 0, 0) != 0) {
        } else {
            Func_02003998(20);
            Value2(Func_02003a68, 8, 0);
            if (Value2(Func_020039c8, 0, 0) != 0) {
            } else {
                Func_020039b4(20);
                record = Func_020039e2(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 low = 0xa000;

                    if (shown < low) {
                        goto L_02000488;
                    }
                }
                record = Func_020039f2_b(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 high = 0xe000;

                    if (shown > high) {
                        goto L_02000488;
                    }
                }
                Call3_scene_primary_script(Func_02003a12_a, 8, 0x8000, 0x4000);
                Func_02003ac4(8, 0, 0);
                Call1(Func_020039f2_a, 10);
                *(u8 *)(Func_02003a20(8) + 90) &= 254;
                Func_02003a6c(8, 152, 120);
                Func_02003a12_b(1);
                {
                    u8 *record = Func_02003a40(8);
                    u8 value = *(volatile u8 *)&record[90];

                    record[90] = (u8)(value | 1);
                }
                Func_02003a28(20);
                Func_02003ab0(8, 3);
                Func_02003a36(20);
                Func_02003aa0(0, 168, 120);
                Func_02003aa2(0, 192, 168);
                Func_02003a50(20);
                Func_02003aba(8, 168, 120);
                Call3_scene_primary_script(Func_02003b3e, 8, 0x3000, 0);
                Func_02003ad4(0);
                goto L_020004aa;
                L_02000488:
                Func_02003ad0(0, 192, 168);
                Func_02003a7e(20);
                Call3_scene_primary_script(Func_02003b62, 8, 0x3000, 0);
                Func_02003af8(0);
                L_020004aa:
                Func_02001d28();
                Func_02003bbc(0, 0);
                Func_02003bca(120);
                Func_02003aa8(120);
                Func_02003bf6(86);
                Func_02003c02();
                Call1(Func_02003aaa, 0x9f0);
                Func_02003bd8(30);
            }
        }
    }
    Func_02003ad4_a();
}

void FieldScene_RunScene378SequenceB(void)
{
    void Func_02003954();
    u8 *Func_020039f2();

    u32 i;
    s32 record;

    Func_02003ade();
    Call1(Func_02003af4, 0x200bc9c);
    Call1((void (*)())Func_02003a22_a, 1);
    Call1(Func_02003ba8, 0x1bfd);
    Value2(Func_02003bb8, 9, 0);
    if (Value2(Func_02003b18, 0, 0) == 0) {
        Func_02003bd4(9, 0);
    } else {
        bump_step();
        Call11(Func_02003c1a, 2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Func_02003c12(9, 0);
    }
    Func_02003b56();
}

void FieldScene_RunActorTenCountStep(void)
{

    Func_02003b68();
    Func_02003c12_a(10, 0, 0);
    Func_02003b70(10);
    Func_02003c36(0x119f);
    Func_02003c46(10, 0);

    if (Func_02003ba6(0, 0) == 1) {
        (*(u16 *)(Data_03001ebc + 472))++;
    }

    Func_02003c72(10, 0);
    Func_02003bb6();
}

void FieldScene_RunActorEightResetSequence(void)
{
    Func_02003cdc();
    Func_02003dd8(0x10000, 0x2000);
    Func_02003df8(1, 1);
    Func_02003df4();
    Func_02003cf2(20);
    Func_02003da4(8, 0, 0);
    Func_02003d02(10);
    Func_02003d8a(8, 4);
    Func_02003d10(20);
    Func_02003dd6(0x116c);
    Func_02003dee(8, 0);
    Func_02003d16(0x200);
    Func_02003d3a();
}

void FieldScene_RunScriptedSceneSequence(void)
{
    u32 i;

    Func_0808a018();
    if (Func_080770c0(0x201) != 0) {
        Func_0808a150(0, 8, 0);
        Func_0808a148(1, 0, 0);
        Func_0808a148(0xb, 0, 0);
        Func_0808a148(0xc, 0, 0);
        Func_0808a148(9, 0, 0);
        Func_0808a148(0xa, 0, 0);
        Func_0808a210(0xc00000, -1, 0xa00000, 1);
        Func_0808a218();
        SCENE_REQUEST = 0x100;
        SCENE_SETUP_WORD = 0x40;
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(0x78);
        goto dialogue;
    }

    Func_0808a330(0x10002, 0);
    Func_0808a348(1);
    Func_0808a010(1);
    Func_0808a210(0xc00000, -1, 0xa00000, 1);
    Func_0808a218();
    SCENE_REQUEST = 0x209;
    Func_0808a360();
    Func_0808a370();
    Func_0200290c();
    Func_0808a330(0x10000, 0);
    Func_0808a348(0x3c);
    Func_0808a010(0x64);
    Func_0808a100(0, 1);
    Func_0808a100(1, 1);
    Func_0808a010(0x1e);
    Func_0808a148(0, 0xc, 0);
    Func_0808a148(1, 0xc, 0);
    Func_0808a010(0x14);
    Func_0808a098(8, 1);
    Func_0808a098(0xc, 1);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(0xb, 0xcccc, 0x6666);
    Func_0808a090(0xc, 0xcccc, 0x6666);
    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a090(0xa, 0xcccc, 0x6666);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a010(0x1e);
    Func_0808a138(0xc, 2);
    Func_0808a170(0x1138);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(0x1e);
    Func_0808a110(0xc, 3);
    Func_0808a010(0x14);
    Func_0808a138(0xb, 2);
    Func_0808a010(0x14);
    Func_0808a148(0, 0xb, 0);
    Func_0808a148(1, 0xb, 0);
    Func_0808a010(0x14);
    Func_0808a178(0xb, 0);
    /* Each arm advances once, on its own side of the object-state call. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(0x14);
        Func_0808a110(0xb, 3);
        Func_0808a010(0x14);
        Func_0808a180(0xb, 0);
        SKIP_BEATS++;
    } else {
        Func_0808a010(0x14);
        Func_0808a110(0xb, 4);
        Func_0808a010(0x14);
        SKIP_BEATS++;
        Func_0808a180(0xb, 0);
    }
    Func_0808a010(0x14);
    Func_0808a138(9, 1);
    Func_0808a010(0x14);
    Func_0808a148(0, 9, 0);
    Func_0808a148(1, 9, 0);
    Func_0808a148(0xb, 9, 0);
    Func_0808a010(0x14);
    Func_0808a178(9, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(0x14);
        Func_0808a110(9, 3);
        Func_0808a010(0x14);
        Func_0808a180(9, 0);
        SKIP_BEATS++;
    } else {
        Func_0808a010(0x14);
        Func_0808a110(9, 4);
        Func_0808a010(0x14);
        SKIP_BEATS++;
        Func_0808a180(9, 0);
    }
    Func_0808a010(0x14);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(0xb, 3);
    Func_0808a100(0xc, 3);
    Func_0808a100(9, 3);
    Func_0808a110(0xa, 3);
    Func_0808a010(0x1e);
    Func_0808a1e8(0xc, 0x101, 0);
    Func_0808a010(0x3c);
    Func_0808a148(0xc, 8, 0);
    Func_0808a010(0x14);
    Func_0808a0d0(0xc, 0xe0, 0x78);
    Func_0808a010(0xa);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x14);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a148(0xb, 8, 0);
    Func_0808a148(9, 8, 0);
    Func_0808a148(0xa, 8, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xb, 1);
    Func_0808a010(0xa);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x1e);
    Func_0808a138(8, 3);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a010(0x3c);
    Func_0808a148(8, 0xc, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 3);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a1b8(0xc, 0x5000, 0);
    Func_0808a010(0x3c);
    Func_0808a148(0xc, 8, 0);
    Func_0808a010(0x14);
    Func_0808a110(0xc, 3);
    Func_0808a010(0xa);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a110(8, 3);
    Func_0808a010(0x14);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a1b8(0xc, 0x5000, 0);
    Func_0808a010(0x3c);
    Func_0808a148(0xc, 8, 0);
    Func_0808a010(0x32);
    Func_0808a1e8(0xc, 0x101, 0);
    Func_0808a010(0x28);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(0xb, 0x101, 0);
    Func_0808a1e8(9, 0x101, 0);
    Func_0808a1e8(0xa, 0x101, 0);
    Func_0808a010(0x3c);
    Func_0808a138(8, 1);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0xa);
    Func_0808a1e8(0, 0x100, 0);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a1e8(0xb, 0x100, 0);
    Func_0808a1e8(0xc, 0x100, 0);
    Func_0808a1e8(9, 0x100, 0);
    Func_0808a1e8(0xa, 0x100, 0);
    Func_0808a010(0x3c);
    Func_0808a138(1, 2);
    Func_0808a010(0xa);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 3);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0xa);
    Func_0808a1e8(0xc, 0x102, 0);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(0xb, 1);
    Func_0808a130(9, 1);
    Func_0808a138(0xa, 1);
    Func_0808a010(0xa);
    Func_0808a110(8, 4);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0x1e);
    Func_0808a148(0xc, 0, 0);
    Func_0808a148(0xb, 0, 0);
    Func_0808a010(0x14);
    Func_0808a138(0, 2);
    Func_0808a010(0x3c);
    Func_0808a138(0xc, 2);
    Func_0808a010(0x14);
    Func_0808a148(0xc, 8, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x1e);
    Func_0808a138(0xb, 2);
    Func_0808a010(0x14);
    Func_0808a148(8, 0, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x14);
    Func_0808a010(0x28);
    Func_0808a138(8, 2);
    Func_0808a100(8, 0);
    Func_0808a158(8, 0x100);
    Func_020025c4();
    Func_080f9010(0xc4);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x20);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x10);
    for (i = 0; i < 6; i++) {
        Func_02002750(8, 0x1200);
        Func_0808a010(8);
    }
    Func_0808a010(8);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x20);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x60);
    Func_0808a010(0x20);
    Func_0808a158(8, 0);
    Func_0808a010(0x1e);
    Func_02002660();
    Func_0808a100(8, 1);
    Func_0808a138(8, 2);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a1f0(0xc, 0x102);
    Func_0808a1f0(0, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a1f0(0xb, 0x102);
    Func_0808a1f0(9, 0x102);
    Func_0808a1f0(0xa, 0x102);
    Func_0808a010(0x3c);
    Func_0808a138(0xa, 1);
    Func_0808a010(0x14);
    Func_0808a180(0xa, 0);
    Func_0808a010(0x14);
    Func_0808a150(0, 1, 0);
    Func_0808a010(0x3c);
    Func_0808a110(8, 4);
    Func_0808a010(0x14);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x1e);
    Func_0808a150(0, 1, 0);
    Func_0808a010(0x14);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a150(9, 0xa, 0);
    Func_0808a010(0x14);
    Func_0808a100(9, 3);
    Func_0808a100(0xa, 3);
    Func_0808a118(0xa);
    Func_0808a010(0x1e);
    Func_0808a150(0, 0xb, 0);
    Func_0808a150(1, 0xc, 0);
    Func_0808a010(0x14);
    Func_0808a100(0, 3);
    Func_0808a100(0xb, 3);
    Func_0808a100(1, 3);
    Func_0808a100(0xc, 3);
    Func_0808a118(0xc);
    Func_0808a010(0x3c);
    Func_0808a138(8, 2);
    Func_0808a100(8, 0);
    Func_0808a158(8, 0x100);
    Func_020025c4();
    Func_080f9010(0xc4);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x20);
    Func_02002750(8, 0x1200);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a148(0xb, 8, 0);
    Func_0808a148(0xc, 8, 0);
    Func_0808a148(9, 8, 0);
    Func_0808a148(0xa, 8, 0);
    Func_0808a010(0x10);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(0xb, 1);
    Func_0808a130(0xc, 1);
    Func_0808a130(9, 1);
    Func_0808a130(0xa, 1);
    for (i = 0; i < 6; i++) {
        Func_02002750(8, 0x1200);
        Func_0808a010(8);
    }
    Func_0808a010(8);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x20);
    Func_02002750(8, 0x1200);
    Func_0808a010(0x80);
    Func_0808a158(8, 0);
    Func_0808a010(0x1e);
    Func_02002660();
    Func_0808a100(8, 1);
    Func_0808a138(8, 2);
    Func_0808a010(0x1e);
    Func_0808a1e8(0xc, 0x105, 0);
    Func_0808a010(0x14);
    Func_0808a150(9, 0xa, 0);
    Func_0808a010(0x28);
    Func_0808a148(9, 8, 0);
    Func_0808a148(0xa, 8, 0);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x14);
    Func_0808a138(0xb, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x14);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a150(0, 1, 0);
    Func_0808a130(9, 1);
    Func_0808a130(0xa, 1);
    Func_0808a150(9, 0xa, 0);
    Func_0808a1f0(0xc, 0x102);
    Func_0808a010(0x3c);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a148(9, 8, 0);
    Func_0808a148(0xa, 8, 0);
    Func_0808a010(0x14);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 3);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(0xb, 0x101, 0);
    Func_0808a1e8(0xc, 0x101, 0);
    Func_0808a1e8(9, 0x101, 0);
    Func_0808a1e8(0xa, 0x101, 0);
    Func_0808a010(0x3c);
    Func_0808a110(8, 4);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(0xb, 3);
    Func_0808a100(0xc, 3);
    Func_0808a100(9, 3);
    Func_0808a100(0xa, 3);
    Func_0808a118(0xa);
    Func_0808a010(0x14);
    Func_0808a138(0xc, 1);
    Func_0808a010(0x14);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x14);
    Func_0808a148(8, 0xc, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 3);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x32);
    Func_0808a148(8, 0, 0);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x1e);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a1e8(0xb, 0x102, 0);
    Func_0808a1e8(0xc, 0x102, 0);
    Func_0808a1e8(9, 0x102, 0);
    Func_0808a1e8(0xa, 0x102, 0);
    Func_0808a010(0x50);
    Func_0808a138(0xb, 1);
    Func_0808a010(0x14);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 4);
    Func_0808a010(0x14);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(0xb, 0x101, 0);
    Func_0808a1e8(0xc, 0x101, 0);
    Func_0808a1e8(9, 0x101, 0);
    Func_0808a1e8(0xa, 0x101, 0);
    Func_0808a010(0x50);
    Func_0808a138(8, 1);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a1f0(0, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a1f0(0xb, 0x102);
    Func_0808a1f0(0xc, 0x102);
    Func_0808a1f0(9, 0x102);
    Func_0808a1f0(0xa, 0x102);
    Func_0808a010(0x3c);
    Func_0808a110(0xc, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x14);
    Func_0808a138(8, 1);
    Func_0808a010(0x14);
    Func_0808a110(8, 4);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x28);
    Func_0808a150(0, 1, 0);
    Func_0808a150(9, 0xa, 0);
    Func_0808a148(0xc, 0, 0);
    Func_0808a010(0x3c);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a148(0xb, 8, 0);
    Func_0808a148(0xc, 8, 0);
    Func_0808a148(9, 8, 0);
    Func_0808a148(0xa, 8, 0);
    Func_0808a010(0x14);
    Func_0808a138(0xb, 1);
    Func_0808a010(0x14);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x3c);
    Func_0808a0d0(8, 0xc8, 0x88);
    Func_0808a148(0, 8, 0);
    Func_0808a148(0xc, 0, 0);
    Func_0808a150(8, 1, 0);
    Func_0808a010(0x28);
    Func_0808a110(8, 3);
    Func_0808a010(0x32);
    Func_0808a148(8, 0, 0);
    Func_0808a010(0x28);
    Func_0808a110(8, 3);
    Func_0808a010(0x32);
    Func_0808a138(8, 1);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x1e);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(0x3c);
    Func_0808a1f0(0xb, 0x102);
    Func_0808a138(0xb, 1);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x1e);
    Func_0808a110(8, 3);
    Func_0808a010(0x1e);
    Func_0808a1e8(0xc, 0x102, 0);
    Func_0808a130(0xc, 1);
    Func_0808a010(0x14);
    Func_0808a180(0xc, 0);
    Func_0808a010(0x1e);
    Func_0808a0d0(8, 0xa8, 0x78);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a148(0xc, 8, 0);
    Func_0808a1b8(8, 0xd000, 0);
    Func_0808a010(0x14);
    Func_0808a110(8, 4);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x1e);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(0x3c);
    Func_0808a110(8, 3);
    Func_0808a010(0xa);
    Func_0808a180(8, 0);
    Func_0808a010(0xa);
    Func_0808a148(8, 0, 0);
    Func_0808a010(0x14);
    Func_0808a180(8, 0);
    Func_0808a010(0x14);
    Func_0808a148(0xc, 0, 0);
    Func_0808a1f0(1, 0x102);
    Func_0808a138(1, 2);
    Func_0808a148(1, 0, 0);
    Func_0808a010(0x14);
    Func_0808a180(1, 0);
    Func_0808a010(0x1e);
dialogue:
    Func_0808a170(0x1162);
    Func_0808a138(8, 1);
    Func_0808a010(0x14);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0200187c();
        Func_0808a330(0, 0);
        Func_0808a348(0x78);
        Func_0808a010(0x78);
        Func_080f9010(0x56);
        Func_080f9078();
        Func_080770c8(0x9f0);
        Func_0808a248(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Func_0808a010(0x1e);
        Func_0808a138(0xc, 1);
        Func_0808a110(0xc, 4);
        Func_0808a010(0x14);
        Func_0808a180(0xc, 0);
        Func_0808a010(0x14);
        actor = Func_0808a080(0xa);
        actor += 0x23;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = Func_0808a080(0xa);
        *(u32 *)(actor + 0x6c) = 0x02008849;
    }
    Func_0808a020();
}

void FieldScene_RunActorUpdateSequence(void)
{
    void Func_02005050();
    u8 *Func_02005f9e();

    u32 i;
    u8 *record;

    BattleRuntime_WaitIfModeZero_1(1, 0x102);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 2);
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(11, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_4(12, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_6(10, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_7(8, 0xcccc, 0x6666);
    ObjectMotion_PlaceWithinCameraBounds_1(0xc00000, -1, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    ObjectMotion_SetAngleToward_1(1, 0, 0);
    ObjectMotion_SetAngleToward_2(12, 0, 0);
    ObjectMotion_SetAngleToward_3(9, 0, 0);
    ObjectMotion_SetAngleToward_4(10, 0, 0);
    Object_LinkPair_1(0, 11, 0);
    BattleRuntime_WaitIfModeZero_3(10);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(11, 3);
    BattleRuntime_WaitIfModeZero_4(60);
    Object_SetModeById_2(8, 3);
    Object_SetModeById_3(12, 3);
    Object_SetModeById_4(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_2(10, 3);
    BattleRuntime_WaitIfModeZero_5(50);
    ObjectMotion_SetAngleToward_5(0, 8, 0);
    ObjectMotion_SetAngleToward_6(11, 8, 0);
    ObjectMotion_SetAngleToward_7(12, 8, 0);
    ObjectMotion_SetAngleToward_8(9, 8, 0);
    ObjectMotion_SetAngleToward_9(10, 8, 0);
    BattleRuntime_WaitIfModeZero_6(30);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 4);
    BattleRuntime_WaitIfModeZero_7(20);
    SceneWork_SetStepValue_1(0x1171);
    BattleEvent_RunActionAndWait_1(1, 0);
    BattleRuntime_WaitIfModeZero_8(20);
    ObjectMotion_SetAngleToward_10(12, 1, 0);
    ObjectMotion_SetVariantCallback_1(12, 2);
    BattleEffect_SpawnLinkedResourceObject_1(12, 0x103, 0);
    BattleRuntime_WaitIfModeZero_9(60);
    BattleEvent_RunActionAndWait_2(12, 0);
    BattleRuntime_WaitIfModeZero_10(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 2);
    BattleRuntime_WaitIfModeZero_11(20);
    ObjectMotion_SetAngleToward_11(1, 12, 0);
    BattleRuntime_WaitIfModeZero_12(20);
    ObjectMotion_CallThenWaitForAnimationChange_4(1, 3);
    BattleRuntime_WaitIfModeZero_13(40);
    ObjectMotion_SetAngleToward_12(1, 8, 0);
    ObjectMotion_SetAngleToward_13(12, 8, 0);
    BattleRuntime_WaitIfModeZero_14(20);
    ObjectMotion_CallThenWaitForAnimationChange_5(8, 3);
    BattleRuntime_WaitIfModeZero_15(20);
    ObjectMotion_ArmCallback_1(8, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_16(30);
    ObjectMotion_CallThenWaitForAnimationChange_6(8, 3);
    BattleRuntime_WaitIfModeZero_17(30);
    BattleEvent_RunActionAndWait_3(8, 0);
    BattleRuntime_WaitIfModeZero_18(80);
    Audio_PlayCue_1(17);
    BattleRuntime_WaitIfModeZero_19(0x10005, 1);
    BattleRuntime_WaitIfModeZero_20(60);
    BattleRuntime_WaitIfModeZero_21(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x6666, 0xccc);
    ObjectMotion_PlaceWithinCameraBounds_2(0xc00000, -1, 0x680000, 1);
    BattleRuntime_WaitIfModeZero_22(120);
    Audio_PlayCue_2(21);
    Audio_PlayCue_3(0x134);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(13, 0xc80000, 0x80000);
    ObjectMotion_SetSpeedParameters_8(13, 0x6666, 0x3333);
    ObjectMotion_SetPositionAndCommit_1(13, 200, 72);
    Audio_PlayCue_4(0x120);
    BattleRuntime_WaitIfModeZero_23(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    Object_SetModeById_5(8, 0);
    Func_020040c0();
    ObjectMotion_SetAngleToward_14(0, 13, 0);
    ObjectMotion_SetAngleToward_15(1, 13, 0);
    ObjectMotion_SetAngleToward_16(11, 13, 0);
    ObjectMotion_SetAngleToward_17(12, 13, 0);
    ObjectMotion_SetAngleToward_18(9, 13, 0);
    ObjectMotion_SetAngleToward_19(10, 13, 0);
    ObjectMotion_SetVariantCallback_2(0, 2);
    ObjectMotion_SetVariantCallback_3(1, 2);
    ObjectMotion_SetVariantCallback_4(11, 2);
    ObjectMotion_SetVariantCallback_5(12, 2);
    ObjectMotion_SetVariantCallback_6(9, 2);
    ObjectMotion_SetVariantCallback_7(10, 2);
    BattleRuntime_WaitIfModeZero_24(40);
    BattleEvent_RunActionAndWait_4(13, 0);
    BattleRuntime_WaitIfModeZero_25(20);
    BattleRuntime_WaitIfModeZero_26(40);
    BattleEvent_RunActionAndWait_5(13, 0);
    BattleRuntime_WaitIfModeZero_27(60);
    ObjectGroup_ConfigureChildValue_1(13, 0x100);
    Audio_PlayCue_5(17);
    Audio_PlayCue_6(0x134);
    /*
     * 32 repeats: step actor 13's animation, then subtract a fixed amount
     * from the record fields at +24 and +28.
     */
    for (i = 0; i < 32; i++) {
        Func_02004d8c(13);
        BattleRuntime_WaitIfModeZero_28(4);
        record = Scene_GetRecord_1(13);
        *(s32 *)(record + 24) += -0x28f;
        record = Scene_GetRecord_2(13);
        *(s32 *)(record + 28) += -0x28f;
    }
    Audio_PlayCue_7(0x120);
    ObjectGroup_ConfigureChildValue_2(13, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(13, 0, 0);
    BattleRuntime_WaitIfModeZero_29(40);
    ObjectMotion_PlaceWithinCameraBounds_3(0xc00000, -1, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    BattleRuntime_WaitIfModeZero_30(0x10000, 0);
    BattleRuntime_WaitIfModeZero_31(60);
    BattleRuntime_WaitIfModeZero_32(120);
    Func_020042a8();
    Object_SetModeById_6(8, 1);
    Audio_PlayCue_8(2);
    BattleRuntime_WaitIfModeZero_33(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(8, 2);
    BattleRuntime_WaitIfModeZero_34(20);
    ObjectMotion_SetVariantCallback_8(0, 1);
    ObjectMotion_SetVariantCallback_9(1, 1);
    ObjectMotion_SetVariantCallback_10(11, 1);
    ObjectMotion_SetVariantCallback_11(12, 1);
    ObjectMotion_SetVariantCallback_12(9, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(10, 1);
    BattleRuntime_WaitIfModeZero_35(30);
    BattleEvent_RunActionAndWait_6(11, 0);
    BattleRuntime_WaitIfModeZero_36(30);
    ObjectMotion_SetAngleToward_20(8, 0, 0);
    BattleRuntime_WaitIfModeZero_37(30);
    ObjectMotion_SetAngleToward_21(12, 8, 0);
    BattleRuntime_WaitIfModeZero_38(20);
    ObjectMotion_SetAngleToward_22(0, 8, 0);
    ObjectMotion_SetAngleToward_23(1, 8, 0);
    ObjectMotion_SetAngleToward_24(11, 8, 0);
    ObjectMotion_SetAngleToward_25(9, 8, 0);
    ObjectMotion_SetAngleToward_26(10, 8, 0);
    BattleRuntime_WaitIfModeZero_39(20);
    BattleEvent_RunActionAndWait_7(12, 0);
    BattleRuntime_WaitIfModeZero_40(20);
    ObjectMotion_SetAngleToward_27(8, 12, 0);
    BattleRuntime_WaitIfModeZero_41(20);
    ObjectMotion_CallThenWaitForAnimationChange_7(8, 3);
    BattleRuntime_WaitIfModeZero_42(20);
    Object_SetModeById_7(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(1, 3);
    BattleRuntime_WaitIfModeZero_43(20);
    ObjectMotion_SetAngleToward_28(8, 0, 0);
    BattleRuntime_WaitIfModeZero_44(20);
    ObjectMotion_CallThenWaitForAnimationChange_9(8, 3);
    BattleRuntime_WaitIfModeZero_45(30);
    BattleEvent_RunActionAndWait_8(8, 0);
    BattleRuntime_WaitIfModeZero_46(20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_4(11, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_5(12, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_6(9, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_7(10, 0x102, 0);
    BattleRuntime_WaitIfModeZero_47(30);
    BattleEffect_SpawnLinkedResourceObject_8(8, 0x102, 0);
    BattleRuntime_WaitIfModeZero_48(30);
    BattleEvent_RunActionAndWait_9(8, 0);
    BattleRuntime_WaitIfModeZero_49(20);
    Object_SetModeById_8(0, 3);
    Object_SetModeById_9(1, 3);
    Object_SetModeById_10(11, 3);
    Object_SetModeById_11(12, 3);
    Object_SetModeById_12(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_10(10, 3);
    BattleRuntime_WaitIfModeZero_50(20);
    ObjectMotion_CallThenWaitForAnimationChange_11(8, 3);
    BattleRuntime_WaitIfModeZero_51(180);
    BattleEffect_SpawnLinkedResourceObject_9(12, 0x105, 0);
    BattleRuntime_WaitIfModeZero_52(60);
    Object_LinkPair_2(0, 1, 0);
    Object_LinkPair_3(9, 10, 0);
    BattleRuntime_WaitIfModeZero_53(40);
    ObjectMotion_SetAngleToward_29(0, 8, 0);
    ObjectMotion_SetAngleToward_30(1, 8, 0);
    ObjectMotion_SetAngleToward_31(9, 8, 0);
    ObjectMotion_SetAngleToward_32(10, 8, 0);
    BattleRuntime_WaitIfModeZero_54(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(1, 2);
    BattleRuntime_WaitIfModeZero_55(20);
    BattleEvent_RunActionAndWait_10(1, 0);
    BattleRuntime_WaitIfModeZero_56(30);
    BattleEffect_SpawnLinkedResourceObject_10(8, 0x101, 0);
    BattleRuntime_WaitIfModeZero_57(60);
    ObjectMotion_SetVariantCallback_13(0, 1);
    ObjectMotion_SetVariantCallback_14(1, 1);
    ObjectMotion_SetVariantCallback_15(11, 1);
    ObjectMotion_SetVariantCallback_16(12, 1);
    ObjectMotion_SetVariantCallback_17(9, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(10, 1);
    BattleRuntime_WaitIfModeZero_58(30);
    BattleEvent_RunActionAndWait_11(11, 0);
    BattleRuntime_WaitIfModeZero_59(20);
    BattleEffect_SpawnLinkedResourceObject_11(8, 0x101, 0);
    BattleRuntime_WaitIfModeZero_60(80);
    BattleEffect_SpawnLinkedResourceObject_12(12, 0x102, 0);
    BattleRuntime_WaitIfModeZero_61(60);
    BattleEvent_RunActionAndWait_12(12, 0);
    BattleRuntime_WaitIfModeZero_62(30);
    BattleEffect_SpawnLinkedResourceObject_13(8, 0x105, 0);
    BattleRuntime_WaitIfModeZero_63(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(8, 1);
    BattleEffect_SpawnLinkedResourceObject_14(8, 0x106, 0);
    BattleRuntime_WaitIfModeZero_64(20);
    ObjectMotion_SetVariantCallback_18(0, 1);
    ObjectMotion_SetVariantCallback_19(1, 1);
    ObjectMotion_SetVariantCallback_20(11, 1);
    ObjectMotion_SetVariantCallback_21(12, 1);
    ObjectMotion_SetVariantCallback_22(9, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(10, 1);
    BattleRuntime_WaitIfModeZero_65(40);
    BattleEvent_RunActionAndWait_13(8, 0);
    BattleRuntime_WaitIfModeZero_66(20);
    Object_SetModeById_13(0, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(11, 3);
    Object_SetModeById_16(12, 3);
    Object_SetModeById_17(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_12(10, 3);
    BattleRuntime_WaitIfModeZero_67(30);
    BattleEvent_RunActionAndWait_14(8, 0);
    BattleRuntime_WaitIfModeZero_68(30);
    Object_SetModeById_18(0, 3);
    Object_SetModeById_19(1, 3);
    Object_SetModeById_20(11, 3);
    Object_SetModeById_21(12, 3);
    Object_SetModeById_22(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_13(10, 3);
    BattleRuntime_WaitIfModeZero_69(120);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(1, 1);
    BattleRuntime_WaitIfModeZero_70(20);
    BattleEvent_RunActionAndWait_15(1, 0);
    BattleRuntime_WaitIfModeZero_71(30);
    ObjectMotion_CallThenWaitForAnimationChange_14(8, 3);
    BattleRuntime_WaitIfModeZero_72(20);
    BattleEvent_RunActionAndWait_16(8, 0);
    BattleRuntime_WaitIfModeZero_73(20);
    BattleEffect_SpawnLinkedResourceObject_15(0, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_16(1, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_17(11, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_18(12, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_19(9, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_20(10, 0x105, 0);
    BattleRuntime_WaitIfModeZero_74(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_11(12, 1);
    BattleEvent_RunActionAndWait_17(12, 0);
    BattleRuntime_WaitIfModeZero_75(20);
    ObjectMotion_SetAngleToward_33(8, 12, 0);
    BattleRuntime_WaitIfModeZero_76(20);
    ObjectMotion_CallThenWaitForAnimationChange_15(8, 4);
    BattleRuntime_WaitIfModeZero_77(20);
    BattleEvent_RunActionAndWait_18(8, 0);
    BattleRuntime_WaitIfModeZero_78(20);
    BattleEffect_SpawnLinkedResourceObject_21(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_22(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_23(11, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_24(12, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_25(9, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_26(10, 0x100, 0);
    BattleRuntime_WaitIfModeZero_79(60);
    ObjectMotion_SetAngleToward_34(8, 0, 0);
    BattleRuntime_WaitIfModeZero_80(20);
    ObjectMotion_CallThenWaitForAnimationChange_16(8, 3);
    BattleEvent_RunActionAndWait_19(8, 0);
    BattleRuntime_WaitIfModeZero_81(40);
    ObjectMotion_SetPositionAndReset_1(8, 168, 176);
    ObjectMotion_SetAngleToward_35(0, 8, 0);
    ObjectMotion_SetAngleToward_36(1, 8, 0);
    ObjectMotion_SetAngleToward_37(11, 8, 0);
    ObjectMotion_SetAngleToward_38(12, 8, 0);
    ObjectMotion_SetAngleToward_39(9, 8, 0);
    ObjectMotion_SetAngleToward_40(10, 8, 0);
    ObjectMotion_SetPositionAndReset_2(8, 200, 200);
    ObjectMotion_SetAngleToward_41(0, 8, 0);
    ObjectMotion_SetAngleToward_42(1, 8, 0);
    ObjectMotion_SetAngleToward_43(12, 8, 0);
    ObjectMotion_ArmCallback_2(11, 0, 0);
    ObjectMotion_ArmCallback_3(9, 0x8000, 0);
    ObjectMotion_ArmCallback_4(10, 0x8000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_1(8, 200, 0x110);
    BattleRuntime_WaitIfModeZero_82(40);
    ObjectMotion_ArmCallback_5(11, 0x3000, 0);
    ObjectMotion_ArmCallback_6(9, 0x5000, 0);
    ObjectMotion_ArmCallback_7(10, 0x5000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(8, 0, 0);
    BattleRuntime_WaitIfModeZero_83(60);
    BattleEffect_SpawnLinkedResourceObject_27(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_28(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_29(11, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_30(12, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_31(9, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_32(10, 0x102, 0);
    BattleRuntime_WaitIfModeZero_84(60);
    BattleRuntime_WaitIfModeZero_85(120);
    ObjectMotion_SetAngleToward_44(0, 9, 0);
    ObjectMotion_SetAngleToward_45(1, 10, 0);
    ObjectMotion_SetAngleToward_46(11, 9, 0);
    ObjectMotion_SetAngleToward_47(12, 9, 0);
    BattleRuntime_WaitIfModeZero_86(120);
    BattleEffect_SpawnLinkedResourceObject_33(9, 0x105, 0);
    BattleRuntime_WaitIfModeZero_87(60);
    ObjectMotion_SetAngleToward_48(9, 0, 0);
    BattleRuntime_WaitIfModeZero_88(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_12(9, 2);
    BattleRuntime_WaitIfModeZero_89(10);
    ObjectMotion_ArmCallback_8(9, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_90(20);
    BattleEffect_SpawnLinkedResourceObject_34(9, 0x102, 0);
    BattleRuntime_WaitIfModeZero_91(80);
    ObjectMotion_SetAngleToward_49(9, 10, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_13(9, 1);
    BattleRuntime_WaitIfModeZero_92(20);
    BattleEvent_RunActionAndWait_20(9, 0);
    BattleRuntime_WaitIfModeZero_93(20);
    ObjectMotion_ArmCallback_9(10, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_94(20);
    BattleEffect_SpawnLinkedResourceObject_35(10, 0x101, 0);
    BattleRuntime_WaitIfModeZero_95(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_14(9, 2);
    BattleRuntime_WaitIfModeZero_96(20);
    ObjectMotion_SetAngleToward_50(10, 0, 0);
    BattleRuntime_WaitIfModeZero_97(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_15(10, 2);
    BattleRuntime_WaitIfModeZero_98(30);
    ObjectMotion_ArmCallback_10(10, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_99(20);
    BattleEvent_RunActionAndWait_21(10, 0);
    BattleRuntime_WaitIfModeZero_100(20);
    Object_SetModeById_23(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_17(10, 3);
    BattleRuntime_WaitIfModeZero_101(20);
    ObjectMotion_ResetAndSetPositionInMode2_2(9, 200, 0x110);
    ObjectMotion_SetPositionAndReset_3(10, 200, 0x110);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(9, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(10, 0, 0);
    BattleEffect_SpawnLinkedResourceObject_36(12, 0x105, 0);
    BattleRuntime_WaitIfModeZero_102(60);
    ObjectMotion_SetPositionAndReset_4(12, 200, 136);
    Object_LinkPair_4(0, 12, 0);
    ObjectMotion_SetAngleToward_51(1, 12, 0);
    ObjectMotion_SetAngleToward_52(11, 12, 0);
    BattleRuntime_WaitIfModeZero_103(30);
    ObjectMotion_CallThenWaitForAnimationChange_18(12, 4);
    BattleRuntime_WaitIfModeZero_104(20);
    BattleEvent_RunActionAndWait_22(12, 0);
    BattleRuntime_WaitIfModeZero_105(30);
    Object_SetModeById_24(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_19(1, 3);
    BattleRuntime_WaitIfModeZero_106(20);
    BattleEvent_RunActionAndWait_23(11, 0);
    BattleRuntime_WaitIfModeZero_107(20);
    ObjectMotion_SetPositionAndReset_5(11, 168, 168);
    ObjectMotion_SetAngleToward_53(11, 12, 0);
    BattleRuntime_WaitIfModeZero_108(20);
    ObjectMotion_SetAngleToward_54(0, 11, 0);
    ObjectMotion_SetAngleToward_55(1, 11, 0);
    BattleRuntime_WaitIfModeZero_109(20);
    ObjectMotion_CallThenWaitForAnimationChange_20(11, 4);
    BattleRuntime_WaitIfModeZero_110(20);
    BattleEvent_RunActionAndWait_24(11, 0);
    BattleRuntime_WaitIfModeZero_111(20);
    BattleEffect_SpawnLinkedResourceObject_37(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_38(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_112(60);
    ObjectMotion_CallThenWaitForAnimationChange_21(12, 4);
    BattleRuntime_WaitIfModeZero_113(20);
    ObjectMotion_SetAngleToward_56(0, 12, 0);
    ObjectMotion_SetAngleToward_57(1, 12, 0);
    BattleRuntime_WaitIfModeZero_114(20);
    BattleEvent_RunActionAndWait_25(12, 0);
    BattleRuntime_WaitIfModeZero_115(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_16(11, 2);
    BattleRuntime_WaitIfModeZero_116(20);
    BattleEvent_RunActionAndWait_26(11, 0);
    BattleRuntime_WaitIfModeZero_117(20);
    BattleEffect_SpawnLinkedResourceObject_39(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_40(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_118(60);
    BattleEffect_SpawnLinkedResourceObject_41(12, 0x102, 0);
    BattleRuntime_WaitIfModeZero_119(60);
    BattleEvent_RunActionAndWait_27(12, 0);
    BattleRuntime_WaitIfModeZero_120(20);
    ObjectMotion_SetVariantCallback_23(0, 2);
    ObjectMotion_SetVariantCallback_24(1, 2);
    BattleEffect_SpawnLinkedResourceObject_42(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_43(1, 0x100, 0);
    BattleRuntime_WaitIfModeZero_121(60);
    ObjectMotion_CallThenWaitForAnimationChange_22(11, 3);
    BattleRuntime_WaitIfModeZero_122(20);
    BattleEvent_RunActionAndWait_28(11, 0);
    BattleRuntime_WaitIfModeZero_123(20);
    ObjectMotion_SetAngleToward_58(0, 11, 0);
    BattleRuntime_WaitIfModeZero_124(30);
    Object_SetModeById_25(11, 3);
    BattleRuntime_WaitIfModeZero_125(30);
    Object_SetModeById_26(12, 3);
}

void SceneState_ResetObject14Word108(void)
{
    u8 *state = Func_02005c76(14);
    *(s32 *)(state + 108) = 0;
    Func_02005ccc(14, 0, 0);
}

void SceneEffect_StepEllipseOrbit(u8 *obj)
{
    u8 *anchor = *(u8 **)(obj + 104);
    u16 *angle = (u16 *)(obj + 100);
    u16 theta = *angle;
    s32 x;
    s32 z;
    s32 tmp;

    x = *(s32 *)(anchor + 8) + Func_02005c52(theta)* 14;
    *(s32 *)(obj + 8) = x;

    z = *(s32 *)(anchor + 16) + Func_02005c5e(theta)* 10;
    tmp = *(s32 *)(obj + 8);
    *(s32 *)(obj + 16) = z;
    *(s32 *)(obj + 64) = z;
    *(s32 *)(obj + 56) = tmp;

    *angle = (u16)(*angle + *(u16 *)(obj + 102));
}

void FieldScene_RunPairedActorChoreography(void)
{
    u8 *Func_02005050();
    s32 Func_02005f9e();

    u32 i;
    u8 *record;
    s32 none;

    ObjectMotion_SetSpeedParameters_1_0200290c(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_2_0200290c(1, 0x18000, 0xc000);
    Call2_scene_primary_script((void (*)())Func_02005fbc, 12, 2); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_1_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_1_0200290c(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_2_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_2_0200290c(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_3_0200290c(15);
    ObjectMotion_SetAngleToward_1_0200290c(0, 1, 0);
    ObjectMotion_SetVariantCallback_5_0200290c(0, 1); /* object 0, variant 1 */
    {
        u8 *record = Scene_GetRecord_1_0200290c(0); /* main:0808a080 */
        u8 value = *(volatile u8 *)&record[90];
        s32 masked = value & 254;

        none = 0;
        record[90] = (u8)masked;
    }
    ObjectMotion_ResetAndSetPositionInMode2_1_0200290c(0, 184, 168);
    *(u8 *)(Scene_GetRecord_2_0200290c(1) + 90) &= 254;
    ObjectMotion_SetPositionAndReset_10(1, 200, 168); /* object 1, x 200, z 168 */
    BattleRuntime_WaitIfModeZero_4_0200290c(1);
    *(u8 *)(Scene_GetRecord_3(1) + 90) |= 1;
    ObjectMotion_CommitCurrentPositionAndActivate_1_0200290c(0);
    Object_SetModeById_1_0200290c(0, 1);
    *(u8 *)(Scene_GetRecord_4(0) + 90) |= 1;
    {
        u8 *record = Scene_GetRecord_5(1);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    ObjectMotion_Launch_1(1, 2, 0);
    BattleRuntime_WaitIfModeZero_5_0200290c(15);
    ObjectMotion_SetAngleToward_2_0200290c(1, 8, 0);
    Call1((void (*)())Func_02005fe8_c, 5); /* main:0808a080 */
    ObjectMotion_Launch_2(1, 2, 0);
    BattleRuntime_WaitIfModeZero_6_0200290c(25);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_0200290c(1, 2);
    ObjectMotion_SetAngleToward_3_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_7_0200290c(5);
    ObjectMotion_CallThenWaitForAnimationChange_3_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_8_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_4_0200290c(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_9_0200290c(5);
    ObjectMotion_CallThenWaitForAnimationChange_5_0200290c(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_10_0200290c(10);
    ObjectMotion_SetAngleToward_4_0200290c(1, 0, 0);
    BattleRuntime_WaitIfModeZero_11_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_6_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_12_0200290c(15);
    Object_SetModeById_2_0200290c(11, 3);
    Object_SetModeById_3_0200290c(12, 3);
    Object_SetModeById_4_0200290c(8, 3);
    Object_SetModeById_5_0200290c(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7_0200290c(10, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_13_0200290c(20);
    ObjectMotion_SetAngleToward_5_0200290c(0, 12, 0);
    ObjectMotion_SetAngleToward_6_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_14_0200290c(10); /* main:0808a138 */
    Object_SetModeById_6_0200290c(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_15_0200290c(20);
    ObjectMotion_SetAngleToward_7_0200290c(0, 11, 0);
    ObjectMotion_SetAngleToward_8_0200290c(1, 11, 0);
    BattleRuntime_WaitIfModeZero_16_0200290c(10);
    Object_SetModeById_7_0200290c(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_17_0200290c(20);
    ObjectMotion_ArmCallback_1_0200290c(0, 0, 0);
    BattleRuntime_WaitIfModeZero_18_0200290c(15);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_0200290c(0, 2); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_19_0200290c(10);
    Value4(Func_02005bc6, 222, 0xb80000, 0x1b0000, 0xa80000);
    ObjectMotion_SetAngleToward_9_0200290c(1, 0, 0);
    BattleRuntime_WaitIfModeZero_20_0200290c(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3_0200290c(1, 1);
    BattleRuntime_WaitIfModeZero_21_0200290c(10);
    ObjectMotion_Launch_3(1, 4, 0); /* main:0808a138 */
    ObjectMotion_ArmCallback_2_0200290c(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_22_0200290c(15);
    ObjectMotion_ArmCallback_3_0200290c(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_23_0200290c(10);
    ObjectMotion_ArmCallback_4_0200290c(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_24_0200290c(10);
    ObjectMotion_ArmCallback_5_0200290c(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_25_0200290c(10);
    ObjectMotion_ArmCallback_6_0200290c(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_26_0200290c(30);
    ObjectMotion_Launch_4(1, 4, 0);
    ObjectMotion_ArmCallback_7_0200290c(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_27_0200290c(15);
    ObjectMotion_ArmCallback_8_0200290c(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_28_0200290c(10);
    ObjectMotion_ArmCallback_9_0200290c(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_29_0200290c(10);
    ObjectMotion_ArmCallback_10_0200290c(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_30_0200290c(10);
    ObjectMotion_ArmCallback_11(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_31_0200290c(30);
    ObjectMotion_ArmCallback_12(0, 0xc000, 0);
    ObjectMotion_ArmCallback_13(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_32_0200290c(10);
    BattleEffect_SpawnLinkedResourceObject_1_0200290c(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_2_0200290c(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_33_0200290c(60);
    ObjectMotion_SetAngleToward_10_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_34_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_10_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_35_0200290c(10);
    {
        u8 *record = Scene_GetRecord_6(8); /* main:0808a080 */
        s32 shown = 1;

        *(volatile u16 *)(record + 100) = shown;
    }
    record = Scene_GetRecord_7(8); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = 0x2008031;
    {
        u8 *record = Scene_GetRecord_8(12); /* main:0808a080 */
        s32 shown = 1;

        *(volatile u16 *)(record + 100) = shown;
    }
    record = Scene_GetRecord_9(12); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = 0x2008031;
    ObjectMotion_SetPositionAndReset_1_0200290c(1, 196, 180);
    ObjectMotion_SetPositionAndReset_2_0200290c(1, 184, 184);
    ObjectMotion_SetPositionAndReset_3_0200290c(1, 180, 180);
    ObjectMotion_SetPositionAndReset_4_0200290c(1, 168, 168);
    ObjectMotion_SetPositionAndReset_5_0200290c(1, 180, 156);
    ObjectMotion_ResetAndSetPositionInMode2_2_0200290c(1, 200, 104);
    ObjectMotion_SetPositionAndReset_6(0, 192, 168);
    ObjectMotion_ArmCallback_14(0, 0xc000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    BattleRuntime_WaitIfModeZero_36_0200290c(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4_0200290c(1, 1); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_37_0200290c(10);
    ObjectMotion_ArmCallback_15(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_38_0200290c(15);
    record = Scene_GetRecord_10(12);
    *(volatile s32 *)(record + 108) = none;
    record = Scene_GetRecord_11(8); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = none;
    ObjectMotion_SetVariantCallback_1_0200290c(8, 2);
    BattleEffect_SpawnLinkedResourceObject_3_0200290c(8, 0x100, 0);
    Call1((void (*)())Func_0200632a_a, 60); /* main:0808a080 */
    Object_SetModeById_8_0200290c(8, 0);
    BattleEffect_SpawnLinkedResourceObject_4_0200290c(0, 0x102, 0);
    BattleRuntime_WaitIfModeZero_39_0200290c(60);
    Object_LinkPair_1_0200290c(0, 11, 0);
    BattleRuntime_WaitIfModeZero_40_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_11_0200290c(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_41_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_12_0200290c(11, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_42_0200290c(10);
    ObjectMotion_ArmCallback_16(0, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_43_0200290c(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5_0200290c(0, 2);
    BattleRuntime_WaitIfModeZero_44_0200290c(10);
    ObjectMotion_Launch_5(0, 2, 0); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_45_0200290c(20);
    ObjectMotion_Launch_6(0, 2, 0);
    BattleRuntime_WaitIfModeZero_46_0200290c(20);
    BattleRuntime_WaitIfModeZero_47_0200290c(15);
    Object_LinkPair_2_0200290c(0, 12, 0);
    BattleRuntime_WaitIfModeZero_48_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_13_0200290c(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_49_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_14_0200290c(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_50_0200290c(60);
    ObjectMotion_SetPositionAndReset_7(1, 208, 168);
    ObjectMotion_SetAngleToward_11_0200290c(0, 11, 0);
    ObjectMotion_SetAngleToward_12_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_51_0200290c(10);
    Object_SetModeById_9_0200290c(0, 4);
    ObjectMotion_CallThenWaitForAnimationChange_15_0200290c(1, 4); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_52_0200290c(10);
    ObjectMotion_SetVariantCallback_2_0200290c(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6_0200290c(1, 1); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_53_0200290c(10);
    ObjectMotion_ArmCallback_17(1, 0, 0);
    BattleRuntime_WaitIfModeZero_54_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_16_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_55_0200290c(10);
    ObjectMotion_SetAngleToward_13_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_56_0200290c(10);
    ObjectMotion_CallThenWaitForAnimationChange_17_0200290c(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_57_0200290c(10);
    ObjectMotion_ArmCallback_18(0, 0, 0);
    ObjectMotion_ArmCallback_19(1, 0, 0);
    BattleRuntime_WaitIfModeZero_58_0200290c(10);
    ObjectMotion_SetVariantCallback_3_0200290c(0, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7_0200290c(1, 2);
    BattleRuntime_WaitIfModeZero_59_0200290c(10);
    ObjectMotion_EnableActionAndSetCallback_1(0, 0x200b740);
    ObjectMotion_EnableActionAndSetCallback_2(1, 0x200b81c);
    Object_LookupAndStep_1(0); /* main:0808a0a0 */
    Object_LookupAndStep_2(1); /* main:0808a0a0 */
    ObjectMotion_SetSpeedParameters_3_0200290c(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_4_0200290c(1, 0x18000, 0xc000);
    ObjectMotion_Launch_7(0, 6, 0);
    ObjectMotion_Launch_8(1, 6, 0);
    ObjectMotion_SetAngleToward_14_0200290c(0, 9, 0);
    ObjectMotion_SetAngleToward_15_0200290c(1, 8, 0);
    BattleRuntime_WaitIfModeZero_60_0200290c(1);
    ObjectMotion_SetAngleToward_16_0200290c(0, 12, 0);
    ObjectMotion_SetAngleToward_17_0200290c(1, 11, 0);
    BattleRuntime_WaitIfModeZero_61_0200290c(1);
    ObjectMotion_SetAngleToward_18_0200290c(0, 8, 0);
    ObjectMotion_SetAngleToward_19_0200290c(1, 9, 0);
    BattleRuntime_WaitIfModeZero_62_0200290c(1); /* main:0808a138 */
    ObjectMotion_ResetAndSetPositionInMode2_3(0, 192, 168);
    ObjectMotion_SetPositionAndReset_8(1, 208, 168);
    ObjectMotion_CommitCurrentPositionAndActivate_3(0);
    BattleRuntime_WaitIfModeZero_63_0200290c(10);
    ObjectMotion_ArmCallback_20(0, 0x3000, 0);
    ObjectMotion_ArmCallback_21(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_64_0200290c(10);
    ObjectMotion_ArmCallback_22(0, 0x5000, 0);
    ObjectMotion_ArmCallback_23(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_65_0200290c(10);
    ObjectMotion_ArmCallback_24(0, 0x3000, 0);
    ObjectMotion_ArmCallback_25(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_66_0200290c(10);
    ObjectMotion_ArmCallback_26(0, 0x5000, 0);
    ObjectMotion_ArmCallback_27(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_67_0200290c(20);
    ObjectMotion_SetAngleToward_20_0200290c(0, 11, 0);
    ObjectMotion_SetAngleToward_21_0200290c(1, 12, 0);
    BattleRuntime_WaitIfModeZero_68_0200290c(20);
    Object_SetModeById_10_0200290c(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_18_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_69_0200290c(10);
    Object_SetModeById_11_0200290c(11, 3);
    ObjectMotion_CallThenWaitForAnimationChange_19_0200290c(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_70_0200290c(30);
    Object_LinkPair_3_0200290c(0, 1, 0);
    BattleRuntime_WaitIfModeZero_71_0200290c(20);
    Object_SetModeById_12_0200290c(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_20_0200290c(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_72_0200290c(10);
    ObjectMotion_ArmCallback_28(1, 0, 0);
    BattleRuntime_WaitIfModeZero_73_0200290c(10);
    Object_SetModeById_13_0200290c(0, 2);
    Object_SetModeById_14_0200290c(1, 2);
    BattleRuntime_WaitIfModeZero_74_0200290c(60);
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Func_020067fe(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Func_0200681a(11, 10, 3, 2, a5, a6);
    }
    Func_020067c8(1);
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
        Func_02006a06(8);
        return;
    }

    Func_020068d4();
    if (Func_020068ba(0x87a) != 0)
        Func_0200699c(0x1bfc);
    else if (Func_020068cc(0x815) != 0)
        Func_020069ae(0x119d);
    else
        Func_020069b6(0x1035);
    Func_020069ce(8, 0);
    Func_02006912();
}

void FieldScene_DispatchBySceneId(void)
{
    s16 *tbl;
    s32 no;

    if (Func_020065e0() != 0) {
        Func_02006a6a(8);
        return;
    }

    Func_02006938();

    tbl = Data_02000240;
    no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        if (Func_020069dc(0x855) != 0) {
            Func_02006abe(0x1376);
        } else {
            Func_02006ac6(0x1288);
        }
        break;
    case 11:
        Func_02006ace(0x1ce8);
        break;
    case 20:
    case 21:
    case 50:
        Func_02006a24();
        Func_02003790();
        return;
    default:
        break;
    }

    Func_02006af2(8, 0);
    Func_02006a36();
}

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (Func_02006708() != 0) {
        Func_02006b92(8);
        return;
    }

    Func_02006a60();
    if (Func_02006a46(0x845) != 0)
        Func_02006b28(0x171c);
    else
        Func_02006b30(0x1408);
    Func_02006b48(8, 0);
    Func_02006a8c();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (Func_02006754() != 0) {
        Func_02006bde(8);
        return;
    }

    Func_02006aac();
    Func_02006b6a(0x190a);
    if (Func_02006a98(0x909) != 0)
        Func_02006b7a(0x1951);
    Func_02006b92_a(8, 0);
    Func_02006ad6();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (Func_0200679c() != 0) {
        Func_02006c26(8);
        return;
    }

    Func_02006af4();
    Func_02006bb2(0x1823);
    Func_02006bca(8, 0);
    Func_02006b0e();
}
