#include "object_runtime.h"

void Object_SetCallback(struct ObjectRuntime *, const void *);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Battle_WaitMode0(s32);
extern const u8 Data_0809fc1c[];

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
            first->unknown_56[3] = 0;
        }
        Object_SetCallback(first, callback);
    }
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
    s32 delta = 0;

    if (object != NULL) {
        s32 target_angle = (u16)object->action;
        s32 current_angle = *(u16 *)((u8 *)object + 6);
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096)
                delta = 2048;
            if (delta < -4096)
                delta = -2048;
            *(u16 *)((u8 *)object + 6) = current_angle + delta;
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
        Object_SetCallback(object, Data_0809fc1c);
        Battle_WaitMode0(wait);
    }
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 variant)
{
    u32 object_address;
    u8 *object_state;
    s32 variant_bits;
    s32 state_mask;
    s32 variant_mask;

    variant_mask = 3;
    object_address = (u32)ObjectTable_Get(object_id);
    if (object_address != 0 && (0xF & *(volatile s8 *)(object_address + 0x54)) == 1) {
        variant_bits = (variant_mask & variant) * 4;
        state_mask = -0xD;
        object_state = *(u8 **)(object_address + 0x50);
        object_state[9] = (state_mask & object_state[9]) | variant_bits;
        object_state[0x15] = (state_mask & object_state[0x15]) | variant_bits;
        *(u8 *)(object_address + 0x23) = 0xFE & *(u8 *)(object_address + 0x23);
    }
}
