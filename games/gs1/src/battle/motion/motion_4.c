#include "types.h"
#include "scene.h"
#include "motion_object.h"
#include "battle_motion.h"

/* battle/motion/apply_variant_motion.c */
struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
u8 *Runtime_GetObject(s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 FixedPoint_Ratio(s32, s32);

extern s32 gRom[];
extern s32 gRom2[];
extern s32 gRom3[];
extern s32 gRom4[];

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 index;
    s32 x;
    s32 *table;
    s32 scale;

    slot = GetBattleObjectSlot(id);
    object = slot->object;
    if (Runtime_GetObject(id)[0x128] != 0x94) {
        table = gRom;
        index = variant * 4;
        object->acceleration = *(s32 *)((u8 *)table + index);
        table = gRom2;
        object->speed_limit = *(s32 *)((u8 *)table + index);
        if (object->y == 0 || variant > 4) {
            table = gRom3;
            object->velocity_y = *(s32 *)((u8 *)table + index);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Object_ResetMotion(object);
        scale = slot->anchor_x;
        table = gRom4;
        x = FixedPoint_Ratio(scale * *(s32 *)((u8 *)table + index), 100);
        Object_SetPosition(object, x, 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}

/* battle/motion/approach_target.c */
struct BattleObjectSlot *GetBattleObjectSlot(s32);
s32 FixedPoint_Ratio(s32, s32);
void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);

void BattleMotion_ApproachTarget(
    s32 actor_id,
    s32 target_id,
    s32 travel_divisor,
    s32 initial_velocity_y
)
{
    struct BattleObjectSlot *actor_slot = GetBattleObjectSlot(actor_id);
    struct BattleObjectSlot *target_slot = GetBattleObjectSlot(target_id);
    struct MotionObject *object = actor_slot->object;
    struct MotionObject *target = target_slot->object;
    s32 scale = 75;
    s32 difference_x = target->x - object->x;
    s32 start_x = object->x;
    s32 delta_x = FixedPoint_Ratio(scale *difference_x, 100);
    s32 difference_z = target->z - object->z;
    s32 start_z = object->z;
    s32 delta_z = FixedPoint_Ratio(scale *difference_z, 100);
    s32 x = start_x + delta_x;
    s32 z = start_z + delta_z;
    s32 short_x = delta_x >> 8;
    s32 short_z = delta_z >> 8;
    s32 distance;

    distance = ((s32 (*)(s32))0x030001d8)(
        short_x *short_x + short_z *short_z);
    distance = FixedPoint_Ratio(distance << 8, travel_divisor);
    object->acceleration = distance;
    object->speed_limit = distance;
    object->snap_to_target = 1;
    if (object->motion_flags & 4)
        object->velocity_y = initial_velocity_y;
    object->velocity_y = initial_velocity_y;
    object->vertical_motion_strength = 0xab85;
    object->auto_face_motion = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
}

/* battle/motion/reset_slot_object_mode2.c */
s32 Object_SetMode(s32, s32);
s32 Object_ResetMotion(s32);
s32 *GetBattleObjectSlot();

void BattleMotion_ResetSlotObjectMode2(void)
{
    s32 object;

    object = *GetBattleObjectSlot();
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

/* battle/motion/reserved_no_op.c */
void BattleMotion_ReservedNoOp83B0(void)
{
}

/* battle/motion/apply_pair_midpoint.c */
struct BattleObjectSlot *GetBattleObjectSlot(s32);
void BattleMotion_ReservedNoOp83B0(void *, s32);

void BattleMotion_ApplyPairMidpoint(s32 arg0, s32 arg1)
{
    struct BattleObjectSlot *second_slot;
    struct MotionObject *obj1;
    struct MotionObject *obj2;
    s32 a1;
    s32 a2;
    s32 b1;
    s32 b2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    obj1 = GetBattleObjectSlot(arg0)->object;
    second_slot = GetBattleObjectSlot(arg1);
    obj2 = second_slot->object;

    a1 = obj1->target_x;
    if (a1 == (s32)0x80000000) {
        a1 = obj1->x;
    }
    b1 = obj1->target_z;
    if (b1 == (s32)0x80000000) {
        b1 = obj1->z;
    }
    a2 = obj2->target_x;
    if (a2 == (s32)0x80000000) {
        a2 = obj2->x;
    }
    b2 = obj2->target_z;
    if (b2 == (s32)0x80000000) {
        b2 = obj2->z;
    }

    pos.x = (a1 + a2) / 2;
    pos.y = 0;
    pos.z = (b1 + b2) / 2;

    BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

/* battle/motion/spawn_slot_effect_and_wait.c */
s32 WaitFrames(s32);
void *Battle_Run(s32, s32);

void **GetBattleObjectSlot();

void BattleMotion_SpawnSlotEffectAndWait(void)
{
    void *object;
    void *effect;

    object = *GetBattleObjectSlot();
    if ((object != NULL) && ((0xF & FIELD_AT_OFFSET(object, u8 *, 0x54)) == 1)) {
        effect = Battle_Run(FIELD_AT_OFFSET(object, s32 *, 0x50), 0x11B);
        if (effect != NULL) {
            Battle_Apply(effect, 1);
            FIELD_AT_OFFSET(effect, s8 *, 6) = 3;
        }
        WaitFrames(0xA);
    }
}
