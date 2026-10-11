#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_status_icon.h"
#include "iwram_call.h"
#include "motobj.h"
#include "animspr.h"
#include "metadata_lookup.h"
#include "fixed_point_position.h"
#include "global_cells.h"
#include "scene.h"
#include "btlwork.h"
#include "btldisp.h"
#include "btlsys.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "field_event.h"
#include "effect_step.h"

void BattleMotion_ReservedNoOp83B0();

#define BattlePres_SetActorModeAndAction ((void (*)(s32))BattlePres_SetActorModeAndAction)

#define gMenuCtrlWork (*(u8 **)&gMenuCtrlWork)

struct PairedObjectList
{
	u8						padding0[8];
	struct AnimationObject *objects[1];
};

struct PairedObjectTable
{
	u8	padding0[0x18];
	s32 count;
};

extern volatile u8 gSchedulerStatus;

void WaitFrames(s32 frames);

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
	s32						 i;
	s32						 id;
	struct BtlObj			*slot;
	struct MotionObject		*object;
	struct AnimationObject	*res;
	struct SpriteEntry		*entry;
	struct AnimationObject **objects;
	u8						*table;
	s32						 resource;
	s32						 object_id;

	for (i = 0; i <= 13; i++)
	{
		if (!BattlePlacement_ContainsId(list, i))
		{
			object_id = i + 120;
			if (i <= 7)
				object_id = i;
			ReleaseBattleObjectRecords(object_id);
		}
	}
	if (gSchedulerStatus == 0)
		WaitFrames(1);
	for (i = 0; i <= 13 && (id = list[i]) != 255; i++)
	{
		if (id == 254)
			continue;
		slot = BtlObjGet(id);
		if (slot == 0)
			continue;
		BattleUnit_BuildStatusFlags(id, slot);
		object = slot->object;
		if (object == 0)
			continue;
		if (object->record_storage_kind != 0)
			continue;
		if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483)
		{
			table = gMenuCtrlWork + ((struct PairedObjectTable *)gMenuCtrlWork)->count * 4;
			objects = ((struct PairedObjectList *)table)->objects;
			resource = slot->resource;
			object->record_storage_kind = 2;
			object->records = objects;
			Iwram_ClearWords(objects, 16);
			res = ResourceObject_CreateFar(resource);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				object->height = Resource_GetMetadataRecordFar(resource)->box_y >> 1;
				*objects = res;
				objects = &((struct PairedObjectList *)table)->objects[1];
			}
			res->flags = 0;
			res = ResourceObject_CreateFar(resource + 0x2001);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				*objects = res;
			}
			res->flags = 0;
		}
		else
		{
			res = ResourceObject_CreateFar(slot->resource);
			if (res != 0)
			{
				object->record_storage_kind = 1;
				object->records = res;
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				entry = (struct SpriteEntry *)res->entries[0];
				entry->priority = 1;
				entry->palette = slot->palette;
				if ((resource = slot->overlay) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					entry->priority = 1;
				}
				if ((resource = slot->animation) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->animation_entry = entry;
					Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
					entry->priority = 3;
				}
				if ((resource = slot->effect) != 0)
				{
					if (res->width == 32)
						resource = 0x1ff;
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->effect_entry = entry;
					entry->priority = 0;
					res->flags = 0;
				}
			}
		}
		BattlePres_SetActorModeAndAction(id);
	}
	if (refresh)
	{
		i = 0;
		if ((resource = list[0]) != 255)
		{
			for (;;)
			{
				if (list[i] != 254 && (slot = BtlObjGet(resource)) != 0 && (object = slot->object) != 0)
					BattlePres_SetActorModeAndAction(resource);
				i++;
				if (i >= 14 || (resource = list[i]) == 255)
					break;
			}
		}
	}
}

s32 ObjectDispatch_ReleaseFar(s32);

s32 Graphics_PrepareTransferAndRun(void *, void *);
s32 Graphics_PrepareTransferInIwramWork(void *, void *);

s32 Render_ProjectPoint(const s32 *, s32 *);

struct BtlObj *BtlObjGet(s32 object_id)
{
	struct BtlWork *work = BtlWk;
	struct BtlObj  *slots = work->slots;
	s32				index;
	if (object_id > 7)
	{
		object_id -= 0x78;
	}
	index = object_id;
	if (work->placement[index] == 0xFF)
	{
		return NULL;
	}
	return &slots[work->placement[index]];
}

void ResetMotionRecordGroup(void *owner)
{
	struct AnimationObject *object = owner;
	s32						i;

	if (object != NULL)
	{
		for (i = 0; i < 4; i++)
		{
			if (object->entries[i] != NULL)
				object->entries[i]->script = NULL;
		}
	}
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
	s32	   storage_kind;
	s32	   remaining;
	void **record_groups;
	void  *record_group;

	if (object != NULL)
	{
		storage_kind = object->record_storage_kind & 0xF;
		switch (storage_kind)
		{
		case 1:
			ResetMotionRecordGroup(object->records);
			return;
		case 2:
			record_groups = object->records;
			remaining = 3;
			do
			{
				record_group = *record_groups++;
				if (record_group != NULL)
				{
					ResetMotionRecordGroup(record_group);
				}
				remaining -= 1;
			} while (remaining >= 0);
			break;
		}
	}
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
	struct BtlObj *slot;
	BattleMotion_ReleaseObjectSlotByValue(object_id);
	slot = BtlObjGet(object_id);
	slot->active = 1;
	return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
	// 戻り値の型はポインタだが return 文は無い（pop {r1}; bx r1）
	s32			   no;
	s32			   i;
	struct BtlObj *slot;

	i = 0;
	do
	{
		no = i + 0x78;
		if (i <= 7)
		{
			no = i;
		}
		slot = BtlObjGet(no);
		if ((slot != NULL) && (slot->active != 0))
		{
			ObjectDispatch_ReleaseFar((s32)slot->object);
			slot->object = NULL;
			slot->active = 0;
		}
		i += 1;
	} while (i <= 0xD);
}

s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
	struct BtlWork *state = BtlWk;

	out->x = state->slots[no].anchor_x;
	out->y = 0;
	out->z = state->slots[no].anchor_z;
	return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
	struct BattleCamera *state = gCameraWork;
	Render_ResetTransformState();
	if (GameFlag_TestFar(0x16B) != 0)
	{
		Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
		return Graphics_PrepareTransferAndRun(state, state->pos);
	}
	else
	{
		return Graphics_PrepareTransferInIwramWork(state, state->pos);
	}
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						position[3];
	s32						scaled;

	Camera_ApplyTransformByFlag();
	position[0] = object->x;
	position[1] = object->y;
	position[2] = object->z;
	scaled = Render_ProjectPoint(position, projected);
	(void)Iwram_MulQ16(scaled, record->scale);
	return 0;
}

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
	s32 storage_kind = object->record_storage_kind & 0xF;
	if (storage_kind == 1)
	{
		if (record_index == 0)
		{
			return object->records;
		}
	}
	else if (storage_kind == 2)
	{
		return ((void **)object->records)[record_index];
	}
	return NULL;
}

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

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
	struct BtlObj		*second_slot;
	struct MotionObject *first;
	struct MotionObject *second;
	s32					 x1;
	s32					 x2;
	s32					 z1;
	s32					 z2;
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} pos;

	first = BtlObjGet(first_id)->object;
	second_slot = BtlObjGet(second_id);
	second = second_slot->object;

	x1 = first->target_x;
	if (x1 == 0x80000000)
	{
		x1 = first->x;
	}
	z1 = first->target_z;
	if (z1 == 0x80000000)
	{
		z1 = first->z;
	}
	x2 = second->target_x;
	if (x2 == 0x80000000)
	{
		x2 = second->x;
	}
	z2 = second->target_z;
	if (z2 == 0x80000000)
	{
		z2 = second->z;
	}

	pos.x = (x1 + x2) / 2;
	pos.y = 0;
	pos.z = (z1 + z2) / 2;

	BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

void BattleMotion_SpawnSlotEffectAndWait(s32 id)
{
	struct MotionObject	  *object;
	struct AnimationEntry *effect;
	s32					   entry;

	object = BtlObjGet(id)->object;
	if ((object != NULL) && ((0xF & object->record_storage_kind) == 1))
	{
		entry = ResourceMetadata_RegisterFar((struct AnimationObject *)object->records, 0x11b);
		if (entry != 0)
		{
			effect = (struct AnimationEntry *)entry;
			Animation_SetWorkEntryFar(effect, 1);
			effect->priority = 3;
		}
		WaitFrames(0xA);
	}
}

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;
	/* FAKEMATCH: needed to match: the input position BattleMotion_ProjectPosition fills, unused here */
	struct EffectPosition input;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
	projected[1] -= scaled;
	return 0;
}

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	if (Summon_IsEntryFlagged(UnitGetFar(id)->class_id) != 0)
		scaled = Iwram_MulQ16(factor, 24);
	else
		scaled = Iwram_MulQ16(factor, 48);
	projected[1] -= scaled;
	return 0;
}

u32 Battle_GetObjectTableValue(s32 id)
{
	u32 value;
	u8	no;

	no = UnitGetFar(id)->class_id;
	value = (u8)Summon_GetEntryByte4(no) << 16;
	if (value == 0)
	{
		no = UnitGetFar(id)->class_id;
		if (Summon_IsEntryFlagged(no) != 0)
		{
			value = 0x180000;
		}
		else
		{
			value = 0x300000;
		}
	}
	return value;
}
