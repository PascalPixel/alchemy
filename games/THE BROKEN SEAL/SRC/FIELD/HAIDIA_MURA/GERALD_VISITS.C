/* Gerald's two talks beside the leader, with actor 8 and then actor 9. Each
 * places Gerald at the leader's position, and ends with him walking back to
 * the leader's tile and a story flag set. */
#include "STAGED_MOTION.H"
extern u8 MsgHaidiaAh[];
extern u8 MsgHaidiaEverProtectFamily[];
extern u8 MsgHaidiaHowHaveYouBeen[];

void FieldScene_RunSecondaryActorSequence(void)
{
    s32 record;
    s32 msg;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
    msg = (s32)MsgHaidiaAh;
    Event_SetMessage(msg);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x18e0000, -1, 0x2460000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a4, 0x260);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(8, 0x3000, 0);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x192, 0x260);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Event_ShowMessage(0x1001, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2);
    Event_OpenMessage(0x4008, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
        Actor_StartRepeatedMotion(8, 1);
    }
    Event_ShowMessageAndWait(0x4008, 0, 40);
    Actor_ShowEmote(8, 0x105, 60);
    Event_SetMessage(msg + 6);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x1001, 0, 40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(0x1001, 0, 120);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Event_ShowMessageAndWait(0x1001, 0, 40);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(0x303);
    Event_End();
}

void FieldScene_RunPrimaryActorSequence(void)
{
    s32 record;

    Event_Begin();
    Camera_MoveTo(0x1650000, -1, 0x2e20000, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16f, 0x2e9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x15a, 0x2e9);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    Event_SetMessage((s32)MsgHaidiaHowHaveYouBeen);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(9, 2);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x3000, 10);
    Actor_FaceDirection(9, 0x5000, 10);
    Actor_FaceDirection(9, 0x3000, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 40);
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 60);
    } else {
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Event_SetMessage((s32)MsgHaidiaEverProtectFamily);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x1000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    GameFlag_Set(0x304);
    Event_End();
}
