#include "unit/field/common/fielddat.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field4.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field10.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime9.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

static __inline__ void StoreHalfword(s16 *address, s32 value)
{
	*address = value;
}

void ObjectDispatch_SetUpdateValue(void *raw, s32 value)
{
	struct ObjectRuntime *object = raw;

	if (object != NULL)
		*(s32 *)object->unknown_6c = value;
}

#if EDITION_INTERNATIONAL

#define ASSIGNED_KEYS gKeyState
#else

#define ASSIGNED_KEYS gKeysHeld
#endif

u32 Field_StoreAssignedKeyValue(u32 value)
{
	u32					  selector = value >> 14;
	u32					  request = 0x3FFF & value;
	struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

	if (GameFlag_TestFar(0x107) != 0)
	{
		StoreHalfword((s16 *)&work->raised_trigger, 0xFA);
	}
	else if (work->mode == 3)
	{
		if (ASSIGNED_KEYS & 0x100)
		{
			StoreHalfword((s16 *)&work->raised_trigger, 0xFC88);
		}
		else if (ASSIGNED_KEYS & 0x200)
		{
			StoreHalfword((s16 *)&work->raised_trigger, 0xFC87);
		}
	}
	else
	{
		switch (selector)
		{
		case 0:
			StoreHalfword(&work->psynergy_request, request);
			break;
		case 1:
			StoreHalfword((s16 *)work->unknown_180, request);
			break;
		}
	}

	return request;
}

s32 Field_CheckConfiguredKeys(void)
{
	struct EventRuntime *work = gEventWork;
	s32					 result = 0;

	if (work == NULL)
		return 0;
	if (gKeyState & gGameState.select_keys)
	{
		work->unknown_172[0] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.a_keys)
	{
		work->unknown_172[1] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.start_keys)
	{
		work->unknown_172[2] = 1;
		result = 1;
	}
	else if (gKeyState & gGameState.first_shortcut_keys)
	{
		result = Field_StoreAssignedKeyValue(gGameState.first_shortcut);
	}
	else if (gKeyState & gGameState.second_shortcut_keys)
	{
		result = Field_StoreAssignedKeyValue(gGameState.second_shortcut);
	}
	return result;
}

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
	const struct FieldPosition *first = (const struct FieldPosition *)a;
	const struct FieldPosition *second = (const struct FieldPosition *)b;
	const s32				   *first_word = &first->x;
	const s32				   *second_word = &second->x;
	s32							dx = (*first_word++ - *second_word++) >> 16;
	s32							dy = (*first_word++ - *second_word++) >> 16;
	s32							dz = (*first_word - *second_word) >> 16;
	s32							radius = radius_a + radius_b;

	if (!(dx > 0x400000) && !(dz > 0x400000) && dx * dx + dy * dy + dz * dz < radius * radius)
		return 0;
	return -1;
}

struct WorldMapSprite
{
	struct FieldSprite sprite;
	u8				   unknown_28[4];
	u8				  *shadow;
};

s32 FieldObject_UpdatePlayerControl(struct ObjectRuntime *object)
{
	struct FieldPosition  position;
	struct FieldPosition  test_position;
	struct FieldPosition  contact;
	s16					  angle_offsets[22];
	s32					  collision;
	s32					  pushed;
	s16					  heading;
	s32					  angle;
	s32					  mode;
	u16					  direction;
	s32					  rotation;
	s32					  angle_index;
	s32					  count;
	struct ObjectRuntime *other;

	collision = 0;
	pushed = 0;
	if (gDebugMode != 0 && GameFlag_TestFar(350) != 0)
	{
		s32 free;
		s32 block;

		free = 0;
		for (block = 0; block < 512; block++)
		{
			if (ResourceBlockOwners[block] == 0xff)
				free++;
		}
		if (free - 136 < 0)
			AudioCommand_PlayFar(135);
	}
	if (*(volatile u8 *)&gDebugMode != 0)
	{
		s32 mask;

		mask = KEY_L;
		if (gKeysHeld & mask)
		{
			s32 wait;

			wait = mask;
		wait_a:
			--wait;
			if (wait != 0)
				goto wait_a;
			wait = 95;
		wait_b:
			wait--;
			if (wait >= 0)
				goto wait_b;
			wait = 63;
		wait_c:
			wait--;
			if (wait >= 0)
				goto wait_c;
			wait = 63;
		wait_d:
			wait--;
			if (wait >= 0)
				goto wait_d;
		}
	}
	if (gKeysHeld & gGameState.dash_keys)
	{
		object->speed_limit = 0x18000;
		object->acceleration = 0x4000;
		mode = 5;
	}
	else
	{
		object->speed_limit = 0x10000;
		object->acceleration = 0x4000;
		mode = 2;
	}
	if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & KEY_B))
	{
		object->speed_limit = 0x40000;
		object->acceleration = 0x10000;
		mode = 5;
	}

	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	if ((u16)angle == 0xffff)
	{
		collision |= 4;
		goto update_object;
	}

	collision = 0;
	position.x = object->x;
	position.y = object->y;
	position.z = object->z;
	Vector_AddPolarOffset(0x80000, (u16)angle, &position);
	if (gDebugMode != 0 && (gKeysHeld & KEY_L))
	{
		heading = angle;
		goto update_object;
	}
	if (Map_CheckObjectStep(object, &position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &test_position);
	if (Map_CheckObjectStep(object, &test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &test_position);
	if (Map_CheckObjectStep(object, &test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &test_position);
	if (Map_CheckObjectStep(object, &test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000, &test_position);
	if (Map_CheckObjectStep(object, &test_position) != 0)
		goto choose_direction;
	heading = angle;
	goto push_objects;

choose_direction:
	direction = angle;
	count = 0;
	angle_offsets[count++] = direction + 0x1000;
	angle_offsets[count++] = direction - 0x1000;
	angle_offsets[count++] = direction + 0x2000;
	angle_offsets[count++] = direction - 0x2000;
	angle_offsets[count++] = direction + 0x3000;
	angle_offsets[count++] = direction - 0x3000;
	for (angle_index = 0; angle_index < count; angle_index++)
	{
		heading = angle_offsets[angle_index];
		position.x = object->x;
		position.y = object->y;
		position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)heading, &position);
		if (Map_CheckObjectStep(object, &position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &test_position);
		if (Map_CheckObjectStep(object, &test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &test_position);
		if (Map_CheckObjectStep(object, &test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &test_position);
		if (Map_CheckObjectStep(object, &test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &test_position);
		if (Map_CheckObjectStep(object, &test_position) == 0)
			goto push_objects;
	}
	position.x = object->x;
	position.y = object->y;
	position.z = object->z;
	collision |= 1;

push_objects:
	contact.x = object->x;
	contact.y = object->y;
	contact.z = object->z;
	Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
	other = gObjectSlots;
	for (angle_index = 63; angle_index >= 0; angle_index -= 1, other++)
	{
		s32 radius;

		radius = object->radius - 2;
		if (other->script == 0)
			continue;
		if ((other->unknown_59 & 1) == 0)
			continue;
		if (other == object)
			continue;
		if (Runtime_CheckRadiusOverlap(&other->x, radius, &contact.x, other->radius - 2) < 0)
			continue;
		if ((other->unknown_59 & 2) == 0 || other->movement_state != 0)
			goto blocked;
		direction = ArcTan2(other->z - object->z, other->x - object->x);
		test_position.x = other->x;
		test_position.y = other->y;
		test_position.z = other->z;
		heading = direction;
		Vector_AddPolarOffset(0x4000, direction, &test_position);
		if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)other, (s32 *)&test_position))
			goto blocked;
		test_position.x = other->x;
		test_position.y = other->y;
		test_position.z = other->z;
		Vector_AddPolarOffset(0xa0000, direction, &test_position);
		if (Map_CheckObjectStep(other, &test_position) != 0)
			goto blocked;
		test_position.x = other->x;
		test_position.y = other->y;
		test_position.z = other->z;
		Vector_AddPolarOffset(0xa0000, direction + 0x1000, &test_position);
		if (Map_CheckObjectStep(other, &test_position) != 0)
			goto blocked;
		if (Map_CheckObjectStep(other, &test_position) != 0)
			goto blocked;
		test_position.x = other->x;
		test_position.y = other->y;
		test_position.z = other->z;
		Vector_AddPolarOffset(0xa0000, direction - 0x1000, &test_position);
		if (Map_CheckObjectStep(other, &test_position) != 0)
			goto blocked;
		Vector_AddPolarOffset(0x4000, direction, (struct FieldPosition *)&other->x);
		other->target_x = -0x80000000;
		other->target_y = -0x80000000;
		other->target_z = -0x80000000;
		pushed |= 1;
		continue;
	blocked:
		collision |= 2;
	}
	if (collision == 0 && pushed != 0)
	{
		object->speed_limit = 0x4000;
		object->acceleration = 0x2000;
	}

update_object:
	if (gEventWork != NULL)
	{
		if (collision & 3)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	if (pushed != 0)
	{
		ObjectDispatch_ApplyArgumentToChildren(object, 8);
	}
	else if (collision != 0)
	{
		s32 action;

		action = 9;
		if (UnitGetFar(gGameState.selected_actor)->hp == 0)
			action = 22;
		ObjectDispatch_ApplyArgumentToChildren(object, action);
	}
	else
	{
		ObjectDispatch_ApplyArgumentToChildren(object, mode);
	}

	if (collision != 0)
	{
		object->target_x = -0x80000000;
		object->target_y = -0x80000000;
		object->target_z = -0x80000000;
		object->velocity_x = 0;
		object->velocity_z = 0;
		if (collision & 3)
		{
			s32 difference;

			difference = (u16)angle - object->angle;
			difference = (s16)difference;
			if (difference > 0x1000)
				difference = 0x1000;
			if (difference < -0x1000)
				difference = -0x1000;
			object->angle += difference;
		}
		object->action = 0;
		object->unknown_66 = 2;
	}
	else
	{
		s32 square;

		Object_SetMoveTarget(object, position.x, position.y, position.z);
		square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
						   + Iwram_MulQ16(object->velocity_z, object->velocity_z));
		object->velocity_x = 0;
		object->velocity_z = 0;
		Vector_AddPolarOffset(square, (u16)heading, (struct FieldPosition *)&object->velocity_x);
		if (object->action != 0)
			object->action--;
	}

	rotation = (u16)angle;
	if (((struct MapScrollWork *)gMapWork[0])->footprints != 0 && object->action == 0
		&& collision == 0)
	{
		struct ObjectRuntime *effect;

		effect = FieldObject_Create(25, object->x, object->y, object->z);
		if (effect != 0)
		{
			struct FieldSprite *animation;

			effect->terrain_height = object->terrain_height;
			animation = effect->animation;
			ObjectDispatch_Initialize(
				(struct DispatchObject *)effect, (u32)WorldMap_FootprintActions);
			effect->unknown_23 = 2;
			effect->flags = collision;
			if (animation != 0)
			{
				AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
				animation->flags = collision;
				animation->rotation = rotation + 0x4000;
				animation->priority = 3;
			}
			if (object->unknown_66 == 2)
			{
				AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 2);
				object->unknown_66 = collision;
			}
			if (object->unknown_66 != 0)
				effect->angle = 0x8000;
			if (mode == 5)
				object->action = 12;
			else
				object->action = 18;
			object->unknown_66 ^= 1;
		}
	}
	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

#if EDITION_INTERNATIONAL
#define WORLD_PROBE_RADIUS 0x70000
#else
#define WORLD_PROBE_RADIUS 0x80000
#endif

s32 Object_MoveOnWorldMap(struct ObjectRuntime *object)
{
	struct FieldPosition position;
#if EDITION_INTERNATIONAL
	struct FieldPosition test_position;
	s16					 angle_offsets[28];
#else
	struct FieldPosition left_position;
	struct FieldPosition right_position;
	s16					 angle_offsets[22];
#endif
	s32						collision;
	s32						angle;
	u16						direction;
	s32						angle_index;
	s32						mode;
	s32						count;
	s32						difference;
	s32						collision_kind;
	s32						square;
	s32						rate;
	struct PerspectiveWork *work;
	s32						step;
	struct FieldSprite	   *sprite;
	struct ObjectRuntime   *effect;
	struct FieldSprite	   *animation;

	mode = 2;
	collision = 0;
#if !EDITION_INTERNATIONAL
	count = 0;
#endif
	if (gKeysHeld & gGameState.dash_keys)
	{
		object->speed_limit = 0x10000;
		object->acceleration = 0x14000;
		mode = 5;
	}
	else
	{
		object->speed_limit = 0x8000;
		object->acceleration = 0x4000;
	}
	if ((gKeysRepeat & KEY_L) != 0)
		object->speed_limit = 0x40000;

	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	direction = angle;
	if (direction == 0xffff)
	{
		collision |= 4;
		goto update_object;
	}

	collision = 0;
	position.x = object->x;
	position.y = object->y;
	position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
	if (gDebugMode != 0 && (gKeysHeld & KEY_L) != 0)
		goto update_object;
#if EDITION_INTERNATIONAL
	if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
		goto choose_direction;

	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
	if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
	if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
	if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
		goto choose_direction;
	test_position.x = object->x;
	test_position.y = object->y;
	test_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
	if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
		goto choose_direction;
	goto update_object;

choose_direction:
	count = 0;
	angle_offsets[count++] = direction + 0x1000;
	angle_offsets[count++] = direction - 0x1000;
	angle_offsets[count++] = direction + 0x2000;
	angle_offsets[count++] = direction - 0x2000;
	angle_offsets[count++] = direction + 0x3000;
	angle_offsets[count++] = direction - 0x3000;
	angle_offsets[count++] = direction + 0x4000;
	angle_offsets[count++] = direction - 0x4000;
	for (angle_index = 0; angle_index < count; angle_index++)
	{
		direction = angle_offsets[angle_index];
		position.x = object->x;
		position.y = object->y;
		position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
		if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000, &test_position);
		if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000, &test_position);
		if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000, &test_position);
		if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
			continue;
		test_position.x = object->x;
		test_position.y = object->y;
		test_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000, &test_position);
		if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
			continue;
		angle = (s16)direction;
		goto update_object;
	}
	collision |= 1;

#else
	left_position.x = object->x;
	left_position.y = object->y;
	left_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
	right_position.x = object->x;
	right_position.y = object->y;
	right_position.z = object->z;
	Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
	if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
			| CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
			| CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position))
		== 0)
		goto update_object;

	angle_offsets[count++] = direction + 0x1000;
	angle_offsets[count++] = direction - 0x1000;
	angle_offsets[count++] = direction + 0x2000;
	angle_offsets[count++] = direction - 0x2000;
	angle_offsets[count++] = direction + 0x3000;
	angle_offsets[count++] = direction - 0x3000;
	for (angle_index = 0; angle_index < count; angle_index++)
	{
		direction = angle_offsets[angle_index];
		position.x = object->x;
		position.y = object->y;
		position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
		left_position.x = object->x;
		left_position.y = object->y;
		left_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555, &left_position);
		right_position.x = object->x;
		right_position.y = object->y;
		right_position.z = object->z;
		Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555, &right_position);
		if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
				| CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
				| CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position))
			!= 0)
			continue;
		angle = (s16)direction;
		goto update_object;
	}
	collision |= 1;
#endif

update_object:
	if (gEventWork != NULL)
	{
		if ((collision & 3) != 0)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	if (collision != 0)
		ObjectDispatch_ApplyArgumentToChildren(object, 9);
	else
		ObjectDispatch_ApplyArgumentToChildren(object, mode);

	if (collision != 0)
	{
		object->target_x = -0x80000000;
		object->target_y = -0x80000000;
		object->target_z = -0x80000000;
		object->velocity_x = 0;
		object->velocity_z = 0;
		if ((collision & 3) != 0)
		{
			direction = angle;
			difference = (s16)(direction - object->angle);
			if (difference > 0x1000)
				difference = 0x1000;
			if (difference < -0x1000)
				difference = -0x1000;
			object->angle += difference;
		}
		object->action = 0;
		object->unknown_66 = 2;
		goto movement_done;
	}

	Object_SetMoveTarget(object, position.x, position.y, position.z);
	square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
					   + Iwram_MulQ16(object->velocity_z, object->velocity_z));
	object->velocity_x = collision;
	object->velocity_z = collision;
	Vector_AddPolarOffset(square, (u16)angle, (struct FieldPosition *)&object->velocity_x);
	if (object->action != 0)
		object->action--;

movement_done:
	work = gMapWork[0];
	rate = WorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
	step = (s16)(rate - work->yaw) / 8;
	if (step > 0x200)
		step = 0x200;
	if (step < -0x200)
		step = -0x200;
	if (step > -16 && step < 16)
		step = rate - work->yaw;
	work->yaw += step;

	if (object->animation_kind == 1)
	{
		sprite = object->animation;
		collision_kind = GetWorldMapCollision((struct WorldPosition *)&object->x);
		if (collision_kind == 9)
		{
			((struct WorldMapSprite *)sprite)->shadow[6] = 1;
			sprite->flags = 0;
		}
		else
		{
			((struct WorldMapSprite *)sprite)->shadow[6] = 9;
			sprite->flags = 1;
		}
		if (collision_kind == 6 && object->action == 0 && collision == 0)
		{
			effect = FieldObject_Create(24, object->x, object->y, object->z);
			if (effect != 0)
			{
				animation = effect->animation;
				ObjectDispatch_Initialize(
					(struct DispatchObject *)effect, (u32)(WorldMap_FootprintActions + 12));
				effect->flags = collision;
				effect->terrain_id = 1;
				if (animation != 0)
				{
					AnimationObjects_SelectAnimation((struct AnimationObject *)animation, 1);
					animation->flags = collision;
					animation->blend_mode = 1;
					animation->priority = 2;
				}
				object->action = 10;
			}
		}
	}
	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

s32 Field_CheckConfiguredKeysAndCount(void *work)
{
	struct ObjectRuntime *object = work;

	Field_CheckConfiguredKeys();
	object->step++;
	return 1;
}

s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
	struct FieldPosition pos;
	u8					 unused[68];
	struct MapCell		*from;
	struct MapCell		*to;
	u16					 angle;
	s32					 motion;
	s32					 blocked;
	s32					 delta_z;
	s32					 height;

	object->speed_limit = 0x8000;
	object->acceleration = 0x4000;
	motion = 12;
	angle = WorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
	if (angle == 0xffff)
	{
		blocked = 4;
	}
	else
	{
		motion = 14;
		if ((angle & 0xf000) != 0)
		{
			motion = 15;
			if ((angle & 0xf000) != 0x8000)
				motion = 10;
		}
		blocked = 0;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		Vector_AddPolarOffset(0x80000, angle, &pos);
		pos.x += object->x;
		delta_z = pos.z;
		if (delta_z < 0)
			object->angle = 0xc000;
		if (delta_z > 0)
			object->angle = 0x4000;
		pos.y = object->y - pos.z;
		pos.z = object->z;
		from =
			&((struct MapCell *)Ram_MapCellBuffer)[object->x / 0x100000 + (pos.z / 0x100000) * 128];
		to = &((struct MapCell *)Ram_MapCellBuffer)[pos.x / 0x100000 + (pos.z / 0x100000) * 128];
		if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0
			|| from->collision_code != to->collision_code)
		{
			blocked = 4;
			motion = 12;
		}
		else if (gKeysHeld & KEY_UP)
		{
			height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z - 0x100000);
			if (height - object->y < 0x100000)
				goto refused;
		}
		else if (gKeysHeld & KEY_DOWN)
		{
			height = Map_GetTerrainHeight(object->terrain_id, pos.x, pos.z);
			if (height - object->y > -0x80000)
			{
			refused:
				blocked = 1;
				motion = 12;
			}
		}
	}
	if (gEventWork != NULL)
	{
		if (blocked & 3)
			gEventWork->blocked_steps++;
		else
			gEventWork->blocked_steps = 0;
	}
	ObjectDispatch_ApplyArgumentToChildren(object, motion);
	if (blocked != 0)
	{
		object->target_x = 0x80000000;
		object->target_y = 0x80000000;
		object->target_z = 0x80000000;
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
	}
	else
	{
		Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
	}
	object->step++;
	return 1;
}
