#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "OBJECT_RUNTIME.H"

void EventRuntime_SetMessage(s32 message)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->message_id = message;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    s32 result = -1;
    struct ObjectRuntime *object = event->objects[(u32)key & 0x0fff];

    if (object != NULL && object->animation_kind == 1)
        result = *((struct BattleAnimationState *)object->animation)->value_28;
    return result;
}
