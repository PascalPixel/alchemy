#include "FIELDOBJ.H"
#include "HAIDIA.H"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        struct FieldSprite *object = obj->sprite;

        object->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
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
