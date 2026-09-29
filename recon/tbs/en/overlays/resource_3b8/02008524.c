/* resource_3b8:02008524..02008af8 (1492 bytes), still linked from the
 * listing. Remaining differences: SceneDialogue_ShowMessage22a8Branch,
 * SceneDialogue_RunChoiceSequence22ab and SceneDialogue_RunChoiceSequence2352
 * derive their answer messages from one loaded base (MsgTorebiMeetBabi,
 * MsgTorebiEasternShoresKaragol, MsgTorebiFoundCloakBall; flag 0xf31 is
 * pool-loaded too); FieldScene_RunScene3b8SequenceB loads MsgTorebiGetUp
 * from the pool. With the messages named the draft is 1624 bytes against
 * 1492, and 468 halfwords differ. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgTorebiGetUp[];
extern u8 MsgTorebiEasternShoresKaragol[];
extern u8 MsgTorebiFoundCloakBall[];
extern u8 MsgTorebiMeetBabi[];
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

void SceneDialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)MsgTorebiMeetBabi;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(k + 1);
    else
        Event_SetMessage(k + 2);
    Engine_EventShowMessage(a, 0);
}

void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)MsgTorebiEasternShoresKaragol;

    Event_SetMessage(msg);
    Event_OpenMessage(no, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
        Event_SetMessage(msg + 1);
    else
        Event_SetMessage(msg + 2);
    Event_ShowMessage(no, 0);
}

void SceneDialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Event_Begin();
    Battle_ResetEffectCounterFar();
    msg = (s32)MsgTorebiFoundCloakBall;
    Event_SetMessage(msg);
    Event_ShowMessage(-1, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 30);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage(msg + 2);
        Event_ShowMessage(14, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(msg + 3);
        Event_ShowMessage(14, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Event_Wait(30);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Event_Wait(30);
        Actor_SetPosition(16, 0, 0);
        Item_ShowFound(ITEM_CLOAK_BALL, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Party_GiveItem(ITEM_CLOAK_BALL, 0);
        GameFlag_Set(0xf31);
    }
}

void FieldScene_RunScene3b8SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiGetUp);
    v5 = 0;
    *(u8 *)(Engine_ActorGet(0) + 84) = v5;
    *(u8 *)(Engine_ActorGet(10) + 84) = v5;
    Engine_TaskWait(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Call2(Engine_EventShowMessage, -1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Engine_ActorGet(0) + 84) = v5;
    *(u8 *)(Engine_ActorGet(10) + 84) = v5;
    Engine_ActorSetAnimation(0, 31);
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorSetPosition, 1, 0x780000, 0x680000);
    Call3(Engine_ActorSetPosition, 3, 0x680000, 0x500000);
    Call3(Engine_ActorSetPosition, 2, 0x780000, 0x780000);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_ActorFaceDirection(3, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 60;
    Event_SetStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Engine_EventWait(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 24;
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_MIA, 16, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Value1(Engine_ActorGet, 0);
    *(s32 *)(record + 64) += -0x30000;
    Engine_ActorSetAnimation(0, 32);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(0, 34);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(0, 33);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x105, 60);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 1, 0x102, 60);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorShowEmote, 0, 0x102, 80);
    Call3(Engine_ActorShowEmote, 2, 0x106, 60);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(45);
    Engine_ActorFaceDirection(1, 0, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    Engine_EventWait(30);
    Value2(Engine_EventOpenMessage, 1, 0);
    if (Value2(Engine_EventChooseYesNo, -1, 0) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 34);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        AdvanceMessageCursor(1);
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(0, 33);
        AdvanceMessageCursor(2);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorWalkByAndWait, 1, -16, 0);
    Engine_ActorFaceDirection(1, 0, 0);
    Engine_EventWait(35);
    Engine_ActorJump(0, 6, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x1e666, 0xf333);
    Call3(Engine_ActorWalkByAndWait, 0, -32, 0);
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorFaceDirection, 3, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(0, 3);
    Event_Wait(30);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Call3(Engine_ActorSetSpeed, 1, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 3, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 2, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        ObjectMotion_ResetAndSetPositionFar(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    ((void (*)())Engine_EventWait)(10);
    Engine_EventEnd();
}
