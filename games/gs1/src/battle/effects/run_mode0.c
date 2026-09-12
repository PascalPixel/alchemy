#include "types.h"
#include "scene.h"
#include "b5_context.h"
#include "motion_object.h"

/* battle/effects/runtime/mode/run_mode_0.c */
void BattleFx_RunMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}

/* battle/effects/runtime/mode/run_mode_1.c */
void BattleFx_RunMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}

/* battle/effects/runtime/mode/run_mode_2.c */
void BattleFx_RunMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}

/* battle/effects/runtime/mode/run_mode_3.c */
void BattleFx_RunMode3(s32 arg0)
{
    Battle_Apply(arg0, 3);
}

/* battle/effects/runtime/mode/run_mode_11.c */
void BattleFx_RunMode11(s32 arg0)
{
    Battle_Apply(arg0, 0xB);
}

/* battle/effects/runtime/mode/run_mode_4.c */
void BattleFx_RunMode4(s32 arg0)
{
    Battle_Apply(arg0, 4);
}

/* battle/effects/runtime/mode/run_mode_5_with_action.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Object_SetMode(struct MotionObject *, s32);
s32 Object_SetAction(s32, s32);

void BattleFx_RunMode5WithAction(void *effect)
{
    s32 object;

    object =
        (s32)Battle_Run(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Battle_Apply(effect, 5);
    Object_SetAction(object, 0x10);
}

/* battle/effects/runtime/mode/run_mode_6_with_action.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))


void BattleFx_RunMode6WithAction(void *effect)
{
    s32 object;

    object =
        (s32)Battle_Run(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Battle_Apply(effect, 6);
    Object_SetAction(object, 0x10);
}

/* battle/effects/runtime/mode/run_mode_7.c */
void BattleFx_RunMode7(s32 arg0)
{
    Battle_Apply(arg0, 7);
}

/* battle/effects/runtime/mode/run_mode_10.c */
void BattleFx_RunMode10(s32 arg0)
{
    Battle_Apply(arg0, 0xA);
}

/* battle/effects/runtime/mode/run_mode_9_with_action.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))


void BattleFx_RunMode9WithAction(void *effect)
{
    s32 object;

    object =
        (s32)Battle_Run(FIELD_AT_OFFSET(effect, s32 *, 8))->object;
    Object_SetMode(object, 2);
    Object_SetAction(object, 0x30);
    Battle_Apply(effect, 9);
    Object_SetAction(object, 0x10);
}

/* battle/effects/motion/initialize_rising_object.c */
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);

struct RisingObjectState {
    u8 padding_00[0x28];
    s32 field_28;
    u8 padding_2c[4];
    s32 field_30;
    s32 field_34;
    u8 padding_38[0xc];
    s32 field_44;
    s32 field_48;
    u8 padding_4c[0xc];
    u8 field_58;
    u8 padding_59;
    u8 field_5a;
};

void BattleFx_InitializeRisingObject(s32 arg0)
{
    struct B5Context *context = Battle_Run(arg0);
    struct RisingObjectState *object = context->object;

    object->field_34 = 0x20000;
    object->field_30 = 0x80000;
    object->field_28 = 0x40000;
    object->field_48 = 0xAB85;
    object->field_44 = 0;
    object->field_5a = 0;
    object->field_58 = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, context->word_0c, 0, context->word_10);
    Object_SetMode(object, 1);
}

/* battle/effects/motion/set_approach_motion.c */
s32 FixedPoint_Ratio(s32, s32);

void BattleFx_SetApproachMotion(s32 first, s32 second, s32 divisor)
{
    struct B5Context *first_context = Battle_Run(first);
    struct B5Context *second_context = Battle_Run(second);
    struct MotionObject *object = first_context->object;
    struct MotionObject *target = second_context->object;
    s32 scale = 80;
    s32 diff_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = FixedPoint_Ratio(scale *diff_x, 100);
    s32 diff_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = FixedPoint_Ratio(scale *diff_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(short_x *short_x + short_z *short_z);
    distance = FixedPoint_Ratio(distance << 8, divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    object->vertical_motion_strength = 0xab85;
    object->velocity_y = 0;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
}
