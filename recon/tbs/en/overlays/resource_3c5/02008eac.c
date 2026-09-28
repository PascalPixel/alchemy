/* Draft of resource_3c5 0x02008eac..0x02008f58 (172 bytes with pool),
 * FieldScene_RunScene3c5SequenceA; the listing keeps the rows. The C
 * compiles to the reference's instructions, but its Audio_PlayCue is the
 * field event header's inline into Engine_AudioPlayCue, while this overlay's
 * audio import veneer is named Audio_PlayCue for the linked staged-actor code;
 * linking it would give that veneer a second name. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

void FieldScene_RunScene3c5SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 12) = 0x820000;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 68) = v5;
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = v5;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(10);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Engine_ActorGet(0) + 85) = 3;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(record + 40) = -0x50000;
    Actor_Get(ACTOR_PARTY_LEADER);
    OverlayObject_WaitUntilIdle();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Event_RequestExit(a0);
    Event_End();
}
