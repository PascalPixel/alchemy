/* Draft of resource_39f 0x02009d04 (FieldScene_RunScene39fSequenceA), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but its cue call reaches the veneer COMMON/OBJECT/STAGED_ACTOR.C names Audio_PlayCue, which FIELD_EVENT.H's inline of that name hides from this source; linking it would give the veneer a second name.
 * The listing keeps these rows. */
#include "MORI.H"

void Func_02002834();
void Func_02004c3c();

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void FieldScene_RunScene39fSequenceA(void)
{

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Actor_Get(15);
    big = 0x80000;
    Event_Begin();
    FieldScene_RunSixCallSetupSequence(15, 0);
    Call4(FieldScene_RunScene39f_02000d90, 15, 0x1d8, 104, big);
    Event_Wait(10);
    Func_02002834(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Camera_FollowActor(15, 1);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(15, 2);
    Actor_ShowEmote(15, 0x103, 0);
    Audio_PlayCue(147);
    Event_Wait(60);
    first = Actor_Get(ACTOR_PARTY_LEADER);
    shown = *(s16 *)(first + 10);
    second = Actor_Get(ACTOR_PARTY_LEADER);
    Call4(FieldScene_RunScene39f_02000d90, 15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Engine_EventWait)(10);
    GameFlag_Set(0x307);
    base3_2000240 = (s32)&gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02004c3c(53, 0);
    Event_End();
}
