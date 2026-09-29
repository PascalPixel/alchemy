/* The ship reaches Tolbi: the crew gathers on deck, the captain announces
   the landing and the party walks down the gangway. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern s32 FuneKanpan_LayerScroll[];
extern s32 FuneKanpan_LayerSpeed[];
extern u8 FuneKanpan_CrewScript[];
extern u8 MsgFuneFinallyReachedTolbi[];

void Event_CallWithLastActiveObjectId(u8 *script);
void FieldScene_RunStepThen10(s32 step);
void FieldScene_CallPairWith10(s32 actor, s32 facing);

void FuneKanpan_ArriveAtTolbi(void)
{
    Event_Begin();
    FuneKanpan_LayerScroll[0] = 0x40000;
    FuneKanpan_LayerSpeed[0] = -0x8000;
    Event_CallWithLastActiveObjectId(FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(21, 0xb60000, 0x26a0000);
    Actor_Get(21)->facing = 0xc000;
    Actor_SetPosition(20, 0xda0000, 0x2040000);
    Actor_Get(20)->facing = 0xb000;
    Actor_SetPosition(22, 0xcc0000, 0x20e0000);
    Actor_Get(22)->facing = 0xb000;
    Actor_SetPosition(23, 0, 0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Task_Wait(1);
    gEventWork->start_transition = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 182, 0x214);
    Actor_FaceDirection(21, 0xb000, 40);
    Event_SetMessage((s32)MsgFuneFinallyReachedTolbi);
    FieldScene_RunStepThen10(21);
    Actor_RunRepeatedMotion(20, 2);
    Actor_SetAnimation(20, 4);
    FieldScene_RunStepThen10(0x6014);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(22, 0x101, 60);
    Actor_SetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Actor_SetAttachedEffect(21, 0x102);
    Actor_SetAttachedEffect(22, 0x102);
    Event_Wait(80);
    Actor_ShowEmote(21, 0x100, 20);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 194, 0x1f4);
    Actor_FaceDirection(21, 0xb000, 20);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    Actor_WalkToAndWait(22, 192, 0x206);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 210, 0x1fc);
    FieldScene_CallPairWith10(20, 0xb000);
    Actor_RunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(0x5015);
    Actor_SetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0xd000);
    FieldScene_RunStepThen10(0x9016);
    Actor_FaceDirection(20, 0x8000, 20);
    Actor_SetAnimation(20, 4);
    FieldScene_RunStepThen10(0xa014);
    Actor_WalkToAndWait(20, 204, 0x218);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_WalkToAndWait(20, 182, 0x224);
    Actor_WalkToAndWait(20, 182, 0x250);
    Actor_WalkTo(20, 182, 0x298);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(16);
}
