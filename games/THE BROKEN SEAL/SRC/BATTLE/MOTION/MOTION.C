#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_MOTION.H"

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

struct State_080b7f9c {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Local_080b7f9c {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080b7f9c *gCameraWork;

void Object_ResetMotion(struct MotionObject *);
void Object_SetPosition(struct MotionObject *, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
s32 ArcTan2(s32, s32);

u8 *Owner_GetStateFar(s32);
extern s32 BattleMotion_VariantAcceleration[];
extern s32 BattleMotion_VariantSpeedLimit[];
extern s32 BattleMotion_VariantVelocityY[];
extern s32 BattleMotion_VariantDistancePercent[];

void Camera_InitDefaultTransform(void)
{
    struct State_080b7f9c *state = gCameraWork;
    struct Local_080b7f9c transfer;

    state->field36 = 192 << 6;
    state->field34 = 254 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field1c = 0;
    state->field18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->field0c);
    SceneTransform_ApplyYaw(state->field36);
    SceneTransform_ApplyPitch(state->field34);

    transfer.first = 0;
    transfer.second = 0;
    transfer.third = state->field20;
    Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

void Actor_ResetMotionAtAnchor(s32 slot_id)
{
    s32 z;
    s32 zero;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(slot_id);
    object = slot->object;
    zero = 0;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xab85;
    object->velocity_y = zero;
    object->vertical_motion_phase = zero;
    object->auto_face_motion = zero;
    object->snap_to_target = 1;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
    z = slot->anchor_z;
    if (z < 0)
        z += 7;
    object->angle =
        (s16)(ArcTan2(z >> 3, slot->anchor_x) - 0x8000);
}

void BattleMotion_SetupEscapeObject(s32 object_id)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(object_id);
    object = slot->object;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->velocity_y = 0x50000;
    object->vertical_motion_strength = 0x7851;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, slot->anchor_x * 3, 0, slot->anchor_z);
    Object_SetMode(object, 1);
}

void BattleMotion_InterpolatePosition(struct BattleObjectSlot *start_slot,
    struct BattleObjectSlot *end_slot, s32 progress)
{
    s32 z_step;
    s32 start_x;
    s32 x;
    s32 start_z;
    s32 end_z;
    struct MotionObject *start;
    struct MotionObject *end;

    start = start_slot->object;
    end = end_slot->object;
    start_x = start->x;
    x = start_x + Math_Div(progress * (end->x - start_x), 100);
    end_z = end->z;
    start_z = start->z;
    z_step = Math_Div(progress * (end_z - start_z), 100);
    *(s16 *)0x04000050 = 0;
    start->acceleration = 0x20000;
    start->speed_limit = 0x80000;
    start->velocity_y = 0x40000;
    start->vertical_motion_strength = 0xab85;
    start->vertical_motion_phase = 0;
    start->auto_face_motion = 1;
    Object_SetPosition(start, x, 0, start_z + z_step);
    Object_SetMode(start, 2);
}

void BattleMotion_SetObjectPosition(struct BattleObjectSlot *slot)
{
    struct MotionObject *object = slot->object;

    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    object->vertical_motion_strength = 0xab85;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_SetPosition(object, slot->anchor_x, 0, slot->anchor_z);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
    u32 scaled_x;
    struct BattleObjectSlot *slot;
    struct MotionObject *object;

    slot = GetBattleObjectSlot(object_id);
    object = slot->object;
    object->acceleration = 0x10000;
    object->speed_limit = 0x40000;
    object->velocity_y = 0x30000;
    object->vertical_motion_strength = 0x9999;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    scaled_x = slot->anchor_x * 3;
    Object_SetPosition(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, slot->anchor_z);
}

void BattleMotion_InitializeObject(s32 id)
{
    struct BattleObjectSlot *slot = GetBattleObjectSlot(id);
    struct MotionObject *object = slot->object;

    object->acceleration = 128 << 9;
    object->speed_limit = 128 << 11;
    object->velocity_y = 128 << 11;
    object->vertical_motion_strength = 0x9999;
    object->vertical_motion_phase = 0;
    object->auto_face_motion = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object,
                       Iwram_MulQ16(slot->anchor_x, 0x14ccc),
                       0, slot->anchor_z);
    Object_SetMode(object, 5);
}

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    /* FAKEMATCH: Byte indexing preserves the load operand order. */
    s32 offset;
    s32 x;
    s32 *table;
    s32 scale;

    slot = GetBattleObjectSlot(id);
    object = slot->object;
    if (Owner_GetStateFar(id)[0x128] != 0x94) {
        table = BattleMotion_VariantAcceleration;
        offset = variant * sizeof(*table);
        object->acceleration = *(s32 *)((u8 *)table + offset);
        table = BattleMotion_VariantSpeedLimit;
        object->speed_limit = *(s32 *)((u8 *)table + offset);
        if (object->y == 0 || variant > 4) {
            table = BattleMotion_VariantVelocityY;
            object->velocity_y = *(s32 *)((u8 *)table + offset);
        }
        object->vertical_motion_strength = 0x9999;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 0;
        Object_ResetMotion(object);
        scale = slot->anchor_x;
        table = BattleMotion_VariantDistancePercent;
        x = Math_Div(scale * *(s32 *)((u8 *)table + offset), 100);
        Object_SetPosition(object, x, 0, slot->anchor_z);
    }
    Object_SetMode(object, 5);
}

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
    s32 dx = target->x - object->x;
    s32 start_x = object->x;
    s32 step_x = Math_Div(scale * dx, 100);
    s32 dz = target->z - object->z;
    s32 start_z = object->z;
    s32 step_z = Math_Div(scale * dz, 100);
    s32 x = start_x + step_x;
    s32 z = start_z + step_z;
    s32 cell_x = step_x >> 8;
    s32 cell_z = step_z >> 8;
    s32 dist;

    dist = Iwram_Sqrt(
        cell_x * cell_x + cell_z * cell_z);
    dist = Math_Div(dist << 8, travel_divisor);
    object->acceleration = dist;
    object->speed_limit = dist;
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

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
    struct MotionObject *object;

    object = GetBattleObjectSlot(id)->object;
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}
