#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
s32 *Object_GetById(s32 slot);

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(8);
#if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
    Engine_EventWait(4);
    Audio_PlayCue(188);
#if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(8, 0, 16);
#else
    Actor_SetDestinationOffset(8, 0, 24);
#endif
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(8, 0x168, 152);
    Engine_ActorWaitForMove(8);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}
