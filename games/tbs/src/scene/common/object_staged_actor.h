#include "battle.h"
#include "engine_call.h"
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
#include "field_runtime.h"
#include "map.h"
#include "map_scroll.h"
#include "field_event.h"
#include "types.h"

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
extern s32 gStagedActor_DirectionSteps[];
extern s32 gStagedActor_FootprintBounds[];

// ここに共通ソースが入る（ROM の並びのとおり）
#include "object_distance.h"

static struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin)
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

static void StagedActor_AdvancePair(void)
{
	s32					dst[3];
	struct StagedActor *lead;
	struct StagedActor *next;
	struct StagedActor *blocker;
	s32					dir;
	u32					step;
	s32					rate;

	lead = (struct StagedActor *)Object_GetById(0);
	dir = lead->directionAndKind >> 12;
	step = gStagedActor_DirectionSteps[dir];
	dst[0] = lead->x.value + (step & 0xffff0000);
	dst[1] = lead->y;
	step <<= 16;
	dst[2] = lead->z.value + step;
	next = StagedActor_FindAtTile(dst, lead);
	if (next == 0)
		return;

	step = gStagedActor_DirectionSteps[dir];
	dst[0] = next->x.value + (step & 0xffff0000);
	dst[1] = next->y;
	step <<= 16;
	dst[2] = next->z.value + step;
	blocker = StagedActor_FindAtTile(dst, next);
	if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
		return;

	dst[0] = next->x.value;
	dst[1] = next->y + 0x100000;
	dst[2] = next->z.value;
	blocker = StagedActor_FindAtTile(dst, next);
	if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
		return;

	next->transitionMode = 2;
	step = gStagedActor_DirectionSteps[dir];
	dst[0] = next->x.value + (step & 0xffff0000);
	dst[1] = next->y;
	step <<= 16;
	dst[2] = next->z.value + step;
	if (FieldObject_CheckMovementCollision(next, dst) > 0)
		return;

	if (next->transitionBusy != 0)
		return;

	Object_SetMode((struct FieldActor *)lead, 8);
	rate = 0x3333;
	Engine_TaskWait(15);
	Engine_AudioPlayCue(185);
	next->moveRateX = rate;
	next->moveRateZ = rate;
	Engine_ObjectSetPosition((struct FieldActor *)next, dst[0], dst[1], dst[2]);
	lead->moveRateX = rate;
	lead->moveRateZ = rate;
	Engine_ObjectSetPosition((struct FieldActor *)lead, dst[0], dst[1], dst[2]);
	Object_CommitPosition(next);
	BattleFx_PlayQueuedSound();
	next->x.value = dst[0];
	next->z.value = dst[2];
	next->velocityX = 0;
	next->velocityZ = 0;
	lead->targetX = 0x80000000;
	lead->targetZ = 0x80000000;
	lead->velocityX = 0;
	lead->velocityZ = 0;
	lead->x.value = lead->x.parts.cell << 16;
	lead->z.value = lead->z.parts.cell << 16;
	Object_SetMode((struct FieldActor *)lead, 1);
}

static s32 StagedActor_FillGridAttributeRectangle(
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
				cell->collisionCode = value;
				cell++;
			}
		}
	}
	return 0;
}

static s32 StagedActor_StopBlockedMotion(struct StagedActor *actor)
{
	struct StagedActorProbePoint probe;
	u32							 dir;
	s32							 step;
	struct StagedActor			*target;

	dir = actor->directionAndKind >> 12;
	step = gStagedActor_DirectionSteps[dir];
	probe.x = actor->x.value + (step & 0xffff0000);
	probe.y = actor->y;
	step <<= 16;
	probe.z = actor->z.value + step;
	target = StagedActor_FindAtTile((s32 *)&probe, actor);
	if (target != 0)
	{
		u32	 i = 0;
		s32	 kind = target->animation->entries[0]->animId;
		s32 *kinds = gStagedActor_FootprintKinds;

		do
		{
			if (kind == *kinds++)
				return 0;
			i++;
		} while (i <= 5);
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->targetX = 0x80000000;
		actor->targetZ = 0x80000000;
	}
	step = gStagedActor_DirectionSteps[dir];
	probe.x = actor->x.value + (step & 0xffff0000);
	probe.y = actor->y;
	step <<= 16;
	probe.z = actor->z.value + step;
	if (FieldObject_CheckMovementCollision(actor, (s32 *)&probe) > 0)
	{
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->targetX = 0x80000000;
		actor->targetZ = 0x80000000;
	}
	return 0;
}

static u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint)
{
	struct FieldStepWork  *work = (struct FieldStepWork *)gEventWork;
	struct StagedActor	  *player = (struct StagedActor *)Object_GetById(0);
	struct ObjectRuntime **list;
	u32					   i, j;

	*direction = player->directionAndKind >> 12;
	list = work->actors + 8;
	for (i = 8; i <= 65; list++, i++)
	{
		struct StagedActor *actor = (struct StagedActor *)*list;
		s32					kind;
		j = 0;
		kind = actor->animation->entries[0]->animId;
		for (j = 0; j < 6; j++)
		{
			if (kind == gStagedActor_FootprintKinds[j])
			{
				s32 x, z, left, top, right, bottom;
				*footprint = j;
				x = ((player->x.value >> 16) + (gStagedActor_DirectionSteps[*direction] >> 16)) >> 4;
				z = ((player->z.value >> 16) + (s16)gStagedActor_DirectionSteps[*direction]) >> 4;
				left = (actor->x.parts.cell + gStagedActor_FootprintBounds[j * 4 + 0]) >> 4;
				top = (actor->z.parts.cell + gStagedActor_FootprintBounds[j * 4 + 1]) >> 4;
				right = (actor->x.parts.cell + gStagedActor_FootprintBounds[j * 4 + 2]) >> 4;
				bottom = (actor->z.parts.cell + gStagedActor_FootprintBounds[j * 4 + 3]) >> 4;
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

static s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe)
{
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
		&direction, &probe->actorSlot, &probe->footprintIndex);
	if (actor == 0)
	{
		return 0;
	}
	mode = &actor->transitionMode;
	*mode = 2;
	{
		s32 footprint = probe->footprintIndex;

		steps = 0;
		offset = footprint * 4;
		near_edge = gStagedActor_FootprintBounds[offset + 1];
		if (near_edge < 0)
		{
			near_edge = -near_edge;
		}
		far_edge = gStagedActor_FootprintBounds[offset + 3];
		if (far_edge < 0)
		{
			far_edge = -far_edge;
		}
		height = (near_edge + far_edge) >> 4;
		near_edge = gStagedActor_FootprintBounds[offset];
		if (near_edge < 0)
		{
			near_edge = -near_edge;
		}
		far_edge = gStagedActor_FootprintBounds[offset + 2];
		if (far_edge < 0)
		{
			far_edge = -far_edge;
		}
		width = (near_edge + far_edge) >> 4;
	}
	position[0] = actor->x.value + (gStagedActor_DirectionSteps[direction] & 0xffff0000);
	{
		s32 y = actor->y;

		position[1] = y;
		position[2] = actor->z.value + (gStagedActor_DirectionSteps[direction] << 16);
		probe->positionY = y;
	}
	for (;;)
	{
		probe->positionZ =
			position[2] + (gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] << 16);
		for (row = 0; row < height; row++)
		{
			probe->positionX =
				position[0] + (gStagedActor_FootprintBounds[probe->footprintIndex * 4] << 16);
			for (column = 0; column < width; column++)
			{
				if (FieldObject_CheckMovementCollision(actor, &probe->positionX) == 2)
				{
					goto found;
				}
				probe->positionX += 0x100000;
			}
			probe->positionZ += 0x100000;
		}
		steps++;
		position[0] += gStagedActor_DirectionSteps[direction] & 0xffff0000;
		position[2] += gStagedActor_DirectionSteps[direction] << 16;
	}
found:
	*mode = 0;
	if (steps == 0)
	{
		return 0;
	}
	probe->positionX =
		actor->x.value + (gStagedActor_DirectionSteps[direction] & 0xffff0000) * steps;
	probe->positionY = actor->y;
	probe->positionZ = actor->z.value + (gStagedActor_DirectionSteps[direction] << 16) * steps;
	return 1;
}

static void SceneActor_MoveAndRedraw(struct StagedActorProbe probe)
{
	/* FAKEMATCH: gMapWork[0] read as a u32 may alias the probe's s32 fields, so
	 * the load stays after the four stores of the incoming probe, as native.
	 * 2026-10-13: the plain gMapWork[0] (a void *) is scheduled above those
	 * stores (differ 6). With gMapWork declared u32[] (global_cells.h) the
	 * plain (struct MapScrollWork *)gMapWork[0] matches and, in all six
	 * editions, only DisplayBlend_StartScript changes; it matches again with
	 * the map work read into a local before the zero is stored. */
	struct MapScrollWork		   *workspace;
	struct StagedActor			   *actor;
	struct StagedActorProbePosition original_position;
	struct StagedActorProbePosition tile_position;
	u8							   *footprint_table;
	s32								direction;
	s32								horizontal_extent;
	s32								vertical_extent;

	workspace = (struct MapScrollWork *)*(u32 *)gMapWork;
	direction = ((struct StagedActor *)Object_GetById(0))->directionAndKind >> 12;
	actor = (struct StagedActor *)Object_GetById(probe.actorSlot);
	footprint_table = (u8 *)gStagedActor_FootprintBounds;
	{
		s32 extent_a = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 1];
		s32 extent_b;

		if (extent_a < 0)
			extent_a = -extent_a;
		extent_b = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 3];
		if (extent_b < 0)
			extent_b = -extent_b;
		vertical_extent = (extent_a + extent_b) >> 4;
		extent_a = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 0];
		if (extent_a < 0)
			extent_a = -extent_a;
		extent_b = gStagedActor_FootprintBounds[probe.footprintIndex * 4 + 2];
		if (extent_b < 0)
			extent_b = -extent_b;
		horizontal_extent = (extent_a + extent_b) >> 4;
	}

	actor->moveRateX = 0x8000;
	actor->moveRateZ = 0x1999;
	original_position.x = actor->x.value;
	original_position.z = actor->z.value;
	{
		s32 table_offset = probe.footprintIndex << 4;
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
		s32 horizontal_delta = probe.positionX - original_position.x;
		if (horizontal_delta < 0)
			horizontal_delta += 0x1ffff;
		horizontal_delta >>= 17;
		{
			s32 vertical_delta = probe.positionZ - original_position.z;
			if (vertical_delta < 0)
				vertical_delta += 0x1ffff;
			vertical_delta >>= 17;
			Engine_ActorSetDestinationOffset(0, horizontal_delta, vertical_delta);
		}
	}

	((struct StagedActor *)Object_GetById(0))->movementCallback =
		(u32)StagedActor_StopBlockedMotion;
	Engine_EventWait(4);
	if ((u32)(direction - 6) <= 7)
		Object_SetMode((struct FieldActor *)actor, 3);
	else
		Object_SetMode((struct FieldActor *)actor, 2);

	Engine_AudioPlayCue(239);
	Engine_ObjectSetPosition(
		(struct FieldActor *)actor, probe.positionX, probe.positionY, probe.positionZ);
	Engine_ActorWaitForMove(0);
	Engine_ActorSetAnimation(0, 2);
	Actor_SetSpeed(0, 0x4ccc, 0x1999);

	{
		s32 packed_orientation = gStagedActor_DirectionSteps[direction];
		Engine_ActorSetDestinationOffset(
			0, ((s16)(packed_orientation >> 16)) / 2, ((s16)packed_orientation) / 2);
	}
	if (probe.callback != 0)
		probe.callback();

	Engine_ActorWaitForMove(0);
	Engine_ActorSetAnimation(0, 1);
	((struct StagedActor *)Object_GetById(0))->movementCallback = 0;
	Object_CommitPosition(actor);
	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(213);

	actor->x.value = probe.positionX;
	actor->z.value = probe.positionZ;
	actor->velocityX = 0;
	actor->velocityZ = 0;
	Object_SetMode((struct FieldActor *)actor, 1);

	{
		u8 *table = (u8 *)gStagedActor_FootprintBounds;
		s32 horizontal_origin;
		s32 vertical_origin;

		{
			s32 table_offset = probe.footprintIndex << 4;
			probe.positionX = (probe.positionX + (*(s32 *)(table + table_offset) << 16));
			table_offset += 4;
			probe.positionZ = (probe.positionZ + (*(s32 *)(table + table_offset) << 16));
			probe.positionX >>= 20;
			probe.positionZ >>= 20;
		}
		horizontal_origin = workspace->layers[1].offsetX >> 20;
		vertical_origin = workspace->layers[1].offsetY >> 20;

		Map_CopyCellAttributes(probe.positionX, probe.positionZ, horizontal_extent,
			vertical_extent, horizontal_origin + probe.positionX,
			vertical_origin + probe.positionZ);
		StagedActor_FillGridAttributeRectangle(
			0, probe.positionX, probe.positionZ, horizontal_extent, vertical_extent, 255);
		StagedActor_FillGridAttributeRectangle(
			2, probe.positionX, probe.positionZ, horizontal_extent, vertical_extent, 255);

		{
			s32 table_offset = probe.footprintIndex << 4;
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

static s32 FieldScene_RedrawActorFootprint(s32 id)
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
		if (actor->animation->entries[0]->animId == gStagedActor_FootprintKinds[idx])
		{
			probe.footprintIndex = idx;
			break;
		}
		probe.footprintIndex = 7;
	}

	if ((u32)probe.footprintIndex > 6)
		return 0;
	probe.positionX = actor->x.value;
	probe.positionY = actor->y;
	probe.positionZ = actor->z.value;
	offset = probe.footprintIndex * 4;
	z0 = gStagedActor_FootprintBounds[offset + 1];
	if (z0 < 0)
		z0 = -z0;
	z1 = gStagedActor_FootprintBounds[offset + 3];
	if (z1 < 0)
		z1 = -z1;
	height = (z0 + z1) >> 4;
	x0 = gStagedActor_FootprintBounds[offset];
	if (x0 < 0)
		x0 = -x0;
	x1 = gStagedActor_FootprintBounds[offset + 2];
	if (x1 < 0)
		x1 = -x1;
	width = (x0 + x1) >> 4;
	probe.positionX += gStagedActor_FootprintBounds[offset] << 16;
	probe.positionZ += gStagedActor_FootprintBounds[offset + 1] << 16;
	probe.positionX >>= 20;
	probe.positionZ >>= 20;
	idx = work->layers[1].offsetX >> 20;
	dst_z = work->layers[1].offsetY >> 20;
	Engine_MapCopyCellAttributes(probe.positionX, probe.positionZ, width, height,
		idx + probe.positionX, dst_z + probe.positionZ);
	StagedActor_FillGridAttributeRectangle(
		0, probe.positionX, probe.positionZ, width, height, 255);
	StagedActor_FillGridAttributeRectangle(
		2, probe.positionX, probe.positionZ, width, height, 255);
	return 1;
}
