#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "FIELD_EVENT.H"

void Object_ResetMotion(struct ObjectRuntime *);
void Battle_WaitMode0(s32);
extern const u8 ObjectMotion_StepAngleScript[];

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_59 = 0;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)first, (u32)callback);
    }
}

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
        Object_SetMode((struct FieldActor *)object, 1);
    }
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct FieldSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->second_priority = priority;
        object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dst_attr;
    u32 merged;

    /* Keep the existing opaque sprite and packed OAM-word boundary. The
       typed sprite attempt changes register choice and read order. */
    p = Object_GetById(source_id);
    p = ((struct ObjectRuntime *)p)->animation;
    flags = ((struct FieldSprite *)p)->vram_block;
    shape = *(u16 *)((u8 *)p + 0x8);

    p = Object_GetById(target_id);
    p = ((struct ObjectRuntime *)p)->animation;
    dst_attr = *(u16 *)((u8 *)p + 0x8);
    ((struct FieldSprite *)p)->vram_block = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dst_attr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}

void ObjectVisual_ReservedNoOp(void)
{
}
