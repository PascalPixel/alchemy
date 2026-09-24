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

#include "TYPES.H"

s32 Equipment_HasValueFar(s32 group, s32 entry);
s32 GameFlag_IsSet(s32 group);

s32 Event_ValidatePackedId(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (GameFlag_IsSet(group) == 0)
        return -2;
    if (Equipment_HasValueFar(group, entry) == 0)
        return -3;
    return 0;
}

#include "EVENT_RUNTIME.H"

extern struct EventValidationWork gGameState;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gGameState.values[0]) != 0)
        gGameState.values[0] = 0;
    if (Event_ValidatePackedId(gGameState.values[1]) != 0)
        gGameState.values[1] = 0;
}
