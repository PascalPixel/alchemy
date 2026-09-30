#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

s32 Event_SpawnObjectTable(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

/* The event work as this setter sees it: one word at +0x10 that scripts
   store through the far-call table. */
struct EventWordWork {
    u8 unknown_00[0x10];
    s32 word_10;
};

/* An empty event hook ahead of the object hooks; nothing in the image
   calls it by name. */

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
