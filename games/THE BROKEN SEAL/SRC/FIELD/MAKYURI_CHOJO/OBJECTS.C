#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

void MakyuriChojo_SetSpritePriority(struct OverlayObject *object, s32 priority)
{
    struct FieldSpritePriority *sprite;

    sprite = (struct FieldSpritePriority *)object->record;
    sprite->priority = priority;
}

void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object;

    object = Engine_ObjectCreate(type, x, y, z);
    if (object != NULL) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 0;
        object->motion_flags = 0;
        object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 14);
        RunOverlayObjectCommand1(object, 1);
        return object;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}
