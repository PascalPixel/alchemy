#include "scene.h"
#include "event_runtime.h"
#include "object_lookup.h"
#include "types.h"

/* event/no_op_hook.c */
void Event_NoOpHook(void)
{
    /* Event hook intentionally left empty. */
}

/* event/run_object_hook_and_wait.c */
extern struct EventValueWork gCell;

void WaitFrames(s32);

void Event_RunObjectHookAndWait(s32 object_id)
{
    Sys_Run();
    Sys_Do(object_id);
    WaitFrames(1);
    ObjectTable_Get(gCell.value);
}

/* event/call_with_last_active_object_id.c */
s32 ObjectTable_FindLastActiveId(void);

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Sys_Apply(event_id, ObjectTable_FindLastActiveId());
}

/* event/prepare_object_and_apply_value.c */
s32 Runtime_GetObject(void);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Sys_Check(event_id);
    if (value != 0) {
        Sys_Apply(event_id, value);
    }
}

/* event/prepare_two_objects_and_apply.c */
void Event_PrepareTwoObjectsAndApply(void *first, void *second)
{
    Sys_Check(first);
    Sys_Check(second);
    Sys_Apply(first, second);
}

/* event/validate_packed_id.c */
s32 GameFlag_IsSet(s32 group);

s32 Event_ValidatePackedId(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (GameFlag_IsSet(group) == 0)
        return -2;
    if (Sys_Apply(group, entry) == 0)
        return -3;
    return 0;
}

/* event/clear_invalid_packed_values.c */
extern struct EventValidationWork gCell;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gCell.values[0]) != 0)
        gCell.values[0] = 0;
    if (Event_ValidatePackedId(gCell.values[1]) != 0)
        gCell.values[1] = 0;
}
