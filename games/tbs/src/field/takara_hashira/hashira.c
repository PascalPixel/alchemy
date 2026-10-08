#include "fieldobj.h"
#include "hashira.h"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        struct FieldSprite *rec = obj->sprite;

        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

    if (effect != NULL) {
        struct FieldSprite *sprite = effect->sprite;

        sprite->priority = 1;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags(effect, 0);
        ObjectGroup_SetChildValue(effect, 15);
        effect->priority_flags = (effect->priority_flags & 0xfe) | 2;
        return effect;
    }
    return NULL;
}
