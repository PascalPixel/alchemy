/* Event wait trial: ordinary C through the maintained EventWork owner.
 * Full linked edition proof remains pending. */
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "SYSTEM.H"

void Battle_WaitMode0(s32 frames)
{
    struct EventWork *state = Ram_HeapSlots->event_work;
    if (state->wait_mode == 0 && frames != 0)
        WaitFrames(frames);
}
