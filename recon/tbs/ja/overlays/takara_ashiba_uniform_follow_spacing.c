/* Draft: First Treasure Isle following sequence.
 * 2026-10-01: Applying the other Japanese sequences16px leader step
 * here leaves one differing Thumb immediate (2 bytes); this first
 * leader and follower both step24px. Complete owner144 bytes.
 * Ordinary approved TBS flags.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
s32 *Engine_GetTriggerActor(s32 slot);

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
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
