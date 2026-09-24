#include "TYPES.H"

s32 Func_08015420(s32 event_id, s32 value);
s32 Runtime_GetObject(void);
s32 Party_AddActiveOwnerFar(s32 event_id);
s32 Func_08015428(s32 first, s32 second);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Party_AddActiveOwnerFar(event_id);
    if (value != 0) {
        Func_08015420(event_id, value);
    }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
    Party_AddActiveOwnerFar(first);
    Party_AddActiveOwnerFar(second);
    Func_08015428(first, second);
}
