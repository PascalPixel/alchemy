#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Sets both sprite priorities of a placed object and stops automatic priority. */
void Object_SetSpritePriority(struct FieldActor *object, s32 priority)
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
