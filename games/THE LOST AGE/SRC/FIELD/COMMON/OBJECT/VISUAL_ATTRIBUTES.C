#include "OBJECT_RUNTIME.H"
#include "FIELD_SPRITE.H"

extern const u8 ObjectMotion_StepAngleScript[];
struct ObjectRuntime *Object_GetById(u32);

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
    s32 delta = 0;

    if (object != NULL) {
        s32 target_angle = (u16)object->action;
        s32 current_angle = object->angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096)
                delta = 2048;
            if (delta < -4096)
                delta = -2048;
            object->angle = current_angle + delta;
        }
    }
    return delta;
}

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
        Object_SetCallback(object, ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}

/* ⚓️ arms the same turn and refreshes the object's selector instead of
   waiting. */
void ObjectMotion_ArmCallbackAndRefresh(s32 object_id, s32 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        Object_SetCallback(object, ObjectMotion_StepAngleScript);
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
        object->unknown_23 &= ~1;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dst_attr;
    u32 merged;

    p = Object_GetById(source_id);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x10);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = Object_GetById(target_id);
    p = *(void **)((u8 *)p + 0x50);
    dst_attr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x10) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dst_attr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}
