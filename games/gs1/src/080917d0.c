#include "types.h"

s32 Func_08015420(s32 event_id, s32 value);
s32 Runtime_GetObject(void);
s32 Func_08077150(s32 event_id);

void Func_080917d0(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Func_08077150(event_id);
    if (value != 0) {
        Func_08015420(event_id, value);
    }
}
