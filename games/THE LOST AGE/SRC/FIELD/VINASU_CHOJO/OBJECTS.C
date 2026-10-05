#include "TYPES.H"
#include "FIELD_EVENT.H"

void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 value);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);

/* ☀️'s ConfigureOverlayObject, under ⚓️'s names for what it calls. */
void ConfigureOverlayObject(struct FieldActor *object, s32 parameter)
{
    object->motion_flags = 0;
    object->collision_flags = 8;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, parameter);
}

void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void Animation_SetStateFlags(struct FieldActor *object, s32 flags);

/* Creates one of the lighthouse top's objects at x, y, z with part
   attribute 14 and its animation flag set. */
struct FieldActor *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 0);
        ConfigureOverlayObject(object, 14);
        Animation_SetStateFlags(object, 1);
        return object;
    }
    return 0;
}

/* Creates an object in front, with part attribute 15 and its second
   priority flag set. */
struct FieldActor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 1);
        ConfigureOverlayObject(object, 15);
        object->priority_flags |= 2;
        return object;
    }
    return 0;
}
