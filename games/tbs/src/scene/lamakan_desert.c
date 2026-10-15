#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/lamakan_desert.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "fixed_point_position.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "iwram_call.h"
#include "field_service.h"
#include "field_effect.h"
#include "game_state.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_write_queue.h"
#include "menu_list.h"
#include "heap_state.h"
#include "ram_buffer.h"
#include "vram_block.h"
#include "io_reg.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void LamakanDesert_SetSandPhase(u16 value);

#define NULL ((void *)0)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"
#include "common/effect_spawn.h"

static void EV13_TALK(void)
{
	BattleFx_RunPageEffectForSlot(13, 3, 3);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_waitdown.h"

static s32 LamakanDesert_EmitSandEffect(struct FieldActor *actor)
{
	struct EffectOptions params;
	s32					 phase;

	if ((gFrameCount & 7) == 0)
	{
		Engine_AudioPlayCue(118);
	}
	phase = gFrameCount & 15;
	if (phase != 0)
	{
		return 0;
	}
	params.startScaleX = 0xcccc;
	params.startScaleY = 0xcccc;
	params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
	Effect_Spawn(
		actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase, 0x00880001, &params);
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_LamakanDesert1)
	{
		return gLamakanDesertEntrances1;
	}
	if (v == (s32)&SceneId_LamakanDesert2)
	{
		return gLamakanDesertEntrances2;
	}
	if (v == (s32)&SceneId_LamakanDesert3)
	{
		return gLamakanDesertEntrances3;
	}
	if (v == (s32)&SceneId_LamakanDesert4)
	{
		return gLamakanDesertEntrances4;
	}
	return gLamakanDesertEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return LamakanDesert_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_LamakanDesert3)
	{
		if (gGameState.entrance == 5)
		{
			Engine_GameFlagSet(0x90a);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			Engine_GameFlagSet(0x87a);
#endif
		}
	}
	if (gGameState.scene == (s32)&SceneId_LamakanDesert1)
	{
		return gLamakanDesertPlacements1;
	}
	if (gGameState.scene == (s32)&SceneId_LamakanDesert2)
	{
		return gLamakanDesertPlacements2;
	}
	if (gGameState.scene == (s32)&SceneId_LamakanDesert3)
	{
		return gLamakanDesertPlacements3;
	}
	return gLamakanDesertPlacementsOther;
}

static void EV09_TOUCH(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x8b2) == 0)
	{
		if (GameFlag_IsSet(0x8b3) == 0)
		{
			GameFlag_Set(0x8b3);
			GameFlag_Set(0x8b2);
		}
	}
	Event_PlayCue(123);
	Engine_EventRequestExit(3);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return LamakanDesert_Events;
}

static s32 LamakanDesert_ApplyEntryState(void)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32 v5;

	Engine_GameFlagClear(0x201);
	if (gGameState.scene == (s32)&SceneId_LamakanDesert1)
	{
		Map_CopyCells(70, 68, 4, 2, 22, 7);
		Map_CopyCells(70, 68, 4, 2, 8, 10);
		Map_CopyCells(70, 68, 4, 2, 23, 21);
		Map_CopyCellAttributes(70, 68, 4, 1, 23, 23);
		Map_CopyCells(70, 68, 4, 2, 16, 42);
		Map_CopyCells(70, 68, 4, 2, 36, 44);
		Map_CopyCells(70, 68, 4, 2, 14, 55);
	}
	else if (gGameState.scene == (s32)&SceneId_LamakanDesert2)
	{
		Map_CopyCells(70, 68, 4, 2, 42, 5);
		Map_CopyCells(70, 68, 4, 2, 20, 11);
		Map_CopyCellAttributes(70, 68, 4, 1, 20, 13);
		Map_CopyCells(70, 68, 4, 2, 14, 12);
		Map_CopyCells(70, 68, 4, 2, 56, 18);
		Map_CopyCells(70, 68, 4, 2, 7, 22);
		Map_CopyCellAttributes(70, 68, 4, 1, 7, 24);
		Map_CopyCells(70, 68, 4, 2, 44, 23);
		Map_CopyCellAttributes(70, 68, 4, 1, 44, 25);
		Map_CopyCells(70, 68, 4, 2, 38, 24);
		Map_CopyCells(70, 68, 4, 2, 26, 28);
		Map_CopyCells(70, 68, 4, 2, 17, 35);
		Map_CopyCells(70, 68, 4, 2, 50, 36);
		Map_CopyCells(70, 68, 4, 2, 34, 43);
		Map_CopyCellAttributes(70, 68, 4, 1, 34, 45);
		Map_CopyCells(70, 68, 4, 2, 6, 46);
		Map_CopyCells(70, 68, 4, 2, 27, 55);
		Map_CopyCells(70, 68, 4, 2, 43, 56);
	}
	else if (gGameState.scene == (s32)&SceneId_LamakanDesert3)
	{
		Engine_MapCopyCellsLayered(69, 99, 4, 2, 8, 16);
		Engine_MapCopyCellsLayered(69, 99, 4, 2, 6, 20);
		Engine_MapCopyCellsLayered(69, 99, 4, 2, 10, 23);
		Map_CopyCellAttributes(69, 99, 4, 2, 8, 14);
		Map_CopyCellAttributes(69, 99, 4, 2, 6, 18);
		Map_CopyCellAttributes(69, 99, 4, 1, 6, 20);
		Engine_MapCopyCellAttributes(69, 99, 4, 2, 10, 21);
		Map_CopyCells(0, 121, 5, 7, 8, 32);
		Map_CopyCells(0, 121, 5, 7, 43, 32);
		Map_CopyCells(6, 120, 3, 1, 9, 5);
		Map_CopyCells(9, 120, 3, 1, 44, 5);
		Map_CopyCellAttributes(9, 0, 3, 3, 9, 6);
	}
	Engine_ActorSetPosition(8, 0, 0);
	Engine_ActorSetPosition(9, 0, 0);
	Engine_ActorSetPosition(10, 0, 0);
	Engine_ActorSetPosition(11, 0, 0);
	Engine_ActorSetPosition(12, 0, 0);
	Engine_ActorSetPosition(13, 0, 0);
	v5 = 100;
	do
	{
		Engine_MapObjectSetPosition(v5, 0, 0);
		v5 = (v5 + 1);
	} while (v5 <= 107);
	if (gGameState.scene != (s32)&SceneId_LamakanDesert4)
	{
		DisplayScroll_StartHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
	}
}

static s32 LamakanDesert_ConfigureAreaLayout(void)
{
	s32 i;

	Engine_GameFlagClear(0x200);
	Engine_GameFlagSet(0x201);
	if (gGameState.scene == (s32)&SceneId_LamakanDesert1)
	{
		Map_CopyCells(64, 126, 4, 2, 22, 7);
		Map_CopyCells(68, 126, 4, 2, 8, 10);
		Map_CopyCells(72, 126, 4, 2, 23, 21);
		Map_CopyCellAttributes(72, 126, 4, 2, 23, 22);
		Map_CopyCells(76, 126, 4, 2, 16, 42);
		Map_CopyCells(80, 126, 4, 2, 36, 44);
		Map_CopyCells(84, 126, 4, 2, 14, 55);
		Actor_SetPosition(9, 0x1900000, 0x16c0000);
	}
	else if (gGameState.scene == (s32)&SceneId_LamakanDesert2)
	{
		Map_CopyCells(64, 126, 4, 2, 42, 5);
		Map_CopyCells(68, 126, 4, 2, 20, 11);
		Map_CopyCellAttributes(68, 126, 4, 2, 20, 12);
		Map_CopyCells(72, 126, 4, 2, 14, 12);
		Map_CopyCells(76, 126, 4, 2, 56, 18);
		Map_CopyCells(80, 126, 4, 2, 7, 22);
		Map_CopyCellAttributes(80, 126, 4, 2, 7, 23);
		Map_CopyCells(84, 126, 4, 2, 44, 23);
		Map_CopyCellAttributes(84, 126, 4, 2, 44, 24);
		Map_CopyCells(88, 126, 4, 2, 38, 24);
		Map_CopyCells(92, 126, 4, 2, 26, 28);
		Map_CopyCells(96, 126, 4, 2, 17, 35);
		Map_CopyCells(100, 126, 4, 2, 50, 36);
		Map_CopyCells(104, 126, 4, 2, 34, 43);
		Map_CopyCellAttributes(104, 126, 4, 2, 34, 44);
		Map_CopyCells(108, 126, 4, 2, 6, 46);
		Map_CopyCells(112, 126, 4, 2, 27, 55);
		Map_CopyCells(116, 126, 4, 2, 43, 56);
		Actor_SetPosition(9, 0x1600000, 0xcc0000);
		Actor_SetPosition(10, 0x2e00000, 0x18c0000);
		Actor_SetPosition(11, 0x900000, 0x17c0000);
		Actor_SetPosition(12, 0x2400000, 0x2cc0000);
		Actor_SetPosition(13, 0x2880000, 0x1980000);
	}
	else if (gGameState.scene == (s32)&SceneId_LamakanDesert3)
	{
		Map_CopyCells(64, 124, 4, 4, 8, 14);
		Map_CopyCells(68, 124, 4, 4, 6, 18);
		Map_CopyCellAttributes(68, 124, 4, 1, 6, 20);
		Map_CopyCells(72, 124, 4, 4, 10, 21);
		Map_CopyCells(10, 121, 5, 7, 8, 32);
		Map_CopyCells(5, 121, 5, 7, 43, 32);
		Map_CopyCells(0, 120, 3, 1, 9, 5);
		Map_CopyCells(3, 120, 3, 1, 44, 5);
		Actor_SetPosition(8, 0xa80000, 0x5c0000);
		Actor_SetPosition(9, 0x800000, 0x13c0000);
		Map_CopyCellAttributes(6, 0, 3, 3, 9, 6);
		if (Engine_GameFlagIsSet(0x90a) == 0)
		{
			Map_CopyCells(0, 119, 3, 1, 9, 5);
		}
	}
	for (i = 100; i <= 107; i++)
	{
		MapObject_SetPosition(i, -1, -1);
	}
	BattleFx_EmitRandomParticle();
	if (gGameState.scene != (s32)&SceneId_LamakanDesert4)
	{
		DisplayScroll_DisarmHBlankDma();
	}
}

static void EV08_TALK(void)
{
	Engine_ActorRunRepeatedMotion(8, 2);
	Party_SetFields1ceAnd1d0((s32)&SceneId_LamakanDesert3, 5);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(53, 5);
}

static void LamakanDesert_ShowRandomPartyLine(s32 a0)
{
	s32 i;
	s32 p8;
	s32 record;
	s32 value;
	s32 v5;
	s32 v6;

	p8 = a0;
	for (i = 0; i <= 2; i++)
	{
		value = Engine_RandomNext();
		v6 = (u32)((value << 1) + value) >> 16;
		v5 = v6 + 0x303;
		record = Engine_GameFlagIsSet(v5);
		if (record == 0)
		{
			Engine_GameFlagSet(v5);
			break;
		}
	}
	Engine_EventBegin();
	Engine_EventSetMessage((((p8 << 1) + p8) + v6) + MSGNO(MsgFuchinTempleIDoNotThinkMasterHama));
	Engine_EventShowMessage((v6 + 1), 0);
	Engine_EventEnd();
}

static void LamakanDesert_RaiseQuarterTriggers(void)
{
	struct EventWork *work = gEventWork;
	u8				 *state = (u8 *)&gGameState;
	s32				  percent = *(s16 *)(state + 0x232) * 100 / *(s16 *)(state + 0x22c);

	if (GameFlag_IsSet(0x201) != 0)
		return;
	if (GameFlag_IsSet(0x302) != 0 && percent <= 74)
	{
		GameFlag_Clear(0x302);
		GameFlag_Clear(0x303);
		GameFlag_Clear(0x304);
		GameFlag_Clear(0x305);
	}
	if (GameFlag_IsSet(0x301) != 0 && percent <= 49)
	{
		GameFlag_Clear(0x301);
		GameFlag_Clear(0x303);
		GameFlag_Clear(0x304);
		GameFlag_Clear(0x305);
	}
	if (GameFlag_IsSet(0x300) != 0 && percent <= 24)
	{
		GameFlag_Clear(0x300);
		GameFlag_Clear(0x303);
		GameFlag_Clear(0x304);
		GameFlag_Clear(0x305);
	}
	if (GameFlag_IsSet(0x300) == 0 && percent > 24)
	{
		GameFlag_Set(0x300);
		work->raisedTrigger = 1;
	}
	if (GameFlag_IsSet(0x301) == 0 && percent > 49)
	{
		GameFlag_Set(0x301);
		work->raisedTrigger = 2;
	}
	if (GameFlag_IsSet(0x302) == 0 && percent > 74)
	{
		GameFlag_Set(0x302);
		work->raisedTrigger = 3;
	}
}

static s32 LamakanDesert_CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
	s32 dx = (*position_b++ - *position_a++) >> 16;
	s32 dz = (*position_b - position_a[1]) >> 16;
	s32 dz_squared = dz * dz;
	s32 dx_squared = dx * dx;

	return Iwram_Sqrt(dx_squared + dz_squared);
}

struct SafePoint
{
	s32 x;
	s32 z;
};

static void EV20_TOUCH(void)
{
	u8		  *work;
	s32		   frames;
	s32		   best;
	s32		   count;
	const s32 *points;
	s32		   left;
	/* Ordinary offset with a generic input/output boundary after its
	 * increment, or an input after point calculation, differs 10 in EN
	 * (228 native/built, 223 common). Repeated input operands retain 10.
	 * Generic offset/count outputs after the increment leave 26; a count
	 * output with offset input or the reverse leaves 22 (228 built).
	 * Reusing offset as the later frame countdown leaves 144 (222 built).
	 * Global allocation ranks point (5 refs/12 insns, 0.83), count (13/56,
	 * 0.70) and offset (7/34, 0.41) and hands out r5, r6 and r7 in that
	 * order; the native needs offset between the other two (8 or 9 refs). */
	/* FAKEMATCH: pins the byte offset to r6, where the game keeps it with the countdown in r7; unpinned, the countdown takes r6. Plain shapes (u32 types, while/for countdowns, offset = 0 before the guard, count reused after the loop) differ 22-112; only an extra reference to offset in the loop preheader (an input barrier) reaches 0. */
	register s32 offset asm("r6");
	const s32			*point;
	s32					 nearest;
	s32					 distance;
	struct FieldActor	*leader;
	struct EffectOptions options;
	struct EffectOptions puff;
	s16					*progress;
	u16					*hold;
	s32					 zero;

	work = (u8 *)gEventWork;
	frames = 60;
	best = 0xf00000;
	GameFlag_Set(0x200);
	LamakanDesert_SetSandPhase(1);
	if (gGameState.scene == (s32)&SceneId_LamakanDesert1)
	{
		count = 3;
		points = LamakanDesert_SafePoints1;
	}
	else if (gGameState.scene == (s32)&SceneId_LamakanDesert2)
	{
		count = 5;
		points = LamakanDesert_SafePoints2;
	}
	else
	{
		count = 2;
		points = LamakanDesert_SafePointsOther;
	}
	left = count;
	point = points;
	if (count != 0)
	{
		offset = 0;
		do
		{
			distance = LamakanDesert_CalculatePlanarDistance(
				&Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, (s32 *)point);
			if (distance <= best)
			{
				best = distance;
				nearest = left - count;
			}
			offset += 8;
			point = (const s32 *)((const u8 *)points + offset);
		} while (--count != 0);
	}
	nearest <<= 1;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	{
		struct FieldActor	   *actor = Actor_Get(ACTOR_PARTY_LEADER);
		const struct SafePoint *spot = (const struct SafePoint *)&points[nearest];

		FieldObject_SetPosition(actor, spot->x, 0, spot->z);
	}
	Actor_Get(ACTOR_PARTY_LEADER)->velocityY = 0x60000;
	Event_PlayCue(152);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	SceneActor_WaitObjectBelowHeight(leader, Actor_Get(ACTOR_PARTY_LEADER)->y.fixed);
	Event_PlayCue(241);
	{
		struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

		options.type = 214;
		options.startScaleX = 0x8000;
		options.startScaleY = 0xcccc;
		options.targetScaleX = 0x10000;
		options.targetScaleY = 0x13333;
		Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
			EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &options);
	}
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x104, 0);
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 18);
	hold = (u16 *)(work + 0xcba);
	progress = &gGameState.hazardSteps;
	zero = 0;
	do
	{
		s32 held = 600;

		*hold = held;
		frames--;
		if (*progress != 0)
		{
			*progress -= 5;
			if (*progress <= 0)
			{
				*progress = zero;
			}
			else if (frames == 0)
			{
				frames = 1;
			}
		}
		Task_Wait(1);
	} while (frames != 0);
	{
		struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);

		puff.type = 214;
		puff.startScaleX = 0x8000;
		puff.startScaleY = 0xcccc;
		puff.targetScaleX = 0x8000;
		puff.targetScaleY = 0x13333;
		Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, frames, frames,
			EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET, &puff);
	}
	Event_PlayCue(0x120);
	Event_PlayCue(152);
	Actor_Get(ACTOR_PARTY_LEADER)->velocityY = 0x60000;
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
	Event_Wait(10);
	*(u16 *)(work + 0xcba) = frames;
	LamakanDesert_SetSandPhase(0);
}

static void LamakanDesert_UpdateTravelDust(void)
{
	struct FieldActor	*actor;
	struct EventWork	*event;
	struct EffectOptions params;
	s32					 dx;
	s32					 z;
	s32					 phase;
	actor = Object_GetById(gGameState.selectedActor);
	event = gEventWork;
	if (actor->targetX == 0x80000000)
	{
		return;
	}
	gGameState.hazardSteps++;
	if (event->touchedTrigger == 30)
	{
		return;
	}
	if (actor->speed <= 0x10000)
	{
		if ((gFrameCount & 15) != 0)
		{
			return;
		}
		params.startScaleX = 0x8000;
		params.startScaleY = 0x8000;
		dx = 0;
		params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
		if (actor->facing != 0 && actor->facing != 0x8000)
		{
			z = actor->z.fixed;
			dx = 0x20000 - ((((z >> 20) & 1) * 5) << 16);
		}
		else
		{
			z = actor->z.fixed;
		}
		Effect_Spawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0, 0x880001, &params);
	}
	else
	{
		phase = gFrameCount & 7;
		if (phase != 0)
		{
			return;
		}
		// フレームカウンタをもう一度読んで捨てる (ROM の読み出しそのまま)
		gFrameCount;
		params.startScaleX = 0xcccc;
		params.startScaleY = 0xcccc;
		params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
		Effect_Spawn(actor->x.fixed, actor->y.fixed + 0x20000, actor->z.fixed, 0,
			((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999, phase, 0x880001, &params);
	}
}

static void EV22_TOUCH(void)
{
	struct FieldActor *target;
	struct FieldActor *actor;
	struct FieldActor *record;
	s32				   id;
	s32				   best;
	s32				   min;
	s32				   dx;
	s32				   dz;

	target = Actor_Get(gGameState.selectedActor);
	best = 9;
	GameFlag_Set(0x200);
	min = 0x100000;
	for (id = 9; id <= 12; id++)
	{
		u8 *other = Actor_Get(id);

		if (other != 0)
		{
			dx = (target->x.fixed - *(s32 *)(other + 8)) / 0x10000;
			dz = (target->z.fixed - *(s32 *)(other + 16)) / 0x10000;
			{
				s32 ax = dx;

				if (ax < 0)
				{
					ax = -ax;
				}
				if (dz < 0)
				{
					dz = -dz;
				}
				if (ax + dz < min)
				{
					best = id;
					min = ax + dz;
				}
			}
		}
	}
	Engine_ActorSetAnimation(0, 1);
	Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Engine_ActorFaceActor(0, best, 0);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(0, 0x102);
	Engine_ActorStartRepeatedMotion(0, 2);
	Engine_EventWait(60);
	Actor_SetAttachedEffect(0, 0x101);
	actor = Actor_Get(0);
	record = Object_GetById(0);
	actor->facing = (record->facing + 0x8000) & -0x1000;
	Engine_ActorSetAnimation(0, 5);
	Object_SetActionById(0, 24);
	Actor_SetSpeed(0, 0x1999, 0xccc);
	record = Object_GetById(0);
	*(s32 *)((u8 *)record + 108) = (s32)LamakanDesert_EmitSandEffect;
	record = Actor_Get(best);
	if (record != 0)
	{
		Engine_ActorSetDestination(0, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_EventWait(60);
	Actor_ShowEmote(best, 0x104, 0);
	Engine_EventWait(60);
	Actor_SetAttachedEffect(0, 0x100);
	Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
	record = Object_GetById(0);
	*(s32 *)((u8 *)record + 108) = 0;
	BattleFx_SetWeightedResult(53, 4);
}

static void EV30_TOUCH(void)
{
	struct FieldActor	*center;
	s32					 id;
	struct FieldActor	*actor;
	s32					 pos[3];
	struct EffectOptions params;
	s32					 hit;
	s32					 phase;
	s32					 x;

	LamakanDesert_UpdateTravelDust();
	{
		struct EventWork *event = gEventWork;

		id = gGameState.selectedActor;
		center = event->viewCenter;
	}
	actor = Object_GetById(id);
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0x18000;
	hit = FieldObject_CheckMovementCollision(actor, pos);
	phase = gFrameCount & 4;
	if (phase == 0)
	{
		params.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
		x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
		Effect_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
			((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd, 0x800000, &params);
	}
	if (hit < 0)
	{
		Actor_SetAttachedEffect(id, 0x102);
		Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000);
		Object_SetMode(actor, 7);
		Engine_ObjectCommitPosition(actor);
		do
		{
			Engine_TaskWait(1);
		} while (actor->y.fixed != actor->terrainHeight);
		Object_SetMode(actor, 6);
		Engine_TaskWait(3);
		return;
	}
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0x80000;
	hit = FieldObject_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		return;
	}
	pos[0] = actor->x.fixed + 0x5b333;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0x5b333;
	hit = FieldObject_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		return;
	}
	pos[0] = actor->x.fixed - 0x5b333;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0x5b333;
	hit = FieldObject_CheckMovementCollision(actor, pos);
	if (hit > 0)
	{
		return;
	}
	center->z.fixed += 0x18000;
	actor->z.fixed += 0x18000;
}

static void EV40_TOUCH(void)
{
	s32				   rec8;
	struct FieldActor *record;
	s32				   rect[3];
	s32				   shown;
	u16				  *shown_addr;
	u8				  *p5;

	p5 = (u8 *)gEventWork;
	LamakanDesert_UpdateTravelDust();
	if (GameFlag_IsSet(0x90a) == 0)
	{
		rec8 = GameFlag_IsSet(0x200);
		if (rec8 == 0)
		{
			GameFlag_Set(0x200);
			LamakanDesert_SetSandPhase(1);
			shown_addr = (u16 *)(p5 + 0xcba);
			shown = 0x258;
			*shown_addr = shown;
			record = Object_GetById(ACTOR_PARTY_LEADER);
			record->velocityX = rec8;
			record = Object_GetById(ACTOR_PARTY_LEADER);
			record->velocityZ = rec8;
			record = Actor_Get(ACTOR_PARTY_LEADER);
			record->targetX = -0x80000000;
			record = Actor_Get(ACTOR_PARTY_LEADER);
			record->targetZ = -0x80000000;
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
			Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
			Engine_EventWait(40);
			Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
			Engine_EventWait(40);
			Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
			rect[0] = rec8;
			rect[1] = rec8;
			rect[2] = rec8;
			record = Actor_Get(ACTOR_PARTY_LEADER);
			FieldVector_AddPolarOffset(-0x100000, record->facing, (struct FieldPosition *)rect);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
			Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, rect[0] / 0x10000, rect[2] / 0x10000);
			Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
			Engine_EventWait(2);
			Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
			Engine_EventWait(30);
			Event_PlayCue(148);
			Engine_ActorRunRepeatedMotion(8, 2);
			Engine_EventWait(20);
			Actor_SetSpeed(8, 0x28000, 0x14000);
			Actor_WalkToAndWait(8, 168, 104);
			Actor_SetSpeed(8, 0x8000, 0x4000);
			Actor_WalkToAndWait(8, 168, 92);
			*shown_addr = shown;
			LamakanDesert_SetSandPhase(0);
		}
	}
}

s32 LamakanDesert_EnterArea(s32 a0, s32 a1)
{
	s32 v5;

	gGameState.hazardLimit = 0x258;
	gGameState.hazardMode = 0;
	gGameState.hazardDamage = 0x119;
	if (gGameState.scene == (s32)&SceneId_LamakanDesert4)
	{
	}
	else
	{
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		LamakanDesert_ClaimSandEffectVram();
		FieldScheduler_AddCallback((s32)LamakanDesert_RaiseQuarterTriggers, 0xc80);
		if (gGameState.scene == (s32)&SceneId_LamakanDesert1)
		{
			Map_CopyCells(22, 7, 4, 2, 64, 126);
			Map_CopyCells(8, 10, 4, 2, 68, 126);
			Map_CopyCells(23, 21, 4, 2, 72, 126);
			Map_CopyCells(16, 42, 4, 2, 76, 126);
			Map_CopyCells(36, 44, 4, 2, 80, 126);
			Map_CopyCells(14, 55, 4, 2, 84, 126);
		}
		else if (gGameState.scene == (s32)&SceneId_LamakanDesert2)
		{
			Map_CopyCells(42, 5, 4, 2, 64, 126);
			Map_CopyCells(20, 11, 4, 2, 68, 126);
			Map_CopyCells(14, 12, 4, 2, 72, 126);
			Map_CopyCells(56, 18, 4, 2, 76, 126);
			Map_CopyCells(7, 22, 4, 2, 80, 126);
			Map_CopyCells(44, 23, 4, 2, 84, 126);
			Map_CopyCells(38, 24, 4, 2, 88, 126);
			Map_CopyCells(26, 28, 4, 2, 92, 126);
			Map_CopyCells(17, 35, 4, 2, 96, 126);
			Map_CopyCells(50, 36, 4, 2, 100, 126);
			Map_CopyCells(34, 43, 4, 2, 104, 126);
			Map_CopyCells(6, 46, 4, 2, 108, 126);
			Map_CopyCells(27, 55, 4, 2, 112, 126);
			Map_CopyCells(43, 56, 4, 2, 116, 126);
		}
		else if (gGameState.scene == (s32)&SceneId_LamakanDesert3)
		{
			v5 = 124;
			BattleFx_SetQueuedSoundAndPlay(169);
			Map_CopyCells(8, 14, 4, 4, 64, v5);
			Map_CopyCells(6, 18, 4, 4, 68, v5);
			Map_CopyCells(10, 21, 4, 4, 72, v5);
		}
		LamakanDesert_ApplyEntryState();
	}
	return 0;
}

static void LamakanDesert_ResetActorEightPosition(void)
{
	Actor_SetSpeed(8, 0x8000, 0x4000);
	Engine_ActorSetAnimation(8, 1);
	Actor_WalkToAndWait(8, 168, 96);
	Engine_ActorSetAnimation(8, 2);
}

static void LamakanDesert_RunSandstorm(void)
{
	u8				  *buffer;
	s32				   tile;
	s16				  *colors;
	s16				  *color;
	u8				  *display;
	u32				  *column;
	u32				  *word;
	u8				  *source;
	u8				  *destination;
	struct MenuSprite *sprite;
	s32				   counter;
	u32				   i;
	u32				   fill;
	u32				  *palette;
	u32				   red;
	u32				   green;
	u32				   blue;
	s32				   level;
	u32				   attributes;
	s32				   x;
	u32				   offset;

	tile = gVramBlockCache[(*(s16 *)&LamakanDesert_SandVramSlot)].offset >> 5;
	if ((*(s16 *)&LamakanDesert_SandPhase) != 0)
	{
		LamakanDesert_SandCounter = 2;
	}
	else if (Engine_GameFlagIsSet(0x104) != 0)
	{
		if (LamakanDesert_SandCounter > 0)
		{
			LamakanDesert_SandCounter--;
		}
	}
	else if (LamakanDesert_SandCounter <= 1)
	{
		if (++LamakanDesert_SandCounter == 1)
		{
			Dma_Set(LamakanDesert_SandPalette, (void *)0x050003c0, 0x80000010, REG_DMA3);
		}
	}
	counter = LamakanDesert_SandCounter;
	if (counter == 0)
	{
		Resource_ActivateEntry((*(s16 *)&LamakanDesert_SandVramSlot));
		return;
	}
	display = *Ram_DisplayWork;
	if (display != NULL)
	{
		s16 *line = (s16 *)(display + display[0x539] * 644);
		s32	 shift = (s16)(counter << 3);

		line = (s16 *)((u8 *)line + 38);
		for (i = 0; i < 144; i++)
		{
			*line = shift;
			line += 2;
		}
	}

	buffer = MemAlloc(0x900);
	Dma_Set(LamakanDesert_SandPalette, buffer, 0x80000010, REG_DMA3);
	colors = (s16 *)(buffer + 12);
	color = colors;
	for (i = 6; i < 12; i++)
	{
		u16 value = *color;

		red = value & 31;
		green = (value >> 5) & 31;
		blue = ((value >> 10) & 31) - 20;
		level = LamakanDesert_SandLevel;
		red += level / 3;
		blue = blue - level / 6 + 20;
		if (level > 60 && (gFrameCount & 1))
		{
			green = green + (level << 6) / 120 - 32;
		}
		if (red > 31)
		{
			red = 31;
		}
		if (green > 31)
		{
			green = 31;
		}
		if (blue > 31)
		{
			blue = 31;
		}
		*color++ = (blue << 10) | (green << 5) | red;
	}
	palette = (u32 *)0x050003cc;
	word = (u32 *)(buffer + 12);
	QueueIoWriteDelay3((u32)palette++, *word++);
	QueueIoWriteDelay3((u32)palette++, *word++);
	QueueIoWriteDelay3((u32)palette++, *word++);

	LamakanDesert_SandLevel = gGameState.hazardSteps * 120 / gGameState.hazardLimit;
	if (LamakanDesert_SandLevel > 118)
	{
		LamakanDesert_SandFlash = 119;
	}
	if (LamakanDesert_SandFlash != 0)
	{
		LamakanDesert_SandLevel = LamakanDesert_SandFlash;
		LamakanDesert_SandFlash -= 8;
		if (LamakanDesert_SandFlash <= 0)
		{
			LamakanDesert_SandFlash = 0;
		}
	}

	Dma_Set(LamakanDesert_SandTileBuffer, buffer, 0x84000240, REG_DMA3);
	if (LamakanDesert_SandFlash <= 118)
	{
		column = (u32 *)(buffer + 80);
		fill = 0xeeeeeeee;
		for (i = 12; i < 128 - LamakanDesert_SandLevel; i++)
		{
			column[8] = fill;
			*column++ = fill;
			if ((i & 7) == 7)
			{
				column += 8;
			}
		}
		column[0] = ((u32 *)buffer)[0];
		column[8] = ((u32 *)buffer)[8];
	}
	destination = buffer;
	source = buffer + 0x480;
	for (i = 0; i < 0x480; i++)
	{
		u8 value = *source++;

		if (value != 0)
		{
			*destination = value;
		}
		destination++;
	}
	Engine_VramLoad((*(s16 *)&LamakanDesert_SandVramSlot), 0x480, buffer);

	attributes = 0x80008000;
	sprite = LamakanDesert_SandWork;
	for (i = 0, offset = 0, x = 8; i < 5; i++)
	{
		u32 y = (LamakanDesert_SandCounter * 8 - 16) & 0x1ff;

		if (i == 4)
		{
			attributes = 0x40000000;
		}
		sprite->next = NULL;
		sprite->oam.word.attr01 = (y << 16) | x | attributes;
		sprite->oam.word.attr23 = 0xe400 | tile;
		Runtime_PushSlotEntry(((u8 *)LamakanDesert_SandWork + offset), 255);
		sprite++;
		tile += 8;
		offset += 12;
		x += 32;
	}
	MemFree(buffer);
}

struct Half
{
	u16 v;
};

void LamakanDesert_ClaimSandEffectVram(void)
{
	s32			slot;
	u16		   *slotp;
	struct Half zero;

	FieldResource_DecodeType01(LamakanDesert_SandEffectTiles, LamakanDesert_SandTileBuffer);
	slotp = &LamakanDesert_SandVramSlot;
	slot = Resource_FindFreeEntry();
	*slotp = slot;
	Engine_VramLoad((s16)slot, 0x480, 0);
	zero.v = 0;
	LamakanDesert_SandCounter = zero.v;
	LamakanDesert_SandPhase = zero.v;
	FieldScheduler_AddCallback(LamakanDesert_RunSandstorm, 0xc76);
}

static void LamakanDesert_SetSandPhase(u16 value)
{
	LamakanDesert_SandPhase = value;
}

const s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 LamakanDesert_SafePoints1[] = {
	PIXELS(384), PIXELS(128),
	PIXELS(288), PIXELS(688),
	PIXELS(608), PIXELS(720),
};

const s32 LamakanDesert_SafePoints2[] = {
	PIXELS(704), PIXELS(96),
	PIXELS(448), PIXELS(464),
	PIXELS(832), PIXELS(592),
	PIXELS(128), PIXELS(752),
	PIXELS(464), PIXELS(896),
};

const s32 LamakanDesert_SafePointsOther[] = {
	PIXELS(192), PIXELS(224),
	PIXELS(192), PIXELS(352),
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gLamakanDesertEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, 0, 0, 0, 0, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 984, 0, 952, FACING_WEST, 0, 0, 0, 1016, 1016, 0 },
	{ 2, CONDITION_ALWAYS, 400, 0, 24, FACING_SOUTH, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 912, 0, 1000, FACING_NORTH, 0, 0, 0, 1016, 1016, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 584, FACING_EAST, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 1511, 0, 104, FACING_WEST, 0, 0, 8, 1528, 504, 0 },
	{ 2, CONDITION_ALWAYS, 727, 0, 104, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 3, CONDITION_ALWAYS, 599, 0, 296, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 4, CONDITION_ALWAYS, 167, 0, 120, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 5, CONDITION_ALWAYS, 167, 0, 136, FACING_NORTH, 0, 0, 8, 1528, 504, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gLamakanDesertEntrances4[] = {
	{ 0, CONDITION_ALWAYS, 424, 0, 824, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 424, 0, 968, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ 2, CONDITION_ALWAYS, 552, 0, 776, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ 3, CONDITION_ALWAYS, 440, 0, 312, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ 4, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ SCENE_TABLE_END },
};

u32 LamakanDesert_Exits[] = {
	SCENE_EXITS(89),
	SCENE_EXIT(1, 2, 18),
	SCENE_EXIT(2, 90, 1),
	SCENE_EXIT(0, 90, 0),
	SCENE_EXIT(1, 89, 2),
	SCENE_EXIT(2, 91, 1),
	SCENE_EXIT(0, 91, 0),
	SCENE_EXIT(1, 90, 2),
	SCENE_EXIT(2, 92, 2),
	SCENE_EXIT(3, 92, 1),
	SCENE_EXIT(4, 92, 3),
	SCENE_EXIT(0, 92, 0),
	SCENE_EXIT(1, 91, 2),
	SCENE_EXIT(2, 91, 3),
	SCENE_EXIT(3, 2, 53),
	SCENE_EXIT(4, 91, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gLamakanDesertPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertPlacements1[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(364), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertPlacements2[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(736), 0, PIXELS(396), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(380), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(576), 0, PIXELS(716), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x5d, 0x6f, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gLamakanDesertPlacements3[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(316), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent LamakanDesert_Events[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 4 },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ EVENT_TOUCH, 20, 0x200, EVENT_SCRIPT(EV20_TOUCH) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 22, 0x200, EVENT_SCRIPT(EV22_TOUCH) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_TOUCH) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_TOUCH) },
	{ EVENT_TALK, 8, 0x90a, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf7b, FIND_ITEM(0x24) },
	{ SEARCH(SEARCH_UNNAMED), 101, 0x350, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 102, 0xf7c, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_UNNAMED), 103, 0x351, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 105, 0xf7d, FIND_COINS(777) },
	{ SEARCH(SEARCH_CHEST), 106, 0xf7e, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 107, 0xf7f, FIND_ITEM(0xc3) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_ConfigureAreaLayout) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_ApplyEntryState) },
	{ EVENT_RAISED, 1, 0x201, EVENT_SCRIPT(LamakanDesert_ShowRandomPartyLine) },
	{ EVENT_RAISED, 2, 0x201, EVENT_SCRIPT(LamakanDesert_ShowRandomPartyLine) },
	{ EVENT_RAISED, 3, 0x201, EVENT_SCRIPT(LamakanDesert_ShowRandomPartyLine) },
	{ EVENT_TOUCH, 100, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 101, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 102, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 103, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 104, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 105, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 106, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ EVENT_TOUCH, 107, CONDITION_ALWAYS, EVENT_SCRIPT(LamakanDesert_UpdateTravelDust) },
	{ SCENE_EVENTS_END },
};

s16 LamakanDesert_SandLevel = 0;

s16 LamakanDesert_SandCounter = 0;

s16 LamakanDesert_SandFlash = 0;
