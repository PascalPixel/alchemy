/* On deck a sailor asks how the party found the ship; when Isaac does not
   want to talk, the captain and the crew gather and the scene closes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 FuneKanpan_ClosingCrewScript[];
extern u8 FuneKanpan_LeaveActions[];
extern u8 MsgFuneHowWasRobinDidExplore[];

void Event_CallWithLastActiveObjectId(u8 *script);
void FieldScene_RunStepThen10(s32 step);
void FieldScene_CallPairWith10(s32 actor, s32 facing);
void FieldScene_RunScene3af_02000bb8(void);

void FuneKanpan_RunRobinTalk(void)
{
    Event_Begin();
    Event_CallWithLastActiveObjectId(FuneKanpan_ClosingCrewScript);
    Task_Wait(1);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
    Actor_ShowEmote(22, 0x100, 0);
    Actor_RunRepeatedMotion(22, 1);
    FieldScene_CallPairWith10(22, 0x5000);
    Event_SetMessage((s32)MsgFuneHowWasRobinDidExplore);
    Event_OpenMessage(0x2016, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        FieldScene_RunStepThen10(0x2016);
        Event_End();
        return;
    }
    gEventWork->message++;
    Event_AskYesNo(0x2016, 0);
    FieldScene_RunScene3af_02000bb8();
    Actor_SetPosition(26, 0xd80000, 0x24c0000);
    Actor_SetSpeed(26, 0x13333, 0x9999);
    Actor_WalkToAndWait(26, 216, 0x254);
    Actor_WalkToAndWait(26, 188, 0x268);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    FieldScene_CallPairWith10(26, 0x5000);
    Actor_Jump(26, 2, 0);
    Actor_SetAnimation(26, 4);
    Event_ShowMessage(26, 0);
    Actor_SetPosition(20, 0xb40000, 0x3090000);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 180, 0x298);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x2014);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(22, 0x3000, 0);
    Actor_ShowEmote(26, 0x101, 60);
    Actor_RunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(0x2014);
    Actor_StartRepeatedMotion(21, 2);
    FieldScene_RunStepThen10(21);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_SetAnimation(20, 3);
    FieldScene_RunStepThen10(0x6014);
    Actor_Jump(26, 2, 20);
    Actor_SetAnimation(26, 4);
    FieldScene_RunStepThen10(26);
    Actor_WalkToAndWait(20, 182, 0x280);
    Actor_FaceDirection(20, 0xd000, 0);
    FieldScene_RunStepThen10(0x8014);
    Actor_ShowEmote(26, 0x100, 20);
    Actor_StartRepeatedMotion(26, 2);
    FieldScene_RunStepThen10(26);
    Actor_SetAnimationAndWait(20, 3);
    FieldScene_CallPairWith10(22, 0);
    Actor_RunRepeatedMotion(22, 1);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x19999, 0xcccc);
    Actor_EnableActionCallback(22, FuneKanpan_LeaveActions);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 168, 0x278);
    Actor_EnableActionCallback(21, FuneKanpan_LeaveActions);
    Event_Wait(80);
    Actor_EnableActionCallback(26, FuneKanpan_LeaveActions);
    Event_Wait(40);
    FieldScene_CallPairWith10(20, 0x8000);
    FieldScene_RunStepThen10(0x2014);
    FieldScene_CallPairWith10(ACTOR_PARTY_LEADER, 0xe000);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(20, 3);
    gEventWork->start_transition = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(17);
}
