#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"

/* Sets the scene and entrance the party travels to next, and sets the
   event work's countdown to 999. */
void Field_SetDestination(s16 scene, s16 entrance)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    gPartyState.scene = scene;
    gPartyState.entrance = entrance;
    event->countdown = 999;
}
