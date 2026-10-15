#include "game_state.h"
#include "map_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_pillar.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
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
	u8				   unk00[24];
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

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/map_panel.h"

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
		if (obj->y.fixed <= obj->terrainHeight)
		{
			break;
		}
		if (obj->y.fixed <= limit)
		{
			break;
		}
		cnt--;
	}

	obj->velocityY = 0;
	obj->targetY = 0x80000000;
}

static s32 FieldScene_QueryActorFootprint(
	s32 id, s32 *width, s32 *depth, struct StagedActorProbe *probe, s32 *left, s32 *top)
{
	u8				   *map = gMapWork[0];
	struct StagedActor *actor = (struct StagedActor *)Object_GetById(id);
	u32					i;
	s32					a;
	s32					b;

	for (i = 0; i <= 5; i++)
	{
		if (actor->animation->entries[0]->animId == StagedActor_FootprintKinds[i])
		{
			probe->footprintIndex = i;
			break;
		}
		probe->footprintIndex = 7;
	}
	if ((u32)probe->footprintIndex > 6)
	{
		return 0;
	}
	probe->positionX = actor->x.value;
	probe->positionY = actor->y;
	probe->positionZ = actor->z.value;
	a = StagedActor_FootprintBounds[probe->footprintIndex * 4 + 1];
	if (a < 0)
	{
		a = -a;
	}
	b = StagedActor_FootprintBounds[probe->footprintIndex * 4 + 3];
	if (b < 0)
	{
		b = -b;
	}
	*depth = (a + b) >> 4;
	a = StagedActor_FootprintBounds[probe->footprintIndex * 4];
	if (a < 0)
	{
		a = -a;
	}
	b = StagedActor_FootprintBounds[probe->footprintIndex * 4 + 2];
	if (b < 0)
	{
		b = -b;
	}
	*width = (a + b) >> 4;
	probe->positionX += StagedActor_FootprintBounds[probe->footprintIndex * 4] << 16;
	{
		s32 step = StagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] << 16;

		a = probe->positionZ;
		a += step;
		a >>= 20;
		probe->positionZ = a;
	}
	probe->positionX >>= 20;
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
	Map_CopyCellAttributes(2, 2, out20, out16, r2, r4);

	Object_SetMode(obj, 4);
	mask = 2;
	obj->priorityFlags |= mask;

	if (out20 > out16)
	{
		Map_CopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
	}
	else
	{
		Map_CopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
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

		Map_CopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
		StagedActor_FillGridAttributeRectangle(0, rec[2], rec[4], out20, out16, 255);
	}

	Object_SetMode(obj, 1);
	obj->priorityFlags &= 0xfd;

	return 1;
}

static void CrossbonePillar_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst)
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

static void CrossbonePillar_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *source)
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

static s32 CrossbonePillar_LowerActorToLedge(s32 id, s32 far)
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
		top = Map_GetTerrainHeight(actor->terrainId, actor->x.fixed, (row << 20) + actor->z.fixed);
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
			/* FAKEMATCH: a one-pass block around the object and height lets sched2 issue the height arithmetic first, as the native does. Without it: differ 2; with height computed before the call it reallocates the function (differ 99, tbs-en). */
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

	obj->motionFlags = 0;

	cnt = 0;
	for (;;)
	{
		if (cnt > 31)
			return;
		Engine_TaskWait(1);
		obj->scaleY += -0x1999;
		obj->y.fixed += -0xcccc;
		cnt++;
		if (obj->scaleY <= 0x1998)
		{
			obj->scaleY = 0x1999;
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
	flags = &leader->motionFlags;
	saved = *flags;
#if EDITION_INTERNATIONAL
	mode = (leader->facing + 0x2000) & 0xc000;
#endif
	if (gGameState.movementMode != 0)
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
	if (FieldObject_CheckMovementCollision(leader, p) != 1
		&& StagedActor_FindAtTile(p, (struct StagedActor *)leader) == 0)
	{
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
		if (StagedActor_FindAtTile(p, (struct StagedActor *)leader) == 0
			&& FieldObject_CheckMovementCollision(leader, p) == 0)
		{
			Engine_EventBegin();
			Object_SetMode(leader, 6);
			Engine_TaskWait(6);
			Engine_AudioPlayCue(152);
			Object_SetMode(leader, 7);
			leader->speed = 0x30000;
			leader->acceleration = 0x20000;
			leader->velocityY = 0x40000;
			*flags &= 126;
			Engine_ActorSetSpriteFlags(leader, 0);
			Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, ((s16 *)p)[1], ((s16 *)p)[5]);
			Object_SetMode(leader, 6);
			Engine_ActorSetSpriteFlags(leader, 1);
			*flags = saved;
			Engine_EventEnd();
			return 1;
		}
	}
	return 0;
}

static void FieldScene_RunScene3b3SequenceE(union FieldObject *object)
{
	s32 velocityX;
	s32 velocityY;
	s32 velocityZ;

	object->effect.x += object->effect.velocityX;
	object->effect.y += object->effect.velocityY;
	object->effect.z += object->effect.velocityZ;
	velocityX = object->effect.velocityX;
	velocityY = object->effect.velocityY;
	velocityZ = object->effect.velocityZ;
	object->effect.velocityX = velocityX - Math_Divide(velocityX, 10);
	object->effect.velocityY = velocityY - Math_Divide(velocityY, 3);
	object->effect.velocityZ = velocityZ - Math_Divide(velocityZ, 10);
	object->effect.scaleX += object->effect.scaleRateX;
	object->effect.scaleY += object->effect.scaleRateY;
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

	if (scene == (s32)&SceneId_CrossbonePillar1)
	{
		return gCrossbonePillarEntrances1;
	}
	if (scene == (s32)&SceneId_CrossbonePillar2)
	{
		return gCrossbonePillarEntrances2;
	}
	if (scene == (s32)&SceneId_CrossbonePillar3)
	{
		return gCrossbonePillarEntrances3;
	}
	if (scene == (s32)&SceneId_CrossbonePillar4)
	{
		return gCrossbonePillarEntrances4;
	}
	if (scene == (s32)&SceneId_CrossbonePillar5)
	{
		return gCrossbonePillarEntrances5;
	}
	return gCrossbonePillarEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gCrossbonePillarExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_CrossbonePillar1)
	{
		return gCrossbonePillarPlacements1;
	}
	if (scene == (s32)&SceneId_CrossbonePillar2)
	{
		return gCrossbonePillarPlacements2;
	}
	if (scene == (s32)&SceneId_CrossbonePillar3)
	{
		return gCrossbonePillarPlacements3;
	}
	if (scene == (s32)&SceneId_CrossbonePillar5)
	{
		return gCrossbonePillarPlacements5;
	}
	return gCrossbonePillarPlacementsOther;
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

static struct PillarSlot sPillarSlots[4];
static s32				 sProbePosition[3];
static u8				 sShakenScroll[12];
static s32				 sShakeChance;

static void CrossbonePillar_JitterBackgroundScroll(void)
{
	u32			  line = REG_VCOUNT;
	u32			 *src = &((u32 *)gBgScroll)[1];
	volatile u32 *reg = (volatile u32 *)0x04000014;

	if (line == 227 || line <= 46)
	{
		if ((u32)(Engine_RandomNext() * 100) >> 16 < sShakeChance)
		{
			src = ((u32 *)sShakenScroll);
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
	destination = (u32 *)sShakenScroll;
	*destination++ = *source++;
	*destination++ = *source++;
	*destination = *source;
	coordinates = (u16 *)sShakenScroll;
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
	sShakeChance = 0;
	FieldScheduler_AddCallback(CopyAndOffsetCoordinatePreset, 0xc80);
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, CrossbonePillar_JitterBackgroundScroll);
	Engine_AudioPlayCue(231);
	sShakeChance = 0;
	do
	{
		Engine_TaskWait(1);
	} while (++sShakeChance <= 100);
	Engine_AudioPlayCue(0x121);
	if (Engine_GameFlagIsSet(0x200) == 0)
	{
		Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
		Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
	}
	else
	{
		Map_CopyCellsTo(0, 64, 32, 0, 32, 32);
		Map_CopyCellsTo(32, 64, 64, 0, 32, 32);
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
	Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
	queried = GameFlag_IsSet(QUERY_FLAG);
	if (queried == 0)
	{
		Event_PlayCue(232);
		Map_AnimateCells(CrossbonePillar_ShiftSteps1, 84, 24);
		Engine_EventWait(30);
		Event_PlayCue(240);
		Engine_ActorSetSpritePriority(REC_ID, 1);
		Object_GetById(REC_ID)->motionFlags = queried;
		record = Actor_Get(REC_ID);
		record->y.fixed = -0x200000;
		Actor_SetPosition(REC_ID, 0x1100000, 0x1a00000);
		Engine_ActorSetAnimation(REC_ID, 1);
		Map_AnimateCells(CrossbonePillar_ShiftSteps3, 80, 24);
		Map_AnimateCells(CrossbonePillar_ShiftSteps5, 80, 28);
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
		Map_AnimateCells(CrossbonePillar_ShiftSteps2, 84, 24);
		Engine_EventWait(30);
		Event_PlayCue(230);
		Object_GetById(REC_ID)->motionFlags = 0;
		record = Actor_Get(REC_ID);
		record->y.fixed = -0x200000;
		Actor_SetPosition(REC_ID, 0x1100000, 0x1b40000);
		Engine_ActorSetAnimation(REC_ID, 2);
		Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
		Map_AnimateCells(CrossbonePillar_ShiftSteps4, 80, 24);
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

static s32 CrossbonePillar_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front)
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
			actor->priorityFlags &= ~1;
			SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
			SPRITE_OF(actor)->secondPriority = SPRITE_OF(front)->secondPriority;
		}
		result = 1;
	}
	return result;
}

static s32 CrossbonePillar_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor)
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
			actor->priorityFlags &= ~1;
			SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
			SPRITE_OF(actor)->secondPriority = SPRITE_OF(front)->secondPriority;
		}
		result = 1;
	}
	return result;
}

static s32 CrossbonePillar_UpdateActorPriority(struct FieldActor *actor)
{
	struct FieldActor *leader = Object_GetById(0);
	s32				   hit;
	u32				   id;

	if (((s8 *)gEventWork)[0xcc7] == 1)
	{
		actor->sprite->priority = leader->sprite->priority;
		actor->collisionFlags |= 1;
		return 0;
	}
	hit = CrossbonePillar_SyncPriorityIfBehind(actor, leader);
	for (id = 8; id <= 11; id++)
	{
		hit += CrossbonePillar_SyncPriorityIfBehind(actor, Object_GetById(id));
	}
	if (hit != 0)
	{
		for (id = 8; id <= 11; id++)
		{
			CrossbonePillar_SyncPriorityIfAhead(actor, Object_GetById(id));
		}
	}
	if (actor->y.fixed < leader->y.fixed)
	{
		actor->priorityFlags |= 2;
		actor->collisionFlags &= 254;
		if (actor->sprite->priority < leader->sprite->priority)
		{
			actor->priorityFlags &= 254;
			SPRITE_OF(actor)->priority = SPRITE_OF(leader)->priority;
			SPRITE_OF(actor)->secondPriority = SPRITE_OF(leader)->secondPriority;
			hit = 1;
		}
	}
	else
	{
		actor->priorityFlags &= 253;
		actor->collisionFlags |= 1;
	}
	if (hit == 0)
	{
		actor->priorityFlags |= 1;
	}
	return 0;
}

static void CrossbonePillar_SortPillarActors(void)
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
			Dma_Set(&sPillarSlots[j], slot, 0x84000004, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(&sPillarSlots[i], &sPillarSlots[j], 0x84000004, DMA3);
			Dma_Wait(DMA3);
			Dma_Set(slot, &sPillarSlots[i], 0x84000004, DMA3);
			Dma_Wait(DMA3);
			if (Engine_GameFlagIsSet(sPillarSlots[i].flag)
				&& !Engine_GameFlagIsSet(sPillarSlots[j].flag))
			{
				Engine_GameFlagClear(sPillarSlots[i].flag);
				Engine_GameFlagSet(sPillarSlots[j].flag);
			}
			else if (!Engine_GameFlagIsSet(sPillarSlots[i].flag)
					 && Engine_GameFlagIsSet(sPillarSlots[j].flag))
			{
				Engine_GameFlagSet(sPillarSlots[i].flag);
				Engine_GameFlagClear(sPillarSlots[j].flag);
			}
		}
	}
}

struct PillarActor
{
	u8	unk00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unk14[14];
	u8	unk22;
	u8	priorityFlags;
	s32 velocityX;
	s32 velocityY;
	u8	unk2c[0x29];
	u8	motionFlags;
};

static void CrossbonePillar_UpdatePillarActors(void)
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
		actor->unk22 = 2;
		i = id - 8;
		if ((actor->x >> 20) == sPillarSlots[i].x
			&& (actor->z >> 20) == sPillarSlots[i].z && actor->velocityY == 0)
		{
			continue;
		}
		Dma_Set(&actor->x, sProbePosition, 0x84000003, DMA3);
		Dma_Wait(DMA3);
		if (FieldObject_CheckMovementCollision(actor, sProbePosition) == -1)
		{
			actor->motionFlags = 3;
		}
		flags = &actor->motionFlags;
		CrossbonePillar_SetCellAttributes(0, sPillarSlots[i].x,
			sPillarSlots[i].z, (struct MapCell *)&sPillarSlots[i].cell);
		CrossbonePillar_SetCellAttributes(2, sPillarSlots[i].x,
			sPillarSlots[i].z, (struct MapCell *)&sPillarSlots[i].cell);
		if (*flags & 1)
		{
			if (GetMapCellCollision(2, actor->x, actor->z) == 50)
			{
				Engine_AudioPlayCue(189);
				// Both u8 accesses are alias set 0, and the view is no struct reference (MEM_IN_STRUCT_P).
				/* FAKEMATCH: the u8 view keeps these flag updates out of the record's alias set, as native; field stores reallocate the loop (score 2573, differ 217), a priority bit-field differs 65, the record as struct FieldActor with the view kept 2 (it matches once FieldActor's x, y and z are s32; then this PillarActor view can go). */
				*(u8 *)&actor->priorityFlags &= 254;
				CrossbonePillar_LowerActorToLedge(id, 1);
				*(u8 *)&actor->priorityFlags |= 1;
			}
			else if (GetMapCellCollision(2, actor->x, actor->z) == 51)
			{
				SceneActor_WaitHeightBelowLimit((struct FieldActor *)actor, 0);
				Engine_AudioPlayCue(189);
				actor->y = 0;
				actor->priorityFlags &= 254;
				StagedActor_StepDownUntilClamp(id);
				actor->x = 0;
				actor->y = 0;
				actor->z = 0;
				actor->priorityFlags |= 1;
			}
			else
			{
				OverlayObject_WaitUntilSettledAndReset((struct FieldActor *)actor);
			}
			*flags = 0;
		}
		CrossbonePillar_ReadMapCell(
			0, actor->x >> 20, actor->z >> 20, &sPillarSlots[i].cell);
		if (actor->y >= 0)
		{
			CrossbonePillar_ReadMapCell(0, 27, (actor->y >> 20) + 6, &cell);
			CrossbonePillar_SetCellAttributes(
				0, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
			cell.kind = sPillarSlots[i].cell.kind;
			CrossbonePillar_SetCellAttributes(
				2, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
		}
		sPillarSlots[i].x = actor->x >> 20;
		sPillarSlots[i].y = actor->y >> 20;
		sPillarSlots[i].z = actor->z >> 20;
		for (k = 0; k <= 3; k++)
		{
			if (k == i)
			{
				continue;
			}
			Engine_GameFlagClear(sPillarSlots[k].flag);
			other = (struct PillarActor *)Object_GetById(k + 8);
			if ((actor->x >> 20) == (other->x >> 20) && (actor->z >> 20) == (other->z >> 20)
				&& actor->y > other->y)
			{
				Engine_GameFlagSet(sPillarSlots[k].flag);
			}
		}
	}
	CrossbonePillar_SortPillarActors();
}

static void CrossbonePillar_PushPillar(void)
{
	Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Object_GetById(0)->y.fixed = Map_GetTerrainHeight(
		Object_GetById(0)->terrainId, Object_GetById(0)->x.fixed, Object_GetById(0)->z.fixed);
#endif
	if (FieldScene_RunScene3b3SequenceD() == 0)
	{
		Object_GetById(0)->motionFlags &= 254;
		Object_GetById(0)->priorityFlags &= 254;
		StagedActor_AdvancePair();
		CrossbonePillar_UpdatePillarActors();
		Object_GetById(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
		Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	}
	Engine_EventEnd();
}

static void FieldScene_RunSingleStep(void)
{
	battle_owner_69();
}

static void CrossbonePillar_RunStagedCellScene(void)
{
	struct FieldActor *leader = Object_GetById(0);
	s32				   pos[3];
	s32				  *p = pos;

	p[0] = leader->x.fixed;
	p[1] = leader->y.fixed;
	p[2] = leader->z.fixed + 0x100000;
	if (StagedActor_FindAtTile(p, leader) != NULL)
	{
		CrossbonePillar_PushPillar();
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
	obj->riseCounter = 1;
}

static void EV41_TOUCH(void)
{
	struct FieldActor *obj = Object_GetById(ACTOR_PARTY_LEADER);

	PILLAR_WORK->rider = NULL;
	obj->riseCounter = 0;
}

static s32 SceneActor_UpdateBit1ByPositionToSlotZero(struct FieldActor *actor)
{
	struct FieldActor *ref = Object_GetById(ACTOR_PARTY_LEADER);

	if (actor->z.fixed > ref->z.fixed)
	{
		actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
		if (actor->y.fixed < ref->y.fixed)
			actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	}

	return 0;
}

static void CrossbonePillar_RunActorAction(s32 id)
{
	struct FieldActor *actor;

	actor = Object_GetById(id);
	Engine_EventBegin();
	actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
	Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
	GameFlag_Set(id + 0x1f5);
	Engine_ActorEnableActionCallback(id, CrossbonePillar_ActionTable);
	Engine_EventEnd();
}

static void EV11_PSYNERGY(void)
{
	CrossbonePillar_RunActorAction(11);
}

static void EV12_PSYNERGY(void)
{
	CrossbonePillar_RunActorAction(12);
}

static void CrossbonePillar_CheckLeaderOnPillar(void)
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

static void CrossbonePillar_RaiseActorFourteen(void)
{
	struct FieldActor *pillar;

	pillar = Object_GetById(14);
	Object_GetById(13)->scaleX = 0x10000;
	Object_GetById(13)->scaleY = 0x10000;
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
	d->startScaleX = 0x8000;
	d->startScaleY = 0x8000;
	d->targetScaleX = 0x19999;
	d->targetScaleY = 0x19999;
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
	p->startScaleX = 0xb333;
	p->startScaleY = 0xb333;

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
		FieldScheduler_AddCallback((s32)SceneEffect_SpawnRandomizedParticle, 0xc80);
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
			rec7->motionFlags = rec8;
			rec7->terrainHeight = -0x20000;
			rec7->y.fixed = -0x20000;
			Engine_GameFlagSet(0x204);
		}
	}
	Engine_EventEnd();
}

static void CrossbonePillar_RunPushScene(void)
{
	struct StagedActorProbe probe;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);
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

	if (scene == (s32)&SceneId_CrossbonePillar1)
	{
		return gCrossbonePillarEvents1;
	}
	if (scene == (s32)&SceneId_CrossbonePillar2)
	{
		return gCrossbonePillarEvents2;
	}
	if (scene == (s32)&SceneId_CrossbonePillar3)
	{
		return gCrossbonePillarEvents3;
	}
	if (scene == (s32)&SceneId_CrossbonePillar4)
	{
		return gCrossbonePillarEvents4;
	}
	if (scene == (s32)&SceneId_CrossbonePillar5)
	{
		return gCrossbonePillarEvents5;
	}
	return gCrossbonePillarEventsOther;
}

static void CrossbonePillar_PrepLoweredActor(s32 id)
{
	struct FieldActor *actor = Object_GetById(id);

	actor->collisionFlags &= 254;
	actor->priorityFlags |= 2;
	actor->motionFlags = 0;
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->sprite->priority = 2;
}

static void CrossbonePillar_RestoreFlaggedPillar(s32 a0)
{
	struct FieldActor *actor;

	actor = Object_GetById(a0);
	if (GameFlag_IsSet(a0 + 0x1f5) != 0)
	{
		Object_SetMode(actor, 5);
		actor->update = (void (*)(union FieldObject *))SceneActor_UpdateBit1ByPositionToSlotZero;
		Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
		Engine_ActorEnableActionCallback(a0, CrossbonePillar_ActionTable);
	}
}

static void OverlayObject_SetCallbackAndMode2(s32 id)
{
	struct FieldActor *actor = Object_GetById(id);

	actor->terrainId = 2;
	actor->motionFlags = 0;
	actor->update = (void (*)(union FieldObject *))CrossbonePillar_UpdateActorPriority;
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
			Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
		}
	}
}

s32 CrossbonePillar_SetupArea(void)
{
	struct FieldActor *actor;
	u32				   i;
	u32				   id;
	s32				   x;
	s32				   z;

	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_CrossbonePillar2)
	{
		FieldScene_RedrawActorFootprint(8);
		FieldScene_RedrawActorFootprint(9);
		FieldScene_RedrawActorFootprint(10);
		FieldScene_RedrawActorFootprint(11);
		FieldScene_RedrawActorFootprint(12);
	}
	else if (gGameState.scene == (s32)&SceneId_CrossbonePillar3)
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
			Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
			Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
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
	else if (gGameState.scene == (s32)&SceneId_CrossbonePillar4)
	{
		OverlayObject_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200), 223);
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			Object_GetById(ACTOR_PARTY_LEADER)->riseCounter = 1;
		}
		BattleFx_StartFadeOverlay(0);
		if (Object_GetById(ACTOR_PARTY_LEADER)->riseCounter == 0)
		{
			EV41_TOUCH();
		}
		OverlayObject_SetCallbackAndMode2(8);
		OverlayObject_SetCallbackAndMode2(9);
		OverlayObject_SetCallbackAndMode2(10);
		OverlayObject_SetCallbackAndMode2(11);
		{
			s32 *entry = (s32 *)sPillarSlots;

			for (i = 0; i < 4; i++)
			{
				entry[0] = 0;
				entry[1] = 0;
				entry[2] = 0;
				entry[4] = i + 0x200;
				entry += 5;
			}
		}
		CrossbonePillar_UpdatePillarActors();
		Engine_EventWait(1);
		FieldScheduler_AddCallback(SceneActor_CheckActors8To11NearSlotZero, TASK_PRIORITY_SCENE);
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
						Map_CopyCellAttributes(27, 8, 1, 1, x, z);
						break;
					}
				}
			}
		}
	}
	else if (gGameState.scene == (s32)&SceneId_CrossbonePillar5)
	{
		Engine_ActorSetAnimation(10, 2);
		Engine_ActorSetChildValue(10, 6);
		FieldScene_RedrawActorFootprint(8);
		FieldScene_RedrawActorFootprint(9);
#if EDITION_INTERNATIONAL
		Object_GetById(8)->motionFlags = 0;
		Object_GetById(9)->motionFlags = 0;
#endif
		EV10_PSYNERGY();
		CrossbonePillar_PrepLoweredActor(11);
		CrossbonePillar_PrepLoweredActor(12);
		CrossbonePillar_PrepLoweredActor(13);
		CrossbonePillar_RestoreFlaggedPillar(11);
		CrossbonePillar_RestoreFlaggedPillar(12);
		CrossbonePillar_RestoreFlaggedPillar(13);
		actor = Object_GetById(13);
		actor->update = 0;
		CrossbonePillar_PrepLoweredActor(14);
		Object_GetById(14)->collisionFlags |= 8;
		if (Engine_GameFlagIsSet(0x202) == 0)
		{
			Object_GetById(13)->scaleX = 0x18000;
			Object_GetById(13)->scaleY = 0x18000;
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

s32 CrossbonePillar_ActionTable[] = {
	SC_SET(0x23, 2),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(2),
	SC_SET(0x23, 0),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(6),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 CrossbonePillar_ShiftSteps1[] = {
	MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 CrossbonePillar_ShiftSteps2[] = {
	MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 CrossbonePillar_ShiftSteps3[] = {
	MAP_CELL_STEP(67, 34, 2, 5, 6), MAP_CELL_STEP(69, 34, 2, 5, 6), MAP_CELL_STEP(71, 34, 2, 5, 6), MAP_CELL_STEP(73, 34, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 CrossbonePillar_ShiftSteps4[] = {
	MAP_CELL_STEP(71, 50, 2, 5, 6), MAP_CELL_STEP(69, 50, 2, 5, 6), MAP_CELL_STEP(67, 50, 2, 5, 6), MAP_CELL_STEP(65, 50, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 CrossbonePillar_ShiftSteps5[] = {
	MAP_CELL_STEP(75, 38, 2, 1, 4), MAP_CELL_STEP(77, 38, 2, 1, 6), MAP_CELL_STEP(79, 38, 2, 1, 8), MAP_CELL_STEP(65, 53, 2, 1, 1), MAP_CELL_STEPS_END,
};

struct SceneEntrance gCrossbonePillarEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePillarEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ 2, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePillarEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ 2, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePillarEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePillarEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ 2, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePillarEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ SCENE_TABLE_END },
};

u32 gCrossbonePillarExits[] = {
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

struct ScenePlacement gCrossbonePillarNoPlacements[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePillarPlacements1[] = {
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

struct ScenePlacement gCrossbonePillarPlacements2[] = {
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

struct ScenePlacement gCrossbonePillarPlacements3[] = {
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

struct ScenePlacement gCrossbonePillarPlacementsOther[] = {
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

struct ScenePlacement gCrossbonePillarPlacements5[] = {
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

struct SceneEvent gCrossbonePillarEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePillarEvents1[] = {
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

struct SceneEvent gCrossbonePillarEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT_2) },
	{ SEARCH(SEARCH_CHEST), 100, 0xec6, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_CHEST), 101, 0xec7, FIND_ITEM(0x18) },
	{ SEARCH(SEARCH_CHEST), 102, 0xec8, FIND_COINS(222) },
	{ SEARCH(SEARCH_CHEST), 103, 0xec9, FIND_ITEM(ITEM_ELIXIR) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePillarEvents3[] = {
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

struct SceneEvent gCrossbonePillarEvents4[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 30, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePillar_PushPillar) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePillar_RunStagedCellScene) },
	{ 0x202, 31, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePillar_PushPillar) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_TOUCH) },
	{ EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_TOUCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xeda, FIND_COINS(777) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ 0x413, 102, 0xedc, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 103, 0xedd, FIND_ITEM(0x32) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x200, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x201, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x202, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x203, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 8, 0x200, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 9, 0x201, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 10, 0x202, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 11, 0x203, EVENT_SCRIPT(CrossbonePillar_UpdatePillarActors) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePillarEvents5[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePillar_RunPushScene) },
	{ SEARCH(SEARCH_CHEST), 100, 0xede, FIND_COINS(888) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedf, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 102, 0xee0, FIND_ITEM(0x10c) },
	{ SEARCH(SEARCH_CHEST), 103, 0xee1, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ 0x10008c15, 10, 0x204, EVENT_SCRIPT(StagedActor_PlaceAtObjectTenCell) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x204, EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x200, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x201, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ 0x10001815, 13, 0x202, EVENT_SCRIPT(CrossbonePillar_CheckLeaderOnPillar) },
	{ 0x50001815, 13, 0x202, EVENT_SCRIPT(CrossbonePillar_RaiseActorFourteen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x202, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ SCENE_EVENTS_END },
};
