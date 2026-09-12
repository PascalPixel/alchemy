#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "motion_object.h"

/* battle/motion/reset_actor_at_anchor.c */
struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
s32 ArcTan2(s32, s32);

void Actor_ResetMotionAtAnchor(s32 slot_id)
{
    s32 z; s32 zero;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    slot = GetBattleObjectSlot(slot_id);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xAB85;
    object->velocity_y = zero;
    object->vertical_motion_phase = zero;
    object->auto_face_motion = zero;
    object->snap_to_target = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
    z = slot->anchor_z;
    if (z < 0) { z += 7; }
    *(s16 *)((u8 *)object + 6) =
        (s16)(ArcTan2(z >> 3, slot->anchor_x) - 0x8000);
}

/* battle/motion/setup_escape_object.c */
void Object_SetMode(void *, s32);

void BattleMotion_SetupEscapeObject(s32 object_id)
{
    void *slot;
    void *object;

    slot = GetBattleObjectSlot(object_id);
    object = FIELD_AT_OFFSET(slot, void **, 0);
    FIELD_AT_OFFSET(object, s32 *, 0x34) = 0x20000;
    FIELD_AT_OFFSET(object, s32 *, 0x30) = 0x80000;
    FIELD_AT_OFFSET(object, s32 *, 0x28) = 0x50000;
    FIELD_AT_OFFSET(object, s32 *, 0x48) = 0x7851;
    FIELD_AT_OFFSET(object, s32 *, 0x44) = 0;
    FIELD_AT_OFFSET(object, s8 *, 0x5A) = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, FIELD_AT_OFFSET(slot, s32 *, 0xC) * 3, 0, FIELD_AT_OFFSET(slot, s32 *, 0x10));
    Object_SetMode(object, 1);
}

/* battle/motion/interpolate_position.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 gIw;

s32 FixedPoint_Ratio(s32, s32);

void BattleMotion_InterpolatePosition(void **start_slot, void **end_slot, s32 progress)
{
    s32 z_step;
    s32 start_x;
    s32 x;
    s32 start_z;
    s32 end_z;
    void *start;
    void *end;

    start = *start_slot;
    end = *end_slot;
    start_x = FIELD_AT_OFFSET(start, s32, 8);
    x = start_x + FixedPoint_Ratio(progress *(FIELD_AT_OFFSET(end, s32, 8) - start_x), 0x64);
    end_z = FIELD_AT_OFFSET(end, s32, 0x10);
    start_z = FIELD_AT_OFFSET(start, s32, 0x10);
    z_step = FixedPoint_Ratio(progress *(end_z - start_z), 0x64);
    *(s16 *)0x04000050 = 0;
    FIELD_AT_OFFSET(start, s32, 0x34) = 0x20000;
    FIELD_AT_OFFSET(start, s32, 0x30) = 0x80000;
    FIELD_AT_OFFSET(start, s32, 0x28) = 0x40000;
    FIELD_AT_OFFSET(start, s32, 0x48) = 0xAB85;
    FIELD_AT_OFFSET(start, s32, 0x44) = 0;
    FIELD_AT_OFFSET(start, s8, 0x5A) = 1;
    Object_SetPosition(start, x, 0, start_z + z_step);
    Object_SetMode((s32)start, 2);
}

/* battle/motion/set_object_position.c */
struct Target_080b8144 {
    u8 filler0[0x30];
    u32 value30;
    u32 value34;
    u8 filler38[0x0C];
    u32 value44;
    u32 value48;
    u8 filler4c[0x0E];
    u8 value5a;
};

struct Wrapper_080b8144 {
    struct Target_080b8144 *target;
    u8 filler4[8];
    u32 first;
    u32 fourth;
};


void BattleMotion_SetObjectPosition(struct Wrapper_080b8144 *slot)
{
    struct Target_080b8144 *target = slot->target;

    target->value34 = 0x20000;
    target->value30 = 0x80000;
    target->value48 = 0xAB85;
    target->value44 = 0;
    target->value5a = 0;
    Object_SetPosition(target, slot->first, 0, slot->fourth);
}

/* battle/motion/reset_object_at_scaled_anchor.c */

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
    u32 scaled_x;
    void *slot;
    void *object;

    slot = GetBattleObjectSlot(object_id);
    object = FIELD_AT_OFFSET(slot, void **, 0);
    FIELD_AT_OFFSET(object, s32 *, 0x34) = 0x10000;
    FIELD_AT_OFFSET(object, s32 *, 0x30) = 0x40000;
    FIELD_AT_OFFSET(object, s32 *, 0x28) = 0x30000;
    FIELD_AT_OFFSET(object, s32 *, 0x48) = 0x9999;
    FIELD_AT_OFFSET(object, s32 *, 0x44) = 0;
    FIELD_AT_OFFSET(object, s8 *, 0x5A) = 0;
    Object_ResetMotion(object);
    scaled_x = FIELD_AT_OFFSET(slot, s32 *, 0xC) * 3;
    Object_SetPosition(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, FIELD_AT_OFFSET(slot, s32 *, 0x10));
}
