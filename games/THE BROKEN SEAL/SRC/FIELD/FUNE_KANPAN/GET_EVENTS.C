#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The deck's events, chosen by the story flags the voyage has set. */
extern u8 gFuneKanpanEventsFlag93e[];
extern u8 gFuneKanpanEventsFlag8a0[];
extern u8 gFuneKanpanEventsFlag928[];
extern u8 gFuneKanpanEvents[];

/* What the deck answers. */
u8 *FuneKanpan_GetEvents(void)
{
    if (GameFlag_IsSet(0x93e))
        return gFuneKanpanEventsFlag93e;
    if (GameFlag_IsSet(0x8A0))
        return gFuneKanpanEventsFlag8a0;
    if (GameFlag_IsSet(0x928))
        return gFuneKanpanEventsFlag928;
    return gFuneKanpanEvents;
}
