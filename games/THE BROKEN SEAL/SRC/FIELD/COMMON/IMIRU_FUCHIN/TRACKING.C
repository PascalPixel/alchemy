/* Two map triggers of the cave: while the entry setup has raised the cave's
 * tracking flag, one hands the leader to the tracking work, the other takes
 * it away. */
#include "IMIRU_FUCHIN.H"

/* Raised by the entry setup in the scenes that track the leader. It follows
 * the overlay's image. */
s32 ImiruFuchin_TrackLeader;

void ImiruFuchin_StartTrackingLeader(void)
{
    if (ImiruFuchin_TrackLeader != 0) {
        struct TrackingWork *work = *(gWorkSlot + 36);

        work->actor = Engine_ActorGet(ACTOR_PARTY_LEADER);
    }
}

void ImiruFuchin_StopTrackingLeader(void)
{
    if (ImiruFuchin_TrackLeader != 0) {
        struct TrackingWork *work = *(gWorkSlot + 36);

        work->actor = NULL;
    }
}
