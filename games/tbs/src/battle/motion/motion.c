#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_point_position.h"
#include "types.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "scene.h"
#include "fixed_math.h"
#include "motobj.h"
#include "battle_motion.h"
#include "btldisp.h"
#include "btlsys.h"
#include "field_event.h"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define SceneTransform_ApplyPosition ((void (*)(void *))SceneTransform_ApplyPosition)

void ObjectDispatch_InitFromTable6Far(struct MotionObject *);
s32	 ArcTan2(s32, s32);

void Camera_InitDefaultTransform(void)
{
	struct BattleCamera		 *state = gCameraWork;
	struct FixedPointPosition transfer;

	state->yaw = 192 << 6;
	state->pitch = 254 << 8;
	state->distance = 255 << 17;
	state->pos[0] = 0;
	state->pos[1] = 0;
	state->pos[2] = 0;
	state->follow_pos = 0;
	state->unknown_18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	transfer.x = 0;
	transfer.y = 0;
	transfer.z = state->distance;
	Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

void ObjMotRst(s32 slot_id)
{
	s32					 z;
	s32					 zero;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(slot_id);
	object = slot->object;
	zero = 0;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->velocity_y = zero;
	object->vertical_motion_phase = zero;
	object->auto_face_motion = zero;
	object->snap_to_target = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
	z = slot->anchor_z;
	if (z < 0)
		z += 7;
	object->angle = ArcTan2(z >> 3, slot->anchor_x) - 0x8000;
}

void BattleMotion_SetupEscapeObject(s32 object_id)
{
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->velocity_y = 0x50000;
	object->vertical_motion_strength = 0x7851;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x * 3, 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void BattleMotion_InterpolatePosition(
	struct BtlObj *start_slot, struct BtlObj *end_slot, s32 progress)
{
	s32					 z_step;
	s32					 start_x;
	s32					 x;
	s32					 start_z;
	s32					 end_z;
	struct MotionObject *start;
	struct MotionObject *end;

	start = start_slot->object;
	end = end_slot->object;
	start_x = start->x;
	x = start_x + progress * (end->x - start_x) / 100;
	end_z = end->z;
	start_z = start->z;
	z_step = progress * (end_z - start_z) / 100;
	REG_BLDCNT = 0;
	start->acceleration = 0x20000;
	start->speed_limit = 0x80000;
	start->velocity_y = 0x40000;
	start->vertical_motion_strength = 0xab85;
	start->vertical_motion_phase = 0;
	start->auto_face_motion = 1;
	Object_SetMoveTargetFar(start, x, 0, start_z + z_step);
	ObjectDispatch_ApplyArgumentToChildrenFar(start, 2);
}

void BattleMotion_SetObjectPosition(struct BtlObj *slot)
{
	struct MotionObject *object = slot->object;

	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
	u32					 scaled_x;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x10000;
	object->speed_limit = 0x40000;
	object->velocity_y = 0x30000;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	scaled_x = slot->anchor_x * 3;
	Object_SetMoveTargetFar(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, slot->anchor_z);
}

void BattleMotion_InitializeObject(s32 id)
{
	struct BtlObj		*slot = BtlObjGet(id);
	struct MotionObject *object = slot->object;

	object->acceleration = 128 << 9;
	object->speed_limit = 128 << 11;
	object->velocity_y = 128 << 11;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, Iwram_MulQ16(slot->anchor_x, 0x14ccc), 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
	struct BtlObj		*slot;
	struct MotionObject *object;
	s32					 x;
	s32					 scale;

	slot = BtlObjGet(id);
	object = slot->object;
	if (UnitGetFar(id)->class_id != 0x94)
	{
		object->acceleration = BattleMotion_VariantAcceleration[variant];
		object->speed_limit = BattleMotion_VariantSpeedLimit[variant];
		if (object->y == 0 || variant > 4)
			object->velocity_y = BattleMotion_VariantVelocityY[variant];
		object->vertical_motion_strength = 0x9999;
		object->vertical_motion_phase = 0;
		object->auto_face_motion = 0;
		ObjectDispatch_InitFromTable6Far(object);
		scale = slot->anchor_x;
		x = (scale * BattleMotion_VariantDistancePercent[variant]) / 100;
		Object_SetMoveTargetFar(object, x, 0, slot->anchor_z);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApproachTarget(
	s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y)
{
	struct BtlObj		*actor_slot = BtlObjGet(actor_id);
	struct BtlObj		*target_slot = BtlObjGet(target_id);
	struct MotionObject *object = actor_slot->object;
	struct MotionObject *target = target_slot->object;
	s32					 scale = 75;
	s32					 dx = target->x - object->x;
	s32					 start_x = object->x;
	s32					 step_x = scale * dx / 100;
	s32					 dz = target->z - object->z;
	s32					 start_z = object->z;
	s32					 step_z = scale * dz / 100;
	s32					 x = start_x + step_x;
	s32					 z = start_z + step_z;
	s32					 cell_x = step_x >> 8;
	s32					 cell_z = step_z >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(cell_x * cell_x + cell_z * cell_z);
	dist = (dist << 8) / travel_divisor;
	object->acceleration = dist;
	object->speed_limit = dist;
	object->snap_to_target = 1;

	if (object->motion_flags & 4)
		object->velocity_y = initial_velocity_y;
	object->velocity_y = initial_velocity_y;
	object->vertical_motion_strength = 0xab85;
	object->auto_face_motion = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
	struct MotionObject *object;

	object = BtlObjGet(id)->object;
	ObjectDispatch_InitFromTable6Far(object);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}
