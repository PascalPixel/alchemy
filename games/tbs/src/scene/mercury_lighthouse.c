#include "game_state.h"
#include "heap_state.h"
#include "iwram_call.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "staged_actor.h"
#include "unit/scene/common/mercury_lights.h"
#include "unit/scene/common/mercury_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "mem.h"
#include "mercury.h"
#include "mercury_lighthouse.h"
#include "ram_buffer.h"
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
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "global_cells.h"
#include "map_render_work.h"
#include "io_reg.h"
#include "dma.h"
#include "object_runtime.h"
#include "animspr.h"
#include "map.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"

// 前方宣言
static void EV11_PSYNERGY(void);
static void EV08_PSYNERGY_3(void);
static void MercuryLighthouse_SyncBlockFlags(void);
static void SceneEffect_SpawnParticleRowsAndDrawTiles(void);
static void MercuryLighthouse_OpenThreeStepStair(void);
static void SceneEffect_SpawnParticleEveryFourthFrame(void);
static void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z);
static void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
static void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w);
static void MercuryLighthouse_RideLift(void);
static void MercuryLighthouse_CastPsynergyAtActor11(void);
static void SceneState_SetRecordTableValue(s32 key, s32 value);

#define NULL ((void *)0)

#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

enum
{
	ITEM_HERMES_WATER = 184
};

struct WorkPointers
{
	u8 *event;
	u8 *unk04[7];
	u8 *scene;
};

struct SceneLookupRecord
{
	u8	pad00[216];
	u16 table[1];
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

static void EV12_TALK(void)
{
	BattleFx_SetPhaseRequest(12, 21);
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
struct LeaderEffectWork
{
	u8				   unk00[24];
	struct FieldActor *target;
};

static void MercuryLighthouse_UpdateLeaderEffectTarget(void)
{
	struct FieldActor		*actor = Object_GetById(0);
	struct LeaderEffectWork *work = *(struct LeaderEffectWork **)Ram_ActorEffectWork;

	if (actor->terrainId == 0 && actor->z.fixed >> 20 > 12 && actor->z.fixed >> 20 <= 18)
		work->target = NULL;
	else
		work->target = actor;
}
#endif

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/mercury_sparks.h"
#include "common/mercury_palette_cycle.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MercuryLighthouse4)
	{
		return gMercuryLighthouseEntrances4;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse3)
	{
		return gMercuryLighthouseEntrances3;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse2)
	{
		return gMercuryLighthouseEntrances2;
	}
	return gMercuryLighthouseEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *MercuryLighthouse_GetExits(void)
{
	return MercuryLighthouse_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MercuryLighthouse1)
	{
		return gMercuryLighthousePlacements1;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse2)
	{
		return gMercuryLighthousePlacements2;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse3)
	{
		return gMercuryLighthousePlacements3;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse4)
	{
		return gMercuryLighthousePlacements4;
	}
	return gMercuryLighthousePlacementsOther;
}

static void EV21_TOUCH(void)
{
	Mercury_TickSpawnTimer();
}

static void MercuryLighthouse_ClearSceneRecordAndTarget(void)
{
	SceneState_ClearCurrentRecordAndReleaseTarget();
}

static void FieldScene_RunSingleStep(void)
{
	Mercury_RunActorMove();
}

static void EV11_TOUCH(void)
{
	MercuryEntrance_RaisePillar();
}

static void EV10_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered(MsgMercuryLighthouseStrangeForcesBlockWay, 1);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MercuryLighthouse1)
	{
		return gMercuryLighthouseEvents1;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse2)
	{
		return gMercuryLighthouseEvents2;
	}
	if (selector == (s32)&SceneId_MercuryLighthouse3)
	{
		return gMercuryLighthouseEvents3;
	}
	return gMercuryLighthouseEventsOther;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/mercury_risedrop.h"

static void EV53_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(gEventWork->touchedTrigger - 50);
}

static void EV63_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(gEventWork->touchedTrigger - 50);
}

static void EV55_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(gEventWork->touchedTrigger - 50);
}

static void MercuryLighthouse_ExitWhenChannelsOpen(void)
{
	if (GameFlag_IsSet(0x310) != 0 && GameFlag_IsSet(0x311) != 0 && GameFlag_IsSet(0x312) != 0)
	{
		GameFlag_Set(0x876);
		Engine_EventWait(30);
		Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		Engine_AudioPlayCue(141);
		Engine_EventWait(60);
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
		Engine_AudioPlayCue(0x121);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_MapRenderWaitForValues();
		Engine_EventRequestExit(13);
	}
	else
	{
		GameFlag_Clear(0x876);
	}
}

static void EV25_SCRIPT(void)
{
	struct StagedActorProbe probe;
	s32						x;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		switch ((u32)probe.actorSlot)
		{
		case 9:
			if (probe.positionZ >> 20 == 8)
			{
				SceneActor_MoveAndRedraw(probe);
				Engine_EventWait(20);
				Map_CopyCellsTo(119, 9, 109, 11, 1, 1);
				SceneEffect_SpawnRandomizedBurst(0x2d60000, 0, 0xb40000, 0x8000);
				GameFlag_Set(0x310);
			}
			else
			{
				s32 one = 1;

				Map_CopyCellsTo(117, 9, 104, 7, one, one);
				Map_CopyCellsTo(119, 8, 109, 11, one, one);
				Map_CopyCellsTo(118, 8, 104, 13, one, one);
				SceneActor_MoveAndRedraw(probe);
				GameFlag_Clear(0x310);
			}
			break;
		case 10:
			if (probe.positionZ >> 20 == 12)
			{
				SceneActor_MoveAndRedraw(probe);
				Engine_EventWait(10);
				if (GameFlag_IsSet(0x310))
				{
					Map_CopyCellsTo(118, 9, 104, 13, 1, 1);
					SceneEffect_SpawnRandomizedBurst(0x2840000, 0, 0xd20000, 0x4000);
				}
				GameFlag_Set(0x311);
			}
			else
			{
				s32 one = 1;

				Map_CopyCellsTo(119, 8, 109, 11, one, one);
				if (GameFlag_IsSet(0x310))
				{
					Map_CopyCellsTo(119, 9, 109, 11, one, one);
					Map_CopyCellsTo(118, 8, 104, 13, one, one);
				}
				SceneActor_MoveAndRedraw(probe);
				GameFlag_Clear(0x311);
			}
			break;
		case 11:
			x = probe.positionX;
			if (x >> 20 == 40)
			{
				SceneActor_MoveAndRedraw(probe);
				GameFlag_Set(0x312);
			}
			else
			{
				SceneActor_MoveAndRedraw(probe);
				GameFlag_Clear(0x312);
			}
			break;
		}
		MercuryLighthouse_ExitWhenChannelsOpen();
	}
	Engine_EventEnd();
}

static void MercuryLighthouse_WalkLeaderIn(void)
{
	struct EventWork *work;

	work = gEventWork;
	Engine_EventBegin();
	FieldScheduler_AddCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame, 0xc80);
	Actor_SetSpeed(0, 0x28000, 0x14000);
	Engine_ActorSetAnimation(0, 1);
	Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
	Engine_AudioPlayCue(228);
	if (work->touchedTrigger == 2)
	{
		Actor_SetDestination(0, 232, 616);
	}
	else if (work->touchedTrigger == 3)
	{
		Actor_SetDestination(0, 360, 728);
	}
	else if (work->touchedTrigger == 4)
	{
		Actor_SetDestination(0, 248, 792);
	}
	else
	{
#if EDITION_INTERNATIONAL
		Actor_MoveToAndWait(0, 696, 592);
#else
		Actor_SetDestination(0, 696, 592);
#endif
#if EDITION_INTERNATIONAL
		Actor_SetDestination(0, 696, 600);
		Engine_EventWait(30);
#endif
	}
	Engine_ActorWaitForMove(0);
	Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
	Engine_TaskRemoveCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame);
	Engine_EventEnd();
}

static void EV10_SCRIPT(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	EV11_PSYNERGY();
}

static void EV11_PSYNERGY(void)
{
	s32				   x;
	s32				   y;
	struct FieldActor *p;

	x = Object_GetById(11)->x.fixed / 0x100000;
	y = Object_GetById(11)->z.fixed / 0x100000;
	Engine_EventBegin();
	if (x == 5 && y == 13)
	{
		Object_GetById(11)->y.fixed += 0xfffe0000;
		p = Object_GetById(11);
		p->targetY = Object_GetById(11)->y.fixed;
		Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
		Engine_AudioPlayCue(0xd9);
		Engine_MapAnimateCells(MercuryLighthouse_GateCells, 9, 7);
		{
			s32 s0 = 9;
			s32 s1 = 10;
			Map_CopyCellAttributes(9, 5, 1, 1, s0, s1);
		}
		Engine_GameFlagSet(0x874);
	}
	Engine_EventEnd();
}

static void EV12_TOUCH_2(void)
{
	s32				   x;
	s32				   z;
	struct FieldActor *actor;

	if (GameFlag_IsSet(0x256) == 0)
	{
		x = Object_GetById(0)->x.part.pixel;
		z = Object_GetById(0)->z.part.pixel;
		if ((u32)(x - 84) <= 7 && z > 211 && z <= 219)
		{
			Engine_EventBegin();
			Engine_GameFlagSet(0x256);
			Engine_EventWait(5);
			Object_GetById(0)->y.fixed += -0x20000;
			actor = Object_GetById(0);
			actor->targetY = Object_GetById(0)->y.fixed;
			Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
			Engine_AudioPlayCue(217);
			Engine_MapAnimateCells((s32)MercuryLighthouse_GateCells, 9, 7);
			Engine_EventEnd();
		}
	}
}

static void MercuryLighthouse_CloseFloorSwitch(void)
{
	struct FieldActor *actor;

	if (GameFlag_IsSet(0x256) != 0)
	{
		Engine_EventBegin();
		GameFlag_Clear(0x256);
		Actor_Get(0)->y.fixed += 0x20000;
		actor = Actor_Get(0);
		actor->targetY = Actor_Get(0)->y.fixed;
		Engine_EventWait(5);
		Map_CopyCellsTo(7, 2, 5, 11, 1, 1);
		Engine_AudioPlayCue(217);
		Map_AnimateCells(MercuryLighthouse_GateCloseCells, 9, 7);
		Engine_EventEnd();
	}
}

static void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1, s32 a2)
{
	u32 i;
	s32 v;

	if (a0 != 0)
	{
		Engine_AudioPlayCue(219);
	}
	i = a1;
	if (i < a2)
	{
		do
		{
			Map_CopyCellsTo(45 - (i << 1), 32, 44 - (i << 1), 32, (i + 1), 6);
			Map_CopyCellsTo(45 - i, 51, 45 - i, 32, 1, 6);
			v = 109 - i;
			Map_CopyCellsTo(v, 32, 108 - i, 32, 1, 4);
			Map_CopyCellsTo(v, 51, v, 32, 1, 4);
			if (a0 != 0)
			{
				Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
				Work_SetValuesIfNonNegative(-1, -1, 0xe666);
				Engine_EventWait(a0);
			}
			i++;
		} while (i < a2);
	}
	Map_CopyCellAttributes(42, 52, 4, 5, 42, 33);
}

static void MercuryLighthouse_RunSequenceG(s32 a0)
{
	u32 i;
	s32 x;
	s32 y;
	s32 k;
	s32 six;
	s32 pos;
	s32 four;
	Engine_AudioPlayCue(219);
	six = 6;
	i = 0;
	x = 41;
	y = 40;
	do
	{
		Map_CopyCellsTo(y, 32, x, 32, 3 - i, six);
		k = 2;
		Map_CopyCellsTo(39, 51, y, 32, 1, six);
		pos = i + 106;
		four = 4;
		/* FAKEMATCH: width and height go through locals k and four, and loop.c hoists k's 2 into r9 as the native has it. As the literals 2 and 4: differ 26, through Map_CopyCellsTo: differ 35; k = 2 set before the loop: differ 8; four as the literal 4: differ 20. */
		Engine_MapCopyCellsTo(105, 51, pos, 32, k, four);
		if (a0 != 0)
		{
			Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_EventWait(a0);
		}
		i++;
		x += 2;
		y += 2;
	} while (i <= 2);
	Engine_AudioPlayCue(0x120);
	Map_CopyCellAttributes(106, 33, 4, 5, 42, 33);
	Engine_MapRenderWaitForValues();
}

static void MercuryLighthouse_SprayAtFountain(void)
{
	SceneEffect_SpawnRandomEveryFourFrames(0x2b20000, 0, 0x2480000);
}

static void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
	s32 *rec;
	s32	 outer;
	s32	 shift4;
	s32	 v8;
	s32	 inner;
	s32	 neg;
	s32	 nsh;
	s32	 va0;
	s32	 vb0;
	s32	 va;
	s32	 vb;
	s32	 next;
	s32	 slot20[10];

	Map_CopyCellsTo(78, 59, 110, 36, 1, 1);
	Map_CopyCellsTo(76, 59, 109, 36, 1, 1);
	rec = slot20;
	rec[1] = 7;
	rec[2] = 0x8000;
	rec[3] = 0x8000;
	outer = 0;
	do
	{
		shift4 = (outer << 4);
		inner = 0;
		nsh = -(outer << 20);
		v8 = (0x2d80000 + nsh);
		do
		{
			if ((inner & 1) != 0)
			{
				va0 = Random_Next();
				va =
					(((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16))
						  + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4))
						 + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16))
								+ (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4))
							 << 8))
						+ -0xcccc);
				neg = -inner;
				vb0 = Random_Next();
				vb =
					(((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16))
						  + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4))
						 + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16))
								+ (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4))
							 << 8))
						+ -0xcccc);
				Effect_Spawn(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
				Engine_EventWait(1);
			}
			else
			{
				neg = -inner;
			}
			SceneEffect_SpawnRandomEveryFourFrames(
				(((neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
			inner = (inner + 1);
			v8 = (v8 + -0x10000);
		} while ((u32)inner <= 7);
		Map_CopyCellsTo(76, 59, (108 - outer), 36, 2, 1);
		next = outer + 1;
		FieldScene_RunOpeningAuxiliarySequence(a0, outer, next);
		outer = next;
	} while ((u32)next <= 1);
	Engine_EventWait(a0);
	FieldScene_RunOpeningAuxiliarySequence(0, next, (next + 1));
	Engine_AudioPlayCue(211);
	FieldScheduler_AddCallback((s32)MercuryLighthouse_SprayAtFountain, 0xc80);
	Engine_MapRenderWaitForValues();
}

static void MercuryLighthouse_RunSequenceF(void)
{
	s32 *rec;
	s32	 outer;
	s32	 inner;
	s32	 base;
	s32	 raw;
	s32	 pos;
	s32	 shown;
	s32	 arr[10];

	Map_CopyCellsTo(78, 58, 110, 36, 1, 1);
	rec = arr;
	rec[1] = 5;
	rec[2] = 0x8000;
	rec[3] = 0x8000;
	outer = 0;
	do
	{
		base = -0x20000;
		inner = 1;
		do
		{
			if ((inner & 1) != 0)
			{
				raw = Engine_RandomNext();
				shown = ((0x248 - ((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
				pos = (base - (outer << 19)) + 0x2d80000;
				Effect_Spawn(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
				Engine_EventWait(1);
			}
			inner++;
			base = base + -0x20000;
		} while ((u32)inner <= 7);
		Map_CopyCellsTo(111, 35, (109 - outer), 36, 1, 1);
		outer++;
	} while ((u32)outer <= 2);
	Engine_TaskRemoveCallback((s32)MercuryLighthouse_SprayAtFountain);
}

static void MercuryLighthouse_RunSequenceE(void)
{
	struct StagedActorProbe probe;
	s32						kind;
	s32						shape;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		kind = probe.actorSlot;
		if (kind == 8)
		{
			shape = probe.positionX;
			if ((shape >> 20) == 11)
			{
				SceneActor_MoveAndRedraw(probe);
				Engine_EventWait(30);
				Engine_AudioPlayCue(211);
				SceneEffect_SpawnParticleRowsAndDrawTiles();
				Map_CopyCellsTo(76, 60, 74, 38, 3, 1);
				Map_CopyCellsTo(77, 60, 76, 38, 2, 1);
				Map_CopyCellsTo(75, 58, 86, 41, 1, 3);
				Map_CopyCellsTo(75, 59, 86, 43, 1, 2);
				Map_CopyCellsTo(76, 59, 80, 49, 2, 1);
				Map_CopyCellsTo(77, 59, 82, 49, 2, 1);
				Engine_GameFlagSet(0x302);
			}
			else
			{
				probe.callback = MercuryLighthouse_OpenThreeStepStair;
				Map_CopyCellsTo(75, 57, 86, 41, 1, 1);
				Map_CopyCellsTo(71, 59, 86, 42, 1, 1);
				Map_CopyCellsTo(71, 59, 86, 43, 1, 1);
				Map_CopyCellsTo(71, 59, 86, 44, 1, 1);
				Map_CopyCellsTo(71, 59, 80, 49, 1, 1);
				Map_CopyCellsTo(71, 59, 81, 49, 1, 1);
				Map_CopyCellsTo(71, 59, 82, 49, 1, 1);
				Map_CopyCellsTo(78, 58, 83, 49, 1, 1);
				SceneActor_MoveAndRedraw(probe);
				GameFlag_Clear(0x302);
			}
		}
		else if (kind == 10)
		{
			if ((probe.positionZ >> 20) == 40)
			{
				SceneActor_MoveAndRedraw(probe);
				if (GameFlag_IsSet(0x307) == 0)
				{
					Camera_SetSpeed(0x18000, 0x3000);
					Camera_MoveTo(0x2ca0000, -1, 0x2500000, 1);
					Engine_CameraWaitForMove();
					GameFlag_Set(0x307);
					FieldScene_RunSupplementalSequenceOne(5);
					Engine_EventWait(50);
				}
				else
				{
					FieldScene_RunSupplementalSequenceOne(5);
				}
				GameFlag_Set(0x306);
			}
			else if ((probe.positionZ >> 20) == 42)
			{
				probe.callback = MercuryLighthouse_RunSequenceF;
				SceneActor_MoveAndRedraw(probe);
				MercuryLighthouse_RunSequenceG(5);
				GameFlag_Clear(0x306);
			}
		}
	}
	Engine_EventEnd();
}

static void FieldScene_RunFourSteps(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	EV08_PSYNERGY_3();
}

static void EV08_PSYNERGY_3(void)
{
	s32 x = Object_GetById(8)->x.fixed / 0x100000;
	s32 y = Object_GetById(8)->z.fixed / 0x100000;

	Engine_EventBegin();
	if (x == 10 && y == 23)
	{
		s32 *p;
		Object_GetById(8)->y.fixed += 0xfffe0000;
		p = (s32 *)Object_GetById(8);
		p[15] = Object_GetById(8)->y.fixed;
		Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
		Engine_AudioPlayCue(0xd9);
		Engine_MapAnimateCells(MercuryLighthouse_FloorSwitchCells, 10, 18);
		Map_CopyCellAttributes(10, 16, 1, 1, x, 19);
		Engine_GameFlagSet(0x878);
	}
	Engine_EventEnd();
}

static void EV12_TOUCH_3(void)
{
	s32 x;
	s32 z;

	if (GameFlag_IsSet(0x256) != 0)
		return;
	x = Object_GetById(0)->x.part.pixel;
	z = Object_GetById(0)->z.part.pixel;
	if (x < 164 || x > 171)
		return;
	if (z < 372)
		return;
	if (z >= 380)
		return;
	Engine_EventBegin();
	Engine_GameFlagSet(0x256);
	Engine_EventWait(5);
	Object_GetById(0)->y.fixed -= 0x20000;
	Object_GetById(0)->targetY = Object_GetById(0)->y.fixed;
	Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
	Engine_AudioPlayCue(217);
	Engine_MapAnimateCells(MercuryLighthouse_FloorSwitchCells, 10, 18);
	Engine_EventEnd();
}

static void EV10_TOUCH(void)
{
	struct FieldActor *actor;

	if (GameFlag_IsSet(0x256) != 0)
	{
		Engine_EventBegin();
		GameFlag_Clear(0x256);
		Actor_Get(0)->y.fixed += 0x20000;
		actor = Actor_Get(0);
		actor->targetY = Actor_Get(0)->y.fixed;
		Engine_EventWait(5);
		Map_CopyCellsTo(8, 29, 10, 23, 1, 1);
		Engine_AudioPlayCue(217);
		Map_AnimateCells(MercuryLighthouse_FloorSwitchCloseCells, 10, 18);
		Engine_EventEnd();
	}
}

static __inline__ s32 Door_DriftStep(void)
{
	u32 drift = Engine_RandomNext();

	return ((drift << 3) >> 16) * 0x3333;
}

static void MercuryLighthouse_OpenStoneDoor(s32 side)
{
	struct EffectOptions  params;
	u32					  j;
	struct EffectOptions *p;
	u32					  i;

	Engine_AudioPlayCue(211);
	if (side == 0)
	{
		Map_CopyCellsTo(111, 57, 113, 42, 1, 1);
		Map_CopyCellsTo(111, 59, 113, 43, 1, 1);
	}
	else if (side == 1)
	{
		Map_CopyCellsTo(113, 58, 112, 46, side, side);
		Map_CopyCellsTo(115, 58, 113, 46, side, side);
	}
	else
	{
		Map_CopyCellsTo(115, 57, 116, 44, 1, 1);
		Map_CopyCellsTo(113, 57, 115, 44, 1, 1);
	}
	p = &params;
	p->palette = 7;
	p->startScaleX = 0x8000;
	p->startScaleY = 0x8000;
	for (i = 0; i <= 1; i++)
	{
		for (j = 0; j <= 7; j++)
		{
			if (j & 1)
			{
				if (side == 0)
				{
					Effect_Spawn(0x3180000, 0, 0x2c00000 + (i * 16 + j) * 0x10000,
						Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
				}
				else if (side == 1)
				{
					Effect_Spawn(0x3200000 + (i * 16 + j) * 0x10000, 0, 0x2ea0000,
						Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
				}
				else
				{
					Effect_Spawn(0x32c0000 + (i * 16 + j) * -0x10000, 0, 0x2ca0000,
						Door_DriftStep() + -0xcccc, 0, Door_DriftStep() + -0xcccc, 0x90000, p);
				}
				Engine_EventWait(1);
			}
		}
		if (side == 0)
		{
			Map_CopyCellsTo(111, 58, 113, i + 43, 1, 1);
			Map_CopyCellsTo(111, 59, 113, i + 44, 1, 1);
		}
		else if (side == 1)
		{
			Map_CopyCellsTo(114, 58, i + 113, 46, side, side);
			Map_CopyCellsTo(115, 58, i + 114, 46, side, side);
		}
		else
		{
			Map_CopyCellsTo(114, 57, 115 - i, 44, 1, 1);
			Map_CopyCellsTo(113, 57, 114 - i, 44, 1, 1);
		}
	}
}

struct LiftLayer
{
	u8	unk00[12];
	s32 offset;
	u8	unk10[12];
	s32 unk1c;
};

static void SceneEffect_SpawnParticleRowsByMode(s32 mode)
{
	s32 buf[10];
	u32 i, j;

	Map_CopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
	Map_CopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
	Map_CopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
	Engine_AudioPlayCue(0x121);
	buf[1] = 5;
	buf[2] = 0x8000;
	buf[3] = 0x8000;
	for (j = 0; j <= 2; j++)
	{
		for (i = 1; i <= 7; i++)
		{
			if ((i & 1) != 0)
			{
				if (mode == 0)
				{
					Effect_Spawn((0x319 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0,
						(((j << 2) + i) << 17) + 0x02b70000, 0, mode, 0x4000, 0x90000, buf);
				}
				else if (mode == 1)
				{
					Effect_Spawn((((j << 2) + i) << 17) + 0x03120000, 0,
						((((u32)Engine_RandomNext() * 5) >> 16) << 16) + 0x2e80000, 0x4000, 0, 0,
						0x90000, buf);
				}
				else
				{
					Effect_Spawn(0x3380000 - (i << 17) - (j << 19), 0,
						((((u32)Engine_RandomNext() * 5) >> 16) << 16) + 0x2c80000, 0x4000, 0, 0,
						0x90000, buf);
				}
				Engine_EventWait(1);
			}
		}
		if (mode == 0)
			Map_CopyCellsTo(0x70, 0x3a, 0x71, j + 43, 1, 1);
		else if (mode == 1)
			Map_CopyCellsTo(0x70, 0x3a, j + 113, 0x2e, mode, mode);
		else
			Map_CopyCellsTo(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
	}
}

static s32 MercuryLighthouse_StartPillarPush(void)
{
	s32 cellX;
	s32 cellZ;

	cellX = Object_GetById(9)->x.fixed / 0x100000;
	cellZ = Object_GetById(9)->z.fixed / 0x100000;
	Engine_CameraSetSpeed(0x50000, 0xa000);
	Engine_CameraMoveTo(0x3300000, -1, 0x2c80000, 1);
	Engine_CameraWaitForMove();
	if (!Engine_GameFlagIsSet(0x877))
	{
		if (cellX == 50 && Engine_GameFlagIsSet(0x319))
		{
			Engine_ActorEnableActionCallback(9, MercuryLighthouse_PushScriptA);
		}
		else if (cellX == 49)
		{
			if (cellZ == 44 && !Engine_GameFlagIsSet(0x319) && !Engine_GameFlagIsSet(0x31a)
				&& !Engine_GameFlagIsSet(0x31b))
			{
				Engine_ActorEnableActionCallback(9, MercuryLighthouse_PushScriptB);
			}
			else if (cellZ == 44 && Engine_GameFlagIsSet(0x319))
			{
				Engine_ActorEnableActionCallback(9, MercuryLighthouse_PushScriptC);
			}
			else if (cellZ == 46 && Engine_GameFlagIsSet(0x31a))
			{
				Engine_ActorEnableActionCallback(9, MercuryLighthouse_PushScriptD);
				Engine_EventWait(30);
				return 1;
			}
		}
	}
	Engine_EventWait(30);
	return 0;
}

static void SceneActor_UseActorNinePositionWithYOffset(void)
{
	s32 *p = Object_GetById(9);
	u32	 v = Engine_RandomNext();

	s32 b = p[3] + (((v << 2) >> 16) << 16);
	s32 c = p[4];

	SceneEffect_SpawnRandomEveryFourFrames(p[2], b, c);
}

static s32 MercuryLighthouse_StartActorNineParticles(void)
{
	s32 n = 0xc80;

	FieldScheduler_AddCallback((s32)SceneActor_UseActorNinePositionWithYOffset, n);
	return 0;
}

static s32 MercuryLighthouse_StopActorNinePositionCallback(void)
{
	Engine_TaskRemoveCallback((s32)SceneActor_UseActorNinePositionWithYOffset);
	return 0;
}

static void EV40_SCRIPT(void)
{
	struct StagedActorProbe probe;
	s32						column;
	s32						start;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		switch (probe.actorSlot)
		{
		case 10:
			SceneActor_MoveAndRedraw(probe);
			if (probe.positionZ >> 20 == 38)
				Engine_GameFlagSet(0x318);
			else
				Engine_GameFlagClear(0x318);
			break;
		case 11:
			start = Object_GetById(11)->x.fixed >> 20;
			SceneActor_MoveAndRedraw(probe);
			column = probe.positionX >> 20;
			if (column == 47)
			{
				Engine_GameFlagSet(0x319);
				Engine_GameFlagClear(0x31a);
				Engine_GameFlagClear(0x31b);
				MercuryLighthouse_StartPillarPush();
				if (start == 54)
					SceneEffect_SpawnParticleRowsByMode(0);
				else if (start == 48)
					SceneEffect_SpawnParticleRowsByMode(1);
				MercuryLighthouse_OpenStoneDoor(2);
				Engine_EventWait(60);
			}
			else if (column == 48)
			{
				Engine_GameFlagSet(0x31a);
				Engine_GameFlagClear(0x31b);
				Engine_GameFlagClear(0x319);
				if (MercuryLighthouse_StartPillarPush() != 0)
				{
					s32 x;

					SceneEffect_SpawnParticleRowsByMode(2);
					x = 210 << 18;
					MercuryLighthouse_StopActorNinePositionCallback();
					MercuryLighthouse_OpenStoneDoor(1);
					Object_RefreshSelectorById(9);
					OverlayObject_PrepareObject(x, 0, 0x3120000, 223);
					OverlayObject_PrepareObject(x, 0, 0x3320000, 223);
					Engine_ActorSetDestination(9, 0x348, 0x2e8);
					Engine_EventWait(5);
					Engine_AudioPlayCue(189);
					Engine_ActorWaitForMove(9);
					Engine_EventWait(40);
					Engine_GameFlagSet(0x877);
					Engine_ColorBufferApplySource(0x10000, 0);
					*(s32 *)&(*(struct GameState **)&gEventWork)->scene = 0x100;
					Engine_EventCloseScreen();
					Engine_EventWaitForScreen();
					Engine_EventRequestExit(15);
				}
				else
				{
					SceneEffect_SpawnParticleRowsByMode(2);
					MercuryLighthouse_StopActorNinePositionCallback();
					MercuryLighthouse_OpenStoneDoor(1);
					Engine_EventWait(60);
				}
			}
			else if (column == 53)
			{
				Engine_GameFlagSet(0x31b);
				Engine_GameFlagClear(0x319);
				Engine_GameFlagClear(0x31a);
				MercuryLighthouse_StartPillarPush();
				SceneEffect_SpawnParticleRowsByMode(0);
				Engine_EventWait(60);
			}
			else
			{
				Engine_GameFlagClear(0x319);
				Engine_GameFlagClear(0x31a);
				Engine_GameFlagClear(0x31b);
				MercuryLighthouse_StartPillarPush();
				if (start == 47)
					SceneEffect_SpawnParticleRowsByMode(2);
				else if (start == 48)
					SceneEffect_SpawnParticleRowsByMode(1);
				MercuryLighthouse_OpenStoneDoor(0);
				Engine_EventWait(60);
			}
			break;
		}
	}
	Engine_EventEnd();
}

static void EV47_SCRIPT(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	MercuryLighthouse_SyncBlockFlags();
	Engine_EventEnd();
}

static void MercuryLighthouse_SyncBlockFlags(void)
{
	s32 i;
	s32 flag;
	s32 x;

	Engine_EventBegin();
	for (i = 0, flag = 0x330; i <= 3; i++, flag += 2)
	{
		x = Object_GetById(i + 15)->x.fixed / 0x100000;
		if (x == (i << 2) + 39)
		{
			Engine_GameFlagSet(flag);
			Engine_GameFlagClear(flag + 1);
		}
		else if (x == (i << 2) + 41)
		{
			Engine_GameFlagSet(flag + 1);
			Engine_GameFlagClear(flag);
		}
		else
		{
			Engine_GameFlagClear(flag);
			Engine_GameFlagClear(flag + 1);
		}
	}
	x = Object_GetById(19)->x.fixed / 0x100000;
	if (x == 57)
	{
		Engine_GameFlagSet(0x338);
		Engine_GameFlagClear(0x339);
		Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
	}
	else if (x == 59)
	{
		Engine_GameFlagSet(0x339);
		Engine_GameFlagClear(0x338);
		Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
	}
	else
	{
		Engine_GameFlagClear(0x338);
		Engine_GameFlagClear(0x339);
		Map_CopyCellAttributes(53, 11, 1, 1, 58, 7);
	}
	Engine_EventEnd();
}

static void EV20_PSYNERGY(void)
{
	s32 col;

	Engine_EventBegin();
	col = Object_GetById(20)->x.fixed / 0x100000;
	if (col == 28)
	{
		Engine_GameFlagSet(840);
		{
			s32 a = 31;
			s32 b = 20;

			Map_CopyCellAttributes(29, 20, 1, 1, a, b);
		}
	}
	Engine_EventEnd();
}

static void SceneEffect_RotatePaletteEntries40To47(void)
{
	unsigned int index;
	u16			*dst;
	u16			*src;
	u32			 front;

	if ((gFrameCount & 7) != 0)
	{
		return;
	}

	dst = (u16 *)0x05000050;
	front = *dst;
	index = 0;
	*(u16 *)0x0500005e = front;

	src = (u16 *)0x05000052;
	while (index <= 6)
	{
		*dst++ = *src++;
		index++;
	}
}

static void FieldScene_RunActorThreeBranchSequence(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(3, FX16_0_8, FX16_0_4);
	Actor_SetSpeed(0, FX16_0_8, FX16_0_4);
	Engine_EventSetMessage(MsgImilSomebodyHere);
	Event_ShowMessageAndWait(3, 0, 20);
	Actor_WalkToAndWait(3, 0x348, 0x288);
	Actor_ShowEmote(3, 0x100, 60);
	Actor_FaceDirection(3, FX16_0_5, 20);
	Engine_ActorSetAnimation(3, 16);
	record = Object_GetById(3);
	record->scaleX = -FX16_1_0;
	Engine_EventWait(20);
	Event_ShowMessageAndWait(3, 0, 20);
	Engine_ActorSetAnimation(3, 1);
	record = Object_GetById(3);
	record->scaleX = FX16_1_0;
	Engine_EventWait(20);
	Actor_FaceDirection(3, FX16_0_25, 20);
	Event_OpenMessage(3, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(3, 3);
		Event_ShowMessageAndWait(3, 0, 20);
		gEventWork->message += 1;
	}
	else
	{
		gEventWork->message += 1;
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(3, 4);
		Event_ShowMessageAndWait(3, 0, 20);
	}
	Engine_EventWait(20);
	Actor_FaceDirection(3, FX16_0_75, 20);
	Camera_SetSpeed(FX16_0_8, FX16_0_1);
	Camera_MoveTo(0x3480000, -1, 0x2780000, 1);
	Actor_WalkToAndWait(3, 0x348, 0x278);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(3, 2);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(3, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(3, 0, 20);
	Engine_GameFlagSet(0x870);
	Engine_EventEnd();
}

static void EV03_TALK(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimationAndWait(3, 4);
	Engine_EventWait(20);
	Engine_EventSetMessage(MSGNO(MsgImilStatueBlocksEntrance));
	Engine_EventShowMessageAndWait(3, 0, 20);
	Engine_EventEnd();
}

static void MercuryLighthouse_FadePaletteToWhite(void)
{
	/* Palette RAM is hardware (its volatile is source, not a device): volatile makes each read of palette RAM a load of its own: the native reads every color with three ldrh, one per component; without it one load is shifted for all three and the function is 104 bytes for 106 in tbs-en (2026-10-13). */
	volatile u16 *pal;
	u32			  i;
	u32			  done;
	s32			  r;
	s32			  g;
	s32			  b;

	do
	{
		pal = (volatile u16 *)0x05000050;
		done = 0;
		for (i = 0; i <= 7; i++)
		{
			r = *pal & 31;
			g = (*pal >> 5) & 31;
			b = (*pal >> 10) & 31;
			if (r == 31 && g == 31 && b == 31)
			{
				done++;
			}
			else
			{
				if (r <= 30)
					r++;
				if (g <= 30)
					g++;
				if (b <= 30)
					b++;
				*pal = (b << 10) | (g << 5) | r;
			}
			pal++;
		}
		Engine_TaskWait(2);
	} while (done <= 7);
}

static void EV10_PSYNERGY(void)
{
	struct FieldActor *record;
	s32				   pos;

	record = Object_GetById(10);
	pos = record->x.fixed;
	if (pos < 0)
		pos += 0xfffff;
	pos >>= 20;
	Engine_EventBegin();
	if (pos != 51)
	{
	}
	else
	{
		Engine_ActorSetSpeed(3, 0xcccc, 0x6666);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(3, 2);
		Engine_EventWait(20);
		Actor_FaceDirection(3, 0xd000, 0);
		Engine_ActorFaceDirection(0, 0x5000, 10);
		Engine_EventSetMessage(MsgMercuryDidThat);
		Engine_EventOpenMessage(3, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			gEventWork->message += 1;
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(3, 3);
			Engine_EventShowMessageAndWait(3, 0, 20);
		}
		else
		{
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(3, 4);
			Event_ShowMessageAndWait(3, 0, 20);
			gEventWork->message += 1;
		}
		Engine_ActorShowEmote(0, 0x100, 60);
		record = Object_GetById(1);
		SetEffectRecordMode(record, 1);
		record = Object_GetById(2);
		SetEffectRecordMode(record, 1);
		Actor_SetSpeed(1, 0xcccc, 0x6666);
		Actor_SetSpeed(2, 0xcccc, 0x6666);
		Actor_SetPosition(1, 0x3680000, 0x2580000);
		Actor_SetPosition(2, 0x3680000, 0x2580000);
		Actor_WalkTo(2, 0x378, 0x278);
		Actor_WalkToAndWait(1, 0x370, 0x268);
		Actor_FaceDirection(1, 0x5000, 0);
		Engine_ActorWaitForMove(2);
		Actor_FaceDirection(2, 0x8000, 0);
		Engine_ActorRunRepeatedMotion(1, 1);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(1, 0, 20);
		Actor_ShowEmote(3, 0x101, 60);
		Event_ShowMessageAndWait(3, 0, 20);
		Actor_FaceDirection(1, 0x3000, 0);
		Actor_FaceDirection(2, 0xb000, 20);
		Engine_ActorSetAnimation(1, 3);
		Engine_ActorSetAnimationAndWait(2, 3);
		Engine_EventWait(30);
		Actor_FaceDirection(1, 0x5000, 0);
		Actor_FaceDirection(2, 0x8000, 20);
		Engine_EventShowMessageAndWait(2, 0, 20);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(3, 0, 20);
		Engine_ActorWalkTo(3, 0x348, 0x290);
		Engine_EventWait(5);
		Engine_ActorFaceDirection(2, 0x5000, 0);
		Engine_EventWait(10);
		Engine_ActorFaceDirection(0, 0x4000, 0);
		Engine_ActorWaitForMove(3);
		Engine_EventWait(10);
		Engine_EventShowMessageAndWait(3, 0, 20);
		Actor_FaceDirection(3, 0xd000, 20);
		Engine_EventShowMessageAndWait(3, 0, 20);
		Engine_ActorRunRepeatedMotion(1, 1);
		Engine_EventWait(20);
		Actor_FaceDirection(1, 0xc000, 20);
		Engine_EventOpenMessage(1, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(1, 3);
			Engine_EventShowMessageAndWait(1, 0, 20);
			gEventWork->message += 1;
		}
		else
		{
			gEventWork->message += 1;
			Engine_EventWait(20);
			Engine_ActorSetAnimationAndWait(1, 4);
			Engine_EventShowMessageAndWait(1, 0, 20);
		}
		Engine_ActorSetAnimation(3, 16);
		Engine_EventWait(30);
		Engine_ActorRunRepeatedMotion(3, 1);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(3, 0, 20);
		Actor_WalkToAndWait(3, 0x348, 0x278);
		Actor_FaceDirection(0, 0x5000, 0);
		Actor_FaceDirection(1, 0x5000, 0);
		Actor_FaceDirection(2, 0x8000, 0);
		Engine_EventWait(30);
		MercuryLighthouse_CastPsynergyAtActor11();
		Engine_EventWait(50);
		Engine_AudioPlayCue(131);
		ColorBuffer_ApplySource(0x10000, 0);
		ColorBuffer_ApplyTarget(0x207e9f, 0);
		Engine_ColorBufferInterpolate(10);
		Engine_TaskWait(1);
		Engine_AudioPlayCue(220);
		Engine_TaskWait(40);
		Engine_ColorBufferApplyTarget(0x10000, 0);
		Engine_ColorBufferInterpolate(60);
		Engine_TaskWait(60);
		Engine_AudioPlayCue(209);
		MercuryLighthouse_FadePaletteToWhite();
		Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
		Engine_MapAnimateCells((s32)MercuryLighthouse_ColumnCells, 116, 35);
		Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries40To47);
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(20);
		Actor_SetSpeed(3, 0x30000, 0x18000);
		Engine_ActorJump(3, 4, 0);
		Actor_WalkTo(3, 0x348, 0x258);
		Engine_ActorWaitForMove(3);
		Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
		OverlayObject_PrepareObject(0x3480000, 0x380000, 0x2600000, 223);
		Actor_SetSpeed(3, 0xcccc, 0x6666);
		Actor_WalkToAndWait(3, 0x348, 0x230);
		Engine_ActorSetPosition(3, 0, 0);
		Engine_EventWait(20);
		Actor_FaceDirection(0, 0x4000, 0);
		Actor_FaceDirection(1, 0x8000, 0);
		Engine_ActorFaceDirection(2, 0xb000, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(0, 3);
		Engine_ActorSetAnimation(1, 3);
		Engine_ActorSetAnimationAndWait(2, 3);
		Engine_EventWait(20);
		Engine_ActorSetAnimation(1, 2);
		record = Object_GetById(0);
		if (record != 0)
		{
			Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorSetAnimation(2, 2);
		record = Object_GetById(0);
		if (record != 0)
		{
			Actor_SetDestination(2, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(1);
		Engine_ActorSetPosition(1, 0, 0);
		Engine_ActorWaitForMove(2);
		Engine_ActorSetPosition(2, 0, 0);
		Engine_GameFlagSet(0x871);
	}
	Engine_EventEnd();
}

static void SceneEffect_RotatePaletteEntries97To103(void)
{
	unsigned int i;
	u16			*dst;
	u16			*src;
	u32			 first;

	if ((gFrameCount & 7) != 0)
	{
		return;
	}

	dst = (u16 *)0x050000c2;
	first = *dst;
	i = 0;
	*(u16 *)0x050000ce = first;

	src = (u16 *)0x050000c4;
	while (i <= 5)
	{
		*dst++ = *src++;
		i++;
	}
}

static void MercuryLighthouse_FadePaletteToBlack(void)
{
	/* Palette RAM is hardware (its volatile is source, not a device): volatile makes each read of palette RAM a load of its own: the native reads every color with three ldrh, one per component, and again after the store; without it one load is shifted for all three and the function is 98 bytes for 100 in tbs-en (2026-10-13). */
	volatile u16 *pal;
	u32			  i;
	s32			  done;
	s32			  r;
	s32			  g;
	s32			  b;

	do
	{
		pal = (volatile u16 *)0x050000c2;
		done = 0;
		for (i = 0; i <= 6; i++)
		{
			r = *pal & 31;
			g = (*pal >> 5) & 31;
			b = (*pal >> 10) & 31;
			if (r > 0)
				r--;
			if (g > 0)
				g--;
			if (b > 0)
				b--;
			*pal = (b << 10) | (g << 5) | r;
			if (*pal == 0)
				done++;
			pal++;
		}
		Engine_TaskWait(5);
	} while (done != 7);
}

static void MercuryLighthouse_SinkActorWithSparks(s32 actor)
{
	struct EffectOptions  params;
	struct EffectOptions *p;
	struct FieldActor	 *rec;
	u32					  i;
	s32					  x;
	s32					  y;
	s32					  speed;

	rec = Object_GetById(actor);
	rec->motionFlags = 0;
	Engine_ActorSetSpriteFlags(rec, 0);
	Actor_SetChildValue(actor, 0x100);
	Engine_AudioPlayCue(221);
	p = &params;
	p->priority = 1;
	p->palette = 5;
	p->type = 0x11e;
	p->script = MercuryLighthouse_SparkBurstScript;
	for (i = 0; i <= 47; i++)
	{
		if (i <= 31)
			SceneEffect_SpawnWithRandomOffset(rec->x.fixed, rec->y.fixed, rec->z.fixed);
		if (i & 1)
		{
			Engine_AudioPlayCue(246);
			x = rec->x.fixed + ((((u32)Engine_RandomNext() * 24) >> 16) << 16) + -0xc0000;
			y = rec->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000;
			speed = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + 0x8000;
			Effect_Spawn(x, y, rec->z.fixed, 0, speed, 0, 0x330000, p);
		}
		rec->y.fixed += i * 0x1999;
		rec->targetY = rec->y.fixed;
		Engine_TaskWait(2);
	}
}

static void EV12_PSYNERGY(void)
{
	struct ConfiguredEffectOptions *effect;
	s32								row_offset;
	u32								value;
	s32								zero;
	s32								phase;
	s32								particle;
	s32								x;
	s32								velocityX;
	struct ConfiguredEffectOptions	options;

	Engine_EventBegin();
	Engine_EventWait(20);
	MercuryLighthouse_FadePaletteToBlack();
	Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
	Map_CopyCellsTo(45, 77, 45, 73, 9, 4);
	Engine_EventWait(30);
	effect = &options;
	effect->modeBits = 1;
	effect->mode = 5;
	effect->kind = 0x11e;
	effect->callbackArg = (s32)MercuryLighthouse_SparkBurstScript;
	zero = 0;
	phase = zero;
	do
	{
		u32 x, z;

		if ((1 & phase) != 0)
		{
			Engine_AudioPlayCue(246);
		}
		value = Engine_RandomNext();
		x = value * 48;
		x >>= 16;
		x <<= 16;
		x += 0x3000000;
		value = Engine_RandomNext();
		z = value * 56;
		z >>= 16;
		z <<= 16;
		z += 0x880000;
		Effect_Spawn(x, 0, z, 0, 0, 0, 0x330001, effect);
		Engine_EventWait(2);
		phase = (phase + 1);
	} while ((u32)phase <= 15);
	Engine_EventWait(40);
	zero = 0;
	phase = zero;
	do
	{
		u32 x, z;
		s32 speed;

		if ((1 & phase) != 0)
		{
			Engine_AudioPlayCue(246);
		}
		value = Engine_RandomNext();
		x = value * 48;
		x >>= 16;
		x <<= 16;
		x += 0x3000000;
		value = Engine_RandomNext();
		z = value * 56;
		z >>= 16;
		z <<= 16;
		z += 0x980000;
		value = Engine_RandomNext();
		speed = -((value * 10 >> 16) * 0x3333) - 0x3333;
		Effect_Spawn(x, 0, z, 0, 0, speed, 0x330001, effect);
		Engine_EventWait(2);
		phase = (phase + 1);
	} while ((u32)phase <= 15);
	Engine_EventWait(60);
	Engine_AudioPlayCue(141);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Engine_EventWait(60);
	effect->mode = 7;
	effect->accum18 = 0xb333;
	effect->accum1c = 0xb333;
	effect->target30 = 0x13333;
	effect->target34 = 0x13333;
	zero = 0;
	phase = zero;
	do
	{
		s32 speed;

		Map_CopyCellsTo(59, (12 - phase), 48, (12 - phase), 3, 1);
		particle = 0;
		row_offset = (phase << 4);
		do
		{
			value = Engine_RandomNext();
			x = ((((((value << 1) + value) << 4) >> 16) << 16) + 0x3000000);
			velocityX = (0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16)) - 0x6664;
			speed = 0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16);
			Effect_Spawn(x, 0, ((-((u32)particle >> 1) - row_offset) << 16) + 0xc00000, velocityX,
				0, speed, 0xd0001, effect);
			particle = (particle + 1);
			Engine_EventWait(2);
		} while ((u32)particle <= 31);
		phase = (phase + 1);
	} while ((u32)phase <= 3);
	Engine_AudioPlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	BattleFx_PlayQueuedSound();
	Engine_EventWait(30);
	if (GameFlag_IsSet(0x881) != 0)
	{
		Actor_SetSpeed(0, 0xcccc, 0x6666);
		Actor_WalkToAndWait(0, 0x338, 232);
		Actor_WalkToAndWait(0, 0x318, 232);
		Actor_WalkToAndWait(0, 0x318, 208);
		Actor_SetSpeed(0, 0x20000, 0x10000);
		Engine_ActorJump(0, 4, 0);
		Actor_WalkTo(0, 0x318, 200);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(0, 18);
		MercuryLighthouse_SinkActorWithSparks(0);
		Engine_EventWait(60);
		Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
		ColorBuffer_ApplySource(0x10000, 0);
		ColorBuffer_ApplyTarget(0x10005, 0);
		Engine_ColorBufferInterpolate(120);
		Engine_EventWait(120);
		ColorBuffer_ApplyTarget(0x7fff, 0);
		Engine_ColorBufferInterpolate(60);
		Engine_EventWait(60);
		Engine_EventRequestExit(9);
		Engine_EventEnd();
	}
	else
	{
		Actor_SetSpeed(0, 0xcccc, 0x6666);
		Actor_SetSpeed(1, 0xcccc, 0x6666);
		Actor_SetSpeed(2, 0xcccc, 0x6666);
		Actor_SetSpeed(3, 0xcccc, 0x6666);
		Actor_WalkToAndWait(0, 0x338, 240);
		Actor_FaceDirection(0, 0xa000, 20);
		Actor_SetPosition(3, 0x3380000, 0xf00000);
		Actor_WalkToAndWait(3, 0x318, 232);
		Actor_FaceDirection(3, 0xc000, 0);
		Engine_EventWait(60);
		Actor_FaceDirection(3, 0x2000, 20);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(40);
		Actor_FaceDirection(3, 0xc000, 20);
		Actor_WalkToAndWait(3, 0x318, 200);
		MercuryLighthouse_SinkActorWithSparks(3);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(0, 2);
		Engine_EventWait(30);
		Actor_WalkToAndWait(0, 0x318, 232);
		Actor_FaceDirection(0, 0xc000, 0);
		Actor_SetPosition(1, 0x3180000, 0xe80000);
		Actor_SetPosition(2, 0x3180000, 0xe80000);
		Actor_WalkTo(1, 0x330, 224);
		Actor_WalkToAndWait(2, 0x300, 224);
		Engine_ActorWaitForMove(1);
		Actor_FaceDirection(1, 0xa000, 0);
		Actor_FaceDirection(2, 0xe000, 20);
		Actor_ShowEmote(0, 0x102, 0);
		Actor_ShowEmote(1, 0x102, 0);
		Actor_ShowEmote(2, 0x102, 80);
		Actor_FaceDirection(1, 0x6000, 0);
		Actor_FaceDirection(2, 0x2000, 20);
		Engine_ActorRunRepeatedMotion(0, 1);
		Engine_EventWait(60);
		Actor_SetSpeed(0, 0x8000, 0x4000);
		Actor_WalkToAndWait(0, 0x318, 224);
		Actor_FaceDirection(1, 0x8000, 0);
		Actor_FaceDirection(2, 0, 0);
		Actor_WalkToAndWait(0, 0x318, 208);
		Actor_FaceDirection(1, 0xa000, 0);
		Actor_FaceDirection(2, 0xe000, 20);
		Engine_ActorSetAnimationAndWait(0, 3);
		Engine_EventWait(20);
		Actor_SetSpeed(0, 0x20000, 0x10000);
		Engine_ActorJump(0, 4, 0);
		Actor_WalkTo(0, 0x318, 200);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(0, 18);
		Actor_ShowEmote(1, 0x100, 0);
		Actor_ShowEmote(2, 0x100, 0);
		Engine_ActorStartRepeatedMotion(1, 2);
		Engine_ActorStartRepeatedMotion(2, 2);
		MercuryLighthouse_SinkActorWithSparks(0);
		Engine_EventWait(60);
		Actor_ShowEmote(1, 0x102, 0);
		Actor_ShowEmote(2, 0x102, 80);
		Engine_ActorFaceEachOther(1, 2, 20);
		Engine_ActorSetAnimation(1, 3);
		Engine_ActorSetAnimationAndWait(2, 3);
		Engine_EventWait(40);
		Actor_WalkToAndWait(1, 0x318, 216);
		Actor_FaceDirection(2, 0xe000, 0);
		Actor_WalkToAndWait(1, 0x318, 200);
		Engine_EventWait(30);
		MercuryLighthouse_SinkActorWithSparks(1);
		Actor_WalkToAndWait(2, 0x318, 216);
		Actor_WalkToAndWait(2, 0x318, 200);
		Engine_EventWait(30);
		MercuryLighthouse_SinkActorWithSparks(2);
		ColorBuffer_ApplySource(0x10000, 0);
		Engine_TaskRemoveCallback((s32)SceneEffect_RotatePaletteEntries97To103);
		ColorBuffer_ApplyTarget(0x10005, 0);
		Engine_ColorBufferInterpolate(120);
		Engine_EventWait(120);
		ColorBuffer_ApplyTarget(0x7fff, 0);
		Engine_ColorBufferInterpolate(60);
		Engine_EventWait(60);
		Engine_EventEnd();
		Engine_EventRequestExit(8);
	}
}

static void EV42_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(0, 1);
	Engine_MessageShowCentered(MSGNO(MsgImilHermesHealingWaterFountain), 1);
	Engine_EventEnd();
}

static void EV20_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(0, 1);
	Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
	Engine_EventEnd();
}

static void EV05_SEARCH(void)
{

	Engine_EventBegin();
	Engine_ActorSetAnimation(0, 1);
	if (Engine_GameFlagIsSet(0x881) == 0)
		Engine_MessageShowCentered(MSGNO(MsgMercuryLighthouseFountainSeemsDry), 1);
	else
		Engine_MessageShowCentered(MSGNO(MsgMercuryLighthouseFountainFlowsWithWater), 1);
	if (PartyInventory_FindOwner(0xb9) != -1)
	{
		s16 *slot = (s16 *)gEventWork + 185;
		s32	 one = 1;

		*slot = one;
	}
	Engine_EventEnd();
}

static void EV05_SCRIPT(void)
{
	Engine_EventBegin();
	Engine_AudioPlayCue(0x53);
	Engine_ItemShowFound(ITEM_HERMES_WATER, 3);
	SceneState_SetRecordTableValue(0xb9, 0xb8);
	UiWork_PushValueSlot(PartyInventory_FindOwner(0xb8), 1);
	UiWork_PushValueSlot(0xb8, 2);
	Engine_MessageShowCentered(MSGNO(MsgMercuryLighthouseIsaacGot), 1);
	Engine_GameFlagSet(512);
	Engine_EventEnd();
}

static void EV15_TOUCH(void)
{
	Engine_EventBegin();
	if (gGameState.scene == (s32)&SceneId_MercuryLighthouse1)
	{
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d8, 0x258);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
		Camera_MoveTo(0x1d00000, -1, 0x2900000, 1);
		Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
		OverlayObject_PrepareObject(Object_GetById(ACTOR_PARTY_LEADER)->x.fixed, 0, 0x2be0000, 223);
		Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
		Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
		Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
		FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, -1);
		gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
		Engine_EventWait(60);
		Engine_EventRequestExit(8);
	}
	else
	{
		FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, -1);
	}
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/mercury_arrive.h"

static void SceneEffect_SpawnParticleRowsAndDrawTiles(void)
{
	s32 buf[10];
	u32 i, j;

	Map_CopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
	buf[1] = 7;
	buf[2] = 0x8000;
	buf[3] = 0x8000;
	for (j = 0; j <= 1; j++)
	{
		for (i = 0; i <= 7; i++)
		{
			if ((i & 1) != 0)
			{
				s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
				s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

				Effect_Spawn(
					0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000, a, 0, b, 0x90000, buf);
				Engine_EventWait(1);
			}
		}
		Map_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
		Map_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
	}
}

static void MercuryLighthouse_OpenThreeStepStair(void)
{
	struct EffectOptions  params;
	struct EffectOptions *p;
	u32					  i;
	u32					  j;
	s32					  z;

	Map_CopyCellsTo(76, 61, 74, 38, 1, 1);
	p = &params;
	p->palette = 5;
	p->startScaleX = 0x8000;
	p->startScaleY = 0x8000;
	for (i = 0; i <= 2; i++)
	{
		z = -0x20000;
		for (j = 1; j <= 7; j++)
		{
			if (j & 1)
			{
				if (j & 2)
				{
					Effect_Spawn((105 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0,
						z - (i << 19) + 0x22e0000, 0, 0, -0x4000, 0x90000, p);
				}
				else
				{
					Effect_Spawn((((i << 2) + j) << 17) + 0xb70000, 0,
						(0x26c - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0x4000, 0, 0,
						0x90000, p);
				}
				Engine_EventWait(1);
			}
			z += -0x20000;
		}
		Map_CopyCellsTo(71, 59, 70, 34 - i, 1, 1);
		Map_CopyCellsTo(71, 59, i + 75, 38, 1, 1);
	}
}

static void SceneEffect_SpawnParticleEveryFourthFrame(void)
{
	s32	 buf[10];
	s32 *p = Object_GetById(0);
	s32	 m = gFrameCount & 3;

	if (m == 0)
	{
		buf[1] = 7;
		if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
			buf[1] = 5;
		buf[2] = 0xb333;
		buf[3] = 0xb333;
		{
			s32 y = p[3] + ((((u32)Engine_RandomNext() << 2) >> 16) << 16);
			s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
			s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

			Effect_Spawn(p[2], y, p[4], a, b, m, 0x90000, buf);
		}
	}
}

static void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
	s32 buf[10];
	s32 m = gFrameCount & 3;

	if (m == 0)
	{
		buf[1] = 7;
		if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
			buf[1] = 5;
		buf[2] = 0xb333;
		buf[3] = 0xb333;
		{
			s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
			s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

			Effect_Spawn(x, y, z, a, b, m, 0x90000, buf);
		}
	}
}

static void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
	s32 buf[10];

	buf[1] = 7;
	buf[0] = 1;
	buf[2] = 0xb333;
	buf[3] = 0xb333;
	{
		s32 a = x + ((((u32)Engine_RandomNext() << 4) >> 16) << 16) + 0xfff80000;
		s32 b = z + ((((u32)Engine_RandomNext() << 3) >> 16) << 16) + 0xfffc0000;

		Effect_Spawn(a, y, b, 0, 0, 0, 0xb0000, buf);
	}
}

static void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
	s32 desc[10];
	s32 tmp[3];
	u32 i;

	Engine_AudioPlayCue(0xd8);
	i = 0;
	do
	{
		if ((i & 1) != 0)
		{
			desc[1] = 7;
			if ((i & 2) != 0)
				desc[1] = 5;
			desc[2] = 0x9999;
			desc[3] = 0x9999;
			tmp[0] = 0;
			tmp[1] = 0;
			tmp[2] = 0;
			Vector_AddPolarOffset((6 - (i >> 1)) * 0x1999, w, tmp);
			{
				s32 a = x + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);
				s32 b = z + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);

				Effect_Spawn(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
			}
		}
		Engine_TaskWait(2);
		i++;
	} while (i <= 11);
}

s32 Scene_Initialize(void)
{
	u32 i;
	s32 flag;
	s32 x;
	s32 set;

	Engine_GameFlagSet(0x111);
	gGameState.retreatEntrance = 11;
	gGameState.retreatScene = (s32)&SceneId_MercuryLighthouse4;
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	Io_Write16(0x3f40, &REG_BLDCNT);
	{
		s32 alpha = 0x1010;
		REG_BLDALPHA = alpha;
	}
	Mercury_SpawnLightObjects(21, (s32)gSceneState);
	BattleFx_StartFadeOverlay(0);
	if (Engine_GameFlagIsSet(0x875))
		FieldScheduler_AddCallback(Mercury_CyclePalette, 0xc80);
	else
		Mercury_ClearPalette();
	if (gGameState.scene == (s32)&SceneId_MercuryLighthouse1)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
		case 3:
			if (Engine_GameFlagIsSet(0x875))
				Map_CopyCellAttributes(66, 5, 27, 23, 2, 5);
			break;
		case 5:
		case 6:
			BattleFx_SetQueuedSoundAndPlay(170);
			break;
		case 15:
			SetEffectRecordMode(Object_GetById(0), 1);
		case 4:
			if (Engine_GameFlagIsSet(0x876))
			{
				Engine_TaskWait(1);
				Actor_SetPosition(9, 168 << 18, 128 << 16);
				Actor_SetPosition(10, 176 << 18, 192 << 16);
				Actor_SetPosition(11, 162 << 18, 240 << 16);
			}
			else if (!Engine_GameFlagIsSet(0x109))
			{
				Engine_GameFlagClear(0x310);
				Engine_GameFlagClear(0x311);
				Engine_GameFlagClear(0x312);
				Engine_GameFlagClear(0x313);
			}
			SetEffectRecordMode(Object_GetById(9), 1);
			SetEffectRecordMode(Object_GetById(10), 1);
			SetEffectRecordMode(Object_GetById(11), 1);
			FieldScene_RedrawActorFootprint(9);
			FieldScene_RedrawActorFootprint(10);
			FieldScene_RedrawActorFootprint(11);
			SetEffectRecordMode(Object_GetById(12), 1);
			if (Engine_GameFlagIsSet(0x310))
			{
				Map_CopyCellsTo(119, 9, 109, 11, 1, 1);
				if (Engine_GameFlagIsSet(0x311))
					Map_CopyCellsTo(118, 9, 104, 13, 1, 1);
				Engine_MapRedraw();
				Engine_TaskWait(1);
			}
			break;
		case 9:
		case 10:
			if (!Engine_GameFlagIsSet(0x873))
			{
				Actor_SetPosition(3, 174 << 18, 158 << 18);
				Engine_ActorFaceDirection(3, 0, 0);
			}
			else
			{
				Actor_SetPosition(8, 194 << 18, 158 << 18);
				Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
			}
			break;
		}
	}
	if (gGameState.scene == (s32)&SceneId_MercuryLighthouse2)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			SetEffectRecordMode(Object_GetById(8), 1);
			SetEffectRecordMode(Object_GetById(9), 1);
			if (Engine_GameFlagIsSet(0x302))
			{
				Engine_TaskWait(1);
				Engine_AudioPlayCue(211);
				Actor_SetPosition(8, 184 << 16, 132 << 18);
				Map_CopyCellAttributes(11, 31, 1, 4, 9, 31);
				Map_CopyCellAttributes(7, 30, 1, 4, 11, 31);
				Map_CopyCellsTo(74, 58, 70, 32, 1, 2);
				Map_CopyCellsTo(74, 59, 70, 34, 1, 1);
				Map_CopyCellsTo(76, 60, 74, 38, 3, 1);
				Map_CopyCellsTo(77, 60, 76, 38, 2, 1);
				Map_CopyCellsTo(75, 58, 86, 41, 1, 3);
				Map_CopyCellsTo(75, 59, 86, 43, 1, 2);
				Map_CopyCellsTo(76, 59, 80, 49, 2, 1);
				Map_CopyCellsTo(77, 59, 82, 49, 2, 1);
			}
			break;
		case 3:
		case 4:
			Engine_MapRedraw();
			Engine_TaskWait(1);
			if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256))
			{
				Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
				Map_CopyCellsTo(9, 1, 9, 7, 1, 2);
			}
			if (Engine_GameFlagIsSet(0x874))
			{
				Engine_ActorSetPosition(11, 176 << 15, 216 << 16);
				Object_GetById(11)->y.fixed += -0x20000;
				Object_GetById(11)->targetY = Object_GetById(11)->y.fixed;
				Map_CopyCellsTo(9, 1, 9, 7, 1, 2);
				Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
				Map_CopyCellAttributes(9, 5, 1, 1, 9, 10);
			}
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			FieldScheduler_AddCallback(MercuryLighthouse_UpdateLeaderEffectTarget, 0xc80);
#endif
			break;
		case 7:
		case 8:
		case 9:
			SetEffectRecordMode(Object_GetById(10), 1);
			if (Engine_GameFlagIsSet(0x306))
			{
				FieldScene_RunSupplementalSequenceOne(0);
				Map_CopyCellAttributes(42, 41, 4, 1, 42, 39);
				Map_CopyCellAttributes(42, 40, 4, 1, 42, 41);
				Actor_SetPosition(10, 176 << 18, 160 << 18);
			}
			break;
		}
	}
	if (gGameState.scene == (s32)&SceneId_MercuryLighthouse3)
	{
		switch (gGameState.entrance)
		{
		case 4:
		case 5:
		case 6:
			SetEffectRecordMode(Object_GetById(15), 1);
			SetEffectRecordMode(Object_GetById(16), 1);
			SetEffectRecordMode(Object_GetById(17), 1);
			SetEffectRecordMode(Object_GetById(18), 1);
			SetEffectRecordMode(Object_GetById(19), 1);
			SetEffectRecordMode(Object_GetById(0), 1);
			i = 0;
			x = 158 << 18;
			flag = 0x330;
			for (; i <= 3; i++)
			{
				if (Engine_GameFlagIsSet(flag))
					Engine_ActorSetPosition(i + 15, x, 176 << 15);
				else if (Engine_GameFlagIsSet(flag + 1))
					Engine_ActorSetPosition(i + 15, x + (128 << 14), 176 << 15);
				x += 128 << 15;
				flag += 2;
			}
			if (Engine_GameFlagIsSet(0x338))
			{
				Actor_SetPosition(19, 230 << 18, 176 << 15);
				Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
			}
			else if (Engine_GameFlagIsSet(0x339))
			{
				Actor_SetPosition(19, 238 << 18, 176 << 15);
				Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
			}
			break;
		case 10:
		case 11:
			if (Engine_GameFlagIsSet(0x348))
			{
				Actor_SetPosition(20, 228 << 17, 164 << 17);
				Map_CopyCellAttributes(29, 20, 1, 1, 31, 20);
			}
			break;
		case 3:
		case 13:
			Mercury_ArriveWithSparks();
		case 1:
		case 2:
		case 12:
			BattleFx_SetQueuedSoundAndPlay(170);
			break;
		case 7:
		case 8:
		case 9:
			if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256))
			{
				Object_GetById(0)->y.fixed = -0x20000;
				Object_GetById(0)->targetY = Object_GetById(0)->y.fixed;
				Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
				Map_CopyCellsTo(10, 28, 10, 18, 1, 2);
			}
			if (Engine_GameFlagIsSet(0x878))
			{
				Engine_ActorSetPosition(8, 168 << 16, 188 << 17);
				Object_GetById(8)->y.fixed += -0x20000;
				Object_GetById(8)->targetY = Object_GetById(8)->y.fixed;
				Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
				Map_CopyCellsTo(10, 28, 10, 18, 1, 2);
				Map_CopyCellAttributes(10, 16, 1, 1, 10, 19);
				Engine_MapRedraw();
			}
			break;
		case 16:
			Engine_TaskWait(1);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
			if (Engine_GameFlagIsSet(0x318))
				Actor_SetPosition(10, 204 << 18, 152 << 18);
			if (Engine_GameFlagIsSet(0x31a))
				Actor_SetPosition(11, 194 << 18, 144 << 18);
			else if (Engine_GameFlagIsSet(0x319))
				Actor_SetPosition(11, 190 << 18, 144 << 18);
			else if (Engine_GameFlagIsSet(0x31b))
				Actor_SetPosition(11, 214 << 18, 144 << 18);
#else
			Actor_SetPosition(10, 204 << 18, 152 << 18);
			Engine_ActorSetPosition(11, 194 << 18, 144 << 18);
#endif
			SetEffectRecordMode(Object_GetById(0), 1);
			SceneEffect_SpawnParticleRowsByMode(0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
			if (Engine_GameFlagIsSet(0x319))
			{
				MercuryLighthouse_OpenStoneDoor(2);
				if (Object_GetById(9)->z.fixed >> 20 == 44)
					FieldScheduler_AddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
			}
			else if (Engine_GameFlagIsSet(0x31a))
			{
				MercuryLighthouse_OpenStoneDoor(1);
			}
			else if (!Engine_GameFlagIsSet(0x31b))
			{
				MercuryLighthouse_OpenStoneDoor(0);
			}
#else
			MercuryLighthouse_OpenStoneDoor(1);
#endif
		case 14:
			if (gGameState.entrance == 14)
				Engine_AudioPlayCue(211);
			SetEffectRecordMode(Object_GetById(9), 1);
			Engine_ActorSetSpritePriority(10, 2);
			Object_GetById(10)->terrainId = 2;
			SetEffectRecordMode(Object_GetById(11), 1);
			SetEffectRecordMode(Object_GetById(12), 1);
			SetEffectRecordMode(Object_GetById(13), 1);
			SetEffectRecordMode(Object_GetById(14), 1);
			FieldScene_RedrawActorFootprint(10);
			FieldScene_RedrawActorFootprint(11);
			if (gGameState.entrance != 14)
				break;
			if (!Engine_GameFlagIsSet(0x109))
			{
				Engine_GameFlagClear(0x318);
				Engine_GameFlagClear(0x319);
				Engine_GameFlagClear(0x31a);
				Engine_GameFlagClear(0x31b);
				break;
			}
			SceneEffect_SpawnParticleRowsByMode(0);
			if (Engine_GameFlagIsSet(0x319))
			{
				MercuryLighthouse_OpenStoneDoor(2);
				if (Object_GetById(9)->z.fixed >> 20 == 44)
					FieldScheduler_AddCallback(SceneActor_UseActorNinePositionWithYOffset, 0xc80);
			}
			else if (Engine_GameFlagIsSet(0x31a))
			{
				MercuryLighthouse_OpenStoneDoor(1);
			}
			else if (!Engine_GameFlagIsSet(0x31b))
			{
				MercuryLighthouse_OpenStoneDoor(0);
			}
			break;
		case 15:
			Engine_EventBegin();
			SetEffectRecordMode(Object_GetById(8), 1);
			Engine_ActorSetChildValue(0, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
			Object_GetById(8)->y.fixed = 144 << 16;
			Object_GetById(8)->motionFlags = 0;
			Object_GetById(8)->bounce = 0;
			Object_GetById(8)->gravity = 0x4ccc;
			gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			Object_GetById(8)->motionFlags = 3;
			Engine_AudioPlayCue(189);
			Engine_EventWait(32);
			Engine_AudioPlayCue(188);
			SetEffectRecordMode(Object_GetById(8), 2);
			Work_SetValuesIfNonNegative(192 << 10, 192 << 10, 128 << 9);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			Engine_EventWait(60);
			Engine_EventRequestExit(16);
			Engine_EventEnd();
			break;
		}
	}
	if (gGameState.scene == (s32)&SceneId_MercuryLighthouse4)
	{
		switch (gGameState.entrance)
		{
		case 10:
			FieldScheduler_AddCallback(SceneEffect_RotatePaletteEntries97To103, 0xc80);
			if (Engine_GameFlagIsSet(0x109))
				break;
			Mercury_ArriveWithSparks();
			BattleFx_SetQueuedSoundAndPlay(170);
			ColorBuffer_ApplySource(128 << 9, 0);
			Engine_ColorBufferApplyTarget(0x10003, 1);
			Engine_ColorBufferInterpolate(30);
			Engine_EventWaitForScreen();
			Engine_ActorSetAnimation(0, 1);
			Engine_EventWait(30);
			UiText_ShowCenteredMessage(MsgMercuryHonorsGoddessRainbows, 0, 0);
			Engine_ColorBufferApplyTarget(128 << 9, 0);
			Engine_ColorBufferInterpolate(30);
			break;
		case 15:
			Engine_ActorSetPosition(3, 0, 0);
			DisplayBlend_EnableRunScript();
			Io_Write16(0, &REG_BLDCNT);
			Object_GetById(9)->scaleX = -0x10000;
			Engine_ActorSetSpritePriority(14, 1);
			Engine_ActorSetSpritePriority(15, 1);
			Engine_ActorSetSpritePriority(16, 1);
			set = Engine_GameFlagIsSet(0x109);
			if (set)
			{
				Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
				Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
				break;
			}
			Actor_SetPosition(10, 206 << 18, 150 << 18);
			Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
			Object_GetById(10)->motionFlags = set;
			SetEffectRecordMode(Object_GetById(10), 1);
			Engine_EventBegin();
			Camera_MoveTo(-1, -1, -1, 0);
			Engine_EventGetViewCenter()->motionFlags = set;
			Engine_ActorSetSpritePriority(0, 1);
			Engine_ActorSetSpritePriority(13, 1);
			MercuryLighthouse_RideLift();
			DisplayBlend_DisableRunScript();
			Engine_EventEnd();
			break;
		case 12:
			Engine_EventWait(1);
			SetEffectRecordMode(Object_GetById(0), 1);
		case 11:
			Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
			Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
			if (!Engine_GameFlagIsSet(0x881))
			{
				Map_CopyCellsTo(30, 45, 50, 45, 5, 6);
				Map_CopyCellsTo(50, 105, 50, 109, 5, 3);
				Engine_MapRedraw();
				Engine_TaskWait(1);
			}
			else
			{
				Engine_ActorSetSpritePriority(14, 1);
				Engine_ActorSetSpritePriority(15, 1);
				Engine_ActorSetSpritePriority(16, 1);
			}
			Object_GetById(9)->scaleX = -0x10000;
			if (!Engine_GameFlagIsSet(0x82b))
			{
				Engine_ActorSetPosition(3, 0, 0);
				Actor_SetPosition(10, 206 << 18, 150 << 18);
				Engine_ActorSetSpritePriority(10, 1);
				Map_CopyCellAttributes(116, 37, 3, 3, 52, 37);
				Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
				FieldScheduler_AddCallback(SceneEffect_RotatePaletteEntries40To47, 0xc80);
				break;
			}
			if (!Engine_GameFlagIsSet(0x871))
			{
				if (!Engine_GameFlagIsSet(0x870))
				{
					Engine_ActorFaceDirection(3, 0, 0);
					Engine_ActorSetAnimation(3, 16);
				}
				else
				{
					Actor_SetPosition(3, 210 << 18, 158 << 18);
					Actor_FaceDirection(3, 192 << 8, 0);
				}
				SetEffectRecordMode(Object_GetById(3), 1);
				Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
				FieldScheduler_AddCallback(SceneEffect_RotatePaletteEntries40To47, 0xc80);
			}
			else
			{
				Engine_ActorSetPosition(3, 0, 0);
				Actor_SetPosition(10, 206 << 18, 150 << 18);
				Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
				Object_GetById(10)->collisionFlags = 254;
				Engine_EventWait(1);
			}
			Object_GetById(10)->motionFlags = 0;
			SetEffectRecordMode(Object_GetById(10), 1);
			break;
		}
	}
	return 0;
}

static void EV08_PSYNERGY_1(void)
{
	s32				   base;
	s32				   position;
	s32				   event;
	struct FieldActor *actor;

	position = Actor_Get(8)->x.fixed / 0x100000;
	if (position != 48)
	{
		return;
	}
	Engine_EventBegin();
	base = MsgMercurySavedMeAgain;
	Engine_EventSetMessage(base);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(3, 1);
	Actor_FaceDirection(0, 32768, 20);
	Event_ShowMessageAndWait(3, 0, 20);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(20);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(3, 16);
	Engine_EventWait(50);
	Engine_ActorSetAnimation(3, 1);
	Engine_EventOpenMessage(3, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(3, 2);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(3, 0, 20);
		Engine_ActorSetAnimationAndWait(3, 4);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(3, 0, 20);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(20);
		Engine_EventOpenMessage(3, 0);
		if (Engine_EventChooseYesNo(0, 0) == 1)
		{
			Engine_EventWait(20);
			/* FAKEMATCH: called through a cast to the unprototyped call the bytes show, which loads r1 before r0; called by its prototype, also through the script layer's Actor_SetAnimationAndWait, r0 loads first: differ 2 at each site. */
			((s32 (*)())Engine_ActorSetAnimationAndWait)(3, 4);
			Engine_EventWait(20);
			event = base + 5;
			for (;;)
			{
				Engine_EventSetMessage(event);
				Engine_EventOpenMessage(3, 0);
				if (Engine_EventChooseYesNo(0, 0) != 1)
				{
					break;
				}
				Engine_EventWait(20);
				/* FAKEMATCH: called through a cast to the unprototyped call the bytes show, which loads r1 before r0; called by its prototype, also through the script layer's Actor_SetAnimationAndWait, r0 loads first: differ 2 at each site. */
				((s32 (*)())Engine_ActorSetAnimationAndWait)(3, 4);
				Engine_EventWait(20);
				event = MsgMercuryDontHideTruth;
			}
		}
	}
	Engine_EventSetMessage(MsgMercuryThoughtSo);
	Actor_SetSpeed(3, 52428, 26214);
	Engine_ActorWalkToAndWait(3, 728, 632);
	Engine_EventWait(20);
	Engine_EventShowMessageAndWait(3, 0, 20);
	Engine_ActorSetAnimation(3, 16);
	Engine_EventShowMessageAndWait(3, 0, 20);
	Engine_ActorSetAnimation(3, 1);
	Engine_ActorFaceActor(3, 0, 20);
	Engine_ActorSetAnimationAndWait(3, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(3, 0, 20);
	Actor_ShowEmote(3, 261, 90);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(3, 0, 20);
	Event_PrepareObjectAndApplyValue(3, 1);
	Engine_GameFlagSet(68);
	Djinn_AddToOwner(3, 1, 0);
	Djinn_Activate(3, 1, 0);
	UnitCalc(3);
	Engine_ActorSetAnimation(3, 2);
	actor = Object_GetById(0);
	if (actor != 0)
	{
		Engine_ActorSetDestination(3, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorWaitForMove(3);
	Engine_ActorSetPosition(3, 0, 0);
	Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
	Engine_GameFlagSet(2163);
	Engine_EventEnd();
}

static void MercuryLighthouse_RideLift(void)
{
	struct LiftLayer *layer;
	s32				  speed;

	layer = (struct LiftLayer *)(((u8 *)gMapWork[0]) + 356);
	speed = 0x9c28;
	layer->offset = 0x04890000;
	layer->unk1c = 0;
	Actor_Get(0)->motionFlags = 0;
	Actor_Get(0)->z.fixed += -0x890000;
	Actor_Get(0)->targetZ = Actor_Get(0)->z.fixed;
	Actor_Get(13)->motionFlags = 0;
	Actor_SetPosition(13, 170 << 18, 220 << 17);
	Actor_Get(13)->z.fixed += -0x890000;
	Actor_Get(13)->targetZ = Actor_Get(13)->z.fixed;
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(60);
	DisplayBlend_EnableRunScript();
	Engine_AudioPlayCue(223);
	for (;;)
	{
		layer->offset -= speed;
		Actor_Get(0)->z.fixed += speed;
		Actor_Get(0)->targetZ = Actor_Get(0)->z.fixed;
		Actor_Get(13)->z.fixed += speed;
		Actor_Get(13)->targetZ = Actor_Get(13)->z.fixed;
		if (layer->offset > 0x04000000)
		{
			if ((gFrameCount & 15) == 0 && speed > 0xccb)
				speed += -0x560;
			Engine_TaskWait(1);
			continue;
		}
		break;
	}
	layer->offset = 0x04000000;
	Engine_MapRedraw();
	Engine_TaskWait(2);
	Actor_Get(0)->motionFlags = 3;
	Actor_Get(13)->z.fixed = 220 << 17;
	Actor_Get(13)->targetZ = Actor_Get(13)->z.fixed;
	Engine_EventWait(30);
	Actor_WalkToAndWait(0, 704, 584);
	Actor_Get(0)->bounce = 0;
	Engine_ActorSetAnimation(0, 6);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(0, 7);
	Actor_Get(0)->speed = 0x30000;
	Actor_Get(0)->acceleration = 0x20000;
	Engine_AudioPlayCue(152);
	Actor_Get(0)->velocityY = 0x40000;
	Engine_ActorSetSpriteFlags(Actor_Get(0), 0);
	Actor_SetDestination(0, 736, 584);
	Engine_ActorWaitForMove(0);
	Engine_ActorSetSpriteFlags(Actor_Get(0), 1);
	Actor_Get(0)->bounce = 0x4000;
	Engine_ActorSetAnimation(0, 6);
	Engine_EventWait(6);
	Actor_FaceDirection(0, 0x8000, 30);
	Engine_AudioPlayCue(223);
	for (;;)
	{
		layer->offset += speed;
		Actor_Get(13)->z.fixed -= speed;
		Actor_Get(13)->targetZ = Actor_Get(13)->z.fixed;
		if (layer->offset >= 0x04890000)
			break;
		if ((gFrameCount & 7) == 0 && speed <= 0xcccc)
			speed += 0x1999;
		Engine_TaskWait(1);
	}
	layer->offset = 0x04000000;
	Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
	Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
	Engine_MapRedraw();
	Engine_TaskWait(2);
	Actor_SetPosition(13, 0, 0);
	Engine_EventWait(30);
	Actor_Get(0)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
}

static void MercuryLighthouse_CastPsynergyAtActor11(void)
{
	u8 *p5;
	u8 *addr;
	u8	v;

	p5 = gEffectWork;
	Actor_SetPosition(11, 0x3480000, 0x2580000);
	Engine_PsynergyBegin(93, 1);
	Engine_PsynergySetTarget(3, 11);
	addr = p5 + 0x71c;
	v = *addr | 8;
	*addr = v;
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_PsynergyLowerHands();
}

static void SceneState_SetRecordTableValue(s32 key, s32 value)
{
	s32 slot = PartyInventory_FindOwner(key);

	if (slot != -1)
	{
		s32 index = Inventory_Find(slot, key);

		if (index != -1)
		{
			((struct SceneLookupRecord *)UnitGet(slot))->table[index] = value;
		}
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/mercury_lights.h"

static void OverlayObject_SpawnKind24AtObject(struct FieldActor *src)
{
	struct FieldActor  *obj;
	struct FieldSprite *sprite;

	obj = Engine_ObjectCreate(24, src->x.fixed, src->y.fixed, src->z.fixed);
	if (obj == 0)
	{
		return;
	}

	sprite = obj->sprite;
	Engine_ObjectSetScript(obj, MercuryLighthouse_RiseScript);

	obj->motionFlags = 0;
	obj->terrainId = 1;
	obj->priorityFlags = 2;

	if (sprite != 0)
	{
		AnimationObjects_SelectAnimation(sprite, 2);
		sprite->flags = 0;
		((struct FieldSpritePriority *)sprite)->priority = 3;
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/mercury_pillar.h"

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

s32 MercuryLighthouse_RiseScript[] = {
	SC_WAIT(30),
	SC_FREE,
};

s32 MercuryLighthouse_SparkScript[] = {
	SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
	SC_FREE,
};

s32 Mercury_RampScript[] = {
	SC_CALL(OverlayObject_RampWords24And28Over16Frames),
	SC_END,
};

s32 Mercury_ScaleCounterScript[] = {
	SC_CALL(OverlayObject_AdvanceScaleCounter),
	SC_FREE,
};

s32 Mercury_PillarScript[] = {
	SC_CALL(Mercury_FollowLeader),
	SC_END,
};

u16 Mercury_MoveAngles[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
	FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 MercuryLighthouse_SparkBurstScript[] = {
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0x4000),
	SC_SET(SW_SCALE_X, 0x4000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_LOOP(4, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 MercuryLighthouse_PushScriptA[] = {
	SC_WAIT(45),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MercuryLighthouse_StartActorNineParticles),
	SC_MOVE(PIXELS(792), 0, PIXELS(712)),
	SC_SYNC,
	SC_END,
};

s32 MercuryLighthouse_PushScriptB[] = {
	SC_WAIT(60),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MercuryLighthouse_StartActorNineParticles),
	SC_MOVE(PIXELS(792), 0, PIXELS(744)),
	SC_SYNC,
	SC_CALL(MercuryLighthouse_StopActorNinePositionCallback),
	SC_END,
};

s32 MercuryLighthouse_PushScriptD[] = {
	SC_WAIT(60),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MercuryLighthouse_StartActorNineParticles),
	SC_MOVE(PIXELS(824), 0, PIXELS(744)),
	SC_SYNC,
	SC_CALL(MercuryLighthouse_StopActorNinePositionCallback),
	SC_END,
};

s32 MercuryLighthouse_PushScriptC[] = {
	SC_WAIT(45),
	SC_CALL(MercuryLighthouse_StartActorNineParticles),
	SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

u16 MercuryLighthouse_GateCells[] = {
	MAP_CELL_STEP(15, 1, 1, 2, 10), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(9, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MercuryLighthouse_GateCloseCells[] = {
	MAP_CELL_STEP(9, 1, 1, 2, 10), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(15, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MercuryLighthouse_FloorSwitchCells[] = {
	MAP_CELL_STEP(16, 28, 1, 2, 10), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(10, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MercuryLighthouse_FloorSwitchCloseCells[] = {
	MAP_CELL_STEP(10, 28, 1, 2, 10), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(16, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MercuryLighthouse_ColumnCells[] = {
	MAP_CELL_STEP(126, 35, 1, 2, 2), MAP_CELL_STEP(126, 38, 1, 2, 2), MAP_CELL_STEP(126, 41, 1, 2, 2), MAP_CELL_STEP(126, 44, 1, 2, 2), MAP_CELL_STEP(126, 47, 1, 2, 2), MAP_CELL_STEPS_END,
};

struct SceneEntrance gMercuryLighthouseEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 56, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ 3, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, 16, 16, 496, 448, 0 },
	{ 4, CONDITION_ALWAYS, 760, 0, 344, FACING_NORTH, 0, 544, 16, 816, 352, 0 },
	{ 5, CONDITION_ALWAYS, 120, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ 6, CONDITION_ALWAYS, 344, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ 8, CONDITION_ALWAYS, 280, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ 9, CONDITION_ALWAYS, 776, 0, 472, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ 10, CONDITION_ALWAYS, 856, 0, 520, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ 11, CONDITION_ALWAYS, 472, 0, 696, FACING_NORTH, 0, 352, 512, 592, 720, 0 },
	{ 15, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 544, 16, 816, 352, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 472, FACING_SOUTH, 0, 32, 400, 480, 848, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 824, FACING_NORTH, 0, 32, 400, 480, 848, 0 },
	{ 3, CONDITION_ALWAYS, 152, 0, 184, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ 4, CONDITION_ALWAYS, 488, 0, 136, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ 5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ 6, CONDITION_ALWAYS, 728, 0, 216, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ 7, CONDITION_ALWAYS, 632, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ 8, CONDITION_ALWAYS, 792, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ 9, CONDITION_ALWAYS, 712, 0, 568, FACING_SOUTH, 0, 576, 480, 832, 736, 0 },
	{ 10, CONDITION_ALWAYS, 888, 0, 440, FACING_NORTH, 0, 760, 304, 1040, 464, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ 2, CONDITION_ALWAYS, 504, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ 3, CONDITION_ALWAYS, 264, 0, 120, FACING_SOUTH, 0, 32, 32, 544, 208, 0 },
	{ 4, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
	{ 5, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ 6, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ 7, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 8, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 9, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 10, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
	{ 11, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
	{ 12, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ 13, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ 14, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ 15, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
	{ 16, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseEntrances4[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
	{ 2, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
	{ 4, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
	{ 10, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
	{ 11, CONDITION_ALWAYS, 840, 0, 856, FACING_NORTH, 0, 576, 328, 960, 912, 0 },
	{ 12, CONDITION_ALWAYS, 840, 0, 600, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ 13, CONDITION_ALWAYS, 872, 0, 600, FACING_SOUTH, 0, 720, 544, 960, 912, 0 },
	{ 15, CONDITION_ALWAYS, 680, 0, 584, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ SCENE_TABLE_END },
};

u32 MercuryLighthouse_SceneTable[] = {
	SCENE_EXITS(54),
	SCENE_EXIT(1, 53, 11),
	SCENE_EXIT(2, 53, 12),
	SCENE_EXIT(3, 54, 9),
	SCENE_EXIT(4, 53, 15),
	SCENE_EXIT(5, 53, 14),
	SCENE_EXIT(6, 56, 7),
	SCENE_EXIT(7, 55, 3),
	SCENE_EXIT(8, 53, 6),
	SCENE_EXIT(9, 54, 3),
	SCENE_EXIT(10, 54, 5),
	SCENE_EXIT(11, 54, 10),
	SCENE_EXIT(12, 55, 1),
	SCENE_EXIT(13, 53, 25),
	SCENE_EXIT(0, 55, 0),
	SCENE_EXIT(1, 54, 11),
	SCENE_EXIT(2, 55, 2),
	SCENE_EXIT(3, 56, 1),
	SCENE_EXIT(4, 57, 1),
	SCENE_EXIT(5, 56, 3),
	SCENE_EXIT(6, 54, 6),
	SCENE_EXIT(7, 55, 4),
	SCENE_EXIT(8, 56, 5),
	SCENE_EXIT(9, 56, 6),
	SCENE_EXIT(10, 56, 2),
	SCENE_EXIT(0, 56, 0),
	SCENE_EXIT(1, 55, 5),
	SCENE_EXIT(2, 55, 9),
	SCENE_EXIT(3, 55, 10),
	SCENE_EXIT(4, 56, 8),
	SCENE_EXIT(5, 55, 7),
	SCENE_EXIT(6, 55, 8),
	SCENE_EXIT(7, 54, 8),
	SCENE_EXIT(8, 56, 14),
	SCENE_EXIT(9, 56, 4),
	SCENE_EXIT(10, 57, 3),
	SCENE_EXIT(11, 56, 13),
	SCENE_EXIT(12, 57, 2),
	SCENE_EXIT(13, 56, 11),
	SCENE_EXIT(14, 56, 9),
	SCENE_EXIT(15, 56, 15),
	SCENE_EXIT(16, 56, 16),
	SCENE_EXIT(0, 57, 0),
	SCENE_EXIT(1, 55, 6),
	SCENE_EXIT(2, 56, 12),
	SCENE_EXIT(3, 56, 10),
	SCENE_EXIT(4, 57, 10),
	SCENE_EXIT(5, 57, 4),
	SCENE_EXIT(6, 2, 10),
	SCENE_EXIT(7, 53, 1),
	SCENE_EXIT(8, 2, 90),
	SCENE_EXIT(9, 58, 5),
	SCENE_EXITS_END,
};

struct ScenePlacement gMercuryLighthousePlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthousePlacements1[] = {
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(672), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(704), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(744), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(648), 0, PIXELS(299), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthousePlacements2[] = {
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(152), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(184), 0, PIXELS(587), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(704), 0, PIXELS(672), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5b, 0x45, ACTOR_STAND,
		PIXELS(888), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthousePlacements3[] = {
	{ 0xe0, IF_FLAG_SET(0x877), ACTOR_STAND,
		PIXELS(200), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, 0x877, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(816), 0, PIXELS(544), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(872), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(760), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(776), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(872), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(776), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(936), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(472), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthousePlacements4[] = {
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(664), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(888), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(824), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, IF_FLAG_SET(0x881), ACTOR_WANDER,
		PIXELS(760), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, IF_FLAG_SET(0x881), ACTOR_WANDER,
		PIXELS(920), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(856), 0, PIXELS(792), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gMercuryLighthouseEvents1[] = {
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ 0x21, 53, CONDITION_ALWAYS, 3 },
	{ 0x31, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 62, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ 0x31, 55, CONDITION_ALWAYS, 5 },
	{ 0x21, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_TOUCH, 32, IF_FLAG_SET(0x875), EVENT_SCRIPT(MercuryLighthouse_ClearSceneRecordAndTarget) },
	{ 0x202, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(MercuryLighthouse_ClearSceneRecordAndTarget) },
	{ 0xa02, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(EV21_TOUCH) },
	{ 0x202, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(EV11_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ 0x202, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SCRIPT) },
	{ EVENT_TALK, 3, 0x873, MSGNO(MsgMercuryLighthouseThePathIsBlockedAgainWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, 0x873, MSGNO(MsgMercuryLighthouseThisStatueWasntHereBeforeI) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x873, EVENT_SCRIPT(EV08_PSYNERGY_1) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf64, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseEvents2[] = {
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 53, CONDITION_ALWAYS, 3 },
	{ 0x21, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ 0x202, 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RunSequenceE) },
	{ EVENT_TOUCH, 2, IF_FLAG_SET(0x302), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
	{ EVENT_TOUCH, 3, IF_FLAG_SET(0x302), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
	{ EVENT_TOUCH, 4, IF_FLAG_SET(0x302), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
	{ 0x202, 10, 0x874, EVENT_SCRIPT(EV10_SCRIPT) },
	{ EVENT_TOUCH, 12, 0x874, EVENT_SCRIPT(EV12_TOUCH_2) },
	{ EVENT_TOUCH, 10, 0x874, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
	{ EVENT_TOUCH, 13, 0x874, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
	{ 0x202, 45, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RunSequenceE) },
	{ EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x874, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf66, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 102, 0xf67, FIND_ITEM(ITEM_NUT) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseEvents3[] = {
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_TOUCH) },
	{ 0x31, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ 0x21, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ 0x21, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 62, CONDITION_ALWAYS, 12 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 63, CONDITION_ALWAYS, EVENT_SCRIPT(EV63_TOUCH) },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ 0x31, 64, CONDITION_ALWAYS, 14 },
	{ 0x202, 10, 0x878, EVENT_SCRIPT(FieldScene_RunFourSteps) },
	{ 0x202, 12, 0x878, EVENT_SCRIPT(FieldScene_RunFourSteps) },
	{ EVENT_TOUCH, 12, 0x878, EVENT_SCRIPT(EV12_TOUCH_3) },
	{ EVENT_TOUCH, 10, 0x878, EVENT_SCRIPT(EV10_TOUCH) },
	{ 0x202, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_SCRIPT) },
	{ 0x202, 47, CONDITION_ALWAYS, EVENT_SCRIPT(EV47_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x878, EVENT_SCRIPT(EV08_PSYNERGY_3) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 16, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 18, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseEventsOther[] = {
	{ EVENT_EXIT, 44, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 43, CONDITION_ALWAYS, 6 },
	{ 0x31, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ 0x31, 53, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(EV55_TOUCH) },
	{ EVENT_TALK, 3, 0x870, EVENT_SCRIPT(FieldScene_RunActorThreeBranchSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x403, 0x870, EVENT_SCRIPT(FieldScene_RunActorThreeBranchSequence) },
	{ EVENT_TALK, 3, 0x871, EVENT_SCRIPT(EV03_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, 0x871, MSGNO(MsgMercuryLighthouseSomeoneDoesntWantMeToGet) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseTheLegendsSaidThatIfThe) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseOurFountainIsBackTheySay) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseIDrankTheHealingWaterAnd) },
	{ EVENT_TALK, 17, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseWeDontHaveToFearAny) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseHowDidTheFountainKnowThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseAllThatLivesThatMeansIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseIKnewItWouldWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, IF_FLAG_SET(0x881), MSGNO(MsgMercuryLighthouseINeverBelievedThoseStoriesBut) },
	{ SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(EV42_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 5, 0x200, EVENT_SCRIPT(EV05_SEARCH) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x82b, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x871, EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x5d), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ 0xb904, 5, IF_FLAG_SET(0x881), EVENT_SCRIPT(EV05_SCRIPT) },
	{ SCENE_EVENTS_END },
};
