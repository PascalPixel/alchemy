/* Draft of resource_373 0x0200af14 (FieldScene_RunPrimaryActorSequence): it
 * matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgHaidiaEverProtectFamily,
 * MsgHaidiaHowHaveYouBeen). The listing keeps these rows until the draft is
 * adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.H"
extern u8 MsgHaidiaEverProtectFamily[];
extern u8 MsgHaidiaHowHaveYouBeen[];

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void FieldScene_RunPrimaryActorSequence(void)
{
    u32 i;
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
    Event_OpenMessage(ACTOR_GERALD, 0); /* main:0808a178 */
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
