#include "EVENTWRK.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "FIELD_SPRITE.H"

extern const u8 ObjectMotion_StepAngleScript[];
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

/* ⚓️ arms the same turn and refreshes the object's selector instead of
   waiting. */
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

/* Gives the target object the source's frames: its VRAM block and tile.
   One pointer holds each object and then its sprite, as in ☀️; separate
   locals load the sprite into another register. */
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
