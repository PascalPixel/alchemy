#include "TYPES.H"
#include "FIELD_EVENT.H"

void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 value);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);

/*
 * Creates a scene object that stays where it is placed: front priority, no
 * terrain motion, the collision flag set and attribute 14. ☀️'s
 * OverlayObject_PrepareObject writes the sprite priority itself; ⚓️ calls
 * the shared setter.
 */
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        Object_SetSpritePriority(object, 0);
        object->motion_flags = 0;
        object->collision_flags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 14);
        Animation_SetStateFlags(object, 1);
        return object;
    }
    return NULL;
}
