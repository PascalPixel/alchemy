#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)

void OverlayObject_SetEntryField(struct FieldActor *object, s32 priority)
{
    struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)object->sprite;

    sprite->priority = priority;
}

void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 0;
        object->motion_flags = 0;
        object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 14);
        Engine_ObjectSetBlendMode(object, 1);
        return object;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 1;
        object->motion_flags = 0;
        object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 15);
        object->priority_flags = (object->priority_flags & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
        return object;
    }
    return NULL;
}
