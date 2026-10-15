#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "field_sprite.h"

struct ObjectRuntime *Object_GetById(u32);

void Object_ResetTargetAndSetMode1(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        Object_ResetMotion(object);
        Object_SetMode(object, 1);
    }
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)ObjectMotion_StepAngleScript);
        EventRuntime_Wait(wait);
    }
}

void ObjectMotion_ArmCallbackAndRefresh(s32 object_id, s32 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)ObjectMotion_StepAngleScript);
        Object_RefreshSelectorById(object_id);
    }
}

void ObjectMotion_SetActionVariant(u32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct FieldSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->part_priority = priority;
        object->priority_flags &= ~1;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    struct FieldSprite *sprite;
    u8 block;
    u32 tile;

    sprite = (struct FieldSprite *)Object_GetById(source_id);
    sprite = ((struct ObjectRuntime *)sprite)->animation;
    block = sprite->vram_block;
    tile = sprite->tile;

    sprite = (struct FieldSprite *)Object_GetById(target_id);
    sprite = ((struct ObjectRuntime *)sprite)->animation;
    sprite->vram_block = block;
    sprite->tile = tile;
}

void Func_080d390c(u32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
}

#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
void Func_080d3928(u32 id, s32 rotation)
{
    struct ObjectRuntime *object = Object_GetById(id);
    struct FieldSprite *sprite = object->animation;

    if (object != NULL && sprite != NULL)
        sprite->rotation = rotation;
}
#endif
