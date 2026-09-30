/*
 * Draft: Event_SetCueState does not yet match; the reference keeps the state
 * in r5 and preloads cue 0x186 into it before the first call.
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"

/* Records the event's cue state; -1 plays cues 0x12a and 0x186. */
void Event_SetCueState(s32 state)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->cue_state = state;
    if ((s16)state == -1) {
        Audio_PlayCue(0x12a);
        Audio_PlayCue(0x186);
    }
}
