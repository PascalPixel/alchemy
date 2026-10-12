#include "game_state.h"
#include "unit/scene/suhara_sabaku.h"
#include "unit/scene/common/effect/move.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/game/flag.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_scene.h"
#include "global_cells.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"

// 前方宣言
static void SuharaSabaku_ApplyVisitFlagBlend(void);
static void SuharaSabaku_ApplyAltarFlagBlend(void);

struct SelectedActorId
{
	u8	reserved000[500];
	s32 actor_id;
};

struct SelectedActorFacing
{
	u8	reserved00[6];
	u16 presentation;
};

struct StateMarkerWork
{
	u8	reserved000[386];
	u16 state_one_marker;
};

static u16 gSuharaSabakuShownLevel;

#define ACTOR_ID 13

#define EncounterPalette (*(volatile u16 *)0x0500019e)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect/create.c"
#include "common/effect/move.c"
#include "common/effect/spawn.c"

// 戦闘開始の種類 (battle/party/party.c と同じ)

struct FieldGlobals
{
	u8				 *map;
	u8				  unknown_04[0x48];
	struct EventWork *event;
};

void QueueSceneSound(s32 cue);

static void EV15_TALK(void)
{
	BattleFx_SetPhaseRequest(15, 45);
}

static s32 SuharaSabaku_ClearActorSpriteAndCollisionFlags(u8 *obj)
{
	Engine_ActorSetSpriteFlags(obj, 0);
	((struct FieldActor *)obj)->collision_flags = 0;
	return 0;
}

static s32 SuharaSabaku_ToggleActorActive(u8 *obj)
{
	((struct FieldActor *)obj)->active ^= 1;
	return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_SuharaSabaku1)
	{
		return gSuharaSabakuEntrances1;
	}
	if (selector == (s32)&SceneId_SuharaSabaku2)
	{
		return gSuharaSabakuEntrances2;
	}
	if (selector == (s32)&SceneId_SuharaSabaku3)
	{
		return gSuharaSabakuEntrances3;
	}
	return gSuharaSabakuEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SuharaSabaku_GetExits(void)
{
	return gSuharaSabakuExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_SuharaSabaku1)
	{
		return gSuharaSabakuPlacements1;
	}
	if (selector == (s32)&SceneId_SuharaSabaku2)
	{
		return gSuharaSabakuPlacements2;
	}
	if (selector == (s32)&SceneId_SuharaSabaku3)
	{
		return gSuharaSabakuPlacements3;
	}
	return gSuharaSabakuPlacementsOther;
}

static void SuharaSabaku_SyncSelectedActorProgress(void)
{
	struct SelectedActorFacing	  *actor;
	struct StateMarkerWork *scene;
	s32						   progress;

	actor = Object_GetById(((struct SelectedActorId *)((s16 *)&gGameState))->actor_id);
	scene = *(struct StateMarkerWork **)((u8 *)&gEventWork);
	actor->presentation = (gFrameCount << 12);

	progress = GameFlag_GetByte(0x210);
	if (progress != 0)
	{
		if (progress == 1)
		{
			scene->state_one_marker = 99;
		}
		else if (Engine_GameFlagIsSet(0x106) == 0)
		{
			progress -= 1;
		}
	}
	GameFlag_SetByte(0x210, progress);
}

static void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
	s32				   p10;
	s32				   rec2;
	struct FieldActor *rec7;
	struct FieldActor *record;
	u8				  *p6;
	u8				  *base;

	base = (u8 *)((s16 *)&gGameState);
	p6 = *(u8 **)(base + 500);
	p10 = a0;
	rec7 = Object_GetById((s32)p6);
	Actor_Get(p10);
	rec2 = GameFlag_IsSet(0x20f);
	if (rec2 == 0)
	{
		Engine_EventBegin();
		Actor_SetAttachedEffect((s32)p6, 0x101);
		Engine_ActorSetAnimation((s32)p6, 9);
		record = Object_GetById(p10);
		if (record != 0)
		{
			Actor_SetDestination((s32)p6, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove((s32)p6);
		Event_PlayCue(244);
		Engine_TaskAddCallback((s32)SuharaSabaku_SyncSelectedActorProgress, 0xc80);
		rec7->motion_flags = rec2;
		Engine_ObjectSetPosition((s32)rec7, rec7->x.fixed, rec7->y.fixed + 0x200000, rec7->z.fixed);
		Engine_ActorWaitForMove((s32)p6);
		rec7->velocity_y = rec2;
		rec7->motion_flags = 4;
		*(base + 498) = 2;
		GameFlag_Set(0x20f);
		GameFlag_SetByte(0x218, p10);
		GameFlag_SetByte(0x210, 180);
		Engine_EventEnd();
		*(u16 *)(*(u8 **)((u8 *)&gEventWork) + 0x17c) = rec2;
	}
}

static void FieldScene_RunActor8Step(void)
{
	FieldScene_RunMiddleAuxiliarySequence(8);
}

static void FieldScene_RunActor9Step(void)
{
	FieldScene_RunMiddleAuxiliarySequence(9);
}

static void FieldScene_RunActor10Step(void)
{
	FieldScene_RunMiddleAuxiliarySequence(10);
}

static void FieldScene_RunActor11Step(void)
{
	FieldScene_RunMiddleAuxiliarySequence(11);
}

static void FieldScene_RunActor12Step(void)
{
	FieldScene_RunMiddleAuxiliarySequence(12);
}

static void SuharaSabaku_RestoreActorScaleAndBlend(s32 a0)
{
	s32				   i;
	s32				   next_frame;
	s32				   p10;
	s32				   p9;
	s32				   p11;
	struct FieldActor *rec;
	s32				   rec2;
	struct FieldActor *rec7;
	struct FieldActor *record;

	p10 = a0;
	rec7 = Object_GetById(gGameState.selected_actor);
	rec = Object_GetById(p10);
	rec2 = Engine_GameFlagIsSet(0x340);
	p9 = rec7->x.part.pixel;
	p11 = rec7->z.part.pixel;
	Engine_EventBegin();
	Engine_AudioPlayCue(244);
	i = 0;
	do
	{
		rec->scale_x = ((i << 11) + 0x800);
		rec->scale_y = ((i << 12) + 0x1000);
		if (rec2 == 0)
		{
			next_frame = i + 1;
			IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - next_frame) << 8) | next_frame, 0x20000);
		}
		else
		{
			next_frame = i + 1;
		}
		Engine_TaskWait(1);
		i = next_frame;
	} while (i <= 15);
	Engine_GameFlagSet((0x1fe + p10));
	Engine_GameFlagSet(0x340);
	if (Engine_GameFlagIsSet(0x9a0) == 0)
	{
	}
	else
	{
		if (GameFlag_IsSet(0x9b6) != 0)
		{
		}
		else
		{
			Engine_GameFlagSet(0x9b6);
			Actor_SetSpeed(13, 0x10000, 0x8000);
			Actor_SetSpeed(0, 0x18000, 0xc000);
			record = Object_GetById(0);
			if ((s32)record != 0)
			{
				Engine_ActorSetPosition(13, record->x.fixed, record->z.fixed);
			}
			Actor_FaceActor(13, 0x4000, 0);
			Object_GetById(13)->steering_flags &= ~ACTOR_FACES_MOTION;
			Engine_ActorWalkTo(13, p9, (p11 - 16));
			Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
			Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
			Engine_EventWait(1);
			Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
			Engine_ActorSetAnimation(13, 1);
			Engine_ActorFaceDirection(0, 0x4000, 0);
			Engine_EventSetMessage((s32)MsgSuharaSandstorm);
			Actor_ShowEmote(13, 0x106, 60);
			Engine_EventShowMessage(13, 0);
			Actor_ShowEmote(13, 0x102, 60);
			Engine_EventShowMessage(13, 0);
			Actor_FaceDirection(13, 0x2000, 0);
			Actor_ShowEmote(13, 0x101, 60);
			Engine_EventShowMessage(13, 0);
			Engine_EventWait(10);
			Actor_FaceDirection(13, 0xc000, 30);
			Engine_ActorRunRepeatedMotion(13, 2);
			Engine_EventShowMessage(13, 0);
			Engine_ActorSetAnimationAndWait(13, 3);
			Engine_EventShowMessage(13, 0);
			Actor_SetSpeed(13, 0x10000, 0x8000);
			Engine_ActorSetAnimation(13, 2);
			record = Object_GetById(0);
			if ((s32)record != 0)
			{
				Engine_ActorSetDestination(13, record->x.part.pixel, record->z.part.pixel);
			}
			Engine_ActorWaitForMove(13);
			Engine_ActorSetPosition(13, 0, 0);
		}
	}
	Engine_EventEnd();
}

static void FieldScene_RunLateActor8Step(void)
{
	SuharaSabaku_RestoreActorScaleAndBlend(8);
}

static void FieldScene_RunLateActor9Step(void)
{
	SuharaSabaku_RestoreActorScaleAndBlend(9);
}

static void FieldScene_RunLateActor10Step(void)
{
	SuharaSabaku_RestoreActorScaleAndBlend(10);
}

static void EV64_TOUCH(void)
{
	SuharaSabaku_RestoreActorScaleAndBlend(11);
}

static void EV65_TOUCH(void)
{
	SuharaSabaku_RestoreActorScaleAndBlend(12);
}

static void EV09_TOUCH(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x9a0) == 0)
	{
	}
	else
	{
		if (GameFlag_IsSet(0x1b7) != 0)
		{
		}
		else
		{
			if (GameFlag_IsSet(0x9b0) == 0)
			{
			}
			else
			{
				GameFlag_Set(0x9b5);
				Engine_EventBegin();
				Engine_EventSetMessage((s32)MsgSuharaNowhereFound);
				record = Actor_Get(ACTOR_PARTY_LEADER);
				if (record != 0)
				{
					Actor_SetPosition(ACTOR_ID, record->x.fixed, record->z.fixed);
				}
				Actor_FaceActor(ACTOR_ID, 0xc000, 0);
				Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 0x4e8);
				Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
				Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1bc, 0x4d8);
				Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 40);
				Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
				Engine_ActorSetAnimationAndWait(ACTOR_ID, 4);
				Event_ShowMessage(ACTOR_ID, 0);
				Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
				Actor_ShowEmote(ACTOR_ID, 0x105, 60);
				Event_ShowMessage(ACTOR_ID, 0);
				Engine_EventWait(30);
				Engine_ActorRunRepeatedMotion(ACTOR_ID, 2);
				Event_ShowMessage(ACTOR_ID, 0);
				Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
				Event_AskYesNo(ACTOR_ID, 0);
				Engine_EventWait(30);
				Actor_ShowEmote(ACTOR_ID, 0x106, 60);
				Event_ShowMessage(ACTOR_ID, 0);
				Engine_ActorSetAnimationAndWait(ACTOR_ID, 3);
				Event_ShowMessage(ACTOR_ID, 0);
				Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
				Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
				Event_ShowMessage(ACTOR_ID, 0);
				Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
				Engine_ActorSetAnimation(ACTOR_ID, 2);
				record = Actor_Get(ACTOR_PARTY_LEADER);
				if (record != 0)
				{
					Actor_SetDestination(ACTOR_ID, record->x.part.pixel, record->z.part.pixel);
				}
				Engine_ActorWaitForMove(ACTOR_ID);
				Actor_SetPosition(ACTOR_ID, 0, 0);
				Engine_EventEnd();
			}
		}
	}
}

static s32 SuharaSabaku_FollowLeaderWithSparks(struct FieldActor *actor)
{
	struct EffectOptions options;
	struct FieldActor	*leader;
	s32					 dx;
	s32					 dz;
	s32					 phase;

	leader = Object_GetById(gGameState.selected_actor);
	actor->acceleration = 0x4000;
	actor->speed = 0x18000;
	actor->motion_flags = 0;
	Engine_ActorSetSpriteFlags(actor, 0);
	actor->active ^= 1;
	if (Engine_GameFlagIsSet(0x104))
	{
		actor->target_x = ACTOR_NO_TARGET;
		actor->target_y = ACTOR_NO_TARGET;
		actor->target_z = ACTOR_NO_TARGET;
	}
	else
	{
		actor->target_x = leader->x.fixed;
		actor->target_y = leader->terrain_height;
		actor->target_z = leader->z.fixed;
		dx = actor->x.fixed - leader->x.fixed;
		if (dx < 0)
		{
			dx = leader->x.fixed - actor->x.fixed;
		}
		dz = actor->z.fixed - leader->z.fixed;
		if (dx + (dz >= 0 ? actor->z.fixed - leader->z.fixed : leader->z.fixed - actor->z.fixed)
			< 0x80000)
		{
			struct EventWork *work = gEventWork;

			if (leader->motion_flags != 0)
			{
				work->raised_trigger = 55;
			}
			actor->motion_flags = 3;
			actor->target_x = leader->x.fixed;
			actor->target_y = leader->y.fixed;
			actor->target_z = leader->z.fixed;
		}
	}
	phase = gFrameCount & 7;
	if (phase == 0)
	{
		options.start_scale_x = 0xcccc;
		options.start_scale_y = 0xcccc;
		options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
		Effect_Spawn(
			actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase, 0x880001, &options);
	}
	return 1;
}

static void EV81_TOUCH(void)
{
	if (GameFlag_IsSet(2487) == 0)
	{
		GameFlag_Set(526);
		Actor_SetPosition(12, 240 << 15, 206 << 18);
		Engine_ActorEnableActionCallback(12, gSuharaSabakuActor12Action);
	}
}

static void SuharaSabaku_EnableEffectChildOption(void)
{
	u8 *state = gEffectWork;
	((struct BattleFxScene *)state)->child_option = 1;
}

static void SuharaSabaku_MeetActor(s32 a0, s32 actor)
{
	struct GameState *state;

	if (gEventWork->raised_trigger == 99)
	{
		gEventWork->raised_trigger = 0;
	}
	Engine_GameFlagClear(0x20f);
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku1)
	{
		Engine_GameFlagSet((actor + 0x2f9));
	}
	else
	{
		if (gGameState.scene == (s32)&SceneId_SuharaSabaku2)
		{
			Engine_GameFlagSet((actor + 0x309));
		}
	}
	GameFlag_SetByte(0x210, 0);
	BattleFx_SetWeightedResult(98, 5);
	Battle_SetStart(3);
	state = &gGameState;
	if (state->scene == (s32)&SceneId_SuharaSabaku2)
	{
		if (actor == 11)
		{
			BattleFx_SetWeightedResult(98, 7);
		}
		else
		{
			if (actor == 12)
			{
				BattleFx_SetWeightedResult(98, 6);
				Engine_ActorStop(12);
				Engine_ActorSetPosition(12, 0, 0);
			}
		}
	}
	Object_GetById(gGameState.selected_actor)->motion_flags = 3;
}

static void EV99_RAISED(void)
{
	s32				   other;
	struct FieldActor *leader;
	struct FieldActor *partner;
	s32				   i;

	other = GameFlag_GetByte(0x218);
	leader = Object_GetById(gGameState.selected_actor);
	partner = Object_GetById(other);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_AudioPlayCue(219);
	Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selected_actor, 0);
	partner->motion_flags = 0;
	leader->motion_flags = 0;
	leader->velocity_y = 0;
	leader->unknown_5d[4] = 1;
	partner->unknown_5d[4] = 1;
	for (i = 0; i < 60; i++)
	{
		leader->velocity_y += 0x3333;
		partner->velocity_y += 0x3333;
		Engine_TaskWait(1);
	}
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
	Engine_GameFlagSet(0x122);
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku2 && GameFlag_GetByte(0x218) == 11)
	{
		Event_SetDestination((s32)&SceneId_WorldMap, 77);
	}
	else
	{
		Event_SetDestination((s32)&SceneId_WorldMap, 27);
	}
}

static void EncounterPalette_Pulse(void)
{
	u16 phase = gFrameCount & 63;
	s32 level;

	if (phase > 31)
		phase = 64 - phase;
	level = (phase >> 1) + 7;
	level |= (level << 10) | (level << 5);
	EncounterPalette = ((u32)level << 16) >> 16;
}

static s32 FieldScene_RunOpeningAuxiliarySequence(void)
{
	s32 record;

	if (gGameState.scene == (s32)&SceneId_SuharaSabaku2)
	{
		Object_GetById(14)->priority_flags = 2;
		Object_GetById(14)->motion_flags = 3;
		Actor_SetPosition(14, 0, 0);
		Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
		MapObject_SetPosition(100, 0, 0);
		Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
		Map_CopyCellAttributes(11, 71, 1, 1, 12, 71);
		record = Engine_TaskRemoveCallback(EncounterPalette_Pulse);
		Io_Write16(gSuharaSabakuShownLevel, &EncounterPalette);
		return record;
	}
}

static s32 FieldScene_RunScene3c0SequenceA(void)
{
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku2)
	{
		Object_GetById(14)->priority_flags = 2;
		Object_GetById(14)->motion_flags = 0;
		Actor_SetPosition(14, 0xf80000, 0x2c80000);
		Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
		MapObject_SetPosition(100, -1, -1);
		BattleFx_EmitRandomParticle();
		Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
		return Engine_TaskAddCallback(EncounterPalette_Pulse, 0xc80);
	}
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku3)
	{
		return gSuharaSabakuEvents3;
	}
	return gSuharaSabakuEventsOther;
}

s32 SuharaSabaku_RunSceneScript(void)
{
	u8 *map;

	map = (*(struct FieldGlobals *)gMapWork).map;
	(*(struct FieldGlobals *)gMapWork).event->start_transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	if (GameFlag_GetByte(0x210) != 0)
	{
		gGameState.movement_mode = 2;
		Engine_TaskAddCallback(SuharaSabaku_SyncSelectedActorProgress, 0xc80);
	}
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku1
		|| gGameState.scene == (s32)&SceneId_SuharaSabaku2)
	{
		gSuharaSabakuShownLevel = EncounterPalette;
		FieldScene_RunOpeningAuxiliarySequence();
	}
	if (gGameState.scene == (s32)&SceneId_SuharaSabaku1)
	{
		SuharaSabaku_ApplyVisitFlagBlend();
	}
	else if (gGameState.scene == (s32)&SceneId_SuharaSabaku2)
	{
		SuharaSabaku_ApplyAltarFlagBlend();
	}
	else
	{
		Engine_AudioPlayCue(0x120);
	}
	if (gGameState.entrance == 0)
	{
		*(u16 *)(map + 20) &= ~0x200;
	}
	return 0;
}

static void SuharaSabaku_ApplyVisitFlagBlend(void)
{
	struct FieldActor *actor;
	s32				   id;
	s32				   alpha = 0;

	if (Engine_GameFlagIsSet(0x301))
	{
		Engine_GameFlagSet(0x206);
	}
	if (Engine_GameFlagIsSet(0x302))
	{
		Engine_GameFlagSet(0x207);
	}
	if (Engine_GameFlagIsSet(0x303))
	{
		Engine_GameFlagSet(0x208);
	}
	if (Engine_GameFlagIsSet(0x304))
	{
		Engine_GameFlagSet(0x209);
	}
	if (Engine_GameFlagIsSet(0x305))
	{
		Engine_GameFlagSet(0x20a);
	}
	for (id = 8; id <= 12; id++)
	{
		actor = Object_GetById(id);
		if (actor != 0)
		{
			if (!Engine_GameFlagIsSet(0x109))
			{
				actor->scale_x = 0x800;
				actor->scale_y = 0x800;
			}
			actor->sprite->flags = 0;
		}
	}
	IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
	if (Engine_GameFlagIsSet(0x340))
	{
		alpha = 16;
		QueueSceneSound(244);
	}
	IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
}

static void SuharaSabaku_ApplyAltarFlagBlend(void)
{
	struct FieldActor  *actor;
	struct FieldSprite *sprite;
	s32					id;
	s32					alpha = 0;

	if (Engine_GameFlagIsSet(0x311))
	{
		Engine_GameFlagSet(0x206);
	}
	if (Engine_GameFlagIsSet(0x312))
	{
		Engine_GameFlagSet(0x207);
	}
	if (Engine_GameFlagIsSet(0x313))
	{
		Engine_GameFlagSet(0x208);
	}
	for (id = 8; id <= 10; id++)
	{
		actor = Object_GetById(id);
		if (actor != 0)
		{
			if (!Engine_GameFlagIsSet(0x109))
			{
				actor->scale_x = 0x800;
				actor->scale_y = 0x800;
			}
			sprite = actor->sprite;
			sprite->flags = 0;
		}
	}
	actor = Object_GetById(11);
	if (actor != 0)
	{
		u8 *part;

		sprite = actor->sprite;
		part = *(u8 **)((u8 *)sprite + 40);
		if (part != 0)
		{
			part[5] = 10;
		}
		sprite->unknown_20[5] = 1;
		sprite->flags = 0;
	}
	if (Engine_GameFlagIsSet(0x315))
	{
		Engine_GameFlagSet(0x9b7);
	}
	IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
	if (Engine_GameFlagIsSet(0x340))
	{
		alpha = 16;
		QueueSceneSound(244);
	}
	IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
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

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 gSuharaSabakuPlacements1_Data[] = {
	SC_CALL(SuharaSabaku_ClearActorSpriteAndCollisionFlags),
	SC_CALL(SuharaSabaku_ToggleActorActive),
	SC_END,
};

s32 gSuharaSabakuPlacements2_1[] = {
	SC_CALL(SuharaSabaku_ClearActorSpriteAndCollisionFlags),
	SC_CALL(SuharaSabaku_ToggleActorActive),
	SC_END,
};

struct SceneEntrance gSuharaSabakuEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 264, 0, 56, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 936, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 488, 0, 456, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 488, 0, 1208, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 440, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 56, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 72, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 440, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 632, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gSuharaSabakuExits[] = {
	SCENE_EXITS(164),
	SCENE_EXIT(1, 2, 28),
	SCENE_EXIT(2, 165, 1),
	SCENE_EXIT(0, 165, 0),
	SCENE_EXIT(1, 164, 2),
	SCENE_EXIT(2, 2, 55),
	SCENE_EXIT(3, 166, 4),
	SCENE_EXIT(4, 166, 1),
	SCENE_EXIT(0, 166, 0),
	SCENE_EXIT(1, 165, 4),
	SCENE_EXIT(2, 166, 3),
	SCENE_EXIT(3, 166, 2),
	SCENE_EXIT(4, 165, 3),
	SCENE_EXITS_END,
};

struct ScenePlacement gSuharaSabakuPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements1[] = {
	{ 0x121, 0x301, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x302, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x303, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x304, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x305, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements2[] = {
	{ 0x121, 0x311, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x312, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x313, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x314, (s32)gSuharaSabakuPlacements2_1,
		PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x315, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5c, 0x5d, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements3[] = {
	{ SCENE_TABLE_END },
};

s32 gSuharaSabakuActor12Action[] = {
	SC_CALL(SuharaSabaku_FollowLeaderWithSparks),
	SC_END,
};

struct SceneEvent gSuharaSabakuEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !EDITION_INTERNATIONAL
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
	{ 0xc401, 4, CONDITION_ALWAYS, 4 },
#endif
	{ EVENT_TOUCH, 9, 0x9b5, EVENT_SCRIPT(EV09_TOUCH) },
	{ EVENT_TOUCH, 41, 0x301, EVENT_SCRIPT(FieldScene_RunActor8Step) },
	{ EVENT_TOUCH, 42, 0x302, EVENT_SCRIPT(FieldScene_RunActor9Step) },
	{ EVENT_TOUCH, 43, 0x303, EVENT_SCRIPT(FieldScene_RunActor10Step) },
	{ EVENT_TOUCH, 44, 0x304, EVENT_SCRIPT(FieldScene_RunActor11Step) },
	{ EVENT_TOUCH, 45, 0x305, EVENT_SCRIPT(FieldScene_RunActor12Step) },
	{ EVENT_TOUCH, 51, 0x311, EVENT_SCRIPT(FieldScene_RunActor8Step) },
	{ EVENT_TOUCH, 52, 0x312, EVENT_SCRIPT(FieldScene_RunActor9Step) },
	{ EVENT_TOUCH, 53, 0x313, EVENT_SCRIPT(FieldScene_RunActor10Step) },
	{ EVENT_TOUCH, 61, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
	{ EVENT_TOUCH, 62, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
	{ EVENT_TOUCH, 63, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
	{ EVENT_TOUCH, 64, 0x209, EVENT_SCRIPT(EV64_TOUCH) },
	{ EVENT_TOUCH, 65, 0x20a, EVENT_SCRIPT(EV65_TOUCH) },
	{ EVENT_TOUCH, 71, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
	{ EVENT_TOUCH, 72, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
	{ EVENT_TOUCH, 73, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
	{ EVENT_TOUCH, 54, 0x314, EVENT_SCRIPT(FieldScene_RunActor11Step) },
	{ EVENT_TOUCH, 81, 0x20e, EVENT_SCRIPT(EV81_TOUCH) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x351, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 103, 0xfb1, FIND_ITEM(0x8d) },
	{ SEARCH(SEARCH_CHEST), 104, 0xfb2, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfb3, FIND_ITEM(0xc0) },
	{ SEARCH(SEARCH_CHEST), 102, 0xef6, 0x500006 },
	{ 0x10002115, 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ PSYNERGY_ON_ACTOR(0x21), 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c0SequenceA) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
	{ EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor12Step) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaSabakuEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SCENE_EVENTS_END },
};
