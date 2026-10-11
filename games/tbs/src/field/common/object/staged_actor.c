#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "mapcopy.h"
#include "staged_actor.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "call.h"
#include "fieldrun.h"
#include "map.h"
#include "map_scroll.h"
#include "field_event.h"

extern u8 gMapCellBuffer[];

extern s16 StagedActor_DirectionSteps[];
extern s32 StagedActor_FootprintBounds[];

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin)
{
	struct FieldStepWork  *work = (struct FieldStepWork *)gEventWork;
	struct ObjectRuntime **slots = work->actors;
	u32					   i;

	for (i = 8; i <= 65; i++)
	{
		struct StagedActor *actor = (struct StagedActor *)slots[i];

		if ((position[0] >> 20) == (actor->x.value >> 20)
			&& (position[1] / 0x10000) == (actor->y / 0x10000)
			&& (position[2] >> 20) == (actor->z.value >> 20))
		{
			return actor;
		}
	}
	return 0;
}

// 向きごとの歩幅の表は s16 の組。ここでは向き一つを一語 (上位が x、下位が z) として読む
struct DirectionSteps
{
	s32 xz[16];
};

#define DIRECTION_STEP(heading) (((struct DirectionSteps *)StagedActor_DirectionSteps)->xz[heading])

void StagedActor_AdvancePair(void)
{
	/* FAKEMATCH: DIRECTION_STEP reads the s16 pair table as one word per heading through an
	   array reference; through a plain s32 pointer cast the address is summed once into a
	   register and 41 instructions differ. */
	s32					dst[3];
	struct StagedActor *lead;
	struct StagedActor *next;
	struct StagedActor *blocker;
	s32					dir;
	u32					step;
	s32					rate;

	lead = (struct StagedActor *)Object_GetById(0);
	dir = lead->direction_and_kind >> 12;
	step = DIRECTION_STEP(dir);
	dst[0] = lead->x.value + (step & 0xffff0000);
	dst[1] = lead->y;
	step <<= 16;
	dst[2] = lead->z.value + step;
	next = StagedActor_FindAtTile(dst, lead);
	if (next == 0)
		return;

	step = DIRECTION_STEP(dir);
	dst[0] = next->x.value + (step & 0xffff0000);
	dst[1] = next->y;
	step <<= 16;
	dst[2] = next->z.value + step;
	blocker = StagedActor_FindAtTile(dst, next);
	if (blocker != 0 && (blocker->collision_flags & 1) != 0)
		return;

	dst[0] = next->x.value;
	dst[1] = next->y + 0x100000;
	dst[2] = next->z.value;
	blocker = StagedActor_FindAtTile(dst, next);
	if (blocker != 0 && (blocker->collision_flags & 1) != 0)
		return;

	next->transition_mode = 2;
	step = DIRECTION_STEP(dir);
	dst[0] = next->x.value + (step & 0xffff0000);
	dst[1] = next->y;
	step <<= 16;
	dst[2] = next->z.value + step;
	if (Object_CheckMovementCollision(next, dst) > 0)
		return;

	if (next->transition_busy != 0)
		return;

	Object_SetMode((struct FieldActor *)lead, 8);
	rate = 0x3333;
	Engine_TaskWait(15);
	Engine_AudioPlayCue(185);
	next->move_rate_x = rate;
	next->move_rate_z = rate;
	Engine_ObjectSetPosition((struct FieldActor *)next, dst[0], dst[1], dst[2]);
	lead->move_rate_x = rate;
	lead->move_rate_z = rate;
	Engine_ObjectSetPosition((struct FieldActor *)lead, dst[0], dst[1], dst[2]);
	Object_CommitPosition(next);
	BattleFx_PlayQueuedSound();
	next->x.value = dst[0];
	next->z.value = dst[2];
	next->velocity_x = 0;
	next->velocity_z = 0;
	lead->target_x = 0x80000000;
	lead->target_z = 0x80000000;
	lead->velocity_x = 0;
	lead->velocity_z = 0;
	lead->x.value = lead->x.parts.cell << 16;
	lead->z.value = lead->z.parts.cell << 16;
	Object_SetMode((struct FieldActor *)lead, 1);
}

s32 StagedActor_FillGridAttributeRectangle(
	u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
	struct MapState *map = gMapWork[0];
	struct MapCell	*cells;
	u32				 row;
	u32				 column;

	if (map != 0)
	{
		if (layer <= 2)
		{
			cells = map->layers[layer].cells;
		}
		else
		{
			cells = (struct MapCell *)gMapCellBuffer;
		}
		cells += x + (z << 7);
		for (row = 0; row < height; row++)
		{
			struct MapCell *cell = cells + (row << 7);

			for (column = 0; column < width; column++)
			{
				cell->collision_code = value;
				cell++;
			}
		}
	}
	return 0;
}

s32 StagedActor_StopBlockedMotion(struct StagedActor *actor)
{
	/* FAKEMATCH: DIRECTION_STEP reads the s16 pair table as one word per heading through an
	   array reference; through a plain s32 pointer cast the address is summed once into a
	   register and 6 instructions differ. */
	struct StagedActorProbePoint probe;
	u32							 dir;
	s32							 step;
	struct StagedActor			*target;

	dir = actor->direction_and_kind >> 12;
	step = DIRECTION_STEP(dir);
	probe.x = actor->x.value + (step & 0xffff0000);
	probe.y = actor->y;
	step <<= 16;
	probe.z = actor->z.value + step;
	target = StagedActor_FindAtTile((s32 *)&probe, actor);
	if (target != 0)
	{
		u32	 i = 0;
		s32	 kind = target->animation->entries[0]->anim_id;
		s32 *kinds = StagedActor_FootprintKinds;

		do
		{
			if (kind == *kinds++)
				goto done;
			i++;
		} while (i <= 5);
		actor->velocity_x = 0;
		actor->velocity_z = 0;
		actor->target_x = 0x80000000;
		actor->target_z = 0x80000000;
	}
	step = DIRECTION_STEP(dir);
	probe.x = actor->x.value + (step & 0xffff0000);
	probe.y = actor->y;
	step <<= 16;
	probe.z = actor->z.value + step;
	if (Object_CheckMovementCollision(actor, (s32 *)&probe) > 0)
	{
		actor->velocity_x = 0;
		actor->velocity_z = 0;
		actor->target_x = 0x80000000;
		actor->target_z = 0x80000000;
	}
done:
	return 0;
}

u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint)
{
	/* FAKEMATCH: DIRECTION_STEP reads the s16 pair table as one word per heading through an
	   array reference; through a plain s32 pointer cast the address is summed once into a
	   register and 33 instructions differ. */
	struct FieldStepWork  *work = (struct FieldStepWork *)gEventWork;
	struct StagedActor	  *player = (struct StagedActor *)Object_GetById(0);
	struct ObjectRuntime **list;
	u32					   i, j;

	*direction = player->direction_and_kind >> 12;
	list = work->actors + 8;
	for (i = 8; i <= 65; list++, i++)
	{
		struct StagedActor *actor = (struct StagedActor *)*list;
		s32					kind;
		j = 0;
		kind = actor->animation->entries[0]->anim_id;
		for (j = 0; j < 6; j++)
		{
			if (kind == StagedActor_FootprintKinds[j])
			{
				s32 x, z, left, top, right, bottom;
				*footprint = j;
				x = ((player->x.value >> 16) + (DIRECTION_STEP(*direction) >> 16)) >> 4;
				z = ((player->z.value >> 16) + (s16)DIRECTION_STEP(*direction)) >> 4;
				left = (actor->x.parts.cell + StagedActor_FootprintBounds[j * 4 + 0]) >> 4;
				top = (actor->z.parts.cell + StagedActor_FootprintBounds[j * 4 + 1]) >> 4;
				right = (actor->x.parts.cell + StagedActor_FootprintBounds[j * 4 + 2]) >> 4;
				bottom = (actor->z.parts.cell + StagedActor_FootprintBounds[j * 4 + 3]) >> 4;
				if (left <= x && x < right && top <= z && z < bottom)
				{
					if (j & 1)
					{
						if (left != (player->x.value >> 20))
						{
							*slot = i;
							return (u8 *)actor;
						}
					}
					else if (top != (player->z.value >> 20))
					{
						*slot = i;
						return (u8 *)actor;
					}
				}
			}
		}
	}
	return NULL;
}

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe)
{
	/* FAKEMATCH: DIRECTION_STEP reads the s16 pair table as one word per heading through an
	   array reference; through a plain s32 pointer cast the address is summed once into a
	   register and 47 instructions differ. */
	s32					direction;
	s32					position[3];
	struct StagedActor *actor;
	s32					steps;
	s32					width, height;
	u8				   *mode;
	s32					column, row;
	s32					near_edge, far_edge;
	s32					offset;

	probe->callback = 0;
	actor = (struct StagedActor *)FieldScene_FindActorRegion(
		&direction, &probe->actor_slot, &probe->footprint_index);
	if (actor == 0)
	{
		return 0;
	}
	mode = &actor->transition_mode;
	*mode = 2;
	{
		s32 footprint = probe->footprint_index;

		steps = 0;
		offset = footprint * 4;
		near_edge = StagedActor_FootprintBounds[offset + 1];
		if (near_edge < 0)
		{
			near_edge = -near_edge;
		}
		far_edge = StagedActor_FootprintBounds[offset + 3];
		if (far_edge < 0)
		{
			far_edge = -far_edge;
		}
		height = (near_edge + far_edge) >> 4;
		near_edge = StagedActor_FootprintBounds[offset];
		if (near_edge < 0)
		{
			near_edge = -near_edge;
		}
		far_edge = StagedActor_FootprintBounds[offset + 2];
		if (far_edge < 0)
		{
			far_edge = -far_edge;
		}
		width = (near_edge + far_edge) >> 4;
	}
	position[0] = actor->x.value + (DIRECTION_STEP(direction) & 0xffff0000);
	{
		s32 y = actor->y;

		position[1] = y;
		position[2] = actor->z.value + (DIRECTION_STEP(direction) << 16);
		probe->position_y = y;
	}
	for (;;)
	{
		probe->position_z =
			position[2] + (StagedActor_FootprintBounds[probe->footprint_index * 4 + 1] << 16);
		for (row = 0; row < height; row++)
		{
			probe->position_x =
				position[0] + (StagedActor_FootprintBounds[probe->footprint_index * 4] << 16);
			for (column = 0; column < width; column++)
			{
				if (Object_CheckMovementCollision(actor, &probe->position_x) == 2)
				{
					goto found;
				}
				probe->position_x += 0x100000;
			}
			probe->position_z += 0x100000;
		}
		steps++;
		position[0] += DIRECTION_STEP(direction) & 0xffff0000;
		position[2] += DIRECTION_STEP(direction) << 16;
	}
found:
	*mode = 0;
	if (steps == 0)
	{
		return 0;
	}
	probe->position_x = actor->x.value + (DIRECTION_STEP(direction) & 0xffff0000) * steps;
	probe->position_y = actor->y;
	probe->position_z = actor->z.value + (DIRECTION_STEP(direction) << 16) * steps;
	return 1;
}

void SceneActor_MoveAndRedraw(struct StagedActorProbe probe)
{
	/* FAKEMATCH: DIRECTION_STEP reads the s16 pair table as one word per heading through an
	   array reference; through a plain s32 pointer cast the address is summed once into a
	   register and 4 instructions differ. */
	/* FAKEMATCH: needed to match: the integer cell read keeps incoming probe spills before the cache load. */
	struct MapScrollWork		   *workspace;
	struct StagedActor			   *actor;
	struct StagedActorProbePosition original_position;
	struct StagedActorProbePosition tile_position;
	u8							   *footprint_table;
	s32								direction;
	s32								horizontal_extent;
	s32								vertical_extent;

	workspace = (struct MapScrollWork *)*(u32 *)gMapWork;
	direction = ((struct StagedActor *)Object_GetById(0))->direction_and_kind >> 12;
	actor = (struct StagedActor *)Object_GetById(probe.actor_slot);
	footprint_table = (u8 *)StagedActor_FootprintBounds;
	{
		s32 extent_a = StagedActor_FootprintBounds[probe.footprint_index * 4 + 1];
		s32 extent_b;

		if (extent_a < 0)
			extent_a = -extent_a;
		extent_b = StagedActor_FootprintBounds[probe.footprint_index * 4 + 3];
		if (extent_b < 0)
			extent_b = -extent_b;
		vertical_extent = (extent_a + extent_b) >> 4;
		extent_a = StagedActor_FootprintBounds[probe.footprint_index * 4 + 0];
		if (extent_a < 0)
			extent_a = -extent_a;
		extent_b = StagedActor_FootprintBounds[probe.footprint_index * 4 + 2];
		if (extent_b < 0)
			extent_b = -extent_b;
		horizontal_extent = (extent_a + extent_b) >> 4;
	}

	actor->move_rate_x = 0x8000;
	actor->move_rate_z = 0x1999;
	original_position.x = actor->x.value;
	original_position.z = actor->z.value;
	{
		s32 table_offset = probe.footprint_index << 4;
		tile_position.x = (actor->x.value + (*(s32 *)(footprint_table + table_offset) << 16));
		table_offset += 4;
		tile_position.z = (actor->z.value + (*(s32 *)(footprint_table + table_offset) << 16));
		tile_position.x >>= 20;
		tile_position.z >>= 20;
	}

	StagedActor_FillGridAttributeRectangle(
		0, tile_position.x, tile_position.z, horizontal_extent, vertical_extent, 0);
	Engine_ActorSetSpeed(0, 0x8000, 0x1999);
	Engine_ActorSetAnimation(0, 8);
	Engine_EventWait(15);

	{
		s32 horizontal_delta = probe.position_x - original_position.x;
		if (horizontal_delta < 0)
			horizontal_delta += 0x1ffff;
		horizontal_delta >>= 17;
		{
			s32 vertical_delta = probe.position_z - original_position.z;
			if (vertical_delta < 0)
				vertical_delta += 0x1ffff;
			vertical_delta >>= 17;
			Engine_ActorSetDestinationOffset(0, horizontal_delta, vertical_delta);
		}
	}

	((struct StagedActor *)Object_GetById(0))->movement_callback =
		(u32)StagedActor_StopBlockedMotion;
	Engine_EventWait(4);
	if ((u32)(direction - 6) <= 7)
		Object_SetMode((struct FieldActor *)actor, 3);
	else
		Object_SetMode((struct FieldActor *)actor, 2);

	Engine_AudioPlayCue(239);
	Engine_ObjectSetPosition(
		(struct FieldActor *)actor, probe.position_x, probe.position_y, probe.position_z);
	Engine_ActorWaitForMove(0);
	Engine_ActorSetAnimation(0, 2);
	Actor_SetSpeed(0, 0x4ccc, 0x1999);

	{
		s32 packed_orientation = DIRECTION_STEP(direction);
		Engine_ActorSetDestinationOffset(
			0, ((s16)(packed_orientation >> 16)) / 2, ((s16)packed_orientation) / 2);
	}
	if (probe.callback != 0)
		probe.callback();

	Engine_ActorWaitForMove(0);
	Engine_ActorSetAnimation(0, 1);
	((struct StagedActor *)Object_GetById(0))->movement_callback = 0;
	Object_CommitPosition(actor);
	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(213);

	actor->x.value = probe.position_x;
	actor->z.value = probe.position_z;
	actor->velocity_x = 0;
	actor->velocity_z = 0;
	Object_SetMode((struct FieldActor *)actor, 1);

	{
		u8 *table = (u8 *)StagedActor_FootprintBounds;
		s32 horizontal_origin;
		s32 vertical_origin;

		{
			s32 table_offset = probe.footprint_index << 4;
			probe.position_x = (probe.position_x + (*(s32 *)(table + table_offset) << 16));
			table_offset += 4;
			probe.position_z = (probe.position_z + (*(s32 *)(table + table_offset) << 16));
			probe.position_x >>= 20;
			probe.position_z >>= 20;
		}
		horizontal_origin = workspace->layers[1].offset_x >> 20;
		vertical_origin = workspace->layers[1].offset_y >> 20;

		Map_CopyCellAttributes(probe.position_x, probe.position_z, horizontal_extent,
			vertical_extent, horizontal_origin + probe.position_x,
			vertical_origin + probe.position_z);
		StagedActor_FillGridAttributeRectangle(
			0, probe.position_x, probe.position_z, horizontal_extent, vertical_extent, 255);
		StagedActor_FillGridAttributeRectangle(
			2, probe.position_x, probe.position_z, horizontal_extent, vertical_extent, 255);

		{
			s32 table_offset = probe.footprint_index << 4;
			original_position.x = (original_position.x + (*(s32 *)(table + table_offset) << 16));
			table_offset += 4;
			original_position.z = (original_position.z + (*(s32 *)(table + table_offset) << 16));
			original_position.x >>= 20;
			original_position.z >>= 20;
		}
		horizontal_origin += original_position.x;
		vertical_origin += original_position.z;

		Engine_MapCopyCellAttributes(horizontal_origin, vertical_origin, horizontal_extent,
			vertical_extent, original_position.x, original_position.z);
		StagedActor_FillGridAttributeRectangle(
			2, original_position.x, original_position.z, horizontal_extent, vertical_extent, 0);
	}
	BattleFx_PlayQueuedSound();
}

s32 FieldScene_RedrawActorFootprint(s32 id)
{
	struct MapScrollWork   *work = gMapWork[0];
	struct StagedActor	   *actor = (struct StagedActor *)Object_GetById(id);
	struct StagedActorProbe probe;
	u32						idx = 0;
	s32						offset;
	s32						width;
	s32						height;
	s32						x0;
	s32						x1;
	s32						z0;
	s32						z1;
	s32						dst_z;

	for (idx = 0; idx <= 5; idx++)
	{
		if (actor->animation->entries[0]->anim_id == StagedActor_FootprintKinds[idx])
		{
			probe.footprint_index = idx;
			break;
		}
		probe.footprint_index = 7;
	}

	if ((u32)probe.footprint_index > 6)
		return 0;
	probe.position_x = actor->x.value;
	probe.position_y = actor->y;
	probe.position_z = actor->z.value;
	offset = probe.footprint_index * 4;
	z0 = StagedActor_FootprintBounds[offset + 1];
	if (z0 < 0)
		z0 = -z0;
	z1 = StagedActor_FootprintBounds[offset + 3];
	if (z1 < 0)
		z1 = -z1;
	height = (z0 + z1) >> 4;
	x0 = StagedActor_FootprintBounds[offset];
	if (x0 < 0)
		x0 = -x0;
	x1 = StagedActor_FootprintBounds[offset + 2];
	if (x1 < 0)
		x1 = -x1;
	width = (x0 + x1) >> 4;
	probe.position_x += StagedActor_FootprintBounds[offset] << 16;
	probe.position_z += StagedActor_FootprintBounds[offset + 1] << 16;
	probe.position_x >>= 20;
	probe.position_z >>= 20;
	idx = work->layers[1].offset_x >> 20;
	dst_z = work->layers[1].offset_y >> 20;
	Engine_MapCopyCellAttributes(probe.position_x, probe.position_z, width, height,
		idx + probe.position_x, dst_z + probe.position_z);
	StagedActor_FillGridAttributeRectangle(
		0, probe.position_x, probe.position_z, width, height, 255);
	StagedActor_FillGridAttributeRectangle(
		2, probe.position_x, probe.position_z, width, height, 255);
	return 1;
}
