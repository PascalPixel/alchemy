#include "FIELDOBJ.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

void SetEffectMode(struct FieldActor *work, u32 mode)
{
    work->sprite->priority = mode;
}

void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *overlay_object;
    struct FieldSprite *object_record;

    overlay_object = Object_Create(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = overlay_object->sprite;
        object_record->priority = 0;
        overlay_object->motion_flags = 0;
        overlay_object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(overlay_object, 0);
        ObjectGroup_SetChildValue(overlay_object, 0xE);
        Engine_ObjectSetBlendMode(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}

void *NewFlippedEffectObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
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
