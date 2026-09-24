#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"

extern struct EventValueWork gGameState;

void WaitFrames(s32);
void FunctionHead_0808b674(s32);
void Event_NoOpHook(void);
s32 Func_0808b3ec(s32 event_id, s32 state);
s32 ObjectTable_FindLastActiveId(void);

void Event_NoOpHook(void)
{
    /* Event hook intentionally left empty. */
}

void Event_RunObjectHookAndWait(s32 object_id)
{
    Event_NoOpHook();
    FunctionHead_0808b674(object_id);
    WaitFrames(1);
    ObjectTable_Get(gGameState.value);
}

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Func_0808b3ec(event_id, ObjectTable_FindLastActiveId());
}
