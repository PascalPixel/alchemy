#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgFuneTheresNothingWeCanDo[];
extern u8 MsgFuneWonderCouldHaveHappened[];
extern u8 FuneKanpan_PresentationActionsA[];
extern u8 FuneKanpan_PresentationActionsB[];

void Battle_ResetEffectCounter();
void FieldScene_RunScene3af_02000bf0(void);
void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, s32 b);

/* FAKEMATCH: a value-returning call spelled through these wrappers sets r0
 * last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    u8 *record;

    if (GameFlag_IsSet(0x911) != 0) {
        Event_Begin();
        Battle_ResetEffectCounter();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0xbe0000, -1, 0x2c40000, 1);
        Camera_WaitForMove();
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((s32)MsgFuneWonderCouldHaveHappened);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x102, 60);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_RunRepeatedMotion(22, 1);
        Actor_FaceDirection(22, 0x5000, 0);
        FieldScene_RunStepThen10(0x4016);
        Actor_RunRepeatedMotion(20, 1);
        FieldScene_CallPairWith10(20, 0xb000);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(23, 0x3000);
        Actor_SetAnimation(23, 3);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(22, 0x101, 40);
        Actor_FaceDirection(22, 0x8000, 20);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(23, 0);
        Actor_SetAnimationAndWait(23, 4);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(20, 0x100, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(20, 0xd000);
        Actor_SetAnimation(23, 3);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(60);
        Actor_ShowEmote(22, 0x106, 40);
        FieldScene_CallPairWith10(22, 0x5000);
        Event_SetMessage((s32)MsgFuneTheresNothingWeCanDo);
        Actor_StartRepeatedMotion(22, 1);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x101, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_ShowEmote(22, 0x108, 20);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(23, 0x102, 60);
        FieldScene_RunStepThen10(0x4017);
        Value2((s32 (*)())FieldScene_CallPairWith10, 22, 0x8000);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(20, 0x102, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(22, 0x5000);
        Actor_SetAnimation(22, 4);
        FieldScene_RunStepThen10(22);
        Actor_FaceDirection(20, 0xb000, 0);
        Actor_FaceDirection(23, 0x3000, 40);
        Actor_FaceDirection(23, 0, 0);
        Actor_FaceDirection(20, 0xd000, 20);
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        FieldScene_RunStepThen10(0x4016);
        Actor_SetAttachedEffect(23, 0x102);
        Actor_SetAttachedEffect(20, 0x102);
        Event_Wait(40);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xb60000, -1, 0x2f80000, 1);
        Actor_SetSpeed(23, 0xcccc, 0x6666);
        Value2((s32 (*)())Engine_ActorEnableActionCallback, 23, (s32)FuneKanpan_PresentationActionsA);
        Actor_SetSpeed(22, 0xcccc, 0x6666);
        Value2((s32 (*)())Engine_ActorEnableActionCallback, 22, (s32)FuneKanpan_PresentationActionsB);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_WalkToAndWait(20, 182, 0x2f8);
        Actor_StartRepeatedMotion(20, 2);
        Actor_ShowEmote(20, 0x100, 60);
        FieldScene_CallPairWith10(20, 0xd000);
        Event_ShowMessageAndWait(20, 0, 20);
        Actor_SetAnimationAndWait(20, 3);
        Actor_Jump(20, 4, 0);
        Actor_FaceDirection(20, 0x3000, 40);
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_MoveTo(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = ((u8 *(*)())Engine_ActorGet)(20);
            u8 bits = 1;

            bits |= record[35];
            record[35] = bits;
        }
        Actor_SetSpeed(20, 0x13333, 0x9999);
        Actor_WalkToAndWait(20, 182, 0x30e);
        Actor_WalkToAndWait(20, 192, 0x328);
        Actor_WalkToAndWait(20, 216, 0x328);
        FieldScene_CallPairWith10(20, 0xd000);
        Actor_RunRepeatedMotion(20, 2);
        FieldScene_RunScene3af_02000bf0();
        Actor_WalkToAndWait(20, 216, 0x31e);
        Actor_SetPosition(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = ((u8 *(*)())Engine_ActorGet)(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = (u8 *)Value1((s32 (*)())Engine_ActorGet, 20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set(0x920);
        Event_End();
    }
}
