#include "TYPES.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

extern struct EventValueWork gGameState;
void ObjectTable_ResetForObject(s32);
void Event_NoOpHook(void);
s32 Event_SpawnObjectTable(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

/* The event work as this setter sees it: one word at +0x10 that scripts
   store through the far-call table. */
struct EventWordWork {
    u8 unknown_00[0x10];
    s32 word_10;
};

extern struct EventWordWork *gEventWork;

s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Owner_GetStateFar(void);
s32 Party_AddActiveOwnerFar(s32 event_id);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Equipment_HasValueFar(s32 group, s32 entry);
s32 GameFlag_TestFar(s32 group);
s32 Event_ValidatePackedId(u32 packed_id);

/* An empty event hook ahead of the object hooks; nothing in the image
   calls it by name. */
void Event_NoOpHookA(void)
{
}

void Event_NoOpHook(void)
{
    /* Event hook intentionally left empty. */
}

void Event_RunObjectHookAndWait(s32 object_id)
{
    Event_NoOpHook();
    ObjectTable_ResetForObject(object_id);
    WaitFrames(1);
    ObjectTable_Get(gGameState.value);
}

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}

void Event_SetWorkWord10(s32 value)
{
    gEventWork->word_10 = value;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Owner_GetStateFar();
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

s32 Event_ValidatePackedId(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (GameFlag_TestFar(group) == 0)
        return -2;
    if (Equipment_HasValueFar(group, entry) == 0)
        return -3;
    return 0;
}

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId((*(struct EventValidationWork *)&gGameState).values[0]) != 0)
        (*(struct EventValidationWork *)&gGameState).values[0] = 0;
    if (Event_ValidatePackedId((*(struct EventValidationWork *)&gGameState).values[1]) != 0)
        (*(struct EventValidationWork *)&gGameState).values[1] = 0;
}
