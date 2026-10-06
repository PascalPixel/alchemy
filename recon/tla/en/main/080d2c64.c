#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"

void Event_SetCueState(s32 state)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->cue_state = state;
    if ((s16)state == -1) {
        Audio_PlayCue(0x12a);
        Audio_PlayCue(0x186);
    }
}
