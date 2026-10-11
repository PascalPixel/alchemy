#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/field/world_map/map.h"
#include "unit/field/world_map/linked_effects.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
#include "unit/game/character/chara.h"
#include "unit/graphics/window/measure.h"
#include "unit/system/scheduler/callback.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "map_render_work.h"
#include "field_service.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "resource.h"
#include "mem.h"
#include "field_effect.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "event_runtime.h"
#include "ability_ids.h"
#include "objcmd.h"

// 戦闘開始の種類 (battle/party/party.c と同じ)

#define SCENE_PHASE (gEventWork->start_transition)
#define ACTOR 8

struct Object
{
	u8	filler00[8];
	s32 x;
	u8	filler0C[4];
	s32 z;
};

struct StoryDialogueWork
{
	u8	reserved000[370];
	u16 story_result;
};

struct StorySharedState
{
	u8 reserved00[52];
	u8 active;
};

struct StoryVerticalEffectActor
{
	u8								 filler00[8];
	s32								 x;
	s32								 y;
	s32								 z;
	u8								 filler14[4];
	s32								 amplitude_x;
	s32								 amplitude_y;
	u8								 filler20[0x44];
	u16								 frame;
	u8								 filler66[2];
	struct StoryVerticalEffectActor *anchor;
};

extern struct StorySharedState *gEffectWork;

enum
{
	CONTACT_LAST_ACTOR = 65,
	CONTACT_TRIGGER_BASE = 100,
	FLAG_CONTACT_PAUSED = 0x163,
	FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 gDebugMode[];

void StoryProgress_TriggerEvent0808(void)
{
	struct EventWork *work = gEventWork;

	if (gGameState.encounter_steps >= Math_Divide(work->encounter_threshold * 9, 10))
	{
		if ((u32)Engine_RandomNext() < 0x8000)
		{
			BattleFx_SetPhaseRequest(0x808, 3);
			work->encounter_bias = 0;
		}
		else
		{
			gGameState.encounter_steps = work->encounter_threshold;
		}
	}
}

void StoryProgress_TriggerEvent0809(void)
{
	struct GameState *state = &gGameState;
	s32				 *steps = &state->encounter_steps;
	struct EventWork *work = gEventWork;

	if (*steps >= Math_Divide(work->encounter_threshold * 9, 10))
	{
		BattleFx_SetPhaseRequest(0x809, 42);
		work->encounter_bias = 0;
	}
}

void StoryProgress_TriggerEvent080A(void)
{
	struct GameState *state = &gGameState;
	s32				 *steps = &state->encounter_steps;
	struct EventWork *work = gEventWork;

	if (*steps >= Math_Divide(work->encounter_threshold * 9, 10))
	{
		BattleFx_SetPhaseRequest(0x80a, 24);
		work->encounter_bias = 0;
	}
}

void StoryActor_AdvanceTimer(struct FieldActor *actor)
{
	if (actor->timer <= 0)
	{
		actor->timer++;
	}
	else
	{
		Engine_ObjectDispatchRelease(actor);
	}
}

void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor)
{
	struct FieldActor *spark;
	s32				   x;
	s32				   y;
	s32				   z;

	if (gFrameCount & 4)
	{
		actor->scale_x = 0x14ccc;
		actor->scale_y = 0x14ccc;
	}
	else
	{
		actor->scale_x = 0x10000;
		actor->scale_y = 0x10000;
	}
	if (!(gFrameCount & 2))
		return;
	x = actor->x.fixed;
	y = actor->y.fixed;
	z = actor->z.fixed;
	spark = Engine_ObjectCreate(0x11d, x, y, z);
	Engine_AudioPlayCue(0xf6);
	if (spark == 0)
		return;
	spark->motion_flags = 0;
	((struct FieldSpritePriority *)spark->sprite)->priority = 1;
	Engine_ActorSetSpriteFlags(spark, 0);
	Object_SetMode(spark, 1);
	spark->timer = 0;
	spark->update = (void (*)(union FieldObject *))StoryActor_AdvanceTimer;
}

void WorldMap_UpdateBobbingMarker(struct FieldActor *actor)
{
	s16 *angle;

	if (gFrameCount & 2)
		Engine_ObjectSetPartPalettes(actor, 10);
	else
		Engine_ObjectSetPartPalettes(actor, 7);
	if ((s16)actor->work == 0)
	{
		angle = (s16 *)&actor->timer;
		actor->x.fixed = 0x15d00000;
		actor->y.fixed = Iwram_MulQ16(Engine_MathSin(*angle << 3), 0x40000) + 0x100000;
		actor->z.fixed = 0x5300000;
		Vector_AddPolarOffset(0x100000, *angle, &actor->x);
		actor->facing = *angle + 0x4000;
		*angle += 0x400;
	}
}

s32 StoryActor_Initialize(struct FieldActor *actor)
{
	if (Engine_GameFlagIsSet(0x30) != 0)
	{
		return 0;
	}
	if (Engine_GameFlagIsSet(0x16E) != 0)
	{
		return 0;
	}
	actor->update = (void (*)(union FieldObject *))WorldMap_UpdateBobbingMarker;
	actor->motion_flags = 0;
	actor->timer = 0;
	actor->work = 0;
	actor->scale_x = 0x8000;
	actor->scale_y = 0x8000;
	return 0;
}

u8 *WorldMap_GetEntrances(void)
{
	return gWorldMapEntrances;
}

s32 WorldMap_GetRegions(void)
{
	return 0;
}

u8 *WorldMap_GetExits(void)
{
	return gWorldMapExits;
}

s32 StoryActor_ApplyFlaggedMode(struct FieldActor *actor)
{
	StoryActor_ApplyMapRotation(actor);
	if (Engine_GameFlagIsSet(0x847) != 0)
	{
		Object_SetMode(actor, 2);
	}
	return 1;
}

s32 StoryActor_ApplyMapRotation(struct FieldActor *actor)
{
	struct MapRenderWork *work = gMapWork[0];
	struct FieldSprite	 *sprite = actor->sprite;

	sprite->rotation = work->rotation;
	sprite->flags = 0;
	return 1;
}

s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor)
{
	struct MapRenderWork *work = gMapWork[0];
	struct FieldSprite	 *sprite = actor->sprite;

	actor->collision_flags |= 1;
	sprite->rotation = work->rotation;
	return 1;
}

s32 StoryActor_ResetPosition(struct FieldActor *actor)
{
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_ObjectSetPartPalettes(actor, 10);
	actor->collision_flags = 0;
	if (Engine_GameFlagIsSet(0x8A0) != 0)
	{
		Engine_GameFlagSet(0x2f1);
		actor->x.fixed = 0;
		actor->y.fixed = 0;
	}
	return 0;
}

s32 StoryActor_ClearActiveFlag(struct FieldActor *actor)
{
	if ((actor->active & 1) && (gFrameCount & 1))
		actor->active ^= 1;
	return 1;
}

struct ScenePlacement *StoryScene_SelectPlacementTable(void)
{
	switch (gGameState.entrance)
	{
	case 49:
		if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0)
		{
			return gWorldMapPlacements49;
		}
		break;
	case 64:
		if (Engine_GameFlagIsSet(0x85a) == 0)
		{
			return gWorldMapPlacements64;
		}
		break;
	case 65:
	case 70:
		return gWorldMapPlacements65;
	case 71:
		return gWorldMapPlacements71;
	case 72:
		return gWorldMapPlacements72;
	case 73:
		return gWorldMapPlacements73;
	case 66:
	case 67:
	case 68:
	case 69:
	case 75:
		return gWorldMapPlacements66;
	case 80:
		return gWorldMapPlacements80;
	default:
		break;
	}

	Engine_GameFlagSet(0x235);
	return gWorldMapPlacements;
}

void StoryScene_SetBranchValueFromX(s32 trigger, s32 exit_before, s32 exit_after)
{
	struct EventWork  *work;
	struct FieldActor *subject;
	struct FieldActor *other;

	subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
	other = Object_GetById(gGameState.selected_actor);
	work = gEventWork;
	if (other->x.fixed < subject->x.fixed)
	{
		work->requested_exit = exit_before;
	}
	else
	{
		work->requested_exit = exit_after;
	}
	Engine_AudioPlayCue(0x7B);
}

void StoryScene_SetBranchValueFromZ(s32 trigger, s32 exit_before, s32 exit_after)
{
	struct EventWork  *work;
	struct FieldActor *subject;
	struct FieldActor *other;

	subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
	other = Object_GetById(gGameState.selected_actor);
	work = gEventWork;
	if (other->z.fixed < subject->z.fixed)
	{
		work->requested_exit = exit_before;
	}
	else
	{
		work->requested_exit = exit_after;
	}
	Engine_AudioPlayCue(0x7B);
}

void SceneState_SetValues130_6_47(void)
{
	StoryScene_SetBranchValueFromX(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
	StoryScene_SetBranchValueFromZ(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
	StoryScene_SetBranchValueFromZ(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
	StoryScene_SetBranchValueFromZ(0x97, 0x19, 0x36);
}

void FieldScene_RunStep7D3B1E(void)
{

	StoryScene_SetBranchValueFromZ(0x7D, 0x3B, 0x1E);
}

struct SceneEvent *WorldMap_GetEvents(void)
{
	return gWorldMapEvents;
}

void MapActor_UpdateContact(void)
{
	struct FieldActor *leader;
	struct EventWork  *work;
	struct FieldActor *actor;
	s32				   left;
	s32				   right;
	s32				   top;
	s32				   bottom;
	s32				   leader_reach;
	u32				   i;

	leader = Object_GetById(gGameState.selected_actor);
	leader_reach = leader->sprite->scale * leader->radius;
	work = gEventWork;
	actor = work->view_center;
	left = actor->x.fixed - PIXELS(160);
	right = actor->x.fixed + PIXELS(160);
	top = actor->z.fixed - PIXELS(300);
	bottom = actor->z.fixed + PIXELS(200);

	for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++)
	{
		s32 x;
		s32 z;
		s32 dx;
		s32 reach;
		s32 scale;

		actor = Engine_ActorLookup(i);
		if (actor == NULL)
		{
			continue;
		}
		x = actor->x.fixed;
		z = actor->z.fixed;
		if (x < left || x > right || z < top || z > bottom)
		{
			actor->active = 0;
			continue;
		}
		actor->active = 1;
		if (gDebugMode[0] != 0 && Engine_GameFlagIsSet(FLAG_CONTACT_PAUSED) != 0)
		{
			continue;
		}
		scale = actor->sprite->scale;
		dx = actor->x.fixed - leader->x.fixed;
		if (dx < 0)
		{
			dx = leader->x.fixed - actor->x.fixed;
		}
		reach = leader_reach + actor->radius * scale;
		if (dx
					+ (actor->z.fixed - leader->z.fixed < 0 ? leader->z.fixed - actor->z.fixed
															: actor->z.fixed - leader->z.fixed)
				< reach
			&& Engine_GameFlagIsSet(FLAG_CONTACT_BLOCKED) == 0)
		{
			work->touched_trigger = i + CONTACT_TRIGGER_BASE;
		}
	}
}

void SceneState_ApplyFlag85aBranch(void)
{
	if (Engine_GameFlagIsSet(0x85a) == 0)
	{
		Engine_EventRequestExit(101);
	}
	else
	{
		Engine_AudioPlayCue(123);
		Engine_EventRequestExit(3);
	}
}

void FieldScene_RunStep74(void)
{

	Engine_EventBegin();
	Engine_EventRequestExit(74);
}

s32 WorldMap_EnterScene(void)
{
	struct GameState *state = &gGameState;
	s16				 *entrance = &state->entrance;

	if (*entrance == 99)
	{
		Engine_GameFlagSet(0x160);
		Engine_GameFlagSet(0x161);
		Engine_GameFlagSet(0x163);
	}
	if (*entrance == 90)
	{
		Scene_RunScene371SequenceA(0);
		/* FAKEMATCH: Event_SetDestination is called as void (*)(s32, s32); as void (*)(u16, u16), its prototype, WorldMap_EnterScene is 8 bytes longer */
		((void (*)(s32, s32))Event_SetDestination)((s32)&SceneId_MakyuriChojo1, 1);
	}
	else if (*entrance == 91)
	{
		Scene_RunScene371SequenceA(1);
		/* FAKEMATCH: Event_SetDestination is called as void (*)(s32, s32); as void (*)(u16, u16), its prototype, WorldMap_EnterScene is 4 bytes longer */
		((void (*)(s32, s32))Event_SetDestination)((s32)&SceneId_VinasuChojo, 93);
	}
	else if (*entrance == 78)
	{
		Engine_EventBegin();
		PartyInventory_Discard(242);
		Engine_EventRequestExit(112);
	}
	else
	{
		Engine_GameFlagSet(0x144);
		gEventWork->start_transition = 0x400;
		gEventWork->transition_frames = 16;
		Engine_TaskWait(1);
		Engine_CameraSetSpeed(0x80000, 0x10000);
		Engine_GameFlagClear(0x12f);
		Engine_TaskAddCallback(MapActor_UpdateContact, 0xc80);
		if (Engine_GameFlagIsSet(0x90a) == 0)
		{
			FieldMap_SetWindowCellTile(128, 256, 176, 56);
		}
		switch (*entrance)
		{
		case 1:
			if (GameFlag_IsSet(0x815) == 0)
			{
				Engine_GameFlagSet(0x815);
				Engine_GameFlagSet(0x85c);
			}
			break;
		case 33:
			if (Engine_GameFlagIsSet(0x109) != 0)
			{
				if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x234) != 0)
				{
					gWorldMapTriggerActor = 55;
					Actor_SetPosition(55, 0x17940000, 0xd480000);
					Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
					WorldMap_ActivateSite138(gWorldMapTriggerActor);
				}
			}
			else if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x9b8) == 0)
			{
				WorldMap_RunBlackOrbScene();
			}
			break;
		case 49:
			if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0)
			{
				RunEventScript01();
			}
			break;
		case 64:
			if (Engine_GameFlagIsSet(0x85a) == 0)
			{
				FieldScene_RunActorTransferSequence();
			}
			break;
		case 65:
			WorldMap_TransferToExit102();
			break;
		case 66:
			WorldMap_TransferToExit103();
			break;
		case 67:
			WorldMap_TransferToExit104();
			break;
		case 68:
			WorldMap_TransferToFuneHeya();
			break;
		case 69:
			WorldMap_TransferToExit106();
			break;
		case 70:
			WorldMap_CompleteTransfer();
			break;
		case 71:
			WorldMap_DisembarkCarriage();
			break;
		case 72:
			WorldMap_RunOpeningRise();
			break;
		case 73:
			StoryScene_StartTransition();
			break;
		case 74:
		case 76:
		case 77:
			Engine_GameFlagSet(0x11c);
			/* FAKEMATCH: GameFlag_GetByte is called as s32 (*)(s32); as u8 (*)(s32), its prototype, WorldMap_EnterScene is 4 bytes longer */
			if (((s32 (*)(s32))GameFlag_GetByte)(0x2f8) != 0)
			{
				gGameState.movement_mode = 2;
				Engine_TaskAddCallback(StoryScene_UpdateSelectedActorProgress, 0xc80);
			}
			break;
		case 75:
			FieldScene_RunActorEightApproach();
			break;
		case 80:
			FieldScene_RunActorPresentationSequence();
			break;
		default:
			Object_GetById(53)->scale_x = 0x14000;
			Object_GetById(53)->scale_y = 0x14000;
			break;
		}
	}
	return 0;
}

void RunEventScript01(void)
{
	u32				   i;
	struct FieldActor *record;

	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Battle_SetObjectFlag5bWhenMode3();
	GameFlag_Set(0x94f);
	Actor_SetPosition(11, 0x16e00000, 0x49c0000);
	Actor_SetDestinationOffset(11, 24, 8);
	Engine_ActorWaitForMove(11);
	Engine_EventWait(60);
	Actor_SetPosition(12, 0x16e00000, 0x49c0000);
	Actor_SetDestinationOffset(12, 12, 24);
	Engine_EventWait(30);
	Actor_FaceDirection(11, 0x5000, 0);
	Actor_FaceDirection(12, 0xd000, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimation(12, 3);
	Engine_EventWait(120);
	Actor_SetPosition(8, 0x16f80000, 0x4b80000);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(12, 2);
	record = Object_GetById(8);
	if (record != 0)
	{
		Actor_SetDestination(12, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(12);
	Actor_SetPosition(12, 0, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(11, 2);
	record = Object_GetById(8);
	if (record != 0)
	{
		Actor_SetDestination(11, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(11);
	Actor_SetPosition(11, 0, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	record = Object_GetById(8);
	if (record != 0)
	{
		Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_EventWait(60);
	Actor_SetSpeed(8, 0x8000, 0x4000);
	Actor_SetDestinationOffset(8, 56, 8);
	Engine_ActorWaitForMove(8);
	Actor_SetDestinationOffset(8, 40, 40);
	Engine_ActorWaitForMove(8);
	Actor_SetDestinationOffset(8, 8, 88);
	Engine_ActorWaitForMove(8);
	Engine_EventCloseScreen();
	Engine_EventRequestExit(108);
	Engine_EventEnd();
}

void FieldScene_RunActorTransferSequence(void)
{
	struct FieldActor *actor;
	struct FieldActor *record;
	const u8		  *action;

	actor = Object_GetById(15);
	Engine_EventBegin();
	BattleFx_ScheduleRatioTransition(0x14000, 1);
	Engine_TaskWait(4);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Battle_SetObjectFlag5bWhenMode3();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Actor_SetPosition(8, 0x16d80000, 0x6280000);
	Engine_TaskWait(1);
	Actor_SetChildValue(8, 15);
	record = Actor_Get(8);
	Engine_ActorSetSpriteFlags(record, 0);
	Actor_SetSpeed(10, 0x19999, 0x6666);
	Actor_SetSpeed(11, 0x19999, 0x6666);
	Actor_SetSpeed(12, 0x19999, 0x6666);
	Actor_SetSpeed(13, 0x19999, 0x6666);
	Event_PlayCue(141);
	Engine_ActorEnableActionCallback(10, gTransferArrive10);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(11, gTransferArrive11);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(12, gTransferArrive12);
	Engine_EventWait(20);
	Object_SetActionCallbackAndRefreshById(13, gTransferArrive13);
	Event_PlayCue(0x121);
	record = Object_GetById(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	Actor_WalkToAndWait(GERALD, 0x1704, 0x640);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Actor_SetPosition(8, 0x16d80000, 0x6380000);
	Engine_TaskWait(1);
	Engine_EventSetMessage((s32)MsgWorldMapWeCantStayAnotherMinute);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Actor_FaceDirection(GERALD, 0x6000, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Actor_FaceDirection(GERALD, 0x6000, 10);
	Event_PlayCue(141);
	Engine_ActorEnableActionCallback(10, gTransferDepart10);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(11, gTransferDepart11);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(12, gTransferDepart12);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceDirection(GERALD, 0x4000, 10);
	Object_SetActionCallbackAndRefreshById(13, gTransferDepart13);
	Event_PlayCue(0x121);
	Engine_EventWait(20);
	Battle_ClearObjectFlag5bWhenMode3();
	Engine_ActorSetAnimation(10, 1);
	Engine_ActorSetAnimation(11, 1);
	Engine_ActorSetAnimation(12, 1);
	Engine_ActorSetAnimation(13, 1);
	Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Battle_SetObjectFlag5bWhenMode3();
	Actor_SetPosition(9, 0x16080000, 0x6d80000);
	Engine_TaskWait(1);
	Actor_SetSpeed(9, 0x13333, 0x9999);
	Actor_WalkToAndWait(9, 0x1608, 0x6c8);
	Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
	Actor_WalkToAndWait(9, 0x15f8, 0x6f8);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(9, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(9, 0, 20);
	Engine_ActorStartRepeatedMotion(9, 3);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_SetPosition(8, 0x16180000, 0x6f80000);
	Engine_TaskWait(1);
	Actor_SetChildValue(8, 0);
	record = Actor_Get(8);
	Engine_ActorSetSpriteFlags(record, 1);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_WalkToAndWait(8, 0x1608, 0x6f8);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(0x2008, 0, 10);
	Actor_FaceDirection(8, 0x3000, 60);
	Actor_FaceDirection(8, 0x8000, 10);
	Actor_SetAttachedEffect(8, 0x102);
	Engine_EventWait(60);
	Actor_FaceDirection(9, 0x3000, 0);
	Actor_FaceDirection(8, 0x3000, 40);
	Actor_SetAttachedEffect(8, 0x102);
	Engine_EventWait(60);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(0x2008, 0, 40);
	Engine_ActorRunRepeatedMotion(9, 1);
	Actor_FaceDirection(9, 0, 10);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_ShowEmote(8, 0x105, 60);
	Event_ShowMessageAndWait(0x2008, 0, 10);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_ActorSetAnimationAndWait(8, 3);
	Event_ShowMessageAndWait(0x2008, 0, 10);
	Actor_ShowEmote(9, 0x101, 60);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Battle_ClearObjectFlag5bWhenMode3();
	Event_PlayCue(107);
	Camera_SetSpeed(0x40000, 0x40000);
	WorldMap_ShakeTransferStartCamera();
	Event_PlayCue(0x121);
	Actor_ShowEmote(8, 0x100, 0);
	Actor_ShowEmote(9, 0x100, 0);
	Actor_FaceDirection(8, 0x8000, 0);
	Actor_FaceDirection(9, 0, 40);
	Actor_FaceDirection(8, 0xb000, 0);
	Actor_FaceDirection(9, 0xb000, 0);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(14, 0x15a80000, 0x6a80000);
	Engine_TaskWait(1);
	Actor_SetSpeed(14, 0x4ccc, 0x2666);
	Engine_ActorEnableActionCallback(14, gTransferGuide14);
	Engine_EventWait(160);
	actor->gravity = 0x1999;
	actor->bounce = 0x1999;
	actor->scale_x = 0x18000;
	actor->scale_y = 0x18000;
	actor->timer = 0;
	actor->y.fixed = 0x400000;
	actor->sprite->rotation = 0xf000;
	Engine_ActorSetSpriteFlags(actor, 0);
	Object_SetMode(actor, 2);
	Engine_TaskWait(1);
	record = Actor_Get(15);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_TaskAddCallback(WorldMap_UpdateTransferredActorLanding, 0xc80);
	do
	{
		Engine_TaskWait(1);
	} while (actor->timer == 0);
	record = Actor_Get(15);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Actor_Get(14);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(10);
	record = Actor_Get(9);
	record->velocity_y = 0x60000;
	record = Object_GetById(8);
	record->velocity_y = 0x60000;
	Event_PlayCue(145);
	Camera_SetSpeed(0x40000, 0x40000);
	WorldMap_ShakeTransferLandingCamera();
	WorldMap_ShakeTransferLandingCamera();
	Engine_EventWait(60);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
	Engine_CameraWaitForMove();
	Battle_SetObjectFlag5bWhenMode3();
	Actor_SetAttachedEffect(9, 0x102);
	Actor_SetAttachedEffect(8, 0x102);
	Engine_EventWait(60);
	Engine_TaskRemoveCallback(WorldMap_UpdateTransferredActorLanding);
	Engine_TaskWait(1);
	Actor_SetPosition(14, 0, 0);
	Actor_SetPosition(15, 0, 0);
	Actor_FaceDirection(8, 0x8000, 10);
	Engine_ActorJump(8, 4, 40);
	Event_ShowMessageAndWait(0x2008, 0, 10);
	Actor_FaceDirection(9, 0, 10);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(8, 0xc000, 40);
	Event_ShowMessageAndWait(0x2008, 0, 20);
	Engine_ActorJump(9, 4, 20);
	Event_ShowMessageAndWait(9, 0, 10);
	Engine_ActorRunRepeatedMotion(8, 1);
	Actor_FaceDirection(8, 0x8000, 10);
	Event_ShowMessageAndWait(0x2008, 0, 10);
	Actor_SetAttachedEffect(9, 0x102);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_ActorSetAnimationAndWait(9, 3);
	Actor_WalkToAndWait(8, 0x1618, 0x6f8);
	Actor_SetPosition(8, 0, 0);
	Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
	Actor_WalkToAndWait(9, 0x1608, 0x6c8);
	Actor_WalkToAndWait(9, 0x1608, 0x6d8);
	Actor_SetPosition(9, 0, 0);
	Event_PlayCue(141);
	Engine_ActorEnableActionCallback(10, gTransferReturn10);
	Engine_ActorEnableActionCallback(11, gTransferReturn11);
	Engine_EventWait(40);
	Engine_ActorEnableActionCallback(12, gTransferReturn12);
	Engine_EventWait(40);
	Object_SetActionCallbackAndRefreshById(13, gTransferReturn13);
	Battle_ClearObjectFlag5bWhenMode3();
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
	Actor_SetPosition(GERALD, 0x17140000, 0x6400000);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
	Engine_CameraWaitForMove();
	action = gTransferGather;
	Engine_ActorEnableActionCallback(10, action);
	Engine_EventWait(20);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x16d80000, -1, 0x6080000, 1);
	Engine_ActorEnableActionCallback(11, action);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(12, action);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
	Actor_FaceDirection(GERALD, 0x8000, 0);
	Engine_ActorEnableActionCallback(13, action);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Object_RefreshSelectorById(13);
	Event_PlayCue(0x121);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0x16f80000, -1, 0x6480000, 1);
	Engine_CameraWaitForMove();
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(GERALD, 0xa000, 80);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Camera_SetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x85a);
	Engine_EventRequestExit(3);
	Engine_EventEnd();
}

void WorldMap_ShakeTransferStartCamera(void)
{
	Camera_MoveTo(0x160c0000, -1, 0x6f80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16040000, -1, 0x6fc0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x160c0000, -1, 0x6f40000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x160c0000, -1, 0x6fc0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16040000, -1, 0x6f40000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x160a0000, -1, 0x6f80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16060000, -1, 0x6fa0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x160a0000, -1, 0x6f60000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x160a0000, -1, 0x6fa0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16060000, -1, 0x6f60000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
	Engine_TaskWait(4);
}

void WorldMap_ShakeTransferLandingCamera(void)
{
	Camera_MoveTo(0x15ec0000, -1, 0x6c80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e40000, -1, 0x6cc0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15ec0000, -1, 0x6c40000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15ec0000, -1, 0x6cc0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e40000, -1, 0x6c40000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15ea0000, -1, 0x6c80000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e60000, -1, 0x6ca0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15ea0000, -1, 0x6c60000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15ea0000, -1, 0x6ca0000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e60000, -1, 0x6c60000, 1);
	Engine_TaskWait(4);
	Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
	Engine_TaskWait(4);
}

void WorldMap_UpdateTransferredActorLanding(void)
{
	struct FieldActor *actor;
	struct FieldActor *guide;

	actor = Object_GetById(15);
	guide = Object_GetById(14);
	actor->x.fixed = guide->x.fixed;
	actor->z.fixed = guide->z.fixed;
	if (actor->y.fixed < 0xa0000)
	{
		actor->y.fixed = 0xa0000;
		if (GameFlag_IsSet(0x200) == 0)
		{
			Event_PlayCue(145);
			Object_SetMode(actor, 3);
			GameFlag_Set(0x200);
			actor->timer = 1;
		}
	}
}

void WorldMap_TransferToExit102(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	actor->scale_y = 0x14000;
	actor->scale_x = 0x14000;
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x6666, 0x3333);
	Actor_WalkToAndWait(8, 0x14a8, 0x918);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x927);
	Engine_EventRequestExit(102);
	Engine_EventEnd();
}

void WorldMap_TransferToExit103(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(8, 0x1f080000, 0xc80000);
	actor->scale_x = 0x14000;
	actor->scale_y = 0x14000;
	Engine_TaskWait(1);
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x9999, 0x4ccc);
	actor->timer = 0;
	Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
	do
	{
		Engine_TaskWait(1);
	} while (actor->timer == 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x927);
	Engine_EventRequestExit(103);
	Engine_EventEnd();
}

void WorldMap_TransferToExit104(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(8, 0x1f080000, 0xc80000);
	actor->scale_x = 0x14000;
	actor->scale_y = 0x14000;
	Engine_TaskWait(1);
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x9999, 0x4ccc);
	actor->timer = 0;
	Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
	do
	{
		Engine_TaskWait(1);
	} while (actor->timer == 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x927);
	Engine_EventRequestExit(104);
	Engine_EventEnd();
}

void WorldMap_TransferToFuneHeya(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(8, 0x1f080000, 0xc80000);
	actor->scale_x = 0x14000;
	actor->scale_y = 0x14000;
	Engine_TaskWait(1);
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x9999, 0x4ccc);
	actor->timer = 0;
	Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
	do
	{
		Engine_TaskWait(1);
	} while (actor->timer == 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x927);
	Engine_EventRequestExit(105);
	Engine_EventEnd();
}

void WorldMap_TransferToExit106(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(8, 0x1f080000, 0xc80000);
	actor->scale_x = 0x14000;
	actor->scale_y = 0x14000;
	Engine_TaskWait(1);
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x9999, 0x4ccc);
	actor->timer = 0;
	if (StoryScene_ComputeOpposingSlotDelta() == 11)
	{
		Engine_ActorEnableActionCallback(8, gTransferLeaderTurn);
	}
	else
	{
		Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
	}
	do
	{
		Engine_TaskWait(1);
	} while (actor->timer == 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x927);
	Engine_EventRequestExit(106);
	Engine_EventEnd();
}

void WorldMap_CompleteTransfer(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(8);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(8, 0x13e80000, 0x9180000);
	actor->scale_y = 0x14000;
	actor->scale_x = 0x14000;
	Engine_TaskWait(1);
	Engine_CameraFollowActor(8, 1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(8, 0x6666, 0x3333);
	Actor_WalkToAndWait(8, 0x13c8, 0x918);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(0x93e);
	GameFlag_Clear(0x927);
	Engine_EventRequestExit(107);
	Engine_EventEnd();
}

void WorldMap_ShowDjinnTutorial(void)
{
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_ScheduleRatioTransition(0x10000, 6);
	Event_WaitForZoomTransition();
	Battle_SetObjectFlag5bWhenMode3();
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventSetMessage((s32)MsgWorldMapAfterBringingDjinniIntoYour);
	Event_ShowMessage(8, 0);
	Engine_EventWait(30);
	Event_PlayCue(111);
	Menu_AnimateSelectionToEntry(0, 2);
	GameFlag_Clear(0x16f);
	GameFlag_Clear(0x171);
	ItemMenu_Open();
	Engine_ActorJump(8, 4, 30);
	Engine_EventSetMessage((s32)MsgWorldMapNextIllShowHowCan);
	Event_ShowMessage(8, 0);
	GameFlag_Clear(0x16f);
	GameFlag_Set(0x171);
	ItemMenu_Open();
	Engine_EventWait(30);
	BattleFx_SetWeightedResult(12, 6);
}

#define DJINNI 8
#define FLAG_DJINNI_MET 0x16e

void WorldMap_MeetVenusDjinni(void)
{
	struct FieldActor *djinni;
	struct FieldActor *leader;
	s32				   x;
	s32				   z;
	s32				   count;
	s32				   i;

	count = 0;
	djinni = Object_GetById(DJINNI);
	leader = Object_GetById(0);
	x = (leader->x.fixed - 0x15d00000) / 2 + 0x15d00000;
	z = (leader->z.fixed - 0x5300000) / 2 + 0x5300000;
	if (!GameFlag_IsSet(FLAG_DJINNI_MET))
	{
		Owner_RefreshActiveRatios(1);
		GameFlag_Set(FLAG_DJINNI_MET);
		Event_Begin();
		leader = Actor_Get(0);
		if (leader != 0)
			Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
		Djinn_AddToOwner(0, 0, 0);
		Trade_AddOffer(0, 0, 0);
		Battle_SetObjectFlag5bWhenMode3();
		Actor_FaceActor(0, DJINNI, 0);
		Event_Wait(10);
		Actor_ShowEmote(0, 0x101, 60);
		djinni->work = 1;
		Actor_FaceActor(DJINNI, 0, 0);
		Task_Wait(16);
		Event_SetMessage((s32)MsgWorldMapOh);
		Event_ShowMessage(DJINNI, 0);
		Battle_ClearObjectFlag5bWhenMode3();
		BattleFx_ScheduleRatioTransition(0x13333, 6);
		Event_WaitForZoomTransition();
		Battle_SetObjectFlag5bWhenMode3();
		djinni->motion_flags = 2;
		djinni->gravity = 0x4000;
		djinni->speed = 0x10000;
		djinni->acceleration = 0x10000;
		djinni->velocity_y = 0;
		djinni->terrain_height = 0;
		FieldObject_SetPosition(djinni, 0x15d00000, 0, 0x5300000);
		for (i = 0; i < 16; i++)
		{
			djinni->scale_x += 0x800;
			djinni->scale_y += 0x800;
			Task_Wait(1);
		}
		Actor_FaceActor(DJINNI, 0, 0);
		Actor_FaceActor(0, DJINNI, 0);
		Task_Wait(16);
		djinni->update = 0;
		Engine_ObjectSetPartPalettes(djinni, 0);
		djinni->gravity = 0x10000;
		Event_ShowMessage(DJINNI, 0);
		Event_PlayCue(131);
		Engine_PsynergyBegin(140, 0);
		for (i = 59; i >= 0; i--)
		{
			if (gFrameCount & 2)
				Engine_ObjectSetPartPalettes(djinni, 7);
			else
				Engine_ObjectSetPartPalettes(djinni, 0);
			if ((gFrameCount & 15) == 0)
				WorldMap_CreateLinkedEffects(djinni);
			Task_Wait(1);
		}
		BattleEffect_CleanupSceneObjects();
		Engine_ObjectSetPartPalettes(djinni, 0);
		Actor_RunRepeatedMotion(DJINNI, 2);
		Event_ShowMessage(DJINNI, 0);
		Actor_ShowEmote(0, 0x102, 30);
		Event_ShowMessage(DJINNI, 0);
		Actor_ShowEmote(0, 0x101, 30);
		Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
		Actor_SetAnimation(0, 22);
		Event_ShowMessage(DJINNI, 0);
		Actor_ShowEmote(0, 0x101, 40);
		Actor_Jump(DJINNI, 4, 30);
		UiWork_PushValueSlot(300, 4);
		Event_ShowMessage(DJINNI, 0);
		Actor_ShowEmote(0, 0x100, 30);
		Event_ShowMessage(DJINNI, 0);
		Actor_RunRepeatedMotion(0, 2);
		Event_ShowMessage(DJINNI, 0);
		Actor_Jump(DJINNI, 2, 30);
		Event_ShowMessage(DJINNI, 0);
		i = 0;
		djinni->motion_flags = 0;
		FieldObject_SetPosition(djinni, x, 0x100000, z);
		for (; i < 16; i++)
		{
			djinni->facing += 0x1000;
			Task_Wait(1);
		}
		Actor_SetAnimation(0, 1);
		Event_ShowMessage(DJINNI, 0);
		djinni->motion_flags = 2;
		djinni->velocity_y = 0;
		djinni->terrain_height = 0;
		for (i = 7; i >= 0; i--)
		{
			djinni->facing += 0x1000;
			Task_Wait(1);
		}
		Actor_SetAnimation(0, 22);
		Event_ShowMessage(DJINNI, 0);
		Actor_ShowEmote(DJINNI, 0x102, 30);
		Actor_FaceActor(DJINNI, 0, 0);
		Actor_RunRepeatedMotion(DJINNI, 2);
		Event_ShowMessage(DJINNI, 0);
		Actor_Jump(DJINNI, 2, 30);
		Event_OpenMessage(DJINNI, 0);
		count = 0;
	plead:
		if (Event_ChooseYesNo(0, 0) == 1)
		{
			Actor_Jump(DJINNI, 2, 20);
			Actor_Jump(DJINNI, 2, 20);
			if (count == 6)
			{
				Event_SetMessage((s32)MsgWorldMapMeanieDontCare);
				Event_ShowMessage(DJINNI, 0);
				goto listened;
			}
			Event_SetMessage(count + (s32)MsgWorldMapComePromiseWont);
			Event_OpenMessage(DJINNI, 0);
			count++;
			goto plead;
		}
		Actor_SetAnimation(0, 22);
		Actor_Jump(DJINNI, 2, 20);
		Actor_Jump(DJINNI, 4, 20);
		Event_SetMessage((s32)MsgWorldMapSeeWontRegret);
		Event_ShowMessage(DJINNI, 0);
	listened:
		UiWork_PushValueSlot(300, 4);
		Event_PlayCue(81);
		count = (s32)MsgWorldMapAbilityVenusDjinni;
		Message_ShowCentered(count++, 3);
		Event_SetMessage(count);
		Actor_Jump(DJINNI, 2, 20);
		Event_ShowMessage(DJINNI, 0);
		Event_PlayCue(9);
		WorldMap_ShowDjinnTutorial();
		Battle_ClearObjectFlag5bWhenMode3();
		return;
	}
	Event_Begin();
	leader = Object_GetById(0);
	if (leader != 0)
		Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
	djinni->velocity_y = 0xa0000;
	FieldObject_SetPosition(djinni, x, 0, z);
	Event_Wait(30);
	Battle_SetObjectFlag5bWhenMode3();
	Actor_FaceActor(DJINNI, 0, 0);
	Actor_FaceActor(0, DJINNI, 0);
	Actor_SetAnimation(0, 22);
	Event_SetMessage((s32)MsgWorldMapSeeDjinnUseful);
	Actor_Jump(DJINNI, 2, 20);
	Actor_Jump(DJINNI, 2, 20);
	Event_ShowMessage(DJINNI, 0);
	Actor_RunRepeatedMotion(DJINNI, 2);
	Event_ShowMessage(DJINNI, 0);
	Event_PlayCue(111);
	Menu_AnimateSelectionToEntry(0, 2);
	GameFlag_Set(0x16f);
	GameFlag_Clear(0x171);
	ItemMenu_Open();
	Event_SetMessage((s32)MsgWorldMapGoSetStandby);
	FieldObject_SetPosition(djinni, 0x15d00000, 0, 0x5300000);
	Event_Wait(30);
	Event_ShowMessage(DJINNI, 0);
	Actor_FaceActor(DJINNI, 0, 0);
	Event_ShowMessage(DJINNI, 0);
	Event_OpenMessage(DJINNI, 0);
	if (Event_ChooseYesNo(0, 0) != 1)
		goto learn;
	Actor_SetAnimation(0, 22);
	Actor_RunRepeatedMotion(DJINNI, 2);
	Event_SetMessage((s32)MsgWorldMapHmmmmExplainAgain);
	Event_OpenMessage(DJINNI, 0);
	if (Event_ChooseYesNo(0, 0) == 1)
		goto learn;
	Event_ShowMessage(DJINNI, 0);
	Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
	WorldMap_ShowDjinnTutorial();
	Battle_ClearObjectFlag5bWhenMode3();
	return;
learn:
	Actor_SetAnimation(0, 22);
	Event_SetMessage((s32)MsgWorldMapYeahWantLearn);
	Actor_Jump(DJINNI, 2, 20);
	Actor_Jump(DJINNI, 2, 20);
	Actor_SetAnimationAndWait(0, 3);
	Actor_ShowEmote(DJINNI, 0x100, 30);
	Event_ShowMessage(DJINNI, 0);
	GameFlag_Set(0x16f);
	GameFlag_Set(0x171);
	ItemMenu_Open();
	Actor_Jump(DJINNI, 2, 20);
	Event_ShowMessage(DJINNI, 0);
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_RunPageEffectForSlot(DJINNI, 0, 0);
	Event_PlayCue(42);
	Event_End();
	GameFlag_Clear(FLAG_DJINNI_MET);
	GameFlag_Clear(0x16f);
	GameFlag_Clear(0x171);
}

s32 gWorldMapTriggerActor;

void WorldMap_DisembarkCarriage(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(10);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_TaskWait(1);
	actor->scale_x = 0x18000;
	actor->scale_y = 0x18000;
	actor->facing = 0x4000;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_SetPosition(10, 0x15680000, 0x8380000);
	Engine_TaskWait(1);
	Event_PlayCue(141);
	Actor_SetSpeed(10, 0x19999, 0x6666);
	Engine_ActorSetAnimation(10, 2);
	Actor_MoveToAndWait(10, 0x156d, 0x858);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x15b80000, -1, 0x8580000, 1);
	Actor_MoveToAndWait(10, 0x159e, 0x858);
	Actor_MoveToAndWait(10, 0x15a8, 0x86e);
	Actor_MoveToAndWait(10, 0x15e8, 0x878);
	Engine_ActorSetAnimation(10, 1);
	Event_PlayCue(0x121);
	Engine_EventWait(20);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x15d80000, 0x8780000);
	Engine_TaskWait(1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15c8, 0x878);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
	Event_PlayCue(141);
	Engine_ActorSetAnimation(10, 2);
	Actor_MoveToAndWait(10, 0x15f8, 0x878);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_MoveToAndWait(10, 0x15f8, 0x838);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_MoveToAndWait(10, 0x15bd, 0x838);
	Actor_MoveToAndWait(10, 0x15b8, 0x853);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
	Actor_MoveToAndWait(10, 0x1572, 0x858);
	Actor_MoveToAndWait(10, 0x1568, 0x838);
	Actor_SetPosition(10, 0, 0);
	Event_PlayCue(0x121);
	Engine_EventWait(40);
	Camera_MoveTo(0x15d80000, -1, 0x8580000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15d8, 0x858);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(20);
	Engine_EventEnd();
}

void WorldMap_RunBlackOrbScene(void)
{
	struct FieldActor *leader;

	Engine_EventBegin();
	gWorldMapTriggerActor = 55;
	WorldMap_ActivateSite138(55);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Battle_SetObjectFlag5bWhenMode3();
	leader = Object_GetById(0);
	if (leader != NULL)
	{
		Engine_ActorSetPosition(gWorldMapTriggerActor, leader->x.fixed, leader->z.fixed);
	}
	Actor_SetSpeed(gWorldMapTriggerActor, 0x19999, 0xcccc);
	Actor_WalkToAndWait(gWorldMapTriggerActor, 0x1768, 0xd78);
	Engine_ActorFaceDirection(gWorldMapTriggerActor, 0, 20);
	Engine_ActorShowEmote(gWorldMapTriggerActor, 0x100, 60);
	Engine_ActorStartRepeatedMotion(gWorldMapTriggerActor, 2);
	Engine_EventSetMessage((s32)MsgWorldMapLook);
	Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 10);
	Engine_ActorFaceDirection(0, 0, 0);
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_ScheduleRatioTransition(0x16666, 10);
	Engine_CameraSetSpeed(0x80000, 0x10000);
	Engine_CameraMoveTo(0x17880000, -1, 0xd680000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Battle_SetObjectFlag5bWhenMode3();
	Engine_ActorSetAttachedEffect(gWorldMapTriggerActor, 0x102);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
	Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x3000, 0, 40);
	Actor_WalkToAndWait(gWorldMapTriggerActor, 0x1768, 0xd48);
	Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 0x1794, 0xd48);
	Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 20);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Engine_ActorEnableActionCallback(0, gBlackOrbLeaderScript);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
	Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
	while (Object_GetById(0)->timer == 0)
	{
		Engine_TaskWait(1);
	}
	Actor_FaceDirection(0, 0x4000, 20);
	Engine_ActorShowEmote(gWorldMapTriggerActor, 0x106, 0);
	Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 2);
	Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
	Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x8000, 10);
	Engine_EventShowMessage(gWorldMapTriggerActor, 0);
	Engine_ActorFaceDirection(0, 0, 20);
	Object_GetById(gWorldMapTriggerActor)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(gWorldMapTriggerActor, 0x178c, 0xd48);
	Engine_EventWait(1);
	Object_GetById(gWorldMapTriggerActor)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventWait(20);
	Object_GetById(gWorldMapTriggerActor)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 0x1794, 0xd48);
	Engine_EventWait(1);
	Object_GetById(gWorldMapTriggerActor)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_ItemShowFound(242, 3);
	Engine_PartyGiveItem(242, 0);
	Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
	Engine_EventSetMessage((s32)MsgWorldMapNowUseOnShip);
	Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
	Engine_ActorFaceDirection(gWorldMapTriggerActor, 0x3000, 0);
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_ScheduleRatioTransition(0x10000, 10);
	Engine_EventWait(20);
	Engine_GameFlagSet(0x234);
	Engine_GameFlagSet(0x9bf);
	gGameState.saved_scene = (s32)&SceneId_WorldMap;
	gGameState.saved_entrance = 78;
	Engine_EventEnd();
}

void WorldMap_ActivateSite138(s32 actor)
{
	s32 i;

	i = 0;
	while (1)
	{
		if (gWorldMapEvents[i].control == EVENT_EXIT && gWorldMapEvents[i].trigger == 138)
		{
			gWorldMapEvents[i].control = EVENT_TOUCH;
			gWorldMapEvents[i].value = (u32)WorldMap_StopLeaderLeavingShip;
		}
		if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
			break;
		i++;
	}
	for (i = 0;; i++)
	{
		if (gWorldMapPlacements[i].sprite == 57)
		{
			gWorldMapPlacements[i].x = 0x17940000;
			gWorldMapPlacements[i].z = 0x0d480000;
			gWorldMapPlacements[i].facing = 0x3000;
			return;
		}
	}
}

void WorldMap_RestoreExitTrigger(void)
{
	s32 i;

	i = 0;
	while (1)
	{
		if (gWorldMapEvents[i].control == EVENT_TOUCH && gWorldMapEvents[i].trigger == 138)
		{
			gWorldMapEvents[i].control = EVENT_EXIT;
			gWorldMapEvents[i].value = 33;
			break;
		}
		if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
			break;
		i++;
	}
}

void WorldMap_PromptBlackOrbUse(void)
{
	Engine_EventBegin();
	Actor_FaceActor(55, ACTOR_PARTY_LEADER, 0);
	Engine_EventSetMessage((s32)MsgWorldMapNowUseOnShip);
	Engine_EventShowMessage(gWorldMapTriggerActor, 0);
	Actor_FaceDirection(55, 0x3000, 0);
	Engine_EventEnd();
}

void WorldMap_StopLeaderLeavingShip(void)
{
	Engine_EventBegin();
	Battle_SetObjectFlag5bWhenMode3();
	Engine_EventSetMessage((s32)MsgWorldMapRobinWhereGoingSaidUse);
	Engine_EventShowMessage(gWorldMapTriggerActor, 0);
	Battle_ClearObjectFlag5bWhenMode3();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1778, 0xd48);
	Engine_EventEnd();
}

void StoryScene_ShowRewardDialogue(void)
{

	Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
	Battle_SetObjectFlag5bWhenMode3();
#endif
	Engine_MessageShowCentered((s32)MsgWorldMapWreckageShipScuttledOffCoast, 1);
	if (Engine_GameFlagIsSet(0x234) != 0)
	{
		((struct StoryDialogueWork *)gEventWork)->story_result = 1;
	}
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
	Battle_ClearObjectFlag5bWhenMode3();
#endif
	Engine_EventEnd();
}

#define SITE 55
#define ITEM_BLACK_ORB 242

void WorldMap_UseBlackOrb(void)
{
	struct FieldActor  *site;
	struct FieldActor  *orb;
	struct FieldActor  *leader;
	struct FieldActor  *actor;
	s32					message;
	struct FieldSprite *sprite;
	u8				   *buffer;

	leader = Actor_Get(ACTOR_PARTY_LEADER);
	Event_Begin();
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_ScheduleRatioTransition(0x16666, 6);
	Camera_SetSpeed(0x30000, 0x6000);
	Camera_MoveTo(0x17880000, -1, 0xd680000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	site = NULL;
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
	leader->paused = 0;
	ObjectDispatch_InitFromTable6(leader);
	if (leader->z.fixed > 0xd680000)
	{
		if (leader->x.fixed > 0x176e0000)
		{
			FieldObject_SetPosition(leader, 0x176e0000, leader->y.fixed, 0xd7d0000);
			Engine_ObjectCommitPosition(leader);
		}
	}
	else if (leader->x.fixed > 0x177a0000)
	{
		FieldObject_SetPosition(leader, 0x177a0000, site->y.fixed, 0xd480000);
		Engine_ObjectCommitPosition(leader);
	}
	FieldObject_SetPosition(leader, 0x17690000, 0, 0xd680000);
	Engine_ObjectCommitPosition(leader);
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
	Battle_SetObjectFlag5bWhenMode3();
	Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Event_Wait(20);
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
	orb = Object_Create(22, leader->x.fixed + 0x20000, 0x260000, leader->z.fixed);
	if (orb != NULL)
	{
		u8 *motion;

		orb->motion_flags = 0;
		sprite = orb->sprite;
		sprite->flags = 0;
		sprite->part_count = 0;
		sprite->full_color = 0;
		sprite->palette = 0;
		/* FAKEMATCH: a second clear of motion_flags through a byte pointer.
         * Post-reload cse removes it as a no-op (the sprite stores are in
         * another alias set, so the first store's value is still known),
         * and the first store survives flow's dead-store pass because the
         * byte-pointer store conflicts with those sprite stores. Until then
         * it keeps the zero and the address alive across the attribute
         * clears, so the zero takes r1 and the address r0, as the game has
         * them. As plain stores the zero takes r3 and the address a copy in
         * r2 (differ 20 in EN); a repeated struct-field store deletes the
         * first store instead and leaves the clear at the end (differ 6). */
		motion = &orb->motion_flags;
		*motion = 0;
		buffer = Heap_Allocate(17, 0x608);
		Item_LoadIcon(ITEM_BLACK_ORB);
		Vram_Load(sprite->vram_block, 128, buffer + 0x400);
		Heap_Release(17);
		Event_Wait(20);
		orb->update = (void (*)(union FieldObject *))StoryActor_ConfigureSpawnedObject;
		Event_Wait(80);
	}
	Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
	Actor_ShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT, 0);
	Actor_RunRepeatedMotion(gWorldMapTriggerActor, 2);
	message = (s32)MsgWorldMapMatter;
	Event_SetMessage(message);
	Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 80);
	if (orb != NULL)
	{
		Engine_ObjectDispatchRelease(orb);
	}
	Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
	Event_Wait(40);
	Actor_Jump(gWorldMapTriggerActor, 6, 40);
	Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_FaceDirection(gWorldMapTriggerActor, 0xd000, 20);
	Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 40);
	Actor_SetAnimationAndWait(gWorldMapTriggerActor, 4);
	Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 20);
	Actor_FaceDirection(gWorldMapTriggerActor, 0x3000, 20);
	Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
	Actor_SetSpeed(gWorldMapTriggerActor, 0xcccc, 0x6666);
	Actor_SetAnimation(gWorldMapTriggerActor, 2);
	site = Actor_Get(SITE);
	FieldObject_SetPosition(site, 0x177a0000, site->y.fixed, 0xd480000);
	Engine_ObjectCommitPosition(site);
	FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
	Engine_ObjectCommitPosition(site);
	Actor_SetAnimation(SITE, 1);
	Actor_FaceDirection(gWorldMapTriggerActor, 0x5000, 10);
	Actor_RunRepeatedMotion(gWorldMapTriggerActor, 1);
	Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x1000, 0, 20);
	Actor_SetSpeed(gWorldMapTriggerActor, 0x10000, 0x8000);
	Object_GetById(SITE)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetAnimation(SITE, 2);
	FieldObject_SetPosition(site, 0x176d0000, 0, 0xd600000);
	Engine_ObjectCommitPosition(site);
	Actor_SetAnimation(SITE, 1);
	Event_Wait(10);
	Actor_SetAnimation(SITE, 2);
	FieldObject_SetPosition(site, 0x17710000, 0, 0xd580000);
	Engine_ObjectCommitPosition(site);
	Actor_SetAnimation(SITE, 1);
	Message_ShowCentered(message + 6, 1);
	gEventWork->message++;
	PartyInventory_Discard(ITEM_BLACK_ORB);
	Event_Wait(20);
	Actor_SetAnimation(gWorldMapTriggerActor, 4);
	Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
	Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_SetAnimationAndWait(gWorldMapTriggerActor, 3);
	Actor_SetAnimation(gWorldMapTriggerActor, 2);
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetDestination(gWorldMapTriggerActor, actor->x.part.pixel, actor->z.part.pixel);
	}
	Actor_WaitForMove(gWorldMapTriggerActor);
	Actor_SetPosition(gWorldMapTriggerActor, 0, 0);
	Battle_ClearObjectFlag5bWhenMode3();
	BattleFx_ScheduleRatioTransition(0x10000, 6);
	Event_Wait(20);
	WorldMap_RestoreExitTrigger();
	Engine_ActorDestroy(gWorldMapTriggerActor);
	GameFlag_Clear(0x234);
	GameFlag_Set(0x85d);
	Event_End();
}

enum
{
	TRANSITION_EFFECT_TYPE = 222,
	TRANSITION_EFFECT_LEFT = 0x17b0,
	TRANSITION_EFFECT_TOP = 0x0c4c,
	TRANSITION_EFFECT_WIDTH = 40,
	TRANSITION_EFFECT_DEPTH = 30
};

s32 __umodsi3(u32 value, s32 divisor);
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

u16	 gWorldMapBlend;

#define QueueFrame(buffer, offset) \
	IO_WRITE_QUEUE_PUSH(0x06002000, (u32)((buffer) + (offset)), 0x84000140)

void FieldScene_RunActorPresentationSequence(void)
{
	PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	Engine_EventBegin();
	BattleFx_ScheduleRatioTransition(0x10000, 0x12c);
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_ActorSetAnimation(JASMINE, 19);
	Engine_ActorSetAnimation(8, 5);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_TaskWait(1);
	BattleFx_ScheduleRatioTransition(0x18000, 16);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	ColorBuffer_ApplyTarget(0x10003, 1);
	gEventWork->transition_frames = 16;
	Engine_EventOpenScreen();
	Event_WaitForZoomTransition();
	Battle_SetObjectFlag5bWhenMode3();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgWorldMapSukuretaHowLongWillIsland);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(JASMINE, 0x107, 20);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_ShowEmote(8, 0x105, 80);
	Event_ShowMessageAndWait(8, 0, 10);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_ShowEmote(8, 0x105, 100);
	Actor_ShowEmote(JASMINE, 0x105, 40);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_ShowEmote(JASMINE, 0x102, 20);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Actor_SetAttachedEffect(8, 0x102);
	Engine_EventWait(80);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(JASMINE, 0x105, 80);
	Event_ShowMessageAndWait(8, 0, 120);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Event_ShowMessageAndWait(JASMINE, 0, 40);
	Event_ShowMessageAndWait(8, 0, 20);
	Actor_ShowEmote(JASMINE, 0x105, 40);
	Event_ShowMessageAndWait(JASMINE, 0, 120);
	Actor_SetSpeed(9, 0x6666, 0x3333);
	Actor_SetPosition(9, 0x1ddc0000, 0xd840000);
	Actor_WalkToAndWait(9, 0x1d94, 0xd8c);
	Actor_WalkToAndWait(9, 0x1d88, 0xda0);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x6009, 0, 20);
	Actor_ShowEmote(8, 0x101, 0);
	Actor_ShowEmote(JASMINE, 0x101, 60);
	Engine_ActorSetAnimationAndWait(9, 3);
	Event_ShowMessage(0x6009, 0);
	Map_LoadDefaultCellsAndUpdateBlock();
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(9, gPresentGuide9);
	Engine_EventWait(80);
	Engine_ActorSetAnimation(8, 1);
	Engine_ActorJump(8, 4, 40);
	Engine_ActorSetAnimation(JASMINE, 1);
	Engine_ActorJump(JASMINE, 4, 60);
	Actor_FaceDirection(8, 0x3000, 0);
	Actor_FaceDirection(JASMINE, 0xb000, 40);
	Actor_SetSpeed(8, 0x9999, 0x4ccc);
	Actor_SetSpeed(JASMINE, 0x9999, 0x4ccc);
	Engine_ActorEnableActionCallback(8, gPresentGuide8);
	Engine_EventWait(20);
	Camera_SetSpeed(0xb333, 0x1666);
	Camera_MoveTo(0x1e380000, -1, 0xdc80000, 1);
	Engine_ActorEnableActionCallback(5, gPresentGuide5);
	do
	{
		Engine_ActorSetAnimation(10, 6);
		Engine_ActorSetAnimation(6, 8);
		Engine_TaskWait(1);
	} while (Object_GetById(5)->timer == 0);
	Engine_EventWait(20);
	Actor_FaceDirection(9, 0x8000, 20);
	Actor_SetAttachedEffect(8, 0x102);
	Actor_SetAttachedEffect(JASMINE, 0x102);
	Engine_EventWait(40);
	Battle_SetObjectFlag5bWhenMode3();
	Event_ShowTwoMessagesAndWait(5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
	Engine_EventWait(20);
	Map_LoadDefaultCellsAndUpdateBlock();
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x1e580000, -1, 0xdc80000, 1);
	Actor_FaceDirection(9, 0x3000, 0);
	Actor_SetSpeed(8, 0x19999, 0xcccc);
	Actor_SetSpeed(JASMINE, 0x19999, 0xcccc);
	Actor_WalkTo(8, 0x1e7c, 0xdb8);
	Actor_WalkToAndWait(JASMINE, 0x1e6c, 0xdd8);
	Engine_ActorSetAnimation(8, 1);
	Battle_SetObjectFlag5bWhenMode3();
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(8, 1);
	Event_ShowMessageAndWait(8, 0, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Event_ShowMessageAndWait(0x1005, 0, 40);
	Actor_FaceDirection(8, 0x8000, 20);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 60);
	Map_LoadDefaultCellsAndUpdateBlock();
	Event_PlayCue(17);
	ColorBuffer_ApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(120);
	Engine_TaskWait(120);
	/* FAKEMATCH: Event_SetDestination is called as void (*)(s32, s32); as void (*)(u16, u16), its prototype, FieldScene_RunActorPresentationSequence has 112 of 1146 bytes different */
	((void (*)(s32, s32))Event_SetDestination)((s32)&SceneId_Title, 10);
}

void StoryScene_StartTransition(void)
{
	Engine_EventBegin();
	Engine_AudioPlayCue(141);
	Engine_ColorBufferApplySource(0, 0);
	Engine_ColorBufferApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(2);
	gEventWork->transition_frames = 1;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	Engine_TaskWait(1);
	Engine_CameraSetSpeed(0x40000, 0x8000);
	{
		s32	  transition_delay = 3200;
		void *transition_callback = (void *)StoryScene_UpdateTransitionEffect;
		Engine_TaskAddCallback(transition_callback, transition_delay);
	}
	Engine_ColorBufferApplySource(0, 0);
	Engine_ColorBufferApplyTarget(0x10004, 1);
	Engine_ColorBufferApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(240);
	Engine_ColorBufferApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(80);
	Engine_TaskWait(90);
	Engine_EventRequestExit(109);
	Engine_GameFlagSet(282);
	Engine_EventEnd();
}

void StoryScene_UpdateTransitionEffect(void)
{
	u32				   x = (u32)Engine_RandomNext() * TRANSITION_EFFECT_WIDTH >> 16;
	u32				   z = (u32)Engine_RandomNext() * TRANSITION_EFFECT_DEPTH >> 16;
	struct FieldActor *object;

	object = Engine_ObjectCreate(TRANSITION_EFFECT_TYPE, PIXELS(x) + PIXELS(TRANSITION_EFFECT_LEFT),
		0, PIXELS(z) + PIXELS(TRANSITION_EFFECT_TOP));
	if (object != NULL)
	{
		struct FieldSprite *sprite = object->sprite;
		s32					scale = (((u32)Engine_RandomNext() << 15) >> 16) + 0x13333;

		sprite->flags = 0;
		sprite->priority = 2;
		object->motion_flags = 0;
		object->scale_x = scale;
		object->scale_y = scale;
		Object_SetMode(object, 1);
		Engine_ObjectSetScript(object, gTransitionSparkScript);
	}
	if (gFrameCount % 3 == 0)
	{
		switch (((u32)Engine_RandomNext() << 2) >> 16)
		{
		case 0:
			Engine_CameraMoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c69), 1);
			break;
		case 1:
			Engine_CameraMoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c67), 1);
			break;
		case 2:
			Engine_CameraMoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c69), 1);
			break;
		case 3:
			Engine_CameraMoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c67), 1);
			break;
		}
	}
}

void FieldScene_RunActorEightApproach(void)
{

	u32				   i;
	struct FieldActor *actor;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Engine_ActorSetAnimation(ACTOR, 2);
	Actor_SetPosition(ACTOR, 0x13080000, 0x3280000);
	actor = Actor_Get(ACTOR);
	{
		s32 value = 0xa000;

		actor->facing = value;
	}
	Engine_TaskWait(1);
	BattleFx_ScheduleRatioTransition(0x13333, 1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_CameraFollowActor(ACTOR, 1);
	Engine_TaskWait(1);
	SCENE_PHASE = 0x100;
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR, 0x6666, 0x3333);
	Actor_MoveToAndWait(ACTOR, 0x12d8, 0x2c8);
	Actor_MoveToAndWait(ACTOR, 0x12a8, 0x268);
	Actor_SetSpeed(ACTOR, 0x4ccc, 0x2666);
	Actor_MoveToAndWait(ACTOR, 0x12a8, 0x1d8);
	Actor_SetSpeed(ACTOR, 0x3333, 0x1999);
	Actor_MoveToAndWait(ACTOR, 0x1298, 0x1c8);
	Actor_SetSpeed(ACTOR, 0x1999, 0xccc);
	Actor_MoveToAndWait(ACTOR, 0x1298, 0x1b8);
	Engine_ActorSetAnimation(ACTOR, 1);
	Engine_EventWait(40);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(110);
}

s32 StoryScene_ComputeOpposingSlotDelta(void)
{
	s32 positive_total = StoryReward_LookupBySelection(0);
	s32 negative_total;

	positive_total += StoryReward_LookupBySelection(2);
	negative_total = StoryReward_LookupBySelection(1);
	negative_total += StoryReward_LookupBySelection(3);
	return positive_total - negative_total;
}

s32 StoryReward_LookupBySelection(u32 selection)
{
	s32 flag_base = 0;
	u32 offset;

	switch (selection)
	{
	case 0:
		flag_base = 0x92C;
		break;
	case 1:
		flag_base = 0x935;
		break;
	case 2:
		flag_base = 0x917;
		break;
	case 3:
		flag_base = 0x990;
		break;
	}
	for (offset = 0; offset < 9; offset++)
	{
		if (Engine_GameFlagIsSet(flag_base + offset) != 0)
			return gWorldMapRewards[offset];
	}
	return 0;
}

void WorldMap_SpawnActorEightPuff(void)
{
	struct FieldActor  *leader;
	struct FieldActor  *obj;
	struct FieldSprite *spr;
	u32					value;

	if ((*(s32 *)&gFrameCount & 15) != 0)
		return;
	leader = Object_GetById(8);
	obj = Engine_ObjectCreate(
		222, leader->x.fixed - 0x200000, leader->y.fixed, leader->z.fixed - 0x100000);
	if (obj == 0)
		return;
	obj->scale_x = 0x8000;
	obj->scale_y = 0x8000;
	spr = obj->sprite;
	if (((u32)Engine_RandomNext() * 2 >> 16))
	{
		s32 back = (((u32)Engine_RandomNext() * 48) >> 16) << 16;

		obj->x.fixed -= back >> 1;
		obj->z.fixed -= back;
	}
	else
	{
		value = (((u32)Engine_RandomNext() << 5) >> 16) << 16;
		obj->x.fixed += value;
		value = (s32)value >> 1;
		obj->z.fixed += value;
	}
	spr->flags = 0;
	spr->priority = leader->sprite->priority;
	obj->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	obj->motion_flags = leader->motion_flags;
	ObjectGroup_SetChildValue(obj, 9);
	Object_SetMode(obj, 2);
	Engine_ObjectSetScript(obj, (s32)gActorEightPuffScript);
}

void WorldMap_RunOpeningRise(void)
{
	struct FieldActor *actor;
	s32				   record;

	actor = Object_GetById(8);
	Engine_EventWait(60);
	Engine_EventBegin();
	BattleFx_ScheduleRatioTransition(0x9999, 1);
	actor->scale_x = 0x13333;
	actor->scale_y = 0x13333;
	Engine_CameraFollowActor(8, 1);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Actor_Get(8);
	Engine_ActorSetSpriteFlags(record, 0);
	Actor_SetSpeed(8, 0x6666, 0x3333);
	actor->timer = 0;
	Engine_ActorEnableActionCallback(8, (s32)gOpeningLeaderRise);
	Engine_TaskAddCallback((s32)WorldMap_SpawnActorEightPuff, 0xc80);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	ColorBuffer_ApplyTarget(0x10003, 1);
	gEventWork->transition_frames = 32;
	Engine_EventOpenScreen();
	Engine_EventWait(120);
	BattleFx_ScheduleRatioTransition(0x16666, 0x12c);
	Engine_EventWait(0x10e);
	gEventWork->transition_frames = 16;
	PLTT_BACKDROP = 0x7fff;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{

	struct FieldActor *actor;
	struct EventWork  *scene;
	s32				   progress;

	actor = Object_GetById(gGameState.selected_actor);
	scene = gEventWork;
	actor->facing = (gFrameCount << 12);

	/* FAKEMATCH: GameFlag_GetByte is called as s32 (*)(s32); as u8 (*)(s32), its prototype, StoryScene_UpdateSelectedActorProgress is 4 bytes longer */
	progress = ((s32 (*)(s32))GameFlag_GetByte)(0x2f8);
	if (progress != 0)
	{
		if (progress == 1)
		{
			scene->raised_trigger = 99;
		}
		else if (Engine_GameFlagIsSet(0x106) == 0)
		{
			progress -= 1;
		}
	}
	/* FAKEMATCH: GameFlag_SetByte is called as void (*)(s32, s32); as u32 (*)(u32, u8), its prototype, StoryScene_UpdateSelectedActorProgress is 4 bytes longer */
	((void (*)(s32, s32))GameFlag_SetByte)(0x2f8, progress);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
	s32				   rec2;
	struct FieldActor *actor;
	struct FieldActor *record;
	s32				   id;

	id = gGameState.selected_actor;
	actor = Object_GetById(id);
	rec2 = GameFlag_IsSet(0x2f0);
	if (rec2 == 0)
	{
		Engine_EventBegin();
		Actor_SetAttachedEffect(id, 0x101);
		Engine_ActorSetAnimation(id, 9);
		record = Object_GetById(a0);
		if (record != 0)
		{
			Actor_SetDestination(id, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(id);
		Event_PlayCue(244);
		Engine_TaskAddCallback((s32)StoryScene_UpdateSelectedActorProgress, 0xc80);
		actor->motion_flags = rec2;
		Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x200000, actor->z.fixed);
		Engine_ActorWaitForMove(id);
		actor->velocity_y = rec2;
		actor->motion_flags = 4;
		gGameState.movement_mode = 2;
		GameFlag_Set(0x2f0);
		/* FAKEMATCH: GameFlag_SetByte is called as void (*)(s32, s32); as u32 (*)(u32, u8), its prototype, FieldScene_RunOpeningAuxiliarySequence has 4 of 224 bytes different */
		((void (*)(s32, s32))GameFlag_SetByte)(0x2f8, 180);
		Engine_EventEnd();
		((struct EventRuntime *)gEventWork)->value_17c = rec2;
	}
}

void StoryScene_SetReferenceActor(void)
{
	FieldScene_RunOpeningAuxiliarySequence(54);
}

void StoryScene_ActivateSharedState(void)
{
	gEffectWork->active = 1;
}

void StoryScene_CompleteActor98(void)
{
	struct FieldActor *selected_actor;

	if (gEventWork->raised_trigger == 99)
	{
		gEventWork->raised_trigger = 0;
	}
	Engine_GameFlagClear(0x2f0);
	Engine_GameFlagSet(0x2f1);
	/* FAKEMATCH: GameFlag_SetByte is called as void (*)(s32, s32); as u32 (*)(u32, u8), its prototype, StoryScene_CompleteActor98 has 4 of 116 bytes different */
	((void (*)(s32, s32))GameFlag_SetByte)(0x2f8, 0);
	BattleFx_SetWeightedResult(98, 5);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(98, 7);
	selected_actor = Object_GetById(gGameState.selected_actor);
	selected_actor->motion_flags = 2;
}

void WorldMap_RaiseActors(void)
{
	struct FieldActor *actor = Actor_Get(gGameState.selected_actor);
	struct FieldActor *other = Actor_Get(54);
	s32				   frames;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Event_PlayCue(219);
	Engine_ActorSetSpriteFlags(actor, 0);
	other->motion_flags = 0;
	actor->motion_flags = 0;
	actor->velocity_y = 0;
	actor->unknown_5d[4] = 1;
	other->unknown_5d[4] = 1;
	for (frames = 59; frames >= 0; --frames)
	{
		actor->velocity_y += 0x3333;
		other->velocity_y += 0x3333;
		Engine_TaskWait(1);
	}
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
	GameFlag_Set(0x122);
	/* FAKEMATCH: Event_SetDestination is called as void (*)(s32, s32); as void (*)(u16, u16), its prototype, WorldMap_RaiseActors has 7 of 176 bytes different */
	((void (*)(s32, s32))Event_SetDestination)((s32)&SceneId_WorldMap, 27);
}

void SceneEffect_RestoreBlendRegisters(void)
{

	QueueIoWriteDelay2(0x04000050, 0x3f41);
	if ((gFrameCount & 2) != 0)
	{
		QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x0c);
	}
	else
	{
		QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x10);
	}
}

void FieldScene_RunLateSequence(void)
{

	struct FieldActor *record;
	s32				   sx;
	s32				   sy;
	u32				   mode;

	record = Actor_Get(gGameState.selected_actor);
	sx = record->x.part.pixel;
	sy = record->z.part.pixel;
	if (gFrameCount % 3 == 0)
	{
		mode = (u32)(Random_Next() << 2) >> 16;
		switch (mode)
		{
		case 0:
			Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
			break;
		case 1:
			Engine_CameraMoveTo((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
			break;
		case 2:
			Camera_MoveTo((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
			break;
		case 3:
			Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
			break;
		}
	}
}

void Scene_RunScene371SequenceA(s32 palette)
{
	u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);

	Engine_TaskWait(1);
	Engine_GameFlagClear(0x109);
	Map_ResumeAnimation();
	Resource_DecodeType01(gWorldMapPackedTiles, buffer);
	Resource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
	IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + palette * 32), 0x80000010);
	IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
	Engine_TaskAddCallback(SceneEffect_RestoreBlendRegisters, 0xc80);
	Engine_EventBegin();
	QueueFrame(buffer, 0x3a80);
	Object_GetById(gGameState.selected_actor)->active = 0;
	gEventWork->transition_frames = 16;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_AudioPlayCue(246);

	gWorldMapBlend = 0xe00;
	QueueFrame(buffer, 0x3480);
	Engine_EventWait(2);
	gWorldMapBlend = 0xd00;
	QueueFrame(buffer, 0x2e80);
	Engine_EventWait(2);
	gWorldMapBlend = 0xc00;
	QueueFrame(buffer, 0x2880);
	Engine_EventWait(2);
	gWorldMapBlend = 0xb00;
	QueueFrame(buffer, 0x2280);
	Engine_EventWait(2);
	gWorldMapBlend = 0xa00;
	QueueFrame(buffer, 0x1c80);
	Engine_EventWait(2);
	gWorldMapBlend = 0x900;
	QueueFrame(buffer, 0x1680);
	Engine_EventWait(2);
	gWorldMapBlend = 0x800;
	QueueFrame(buffer, 0x1080);
	Engine_EventWait(140);

	QueueFrame(buffer, 0x1680);
	Engine_EventWait(4);
	QueueFrame(buffer, 0x1c80);
	Engine_EventWait(4);
	QueueFrame(buffer, 0x2280);
	Engine_EventWait(4);
	gWorldMapBlend = 0x900;
	QueueFrame(buffer, 0x2880);
	Engine_EventWait(4);
	gWorldMapBlend = 0xa00;
	QueueFrame(buffer, 0x2e80);
	Engine_EventWait(4);
	gWorldMapBlend = 0xb00;
	QueueFrame(buffer, 0x3480);
	Engine_EventWait(4);
	gWorldMapBlend = 0xc00;
	QueueFrame(buffer, 0x3a80);
	Map_LoadAreaGraphics();
	Engine_TaskAddCallback(FieldScene_RunLateSequence, 0xc80);
	Engine_AudioPlayCue(141);
	gWorldMapBlend = 0xd00;
	Engine_EventWait(4);
	gWorldMapBlend = 0xe00;
	Engine_EventWait(4);
	gWorldMapBlend = 0xf00;
	Engine_EventWait(4);
	gWorldMapBlend = 0x1000;
	Engine_EventWait(45);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	MemFree(buffer);
	Engine_GameFlagSet(0x101);
}

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
	if ((gFrameCount & 2) != 0)
	{
		Engine_ObjectSetPartPalettes(obj, 7);
	}
	else
	{
		Engine_ObjectSetPartPalettes(obj, 0);
	}
	if ((gFrameCount & 15) == 0)
	{
		WorldMap_CreateLinkedEffects(obj);
	}
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
	if ((gFrameCount & 1) != 0)
	{
		Engine_ObjectSetPartPalettes(obj, __umodsi3((gFrameCount >> 1), 6));
	}
	if ((gFrameCount & 15) == 0)
	{
		WorldMap_CreateLinkedEffects(obj);
	}
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
	if ((gFrameCount & 1) != 0)
	{
		s32 slot = (gFrameCount >> 1) % 6;

		Engine_ObjectSetPartPalettes(arg0, slot);
	}
}

void Effect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
	struct StoryVerticalEffectActor *anchor_actor;
	s32								 animation_frame;
	s32								 vertical_amplitude;

	anchor_actor = effect->anchor;
	effect->frame = (effect->frame + 1);
	animation_frame = (s16)effect->frame;

	if (animation_frame > 31)
	{
		Engine_ObjectDispatchRelease(effect);
		return;
	}

	vertical_amplitude = Engine_MathSin(animation_frame << 10);
	effect->amplitude_x = vertical_amplitude;
	effect->amplitude_y = vertical_amplitude;
	effect->x = anchor_actor->x;
	effect->y += 0x10000;
	effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void Effect_AnimateVerticalNegative(struct StoryVerticalEffectActor *effect)
{
	struct StoryVerticalEffectActor *anchor_actor;
	s32								 animation_frame;
	s32								 vertical_amplitude;

	anchor_actor = effect->anchor;
	effect->frame = (effect->frame + 1);
	animation_frame = (s16)effect->frame;

	if (animation_frame > 31)
	{
		Engine_ObjectDispatchRelease(effect);
		return;
	}

	vertical_amplitude = Engine_MathSin(animation_frame << 10);
	effect->amplitude_x = vertical_amplitude;
	effect->amplitude_y = -vertical_amplitude;
	effect->x = anchor_actor->x;
	effect->y += 0x10000;
	effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "linked_effects.c"

s32 gTransferArrive10[] = {
	SC_POS(PIXELS(5848), 0, PIXELS(1576)),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
	SC_SYNC,
	SC_MOVE(PIXELS(5864), 0, PIXELS(1624)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gTransferDepart10[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5868), 0, PIXELS(1624)),
	SC_SYNC,
	SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(PIXELS(5656), 0, PIXELS(1800)),
	SC_END,
};

s32 gTransferReturn10[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 gTransferGather[] = {
	SC_POS(PIXELS(5868), 0, PIXELS(1704)),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(5872), 0, PIXELS(1528)),
	SC_SYNC,
	SC_MOVE(PIXELS(5880), 0, PIXELS(1352)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 gTransferArrive11[] = {
	SC_POS(PIXELS(5848), 0, PIXELS(1576)),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1630)),
	SC_SYNC,
	SC_MOVE(PIXELS(5832), 0, PIXELS(1640)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gTransferDepart11[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5863), 0, PIXELS(1640)),
	SC_SYNC,
	SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(PIXELS(5640), 0, PIXELS(1768)),
	SC_END,
};

s32 gTransferReturn11[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5640), 0, PIXELS(1795)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 gTransferArrive12[] = {
	SC_POS(PIXELS(5848), 0, PIXELS(1576)),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
	SC_SYNC,
	SC_MOVE(PIXELS(5832), 0, PIXELS(1624)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gTransferDepart12[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5863), 0, PIXELS(1624)),
	SC_SYNC,
	SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(PIXELS(5656), 0, PIXELS(1768)),
	SC_END,
};

s32 gTransferReturn12[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5656), 0, PIXELS(1795)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 gTransferArrive13[] = {
	SC_POS(PIXELS(5848), 0, PIXELS(1576)),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1598)),
	SC_SYNC,
	SC_MOVE(PIXELS(5832), 0, PIXELS(1608)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gTransferDepart13[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(5863), 0, PIXELS(1608)),
	SC_SYNC,
	SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(PIXELS(5672), 0, PIXELS(1768)),
	SC_END,
};

s32 gTransferReturn13[] = {
	SC_ARG(3),
	SC_MOVE(PIXELS(5672), 0, PIXELS(1795)),
	SC_SYNC,
	SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 gTransferGuide14[] = {
	SC_SET(SW_SCALE_X, 0x1999),
	SC_SET(SW_SCALE_Y, 0x1999),
	SC_WAIT(1),
	SC_MOVE(PIXELS(5608), 0, PIXELS(1720)),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 655),
	SC_ADD(SW_SCALE_Y, 655),
	SC_WAIT(1),
	SC_LOOP(160, SC_TO(1)),
	SC_END,
};

s32 gBlackOrbLeaderScript[] = {
	SC_SET(0x23, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(6008), 0, PIXELS(3400)),
	SC_SYNC,
	SC_MOVE(PIXELS(6020), 0, PIXELS(3400)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(0x23, 1),
	SC_WAIT(1),
	SC_END,
};

s32 gPresentGuide9[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
	SC_SYNC,
	SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
	SC_SYNC,
	SC_MOVE(PIXELS(7720), 0, PIXELS(3480)),
	SC_SYNC,
	SC_MOVE(PIXELS(7752), 0, PIXELS(3496)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(1),
	SC_END,
};

s32 gPresentGuide8[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
	SC_SYNC,
	SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
	SC_SYNC,
	SC_MOVE(PIXELS(7704), 0, PIXELS(3512)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

s32 gPresentGuide5[] = {
	SC_SET(0x23, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
	SC_SYNC,
	SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
	SC_SYNC,
	SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
	SC_SYNC,
	SC_MOVE(PIXELS(7688), 0, PIXELS(3528)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(0x23, 1),
	SC_WAIT(1),
	SC_END,
};

s32 gTransitionSparkScript[] = {
	SC_WAIT(16),
	SC_FREE,
};

s32 gTransferLeaderIdle[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_LOOP(40, SC_TOP),
	SC_SET(0x23, 1),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_END,
};

s32 gTransferLeaderTurn[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
	SC_SYNC,
	SC_LOOP(40, SC_TOP),
	SC_SET(0x23, 1),
	SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
	SC_SYNC,
	SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
	SC_SYNC,
	SC_END,
};

s32 gOpeningLeaderRise[] = {
	SC_MOVE_BY(PIXELS(24), 0, 0),
	SC_SYNC,
	SC_LOOP(10, SC_TOP),
	SC_SET(0x23, 1),
	SC_MOVE_BY(PIXELS(24), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(PIXELS(24), 0, 0),
	SC_SYNC,
	SC_END,
};

s32 gWorldMapPlacement0[] = {
	SC_CALL(StoryActor_Initialize),
	SC_END,
};

struct SceneEntrance gWorldMapEntrances[] = {
	{ 0, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 5680, 0, 1226, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 5688, 0, 1224, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 6424, 0, 1320, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 6584, 0, 1224, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 6928, 0, 1368, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 7328, 0, 1240, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 7136, 0, 1054, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 5928, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 6024, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 7568, 0, 1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 7880, 0, 1592, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 7852, 0, 1688, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 7544, 0, 2008, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 15, CONDITION_ALWAYS, 7144, 0, 1736, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 16, CONDITION_ALWAYS, 7008, 0, 1624, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 17, CONDITION_ALWAYS, 6888, 0, 1736, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 18, CONDITION_ALWAYS, 6888, 0, 1828, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 19, CONDITION_ALWAYS, 5560, 0, 2136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 20, CONDITION_ALWAYS, 5592, 0, 2168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 21, CONDITION_ALWAYS, 5424, 0, 2264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 22, CONDITION_ALWAYS, 5004, 0, 2328, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 23, CONDITION_ALWAYS, 4904, 0, 2312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 24, CONDITION_ALWAYS, 4840, 0, 1992, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 25, CONDITION_ALWAYS, 4940, 0, 2648, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 26, CONDITION_ALWAYS, 5656, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 27, CONDITION_ALWAYS, 5388, 0, 2840, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 28, CONDITION_ALWAYS, 5448, 0, 2920, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 29, CONDITION_ALWAYS, 5864, 0, 3400, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 30, CONDITION_ALWAYS, 5996, 0, 2930, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 31, CONDITION_ALWAYS, 5996, 0, 2840, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 32, CONDITION_ALWAYS, 6032, 0, 3192, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 33, CONDITION_ALWAYS, 5976, 0, 3432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 34, CONDITION_ALWAYS, 5896, 0, 1160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 35, CONDITION_ALWAYS, 5990, 0, 804, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 38, CONDITION_ALWAYS, 7376, 0, 1240, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 39, CONDITION_ALWAYS, 5956, 0, 266, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 40, CONDITION_ALWAYS, 6484, 0, 1212, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 41, CONDITION_ALWAYS, 5480, 0, 2632, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 42, CONDITION_ALWAYS, 5928, 0, 3272, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 43, CONDITION_ALWAYS, 7792, 0, 2104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 44, CONDITION_ALWAYS, 5848, 0, 1544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 45, CONDITION_ALWAYS, 6248, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 46, CONDITION_ALWAYS, 7568, 0, 1352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 47, CONDITION_ALWAYS, 6976, 0, 1360, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 48, CONDITION_ALWAYS, 6228, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 49, CONDITION_ALWAYS, 5860, 0, 1212, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 50, CONDITION_ALWAYS, 7056, 0, 1656, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 51, CONDITION_ALWAYS, 7000, 0, 1736, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 52, CONDITION_ALWAYS, 6920, 0, 1696, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 53, CONDITION_ALWAYS, 6090, 0, 2232, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 54, CONDITION_ALWAYS, 4940, 0, 2728, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 55, CONDITION_ALWAYS, 5416, 0, 3272, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 56, CONDITION_ALWAYS, 5416, 0, 2808, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 57, CONDITION_ALWAYS, 5444, 0, 2840, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 58, CONDITION_ALWAYS, 5424, 0, 2200, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 59, CONDITION_ALWAYS, 5996, 0, 2856, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 60, CONDITION_ALWAYS, 5480, 0, 2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 61, CONDITION_ALWAYS, 6152, 0, 3212, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 64, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 65, CONDITION_ALWAYS, 5384, 0, 2248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 66, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 67, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 68, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 69, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, 128, -1, -1, 0 },
	{ 70, CONDITION_ALWAYS, 5096, 0, 2328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 71, CONDITION_ALWAYS, 5528, 0, 2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 72, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 73, CONDITION_ALWAYS, 6088, 0, 3176, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 74, CONDITION_ALWAYS, 4728, 0, 424, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 75, CONDITION_ALWAYS, 4872, 0, 808, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 76, CONDITION_ALWAYS, 4576, 0, 508, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 77, CONDITION_ALWAYS, 4392, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 78, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 80, CONDITION_ALWAYS, 7552, 0, 3504, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 90, CONDITION_ALWAYS, 6024, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 91, CONDITION_ALWAYS, 6088, 0, 3168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 97, CONDITION_ALWAYS, 6248, 0, 872, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 98, CONDITION_ALWAYS, 4248, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 99, CONDITION_ALWAYS, 5576, 0, 2088, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gWorldMapExits[] = {
	SCENE_EXITS(2),
	SCENE_EXIT(1, 5, 10),
	SCENE_EXIT(2, 2, 2),
	SCENE_EXIT(3, 20, 1),
	SCENE_EXIT(4, 25, 1),
	SCENE_EXIT(5, 30, 1),
	SCENE_EXIT(6, 35, 1),
	SCENE_EXIT(7, 36, 1),
	SCENE_EXIT(8, 40, 4),
	SCENE_EXIT(9, 50, 1),
	SCENE_EXIT(10, 57, 11),
	SCENE_EXIT(11, 39, 1),
	SCENE_EXIT(12, 60, 1),
	SCENE_EXIT(13, 68, 1),
	SCENE_EXIT(14, 72, 1),
	SCENE_EXIT(15, 74, 1),
	SCENE_EXIT(16, 75, 1),
	SCENE_EXIT(17, 88, 1),
	SCENE_EXIT(18, 89, 1),
	SCENE_EXIT(19, 2, 19),
	SCENE_EXIT(20, 99, 9),
	SCENE_EXIT(21, 107, 1),
	SCENE_EXIT(22, 112, 2),
	SCENE_EXIT(23, 135, 1),
	SCENE_EXIT(24, 146, 1),
	SCENE_EXIT(25, 152, 1),
	SCENE_EXIT(26, 158, 1),
	SCENE_EXIT(27, 167, 10),
	SCENE_EXIT(28, 164, 1),
	SCENE_EXIT(29, 169, 1),
	SCENE_EXIT(30, 178, 20),
	SCENE_EXIT(31, 177, 1),
	SCENE_EXIT(32, 181, 1),
	SCENE_EXIT(33, 171, 4),
	SCENE_EXIT(34, 104, 1),
	SCENE_EXIT(35, 49, 1),
	SCENE_EXIT(40, 29, 2),
	SCENE_EXIT(41, 158, 6),
	SCENE_EXIT(42, 169, 5),
	SCENE_EXIT(43, 70, 13),
	SCENE_EXIT(44, 20, 3),
	SCENE_EXIT(45, 49, 1),
	SCENE_EXIT(46, 39, 2),
	SCENE_EXIT(47, 35, 2),
	SCENE_EXIT(48, 47, 3),
	SCENE_EXIT(49, 106, 1),
	SCENE_EXIT(50, 74, 2),
	SCENE_EXIT(51, 74, 3),
	SCENE_EXIT(52, 87, 4),
	SCENE_EXIT(53, 92, 4),
	SCENE_EXIT(54, 152, 2),
	SCENE_EXIT(55, 165, 2),
	SCENE_EXIT(56, 107, 4),
	SCENE_EXIT(59, 178, 21),
	SCENE_EXIT(60, 153, 2),
	SCENE_EXIT(61, 181, 3),
	SCENE_EXIT(74, 109, 6),
	SCENE_EXIT(75, 113, 1),
	SCENE_EXIT(100, 2, 1),
	SCENE_EXIT(101, 2, 64),
	SCENE_EXIT(102, 110, 11),
	SCENE_EXIT(103, 110, 12),
	SCENE_EXIT(104, 110, 13),
	SCENE_EXIT(105, 111, 21),
	SCENE_EXIT(106, 110, 14),
	SCENE_EXIT(107, 109, 18),
	SCENE_EXIT(108, 153, 10),
	SCENE_EXIT(109, 187, 4),
	SCENE_EXIT(110, 111, 23),
	SCENE_EXIT(111, 2, 80),
	SCENE_EXIT(112, 105, 10),
	SCENE_EXITS_END,
};

s32 gWorldMapPlacement2[] = {
	SC_CALL(StoryActor_ApplyFlaggedMode),
	SC_END,
};

s32 gWorldMapPlacement1[] = {
	SC_CALL(StoryActor_ApplyMapRotation),
	SC_END,
};

s32 gWorldMapPlacement4[] = {
	SC_CALL(StoryActor_ApplyMapRotationWithCollision),
	SC_END,
};

s32 gWorldMapPlacement3[] = {
	SC_CALL(StoryActor_ResetPosition),
	SC_CALL(StoryActor_ClearActiveFlag),
	SC_END,
};

struct ScenePlacement gWorldMapPlacements[] = {
	{ 0x5a, 0x33, (s32)gWorldMapPlacement0,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x5c, 0x5a, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x5b, 0x48, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5678), 0, PIXELS(1200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6976), 0, PIXELS(1624), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5416), 0, PIXELS(2840), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5036), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x109, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5928), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x108, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7352), 0, PIXELS(1240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7544), 0, PIXELS(1976), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6888), 0, PIXELS(1712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7880), 0, PIXELS(1560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6584), 0, PIXELS(1192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5996), 0, PIXELS(2894), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(4904), 0, PIXELS(2280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10d, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6024), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10f, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5656), 0, PIXELS(1112), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x114, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6088), 0, PIXELS(3176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10e, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6952), 0, PIXELS(1362), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x110, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5996), 0, PIXELS(2808), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6424), 0, PIXELS(1302), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6484), 0, PIXELS(1184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(4840), 0, PIXELS(1960), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5656), 0, PIXELS(2392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5480), 0, PIXELS(2600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7790), 0, PIXELS(2080), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5976), 0, PIXELS(3400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5988), 0, PIXELS(774), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6224), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6920), 0, PIXELS(1664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6092), 0, PIXELS(2200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7166), 0, PIXELS(1054), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7856), 0, PIXELS(1736), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x116, CONDITION_ALWAYS, (s32)gWorldMapPlacement2,
		PIXELS(7568), 0, PIXELS(1384), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x117, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(4942), 0, PIXELS(2690), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f5, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6036), 0, PIXELS(3458), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xa1, IF_FLAG_SET(0x8a0), (s32)gWorldMapPlacement1,
		PIXELS(4760), 0, PIXELS(440), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x2f1, (s32)gWorldMapPlacement3,
		PIXELS(4392), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x39, CONDITION_ALWAYS, (s32)gWorldMapPlacement4,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
	{ GERALD, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements64[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x31, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements49[] = {
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x44, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x31, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements65[] = {
	{ 0xa1, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5384), 0, PIXELS(2248), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5028), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements66[] = {
	{ 0xa1, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5432), 0, PIXELS(2312), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements71[] = {
	{ 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements80[] = {
	{ JASMINE, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7560), 0, PIXELS(3512), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7816), 0, PIXELS(3528), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x1e, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7544), 0, PIXELS(3496), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x2b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7708), 0, PIXELS(3456), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x23, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7800), 0, PIXELS(3544), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x1f5, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7604), 0, PIXELS(3522), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements72[] = {
	{ 0x1f8, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(7464), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements73[] = {
	{ 0x114, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
		PIXELS(6080), 0, PIXELS(3168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gWorldMapEvents[] = {
	{ EVENT_TOUCH, 90, 0x30, EVENT_SCRIPT(WorldMap_MeetVenusDjinni) },
	{ EVENT_TOUCH, 95, 0x2f1, EVENT_SCRIPT(StoryScene_SetReferenceActor) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(StoryProgress_TriggerEvent0808) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(StoryProgress_TriggerEvent0809) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(StoryProgress_TriggerEvent080A) },
	{ EVENT_TOUCH, 92, 0x33, EVENT_SCRIPT(StoryProgress_TriggerEvent0808) },
	{ EVENT_TOUCH, 91, 0x5a, EVENT_SCRIPT(StoryProgress_TriggerEvent0809) },
	{ EVENT_TOUCH, 93, 0x48, EVENT_SCRIPT(StoryProgress_TriggerEvent080A) },
	{ SEARCH(SEARCH_UNNAMED), 96, CONDITION_ALWAYS, EVENT_SCRIPT(StoryScene_ShowRewardDialogue) },
	{ 0xf204, 96, 0x85d, EVENT_SCRIPT(WorldMap_UseBlackOrb) },
	{ SEARCH(SEARCH_GROUND), 100, 0xfcf, FIND_ITEM(0x103) },
	{ EVENT_TALK, 55, CONDITION_ALWAYS, EVENT_SCRIPT(WorldMap_PromptBlackOrbUse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x437, CONDITION_ALWAYS, EVENT_SCRIPT(WorldMap_PromptBlackOrbUse) },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 50, CONDITION_ALWAYS, 50 },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 51 },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 55 },
	{ EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
	{ EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
	{ EVENT_EXIT, 32, CONDITION_ALWAYS, 32 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 61 },
	{ EVENT_EXIT, 47, CONDITION_ALWAYS, 47 },
	{ EVENT_EXIT, 111, CONDITION_ALWAYS, 1 },
	{ EVENT_TOUCH, 112, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyFlag85aBranch) },
	{ EVENT_EXIT, 132, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 133, CONDITION_ALWAYS, 40 },
	{ EVENT_EXIT, 123, CONDITION_ALWAYS, 5 },
	{ EVENT_TOUCH, 130, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValues130_6_47) },
	{ EVENT_EXIT, 119, CONDITION_ALWAYS, 7 },
	{ EVENT_TOUCH, 150, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues150And46And11) },
	{ EVENT_EXIT, 144, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 139, CONDITION_ALWAYS, 35 },
	{ EVENT_EXIT, 140, CONDITION_ALWAYS, 48 },
	{ EVENT_EXIT, 118, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 127, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 122, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 145, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 137, CONDITION_ALWAYS, 43 },
	{ EVENT_EXIT, 120, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 113, CONDITION_ALWAYS, 16 },
	{ EVENT_EXIT, 142, CONDITION_ALWAYS, 52 },
	{ EVENT_EXIT, 121, CONDITION_ALWAYS, 17 },
	{ EVENT_EXIT, 143, CONDITION_ALWAYS, 53 },
	{ EVENT_EXIT, 124, CONDITION_ALWAYS, 20 },
	{ EVENT_EXIT, 115, CONDITION_ALWAYS, 34 },
	{ EVENT_EXIT, 141, CONDITION_ALWAYS, 49 },
	{ EVENT_TOUCH, 116, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues116And56And21) },
	{ EVENT_EXIT, 117, CONDITION_ALWAYS, 22 },
	{ EVENT_EXIT, 126, CONDITION_ALWAYS, 23 },
	{ EVENT_EXIT, 134, CONDITION_ALWAYS, 24 },
	{ EVENT_TOUCH, 151, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues151And25And54) },
	{ EVENT_EXIT, 146, CONDITION_ALWAYS, 60 },
	{ EVENT_EXIT, 135, CONDITION_ALWAYS, 26 },
	{ EVENT_EXIT, 136, CONDITION_ALWAYS, 41 },
	{ EVENT_EXIT, 114, CONDITION_ALWAYS, 27 },
	{ EVENT_EXIT, 138, CONDITION_ALWAYS, 33 },
	{ EVENT_EXIT, 131, CONDITION_ALWAYS, 31 },
	{ EVENT_TOUCH, 125, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep7D3B1E) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 74, IF_FLAG_SET(0x8a0), EVENT_SCRIPT(FieldScene_RunStep74) },
	{ 0xc401, 75, CONDITION_ALWAYS, 75 },
	{ 0x10002115, 54, IF_FLAG_SET(0x2f0), EVENT_SCRIPT(StoryScene_ActivateSharedState) },
	{ PSYNERGY_ON_ACTOR(0x21), 54, IF_FLAG_SET(0x2f0), EVENT_SCRIPT(StoryScene_CompleteActor98) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(WorldMap_RaiseActors) },
	{ SCENE_EVENTS_END },
};

u32 gWorldMapRewards[] = {
	7, 5, 8, 4, 9, 12, 6, 3,
	0,
};

s32 gActorEightPuffScript[] = {
	SC_ADD(SW_X, -0x1999),
	SC_ADD(SW_Z, 0x1999),
	SC_ADD(SW_SCALE_X, 3584),
	SC_ADD(SW_SCALE_Y, -384),
	SC_WAIT(1),
	SC_LOOP(48, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};
