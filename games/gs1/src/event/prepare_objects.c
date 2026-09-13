#include "types.h"

s32 Func_08015420(s32 event_id, s32 value);
s32 Runtime_GetObject(void);
s32 Func_08077150(s32 event_id);
s32 Func_08015428(s32 first, s32 second);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Func_08077150(event_id);
    if (value != 0) {
        Func_08015420(event_id, value);
    }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
    Func_08077150(first);
    Func_08077150(second);
    Func_08015428(first, second);
}
