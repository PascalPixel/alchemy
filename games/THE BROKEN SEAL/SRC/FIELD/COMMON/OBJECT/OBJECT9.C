#include "EDITION.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "TYPES.H"
#include "ANIMSPR.H"

struct ObjectValueTable {
    u8 unknown_00[0x14];
    struct ObjectRuntime *objects[192];
};

extern struct ObjectValueTable *gEventWork;

void Event_SetValue1d8(s16 value)
{
    gWork->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    s32 result = -1;
    struct ObjectRuntime *entry =
        gEventWork->objects[(u32)key & 0x0fff];

    if (entry != 0 && entry->animation_kind == 1)
        result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct ObjectValueTable *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectRuntime *object = state->objects[index];

    if (object != 0 && object->animation_kind == 1 && ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->animation_kind != 1 ||
                ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
                goto next;
            result = index;
        }
    }
    return result;
}
