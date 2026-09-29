/* Draft of resource_3c7 0x02008eec (FieldScene_RunSecondaryScript): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgRariberoTellOthers). The listing
 * keeps these rows until the draft is adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.H"
extern u8 MsgRariberoTellOthers[];

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

    Event_SetMessage((s32)MsgRariberoTellOthers);
    Event_Wait(20);

    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);

    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);

    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    Event_Wait(20);

    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Event_Wait(25);

    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(30);

    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);

    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);

    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Event_OpenMessage(0x2002, 0);
}
