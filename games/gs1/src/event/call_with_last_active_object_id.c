#include "types.h"

#define Event_CallWithLastActiveObjectId Func_080917ac

s32 Func_0808b3ec(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Func_0808b3ec(event_id, ObjectTable_FindLastActiveId());
}
