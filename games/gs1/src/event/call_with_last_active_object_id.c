#include "types.h"
#include "scene.h"
#include "abi/event/call_with_last_active_object_id.h"

s32 ObjectTable_FindLastActiveId(void);

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Sys_Apply(event_id, ObjectTable_FindLastActiveId());
}
