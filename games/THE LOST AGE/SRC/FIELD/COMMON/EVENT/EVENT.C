#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

s32 Event_SpawnObjectTable(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

struct EventWordWork {
    u8 unknown_00[0x10];
    s32 word_10;
};

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
