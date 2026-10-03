#include "FIELDOBJ.H"
/*
 * Scene objects: an effect record's mode, and objects created where the
 * caller asks and made ready for the scene.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

void SceneEffect_SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Object_Create(fourth, first, second, third);
    if (obj != NULL) {
        rec = obj->sprite;
        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Object_Create(arg3, arg0, arg1, arg2);

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
