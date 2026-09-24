#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum PoseMessage {
    MSG_HOW_DID_SEARCH_FOR_SHEBA = 0x2850,
    MSG_DO_NOT_WORRY_ABOUT_SHEBA = 0x2861,
    MSG_PLEASE_WAIT_FOR_ME_OUTSIDE = 0x288b
};


#define SceneActor_SetActor14Pose258 Func_02000030
#define SceneData_GetTable9438 Func_02000044
#define SceneData_SelectTableByWord224 Func_0200004c
#define SceneData_GetTable96d0 Func_0200007c
#define SceneData_SelectTableBySceneIdAndFlag9a7 Func_02000084
#define FieldScene_RunSequenceA Func_020003f4
#define FieldScene_RunThreeCallSequence Func_02000468
#define SceneState_ForwardWord16cAndApply7b Func_0200048c
#define SceneData_SelectScriptBySceneIdAndFlag9a7 Func_020004b0
#define FieldScene_RunPrimaryScript Func_02000508
#define FieldScene_RunSecondaryScript Func_02000eec

extern u8 Value_000000b3;
extern u8 Data_02009690[];
extern u8 Data_020096b0[];
extern u8 Value_000000b4;
extern u8 Data_02009734[];
extern u8 Data_02009974[];
extern u8 Data_0200989c[];
extern u8 *Data_03001ebc;
extern u8 Data_0200a010[];
extern u8 Data_02009eb4[];
extern u8 Data_02009ca4[];
extern u8 Data_02009a94[];
extern u8 Value_00002880;

void Func_020016d6(void);
void Func_02001590();
void Func_020016fe();
void Func_0200186c();
s32 Func_0200187a();
void Func_02001888();

/*
 * The import's main-image target is Func_0808a1f0; its second argument is a
 * pose id.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x02009438.
 */

/*
 * The eight-byte owner at 0x0200007c includes its one pool word, which holds
 * the returned table address 0x020096d0.
 */

/*
 * Select the scene script at 0x020004b0.  The owner runs to 0x020004e6 and
 * also owns the alignment halfword at 0x020004ea and the literal pool at
 * 0x020004ec-0x02000507.  The 0x9a7 predicate is evaluated on both sides of
 * the scene-id test, so the two calls stay separate -- they are two sites on
 * disjoint paths.  Index 224 of the scene-state block at 0x02000240 is the
 * signed scene id.
 */

/* Four scripts embedded in this overlay's own data. */

/*
 * Import veneer; used in a condition, so the return type is stated and the
 * arity left open.
 */

/*
 * The calls appear in machine order; that order is what reproduces the
 * reference.
 */

/*
 * Loader-relocated ROM calls: each site names the pre-relocation call word the
 * image holds, not a runtime address.
 */

/*
 * Import veneers, declared old-style: one import name takes different argument
 * counts at different sites in this overlay, so no arity is asserted and none
 * of these results is used.
 */
static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    Actor_SetAttachedEffect(actor, pose);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Event_ShowMessage();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessage();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Event_ShowMessage();

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Event_ShowMessage();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Event_ShowMessage();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Event_ShowMessage();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Event_ShowMessage();

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Event_ShowMessage();

    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Event_ShowMessage();

    return f(a0, a1, a2, a3);
}

s32 SceneActor_SetActor14Pose258(void)
{
    ConfigureActorPose(14, 258);
    return 0;
}

u8 *SceneData_GetTable9438(void)
{
    return (u8 *)0x02009438;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_000000b3) {
        return (s32)Data_02009690;
    }
    return (s32)Data_020096b0;
}

u8 *SceneData_GetTable96d0(void)
{
    return (u8 *)0x020096d0;
}

u8 *SceneData_SelectTableBySceneIdAndFlag9a7(void)
{
    if (gGameState.scene == (s32)&Value_000000b4) {
        if (GameFlag_IsSet(0x9A7) != 0) {
            return Data_02009974;
        }
        return Data_0200989c;
    }
    return Data_02009734;
}

void FieldScene_RunSequenceA(void)
{

    GameFlag_Set(0x9BC);
    Event_Begin();
    Func_020016d6();
    Event_Wait(0xA);
    Camera_MoveTo(0x780000, -1, 0x600000, 1);
    Camera_WaitForMove();
    Event_Wait(0x1E);
    Event_SetMessage(MSG_PLEASE_WAIT_FOR_ME_OUTSIDE);
    Event_ShowMessage(0xC, 0);
    Event_Wait(0xA);
    Actor_FaceActor(0, 0xC, 0);
    Event_Wait(0x1E);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(0x1E);
    Event_End();
}

void FieldScene_RunThreeCallSequence(void)
{
    void Event_ShowMessage(s32, s32);

    GameFlag_Set(0x9BC);
    Event_SetMessage(MSG_PLEASE_WAIT_FOR_ME_OUTSIDE);
    Event_ShowMessage(0xC, 0);
}

void SceneState_ForwardWord16cAndApply7b(void)
{
    u8 *work = Data_03001ebc;
    s16 *p = (s16 *)(work + 0x16C);

    Event_RequestExit(*p);
    Audio_PlayCue(0x7B);
}

u8 *SceneData_SelectScriptBySceneIdAndFlag9a7(void)
{
    s32 GameFlag_IsSet();

    if (gGameState.scene == (s32)&Value_000000b4) {
        if (GameFlag_IsSet(0x9a7) != 0) {
            return Data_0200a010;
        }
        return Data_02009eb4;
    }
    if (GameFlag_IsSet(0x9a7) != 0) {
        return Data_02009ca4;
    }
    return Data_02009a94;
}

void FieldScene_RunPrimaryScript(void)
{
    void Event_Begin();

    Value0(Engine_EventBegin);
    Value1(Func_020016fe, 1);
    Actor_SetPosition(0, 6291456, 12058624);
    Actor_FaceDirection(0, 49152, 0);
    Actor_FaceDirection(11, 0, 0);
    Value3(Engine_ActorFaceDirection, 12, 32768, 0);
    Event_SetMessage(MSG_HOW_DID_SEARCH_FOR_SHEBA);
    Value0(Engine_EventOpenScreen);
    Value0(Engine_EventWaitForScreen);
    Value1(Engine_EventWait, 10);
    Actor_SetSpeed(0, 65536, 32768);
    Actor_WalkByAndWait(0, 0, -16);
    Value3(Engine_ActorWalkToAndWait, 0, 104, 136);
    Value1(Engine_EventWait, 10);
    Call4(Func_0200186c, 1, -16, 16, 49152);
    Call4(Func_0200187a, 3, 0, 24, 49152);
    Value4(Func_02001888, 2, 16, 16, 49152);
    Value1(Engine_ActorWaitForMove, 1);
    Value1(Engine_EventWait, 20);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 2, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 2, 0);
    Value1(Engine_EventWait, 10);
    Actor_FaceDirection(11, 16384, 0);
    Value3(Engine_ActorFaceDirection, 12, 16384, 0);
    Value1(Engine_EventWait, 30);
    Actor_ShowEmote(11, 261, 70);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_ActorRunRepeatedMotion, 11, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 20);
    Actor_ShowEmote(12, 258, 40);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 12, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 20);
    Event_Wait(10);
    Actor_ShowEmote(11, 261, 50);
    Value2(Engine_EventShowMessage, 11, 0);
    if (GameFlag_IsSet(2495) == 0) {
        Value0(Func_02001590);
    } else {
    Event_Wait(20);
    Value2(Engine_ActorRunRepeatedMotion, 11, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_FaceActor(1, 0, 50);
    Actor_ShowEmote(0, 261, 60);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 1, 49152, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 1, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 3, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 3, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 2, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 2, 0);
    Value1(Engine_EventWait, 20);
    Actor_ShowEmote(12, 257, 40);
    Event_OpenMessage(12, 0);
    Event_ChooseYesNo(0, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_ActorRunRepeatedMotion, 12, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_ActorSetAnimationAndWait, 0, 3);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 1, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 1, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 3, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 3, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 2, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 2, 0);
    Value1(Engine_EventWait, 10);
    Actor_FaceActor(2, 0, 30);
    Event_OpenMessage(8194, 0);
    }
    Event_SetMessage(MSG_DO_NOT_WORRY_ABOUT_SHEBA);
    Value1(Engine_EventWait, 10);
    Actor_FaceDirection(0, 16384, 0);
    Actor_FaceActor(1, 0, 20);
    if (Event_ChooseYesNo(0, 0) == 0) {
    Value1(Engine_EventWait, 20);
    Actor_FaceDirection(0, 49152, 0);
    Actor_FaceDirection(1, 49152, 0);
    Value3(Engine_ActorFaceDirection, 2, 49152, 0);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    } else {
    Event_Wait(20);
    Actor_FaceDirection(0, 49152, 0);
    Actor_FaceDirection(1, 49152, 0);
    Value3(Engine_ActorFaceDirection, 2, 49152, 0);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 4);
    Value1(Engine_EventWait, 20);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    Event_ShowMessage(11, 0);
    }
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 11, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_FaceEachOther(1, 0, 0);
    Value3(Engine_ActorFaceEachOther, 3, 2, 0);
    Value1(Engine_EventWait, 40);
    Value1(Engine_EventWait, 10);
    Actor_ShowEmote(11, 258, 40);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_FaceDirection(0, 49152, 0);
    Actor_FaceDirection(1, 49152, 0);
    Actor_FaceDirection(2, 49152, 0);
    Value3(Engine_ActorFaceDirection, 3, 49152, 0);
    Value1(Engine_EventWait, 50);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 11, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_ShowEmote(3, 256, 40);
    Value2(Engine_EventShowMessage, 3, 0);
    Value1(Engine_EventWait, 20);
    Value3(Engine_ActorFaceDirection, 11, 0, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 12, 2);
    Value1(Engine_EventWait, 20);
    Value3(Engine_ActorFaceDirection, 12, 32768, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 2, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 2, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 11, 16384, 0);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_ShowEmote(12, 258, 50);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 20);
    Actor_ShowEmote(11, 257, 65);
    Value3(Engine_ActorFaceDirection, 11, 0, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 20);
    Actor_ShowEmote(12, 256, 40);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 20);
    Value3(Engine_ActorFaceDirection, 12, 16384, 0);
    Event_Wait(40);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 11, 16384, 0);
    Value1(Engine_EventWait, 50);
    Actor_ShowEmote(11, 262, 60);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Value2(Engine_ActorRunRepeatedMotion, 2, 2);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 11, 0, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 12, 2);
    Value1(Engine_EventWait, 30);
    Value3(Engine_ActorFaceDirection, 12, 32768, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 10);
    Actor_ShowEmote(11, 257, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 1, 57344, 0);
    Value1(Engine_EventWait, 30);
    Value2(Engine_ActorSetAnimationAndWait, 1, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 1, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 12, 16384, 0);
    Value1(Engine_EventWait, 30);
    Actor_ShowEmote(12, 261, 60);
    Value2(Engine_EventShowMessage, 12, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 1, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 1, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 11, 16384, 0);
    Value1(Engine_EventWait, 40);
    Actor_ShowEmote(11, 258, 40);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value3(Engine_ActorFaceDirection, 2, 32768, 0);
    Value1(Engine_EventWait, 30);
    Actor_ShowEmote(2, 263, 60);
    Value3(Engine_ActorFaceDirection, 2, 49152, 0);
    Value1(Engine_EventWait, 30);
    Value2(Engine_ActorSetAnimationAndWait, 2, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 2, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 4);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Actor_StartRepeatedMotion(0, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Value2(Engine_ActorRunRepeatedMotion, 2, 2);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Value2(Engine_ActorSetAnimationAndWait, 2, 3);
    Value1(Engine_EventWait, 30);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 11, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorRunRepeatedMotion, 1, 2);
    Value1(Engine_EventWait, 20);
    Value2(Engine_EventShowMessage, 1, 0);
    Value1(Engine_EventWait, 10);
    Value2(Engine_ActorSetAnimationAndWait, 11, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 11, 0);
    Value1(Engine_EventWait, 20);
    Value2(Engine_ActorSetAnimationAndWait, 3, 3);
    Value1(Engine_EventWait, 30);
    Value2(Engine_EventShowMessage, 3, 0);
    Value1(Engine_EventWait, 20);
    Actor_FaceDirection(0, 16384, 0);
    Actor_FaceDirection(1, 57344, 0);
    Value3(Engine_ActorFaceDirection, 2, 40960, 0);
    Value1(Engine_EventWait, 30);
    Value2(Engine_ActorSetAnimationAndWait, 0, 3);
    Value1(Engine_EventWait, 10);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Value2(Engine_ActorSetAnimationAndWait, 3, 3);
    Value1(Engine_EventWait, 30);
    Actor_SetSpeed(1, 78643, 39321);
    Actor_SetSpeed(3, 78643, 39321);
    Actor_SetSpeed(2, 78643, 39321);
    Value2(Engine_ActorSetAnimation, 1, 2);
    {
        u8 *rec = Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Value3(Engine_ActorSetDestination, 1, y, x);
        }
    }
    Value1(Engine_ActorWaitForMove, 1);
    Actor_SetPosition(1, 0, 0);
    Value2(Engine_ActorSetAnimation, 3, 2);
    {
        u8 *rec = Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Value3(Engine_ActorSetDestination, 3, y, x);
        }
    }
    Value1(Engine_ActorWaitForMove, 3);
    Actor_SetPosition(3, 0, 0);
    Value2(Engine_ActorSetAnimation, 2, 2);
    {
        u8 *rec = Value1(Engine_ActorGet, 0);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Value3(Engine_ActorSetDestination, 2, y, x);
        }
    }
    Value1(Engine_ActorWaitForMove, 2);
    Value3(Engine_ActorSetPosition, 2, 0, 0);
    Value1(Engine_EventWait, 10);
    Value0(Engine_EventEnd);
}

/*
 * Cutscene script at 0x02000eec.  The owner runs to 0x0200103a and also owns
 * the alignment halfword at 0x0200103e and the literal pool at
 * 0x02001040-0x0200104b; the body is straight-line, with no branch.  The
 * script is a sequence of "act on channel N, then wait k frames" beats.  The
 * channel ids and beat constants are transcribed literally: what each channel
 * drives is not established, and the middle argument 0x105 is unidentified.
 */
void FieldScene_RunSecondaryScript(void)
{
    void Event_Wait(s32);
    void Event_Wait(s32);
    void Event_Wait(s32);
    void Event_Wait(s32);

    Event_SetMessage(((s32)&Value_00002880));
    Event_Wait(20);

    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);

    Actor_FaceActor(1, 0, 50);
    Actor_ShowEmote(0, 0x105, 60);
    Event_Wait(10);
    Actor_FaceDirection(1, 0xc000, 0);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);

    Actor_RunRepeatedMotion(0, 2);
    Event_Wait(20);
    Event_Wait(25);

    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(30);

    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);

    Actor_FaceActor(2, 0, 30);
    Event_OpenMessage(0x2002, 0);
}

enum {
    ENTRANCE_SANCTUM_RETURN = 12,
    ENTRANCE_HOUSE_AFTER_REPORT = 21,
    ENTRANCE_FROM_AERIE = 90,
    ENTRANCE_HOUSE_REPORT = 99
};

enum {
    ACTOR_HOUSE_REPORT_SECOND = 11,
    ACTOR_HOUSE_REPORT_FIRST = 12,
    ACTOR_HOUSE_RESIDENT = 13,
    ACTOR_SANCTUM_SECOND = 18,
    ACTOR_SANCTUM_THIRD = 19,
    ACTOR_SANCTUM_FIRST = 20
};

enum {
    FLAG_AERIE_EVENTS_DONE = 0x9a7
};

/* The sanctum and the house, fixed when the overlay is linked. */
extern u8 LinkedScene_RariberoSanctum;
extern u8 LinkedScene_RariberoHeya;

/* The action table actor 14 takes while flag 0x300 is set. */
extern const u8 Data_02009314[];

/*
 * Opens both Lalivero interiors. Arriving from the aerie by entrance 90 sets
 * flag 0x9a7. In the sanctum three actors take collision flag 4 and sprite
 * priority 2, and returning by entrance 12 records the sanctum as the scene
 * to come back to. In the house the resident is set up the same way, actor
 * 14 takes its action table while flag 0x300 is set, and arriving by
 * entrance 99 plays the report before the entrance becomes 21.
 */
s32 Scene_Initialize(void)
{
    s32 scene;
    s16 entrance;
    struct FieldActor *actor;

    if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.scene;
    if (scene == (s32)&LinkedScene_RariberoSanctum) {
        actor = Actor_Get(ACTOR_SANCTUM_FIRST);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Actor_Get(ACTOR_SANCTUM_SECOND);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Actor_Get(ACTOR_SANCTUM_THIRD);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        Actor_SetAnimation(15, 6);
        entrance = gGameState.entrance;
        if (entrance == ENTRANCE_SANCTUM_RETURN) {
            gGameState.saved_scene = scene;
            gGameState.saved_entrance = entrance;
        }
    }
    if (gGameState.scene == (s32)&LinkedScene_RariberoHeya) {
        actor = Actor_Get(ACTOR_HOUSE_RESIDENT);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        if (GameFlag_IsSet(0x300) != 0) {
            Actor_EnableActionCallback(14, Data_02009314);
        }
        if (gGameState.entrance == ENTRANCE_HOUSE_REPORT) {
            Scene_RunPrimaryScript();
            Actor_SetActionCallback(Actor_Get(ACTOR_HOUSE_REPORT_FIRST), 6);
            Actor_SetActionCallback(Actor_Get(ACTOR_HOUSE_REPORT_SECOND), 6);
            gGameState.entrance = ENTRANCE_HOUSE_AFTER_REPORT;
        }
    }
    return 0;
}
