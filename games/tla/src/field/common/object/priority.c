#include "object_dispatch.h"
#include "field_sprite.h"

void Animation_ApplyChildValues(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        Animation_ApplyChildValuesToRecord(object->target.child, value);
}

void ObjectDispatch_SetSpritePriority(struct DispatchObject *object, s32 priority)
{
    struct FieldSprite *sprite;

    if (object == NULL || object->kind != 1)
    {
        return;
    }
    sprite = object->target.sprite;
    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= ~1;
}
