#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_event.h"

void Field_SetDestination(s16 scene, s16 entrance)
{
    struct EventWork *event = Ram_HeapSlots->eventWork;

    gPartyState.scene = scene;
    gPartyState.entrance = entrance;
    event->countdown = 999;
}

void Func_080d2c64(s32 cue)
{
    struct EventWork *event = Ram_HeapSlots->eventWork;
    s32 state;

    /* FAKEMATCH: loads the event work before it builds the field offset. */
    __asm__("");
    state = cue;
    event->cueState = state;
    if ((s16)state == -1)
        state = 0x186;
    Audio_PlayCue(0x12a);
    Audio_PlayCue(state);
}
