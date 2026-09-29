/* Draft of resource_373 0x0200acb0 (FieldScene_RunSecondaryActorSequence): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgHaidiaAh). The listing keeps these
 * rows until the draft is adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.H"
extern u8 MsgHaidiaAh[];

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void FieldScene_RunSecondaryActorSequence(void)
{
    u32 i;
    s32 record;
    s32 slot_table;

    Event_Begin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
    slot_table = (s32)MsgHaidiaAh;
    Event_SetMessage(slot_table);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Camera_SetSpeed(0x10000, 0x2000); /* main:0808a208 */
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
    Actor_SetAnimationAndWait(8, 3); /* main:0808a110 */
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2); /* main:0808a138 */
    Event_OpenMessage(0x4008, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 1) { /* main:0808a070 */
        bump_step(1);
        Actor_StartRepeatedMotion(8, 1);
    }
    Event_ShowMessageAndWait(0x4008, 0, 40); /* main:0808a188 */
    Actor_ShowEmote(8, 0x105, 60);
    /* Pass the slot table's field at +6 for the slot passed above. */
    Event_SetMessage((slot_table + 6)); /* main:0808a170 */
    Event_ShowMessageAndWait(0x4008, 0, 20); /* main:0808a188 */
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1); /* main:0808a138 */
    Event_Wait(40);
    Event_ShowMessageAndWait(0x1001, 0, 40); /* main:0808a188 */
    Actor_RunRepeatedMotion(8, 1); /* main:0808a138 */
    Actor_FaceDirection(8, 0xd000, 20);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_ShowMessageAndWait(0x1001, 0, 120); /* main:0808a188 */
    Event_ShowMessageAndWait(0x4008, 0, 20); /* main:0808a188 */
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Event_ShowMessageAndWait(0x1001, 0, 40); /* main:0808a188 */
    Actor_SetAnimationAndWait(8, 4); /* main:0808a110 */
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
    Event_Wait(40);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10); /* main:0808a188 */
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3); /* main:0808a110 */
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
    Event_End(); /* main:0808a020 */
}
