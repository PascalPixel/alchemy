#include "entry_setup.h"

void ConfigureOverlayObject(struct FieldActor *object, s32 palette)
{
    object->motion_flags = 0;
    object->collision_flags = 8;
    Engine_ActorSetSpriteFlags(object, 0);
    ObjectGroup_SetChildValue(object, palette);
}

void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *obj;

    obj = Engine_ObjectCreate(type, x, y, z);
    if (obj != 0) {
        ((struct FieldSpritePriority *)obj->sprite)->priority = 0;
        ConfigureOverlayObject(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return 0;
}

void *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object;

    object = Engine_ObjectCreate(type, x, y, z);
    if (object != 0) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 1;
        ConfigureOverlayObject(object, 15);
        object->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        return object;
    }
    return 0;
}
