#include "object_lookup.h"
#include "fixed_math.h"
#include "types.h"
#include "scene.h"
#include "object_dispatch.h"

/* object/motion/aim/set_angle_toward.c */
struct Object_0809280c {
    u8 padding_00[6];
    s16 angle;
    s32 y;
    u8 padding_0c[4];
    s32 x;
};

s32 ArcTan2(s32, s32);
void Battle_WaitMode0(s32 arg0);

void Motion_SetAngleToward(s32 arg0, s32 arg1, s32 arg2)
{
    struct Object_0809280c *object1;
    struct Object_0809280c *object2;

    object1 = ObjectTable_Get(arg0);
    object2 = ObjectTable_Get(arg1);
    if (object1 != 0 && object2 != 0) {
        object1->angle = ArcTan2(object2->x - object1->x,
                                      object2->y - object1->y);
        Battle_WaitMode0(arg2);
    }
}

/* object/link/link_pair.c */
void ObjectLink_RotatePairToward(void *, void *);
void Battle_WaitMode0(s32 arg0);

void Object_LinkPair(s32 arg0, s32 arg1, s32 arg2)
{
    void *first = ObjectTable_Get(arg0);
    void *second = ObjectTable_Get(arg1);
    if (first != NULL && second != NULL) {
        ObjectLink_RotatePairToward(first, second);
        Battle_WaitMode0(arg2);
    }
}

/* object/table/destroy_by_id.c */
extern u8 *gWork;
/* 生成順維持のため戻り値を整数で受け、直後にオブジェクト番地として扱う。 */
s32 ObjectTable_Get(u32 object_id);
/* Object table: 192 pointers at gWork + 0x14 (object/table/get.c). */
void Object_Destroy(void *);

void ObjectTable_DestroyById(s32 index)
{
    void *object = (void *)ObjectTable_Get(index);
    u8 *base = gWork;
    s32 offset;

    if (object != 0) {
        Object_Destroy(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void ObjectTable_ReservedNoOp294C(void)
{
}

/* object/group/configure_child_value.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 gRom;
void ObjectGroup_SetChildValue(void *, s32);

void ObjectGroup_ConfigureChildValue(s32 arg0, s32 arg1)
{
    s32 mode_flags;
    void *object;

    object = ObjectTable_Get();
    if (object != NULL) {
        mode_flags = 0x100 & arg1;
        if (mode_flags != 0) {
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&gRom;
            return;
        }
        FIELD_AT_OFFSET(object, s32 *, 0x6C) = mode_flags;
        ObjectGroup_SetChildValue(object, arg1);
    }
}

/* object/group/apply_indexed_child_value.c */
extern u32 gIw;
extern u8 gRom[];

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        u8 *container;
        u8 child_count;

        child_value = gRom[(gIw >> 1) & 3];
        container = object->target.child;
        child_count = *(container + 0x27);
        if (child_count != 0) {
            u8 **entries = (u8 **)(container + 0x28);
            s32 remaining = child_count;
            do {
                u8 *entry = *entries++;
                if (entry != 0 && *(u32 *)(entry + 0x10) != 0) {
                    *(entry + 5) = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        *(container + 0x25) = 1;
    }
}

/* object/group/set_child_value.c */
void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value)
{
    if ((object->kind & 0xf) == 1) {
        u8 *container = object->target.child;
        u8 raw_count = container[0x27];

        if (raw_count != 0) {
            void **entry = (void **)(container + 0x28);
            u32 count = raw_count;
            do {
                void *item = *entry++;
                if (item != NULL && *(s32 *)((u8 *)item + 0x10) != 0) {
                    *((s8 *)item + 5) = value;
                }
                count--;
            } while (count != 0);
        }
        container[0x25] = 1;
    }
}

/* object/link/set_target_callback.c */
struct Object_08092a1c {
    u8 padding_00[0x30];
    s32 field_30;
    s32 field_34;
    u8 padding_38[0x21];
    u8 field_59;
    u8 padding_5a[0x0a];
    s16 field_64;
    u8 padding_66[2];
    struct Object_08092a1c *field_68;
};

void Object_SetCallback(struct Object_08092a1c *, s32);

void Object_SetTargetAndCallback(u32 arg0, s32 arg1, s32 arg2)
{
    struct Object_08092a1c *first = ObjectTable_Get(arg0);
    struct Object_08092a1c *second = ObjectTable_Get(arg1 & 0xff);

    if (first != 0 && second != 0) {
        first->field_68 = second;
        if (!(arg1 & 0x10000)) {
            first->field_64 = 40;
            first->field_34 = second->field_34 * 2;
            first->field_30 = second->field_30;
            first->field_59 = 0;
        }
        Object_SetCallback(first, arg2);
    }
}

/* object/motion/aim/step_angle.c */
struct ObjectMotionAngleState {
    u8 unknown_000[6];
    u16 current_angle;
    u8 unknown_008[92];
    u16 target_angle;
};

s32 Motion_StepAngle(struct ObjectMotionAngleState *object)
{
    s32 delta = 0;

    if (object != 0) {
        s32 target_angle = object->target_angle;
        s32 current_angle = object->current_angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096) {
                delta = 2048;
            }
            if (delta < -4096) {
                delta = -2048;
            }
            object->current_angle = current_angle + delta;
        }
    }
    return delta;
}

/* object/reset_target_and_set_mode1.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_SetMode(void *, s32);
s32 Object_ResetMotion();

void Object_ResetTargetAndSetMode1(void)
{
    void *temp_r0;

    temp_r0 = ObjectTable_Get();
    if (temp_r0 != NULL) {
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x38) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x3C) = 0x80000000;
        FIELD_AT_OFFSET(temp_r0, s32 *, 0x40) = 0x80000000;
        Object_ResetMotion();
        Object_SetMode(temp_r0, 1);
    }
}

/* object/motion/act/arm_callback.c */
void Object_SetCallback(u8 *, void *);
void Battle_WaitMode0(s32 arg0);
extern u8 gRom[];

void Motion_ArmCb(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *object = ObjectTable_Get(arg0);
    if (object != NULL) {
        *(s16 *)(object + 0x64) = arg1;
        Object_SetCallback(object, gRom);
        Battle_WaitMode0(arg2);
    }
}

/* object/motion/act/set_action_variant.c */
void Motion_SetActionVariant(s32 arg0, s32 arg1)
{
    u32 object_address;
    u8 *object_state;
    s32 variant_bits;
    s32 state_mask;
    s32 variant_mask;

    variant_mask = 3;
    object_address = (u32)ObjectTable_Get(arg0);
    if (object_address != 0 && (0xF & *(volatile s8 *)(object_address + 0x54)) == 1) {
        variant_bits = (variant_mask & arg1) * 4;
        state_mask = -0xD;
        object_state = *(u8 **)(object_address + 0x50);
        object_state[9] = (state_mask & object_state[9]) | variant_bits;
        object_state[0x15] = (state_mask & object_state[0x15]) | variant_bits;
        *(u8 *)(object_address + 0x23) = 0xFE & *(u8 *)(object_address + 0x23);
    }
}

/* object/visual/copy_attributes.c */
extern s32 Object_GetById(u32);

void ObjectVisual_CopyAttributes(u32 arg0, u32 arg1)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dstAttr;
    u32 merged;

    p = (void *)Object_GetById(arg1);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x1C);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = (void *)Object_GetById(arg0);
    p = *(void **)((u8 *)p + 0x50);
    dstAttr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x1C) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dstAttr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}

void ObjectVisual_ReservedNoOp2B90(void)
{
}
