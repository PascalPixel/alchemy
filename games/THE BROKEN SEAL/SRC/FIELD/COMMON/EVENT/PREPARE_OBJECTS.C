#include "TYPES.H"

s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Runtime_GetObject(void);
s32 Party_AddActiveOwnerFar(s32 event_id);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Party_AddActiveOwnerFar(event_id);
    if (value != 0) {
        Party_ShowJoinedMessageFar(event_id, value);
    }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
    Party_AddActiveOwnerFar(first);
    Party_AddActiveOwnerFar(second);
    Party_ShowPairJoinedMessageFar(first, second);
}
