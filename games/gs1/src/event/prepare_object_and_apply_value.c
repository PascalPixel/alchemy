#include "types.h"
#include "scene.h"
#include "abi/event/prepare_object_and_apply_value.h"

s32 Runtime_GetObject(void);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Sys_Check(event_id);
    if (value != 0) {
        Sys_Apply(event_id, value);
    }
}
