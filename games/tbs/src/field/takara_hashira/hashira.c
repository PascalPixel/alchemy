#include "game_state.h"
#include "map_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/takara_hashira/hashira.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_reg.h"
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
#include "dma.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "call.h"
#include "field_service.h"
#include "irq.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "heap_state.h"

#define NULL ((void *)0)
#define REC_ID 16
#define QUERY_FLAG 0x200

struct PillarWork
{
	u8				   unknown_00[24];
	struct FieldActor *rider;
};

#define PILLAR_WORK (*(struct PillarWork **)&gWorkSlot[36])

struct MapCellFields
{
	u32 tile : 12;
	u32 layer : 2;
	u8	kind : 2;
	u8	height;
	u8	event;
};

struct MapLayer_2
{
	struct MapCellFields *cells;
	u8					  pad04[44];
};

static void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

static void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 0;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 14);
		Engine_ObjectSetBlendMode(object, 1);
		return object;
	}
	return 0;
}

static void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 1;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 15);
		object->priority_flags = (object->priority_flags & 0xfe) | 2;
		return object;
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"
#include "../common/object/staged_actor.c"

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/map/panel.c"

static void OverlayObject_WaitUntilSettledAndReset(u8 *obj)
{
	s32 cnt = 60;

	for (;;)
	{
		if (cnt == 0)
		{
			break;
		}
		Engine_TaskWait(1);
		if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20))
		{
			break;
		}
		cnt--;
	}

	*(u32 *)(obj + 0x28) = 0;
	*(u32 *)(obj + 0x3c) = 0x80000000;
	*(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

static void SceneActor_WaitHeightBelowLimit(struct FieldActor *obj, s32 limit)
{
	s32 cnt = 60;

	for (;;)
	{
		if (cnt == 0)
		{
			break;
		}
		Engine_TaskWait(1);
		if (obj->y.fixed <= obj->terrain_height)
		{
			break;
		}
		if (obj->y.fixed <= limit)
		{
			break;
		}
		cnt--;
	}

	obj->velocity_y = 0;
	obj->target_y = 0x80000000;
}

static s32 FieldScene_QueryActorFootprint(
	s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top)
{
	u8				   *map = gMapWork[0];
	struct StagedActor *actor = (struct StagedActor *)Object_GetById(id);
	u32					i;
	s32					a;
	s32					b;

	i = 0;
	if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i])
	{
	miss:
		probe->footprint_index = 7;
		if (++i > 5)
		{
			goto check;
		}
		if (actor->animation->entries[0]->anim_id != StagedActor_FootprintKinds[i])
		{
			goto miss;
		}
	}
	probe->footprint_index = i;
check:
	if ((u32)probe->footprint_index > 6)
	{
		return 0;
	}
	probe->position_x = actor->x.value;
	probe->position_y = actor->y;
	probe->position_z = actor->z.value;
	a = StagedActor_FootprintBounds[probe->footprint_index * 4 + 1];
	if (a < 0)
	{
		a = -a;
	}
	b = StagedActor_FootprintBounds[probe->footprint_index * 4 + 3];
	if (b < 0)
	{
		b = -b;
	}
	*depth = (a + b) >> 4;
	a = StagedActor_FootprintBounds[probe->footprint_index * 4];
	if (a < 0)
	{
		a = -a;
	}
	b = StagedActor_FootprintBounds[probe->footprint_index * 4 + 2];
	if (b < 0)
	{
		b = -b;
	}
	*width = (a + b) >> 4;
	probe->position_x += StagedActor_FootprintBounds[probe->footprint_index * 4] << 16;
	{
		s32 step = StagedActor_FootprintBounds[probe->footprint_index * 4 + 1] << 16;

		a = probe->position_z;
		a += step;
		a >>= 20;
		probe->position_z = a;
	}
	probe->position_x >>= 20;
	*left = *(s32 *)(map + 0x13c) >> 20;
	*top = *(s32 *)(map + 0x140) >> 20;
	return 1;
}

static s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
	struct FieldActor *obj = Object_GetById(no);
	u32				   out20, out16;
	s32				   out12, out8;
	s32				   rec[6];
	s32				   r2, r4;
	u8				   mask;

	if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0)
	{
		return 0;
	}

	r2 = rec[2];
	r4 = rec[4];
	Engine_MapCopyCellAttributes(2, 2, out20, out16, r2, r4);

	Object_SetMode(obj, 4);
	mask = 2;
	obj->priority_flags |= mask;

	if (out20 > out16)
	{
		Engine_MapCopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
	}
	else
	{
		Engine_MapCopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
	}

	return 1;
}

static s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
	struct FieldActor *obj = Object_GetById(no);
	s32				   out20, out16, out12, out8;
	s32				   rec[6];

	if (FieldScene_QueryActorFootprint(no, &out20, &out16, rec, &out12, &out8) == 0)
	{
		return 0;
	}

	{
		s32 x = out12 + rec[2];
		s32 z = out8 + rec[4];

		Engine_MapCopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
		StagedActor_FillGridAttributeRectangle(0, rec[2], rec[4], out20, out16, 255);
	}

	Object_SetMode(obj, 1);
	obj->priority_flags &= 0xfd;

	return 1;
}

static void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst)
{
	struct MapState *map = gMapWork[0];

	if (map != 0)
	{
		u32 *cell = (u32 *)map->layers[layer].cells;

		cell += x + (y << 7);
		Dma_Set(cell, dst, 0x84000001, REG_DMA3);
		{
			volatile u32 *dma = REG_DMA3;

			while (dma[2] & 0x80000000)
				;
		}
	}
}

static void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *source)
{
	struct MapCellFields *src = (struct MapCellFields *)source;
	struct MapState		 *map = gMapWork[0];

	if (map != 0)
	{
		struct MapCellFields *cell = (struct MapCellFields *)map->layers[layer].cells;

		cell += x + (y << 7);

		cell->layer = src->layer;
		cell->kind = src->kind;
		cell->height = src->height;
		cell->event = src->event;
	}
}

static s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far)
{
	struct FieldActor *actor = Object_GetById(id);
	s32				   found;
	s32				   object;
	s32				   height;
	s32				   level;
	s32				   top;
	u32				   row;
	u32				   rows;
	s32				   z;
	s32				   y;
	s32				   x;
	s32				   kind;

	found = 0;
	object = found;
	Engine_ActorSetSpriteFlags(actor, found);
	height = Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
	level = height / 0x100000;
	rows = level;
	if (level < 0)
	{
		rows = -level;
	}
	rows++;
	for (row = 0; row <= rows; row++)
	{
		top = Map_GetTerrainHeight(actor->terrain_id, actor->x.fixed, (row << 20) + actor->z.fixed);
		top /= 0x100000;
		if (level < top)
		{
			x = ((actor->x.fixed >> 20) << 20) + 0x80000;
			if (far == 0)
			{
				row += 2;
				z = ((row + (actor->z.fixed >> 20)) << 20) + 0x20000;
				y = top << 20;
				kind = 223;
			}
			else
			{
				row += 3;
				z = ((row + (actor->z.fixed >> 20)) << 20) - 0x20000;
				y = top << 20;
				kind = 253;
			}
			/* FAKEMATCH: a do-while(0) around the object and height lets sched2 issue the height arithmetic first, as the reference does */
			do
			{
				object = OverlayObject_PrepareObject(x, y, z, kind);
				height = actor->z.fixed - z + y;
			} while (0);
			found = 1;
			break;
		}
	}
	SceneActor_WaitHeightBelowLimit(actor, height);
	actor->x.fixed = 0;
	actor->y.fixed = 0;
	actor->z.fixed = 0;
	if (object != 0)
	{
		Engine_ObjectDispatchRelease((void *)object);
	}
	return found;
}

static void StagedActor_StepDownUntilClamp(s32 index)
{
	struct FieldActor *obj = Object_GetById(index);
	u32				   cnt;

	obj->motion_flags = 0;

	cnt = 0;
	for (;;)
	{
		if (cnt > 31)
			return;
		Engine_TaskWait(1);
		obj->scale_y += -0x1999;
		obj->y.fixed += -0xcccc;
		cnt++;
		if (obj->scale_y <= 0x1998)
		{
			obj->scale_y = 0x1999;
			return;
		}
	}
}

static void FieldScene_RunPrimarySequence(void)
{

	struct FieldActor *rec;
	s32				   flag;
	s32				   p6;
	s32				   p5;
	s32				   record;
	s32				   v1;
	s32				   v2;
	s32				   v3;
	u8				  *base;
	u8				   slot16[40];

	rec = Object_GetById(ACTOR_PARTY_LEADER);
	flag = gFrameCount & 3;
	if (flag == 0)
	{
		base = slot16;
		*(s32 *)(base + 4) = 10;
		*(s32 *)(base + 8) = 0xb333;
		*(s32 *)(base + 12) = 0xb333;
		v1 = Engine_RandomNext();
		p6 = rec->x.fixed + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
		v2 = Engine_RandomNext();
		p5 = rec->z.fixed + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
		v3 = Engine_RandomNext();
		record = Math_Divide((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
		Effect_Spawn(p6, rec->y.fixed, p5, 0, record, flag, 0x90001, (s32)base);
	}
}

static s32 FieldScene_RunScene3b3SequenceD(void)
{
	struct FieldActor *leader;
	u8				  *flags;
	s32				   saved;
	s32				   mode;
#if !EDITION_INTERNATIONAL
	s32 step;
#endif
	s32 *p;
	s32	 buf[3];

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	flags = &leader->motion_flags;
	saved = *flags;
#if EDITION_INTERNATIONAL
	mode = (leader->facing + 0x2000) & 0xc000;
#endif
	if (gGameState.movement_mode != 0)
	{
		return 0;
	}
	p = buf;
#if EDITION_INTERNATIONAL
	p[0] = (leader->x.fixed & -0x100000) + 0x80000;
	p[1] = leader->y.fixed;
	p[2] = (leader->z.fixed & -0x100000) + 0x80000;
	Vector_AddPolarOffset(0x100000, mode, p);
#else
	mode = leader->facing >> 12;
	step = StagedActor_DirectionSteps[mode];
	p[0] = leader->x.fixed + (step & 0xffff0000);
	p[1] = leader->y.fixed;
	step <<= 16;
	p[2] = leader->z.fixed + step;
#endif
	if (Object_CheckMovementCollision(leader, p) == 1)
	{
		goto reject;
	}
	if (StagedActor_FindAtTile(p, (struct StagedActor *)leader) != 0)
	{
		goto reject;
	}
#if EDITION_INTERNATIONAL
	p[0] = (leader->x.fixed & -0x100000) + 0x80000;
	p[1] = leader->y.fixed;
	p[2] = (leader->z.fixed & -0x100000) + 0x80000;
	Vector_AddPolarOffset(0x200000, mode, p);
#else
	step = StagedActor_DirectionSteps[mode];
	p[0] = leader->x.fixed + ((step & 0xffff0000) * 2);
	p[1] = leader->y.fixed;
	step <<= 17;
	p[2] = leader->z.fixed + step;
#endif
	if (StagedActor_FindAtTile(p, (struct StagedActor *)leader) != 0)
	{
		goto reject;
	}
	if (Object_CheckMovementCollision(leader, p) != 0)
	{
		goto reject;
	}
	Engine_EventBegin();
	Object_SetMode(leader, 6);
	Engine_TaskWait(6);
	Engine_AudioPlayCue(152);
	Object_SetMode(leader, 7);
	leader->speed = 0x30000;
	leader->acceleration = 0x20000;
	leader->velocity_y = 0x40000;
	*flags &= 126;
	Engine_ActorSetSpriteFlags(leader, 0);
	Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, ((s16 *)p)[1], ((s16 *)p)[5]);
	Object_SetMode(leader, 6);
	Engine_ActorSetSpriteFlags(leader, 1);
	*flags = saved;
	Engine_EventEnd();
	return 1;
reject:
	return 0;
}

static void FieldScene_RunScene3b3SequenceE(union FieldObject *object)
{
	s32 velocity_x;
	s32 velocity_y;
	s32 velocity_z;

	object->effect.x += object->effect.velocity_x;
	object->effect.y += object->effect.velocity_y;
	object->effect.z += object->effect.velocity_z;
	velocity_x = object->effect.velocity_x;
	velocity_y = object->effect.velocity_y;
	velocity_z = object->effect.velocity_z;
	object->effect.velocity_x = velocity_x - Math_Divide(velocity_x, 10);
	object->effect.velocity_y = velocity_y - Math_Divide(velocity_y, 3);
	object->effect.velocity_z = velocity_z - Math_Divide(velocity_z, 10);
	object->effect.scale_x += object->effect.scale_rate_x;
	object->effect.scale_y += object->effect.scale_rate_y;
	object->effect.sprite->rotation += object->effect.spin;
}

static s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor)
{
	ObjectGroup_SetChildValue(actor, *(u16 *)(actor + 100) & 15);
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraHashira1)
	{
		return gTakaraHashiraEntrances1;
	}
	if (scene == (s32)&SceneId_TakaraHashira2)
	{
		return gTakaraHashiraEntrances2;
	}
	if (scene == (s32)&SceneId_TakaraHashira3)
	{
		return gTakaraHashiraEntrances3;
	}
	if (scene == (s32)&SceneId_TakaraHashira4)
	{
		return gTakaraHashiraEntrances4;
	}
	if (scene == (s32)&SceneId_TakaraHashira5)
	{
		return gTakaraHashiraEntrances5;
	}
	return gTakaraHashiraEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gTakaraHashiraExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraHashira1)
	{
		return gTakaraHashiraPlacements1;
	}
	if (scene == (s32)&SceneId_TakaraHashira2)
	{
		return gTakaraHashiraPlacements2;
	}
	if (scene == (s32)&SceneId_TakaraHashira3)
	{
		return gTakaraHashiraPlacements3;
	}
	if (scene == (s32)&SceneId_TakaraHashira5)
	{
		return gTakaraHashiraPlacements5;
	}
	return gTakaraHashiraPlacementsOther;
}

static void EV10_SCRIPT_1(void)
{
	Engine_EventBegin();
	if (FieldScene_RunScene3b3SequenceD() == 0)
		StagedActor_AdvancePair();
	Engine_EventEnd();
}

static void EV10_SCRIPT_2(void)
{
	struct StagedActorProbe out;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&out) != 0)
		SceneActor_MoveAndRedraw(out);
	Engine_EventEnd();
}

struct PillarSlot
{
	s32					 x;
	s32					 y;
	s32					 z;
	struct MapCellFields cell;
	s32					 flag;
};

static struct PillarSlot TakaraHashira_PillarSlots[4];
static s32				 TakaraHashira_ProbePosition[3];
static u8				 TakaraHashira_ShakenScroll[12];
static s32				 TakaraHashira_ShakeChance;

static void TakaraHashira_JitterBackgroundScroll(void)
{
	u32			  line = REG_VCOUNT;
	u32			 *src = &((u32 *)gBgScroll)[1];
	volatile u32 *reg = (volatile u32 *)0x04000014;

	if (line == 227 || line <= 46)
	{
		if ((u32)(Engine_RandomNext() * 100) >> 16 < TakaraHashira_ShakeChance)
		{
			src = ((u32 *)TakaraHashira_ShakenScroll);
		}
	}
	*reg = *src++;
	reg = (volatile u32 *)0x04000018;
	*reg++ = *src++;
	*reg = *src;
}

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

#define DMA3 ((volatile u32 *)0x040000d4)

static __inline__ void Dma_Wait(volatile u32 *dma)
{
	while (dma[2] & 0x80000000)
		;
}

static void CopyAndOffsetCoordinatePreset(void)
{
	u32		  *destination;
	const u32 *source;
	u16		  *coordinates;

	source = (const u32 *)&((u32 *)gBgScroll)[1];
	destination = (u32 *)TakaraHashira_ShakenScroll;
	*destination++ = *source++;
	*destination++ = *source++;
	*destination = *source;
	coordinates = (u16 *)TakaraHashira_ShakenScroll;
	coordinates[1] += 0xc0;
	coordinates[3] += 0xc0;
	coordinates[5] += 0xc0;
}

static void FieldScene_RunScene3b3SequenceA(void)
{
	s32 record;

	record = Engine_GameFlagIsSet(0x200);
	if (record == 0)
	{
		Map_DrawCellPanel(10, 19, 16, 5, record, 10, 31);
		Map_DrawCellPanel(10, 51, 16, 5, 1, 10, 31);
		Map_DrawCellPanel(42, 51, 16, 5, 2, 10, 31);
	}
	else
	{
		Map_DrawCellPanel(10, 19, 16, 5, 0, 10, 31);
		Map_DrawCellPanel(10, 83, 16, 5, 1, 10, 31);
		Map_DrawCellPanel(42, 83, 16, 5, 2, 10, 31);
	}
	TakaraHashira_ShakeChance = 0;
	Engine_TaskAddCallback(CopyAndOffsetCoordinatePreset, 0xc80);
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, TakaraHashira_JitterBackgroundScroll);
	Engine_AudioPlayCue(231);
	TakaraHashira_ShakeChance = 0;
	do
	{
		Engine_TaskWait(1);
	} while (++TakaraHashira_ShakeChance <= 100);
	Engine_AudioPlayCue(0x121);
	if (Engine_GameFlagIsSet(0x200) == 0)
	{
		Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
		Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
	}
	else
	{
		Engine_MapCopyCellsTo(0, 64, 32, 0, 32, 32);
		Engine_MapCopyCellsTo(32, 64, 64, 0, 32, 32);
	}
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, 0);
	Engine_TaskWait(1);
	Engine_TaskRemoveCallback((s32)CopyAndOffsetCoordinatePreset);
	Engine_MapRedraw();
	Engine_TaskWait(30);
}

static void EV20_SEARCH(void)
{
	u8				  *queried;
	struct FieldActor *record;

	Engine_EventBegin();
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x1190000, -1, 0x1b00000, 1);
	Engine_CameraWaitForMove();
	Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
	queried = GameFlag_IsSet(QUERY_FLAG);
	if (queried == 0)
	{
		Event_PlayCue(232);
		Map_AnimateCells(TakaraHashira_ShiftSteps1, 84, 24);
		Engine_EventWait(30);
		Event_PlayCue(240);
		Engine_ActorSetSpritePriority(REC_ID, 1);
		Object_GetById(REC_ID)->motion_flags = queried;
		record = Actor_Get(REC_ID);
		record->y.fixed = -0x200000;
		Actor_SetPosition(REC_ID, 0x1100000, 0x1a00000);
		Engine_ActorSetAnimation(REC_ID, 1);
		Map_AnimateCells(TakaraHashira_ShiftSteps3, 80, 24);
		Map_AnimateCells(TakaraHashira_ShiftSteps5, 80, 28);
		Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
		FieldScene_RunScene3b3SequenceA();
		SceneActor_ApplyPlacementQueryAndTag(9);
		SceneActor_ApplyPlacementQueryAndTag(10);
		SceneActor_ApplyPlacementQueryAndTag(11);
		SceneActor_ApplyPlacementQueryAndTag(12);
		SceneActor_ApplyPlacementQueryAndTag(13);
		SceneActor_ApplyPlacementQueryAndTag(14);
		SceneActor_ApplyPlacementQueryAndTag(15);
		Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
		GameFlag_Set(QUERY_FLAG);
	}
	else
	{
		Event_PlayCue(232);
		Map_AnimateCells(TakaraHashira_ShiftSteps2, 84, 24);
		Engine_EventWait(30);
		Event_PlayCue(230);
		Object_GetById(REC_ID)->motion_flags = 0;
		record = Actor_Get(REC_ID);
		record->y.fixed = -0x200000;
		Actor_SetPosition(REC_ID, 0x1100000, 0x1b40000);
		Engine_ActorSetAnimation(REC_ID, 2);
		Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
		Map_AnimateCells(TakaraHashira_ShiftSteps4, 80, 24);
		FieldScene_RunScene3b3SequenceA();
		SceneActor_ApplyPlacementQuery(9);
		SceneActor_ApplyPlacementQuery(10);
		SceneActor_ApplyPlacementQuery(11);
		SceneActor_ApplyPlacementQuery(12);
		SceneActor_ApplyPlacementQuery(13);
		SceneActor_ApplyPlacementQuery(14);
		SceneActor_ApplyPlacementQuery(15);
		Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
		GameFlag_Clear(QUERY_FLAG);
	}
	Engine_EventEnd();
}

static s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front)
{
	s32 result = 0;

	if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed
		&& front->z.fixed == actor->z.fixed)
	{
		return result;
	}
	if (front->x.fixed - 0x100000 < actor->x.fixed && actor->x.fixed < front->x.fixed + 0x100000
		&& front->y.fixed / 0x10000 == actor->y.fixed / 0x10000 && front->z.fixed > actor->z.fixed
		&& front->z.fixed - 0x200000 < actor->z.fixed)
	{
		if (SPRITE_OF(actor)->priority < SPRITE_OF(front)->priority)
		{
			actor->priority_flags &= ~1;
			SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
			SPRITE_OF(actor)->second_priority = SPRITE_OF(front)->second_priority;
		}
		result = 1;
	}
	return result;
}

static s32 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor)
{
	s32 result = 0;

	if (actor->x.fixed == front->x.fixed && actor->y.fixed == front->y.fixed
		&& actor->z.fixed == front->z.fixed)
	{
		return result;
	}
	if (front->x.fixed - 0x100000 < actor->x.fixed && actor->x.fixed < front->x.fixed + 0x100000
		&& actor->y.fixed / 0x10000 == front->y.fixed / 0x10000 && front->z.fixed > actor->z.fixed
		&& front->z.fixed - 0x200000 < actor->z.fixed)
	{
		if (SPRITE_OF(front)->priority > SPRITE_OF(actor)->priority)
		{
			actor->priority_flags &= ~1;
			SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
			SPRITE_OF(actor)->second_priority = SPRITE_OF(front)->second_priority;
		}
		result = 1;
	}
	return result;
}

static s32 TakaraHashira_UpdateActorPriority(struct FieldActor *actor)
{
	struct FieldActor *leader = Object_GetById(0);
	s32				   hit;
	u32				   id;

	if (((s8 *)gEventWork)[0xcc7] == 1)
	{
		actor->sprite->priority = leader->sprite->priority;
		actor->collision_flags |= 1;
		return 0;
	}
	hit = TakaraHashira_SyncPriorityIfBehind(actor, leader);
	for (id = 8; id <= 11; id++)
	{
		hit += TakaraHashira_SyncPriorityIfBehind(actor, Object_GetById(id));
	}
	if (hit != 0)
	{
		for (id = 8; id <= 11; id++)
		{
			TakaraHashira_SyncPriorityIfAhead(actor, Object_GetById(id));
		}
	}
	if (actor->y.fixed < leader->y.fixed)
	{
		actor->priority_flags |= 2;
		actor->collision_flags &= 254;
		if (actor->sprite->priority < leader->sprite->priority)
		{
			actor->priority_flags &= 254;
			SPRITE_OF(actor)->priority = SPRITE_OF(leader)->priority;
			SPRITE_OF(actor)->second_priority = SPRITE_OF(leader)->second_priority;
			hit = 1;
		}
	}
	else
	{
		actor->priority_flags &= 253;
		actor->collision_flags |= 1;
	}
	if (hit == 0)
	{
		actor->priority_flags |= 1;
	}
	return 0;
}

static void TakaraHashira_SortPillarActors(void)
{
	u32				   i;
	u32				   j;
	struct FieldActor *a;
	struct FieldActor *b;
	u8				   actor[112];
	s32				   slot[3];

	for (i = 0; i <= 2; i++)
	{
		a = Object_GetById(i + 8);
		for (j = i; j <= 3; j++)
		{
			b = Object_GetById(j + 8);
			if (a->y.fixed <= b->y.fixed && a->z.fixed < b->z.fixed)
			{
				continue;
			}
			Dma_Set(b, actor, 0x8400001c, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(a, b, 0x8400001c, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(actor, a, 0x8400001c, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(&TakaraHashira_PillarSlots[j], slot, 0x84000004, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(&TakaraHashira_PillarSlots[i], &TakaraHashira_PillarSlots[j], 0x84000004, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(slot, &TakaraHashira_PillarSlots[i], 0x84000004, DMA3);
			Dma_Wait(DMA3);
			if (Engine_GameFlagIsSet(TakaraHashira_PillarSlots[i].flag)
				&& !Engine_GameFlagIsSet(TakaraHashira_PillarSlots[j].flag))
			{
				Engine_GameFlagClear(TakaraHashira_PillarSlots[i].flag);
				Engine_GameFlagSet(TakaraHashira_PillarSlots[j].flag);
			}
			else if (!Engine_GameFlagIsSet(TakaraHashira_PillarSlots[i].flag)
					 && Engine_GameFlagIsSet(TakaraHashira_PillarSlots[j].flag))
			{
				Engine_GameFlagSet(TakaraHashira_PillarSlots[i].flag);
				Engine_GameFlagClear(TakaraHashira_PillarSlots[j].flag);
			}
		}
	}
}

struct PillarActor
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[14];
	u8	unknown_22;
	u8	priority_flags;
	s32 velocity_x;
	s32 velocity_y;
	u8	unknown_2c[0x29];
	u8	motion_flags;
};

static void TakaraHashira_UpdatePillarActors(void)
{
	u32					 id;
	struct PillarActor	*actor;
	u8					*flags;
	u32					 i;
	u32					 k;
	struct PillarActor	*other;
	struct MapCellFields cell;

	for (id = 8; id <= 11; id++)
	{
		actor = (struct PillarActor *)Object_GetById(id);
		actor->unknown_22 = 2;
		i = id - 8;
		if ((actor->x >> 20) == TakaraHashira_PillarSlots[i].x
			&& (actor->z >> 20) == TakaraHashira_PillarSlots[i].z && actor->velocity_y == 0)
		{
			continue;
		}
		Dma_Set(&actor->x, TakaraHashira_ProbePosition, 0x84000003, DMA3);
		Dma_Wait(DMA3);
		if (Object_CheckMovementCollision(actor, TakaraHashira_ProbePosition) == -1)
		{
			actor->motion_flags = 3;
		}
		flags = &actor->motion_flags;
		TakaraHashira_SetCellAttributes(0, TakaraHashira_PillarSlots[i].x,
			TakaraHashira_PillarSlots[i].z, (struct MapCell *)&TakaraHashira_PillarSlots[i].cell);
		TakaraHashira_SetCellAttributes(2, TakaraHashira_PillarSlots[i].x,
			TakaraHashira_PillarSlots[i].z, (struct MapCell *)&TakaraHashira_PillarSlots[i].cell);
		if (*flags & 1)
		{
			if (GetMapCellCollision(2, actor->x, actor->z) == 50)
			{
				Engine_AudioPlayCue(189);
				/* FAKEMATCH: the byte view keeps these flag updates out of the record's alias set; field stores reallocate the loop (score 2573). */
				*(u8 *)&actor->priority_flags &= 254;
				TakaraHashira_LowerActorToLedge(id, 1);
				*(u8 *)&actor->priority_flags |= 1;
			}
			else if (GetMapCellCollision(2, actor->x, actor->z) == 51)
			{
				SceneActor_WaitHeightBelowLimit((struct FieldActor *)actor, 0);
				Engine_AudioPlayCue(189);
				actor->y = 0;
				actor->priority_flags &= 254;
				StagedActor_StepDownUntilClamp(id);
				actor->x = 0;
				actor->y = 0;
				actor->z = 0;
				actor->priority_flags |= 1;
			}
			else
			{
				OverlayObject_WaitUntilSettledAndReset((struct FieldActor *)actor);
			}
			*flags = 0;
		}
		TakaraHashira_ReadMapCell(
			0, actor->x >> 20, actor->z >> 20, &TakaraHashira_PillarSlots[i].cell);
		if (actor->y >= 0)
		{
			TakaraHashira_ReadMapCell(0, 27, (actor->y >> 20) + 6, &cell);
			TakaraHashira_SetCellAttributes(
				0, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
			cell.kind = TakaraHashira_PillarSlots[i].cell.kind;
			TakaraHashira_SetCellAttributes(
				2, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
		}
		TakaraHashira_PillarSlots[i].x = actor->x >> 20;
		TakaraHashira_PillarSlots[i].y = actor->y >> 20;
		TakaraHashira_PillarSlots[i].z = actor->z >> 20;
		for (k = 0; k <= 3; k++)
		{
			if (k == i)
			{
				continue;
			}
			Engine_GameFlagClear(TakaraHashira_PillarSlots[k].flag);
			other = (struct PillarActor *)Object_GetById(k + 8);
			if ((actor->x >> 20) == (other->x >> 20) && (actor->z >> 20) == (other->z >> 20)
				&& actor->y > other->y)
			{
				Engine_GameFlagSet(TakaraHashira_PillarSlots[k].flag);
			}
		}
	}
	TakaraHashira_SortPillarActors();
}

struct Probe
{
	s32 word[6];
};

static void TakaraHashira_PushPillar(void)
{
	Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Object_GetById(0)->y.fixed = Map_GetTerrainHeight(
		Object_GetById(0)->terrain_id, Object_GetById(0)->x.fixed, Object_GetById(0)->z.fixed);
#endif
	if (FieldScene_RunScene3b3SequenceD() == 0)
	{
		Object_GetById(0)->motion_flags &= 254;
		Object_GetById(0)->priority_flags &= 254;
		StagedActor_AdvancePair();
		TakaraHashira_UpdatePillarActors();
		Object_GetById(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FOLLOWS_TERRAIN;
		Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	}
	Engine_EventEnd();
}

static void FieldScene_RunSingleStep(void)
{
	battle_owner_69();
}

static void TakaraHashira_RunStagedCellScene(void)
{
	struct FieldActor *leader = Object_GetById(0);
	s32				   pos[3];
	s32				  *p = pos;

	p[0] = leader->x.fixed;
	p[1] = leader->y.fixed;
	p[2] = leader->z.fixed + 0x100000;
	if (StagedActor_FindAtTile(p, leader) != NULL)
	{
		TakaraHashira_PushPillar();
	}
	else if (FieldScene_RunScene3b3SequenceD() == 0)
	{
		FieldEffect_UpdateGridPlacement();
	}
}

static void EV40_TOUCH(void)
{
	struct FieldActor *obj = Object_GetById(ACTOR_PARTY_LEADER);
	PILLAR_WORK->rider = obj;
	obj->rise_counter = 1;
}

static void EV41_TOUCH(void)
{
	struct FieldActor *obj = Object_GetById(ACTOR_PARTY_LEADER);

	PILLAR_WORK->rider = NULL;
	obj->rise_counter = 0;
}

static s32 SceneActor_UpdateBit1ByPositionToSlotZero(struct FieldActor *actor)
{
	struct FieldActor *ref = Object_GetById(ACTOR_PARTY_LEADER);

	if (actor->z.fixed > ref->z.fixed)
	{
		actor->priority_flags &= ~ACTOR_PRIORITY_UNDERFOOT;
		if (actor->y.fixed < ref->y.fixed)
			actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	}

	return 0;
}

static void TakaraHashira_RunActorAction(s32 id)
{
	struct FieldActor *actor;

	actor = Object_GetById(id);
	Engine_EventBegin();
	actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
	Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
	GameFlag_Set(id + 0x1f5);
	Engine_ActorEnableActionCallback(id, TakaraHashira_ActionTable);
	Engine_EventEnd();
}

static void EV11_PSYNERGY(void)
{
	TakaraHashira_RunActorAction(11);
}

static void EV12_PSYNERGY(void)
{
	TakaraHashira_RunActorAction(12);
}

static void TakaraHashira_CheckLeaderOnPillar(void)
{
	struct FieldActor *leader;
	struct FieldActor *pillar;
	u8				  *effects;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	pillar = Object_GetById(13);
	effects = gEffectWork;
	if (pillar->x.fixed >> 20 == leader->x.fixed >> 20
		&& pillar->z.fixed >> 20 == leader->z.fixed >> 20)
	{
		Engine_GameFlagSet(0x203);
		effects[53] = 1;
	}
	else
	{
		Engine_GameFlagClear(0x203);
	}
}

static void TakaraHashira_RaiseActorFourteen(void)
{
	struct FieldActor *pillar;

	pillar = Object_GetById(14);
	Object_GetById(13)->scale_x = 0x10000;
	Object_GetById(13)->scale_y = 0x10000;
	((struct FieldSpritePriority *)Object_GetById(13)->sprite)->priority = 2;
	((struct FieldSpritePriority *)pillar->sprite)->priority = 2;
	pillar->acceleration = 0x6666;
	pillar->speed = 0xcccc;
	FieldObject_SetPosition(pillar, pillar->x.fixed, 0x200000, pillar->z.fixed);
	Engine_ActorWaitForMove(14);
	Map_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

static void SceneEffect_SpawnRandomizedParticle(void)
{
	struct EffectOptions  options;
	struct EffectOptions *d;
	s32					  spread;
	s32					  secondary;
	u32					  draw;
	u32					  mask;

	if ((gFrameCount & 2) != 0)
		return;
	if ((gFrameCount & 7) == 0)
		Engine_AudioPlayCue(136);

	d = &options;
	d->palette = 10;
	d->start_scale_x = 0x8000;
	d->start_scale_y = 0x8000;
	d->target_scale_x = 0x19999;
	d->target_scale_y = 0x19999;
	draw = Engine_RandomNext();
	mask = 0x0ffff000;
	mask &= draw;
	d->rotation = mask;
	d->update = FieldScene_RunScene3b3SequenceE;

	draw = Engine_RandomNext();
	spread = -(((draw * 5) >> 16) * 0x10000 + 0x60000);
	spread /= 2;
	draw = Engine_RandomNext();
	secondary = -(((draw * 5) >> 16) * 0x10000 + 0x50000);

	Effect_Spawn(PIXELS(324), PIXELS(48), PIXELS(228), spread, secondary, 0,
		EFFECT_USE_UPDATE | EFFECT_USE_ROTATION | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET
			| EFFECT_USE_PALETTE,
		d);
}

static s32 SceneEffect_SpawnRandomEffectEveryEightFrames(struct FieldActor *actor)
{
	struct EffectOptions  options;
	struct EffectOptions *p;
	u32					  phase = gFrameCount & 7;
	s32					  x;
	s32					  y;
	s32					  z;
	s32					  scale;

	if (phase != 0)
		return 0;

	p = &options;
	p->palette = 7;
	p->start_scale_x = 0xb333;
	p->start_scale_y = 0xb333;

	x = actor->x.fixed + (((((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
	y = actor->y.fixed + ((((u32)Engine_RandomNext() * 17) >> 16) << 16);
	z = actor->z.fixed + (((((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
	scale = Math_Divide((((u32)Engine_RandomNext() * 5) >> 16) * 0x10000 + 0x30000, 10);

	Effect_Spawn(x, y, z, 0, scale, phase, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, p);
	return 0;
}

static void EV13_PSYNERGY(void)
{
	s32 set;

	set = Engine_GameFlagIsSet(0x203);
	if (set == 0)
	{
		Engine_GameFlagSet(0x202);
		Engine_EventBegin();
		Engine_CameraSetSpeed(0x9999, 0x1333);
		Engine_CameraMoveTo(0x1380000, -1, 0xb80000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(20);
		Map_CopyCellsTo(73, 10, 60, 10, 1, 2);
		Engine_EventWait(20);
		Engine_TaskAddCallback((s32)SceneEffect_SpawnRandomizedParticle, 0xc80);
		Engine_EventWait(40);
		if (GameFlag_IsSet(0x201) != 0)
		{
			Object_GetById(12)->update =
				(void (*)(union FieldObject *))SceneEffect_SpawnRandomEffectEveryEightFrames;
			Engine_ActorSetAnimation(12, 6);
			ObjectMotion_WaitForAnimationChange(12);
			Object_GetById(12)->update = (void (*)(union FieldObject *))set;
			Map_CopyCellAttributes(17, 13, 1, 1, 18, 13);
			Engine_GameFlagClear(0x201);
			Engine_ActorSetChildValue(12, 0);
			Engine_ActorEnableActionCallback(12, 1);
		}
		else
		{
			Engine_EventWait(60);
		}
		Engine_TaskRemoveCallback(SceneEffect_SpawnRandomizedParticle);
		Engine_EventWait(20);
		Map_CopyCellsTo(72, 10, 60, 10, 1, 2);
		Engine_EventWait(20);
		Engine_EventEnd();
	}
}

static void StagedActor_PlaceAtObjectTenCell(void)
{
	struct FieldActor *obj = Object_GetById(10);
	s32				   x;
	s32				   z;

	Engine_EventBegin();

	x = obj->x.fixed >> 20;
	z = obj->z.fixed >> 20;

	StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
	Engine_EventEnd();
}

static void EV10_PSYNERGY(void)
{
	struct FieldActor *rec7;
	s32				   rec8;

	rec7 = Object_GetById(10);
	Engine_EventBegin();
	StagedActor_FillGridAttributeRectangle(
		2, (rec7->x.fixed >> 20), (rec7->z.fixed >> 20), 1, 1, 255);
	if ((rec7->x.fixed >> 20) == 16)
	{
		rec8 = GameFlag_IsSet(0x204);
		if (rec8 == 0)
		{
			Engine_EventWait(10);
			Engine_AudioPlayCue(159);
			rec7->motion_flags = rec8;
			rec7->terrain_height = -0x20000;
			rec7->y.fixed = -0x20000;
			Engine_GameFlagSet(0x204);
		}
	}
	Engine_EventEnd();
}

static void TakaraHashira_RunPushScene(void)
{
	struct Probe probe;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		((void (*)(struct Probe))SceneActor_MoveAndRedraw)(probe);
	}
	else
	{
		StagedActor_PlaceAtObjectTenCell();
		StagedActor_AdvancePair();
		EV10_PSYNERGY();
	}
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_TakaraHashira1)
	{
		return gTakaraHashiraEvents1;
	}
	if (scene == (s32)&SceneId_TakaraHashira2)
	{
		return gTakaraHashiraEvents2;
	}
	if (scene == (s32)&SceneId_TakaraHashira3)
	{
		return gTakaraHashiraEvents3;
	}
	if (scene == (s32)&SceneId_TakaraHashira4)
	{
		return gTakaraHashiraEvents4;
	}
	if (scene == (s32)&SceneId_TakaraHashira5)
	{
		return gTakaraHashiraEvents5;
	}
	return gTakaraHashiraEventsOther;
}

static void TakaraHashira_PrepLoweredActor(s32 id)
{
	struct FieldActor *actor = Object_GetById(id);

	actor->collision_flags &= 254;
	actor->priority_flags |= 2;
	actor->motion_flags = 0;
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->sprite->priority = 2;
}

static void TakaraHashira_RestoreFlaggedPillar(s32 a0)
{
	struct FieldActor *actor;

	actor = Object_GetById(a0);
	if (GameFlag_IsSet(a0 + 0x1f5) != 0)
	{
		Object_SetMode(actor, 5);
		actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
		Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
		Engine_ActorEnableActionCallback(a0, TakaraHashira_ActionTable);
	}
}

static void OverlayObject_SetCallbackAndMode2(s32 id)
{
	struct FieldActor *actor = Object_GetById(id);

	actor->terrain_id = 2;
	actor->motion_flags = 0;
	actor->update = (void (*)(union FieldObject *))TakaraHashira_UpdateActorPriority;
}

static void SceneActor_CheckActors8To11NearSlotZero(void)
{
	struct FieldActor *hero = Object_GetById(ACTOR_PARTY_LEADER);
	u32				   selector;
	struct FieldActor *actor;

	for (selector = 8; selector <= 11; selector++)
	{
		actor = Object_GetById(selector);
		if (hero->y.fixed / 0x10000 == actor->y.fixed / 0x10000
			&& hero->z.fixed <= actor->z.fixed - 0x80000
			&& hero->z.fixed > actor->z.fixed - 0x180000)
		{
			if (hero->x.fixed - 0x100000 <= actor->x.fixed
				&& actor->x.fixed < hero->x.fixed + 0x100000)
			{
				Engine_ActorSetSpritePriority(
					ACTOR_PARTY_LEADER, ((struct FieldSpritePriority *)actor->sprite)->priority);
				return;
			}
		}
		else
		{
			Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
		}
	}
}

s32 TakaraHashira_SetupArea(void)
{
	struct FieldActor *actor;
	u32				   i;
	u32				   id;
	s32				   x;
	s32				   z;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_TakaraHashira2)
	{
		FieldScene_RedrawActorFootprint(8);
		FieldScene_RedrawActorFootprint(9);
		FieldScene_RedrawActorFootprint(10);
		FieldScene_RedrawActorFootprint(11);
		FieldScene_RedrawActorFootprint(12);
	}
	else if (gGameState.scene == (s32)&SceneId_TakaraHashira3)
	{
		Map_CopyCells(32, 0, 64, 32, 0, 64);
		FieldScene_RedrawActorFootprint(8);
		FieldScene_RedrawActorFootprint(9);
		FieldScene_RedrawActorFootprint(10);
		FieldScene_RedrawActorFootprint(11);
		FieldScene_RedrawActorFootprint(12);
		FieldScene_RedrawActorFootprint(13);
		FieldScene_RedrawActorFootprint(14);
		FieldScene_RedrawActorFootprint(15);
		if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x200) != 0)
		{
			Map_CopyCellsTo(79, 34, 84, 24, 1, 2);
			Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
			Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
			SceneActor_ApplyPlacementQueryAndTag(9);
			SceneActor_ApplyPlacementQueryAndTag(10);
			SceneActor_ApplyPlacementQueryAndTag(11);
			SceneActor_ApplyPlacementQueryAndTag(12);
			SceneActor_ApplyPlacementQueryAndTag(13);
			SceneActor_ApplyPlacementQueryAndTag(14);
			SceneActor_ApplyPlacementQueryAndTag(15);
			Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
		}
	}
	else if (gGameState.scene == (s32)&SceneId_TakaraHashira4)
	{
		OverlayObject_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200), 223);
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			Object_GetById(ACTOR_PARTY_LEADER)->rise_counter = 1;
		}
		BattleFx_StartFadeOverlay(0);
		if (Object_GetById(ACTOR_PARTY_LEADER)->rise_counter == 0)
		{
			EV41_TOUCH();
		}
		OverlayObject_SetCallbackAndMode2(8);
		OverlayObject_SetCallbackAndMode2(9);
		OverlayObject_SetCallbackAndMode2(10);
		OverlayObject_SetCallbackAndMode2(11);
		{
			s32 *entry = (s32 *)TakaraHashira_PillarSlots;

			for (i = 0; i < 4; i++)
			{
				entry[0] = 0;
				entry[1] = 0;
				entry[2] = 0;
				entry[4] = i + 0x200;
				entry += 5;
			}
		}
		TakaraHashira_UpdatePillarActors();
		Engine_EventWait(1);
		Engine_TaskAddCallback(SceneActor_CheckActors8To11NearSlotZero, TASK_PRIORITY_SCENE);
		if (Engine_GameFlagIsSet(0x109) != 0)
		{
			for (id = 8; id < 12; id++)
			{
				actor = Object_GetById(id);
				x = actor->x.fixed >> 20;
				if (x == 37)
				{
					z = actor->z.fixed >> 20;
					if (z == 9)
					{
						Engine_MapCopyCellAttributes(27, 8, 1, 1, x, z);
						break;
					}
				}
			}
		}
	}
	else if (gGameState.scene == (s32)&SceneId_TakaraHashira5)
	{
		Engine_ActorSetAnimation(10, 2);
		Engine_ActorSetChildValue(10, 6);
		FieldScene_RedrawActorFootprint(8);
		FieldScene_RedrawActorFootprint(9);
#if EDITION_INTERNATIONAL
		Object_GetById(8)->motion_flags = 0;
		Object_GetById(9)->motion_flags = 0;
#endif
		EV10_PSYNERGY();
		TakaraHashira_PrepLoweredActor(11);
		TakaraHashira_PrepLoweredActor(12);
		TakaraHashira_PrepLoweredActor(13);
		TakaraHashira_RestoreFlaggedPillar(11);
		TakaraHashira_RestoreFlaggedPillar(12);
		TakaraHashira_RestoreFlaggedPillar(13);
		actor = Object_GetById(13);
		actor->update = 0;
		TakaraHashira_PrepLoweredActor(14);
		Object_GetById(14)->collision_flags |= 8;
		if (Engine_GameFlagIsSet(0x202) == 0)
		{
			Object_GetById(13)->scale_x = 0x18000;
			Object_GetById(13)->scale_y = 0x18000;
			((u8 *)Object_GetById(13)->sprite)[9] |= 12;
			((u8 *)Object_GetById(14)->sprite)[9] |= 12;
			Map_CopyCellAttributes(26, 12, 1, 1, 22, 16);
		}
	}
	return 0;
}

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 StagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};

u32 StagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 StagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 TakaraHashira_ActionTable[] = {
	SC_SET(0x23, 2),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(2),
	SC_SET(0x23, 0),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(6),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 TakaraHashira_ShiftSteps1[] = {
	MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps2[] = {
	MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps3[] = {
	MAP_CELL_STEP(67, 34, 2, 5, 6), MAP_CELL_STEP(69, 34, 2, 5, 6), MAP_CELL_STEP(71, 34, 2, 5, 6), MAP_CELL_STEP(73, 34, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps4[] = {
	MAP_CELL_STEP(71, 50, 2, 5, 6), MAP_CELL_STEP(69, 50, 2, 5, 6), MAP_CELL_STEP(67, 50, 2, 5, 6), MAP_CELL_STEP(65, 50, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps5[] = {
	MAP_CELL_STEP(75, 38, 2, 1, 4), MAP_CELL_STEP(77, 38, 2, 1, 6), MAP_CELL_STEP(79, 38, 2, 1, 8), MAP_CELL_STEP(65, 53, 2, 1, 1), MAP_CELL_STEPS_END,
};

struct SceneEntrance gTakaraHashiraEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ 2, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ 2, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ 2, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ SCENE_TABLE_END },
};

u32 gTakaraHashiraExits[] = {
	SCENE_EXITS(115),
	SCENE_EXIT(1, 126, 3),
	SCENE_EXIT(2, 126, 4),
	SCENE_EXIT(0, 116, 0),
	SCENE_EXIT(1, 127, 3),
	SCENE_EXIT(2, 127, 4),
	SCENE_EXIT(0, 119, 0),
	SCENE_EXIT(1, 130, 3),
	SCENE_EXIT(2, 130, 4),
	SCENE_EXIT(0, 121, 0),
	SCENE_EXIT(1, 132, 3),
	SCENE_EXIT(2, 132, 4),
	SCENE_EXIT(0, 122, 0),
	SCENE_EXIT(1, 133, 3),
	SCENE_EXIT(2, 133, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gTakaraHashiraNoPlacements[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements1[] = {
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements2[] = {
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(256), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x129, CONDITION_ALWAYS, 7,
		PIXELS(280), 0, PIXELS(160), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x129, CONDITION_ALWAYS, 7,
		PIXELS(184), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements3[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(352), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd6, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacementsOther[] = {
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(184), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(680), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements5[] = {
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x125, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gTakaraHashiraEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT_1) },
	{ SEARCH(SEARCH_CHEST), 100, 0xec2, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_CHEST), 101, 0xec3, FIND_ITEM(ITEM_NUT) },
	{ SEARCH(SEARCH_CHEST), 102, 0xec4, FIND_COINS(111) },
	{ SEARCH(SEARCH_CHEST), 103, 0xec5, FIND_ITEM(0xc2) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, 0 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT_2) },
	{ SEARCH(SEARCH_CHEST), 100, 0xec6, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_CHEST), 101, 0xec7, FIND_ITEM(0x18) },
	{ SEARCH(SEARCH_CHEST), 102, 0xec8, FIND_COINS(222) },
	{ SEARCH(SEARCH_CHEST), 103, 0xec9, FIND_ITEM(ITEM_ELIXIR) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT_2) },
	{ 0x413, 100, 0xed2, FIND_COINS(555) },
	{ 0xc413, 100, 0xed2, FIND_COINS(555) },
	{ 0x413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ 0xe413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 102, 0xed4, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 103, 0xed5, FIND_ITEM(0x62) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents4[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 30, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_PushPillar) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_RunStagedCellScene) },
	{ 0x202, 31, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_PushPillar) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_TOUCH) },
	{ EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_TOUCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xeda, FIND_COINS(777) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ 0x413, 102, 0xedc, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 103, 0xedd, FIND_ITEM(0x32) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x200, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x201, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x202, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x203, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 8, 0x200, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 9, 0x201, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 10, 0x202, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 11, 0x203, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents5[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_RunPushScene) },
	{ SEARCH(SEARCH_CHEST), 100, 0xede, FIND_COINS(888) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedf, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 102, 0xee0, FIND_ITEM(0x10c) },
	{ SEARCH(SEARCH_CHEST), 103, 0xee1, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ 0x10008c15, 10, 0x204, EVENT_SCRIPT(StagedActor_PlaceAtObjectTenCell) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x204, EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x200, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x201, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ 0x10001815, 13, 0x202, EVENT_SCRIPT(TakaraHashira_CheckLeaderOnPillar) },
	{ 0x50001815, 13, 0x202, EVENT_SCRIPT(TakaraHashira_RaiseActorFourteen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x202, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ SCENE_EVENTS_END },
};
