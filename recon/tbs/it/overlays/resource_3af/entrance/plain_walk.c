/* NONMATCHING: localized ship deck entrance, 2026-10-01.
 * The approved TBS flags compile the complete 60-byte extent. Six bytes
 * differ only in the ordering of movement argument setup; the plain direct
 * call shifts the z argument before setting actor and x. Production retains
 * the measured Call3 spelling and matches this edition.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FuneKanpan_RunDeckStateEvent(void);

void FuneKanpan_RunDeckStateEventFromEntry(void)
{
    if ((u16)(Object_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe) {
        Engine_EventBegin();
        Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 214, 664);
        ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
        FuneKanpan_RunDeckStateEvent();
    }
}
