#include "types.h"
#include "field_event.h"

void Object_SetSpritePriorityB(struct FieldActor *object, s32 priority)
{
    struct FieldSprite *sprite;

    if (object == 0 || object->animation_kind == 0) {
        return;
    }
    sprite = object->sprite;
    sprite->priority = priority;
    sprite->part_priority = priority;
    object->priority_flags &= ~1;
}
