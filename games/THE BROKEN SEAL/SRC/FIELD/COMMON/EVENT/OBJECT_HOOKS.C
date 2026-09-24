#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

extern struct EventValueWork gGameState;

void ObjectTable_ResetForObject(s32);
void Event_NoOpHook(void);
s32 Event_SpawnObjectTable(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

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
