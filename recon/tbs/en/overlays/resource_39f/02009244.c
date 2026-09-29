/* Draft of resource_39f 0x02009244 (FieldScene_RunActorElevenPresentationBeat), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but its cue call reaches the veneer COMMON/OBJECT/STAGED_ACTOR.C names Audio_PlayCue, which FIELD_EVENT.H's inline of that name hides from this source; linking it would give the veneer a second name.
 * The listing keeps these rows. */
#include "MORI.H"

void Func_02001d72();
u8 *Func_02004082_a();
u8 *Func_0200408c();
void Func_02004184();

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * Data_02000240 + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void FieldScene_RunActorElevenPresentationBeat(void)
{
    extern s16 Data_02000240[];

    u8 *slot;
    s32 offset;

    slot = Actor_Get(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Event_Begin();

    FieldScene_RunSixCallSetupSequence(11, 0);
    FieldScene_RunScene39f_02000d90(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Camera_FollowActor(11, 1);
    Actor_FaceEachOther(11, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(11, 2);
    Actor_ShowEmote(11, 0x103, 0);
    Audio_PlayCue(147);
    Event_Wait(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    FieldScene_RunScene39f_02000d90(11,
                  *(s16 *)(Func_02004082_a(0) + 10),
                  *(s16 *)(Func_0200408c(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Event_Wait(10);
    GameFlag_Set(0x301);
    Actor_SetPosition(14, 0, 0);

    offset = 0x22b;
    ((u8 *)Data_02000240)[offset] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Event_End();
}
