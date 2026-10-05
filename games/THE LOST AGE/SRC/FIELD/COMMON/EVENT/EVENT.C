#include "PROTO.H"
#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"


struct EventWordWork {
    u8 unknown_00[0x10];
    s32 word_10;
};

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
