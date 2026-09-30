#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"

struct EventWork {
    u8 unknown_000[0x158];
    s16 countdown;
};

/* Sets the scene and entrance the party travels to next, and sets the
   event work's countdown to 999. */
void Field_SetDestination(s16 scene, s16 entrance)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    gPartyState.scene = scene;
    gPartyState.entrance = entrance;
    event->countdown = 999;
}
