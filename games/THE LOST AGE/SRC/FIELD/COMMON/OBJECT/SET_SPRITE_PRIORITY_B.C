#include "TYPES.H"
#include "FIELD_EVENT.H"

/* ⚓️'s second copy of Object_SetSpritePriority, which overlays 6AA and 6B0
   link beside their first. */
void Object_SetSpritePriorityB(struct FieldActor *object, s32 priority)
{
    struct FieldSprite *sprite;

    if (object == 0 || object->unknown_54 == 0) {
        return;
    }
    sprite = object->sprite;
    sprite->priority = priority;
    sprite->part_priority = priority;
    object->priority_flags &= ~1;
}
