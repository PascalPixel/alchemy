#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/kuupuappu_dou/dou.h"
#include "unit/battle/effect/slots.h"
#include "unit/system/scheduler/runtime.h"
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
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "callback_scheduler.h"
#include "call.h"
#include "object_runtime.h"
#include "ability_ids.h"
#include "objcmd.h"

s32 Math_RemainderUnsigned();

struct MapShake
{
	u8	unknown_00[0x18];
	s32 direction_x;
	s32 direction_z;
	u8	unknown_20[4];
	s32 scroll;
};

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif
void QueueIoWriteDelay2(u32 address, s32 value);

#define TILE(position) ((position) / 0x100000)

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KuupuappuDou1)
	{
		return gKuupuappuDouEntrances1;
	}
	if (scene == (s32)&SceneId_KuupuappuDou2)
	{
		return gKuupuappuDouEntrances2;
	}
	if (scene == (s32)&SceneId_KuupuappuDou3)
	{
		return gKuupuappuDouEntrances3;
	}
	return gKuupuappuDouEntrancesOther;
}

u8 *KuupuappuDou_GetRegions(void)
{
	return (u8 *)gKuupuappuDouRegions;
}

const u32 *Scene_GetExits(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KuupuappuDou1)
	{
		return gKuupuappuDouExits1;
	}
	if (scene == (s32)&SceneId_KuupuappuDou2)
	{
		return gKuupuappuDouExits2;
	}
	if (scene == (s32)&SceneId_KuupuappuDou3)
	{
		return gKuupuappuDouExits3;
	}
	return gKuupuappuDouExitsOther;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KuupuappuDou1)
	{
		return gKuupuappuDouPlacements1;
	}
	if (scene == (s32)&SceneId_KuupuappuDou2)
	{
		return gKuupuappuDouPlacements2;
	}
	if (scene == (s32)&SceneId_KuupuappuDou3)
	{
		return gKuupuappuDouPlacements3;
	}
	return gKuupuappuDouPlacementsOther;
}

s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz)
{
	s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
	u32	  i;

	for (i = 8; i <= 65; i++)
	{
		s32 *p = slots[i];

		if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0)
		{
			return p;
		}
	}
	return 0;
}

struct DirectionSteps
{
	s32 xz[16];
};

#define DIRECTION_STEP(heading) \
	(((struct DirectionSteps *)KuupuappuDou_DirectionSteps)->xz[heading])

void KuupuappuDou_PushBlockAhead(void)
{
	/* FAKEMATCH: the table is s16 pairs, read here as one word per heading
	   (x in the high half, z in the low). The read must stay an array
	   reference: through a plain s32 pointer cast the address is summed once
	   into a register and the function no longer matches. */
	struct FieldActor	  *leader = Object_GetById(0);
	s32					   direction = leader->facing >> 12;
	struct FieldActor	  *block;
	struct FieldActor	  *beyond;
	union FieldCoordinate  pos[3];
	union FieldCoordinate *p;
	s32					   zero;
	s32					   i;
	s32					   cx;
	s32					   cz;

	cx = (leader->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
	cz = (leader->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
	block = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
	if (block->collision_flags == 0 || block == 0)
	{
		return;
	}
	for (i = 0; i <= 3; i++)
	{
		if (block == Object_GetById(i + 11))
		{
			return;
		}
	}
	cx = (block->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
	cz = (block->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
	beyond = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
	if (beyond != 0 && (beyond->collision_flags & 1))
	{
		return;
	}
	block->terrain_id = 2;
	zero = 0;
	p = pos;
	p[0].fixed = block->x.fixed + (DIRECTION_STEP(direction) & 0xffff0000);
	p[1].fixed = block->y.fixed;
	p[2].fixed = block->z.fixed + (DIRECTION_STEP(direction) << 16);
	if (Object_CheckMovementCollision(block, p) > 0)
	{
		return;
	}
	Object_SetMode(leader, 8);
	Engine_TaskWait(15);
	Engine_AudioPlayCue(238);
	block->speed = 0x3333;
	block->acceleration = 0x3333;
	Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
	leader->speed = 0x3333;
	leader->acceleration = 0x3333;
	Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
	Engine_ObjectCommitPosition(block);
	Engine_AudioPlayCue(288);
	block->x.fixed = p[0].fixed;
	block->z.fixed = p[2].fixed;
	block->velocity_x = zero;
	block->velocity_z = zero;
	leader->velocity_x = zero;
	leader->velocity_z = zero;
	leader->target_x = ACTOR_NO_TARGET;
	leader->target_z = ACTOR_NO_TARGET;
	Object_SetMode(leader, 1);
}

void SceneState_ApplyFlag300(void)
{
	Engine_GameFlagSet(0x300);
}

void KuupuappuDou_ShowLockedDoorMessage(void)
{
	Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KuupuappuDou1)
	{
		return gKuupuappuDouEvents1;
	}
	if (scene == (s32)&SceneId_KuupuappuDou2)
	{
		return gKuupuappuDouEvents2;
	}
	if (scene == (s32)&SceneId_KuupuappuDou3)
	{
		return gKuupuappuDouEvents3;
	}
	return gKuupuappuDouEventsOther;
}

s32 IsActor9AtTile15x54(void)
{
	struct FieldActor *actor = Object_GetById(9);
	s32				   z = actor->z.fixed;
	s32				   x;
	s32				   z_tile;
	s32				   x_tile;

	if (z < 0)
	{
		z += 0x000FFFFF;
	}
	x = actor->x.fixed;
	z_tile = z >> 20;
	if (x < 0)
	{
		x += 0x000FFFFF;
	}
	x_tile = x >> 20;
	if (x_tile == 15 && z_tile == 54)
	{
		return 1;
	}
	return 0;
}

void FieldScene_RunFlag9a9GuardedScene(void)
{
	if (GameFlag_IsSet(0x9a9) == 0)
	{
		KuupuappuDou_PushBlockAhead();
		if (IsActor9AtTile15x54() != 0)
		{
			GameFlag_Set(0x9a9);
			Event_PlayCue(80);
			SceneState_ApplyThreeRects();
		}
	}
}

void SceneState_ApplyThreeRects(void)
{
	s32 strip = 16;

	{
		s32 fifth = 80;
		s32 sixth = 50;

		Engine_MapCopyCells(87, 50, 2, 4, fifth, sixth);
	}
	Engine_MapCopyCells(23, 52, 1, 2, strip, 52);
	Engine_MapCopyCellAttributes(16, 52, 1, 1, strip, 53);
}

void FieldScene_RunScene3a7SequenceA(void)
{
	if (GameFlag_IsSet(0x9a9) == 0)
	{
		if (IsActor9AtTile15x54() != 0)
		{
			GameFlag_Set(0x9a9);
			Event_PlayCue(80);
			SceneState_ApplyThreeRects();
		}
	}
}

void FieldScene_NoOp(void)
{
}

void SceneState_ApplyThreeRectsRows9And10(void)
{
	s32 strip = 17;

	{
		s32 p5 = 80;
		s32 p6 = 9;

		Engine_MapCopyCells(90, 9, 2, 3, p5, p6);
	}
	Engine_MapCopyCells(27, 10, 1, 2, strip, 10);
	Engine_MapCopyCellAttributes(17, 10, 1, 1, strip, 11);
}

s32 SceneActor_IsActor10AtTile16x12(void)
{
	struct FieldActor *p = Object_GetById(10);
	s32				   z = p->z.fixed;
	s32				   x;
	s32				   cz;
	s32				   cx;

	if (z < 0)
	{
		z += 0x000FFFFF;
	}
	x = p->x.fixed;
	cz = z >> 20;
	if (x < 0)
	{
		x += 0x000FFFFF;
	}
	cx = x >> 20;
	if (cx == 16 && cz == 12)
	{
		return 1;
	}
	return 0;
}

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
	KuupuappuDou_PushBlockAhead();
	if (GameFlag_IsSet(0x9aa) == 0)
	{
		if (SceneActor_IsActor10AtTile16x12() != 0)
		{
			if (GameFlag_IsSet(0x207) == 0)
			{
				Event_PlayCue(80);
				SceneState_ApplyThreeRectsRows9And10();
				GameFlag_Set(0x9aa);
			}
		}
	}
}

void Resource3a7_NoOpCallback(void)
{
}

void FieldScene_RunGuardedStep9AA(void)
{
	if (GameFlag_IsSet(0x9aa) == 0)
	{
		if (SceneActor_IsActor10AtTile16x12() != 0)
		{
			if (GameFlag_IsSet(0x207) == 0)
			{
				Event_PlayCue(80);
				SceneState_ApplyThreeRectsRows9And10();
				GameFlag_Set(0x9aa);
			}
		}
	}
}

void SceneState_ApplyRectAndMarkActor16(void)
{
	struct FieldActor *rec = Object_GetById(16);

	s32 fifth = 23;
	s32 sixth = 32;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

	if (rec != 0)
	{

		Object_GetById(16)->motion_flags = 0;
		rec->priority_flags = 1;
	}

	Engine_GameFlagSet(0x200);
}

void SceneState_ConfigureRegion26_30AndMarkActor17(void)
{
	struct FieldActor *rec = Object_GetById(17);

	s32 p5 = 23;
	s32 p6 = 34;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, p5, p6);

	if (rec != 0)
	{

		Object_GetById(17)->motion_flags = 0;
		rec->priority_flags = 1;
	}

	Engine_GameFlagSet(0x201);
}

void SceneState_ConfigureRegion26_30AndClearActor18Mode(void)
{
	struct FieldActor *record = Object_GetById(18);

	s32 a = 24;
	s32 b = 34;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, a, b);

	if (record != 0)
	{

		Object_GetById(18)->motion_flags = 0;
		record->priority_flags = 1;
	}

	Engine_GameFlagSet(0x202);
}

void SceneState_ApplyRectAndSetupActor19(void)
{
	struct FieldActor *p = Object_GetById(19);

	s32 a5 = 26;
	s32 a6 = 32;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, a5, a6);

	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);

		Object_GetById(19)->motion_flags = 0;
		p->priority_flags = 1;
	}

	Engine_GameFlagSet(0x203);
}

void SceneActor_SetupSlotTwenty(void)
{
	struct FieldActor *rec = Object_GetById(20);

	s32 fifth = 26;
	s32 sixth = 34;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

	if (rec != 0)
	{
		Engine_ActorSetSpriteFlags(rec, 0);

		Object_GetById(20)->motion_flags = 0;
		rec->priority_flags = 1;
	}

	Engine_GameFlagSet(0x204);
}

void SceneActor_MarkSlot21AndSetFlag205(void)
{
	struct FieldActor *record = Object_GetById(21);

	s32 fifth = 28;
	s32 sixth = 33;

	Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

	if (record != 0)
	{
		Engine_ActorSetSpriteFlags(record, 0);

		Object_GetById(21)->motion_flags = 0;
		record->priority_flags = 1;
	}

	Engine_GameFlagSet(0x205);
}

void SceneState_DispatchByActorZeroDepth(void)
{
	struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);

	if (p->y.fixed >= 0x100000)
	{
		KuupuappuDou_RaiseActorPriorities();
	}
	else
	{
		KuupuappuDou_EnableGroupAutomaticPriority();
	}
}

void KuupuappuDou_RaiseActorPriorities(void)
{
	s32 i;
	s32 id;

	id = 16;
	for (i = 0; i < 6; i++)
	{
		Object_GetById(id)->priority_flags |= 2;
		id++;
	}
}

void KuupuappuDou_EnableGroupAutomaticPriority(void)
{
	s32 index = 16;
	s32 flag = 1;
	s32 remaining = 5;

	do
	{
		struct FieldActor *entry = Object_GetById(index);

		remaining--;
		entry->priority_flags = flag;
		index++;
	} while (remaining >= 0);
}

void KuupuappuDou_RunRumble(void)
{
	struct MapShake *shake = (struct MapShake *)(((u8 *)gMapWork[0]) + 0x164);
	s32				 frames;
	s32				 eva;
	s32				 evb;
	s32				 top;

	Engine_EventBegin();
	if (gFrameCount & 1)
	{
		shake->direction_x = 1;
		shake->direction_z = 1;
	}
	else
	{
		shake->direction_x = -1;
		shake->direction_z = -1;
	}
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Event_PlayCue(163);
	for (frames = 0x1df; frames >= 0; frames--)
	{
		u32 drift = (u32)(Random_Next() << 11) >> 16;

		shake->scroll = (double)shake->scroll - (4718.592 - (double)drift);
		Engine_EventWait(1);
	}
	eva = 6;
	evb = 6;
	frames = 0;
	top = eva << 10;
	do
	{
		QueueIoWriteDelay2(0x4000052, top | (eva << 5) | evb);
		Engine_EventWait(1);
		if (frames % 20 == 0)
		{
			evb--;
			eva--;
		}
		frames++;
	} while (frames <= 69);
	Map_CopyCells(19, 83, 15, 8, 19, 91);
	Event_PlayCue(0x120);
	Engine_MapRedraw();
	Engine_MapRenderWaitForValues();
	Engine_EventEnd();
}

void FieldScene_RunScene3a7SequenceB(void)
{
	s32 v5;
	s32 v6;

	if (GameFlag_IsSet(0x9a8) == 0)
	{
		Engine_MessageShowCentered(MSGNO(MsgFieldFlippedSwitch), 1);
		GameFlag_Set(0x9a8);
		v5 = 27;
		v6 = 92;
		Event_PlayCue(155);
		Map_CopyCells(107, 27, 1, 1, v6, v5);
		Engine_EventWait(39);
		Map_CopyCells(108, 27, 1, 1, v6, v5);
		Engine_EventWait(50);
		v6 = 25;
		Event_PlayCue(156);
		Map_CopyCells(1, 24, 1, 2, v6, v5);
		Engine_EventWait(40);
		Map_CopyCells(2, 24, 1, 2, v6, v5);
		Engine_EventWait(40);
		KuupuappuDou_RunRumble();
	}
}

s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor)
{
	struct FieldActor *subject = Object_GetById(subject_actor);
	s32				   index = 0;

	do
	{
		struct FieldActor *actor = Object_GetById(index + 11);

		if ((u32)(actor->y.fixed - 1) <= 0x000ffffe)
		{
			s32 actor_z = TILE(actor->z.fixed);
			s32 actor_x = TILE(actor->x.fixed);
			s32 subject_z = TILE(subject->z.fixed);
			s32 subject_x = TILE(subject->x.fixed);
			s32 z_delta = subject_z - actor_z;
			s32 x_delta = subject_x - actor_x;

			if (x_delta == 0 && z_delta == 0)
			{
				actor->y.fixed = 0x00ff0000;
				actor->gravity = 0;
				actor->velocity_y = 0;
				return 1;
			}
		}

		index++;
	} while (index <= 3);

	return 0;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
	s32				   i;
	struct FieldActor *rec7;
	s32				   flag;
	s32				   index;

	flag = Object_GetById(10)->paused;
	if (flag == 0)
	{
		if (++KuupuappuDou_ScheduleTimer > 190)
		{
			KuupuappuDou_ScheduleTimer = 0;
		}
		index = KuupuappuDou_ScheduleIndex;
		if (KuupuappuDou_ScheduleFrames[index] == KuupuappuDou_ScheduleTimer)
		{
			rec7 = Actor_Get((index + 11));
			rec7->gravity = 0xa3d;
			if (++KuupuappuDou_ScheduleIndex > 3)
			{
				KuupuappuDou_ScheduleIndex = 0;
			}
		}
		for (i = 0; i <= 3; i++)
		{
			rec7 = Actor_Get((i + 11));
			if (rec7->velocity_y >= 0)
			{
				if (rec7->y.fixed <= 0xffff)
				{
					KuupuappuDou_SpawnPuffs(rec7);
					rec7->y.fixed = 0xff0000;
					rec7->gravity = 0;
					rec7->velocity_y = 0;
					rec7->paused = 0;
					Event_PlayCue(106);
				}
			}
		}
		if (SceneActor_LiftLowActorOnSubjectTile(10) != 0)
		{
			Engine_ActorSetAnimation(10, 1);
			if (GameFlag_IsSet(0x207) == 0)
			{
				GameFlag_Set(0x207);
				Event_PlayCue(204);
			}
			else
			{
				Event_PlayCue(106);
			}
		}
		if (SceneActor_LiftLowActorOnSubjectTile(9) != 0)
		{
			Event_PlayCue(106);
		}
	}
}

void FieldScene_RunScene3a7SequenceD(void)
{

	s32				   i;
	struct FieldActor *rec7;
	s32				   record;
	s32				  *selected;

	rec7 = Object_GetById(10);
	if (rec7->paused == 0)
	{
		if ((++KuupuappuDou_TickCounter & 63) == 0)
		{
			selected = &KuupuappuDou_TickValue;
			record = Engine_RandomNext();
			record = Math_RemainderUnsigned(record, 6);
			*selected = record;
			rec7 = Object_GetById((record + 10));
			rec7->gravity = 0xa3d;
		}
		for (i = 0; i <= 5; i++)
		{
			rec7 = Object_GetById((i + 10));
			record = Engine_GameFlagIsSet((i + 0x200));
			if (record != 0)
			{
				if (rec7->velocity_y <= 0)
				{
					if (rec7->y.fixed > 0x20ffff)
					{
						continue;
					}
				}
				rec7->y.fixed = 0xff0000;
				rec7->gravity = 0;
				rec7->velocity_y = 0;
				Engine_AudioPlayCue(106);
			}
			else
			{
				if (rec7->velocity_y <= 0)
				{
					if (rec7->y.fixed > 0xffff)
					{
						continue;
					}
				}
				rec7->gravity = record;
				rec7->velocity_y = record;
				rec7->y.fixed = 0xff0000;
				Engine_AudioPlayCue(106);
			}
		}
	}
}

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
	s32 pos[3];

	if (rec == 0)
	{
		return;
	}
	pos[0] = rec[2];
	pos[1] = rec[3];
	pos[2] = rec[4];
	Vector_AddPolarOffset(v0, v1, pos);
	Engine_ObjectSetPosition(rec, pos[0], pos[1], pos[2]);
}

void KuupuappuDou_SpawnPuffs(struct FieldActor *source)
{
	struct FieldActor *puff;
	s32				   i;

	for (i = 0; i < 4; i++)
	{
		puff = Engine_ObjectCreate(240, source->x.fixed, source->y.fixed, source->z.fixed);
		if (puff == 0)
			break;
		puff->scale_y = 0x8ccc;
		puff->scale_x = 0x8ccc;
		puff->motion_flags = 2;
		puff->velocity_y = -0x10000;
		puff->speed = Engine_RandomNext() + 0xcccc;
		puff->collision_flags = 1;
		SceneActor_TransformAndApplyRecordPosition(puff, 0x200000, Engine_RandomNext());
		{
			u16 *timer = (u16 *)&puff->unknown_5d[1];
			s32	 frames = 8;

			*timer = frames;
		}
		Engine_ObjectSetScript(puff, KuupuappuDou_PuffScript);
	}
}

void SceneActor_InitSlots10To15AndStartTask(void)
{
	s32 selector = 10;
	s32 remaining = 5;

	do
	{
		struct FieldActor *record;

		Engine_ActorSetSpriteFlags(Object_GetById(selector), 0);
		record = Object_GetById(selector);
		record->bounce = 0x1999;
		record->gravity = 0;
		remaining--;
		record->y.fixed = 0x00ff0000;
		selector++;
	} while (remaining >= 0);

	{
		s32 rank = 0xc80;

		Engine_TaskAddCallback((s32)FieldScene_RunScene3a7SequenceD, rank);
	}
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
	s32 no = 11;
	s32 i = 0;

	do
	{
		struct FieldActor *rec;

		Engine_ActorSetSpriteFlags(Object_GetById(no), 0);
		rec = Object_GetById(no);
		rec->bounce = 0x1999;
		rec->gravity = 0;
		rec->y.fixed = 0x00ff0000;
		Engine_ActorSetSpritePriority(i + 11, 1);
		i++;
		no++;
	} while (i <= 3);

	{
		s32 rate = 0xc80;

		Engine_TaskAddCallback((s32)FieldScene_RunOpeningAuxiliarySequence, rate);
	}
}

s32 KuupuappuDou_ApplyEntryState(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_KuupuappuDou1)
	{
		switch (gGameState.entrance)
		{
		case 5:
		case 6:
		case 7:
		case 8:
		case 13:
			if (Engine_GameFlagIsSet(0x9a8) == 0)
			{
				Map_CopyCellAttributes(22, 29, 1, 1, 21, 29);
			}
			else
			{
				Engine_MapCopyCells(108, 27, 1, 1, 92, 27);
				Engine_TaskWait(1);
				Map_CopyCells(19, 83, 15, 8, 19, 91);
				Engine_TaskWait(1);
				Engine_MapCopyCells(2, 24, 1, 2, 25, 27);
			}
			Engine_MapRedraw();
			Engine_TaskWait(1);
			break;
		case 10:
			Engine_GameFlagClear(0x9a8);
			break;
		}
	}
	if (gGameState.scene == (s32)&SceneId_KuupuappuDou2)
	{
		if (Engine_GameFlagIsSet(0x300) == 0)
		{
			Object_GetById(22)->scale_y = 0x18000;
		}
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			if (Engine_GameFlagIsSet(0x9a8) == 0)
			{
				Map_CopyCells(5, 81, 11, 7, 5, 73);
			}
			else
			{
				Map_CopyCellAttributes(5, 12, 1, 1, 6, 12);
				Engine_MapCopyCellAttributes(12, 10, 1, 1, 12, 11);
			}
			break;
		case 8:
		case 9:
		case 14:
			SceneActor_InitSlots10To15AndStartTask();
			if (Engine_GameFlagIsSet(0x200) != 0)
			{
				Engine_ActorSetAnimation(16, 5);
				SceneState_ApplyRectAndMarkActor16();
			}
			if (Engine_GameFlagIsSet(0x201) != 0)
			{
				Engine_ActorSetAnimation(17, 5);
				SceneState_ConfigureRegion26_30AndMarkActor17();
			}
			if (Engine_GameFlagIsSet(0x202) != 0)
			{
				Engine_ActorSetAnimation(18, 5);
				SceneState_ConfigureRegion26_30AndClearActor18Mode();
			}
			if (Engine_GameFlagIsSet(0x203) != 0)
			{
				Engine_ActorSetAnimation(19, 5);
				SceneState_ApplyRectAndSetupActor19();
			}
			if (Engine_GameFlagIsSet(0x204) != 0)
			{
				Engine_ActorSetAnimation(20, 5);
				SceneActor_SetupSlotTwenty();
			}
			if (Engine_GameFlagIsSet(0x205) != 0)
			{
				Engine_ActorSetAnimation(21, 5);
				SceneActor_MarkSlot21AndSetFlag205();
			}
			Engine_TaskAddCallback((s32)SceneState_DispatchByActorZeroDepth, 0xc80);
			break;
		case 10:
		case 11:
			if (Engine_GameFlagIsSet(0x9a9) != 0)
			{
				SceneState_ApplyThreeRects();
				Actor_SetPosition(9, 0xf80000, 0x36c0000);
			}
			Object_GetById(8)->priority_flags = 2;
			break;
		}
		Engine_ActorSetAnimation(8, 2);
		Engine_ActorSetAnimation(9, 2);
		Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		Object_GetById(9)->collision_flags = 1;
	}
	if (gGameState.scene == (s32)&SceneId_KuupuappuDou3)
	{
		Engine_ActorSetAnimation(8, 2);
		if (Engine_GameFlagIsSet(0x207) == 0)
		{
			Engine_ActorSetAnimation(10, 2);
		}
		Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		Object_GetById(10)->collision_flags |= 0x80;
		Object_GetById(9)->collision_flags |= 0x80;
		switch (gGameState.entrance)
		{
		case 5:
		case 6:
			SceneActor_SetupActors11To14AndInstallTask();
			Object_GetById(11)->collision_flags = 2;
			Object_GetById(12)->collision_flags = 2;
			Object_GetById(13)->collision_flags = 2;
			Object_GetById(14)->collision_flags = 2;
			Object_GetById(8)->collision_flags = 1;
			Object_GetById(10)->collision_flags = 1;
			Object_GetById(9)->collision_flags = 1;
			if (Engine_GameFlagIsSet(0x9aa) != 0)
			{
				SceneState_ApplyThreeRectsRows9And10();
				Actor_SetPosition(10, 0x1080000, 0xcc0000);
			}
			break;
		}
	}
	gGameState.retreat_entrance = 10;
	gGameState.retreat_scene = (s32)&SceneId_KuupuappuDou1;
	return 0;
}

const s16 KuupuappuDou_DirectionSteps[] = {
	0, 16,
	0, 16,
	0, 16,
	16, 0,
	16, 0,
	16, 0,
	16, 0,
	0, -16,
	0, -16,
	0, -16,
	0, -16,
	-16, 0,
	-16, 0,
	-16, 0,
	-16, 0,
	0, 16,
};

const s32 KuupuappuDou_PuffScript[] = {
	SC_FREE,
};

struct SceneEntrance gKuupuappuDouEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKuupuappuDouEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 256, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
	{ 4, CONDITION_ALWAYS, 184, 0, 264, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
	{ 5, CONDITION_ALWAYS, 344, 0, 464, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ 6, CONDITION_ALWAYS, 488, 0, 488, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ 7, CONDITION_ALWAYS, 584, 0, 608, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ 8, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 280, 360, 640, 640, 0 },
	{ 13, CONDITION_ALWAYS, 408, 0, 472, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
	{ 9, CONDITION_ALWAYS, 88, 0, 704, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
	{ 12, CONDITION_ALWAYS, 184, 0, 864, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
	{ 10, CONDITION_ALWAYS, 456, 0, 800, FACING_SOUTH, 0, 336, 696, 576, 856, 0 },
	{ 11, CONDITION_ALWAYS, 456, 0, 840, FACING_NORTH, 0, 336, 696, 576, 856, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKuupuappuDouEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
	{ 2, CONDITION_ALWAYS, 344, 0, 152, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
	{ 3, CONDITION_ALWAYS, 344, 0, 184, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
	{ 4, CONDITION_ALWAYS, 168, 0, 328, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
	{ 5, CONDITION_ALWAYS, 56, 0, 480, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ 6, CONDITION_ALWAYS, 152, 0, 472, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 576, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
	{ 8, CONDITION_ALWAYS, 344, 0, 456, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
	{ 9, CONDITION_ALWAYS, 424, 0, 664, FACING_NORTH, 0, 280, 344, 544, 672, 0 },
	{ 14, CONDITION_ALWAYS, 456, 0, 472, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
	{ 10, CONDITION_ALWAYS, 200, 0, 776, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
	{ 11, CONDITION_ALWAYS, 264, 0, 808, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
	{ 13, CONDITION_ALWAYS, 536, 0, 168, FACING_NORTH, 0, 416, 24, 656, 184, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKuupuappuDouEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
	{ 3, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
	{ 4, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
	{ 5, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
	{ 6, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
	{ SCENE_TABLE_END },
};

s16 gKuupuappuDouRegions[] = {
	447, 47, 783, 463, 63, 799, -1, 10,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gKuupuappuDouExitsOther[] = {
	SCENE_EXITS(511),
};

u32 gKuupuappuDouExits1[] = {
	SCENE_EXITS(96),
	SCENE_EXIT(1, 98, 2),
	SCENE_EXIT(2, 97, 1),
	SCENE_EXIT(3, 96, 5),
	SCENE_EXIT(4, 96, 13),
	SCENE_EXIT(5, 96, 3),
	SCENE_EXIT(6, 97, 6),
	SCENE_EXIT(7, 97, 10),
	SCENE_EXIT(8, 96, 9),
	SCENE_EXIT(9, 96, 8),
	SCENE_EXIT(10, 23, 13),
	SCENE_EXIT(10, 96, 11),
	SCENE_EXIT(11, 96, 12),
	SCENE_EXIT(12, 96, 11),
	SCENE_EXIT(13, 96, 4),
	SCENE_EXITS_END,
};

u32 gKuupuappuDouExits2[] = {
	SCENE_EXITS(97),
	SCENE_EXIT(1, 96, 2),
	SCENE_EXIT(2, 98, 6),
	SCENE_EXIT(3, 97, 8),
	SCENE_EXIT(4, 97, 5),
	SCENE_EXIT(5, 97, 4),
	SCENE_EXIT(6, 96, 6),
	SCENE_EXIT(7, 98, 3),
	SCENE_EXIT(8, 97, 3),
	SCENE_EXIT(9, 97, 11),
	SCENE_EXIT(10, 96, 7),
	SCENE_EXIT(11, 97, 9),
	SCENE_EXIT(13, 97, 13),
	SCENE_EXIT(14, 97, 14),
	SCENE_EXITS_END,
};

u32 gKuupuappuDouExits3[] = {
	SCENE_EXITS(98),
	SCENE_EXIT(1, 23, 14),
	SCENE_EXIT(2, 96, 1),
	SCENE_EXIT(3, 97, 7),
	SCENE_EXIT(4, 98, 5),
	SCENE_EXIT(5, 98, 4),
	SCENE_EXIT(6, 97, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement gKuupuappuDouPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKuupuappuDouPlacements1[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKuupuappuDouPlacements2[] = {
	{ 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(876), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(892), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(372), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(372), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(388), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(420), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(420), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(452), 0, PIXELS(528), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(424), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(424), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(536), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf8, 0x300, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKuupuappuDouPlacements3[] = {
	{ 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(236), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKuupuappuDouEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuDouEvents1[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x31, 6, CONDITION_ALWAYS, 6 },
	{ 0x31, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3a7SequenceB) },
	{ SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuDou_ShowLockedDoorMessage) },
	{ SEARCH(SEARCH_CHEST), 100, 0xef3, 0x500003 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuDouEvents2[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x21, 6, CONDITION_ALWAYS, 6 },
	{ 0x31, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ 0x21, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ SEARCH(SEARCH_CHEST), 100, 0xfc4, FIND_ITEM(0x82) },
	{ SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuDou_ShowLockedDoorMessage) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x9a9, EVENT_SCRIPT(FieldScene_RunScene3a7SequenceA) },
	{ 0x202, 60, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlag9a9GuardedScene) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x200, EVENT_SCRIPT(SceneState_ApplyRectAndMarkActor16) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, 0x201, EVENT_SCRIPT(SceneState_ConfigureRegion26_30AndMarkActor17) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x202, EVENT_SCRIPT(SceneState_ConfigureRegion26_30AndClearActor18Mode) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x203, EVENT_SCRIPT(SceneState_ApplyRectAndSetupActor19) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x204, EVENT_SCRIPT(SceneActor_SetupSlotTwenty) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 21, 0x205, EVENT_SCRIPT(SceneActor_MarkSlot21AndSetFlag205) },
	{ PSYNERGY_ON_ACTOR(0x4e), 22, 0x300, EVENT_SCRIPT(SceneState_ApplyFlag300) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuDouEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x21, 6, CONDITION_ALWAYS, 6 },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunGuardedStep9AAAfterSetup) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x9aa, EVENT_SCRIPT(FieldScene_RunGuardedStep9AA) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3a7_NoOpCallback) },
	{ SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuDou_ShowLockedDoorMessage) },
	{ SCENE_EVENTS_END },
};

u32 KuupuappuDou_ScheduleFrames[] = {
	20, 80, 115, 170,
};

s32 KuupuappuDou_ScheduleTimer = 0;

s32 KuupuappuDou_ScheduleIndex = 0;

s32 KuupuappuDou_TickCounter = 0;

s32 KuupuappuDou_TickValue = 0;
