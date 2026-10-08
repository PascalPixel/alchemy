#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "create_configured_overlay_object.h"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Engine_ObjectCreate(fourth, first, second, third);
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
#include "create_configured_overlay_object_body.inc"
}
