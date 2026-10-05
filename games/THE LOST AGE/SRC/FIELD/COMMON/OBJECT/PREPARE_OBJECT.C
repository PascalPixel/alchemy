#include "PROTO.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define ObjectDispatch_SetSingleChildFlags ((void (*)(struct FieldActor*,s32))ObjectDispatch_SetSingleChildFlags)

#define Object_SetPartAttribute ((void (*)(struct FieldActor*,s32))Object_SetPartAttribute)


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
