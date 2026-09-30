#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneIfWeArentGoingSet[];
extern u8 MsgFuneItsMyLuckyAnchor[];
extern u8 MsgFuneWhereGoingSlay[];
extern u8 MsgFuneWonderCouldHaveHappened[];
extern u8 MsgFuneYoureTryingLaunchShip[];

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
extern u8 FuneHeya_ActionScriptA[];
extern u8 FuneHeya_ActionScriptB[];
void UiText_ShowCenteredMessage();
void Ui_SetRenderResultFromObject();
void FieldScene_CallPairWith10();
void ConfigureSceneMotionFlags();
void Battle_ResetEffectCounterFar();
void Object_RefreshSelectorById();
u8 *Object_GetByIdFar();
void Audio_PlayCueFromEventWork();
void FieldScene_RunSceneStep();
void PartyInventory_Discard();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

void FieldScene_RunScene3b1SequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x301) != 0) {
        Event_Begin();
        Ui_SetRenderResultFromObject(8);
        UiText_ShowCenteredMessage((s32)MsgFuneWhereGoingSlay, 1, 8);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
        FieldScene_CallPairWith10(0, 0x4000);
        Event_End();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * ConfigureSceneMotionFlags and FieldScene_CallPairWith10 are the same stub declared twice without
 * a prototype, because the two call sites pass different argument counts.
 */

void FieldScene_RunFlagGatedThreeActorSetup(void)
{
    if (GameFlag_IsSet(0x922) == 0)
        return;

    Event_Begin();
    Battle_ResetEffectCounterFar();
    Camera_SetSpeed(0x19999, 0x3333);
    ConfigureSceneMotionFlags(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Event_SetMessage((s32)MsgFuneWonderCouldHaveHappened);

    FieldScene_RunStepThen10(8);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(8, 0x3000);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(10, 0xd000);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(9, 0x5000);
    FieldScene_RunStepThen10(9);

    Actor_FaceDirection(8, 0, 20);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(9, 0x8000);
    FieldScene_RunStepThen10(9);
    FieldScene_RunStepThen10(10);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(10, 0xb000);
    FieldScene_RunStepThen10(8);

    GameFlag_Set(0x920);
    Event_End();
}

void FieldScene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (GameFlag_IsSet(0x911) == 0) {
    } else {
        Event_Begin();
        Battle_ResetEffectCounterFar();
        Camera_SetSpeed(0x26666, 0x4ccc);
        ConfigureSceneMotionFlags( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Actor_RunRepeatedMotion(13, 1);
        Event_SetMessage((s32)MsgFuneIfWeArentGoingSet);
        FieldScene_RunStepThen10(0x200d);
        FieldScene_CallPairWith10(12, 0xd000);
        request_a = 0x800c;
        Actor_ShowEmote(12, 0x102, 20);
        Actor_StartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Actor_RunRepeatedMotion(14, 1);
        Event_ShowMessageAndWait(0xa00e, 0, 20);
        FieldScene_CallPairWith10(12, 0);
        request_b = 0xa00e;
        Actor_ShowEmote(12, 0x101, 40);
        Actor_ShowEmote(14, 0x103, 40);
        Actor_StartRepeatedMotion(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_SetAttachedEffect(12, 0x102);
        Event_Wait(40);
        Actor_StartRepeatedMotion(12, 3);
        FieldScene_RunStepThen10(request_a);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0xb000);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(12, 0xd000);
        Actor_ShowEmote(12, 0x100, 30);
        Actor_StartRepeatedMotion(12, 1);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(13, 4);
        FieldScene_RunStepThen10(0x200d);
        Actor_StartRepeatedMotion(13, 2);
        FieldScene_RunStepThen10(0x200d);
        Actor_SetAnimation(12, 4);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(14, 4);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0x8000);
        Actor_StartRepeatedMotion(14, 2);
        Event_ShowMessageAndWait(request_b, 0, 20);
        Actor_FaceDirection(12, 0, 0);
        Actor_ShowEmote(12, 0x102, 80);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_ShowEmote(14, 0x103, 0);
        Actor_ShowEmote(13, 0x103, 60);
        Actor_StartRepeatedMotion(14, 2);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0xb000);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(13, 0x3000);
        Actor_ShowEmote(13, 0x101, 0);
        Actor_ShowEmote(12, 0x101, 60);
        Actor_RunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Actor_ShowEmote(14, 0x103, 40);
        Actor_RunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0x5000, 40);
        Actor_FaceDirection(12, 0, 0);
        FieldScene_CallPairWith10(13, 0x3000);
        Actor_RunRepeatedMotion(12, 2);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_FaceDirection(14, 0x4000, 40);
        FieldScene_RunStepThen10(request_b);
        Actor_StartRepeatedMotion(12, 2);
        Actor_RunRepeatedMotion(13, 2);
        Event_Wait(60);
        Actor_RunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Actor_SetAnimationAndWait(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_ShowEmote(12, 0x102, 40);
        Actor_RunRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Actor_SetAnimationAndWait(13, 3);
        FieldScene_RunStepThen10(13);
        Actor_FaceDirection(14, 0xb000, 40);
        Actor_SetAnimation(14, 3);
        Actor_SetAnimationAndWait(13, 3);
        Actor_SetSpeed(14, 0x19999, 0xcccc);
        Actor_SetSpeed(13, 0x19999, 0xcccc);
        action = (s32)FuneHeya_ActionScriptA;
        Actor_EnableActionCallback(14, action);
        Actor_EnableActionCallback(13, action);
        Event_Wait(20);
        Actor_FaceDirection(12, 0x4000, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x26666, 0x13333);
        *(u8 *)(Object_GetByIdFar(0) + 90) &= 254;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 0x208);
        Event_Wait(1);
        {
            u8 *record = Object_GetByIdFar(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Actor_Jump(12, 4, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 20);
        Actor_StartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(12);
        Actor_SetSpeed(12, 0x19999, 0xcccc);
        Actor_EnableActionCallback(12, action);
        Event_Wait(40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Object_RefreshSelectorById(12);
        GameFlag_Set(0x922);
        Event_End();
    }
}

void FieldScene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Audio_PlayCue(28);
    Camera_SetSpeed(0x26666, 0x4ccc);
    ConfigureSceneMotionFlags(0x1c80000, -1, 0x2880000, 0x10000014);
    Actor_RunRepeatedMotion(9, 1);
    Event_SetMessage((s32)MsgFuneYoureTryingLaunchShip);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(13, 0x8000, 40);
    Actor_ShowEmote(9, 0x103, 40);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(12, 0, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 20);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x102, 20);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x105, 60);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x104, 20);
    FieldScene_RunStepThen10(0x900c);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(12, 0x3000);
    FieldScene_RunStepThen10(0x900c);
    FieldScene_CallPairWith10(11, 0xb000);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(13, 1);
    Actor_SetAnimationAndWait(13, 3);
    FieldScene_RunStepThen10(13);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(12, 0x5000, 0);
    FieldScene_CallPairWith10(11, 0x5000);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(12, 0x1bc, 0x29c);
    Actor_WalkToAndWait(13, 0x1d8, 0x29c);
    Actor_WaitForMove(12);
    Actor_SetAnimation(12, 1);
    Event_Wait(80);
    FieldScene_CallPairWith10(12, 0xd000);
    Actor_ShowEmote(12, 0x101, 60);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x400b, 0, 40);
    Actor_RunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0xd000, 0);
    FieldScene_RunStepThen10(0x100b);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_ShowEmote(9, 0x101, 60);
    Actor_SetAnimation(11, 4);
    Event_Wait(20);
    FieldScene_RunStepThen10(0x100b);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetAttachedEffect(13, 0x102);
    Actor_Jump(13, 2, 20);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimationAndWait(9, 3);
    request_a = 0x100c;
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(11, 0xd000);
    Actor_RunRepeatedMotion(12, 1);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x105, 40);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x102, 40);
    Actor_Jump(13, 4, 0);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Actor_StartRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x100b, 0, 40);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x5000, 20);
    Actor_StartRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimation(11, 3);
    Event_Wait(20);
    Actor_ShowEmote(12, 0x100, 40);
    Actor_StartRepeatedMotion(12, 2);
    request_b = 0x400b;
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 20);
    FieldScene_RunStepThen10(request_b);
    Actor_StartRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(request_a);
    Actor_SetAnimationAndWait(11, 3);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Actor_ShowEmote(12, 0x102, 60);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x101, 40);
    Actor_FaceDirection(11, 0xd000, 20);
    Actor_SetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x103, 20);
    Actor_StartRepeatedMotion(11, 2);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(9, 0x108, 40);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(9, 0xd000, 40);
    FieldScene_RunStepThen10(9);
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(10, 0xb000, 40);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Actor_SetAnimation(12, 3);
    Event_ShowMessageAndWait(request_a, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x108, 40);
    Actor_SetAnimation(12, 3);
    FieldScene_RunStepThen10(request_a);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0x8000, 20);
    Audio_PlayCue(19);
    Actor_StartRepeatedMotion(8, 2);
    request_c = 0x8008;
    Actor_ShowEmote(8, 0x100, 80);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(12, 0x101, 0);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(13, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_Jump(8, 4, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessage(request_c, 0);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_WalkToAndWait(8, 0x1db, 0x256);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    Actor_WalkToAndWait(9, 0x1ce, 0x26a);
    FieldScene_CallPairWith10(9, 0xb000);
    Actor_ShowEmote(9, 0x100, 40);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(0x8009);
    Actor_ShowEmote(11, 0x101, 60);
    FieldScene_RunStepThen10(11);
    Actor_ShowEmote(12, 0x102, 20);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x103, 20);
    Actor_Jump(8, 4, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    FieldScene_RunStepThen10(8);
    Audio_PlayCue(28);
    Actor_StartRepeatedMotion(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x101, 60);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(8, 0x3000);
    Actor_SetAnimationAndWait(8, 4);
    FieldScene_RunStepThen10(8);
    Actor_WalkToAndWait(12, 0x1bc, 0x274);
    FieldScene_CallPairWith10(12, 0xd000);
    FieldScene_RunStepThen10(0x900c);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(11, 0x102, 60);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x107, 40);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(9, 0x3000);
    Actor_SetAnimationAndWait(9, 4);
    FieldScene_RunStepThen10(0x1009);
    FieldScene_CallPairWith10(12, 0);
    Actor_RunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(11, 0, 0);
    Actor_ShowEmote(12, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 60);
    Camera_SetSpeed(0x13333, 0x2666);
    ConfigureSceneMotionFlags(0x1d00000, -1, 0x2a80000, 0x10000000);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10, 0);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(10, 0x8000, 20);
    Actor_ShowEmote(10, 0x100, 0);
    Actor_Jump(10, 4, 40);
    FieldScene_RunStepThen10(10);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_RunStepThen10(10);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkTo(13, 0x1b6, 0x293);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Audio_PlayCue(17);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkToAndWait(10, 0x1e8, 0x2ae);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_WaitForMove(13);
    Actor_SetAnimation(13, 1);
    Actor_FaceDirection(13, 0xd000, 0);
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x921);
}

void FieldScene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Event_Begin();
    Battle_ResetEffectCounterFar();
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(8, 0x100, 40);
    request_a = 0x1008;
    Actor_StartRepeatedMotion(8, 3);
    Event_SetMessage((s32)MsgFuneItsMyLuckyAnchor);
    FieldScene_RunStepThen10(request_a);
    Actor_StartRepeatedMotion(9, 1);
    Actor_StartRepeatedMotion(12, 1);
    Actor_StartRepeatedMotion(11, 1);
    Actor_StartRepeatedMotion(13, 1);
    Actor_RunRepeatedMotion(10, 1);
    Actor_FaceDirection(9, 0xd000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_FaceDirection(10, 0xb000, 20);
    Actor_RunRepeatedMotion(8, 1);
    Event_OpenMessage(request_a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(9, 2);
        FieldScene_RunStepThen10(0x9009);
        Actor_ShowEmote(8, 0x108, 40);
        FieldScene_RunStepThen10(request_a);
        gEventWork->message += 2;
    } else {
        gEventWork->message += 2;
        Actor_RunRepeatedMotion(9, 1);
        FieldScene_RunStepThen10(0x9009);
        Actor_StartRepeatedMotion(8, 2);
        FieldScene_RunStepThen10(0x9008);
    }
    Actor_ShowEmote(13, 0x105, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1d80000, -1, 0x27c0000, 1);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkToAndWait(13, 0x1d8, 0x296);
    FieldScene_CallPairWith10(13, 0xb000);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(8, 0x5000);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(11, 0, 0);
    Actor_FaceDirection(13, 0x8000, 20);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(12, 1);
    FieldScene_CallPairWith10(12, 0x3000);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(11, 0x101, 40);
    FieldScene_RunStepThen10(11);
    request_b = 0x900c;
    FieldScene_CallPairWith10(12, 0xd000);
    Actor_SetAnimation(12, 4);
    FieldScene_RunStepThen10(request_b);
    FieldScene_CallPairWith10(13, 0xb000);
    Actor_RunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x100, 20);
    FieldScene_CallPairWith10(9, 0x3000);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Actor_SetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(12, 0xd000);
    Actor_SetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(11, 2);
    FieldScene_CallPairWith10(11, 0xb000);
    FieldScene_RunStepThen10(11);
    FieldScene_CallPairWith10(12, 0);
    FieldScene_RunStepThen10(request_b);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e6, 0x260);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    record = Object_GetByIdFar(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1e6, 0x270);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    record = Object_GetByIdFar(1);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x1e6, 0x280);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    record = Object_GetByIdFar(2);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MIA, 0x1e6, 0x290);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_ShowEmote(12, 0x108, 40);
    FieldScene_RunStepThen10(request_b);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(0x1009);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8, 0x5000);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(8, 0x3000);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Actor_StartRepeatedMotion(8, 2);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimationAndWait(12, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_StartRepeatedMotion(9, 1);
        Event_ShowMessageAndWait(0x9009, 0, 40);
        gEventWork->message += 1;
    } else {
        gEventWork->message += 3;
        Actor_StartRepeatedMotion(8, 3);
        Event_ShowMessageAndWait(8, 0, 40);
    }
    Actor_RunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_RunRepeatedMotion(8, 1);
    value = 176;
    FieldScene_CallPairWith10(8, 0x5000);
    FieldScene_RunStepThen10(8);
    Actor_RunRepeatedMotion(13, 1);
    FieldScene_CallPairWith10(13, (value << 8));
    Event_ShowMessageAndWait(13, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Actor_WalkToAndWait(8, 0x1d8, 0x278);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x103, 40);
    Actor_StartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Actor_SetAnimation(8, 4);
    Event_ShowMessageAndWait(request_c, 0, 40);
    Actor_RunRepeatedMotion(11, 1);
    FieldScene_CallPairWith10(11, (value << 8));
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(10, 0x102, 20);
    Actor_SetSpeed(10, 0x26666, 0x13333);
    Actor_Jump(10, 2, 0);
    Actor_WalkToAndWait(10, 0x1ce, 0x2a2);
    FieldScene_CallPairWith10(10, (value << 8));
    Actor_StartRepeatedMotion(10, 2);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(9, 0x5000);
    Actor_SetAnimationAndWait(9, 4);
    FieldScene_RunStepThen10(9);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessageAndWait(13, 0, 40);
    FieldScene_CallPairWith10(9, 0x3000);
    Actor_StartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(0x1009);
    FieldScene_CallPairWith10(12, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(11, (value << 8), 0);
    Actor_FaceDirection(13, (value << 8), 0);
    Actor_FaceDirection(10, (value << 8), 20);
    Actor_RunRepeatedMotion(12, 1);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_ShowEmote(8, 0x101, 40);
    FieldScene_CallPairWith10(8, 0xd000);
    Event_OpenMessage(0x1008, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(8, 3);
        FieldScene_RunStepThen10(0x1008);
        gEventWork->message += 1;
    } else {
        gEventWork->message += 1;
        FieldScene_RunStepThen10(0x1008);
    }
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(0x1008);
    FieldScene_CallPairWith10(8, 0x8000);
    FieldScene_RunStepThen10(0x4008);
    FieldScene_RunSceneStep(2, 0, 0);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(9, 3);
    Actor_StartRepeatedMotion(10, 2);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    action = (s32)FuneHeya_ActionScriptB;
    Actor_EnableActionCallback(10, action);
    Event_Wait(4);
    Actor_EnableActionCallback(11, action);
    Event_Wait(4);
    Actor_EnableActionCallback(12, action);
    Event_Wait(4);
    Actor_EnableActionCallback(9, action);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = Object_GetByIdFar(2);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    record = Object_GetByIdFar(1);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Object_GetByIdFar(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_EnableActionCallback(13, action);
    Actor_WalkToAndWait(8, 0x1c8, 0x288);
    FieldScene_CallPairWith10(8, 0);
    PartyInventory_Discard(232);
    GameFlag_Set(0x925);
    Event_End();
}
