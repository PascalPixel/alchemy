#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "overlay_object.h"
#include "create_configured_overlay_object.h"
#include "aerie.h"

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
#include "create_configured_overlay_object_body.inc"
}
