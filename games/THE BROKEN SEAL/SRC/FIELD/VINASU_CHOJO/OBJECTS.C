#include "FIELDOBJ.H"
/* Overlay object setup. */
#include "CHOJO.H"


void OverlayObject_SetRecordField1(struct FieldActor *actor, u32 value)
{
    actor->sprite->priority = value;
}

/* Create a scene object with sprite priority zero. */
void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *ret = Engine_ObjectCreate(kind, x, y, z);

    if (ret != NULL) {
        struct FieldSprite *obj = ret->sprite;

        obj->priority = 0;
        ret->motion_flags = 0;
        ret->collision_flags = 8;
        Engine_ActorSetSpriteFlags(ret, 0);
        ObjectGroup_SetChildValue(ret, 14);
        OverlayObject_SetValue1(ret, 1);
        return ret;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        struct FieldSprite *object = result->sprite;

        object->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
}
