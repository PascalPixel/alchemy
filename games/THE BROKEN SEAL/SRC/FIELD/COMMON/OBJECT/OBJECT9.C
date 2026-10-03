#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "TYPES.H"
#include "ANIMSPR.H"
#include "FIELDRUN.H"

extern struct FieldStepWork *gEventWork;

void Event_SetValue1d8(s16 value)
{
    gWork->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectSlotTable *state = (struct ObjectSlotTable *)gEventWork;
    /* FAKEMATCH: byte/scalar cells keep 56 bytes but fold the prefix into
       the load; retain the existing record-array access without a capacity. */
    struct ObjectRuntime *entry = state->slots[(u32)key & 0x0fff];

    if (entry != 0 && entry->animation_kind == 1)
        result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct FieldStepWork *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectRuntime *object = state->actors[index];

    if (object != 0 && object->animation_kind == 1 && ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->actors[index];
            if (object == 0 || object->animation_kind != 1 ||
                ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
                goto next;
            result = index;
        }
    }
    return result;
}
