/* The leader's arrival when the cave first opens. */
#include "IMIRU_FUCHIN.H"

void FieldScene_RunScene39aSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    record = Engine_ActorGet(8);
    Actor_SetSpriteFlags(record, 0);
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x108, 196);
    Event_End();
}
