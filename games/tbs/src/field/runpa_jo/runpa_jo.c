/*----------------------------------------------------------
    ルンパ城  イベント
----------------------------------------------------------*/
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
#include "edition.h"
#include "fortress.h"
#include "event.h"
#include "scene_ids.h"
#include "tbs_edition.h"
#include "map_scroll.h"
#include "text/msg_ids.h"

void Engine_TaskWait(s32 frames);

void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);

void Engine_ActorWaitForMove(s32 actor);

void Engine_MapCopyCellAttributes(
	s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);

void Object_RefreshSelectorById(s32 actor);

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_RunpaJo1)
	{
		return gRunpaJoEntrances1;
	}
	if (scene == (s32)&SceneId_RunpaJo2)
	{
		return gRunpaJoEntrances2;
	}
	if (scene == (s32)&SceneId_RunpaJo3)
	{
		return gRunpaJoEntrances3;
	}
	return gRunpaJoEntrancesOther;
}

s32 GetEmptySceneData(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_RunpaJo2)
	{
		return gRunpaJoExits2;
	}
	if (scene == (s32)&SceneId_RunpaJo3 || scene == (s32)&SceneId_RunpaJo4)
	{
		return gRunpaJoExits3And4;
	}
	return gRunpaJoExitsOther;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_RunpaDou)
	{
		return gRunpaJoPlacementsRunpaDou;
	}
	if (scene == (s32)&SceneId_RunpaJo3)
	{
		return gRunpaJoPlacements3;
	}
	if (scene == (s32)&SceneId_RunpaJo2)
	{
		return gRunpaJoPlacements2;
	}
	if (scene == (s32)&SceneId_RunpaJo1)
	{
		return gRunpaJoPlacements1;
	}
	if (scene == (s32)&SceneId_RunpaJo4)
	{
		return gRunpaJoPlacements4;
	}
	return gRunpaJoPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_RunpaJo1)
	{
		return gRunpaJoEvents1;
	}
	if (scene == (s32)&SceneId_RunpaJo2)
	{
		return gRunpaJoEvents2;
	}
	if (scene == (s32)&SceneId_RunpaJo3)
	{
		return gRunpaJoEvents3;
	}
	return gRunpaJoEventsOther;
}

void ConfigureSceneActor12(void)
{

	s32				   actor_slot = 15;
	struct FieldActor *actor;

	Engine_MapCopyCellAttributes(15, 20, 1, 1, actor_slot, 22);
	Engine_MapCopyCellAttributes(17, 23, 1, 3, actor_slot, 23);
	actor = Object_GetById(12);
	if (actor != 0)
	{
		Engine_ActorSetSpriteFlags(actor, 0);
		actor->motion_flags = 0;
		actor->priority_flags = 2;
	}
}

void EV70_INIT(void)
{

	StagedActor_AdvancePair();
}

void EV50_START(void)
{

	OBJ_SPD(12, 0x10000, 0x8000);
	Actor_SetDestination(12, 248, 0x178);
	Engine_ActorWaitForMove(12);
	SE(215);
	WAIT(60);
	ConfigureSceneActor12();
	FLAG_SET(0x943);
}

void EV12_MOVE(void)
{
	struct FieldActor *actor;

	actor = Actor_Get(12);
	if ((actor->z.fixed >> 20) > 22)
	{
		Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		SE(144);
		Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
		Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
		actor = (struct FieldActor *)(s32)Actor_Get(12);
		if (actor != NULL)
		{
			Engine_ActorSetSpriteFlags(actor, 0);
			actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		}
		FLAG_SET(0x943);
	}
}

void PlaceActorTwelveAndFinishScene(void)
{
	OBJ_POS_SET(12, 248, 376);
	ConfigureSceneActor12();
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{

	s32 position_x = gRunpaJoPairTableA[table_index * 2];
	s32 position_z = gRunpaJoPairTableA[table_index * 2 + 1];

	Engine_MapCopyCells(0, 0x4d, 1, 3, position_x, position_z);
	Engine_MapCopyCells(1, 0x4d, 1, 1, position_x + 1, position_z);
	Engine_MapCopyCells(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void EV40_A(void)
{

	struct EventWork *work;
	s32				  trigger;
	s32				  index;

	work = gEventWork;
	if (Engine_PartyInventoryFindOwner(234) != -1)
	{
		trigger = work->touched_trigger;
		index = trigger - 40;
		if (FLAG_CHK(0x941) == 0 || index != 4)
		{
			PlaceSceneObjectPairFromTableA(index);
			SE(157);
			Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			FLAG_SET(trigger + 0x328);
		}
	}
}

void FieldScene_SetPositionPairs(s32 idx)
{
	s32 top_x;
	s32 top_y;
	s32 bottom_y;

	top_x = gRunpaJoSupportPairs[idx * 2];
	top_y = gRunpaJoSupportPairs[idx * 2 + 1];
	Map_CopyCells(0, 77, 1, 3, top_x, top_y);
	Map_CopyCells(1, 77, 1, 1, top_x + 1, top_y);
	bottom_y = top_y - 44;
	Map_CopyCellAttributes(top_x, top_y - 45, 1, 1, top_x, bottom_y);
	if (idx == 1)
		Map_CopyCellAttributes(top_x, bottom_y, 1, 1, top_x, top_y - 43);
}

void EV40_B(void)
{

	struct EventWork *work;
	s32				  trigger;

	work = gEventWork;
	if (Engine_PartyInventoryFindOwner(234) != -1)
	{
		trigger = work->touched_trigger;
		FieldScene_SetPositionPairs(trigger - 40);
		SE(157);
		Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		FLAG_SET(trigger + 0x32d);
	}
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{

	s32 position_x = gRunpaJoPairTableB[table_index * 2];
	s32 position_z = gRunpaJoPairTableB[table_index * 2 + 1];

	Engine_MapCopyCells(0x37, 0x79, 1, 3, position_x, position_z);
	Engine_MapCopyCells(0x38, 0x79, 1, 1, position_x + 1, position_z);
	Engine_MapCopyCells(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void EV40_D(void)
{

	struct EventWork *work;
	s32				  trigger;

	work = gEventWork;
	if (Engine_PartyInventoryFindOwner(234) != -1)
	{
		trigger = work->touched_trigger;
		PlaceSceneObjectPairFromTableB(trigger - 40);
		SE(157);
		Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		FLAG_SET(trigger + 0x330);
	}
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{

	s32 position_x = gRunpaJoPairTableC[table_index * 2];
	s32 position_z = gRunpaJoPairTableC[table_index * 2 + 1];

	Engine_MapCopyCells(1, 0x50, 1, 3, position_x, position_z);
	Engine_MapCopyCells(2, 0x50, 1, 1, position_x + 1, position_z);
	Engine_MapCopyCellAttributes(
		position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void EV40_C(void)
{

	struct EventWork *work;
	s32				  trigger;

	work = gEventWork;
	if (Engine_PartyInventoryFindOwner(234) != -1)
	{
		trigger = work->touched_trigger;
		PlaceSceneObjectPairFromTableC(trigger - 40);
		SE(157);
		Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		FLAG_SET(trigger + 0x332);
	}
}

void EV99_ROUYA(void)
{
	if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) == -1)
	{
		Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
	}
}

void EV98_TOBIRA(void)
{
	Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
}

void Actor8_Interact(void)
{
	if (TryStartActorInteraction(8, 8) != 0)
	{
		Engine_GameFlagSet(0xf2a);
	}
}

void Actor9_Interact(void)
{
	if (TryStartActorInteraction(9, 7) != 0)
	{
		Engine_GameFlagSet(0xf2b);
	}
}

void Actor10_Interact(void)
{
	if (TryStartActorInteraction(10, 6) != 0)
	{
		Engine_GameFlagSet(0xf2c);
	}
}

void Actor11_Interact(void)
{
	if (TryStartActorInteraction(11, 5) != 0)
	{
		Engine_GameFlagSet(0xf2d);
	}
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
	s32 started = 0;
	s32 interaction;

	EV_BEGIN();
	interaction = Engine_BattleFxPlayCueAndStartEmitterOnTarget(0, actor_id, interaction_id);
	if (Engine_PartyGiveItem(interaction_id, 0) != -1)
	{
		OBJ_MODE(actor_id, 2);
		started = 1;
	}
	else
	{
		SE(0x7d);
		OBJ_MODE(actor_id, 5);
	}
	Engine_ObjectDispatchRelease(interaction);
	EV_END();
	return started;
}

void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void EV08_KAGI(void)
{

	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x108, 0x318);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	WAIT(10);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	Engine_ItemShowFound(ITEM_CELL_KEY, 3);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
	Engine_GameFlagSet(0xf2e);
	Engine_ActorSetPosition(8, 0, 0);
}

s32 IsPlayerInAccidentTriggerArea(void)
{
	SceneActor *player = Object_GetById(0);
	s32			z = player->z;
	s32			x;
	s32			zz, xx;

	if (z < 0)
	{
		z += 0xfffff;
	}
	x = player->x;
	zz = z >> 20;
	if (x < 0)
	{
		x += 0xfffff;
	}
	xx = x >> 20;
	if ((u32)(zz - 5) <= 2 && xx <= 10)
	{
		return 1;
	}
	if ((u32)(xx - 8) <= 1 && zz > 22)
	{
		return 1;
	}
	return 0;
}

void FieldScene_UpdateActorPairInteraction(void)
{
	struct ObjectRuntime  *actor = Actor_Get(9);
	struct ObjectRuntime  *other = Actor_Get(10);
	struct MapLayerScroll *layer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	s16					  *scene = *(s16 **)&gMapWork[19];

	if (gFrameCount & 1)
	{
		layer->speed_x = 1;
		layer->speed_y = 1;
	}
	else
	{
		layer->speed_x = -1;
		layer->speed_y = -1;
	}
	if (FLAG_CHK(0x106) || scene[191] != 0 || scene[192] != 0)
	{
		actor->movement_state = 1;
		other->movement_state = 1;
	}
	else if (!FLAG_CHK(0x214))
	{
		actor->movement_state = 0;
		other->movement_state = 0;
		if (!FLAG_CHK(0x214) && actor->movement_state == 0)
		{
			layer->phase_x = 8912896.0 - actor->x;
		}
		if (!IsPlayerInAccidentTriggerArea())
		{
			if (gGameState.cloaked != 0)
			{
				if (IsSceneActorWithinFourSteps(9) && gGameState.cloaked != 0)
				{
					SetSceneValue(&scene[191], 0x2092);
					return;
				}
				if (IsSceneActorWithinFourSteps(10) && gGameState.cloaked != 0)
				{
					SetSceneValue(&scene[191], 0x2092);
					return;
				}
			}
			if (gGameState.cloaked == 0)
			{
				if (IsActorInteractionAvailable(9))
				{
					FLAG_SET(0x215);
					FLAG_SET(0x214);
				}
				if (IsActorInteractionAvailable(10))
				{
					FLAG_SET(0x215);
					FLAG_SET(0x214);
				}
			}
			if (FLAG_CHK(0x214))
			{
				SetSceneValue(&scene[193], 91);
			}
		}
	}
}

void EV91_SET(void)
{
	EV_BEGIN();
	Engine_ActorStop(9);
	Engine_ActorSetDestinationOffset(9, 0, 0);
	OBJ_MODE(9, 0);
	OBJ_FACE(9, ACTOR_PARTY_LEADER, 0);
	Engine_ActorShowEmote(9, 256, 0);
	RunActorScriptedSequenceA(10);
	EV_END();
}

s32 AreSceneActorsInPassingLane(void)
{
	SceneActor *player = Object_GetById(0);
	SceneActor *passing_actor = Object_GetById(17);
	s32			ox = player->x;
	s32			pz;
	s32			px;
	s32			oxx, pzz, pxx;

	if (ox < 0)
	{
		ox += 0xfffff;
	}
	oxx = ox >> 20;
	pz = passing_actor->z;
	if (pz < 0)
	{
		pz += 0xfffff;
	}
	px = passing_actor->x;
	pzz = pz >> 20;
	if (px < 0)
	{
		px += 0xfffff;
	}
	pxx = px >> 20;
	if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40)
	{
		return 1;
	}
	if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40)
	{
		return 1;
	}
	return 0;
}

void FieldScene_UpdateActorSeventeenInteraction(void)
{

	struct ObjectRuntime  *actor = Actor_Get(17);
	struct MapLayerScroll *layer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	s16					  *scene = *(s16 **)&gMapWork[19];

	Engine_EventGetViewCenter();
	if (gFrameCount & 1)
	{
		layer->speed_x = 1;
		layer->speed_y = 1;
	}
	else
	{
		layer->speed_x = -1;
		layer->speed_y = -1;
	}
	if (FLAG_CHK(0x106) || scene[191] != 0 || scene[192] != 0)
	{
		actor->movement_state = 1;
	}
	else if (!FLAG_CHK(0x214))
	{
		actor->movement_state = 0;
		if (!FLAG_CHK(0x214) && actor->movement_state == 0)
		{
			layer->phase_x = 0x3400000 - actor->x;
			layer->phase_y = 0x2400000 - actor->z;
		}
		if (!AreSceneActorsInPassingLane())
		{
			IsSceneActorWithinTriggerBox(17);
			if (IsSceneActorWithinFourSteps(17) && gGameState.cloaked != 0)
			{
				SetSceneValue(&scene[191], 0x2092);
				return;
			}
			if (gGameState.cloaked == 0)
			{
				if (IsActorInteractionAvailable(17))
				{
					FLAG_SET(0x215);
					FLAG_SET(0x214);
				}
			}
			if (FLAG_CHK(0x214))
			{
				SetSceneValue(&scene[193], 92);
			}
		}
	}
}

void EV92_ON(void)
{
	RunActorScriptedSequenceA(17);
	EV_END();
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
	SceneActor *player = Object_GetById(0);
	s32			zz = player->z / 0x100000;
	s32			xx = player->x / 0x100000;

	if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28)
	{
		return 1;
	}
	if (xx == 41 && zz > 37 && zz <= 41)
	{
		return 1;
	}
	if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40)
	{
		return 1;
	}
	return 0;
}

void FieldScene_UpdateActorEighteenInteraction(void)
{

	struct ObjectRuntime  *actor = Actor_Get(18);
	struct MapLayerScroll *layer = &((struct MapScrollWork *)gMapWork[0])->layers[2];
	s16					  *scene = *(s16 **)&gMapWork[19];

	if (gFrameCount & 1)
	{
		layer->speed_x = 1;
		layer->speed_y = 1;
	}
	else
	{
		layer->speed_x = -1;
		layer->speed_y = -1;
	}
	if (FLAG_CHK(0x106) || scene[191] != 0 || scene[192] != 0)
	{
		actor->movement_state = 1;
	}
	else if (!FLAG_CHK(0x214))
	{
		actor->movement_state = 0;
		if (!FLAG_CHK(0x214) && actor->movement_state == 0)
		{
			layer->phase_x = 0x2f00000 - actor->x;
			layer->phase_y = 0x1f00000 - actor->z;
		}
		if (!IsPlayerInSecondaryTriggerArea())
		{
			if (IsSceneActorWithinFourSteps(18) && gGameState.cloaked != 0)
			{
				SetSceneValue(&scene[191], 0x2092);
				return;
			}
			if (gGameState.cloaked == 0)
			{
				if (IsActorInteractionAvailable(18))
				{
					FLAG_SET(0x215);
					FLAG_SET(0x214);
				}
			}
			if (FLAG_CHK(0x214))
			{
				SetSceneValue(&scene[193], 93);
			}
		}
	}
}

void EV93_ON(void)
{
	RunActorScriptedSequenceA(18);
	EV_END();
}

s32 IsPlayerOutsideSceneRectangle(void)
{
	SceneActor *player = Object_GetById(0);
	s32			zz = player->z / 0x100000;
	s32			xx = player->x / 0x100000;

	if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16)
	{
		return 0;
	}
	return 1;
}

void FieldScene_RunScene3bfSequenceA(void)
{

	struct EventWork *work;

	work = gEventWork;
	if (FLAG_CHK(0x214) == 0)
	{
		if (IsPlayerOutsideSceneRectangle() == 0)
		{
			if (gGameState.cloaked == 0)
			{
				if (IsActorInteractionAvailable(17) != 0)
				{
					FLAG_SET(0x215);
					FLAG_SET(0x214);
				}
			}
			if (FLAG_CHK(0x214) != 0)
			{
				work->raised_trigger = 94;
			}
		}
	}
}

void EV94_MAIN(void)
{
	RunActorScriptedSequenceA(17);
	EV_END();
}

void TriggerSceneStage95FromActor12(void)
{

	struct EventWork *scene_state = gEventWork;

	if (IsActorInteractionAvailable(12) != 0 && gGameState.cloaked == 0)
	{
		s16 *scene_stage;
		s32	 next_stage;

		Engine_TaskRemoveCallback(TriggerSceneStage95FromActor12);
		scene_stage = &scene_state->raised_trigger;
		next_stage = 95;
		*scene_stage = next_stage;
	}
}

void FieldScene_RunScene3bfSequenceB(void)
{

	struct EventWork *work;

	work = gEventWork;
	if (FLAG_CHK(0x225) == 0)
	{
		if (IsActorInteractionAvailable(13) != 0)
		{
			if (gGameState.cloaked == 0)
			{
				FLAG_SET(0x225);
				Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
				Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
				work->raised_trigger = 96;
			}
		}
	}
}

void FieldScene_RunScene3bfSequenceC(void)
{

	struct EventWork *work;

	work = gEventWork;
	if (FLAG_CHK(0x225) == 0)
	{
		if (IsActorInteractionAvailable(21) != 0)
		{
			if (gGameState.cloaked == 0)
			{
				FLAG_SET(0x225);
				Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
				Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
				work->raised_trigger = 96;
			}
		}
	}
}

s32 IsSceneActorVerticallyNearPlayer(s32 actor_id)
{
	SceneActor *scene_actor = Object_GetById(actor_id);
	SceneActor *player = Object_GetById(0);
	s32			actor_z = scene_actor->z / 0x100000;
	s32			actor_x = scene_actor->x / 0x100000;
	s32			player_z = player->z / 0x100000;
	s32			player_x = player->x / 0x100000;
	s32			z_distance = actor_z - player_z;

	if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x)
	{
		return 1;
	}
	return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(s32 actor_id)
{
	SceneActor *scene_actor = Object_GetById(actor_id);
	SceneActor *player = Object_GetById(0);
	s32			actor_z = scene_actor->z / 0x100000;
	s32			actor_x = scene_actor->x / 0x100000;
	s32			player_z = player->z / 0x100000;
	s32			player_x = player->x / 0x100000;
	s32			x_distance = actor_x - player_x;

	if (x_distance < -6 || x_distance > 6)
	{
		return 0;
	}
	if (actor_z - 2 < player_z && actor_z + 2 > player_z)
	{
		return 1;
	}
	return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
	if (IsSceneActorWithinTriggerBox(actor_id) == 0)
	{
		return 0;
	}
	if (IsSceneActorVerticallyNearPlayer(actor_id) != 0)
	{
		return 1;
	}
	{
		s32 result = IsSceneActorHorizontallyNearPlayer(actor_id);

		return (u32)(result | -result) >> 31;
	}
}

s32 IsSceneActorWithinFourSteps(s32 actor_id)
{
	SceneActor *scene_actor = Object_GetById(actor_id);
	SceneActor *player = Object_GetById(0);
	s32			actor_z = scene_actor->z / 0x100000;
	s32			actor_x = scene_actor->x / 0x100000;
	s32			player_z = player->z / 0x100000;
	s32			player_x = player->x / 0x100000;
	s32			x_distance = actor_x - player_x;
	s32			z_distance;

	actor_z += 1;
	if (x_distance < 0)
	{
		x_distance = -x_distance;
	}
	z_distance = actor_z - player_z;
	if (z_distance < 0)
	{
		z_distance = -z_distance;
	}
	if (x_distance + z_distance <= 4)
	{
		return 1;
	}
	return 0;
}

s32 IsSceneActorWithinTriggerBox(s32 actor_id)
{
	SceneActor *scene_actor = Object_GetById(actor_id);
	SceneActor *player = Engine_EventGetViewCenter();
	s32			actor_x = scene_actor->x / 0x100000;
	s32			actor_z = scene_actor->z / 0x100000;
	s32			player_x = player->x / 0x100000;
	s32			player_z = player->z / 0x100000;
	s32			x_distance = actor_x - player_x;
	s32			z_distance;

	if (x_distance < 0)
	{
		x_distance = -x_distance;
	}
	z_distance = actor_z - player_z;
	if (z_distance < 0)
	{
		z_distance = -z_distance;
	}
	if (x_distance > 7 || z_distance > 5)
	{
		return 0;
	}
	return 1;
}

void TriggerScene41AtVillagePath(void)
{

	SceneActor *player = Object_GetById(0);

	if (Engine_GameFlagIsSet(859) == 0)
	{
		s32 player_x = player->x / 0x100000;
		s32 player_z = player->z / 0x100000;

		if (player_x == 43 && player_z > 28 && player_z <= 31)
		{
			s16 *q = &gEventWork->touched_trigger;
			s32	 v = 41;

			*q = v;
			EV40_C();
		}
	}
}

void TriggerScene40AtVillagePath(void)
{

	DirectionalSceneActor *player = Object_GetById(0);

	if (Engine_GameFlagIsSet(856) == 0)
	{
		s32 player_x = player->x / 0x100000;
		s32 player_z = player->z / 0x100000;

		if (player_x == 16 && player_z > 55 && player_z <= 58
			&& (player->dir == 0xc000 || player->dir == 0x4000))
		{
			s16 *q = &gEventWork->touched_trigger;
			s32	 v = 40;

			*q = v;
			EV40_D();
		}
	}
}

void RunActor9ScriptedSequence(void)
{
	EV_BEGIN();
	Engine_ActorSetDestinationOffset(9, 0, 0);
	Engine_ActorEnableActionCallback(9, 1);
	Engine_ActorStop(9);
	OBJ_MODE(9, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
	{
		s32 t = (s32)MsgRunpaWho2;

		MES_SET(t);
		MES_PUT(9, 0);
		Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 60);
		MES_SET(t + 1);
	}
	MES_PUT(9, 0);
	Engine_EventRequestExit(60);
	Engine_EventCloseScreen();
	EV_END();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
	EV_BEGIN();
	EV_BEGIN();
	Engine_ActorShowEmote(actor_id, 256, 1);
	Engine_ActorSetDestinationOffset(actor_id, 0, 0);
	Engine_ActorEnableActionCallback(actor_id, 1);
	OBJ_MODE(actor_id, 0);
	OBJ_FACE(actor_id, ACTOR_PARTY_LEADER, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetDestinationOffset(actor_id, 0, 0);
	Engine_ActorEnableActionCallback(actor_id, 1);
	Engine_ActorStop(actor_id);
	OBJ_MODE(actor_id, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
	{
		s32 t = (s32)MsgRunpaWho2;

		MES_SET(t);
		MES_PUT(actor_id, 0);
		OBJ_FACE(ACTOR_PARTY_LEADER, actor_id, 0);
		Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 60);
		MES_SET(t + 1);
	}
	MES_PUT(actor_id, 0);
	Engine_EventCloseScreen();
	WAIT(60);
	Engine_EventRequestExit(60);
	EV_END();
}

void TurnActorToSceneDirection(s32 actor_id)
{

	OBJ_FACE(actor_id, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(ACTOR_PARTY_LEADER, actor_id, 0);
	switch ((s32)gRunpaJoRandomPick & 3)
	{
	case 0:
		RunActorScriptedSequenceB(actor_id);
		break;
	case 1:
		RunActorScriptedSequenceC(actor_id);
		break;
	case 2:
		RunpaJo_ActOutGuardShiftDialogue(actor_id);
		break;
	case 3:
		RunActorScriptedSequenceD(actor_id);
		break;
	default:
		RunActorScriptedSequenceC(actor_id);
		break;
	}
}

void RunActorScriptedSequenceB(s32 handle)
{
	s32 id;

	Engine_ActorRunRepeatedMotion(handle, 1);
	id = (s32)MsgRunpaIntruder;
	MES_SET(id);
	MES_PUT(handle, 0);
	OBJ_EMO(handle, 258, 60);
	MES_SET(id + 1);
	MES_PUT(handle, 0);
	id += 2;
	Engine_ActorSetAnimationAndWait(handle, 4);
	MES_SET(id);
	MES_PUT(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
	u8 *t = MsgRunpaGuardWhosThat;

	MES_SET((s32)t);
	MES_PUT(actor_id, 0);
	Engine_ActorRunRepeatedMotion(actor_id, 1);
	MES_SET((s32)(t + 1));
	MES_PUT(actor_id, 0);
	Engine_ActorSetAnimationAndWait(actor_id, 4);
	MES_SET((s32)(t + 2));
	MES_PUT(actor_id, 0);
}

void RunpaJo_ActOutGuardShiftDialogue(s32 a0)
{
	u32 i;
	s32 record;
	s32 intruder;

	intruder = (s32)MsgRunpaShiftAlready;
	MES_SET(intruder);
	MES_PUT(a0, 0);
	WAIT(120);
	OBJ_EMO(a0, 0x101, 60);
	MES_SET((intruder + 1));
	MES_PUT(a0, 0);
	Engine_ActorRunRepeatedMotion(a0, 1);
	MES_SET((intruder + 2));
	MES_PUT(a0, 0);
	Engine_ActorSetAnimationAndWait(a0, 4);
	MES_SET((intruder + 3));
	MES_PUT(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
	u8 *t = (s32)MsgRunpaScoundrel;

	MES_SET((s32)t);
	MES_PUT(actor_id, 0);
	Engine_ActorSetAnimationAndWait(actor_id, 4);
	MES_SET((s32)(t + 1));
	MES_PUT(actor_id, 0);
	Engine_ActorRunRepeatedMotion(actor_id, 1);
	MES_SET((s32)(t + 2));
	MES_PUT(actor_id, 0);
	Engine_ActorSetAnimationAndWait(actor_id, 3);
	MES_SET((s32)(t + 3));
	MES_PUT(actor_id, 0);
}

void EV33_SHIRABE(void)
{

	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	SE(113);
	Engine_ActorShowEmote(15, 256, 60);
	TurnActorToSceneDirection(15);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Battle_SetStart(3);
	Engine_BattleFxSetWeightedResult(98, 2);
	Engine_ActorSetPosition(15, 0, 0);
	EV_END();
	Engine_GameFlagSet(2380);
}

void EV30_TAKARABAKO(void)
{

	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	SE(113);
	Engine_ActorShowEmote(11, 256, 60);
	TurnActorToSceneDirection(11);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Battle_SetStart(3);
	Engine_BattleFxSetWeightedResult(98, 2);
	Engine_ActorSetPosition(11, 0, 0);
	EV_END();
	Engine_GameFlagSet(2377);
}

void EV31_BANPEI(void)
{
	s32 line;

	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	OBJ_MODE(12, 1);
	OBJ_MODE(13, 1);
	OBJ_MODE(14, 1);
	SE(113);
	OBJ_EMO(12, 0x100, 0);
	WAIT(30);
	OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
	line = (s32)MsgRunpaWho3;
	MES_SET(line);
	MES_PUT(12, 0);
	OBJ_EMO(13, 0x100, 0);
	OBJ_EMO(14, 0x100, 0);
	OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	WAIT(65);
	OBJ_DIR(13, 0x5000, 0);
	OBJ_DIR(14, 0xd000, 0);
	MES_SET((line + 1));
	MES_PUT(13, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	MES_SET((line + 2));
	MES_PUT(14, 0);
	MES_SET((line + 3));
	MES_PUT(12, 0);
	Engine_ActorRunRepeatedMotion(13, 1);
	MES_SET((line + 4));
	MES_PUT(13, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	MES_SET((line + 5));
	MES_PUT(14, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	WAIT(60);
	OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	WAIT(70);
	OBJ_WALK(12, 672, 88);
	Engine_ActorWaitForMove(12);
#if EDITION_INTERNATIONAL
	OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
#endif
	Engine_ActorSetAnimationAndWait(12, 3);
	WAIT(30);
	MES_SET((line + 6));
	MES_PUT(12, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Engine_PartySetFields1ceAnd1d0((s32)&SceneId_RunpaJo2, 31);
	Battle_SetStart(3);
	Engine_BattleFxSetWeightedResult(98, 3);
	OBJ_POS_SET(12, 0, 0);
	OBJ_POS_SET(13, 0, 0);
	OBJ_POS_SET(14, 0, 0);
	EV_END();
	FLAG_SET(0x94a);
}

void FieldScene_RunSequenceTail(void)
{
	EV_BEGIN();
	OBJ_POS_SET(12, 688, 88);
	OBJ_POS_SET(13, 704, 88);
	OBJ_POS_SET(14, 720, 96);
	OBJ_MODE(12, 5);
	OBJ_MODE(13, 5);
	OBJ_MODE(14, 5);
	OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	EV_END();
	Engine_EventOpenScreen();
}

void EV32_KARA(void)
{

	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	SE(113);
	Engine_ActorShowEmote(16, 256, 60);
	TurnActorToSceneDirection(16);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Battle_SetStart(3);
	Engine_BattleFxSetWeightedResult(98, 2);
	Engine_ActorSetPosition(16, 0, 0);
	EV_END();
	Engine_GameFlagSet(2379);
}

void EV50_MAIN(void)
{
	EV_BEGIN();
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
	SE(113);
	Engine_ActorShowEmote(12, 256, 60);
	{
		s32 t = (s32)MsgRunpaWho2;

		MES_SET(t);
		MES_PUT(12, 0);
		Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 50);
		MES_SET(t + 1);
	}
	MES_PUT(12, 0);
	Engine_EventCloseScreen();
	WAIT(60);
	Engine_EventRequestExit(60);
	EV_END();
	Engine_GameFlagSet(548);
}

void EV51_MAIN(void)
{
	u32 i;
	s32 record;
	s32 msg;

	EV_BEGIN();
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	SE(113);
	OBJ_EMO(21, 0x100, 0);
	OBJ_EMO(13, 0x100, 60);
	OBJ_FACE(21, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
	msg = (s32)MsgRunpaWho2;
	MES_SET(msg);
	MES_PUT(13, 0);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x102, 30);
	MES_SET(msg + 1);
	MES_PUT(13, 0);
	Engine_EventCloseScreen();
	WAIT(60);
	Engine_EventRequestExit(60);
	EV_END();
	FLAG_SET(0x225);
}

void ConfigureInteractionRegionA(void)
{
	Engine_MapCopyCells(2, 82, 1, 2, 21, 81);
	Engine_MapCopyCellAttributes(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
	Engine_MapCopyCells(2, 84, 1, 2, 6, 55);
	Engine_MapCopyCellAttributes(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
	Engine_MapCopyCells(2, 86, 1, 2, 27, 62);
	Engine_MapCopyCellAttributes(26, 16, 1, 1, 27, 17);
}

void EV52_IDO(void)
{

	if (*(s16 *)(((u8 *)gEventWork) + 0xcb8) != 0)
	{
		if (FLAG_CHK(0x947) == 0)
		{
			Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
			SE(188);
			WAIT(1);
			Map_CopyCells(6, 77, 1, 2, 17, 82);
			WAIT(5);
			Map_CopyCells(7, 77, 1, 2, 17, 82);
			WAIT(1);
			ConfigureInteractionRegionA();
			FLAG_SET(0x947);
		}
	}
}

void EV51_SUB(void)
{

	if (*(s16 *)(((u8 *)gEventWork) + 0xcb8) != 0)
	{
		if (FLAG_CHK(0x948) == 0)
		{
			Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
			SE(188);
			WAIT(1);
			Map_CopyCells(6, 77, 1, 2, 3, 55);
			WAIT(5);
			Map_CopyCells(7, 77, 1, 2, 3, 55);
			WAIT(1);
			ConfigureInteractionRegionB();
			FLAG_SET(0x948);
		}
	}
}

void EV00_HANTEI1(void)
{
	Engine_MapCopyCells(5, 77, 1, 2, 17, 82);
	Engine_MapCopyCells(5, 77, 1, 2, 3, 55);
	Engine_MapCopyCellAttributes(15, 33, 1, 1, 17, 35);
	Engine_MapCopyCellAttributes(3, 8, 1, 1, 3, 10);
}

void EV00_HANTEI2(void)
{
	Engine_MapCopyCells(8, 77, 1, 2, 17, 82);
	Engine_MapCopyCells(8, 77, 1, 2, 3, 55);
	Engine_MapCopyCellAttributes(18, 35, 1, 1, 17, 35);
	Engine_MapCopyCellAttributes(2, 10, 1, 1, 3, 10);
}

void EV08_TANSU(void)
{
	Engine_GameFlagSet(2372);
	Engine_GameFlagClear(535);
	Engine_ActorSetPosition(8, 0, 0);
}

void EV09_MAKI(void)
{
	Engine_GameFlagSet(2373);
	ConfigureInteractionRegionC();
	Engine_ActorSetPosition(9, 0, 0);
}

void EV10_HON(void)
{
	Engine_GameFlagSet(2374);
	Engine_GameFlagClear(536);
	Engine_ActorSetPosition(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
}

void RunpaJo_GatherPartyAroundActorTwelve(void)
{
	struct FieldActor *actor;

	actor = Actor_Get(0);
	if (actor != NULL)
	{
		Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(0);
	if (actor != NULL)
	{
		Actor_SetPosition(MEARI, actor->x.fixed, actor->z.fixed);
	}
	actor = Actor_Get(0);
	if (actor != NULL)
	{
		Actor_SetPosition(GERALD, actor->x.fixed, actor->z.fixed);
	}
	OBJ_DIR(ACTOR_PARTY_LEADER, 0, 0);
	OBJ_SPD(IWAN, 0xb333, 0x5999);
	OBJ_WALK(IWAN, 456, 192);
	OBJ_SPD(MEARI, 0xb333, 0x5999);
	OBJ_WALK(MEARI, 440, 184);
	OBJ_SPD(GERALD, 0xb333, 0x5999);
	OBJ_WALK(GERALD, 448, 240);
	Engine_ActorWaitForMove(IWAN);
	OBJ_FACE(IWAN, 12, 0);
	Engine_ActorWaitForMove(GERALD);
	Engine_ActorWaitForMove(MEARI);
	OBJ_FACE(GERALD, 12, 0);
	OBJ_FACE(MEARI, 12, 0);
	WAIT(15);
}

void RunpaJo_ActivateLeverTrap(void)
{
	u32 i;
	s32 record;

	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	SE(141);
	WAIT(80);
	SE(0x120);
	WAIT(5);
	SE(145);
	Map_CopyCells(16, 75, 7, 4, 26, 55);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x100, 0);
	OBJ_EMO(GERALD, 0x100, 0);
	OBJ_EMO(IWAN, 0x100, 0);
	OBJ_EMO(MEARI, 0x100, 0);
	OBJ_EMO(12, 0x100, 0);
	WAIT(60);
}

void RunpaJo_RevealMonster(void)
{
	u32 i;
	s32 record;

	Engine_CameraMoveToActor(11, 1);
	Engine_CameraWaitForMove();
	WAIT(60);
	MES_SET((s32)MsgRunpaPrepareBecomeMonster);
	MES_PUT(13, 0);
	OBJ_SPD(11, 0x10000, 0x8000);
	OBJ_SPD(15, 0x10000, 0x8000);
	OBJ_WALK(11, 472, 180);
	OBJ_WALK(15, 472, 180);
	Engine_CameraFollowActor(11, 1);
	Engine_ActorWaitForMove(11);
	OBJ_MODE(11, 4);
	WAIT(30);
}

void RunpaJo_PositionPartyForMonster(void)
{
	u32 i;
	s32 record;

	OBJ_SPD(IWAN, 0x10000, 0x8000);
	OBJ_WALK(IWAN, 504, 216);
	OBJ_SPD(MEARI, 0x10000, 0x8000);
	OBJ_WALK(MEARI, 440, 232);
	OBJ_SPD(GERALD, 0x10000, 0x8000);
	OBJ_WALK(GERALD, 480, 224);
	Engine_ActorWaitForMove(GERALD);
	OBJ_DIR(GERALD, 0xc000, 0);
	Engine_ActorWaitForMove(IWAN);
	OBJ_DIR(IWAN, 0xc000, 0);
	Engine_ActorWaitForMove(MEARI);
	OBJ_DIR(MEARI, 0xc000, 0);
	OBJ_DIR(ACTOR_PARTY_LEADER, 0xc000, 0);
}

void EV20_DEMO(void)
{
	s32 text_line;

	if (FLAG_CHK(769) != 0)
		return;
	FLAG_SET(624);
	EV_BEGIN();
	if (FLAG_CHK(2370) != 0)
	{
		OBJ_SPD(ACTOR_PARTY_LEADER, 32768, 16384);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
		OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
		RunpaJo_GatherPartyAroundActorTwelve();
		OBJ_EMO(12, 256, 60);
		OBJ_DIR(12, 32768, 0);
		Engine_ActorJump(12, 4, 0);
		Engine_ActorSetSpriteFlags(Actor_Get(12), 1);
		WAIT(30);
		OBJ_SPD(IWAN, 45875, 22937);
		OBJ_WALK(2, 464, 192);
		Engine_ActorWaitForMove(IWAN);
		WAIT(30);
		OBJ_DIR(ACTOR_PARTY_LEADER, 16384, 0);
		OBJ_DIR(IWAN, 16384, 0);
		OBJ_DIR(GERALD, 16384, 0);
		OBJ_DIR(MEARI, 16384, 0);
		OBJ_POS_SET(13, 456, 320);
		Camera_SetSpeed(131072, 16384);
		text_line = (s32)MsgRunpaBackMoreGuess;
		MES_SET(text_line);
		MES_PUT(13, 0);
		OBJ_WALK(13, 458, 272);
		Engine_ActorWaitForMove(13);
		OBJ_DIR(13, 20480, 0);
		WAIT(40);
		Actor_SetDestinationOffset(13, -8, 8);
		Engine_ActorWaitForMove(13);
		WAIT(60);
		SE(155);
		Engine_MessageShowCentered(text_line + 1, 1);
		Actor_SetDestinationOffset(13, 8, -8);
		RunpaJo_ActivateLeverTrap();
		WAIT(120);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_ActorStartRepeatedMotion(IWAN, 2);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Engine_ActorStartRepeatedMotion(MEARI, 2);
		WAIT(20);
		text_line += 2;
		RunpaJo_PositionPartyForMonster();
		OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
		MES_SET(text_line);
		MES_PUT(13, 0);
		RunpaJo_RevealMonster();
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		WAIT(1);
		Battle_SetStart(3);
		Engine_PartySetFields1ceAnd1d0((s32)&SceneId_RunpaJo4, 4);
		Engine_BattleFxSetWeightedResult(98, 4);
	}
	else
	{
		OBJ_MODE(ACTOR_PARTY_LEADER, 1);
		SE(17);
		WAIT(30);
		text_line = (s32)MsgRunpaTimeEat;
		MES_SET(text_line);
		MES_PUT(12, 0);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
		WAIT(140);
		OBJ_DIR(12, 32768, 0);
		Engine_ActorJump(12, 4, 0);
		Engine_ActorSetSpriteFlags(Actor_Get(12), 1);
		MES_SET(text_line + 1);
		MES_PUT(12, 0);
		OBJ_SPD(ACTOR_PARTY_LEADER, 32768, 16384);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
		OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
		RunpaJo_GatherPartyAroundActorTwelve();
		OBJ_SPD(IWAN, 45875, 22937);
		OBJ_WALK(2, 464, 192);
		Engine_ActorWaitForMove(IWAN);
		WAIT(30);
		MES_SET(text_line + 2);
		MES_PUT(IWAN, 0);
		OBJ_EMO(12, 256, 0);
		WAIT(110);
		SE(60);
		MES_SET(text_line + 3);
		MES_PUT(12, 0);
		WAIT(30);
		Engine_ActorSetAnimationAndWait(IWAN, 3);
		WAIT(20);
		OBJ_MODE(IWAN, 1);
		Engine_ActorStartRepeatedMotion(12, 1);
		WAIT(20);
		OBJ_SPD(12, 26214, 13107);
		OBJ_WALK(12, 520, 208);
		Engine_ActorWaitForMove(12);
		OBJ_MODE(12, 1);
		WAIT(20);
		OBJ_DIR(12, 45056, 0);
		WAIT(30);
		OBJ_DIR(12, 20480, 0);
		WAIT(30);
		OBJ_FACE(12, IWAN, 0);
		WAIT(20);
		MES_SET(text_line + 4);
		MES_PUT(12, 0);
		WAIT(40);
		Engine_ActorSetAnimationAndWait(IWAN, 3);
		WAIT(20);
		OBJ_EMO(12, 264, 0);
		WAIT(120);
		MES_SET(text_line + 5);
		MES_PUT(12, 0);
		WAIT(25);
		Engine_ActorSetAnimationAndWait(IWAN, 3);
		WAIT(30);
		Engine_ActorSetAnimationAndWait(12, 3);
		WAIT(40);
		OBJ_WALK(2, 480, 200);
		Engine_ActorWaitForMove(IWAN);
		Engine_ActorFaceEachOther(IWAN, 12, 0);
		WAIT(60);
		MES_SET(text_line + 6);
		MES_PUT(IWAN, 0);
		WAIT(20);
		OBJ_MODE(12, 4);
		WAIT(80);
		MES_SET(text_line + 7);
		MES_PUT(12, 0);
		OBJ_POS_SET(13, 456, 320);
		SE(19);
		MES_SET(text_line + 8);
		MES_PUT(13, 0);
		OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
		OBJ_FACE(IWAN, 13, 0);
		OBJ_FACE(GERALD, 13, 0);
		WAIT(5);
		OBJ_DIR(MEARI, 16384, 0);
		OBJ_FACE(12, 13, 0);
		WAIT(30);
		SE(61);
		Camera_SetSpeed(131072, 16384);
		Engine_CameraMoveToActor(13, 1);
		Engine_CameraWaitForMove();
		OBJ_SPD(13, 52428, 26214);
		OBJ_WALK(13, 456, 304);
		Engine_CameraFollowActor(13, 1);
		Engine_ActorWaitForMove(13);
		Engine_CameraFollowActor(GERALD, 1);
		OBJ_EMO(ACTOR_PARTY_LEADER, 258, 0);
		OBJ_EMO(IWAN, 258, 0);
		OBJ_EMO(GERALD, 258, 0);
		OBJ_EMO(MEARI, 258, 0);
		OBJ_EMO(12, 258, 0);
		WAIT(60);
		OBJ_FACE(12, 13, 0);
		Engine_ActorStartRepeatedMotion(12, 2);
		WAIT(60);
		MES_SET(text_line + 9);
		MES_PUT(12, 0);
		OBJ_FACE(13, 12, 0);
		MES_SET(text_line + 10);
		MES_PUT(13, 0);
		WAIT(60);
		OBJ_FACE(13, IWAN, 0);
		WAIT(30);
		MES_SET(text_line + 11);
		MES_PUT(13, 0);
		Engine_ActorFaceEachOther(MEARI, IWAN, 0);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
		WAIT(60);
		OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
		OBJ_FACE(IWAN, 13, 0);
		OBJ_FACE(GERALD, 13, 0);
		OBJ_FACE(MEARI, 13, 0);
		Engine_ActorStartRepeatedMotion(13, 1);
		WAIT(60);
		MES_SET(text_line + 12);
		MES_PUT(13, 0);
		OBJ_EMO(1, 259, 0);
		WAIT(60);
		OBJ_MODE(13, 4);
		MES_SET(text_line + 13);
		MES_PUT(13, 0);
		OBJ_WALK(1, 456, 248);
		Engine_ActorWaitForMove(GERALD);
		OBJ_DIR(1, 16384, 0);
		MES_SET(text_line + 14);
		MES_PUT(GERALD, 0);
		OBJ_WALK(2, 472, 216);
		Engine_ActorWaitForMove(IWAN);
		OBJ_DIR(2, 16384, 0);
		WAIT(10);
		MES_SET(text_line + 15);
		MES_PUT(IWAN, 0);
		OBJ_EMO(12, 261, 0);
		WAIT(60);
		MES_SET(text_line + 16);
		MES_PUT(12, 0);
		OBJ_WALK(3, 440, 216);
		Engine_ActorWaitForMove(MEARI);
		OBJ_FACE(MEARI, 13, 0);
		Engine_ActorSetAnimationAndWait(MEARI, 3);
		WAIT(10);
		MES_SET(text_line + 17);
		MES_PUT(MEARI, 0);
		OBJ_MODE(13, 4);
		MES_SET(text_line + 18);
		MES_PUT(13, 0);
		OBJ_EMO(ACTOR_PARTY_LEADER, 258, 0);
		OBJ_EMO(GERALD, 258, 0);
		OBJ_EMO(MEARI, 258, 0);
		OBJ_EMO(IWAN, 258, 0);
		OBJ_EMO(13, 264, 0);
		WAIT(60);
		MES_SET(text_line + 19);
		MES_PUT(13, 0);
		WAIT(20);
		OBJ_EMO(1, 259, 0);
		WAIT(60);
		MES_SET(text_line + 20);
		MES_PUT(GERALD, 0);
		Engine_ActorStartRepeatedMotion(13, 1);
		WAIT(60);
		MES_SET(text_line + 21);
		MES_PUT(13, 0);
		OBJ_WALK(13, 456, 280);
		Engine_ActorWaitForMove(13);
		OBJ_DIR(13, 20480, 0);
		WAIT(80);
		Actor_SetDestinationOffset(13, -8, 8);
		Engine_ActorWaitForMove(13);
		WAIT(60);
		SE(155);
		Engine_MessageShowCentered((s32)MsgRunpaDodonpaPulledLever, 1);
		Actor_SetDestinationOffset(13, 8, -8);
		OBJ_FACE(13, 11, 0);
		RunpaJo_ActivateLeverTrap();
		SE(52);
		MES_SET(text_line + 23);
		MES_PUT(13, 0);
		WAIT(60);
		OBJ_FACE(ACTOR_PARTY_LEADER, 11, 0);
		OBJ_FACE(GERALD, 11, 0);
		OBJ_FACE(IWAN, 11, 0);
		OBJ_FACE(MEARI, 11, 0);
		OBJ_FACE(12, 11, 0);
		RunpaJo_PositionPartyForMonster();
		RunpaJo_RevealMonster();
		FLAG_SET(2370);
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		WAIT(1);
		Battle_SetStart(3);
		Engine_PartySetFields1ceAnd1d0((s32)&SceneId_RunpaJo4, 4);
		Engine_BattleFxSetWeightedResult(98, 4);
	}
	EV_END();
}

void FieldScene_RunMainScriptSequence(void)
{
	u32				   i;
	struct FieldActor *record;
	s32				   script_a;
	s32				   script_b;
	s32				   script_c;

	FLAG_SET(0x301);
	FLAG_SET(0x941);
	Map_CopyCells(16, 75, 7, 4, 26, 55);
	PlaceSceneObjectPairFromTableA(4);
	EV_BEGIN();
	record = Actor_Get(12);
	Engine_ActorSetSpriteFlags(record, 1);
	OBJ_POS_SET(ACTOR_PARTY_LEADER, 456, 184);
	OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
	OBJ_POS_SET(GERALD, 440, 192);
	OBJ_DIR(GERALD, 0x4000, 0);
	OBJ_POS_SET(MEARI, 488, 184);
	OBJ_DIR(MEARI, 0x4000, 0);
	OBJ_POS_SET(IWAN, 472, 184);
	OBJ_DIR(IWAN, 0x4000, 0);
	OBJ_POS_SET(12, 520, 224);
	OBJ_DIR(12, 0xb000, 0);
	OBJ_POS_SET(11, 448, 237);
	OBJ_DIR(11, 0x8000, 0);
	OBJ_POS_SET(15, 448, 238);
	Engine_ActorSetSpritePriority(15, 3);
	OBJ_MODE(11, 3);
	OBJ_POS_SET(13, 458, 243);
	OBJ_DIR(13, 0x4000, 0);
	OBJ_MODE(13, 5);
	Engine_EventOpenScreen();
	WAIT(120);

	script_a = (s32)MsgRunpaUhnnGetOff;
	MES_SET(script_a);
	MES_PUT(13, 0);
	OBJ_EMO(GERALD, 0x101, 60);
	MES_SET((script_a + 1));
	MES_PUT(GERALD, 0);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x101, 1);
	OBJ_EMO(IWAN, 0x101, 1);
	OBJ_EMO(MEARI, 0x101, 1);
	WAIT(60);
	OBJ_FACE(12, 13, 0);
	WAIT(60);
	MES_SET((script_a + 2));
	MES_PUT(12, 0);
	OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
	OBJ_FACE(GERALD, 12, 0);
	OBJ_FACE(IWAN, 12, 0);
	OBJ_FACE(MEARI, 12, 0);
	WAIT(60);
	OBJ_WALK(12, 512, 232);
	OBJ_MODE(12, 4);
	WAIT(60);
	MES_SET((script_a + 3));
	MES_PUT(12, 0);
	WAIT(15);
	Engine_ActorStartRepeatedMotion(13, 2);
	MES_SET((script_a + 4));
	MES_PUT(13, 0);
	OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	OBJ_FACE(IWAN, 13, 0);
	OBJ_FACE(MEARI, 13, 0);
	OBJ_FACE(GERALD, 13, 0);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	MES_SET((script_a + 5));
	MES_PUT(IWAN, 0);
	OBJ_EMO(MEARI, 0x105, 0);
	OBJ_MODE(MEARI, 4);
	WAIT(80);
	OBJ_MODE(MEARI, 1);
	MES_SET((script_a + 6));
	MES_PUT(MEARI, 0);
	OBJ_EMO(12, 0x102, 65);
	Engine_ActorStartRepeatedMotion(12, 2);
	WAIT(100);
	MES_SET((script_a + 7));
	MES_PUT(13, 0);
	OBJ_FACE(GERALD, ACTOR_PARTY_LEADER, 0);
	WAIT(10);
	MES_SET((script_a + 8));
	MES_PUT(GERALD, 0);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	WAIT(30);
	MES_SET((script_a + 9));
	MES_PUT(IWAN, 0);
	OBJ_FACE(MEARI, ACTOR_PARTY_LEADER, 0);
	MES_SET((script_a + 10));
	MES_OPEN(MEARI, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		OBJ_MODE(IWAN, 3);
		OBJ_MODE(GERALD, 3);
		OBJ_MODE(MEARI, 3);
		OBJ_FACE(12, 13, 0);
		WAIT(60);
	}
	else
	{
		MES_SET((script_a + 11));
		MES_PUT(MEARI, 0);
		OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
		OBJ_FACE(GERALD, 12, 0);
		OBJ_FACE(IWAN, 12, 0);
		OBJ_FACE(MEARI, 12, 0);
		WAIT(20);
		OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
		WAIT(60);
		OBJ_FACE(12, 13, 0);
		WAIT(80);
		Engine_ActorSetAnimationAndWait(12, 3);
		WAIT(30);
		OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
		MES_SET((script_a + 12));
		MES_PUT(12, 0);
		WAIT(60);
		OBJ_FACE(IWAN, 12, 0);
		OBJ_FACE(GERALD, 12, 0);
		OBJ_FACE(MEARI, 12, 0);
		OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
		OBJ_MODE(IWAN, 3);
		OBJ_MODE(GERALD, 3);
		OBJ_MODE(MEARI, 3);
		OBJ_MODE(ACTOR_PARTY_LEADER, 3);
		WAIT(80);
		OBJ_FACE(12, 13, 0);
		OBJ_FACE(IWAN, 13, 0);
		OBJ_FACE(GERALD, 13, 0);
		OBJ_FACE(MEARI, 13, 0);
		OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	}
	OBJ_SPD(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	OBJ_SPD(GERALD, 0x8000, 0x4000);
	OBJ_SPD(MEARI, 0x8000, 0x4000);
	OBJ_SPD(IWAN, 0x8000, 0x4000);
	OBJ_WALK(GERALD, 416, 216);
	Engine_ActorWaitForMove(GERALD);
	OBJ_WALK(GERALD, 416, 248);
	Engine_ActorWaitForMove(GERALD);
	OBJ_WALK(GERALD, 440, 248);
	Engine_ActorWaitForMove(GERALD);
	OBJ_DIR(GERALD, 0xc000, 0);
	OBJ_WALK(ACTOR_PARTY_LEADER, 440, 216);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	OBJ_DIR(ACTOR_PARTY_LEADER, 0, 0);
	OBJ_WALK(MEARI, 488, 248);
	Engine_ActorWaitForMove(MEARI);
	OBJ_WALK(MEARI, 456, 248);
	Engine_ActorWaitForMove(MEARI);
	OBJ_DIR(MEARI, 0xc000, 0);
	OBJ_WALK(IWAN, 456, 216);
	Engine_ActorWaitForMove(IWAN);
	OBJ_DIR(IWAN, 0x8000, 0);
	Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceEachOther(MEARI, IWAN, 0);
	WAIT(60);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	OBJ_MODE(IWAN, 3);
	OBJ_MODE(GERALD, 3);
	OBJ_MODE(MEARI, 3);
	WAIT(100);
	SE(226);
	OBJ_MODE(13, 7);
	OBJ_SPD(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
	OBJ_SPD(GERALD, 0x1999, 0xccc);
	Actor_SetDestinationOffset(GERALD, -24, 0);
	OBJ_SPD(MEARI, 0x1999, 0xccc);
	Actor_SetDestinationOffset(MEARI, -24, 0);
	OBJ_SPD(IWAN, 0x1999, 0xccc);
	Actor_SetDestinationOffset(IWAN, -24, 0);
	OBJ_SPD(11, 0x1999, 0xccc);
	OBJ_SPD(15, 0x1999, 0xccc);
	Actor_SetDestinationOffset(11, -24, 0);
	Actor_SetDestinationOffset(15, -24, 0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	SE(0x120);
	WAIT(60);
	OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
	OBJ_DIR(GERALD, 0xc000, 0);
	OBJ_DIR(IWAN, 0x4000, 0);
	OBJ_DIR(MEARI, 0xc000, 0);
	WAIT(60);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	OBJ_MODE(GERALD, 3);
	OBJ_MODE(IWAN, 3);
	OBJ_MODE(MEARI, 3);
	WAIT(100);
	OBJ_SPD(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
	OBJ_SPD(GERALD, 0xb333, 0x5999);
	OBJ_SPD(IWAN, 0xb333, 0x5999);
	OBJ_SPD(MEARI, 0xb333, 0x5999);
	OBJ_WALK(ACTOR_PARTY_LEADER, 456, 184);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	OBJ_MODE(ACTOR_PARTY_LEADER, 1);
	OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
	OBJ_WALK(GERALD, 464, 256);
	Engine_ActorWaitForMove(GERALD);
	OBJ_WALK(GERALD, 480, 248);
	Engine_ActorWaitForMove(GERALD);
	OBJ_WALK(GERALD, 440, 192);
	Engine_ActorWaitForMove(GERALD);
	OBJ_MODE(GERALD, 1);
	OBJ_DIR(GERALD, 0x4000, 0);
	OBJ_WALK(MEARI, 488, 248);
	Engine_ActorWaitForMove(MEARI);
	OBJ_WALK(MEARI, 488, 184);
	OBJ_WALK(IWAN, 472, 184);
	Engine_ActorWaitForMove(IWAN);
	OBJ_MODE(IWAN, 1);
	OBJ_DIR(IWAN, 0x4000, 0);
	Engine_ActorWaitForMove(MEARI);
	OBJ_MODE(MEARI, 1);
	OBJ_DIR(MEARI, 0x4000, 0);
	WAIT(30);
	WAIT(60);
	script_b = (s32)MsgRunpaAbleGet;
	MES_SET(script_b);
	MES_PUT(GERALD, 0);
	OBJ_MODE(13, 6);
	WAIT(120);
	Engine_ActorStartRepeatedMotion(13, 2);
	WAIT(60);
	OBJ_MODE(13, 7);
	MES_SET((script_b + 1));
	MES_PUT(13, 0);
	WAIT(20);
	Engine_ActorFaceEachOther(MEARI, IWAN, 0);
	WAIT(10);
	OBJ_EMO(MEARI, 0x101, 80);
	OBJ_FACE(MEARI, 13, 0);
	OBJ_FACE(IWAN, 13, 0);
	MES_SET((script_b + 2));
	MES_PUT(MEARI, 0);
	OBJ_EMO(13, 0x102, 70);
	Engine_ActorStartRepeatedMotion(13, 2);
	WAIT(60);
	OBJ_MODE(13, 5);
	WAIT(70);
	MES_SET((script_b + 3));
	MES_PUT(13, 0);
	OBJ_MODE(13, 7);
	OBJ_EMO(IWAN, 0x108, 40);
	MES_SET((script_b + 4));
	MES_PUT(IWAN, 0);
	Engine_ActorSetAnimationAndWait(12, 3);
	OBJ_EMO(GERALD, 0x103, 60);
	OBJ_WALK(GERALD, 440, 208);
	Engine_ActorWaitForMove(GERALD);
	OBJ_FACE(GERALD, ACTOR_PARTY_LEADER, 0);
	MES_SET((script_b + 5));
	MES_PUT(GERALD, 0);
	WAIT(20);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x101, 0);
	OBJ_EMO(IWAN, 0x101, 0);
	OBJ_EMO(MEARI, 0x101, 0);
	OBJ_EMO(12, 0x101, 0);
	WAIT(70);
	OBJ_FACE(GERALD, IWAN, 0);
	OBJ_EMO(GERALD, 0x102, 75);
	OBJ_FACE(GERALD, ACTOR_PARTY_LEADER, 0);
	OBJ_MODE(GERALD, 4);
	MES_SET((script_b + 6));
	MES_PUT(GERALD, 0);
	OBJ_EMO(12, 0x101, 0);
	WAIT(60);
	MES_SET((script_b + 7));
	MES_PUT(12, 0);
	OBJ_WALK(GERALD, 448, 208);
	Engine_ActorWaitForMove(GERALD);
	OBJ_MODE(GERALD, 1);
	OBJ_FACE(GERALD, 12, 0);
	MES_SET((script_b + 8));
	MES_PUT(GERALD, 0);
	Engine_ActorStartRepeatedMotion(12, 1);
	WAIT(60);
	OBJ_FACE(GERALD, 12, 0);
	WAIT(60);
	MES_SET((script_b + 9));
	MES_PUT(12, 0);
	OBJ_MODE(GERALD, 4);
	WAIT(60);
	MES_SET((script_b + 10));
	MES_PUT(GERALD, 0);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	WAIT(30);
	MES_SET((script_b + 11));
	MES_PUT(IWAN, 0);
	OBJ_FACE(GERALD, MEARI, 0);
	WAIT(20);
	MES_SET((script_b + 12));
	MES_PUT(GERALD, 0);
	WAIT(30);
	OBJ_EMO(MEARI, 0x100, 80);
	OBJ_FACE(MEARI, GERALD, 0);
	MES_SET((script_b + 13));
	MES_PUT(MEARI, 0);
	OBJ_FACE(MEARI, 13, 0);
	MES_SET((script_b + 14));
	MES_PUT(MEARI, 0);
	OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	OBJ_FACE(IWAN, 13, 0);
	OBJ_FACE(GERALD, 13, 0);
	OBJ_FACE(MEARI, 13, 0);
	WAIT(120);
	OBJ_EMO(13, 0x102, 30);
	Engine_ActorStartRepeatedMotion(13, 1);
	WAIT(120);
	Engine_ActorStartRepeatedMotion(12, 1);
	WAIT(60);
	MES_SET((script_b + 15));
	MES_PUT(12, 0);
	OBJ_EMO(GERALD, 0x107, 110);
	OBJ_WALK(GERALD, 456, 212);
	Engine_ActorWaitForMove(GERALD);
	OBJ_MODE(GERALD, 1);
	MES_SET((script_b + 16));
	MES_PUT(GERALD, 0);
	OBJ_FACE(IWAN, GERALD, 0);
	WAIT(60);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	MES_SET((script_b + 17));
	MES_PUT(IWAN, 0);
	OBJ_FACE(GERALD, 13, 0);
	WAIT(80);
	OBJ_FACE(GERALD, IWAN, 0);
	WAIT(60);
	OBJ_FACE(GERALD, 13, 0);
	WAIT(30);
	OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
	OBJ_FACE(MEARI, 13, 0);
	OBJ_FACE(13, 13, 0);
	OBJ_FACE(12, 13, 0);
	WAIT(80);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	WAIT(30);
	MES_SET((script_b + 18));
	MES_PUT(GERALD, 0);
	Engine_ActorStartRepeatedMotion(13, 2);
	WAIT(70);
	MES_SET((script_b + 19));
	MES_PUT(13, 0);
	OBJ_EMO(MEARI, 0x100, 60);
	MES_SET((script_b + 20));
	MES_PUT(MEARI, 0);
	OBJ_FACE(IWAN, ACTOR_PARTY_LEADER, 0);
	WAIT(80);
	MES_SET((script_b + 21));
	MES_OPEN(IWAN, 0);
	OBJ_FACE(GERALD, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(MEARI, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(IWAN, ACTOR_PARTY_LEADER, 0);
	OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
	OBJ_POS_SET(14, 456, 304);
	OBJ_SPD(14, 0x8000, 0x4000);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		SE(19);
		MES_SET((script_b + 22));
		MES_PUT(14, 0);
	}
	else
	{
		SE(19);
		MES_SET((script_b + 23));
		MES_PUT(14, 0);
	}
	OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
	OBJ_FACE(GERALD, 14, 0);
	OBJ_FACE(MEARI, 14, 0);
	OBJ_FACE(IWAN, 14, 0);
	OBJ_FACE(12, 14, 0);
	OBJ_FACE(13, 14, 0);
	Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
	Engine_ActorStartRepeatedMotion(13, 1);
	WAIT(60);
	SE(8);
	script_c = (s32)MsgRunpaDad;
	MES_SET(script_c);
	MES_PUT(13, 0);
	OBJ_WALK(14, 456, 280);
	Engine_ActorWaitForMove(14);
	OBJ_WALK(14, 440, 256);
	Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
	MES_SET((script_c + 1));
	MES_PUT(14, 0);
	Engine_ActorWaitForMove(14);
	OBJ_DIR(14, 0xd000, 0);
	OBJ_EMO(13, 0x102, 80);
	OBJ_MODE(14, 4);
	WAIT(89);
	MES_SET((script_c + 2));
	MES_PUT(14, 0);
	Engine_ActorRunRepeatedMotion(13, 2);
	Engine_ActorSetAnimationAndWait(14, 3);
	MES_SET((script_c + 3));
	MES_PUT(14, 0);
	OBJ_EMO(13, 0x100, 80);
	OBJ_MODE(14, 4);
	WAIT(80);
	OBJ_DIR(14, 0x3000, 0);
	WAIT(20);
	MES_SET((script_c + 4));
	MES_PUT(14, 0);
	Engine_ActorStartRepeatedMotion(13, 1);
	MES_SET((script_c + 5));
	MES_PUT(13, 0);
	WAIT(30);
	Engine_ActorSetAnimationAndWait(14, 3);
	OBJ_FACE(14, 13, 0);
	WAIT(20);
	MES_SET((script_c + 6));
	MES_PUT(14, 0);
	Engine_ActorStartRepeatedMotion(13, 1);
	OBJ_EMO(13, 0x102, 80);
	MES_SET((script_c + 7));
	MES_PUT(13, 0);
	OBJ_EMO(14, 0x103, 60);
	MES_SET((script_c + 8));
	MES_PUT(14, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	MES_SET((script_c + 9));
	MES_PUT(14, 0);
	WAIT(20);
	Actor_SetAttachedEffect(13, 0x101);
	WAIT(80);
	Actor_SetAttachedEffect(13, 0);
	OBJ_WALK(14, 496, 240);
	Engine_ActorWaitForMove(14);
	OBJ_FACE(14, 12, 0);
	WAIT(20);
	OBJ_FACE(12, 14, 0);
	MES_SET((script_c + 10));
	MES_PUT(14, 0);
	Engine_ActorStartRepeatedMotion(12, 1);
	MES_SET((script_c + 11));
	MES_PUT(12, 0);
	WAIT(40);
	Engine_ActorSetAnimationAndWait(14, 3);
	WAIT(20);
	MES_SET((script_c + 12));
	MES_PUT(14, 0);
	OBJ_MODE(14, 3);
	OBJ_EMO(12, 0x102, 60);
	MES_SET((script_c + 13));
	MES_PUT(12, 0);
	OBJ_EMO(14, 0x100, 70);
	MES_SET((script_c + 14));
	MES_PUT(14, 0);
	OBJ_MODE(12, 3);
	WAIT(140);
	OBJ_MODE(14, 3);
	WAIT(120);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 14, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	OBJ_MODE(IWAN, 3);
	OBJ_MODE(GERALD, 3);
	OBJ_MODE(MEARI, 3);
	WAIT(120);
	OBJ_EMO(14, 0x108, 180);
	MES_SET((script_c + 15));
	MES_PUT(14, 0);
	OBJ_EMO(12, 0x101, 80);
	MES_SET((script_c + 16));
	MES_PUT(12, 0);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
	Engine_ActorFaceEachOther(MEARI, IWAN, 0);
	WAIT(80);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	WAIT(60);
	OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
	OBJ_FACE(GERALD, 14, 0);
	OBJ_FACE(IWAN, 14, 0);
	OBJ_FACE(MEARI, 14, 0);
	WAIT(60);
	MES_SET((script_c + 17));
	MES_PUT(14, 0);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x101, 0);
	OBJ_EMO(GERALD, 0x101, 0);
	OBJ_EMO(IWAN, 0x101, 0);
	OBJ_EMO(MEARI, 0x101, 0);
	WAIT(100);
	MES_SET((script_c + 18));
	MES_PUT(GERALD, 0);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	WAIT(60);
	MES_SET((script_c + 19));
	MES_PUT(IWAN, 0);
	OBJ_FACE(14, IWAN, 0);
	WAIT(20);
	MES_SET((script_c + 20));
	MES_PUT(14, 0);
	OBJ_EMO(ACTOR_PARTY_LEADER, 0x101, 0);
	OBJ_EMO(GERALD, 0x101, 0);
	OBJ_EMO(IWAN, 0x101, 0);
	OBJ_EMO(MEARI, 0x101, 0);
	WAIT(120);
	OBJ_MODE(14, 4);
	WAIT(120);
	MES_SET((script_c + 21));
	MES_PUT(14, 0);
	OBJ_EMO(14, 0x102, 90);
	MES_SET((script_c + 22));
	MES_PUT(14, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	OBJ_MODE(IWAN, 3);
	OBJ_MODE(MEARI, 3);
	OBJ_MODE(GERALD, 3);
	OBJ_MODE(12, 3);
	WAIT(80);
	OBJ_MODE(14, 4);
	WAIT(120);
	MES_SET((script_c + 23));
	MES_PUT(14, 0);
	OBJ_EMO(12, 0x100, 60);
	MES_SET((script_c + 24));
	MES_PUT(12, 0);
	OBJ_FACE(14, 12, 0);
	WAIT(20);
	Engine_ActorStartRepeatedMotion(14, 1);
	WAIT(50);
	MES_SET((script_c + 25));
	MES_PUT(14, 0);
	OBJ_MODE(GERALD, 3);
	WAIT(70);
	MES_SET((script_c + 26));
	MES_PUT(GERALD, 0);
	OBJ_MODE(MEARI, 3);
	MES_SET((script_c + 27));
	MES_PUT(MEARI, 0);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	WAIT(20);
	MES_SET((script_c + 28));
	MES_PUT(14, 0);
	OBJ_EMO(IWAN, 0x101, 90);
	MES_SET((script_c + 29));
	MES_PUT(IWAN, 0);
	OBJ_DIR(14, 0x5000, 0);
	WAIT(20);
	MES_SET((script_c + 30));
	MES_PUT(14, 0);
	Engine_ActorStartRepeatedMotion(12, 1);
	WAIT(60);
	MES_SET((script_c + 31));
	MES_PUT(12, 0);
	OBJ_FACE(14, 12, 0);
	WAIT(20);
	Engine_ActorSetAnimationAndWait(14, 3);
	WAIT(20);
	MES_SET((script_c + 32));
	MES_PUT(14, 0);
	OBJ_EMO(GERALD, 0x100, 70);
	MES_SET((script_c + 33));
	MES_PUT(GERALD, 0);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	WAIT(30);
	MES_SET((script_c + 34));
	MES_PUT(14, 0);
	OBJ_EMO(MEARI, 0x100, 80);
	MES_SET((script_c + 35));
	MES_PUT(MEARI, 0);
	Engine_ActorSetAnimationAndWait(14, 3);
	MES_SET((script_c + 36));
	MES_PUT(14, 0);
	OBJ_FACE(IWAN, ACTOR_PARTY_LEADER, 0);
	WAIT(30);
	OBJ_MODE(IWAN, 3);
	WAIT(100);
	MES_SET((script_c + 37));
	MES_OPEN(IWAN, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		MES_SET((script_c + 38));
		MES_PUT(14, 0);
	}
	WAIT(30);
	OBJ_MODE(GERALD, 3);
	OBJ_MODE(IWAN, 3);
	OBJ_MODE(MEARI, 3);
	WAIT(100);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	OBJ_MODE(12, 3);
	WAIT(100);
	OBJ_MODE(GERALD, 2);
	record = Actor_Get(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	OBJ_POS_SET(GERALD, 0, 0);
	OBJ_MODE(IWAN, 2);
	record = Actor_Get(0);
	if (record != 0)
	{
		Actor_SetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(IWAN);
	OBJ_POS_SET(IWAN, 0, 0);
	OBJ_MODE(MEARI, 2);
	record = Actor_Get(0);
	if (record != 0)
	{
		Actor_SetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(MEARI);
	OBJ_POS_SET(MEARI, 0, 0);
	WAIT(30);
	OBJ_SPD(12, 0x6666, 0x3333);
	OBJ_WALK(12, 472, 184);
	Engine_ActorWaitForMove(12);
	OBJ_MODE(12, 1);
	OBJ_DIR(12, 0x3000, 0);
	OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
	OBJ_MODE(14, 3);
	WAIT(20);
	OBJ_FACE(14, 13, 0);
	WAIT(20);
	MES_SET((script_c + 39));
	MES_PUT(14, 0);
	Engine_ActorRunRepeatedMotion(13, 2);
	MES_SET((script_c + 40));
	MES_PUT(13, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	WAIT(30);
	OBJ_SPD(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	OBJ_SPD(12, 0x10000, 0x8000);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	OBJ_WALK(ACTOR_PARTY_LEADER, 480, 248);
	WAIT(40);
	Engine_ActorSetSpritePriority(12, 0);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
	OBJ_WALK(12, 480, 216);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
	Engine_ActorWaitForMove(12);
	OBJ_FACE(12, 14, 0);
	OBJ_DIR(12, 0x3000, 0);
	OBJ_MODE(ACTOR_PARTY_LEADER, 3);
	WAIT(5);
	OBJ_MODE(12, 3);
	WAIT(100);
	OBJ_MODE(14, 3);
	WAIT(100);
	OBJ_WALK(12, 480, 248);
	OBJ_WALK(ACTOR_PARTY_LEADER, 456, 248);
	Engine_ActorWaitForMove(12);
	OBJ_WALK(12, 456, 248);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	OBJ_WALK(ACTOR_PARTY_LEADER, 456, 360);
	Engine_ActorWaitForMove(12);
	OBJ_WALK(12, 456, 360);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	OBJ_WALK(ACTOR_PARTY_LEADER, 352, 360);
	Engine_ActorWaitForMove(12);
	OBJ_WALK(12, 352, 360);
	WAIT(20);
	SE(17);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	Engine_EventCloseScreen();
	WAIT(1);
	WAIT(210);
	Engine_EventRequestExit(4);
	EV_END();
}

void EV21_MES(void)
{

	switch (gRunpaJoRandomPick)
	{
	case 0:
		MES_SET((s32)MsgRunpaHammetGreatMerchant);
		MES_PUT(21, 0);
		break;
	case 1:
		MES_SET((s32)MsgRunpaZZZ);
		MES_PUT(21, 0);
		break;
	case 2:
		MES_SET((s32)MsgRunpaDodonpasOrdersAbsolute);
		MES_PUT(21, 0);
		break;
	case 3:
		MES_SET((s32)MsgRunpaSighBadCouldnt);
		MES_PUT(21, 0);
		break;
	case 4:
		MES_SET((s32)MsgRunpaStrangeSwearSomeone);
		MES_PUT(21, 0);
		break;
	case 6:
		MES_SET((s32)MsgRunpaToldStandGuard);
		MES_PUT(21, 0);
		break;
	case 7:
		MES_SET((s32)MsgRunpaWhoDisruptingSleep);
		MES_PUT(21, 0);
		break;
	case 5:
		Engine_ActorFaceDirection(21, 0xd000, 0);
		WAIT(50);
		Engine_ActorFaceDirection(21, 0xb000, 0);
		WAIT(50);
		Engine_ActorFaceDirection(21, 0x5000, 0);
		WAIT(50);
		MES_SET((s32)MsgRunpaTakeCareAnybody);
		MES_PUT(21, 0);
		break;
	}
}

void EV21_MAIN(void)
{

	s32 msg;

	OBJ_EMO(21, 0x101, 30);
	OBJ_DIR(21, 0xd000, 0);
	WAIT(50);
	OBJ_DIR(21, 0xb000, 0);
	WAIT(50);
	OBJ_DIR(21, 0x5000, 0);
	WAIT(50);
	msg = (s32)MsgRunpaLeftGuardHearsSomeone;
	MES_SET(msg);
	MES_PUT(21, 0);
	OBJ_MODE(21, 4);
	WAIT(60);
	OBJ_DIR(21, 0xb000, 0);
	WAIT(40);
	MES_SET(msg + 1);
	MES_PUT(21, 0);
}

void EV25_DONPA(void)
{
	u32 i;
	s32 record;
	s32 msg;
	s32 msg2;

	EV_BEGIN();
	if (FLAG_CHK(0x941) != 0)
	{
		MES_SET((s32)MsgRunpaDonpaGrateful);
		MES_PUT(18, 0);
		EV_END();
	}
	else
	{
		if (FLAG_CHK(0x313) != 0)
		{
			MES_SET((s32)MsgRunpaMaybeDodonpasEyes);
			MES_OPEN(25, 0);
			EV_END();
		}
		else
		{
			OBJ_EMO(25, 0x102, 30);
			OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
			msg = (s32)MsgRunpaShhhPleaseDont;
			MES_SET(msg);
			MES_PUT(25, 0);
			OBJ_FACE(25, 24, 0);
			Engine_CameraMoveToActor(24, 1);
			Engine_CameraWaitForMove();
			WAIT(60);
			Engine_CameraMoveToActor(0, 1);
			WAIT(20);
			OBJ_EMO(25, 0x105, 60);
			MES_SET(msg + 1);
			MES_PUT(25, 0);
			OBJ_EMO(25, 0x107, 60);
			MES_SET(msg + 2);
			MES_PUT(25, 0);
			WAIT(70);
			OBJ_EMO(25, 0x100, 60);
			OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
			MES_SET(msg + 3);
			MES_OPEN(25, 0);
			if (Engine_EventChooseYesNo(0, 0) == 0)
			{
				MES_SET(msg + 4);
				MES_OPEN(25, 0);
			}
			else
			{
				MES_SET(msg + 5);
				MES_OPEN(25, 0);
			}
			WAIT(60);
			OBJ_EMO(25, 0x105, 60);
			msg2 = (s32)MsgRunpaDonpaKnowsCoddled;
			MES_SET(msg2);
			MES_OPEN(25, 0);
			Engine_ActorRunRepeatedMotion(25, 1);
			MES_SET(msg2 + 1);
			MES_OPEN(25, 0);
			Engine_ActorSetAnimationAndWait(25, 3);
			MES_SET(msg2 + 2);
			MES_OPEN(25, 0);
			FLAG_SET(0x313);
			EV_END();
		}
	}
}

void EV25_SEL(void)
{
	if (Engine_GameFlagIsSet(0x941))
	{
		MES_SET((s32)MsgRunpaDifficultDonpaRight);
		MES_PUT(25, 0);
	}
	else
	{
		MES_SET((s32)MsgRunpaSomeonePunishDodonpa);
		MES_PUT(25, 0);
	}
}

void EV24_SEL(void)
{
	if (Engine_GameFlagIsSet(0x941))
	{
		MES_SET((s32)MsgRunpaFatherStayAngry);
		MES_PUT(24, 0);
	}
	else
	{
		MES_SET((s32)MsgRunpaFatherSorryDodonpa);
		MES_PUT(24, 0);
	}
}

void EV24_MAIN(void)
{
	s32 sequence_id;

	if (FLAG_CHK(2369) != 0)
	{
		if (FLAG_CHK(2382) == 0 && FLAG_CHK(788) == 0)
		{
			sequence_id = (s32)MsgRunpaThankHelpDodonpa;
			MES_SET(sequence_id);
			MES_PUT(PRIMARY_ID, 0);
			Engine_ActorRunRepeatedMotion(PRIMARY_ID, 1);
			WAIT(30);
			OBJ_SPD(PRIMARY_ID, 6553, 3276);
			Actor_SetDestinationOffset(PRIMARY_ID, -4, 0);
			Engine_ActorWaitForMove(PRIMARY_ID);
			OBJ_MODE(PRIMARY_ID, 3);
			WAIT(60);
			OBJ_SPD(PRIMARY_ID, 13107, 6553);
			Actor_SetDestinationOffset(PRIMARY_ID, -6, 0);
			OBJ_FACE(PRIMARY_ID, 0, 0);
			Engine_ActorWaitForMove(PRIMARY_ID);
			MES_SET(sequence_id + 1);
			MES_PUT(PRIMARY_ID, 0);
			Engine_ActorRunRepeatedMotion(PRIMARY_ID, 1);
			OBJ_FACE(DERIVED_ID, PRIMARY_ID, 0);
			MES_SET(sequence_id + 2);
			MES_PUT(PRIMARY_ID, 0);
			WAIT(70);
			OBJ_MODE(DERIVED_ID, 3);
			WAIT(60);
			OBJ_SPD(DERIVED_ID, 26214, 13107);
			OBJ_WALK(DERIVED_ID, 880, 112);
			Engine_ActorWaitForMove(DERIVED_ID);
			OBJ_DIR(DERIVED_ID, 53248, 0);
			MES_SET(sequence_id + 3);
			MES_PUT(PRIMARY_ID, 0);
			OBJ_MODE(PRIMARY_ID, 3);
			WAIT(70);
			Actor_SetDestinationOffset(PRIMARY_ID, 8, 0);
			Engine_ActorWaitForMove(PRIMARY_ID);
			OBJ_MODE(PRIMARY_ID, 5);
			MES_SET(sequence_id + 4);
			MES_PUT(PRIMARY_ID, 0);
			OBJ_WALK(0, 896, 120);
			Engine_ActorWaitForMove(0);
			Engine_ActorFaceEachOther(0, DERIVED_ID, 0);
			WAIT(60);
			OBJ_MODE(DERIVED_ID, 3);
			WAIT(30);
			FLAG_SET(788);
		}
		else
		{
			MES_SET((s32)MsgRunpaWellWorriedDodonpa);
			MES_PUT(PRIMARY_ID, 0);
		}
	}
	else
	{
		MES_SET((s32)MsgRunpaZZZZ);
		MES_PUT(PRIMARY_ID, 0);
	}
}

void EV26_SET(void)
{
	Engine_BattleFxRunPageEffectForSlot(26, 1, 5);
	Engine_GameFlagSet(0x94e);
}

void EV14_SET(void)
{
	Engine_ActorRunRepeatedMotion(14, 2);
	MES_SET((s32)MsgRunpaOwwwDontHurt);
	MES_PUT(14, 0);
}

void EV13_SET(void)
{
	Engine_ActorRunRepeatedMotion(13, 2);
	MES_SET((s32)MsgRunpaRightRightGive);
	MES_PUT(13, 0);
}

void EV12_SET(void)
{
	Engine_ActorRunRepeatedMotion(12, 2);
	MES_SET((s32)MsgRunpaGuysTougherThought);
	MES_PUT(12, 0);
}

void EV18_SET(void)
{
	MES_SET((s32)MsgRunpaKnowWhereDodonpa);
	Engine_EventAskYesNo(18, 0);
}

void EV20_MAIN(void)
{
	u32 i;
	s32 record;
	s32 msg;
	s32 msg2;

	if (FLAG_CHK(0x226) != 0)
	{
		MES_SET((s32)MsgRunpaWontTellAnyone);
		MES_PUT(20, 0);
	}
	else
	{
		EV_BEGIN();
		OBJ_FACE(20, ACTOR_PARTY_LEADER, 0);
		if (FLAG_CHK(0x227) == 0)
		{
			Engine_ActorJump(20, 4, 0);
			Engine_ActorStop(20);
			Object_RefreshSelectorById(20);
			WAIT(20);
			msg = (s32)MsgRunpaWhWhWho;
			MES_SET(msg);
			MES_PUT(20, 0);
			OBJ_EMO(20, 0x102, 30);
			MES_SET(msg + 1);
			MES_PUT(20, 0);
			WAIT(30);
			OBJ_MODE(20, 4);
			WAIT(30);
		}
		msg2 = (s32)MsgRunpaDontLookNearly;
		MES_SET(msg2);
		MES_PUT(20, 0);
		OBJ_EMO(20, 0x101, 40);
		MES_SET(msg2 + 1);
		MES_OPEN(20, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			MES_SET(msg2 + 2);
			MES_OPEN(20, 0);
			FLAG_SET(0x226);
		}
		else
		{
			MES_SET(msg2 + 3);
			MES_OPEN(20, 0);
		}
		FLAG_SET(0x227);
		EV_END();
	}
}

void EV20_END(void)
{

	if (Engine_GameFlagIsSet(0x226))
	{
		MES_SET((s32)MsgRunpaMaybeMerchantReason);
		MES_PUT(20, 0);
	}
	else
	{
		s16 *q = &gEventWork->psynergy_request;

		*q = 0;
		Engine_PsynergyCancel();
		EV20_MAIN();
	}
}

void NoOpActorCallback(void)
{
}

void EV17_TALK(void)
{
	s32 msg = (s32)MsgRunpaWrongTurnOver;

	MES_SET(msg);
	MES_PUT(0x800d, 0);
	if (Engine_PartyInventoryFindOwner(234) != -1)
	{
		Engine_MessageShowCentered(msg + 2, 1);
	}
}

void EV17_LOAD(void)
{
	MES_SET((s32)MsgRunpaCantBelieveWhen);
	MES_PUT(13, 0);
}

s32 FieldScene_DispatchActorUpdate(void)
{
	struct FieldActor *actor;

	gRunpaJoRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
	if (gGameState.scene == (s32)&SceneId_RunpaJo1)
	{
		Engine_MapSetWorkFlagBits9To11(0xe00);
		FieldScene_InstallSceneTasks();
	}
	if (gGameState.scene == (s32)&SceneId_RunpaJo2)
		FieldScene_SetupActorsForScene();
	if (gGameState.scene == (s32)&SceneId_RunpaJo3)
		FieldScene_RestoreActorsFromFlags();
	if (gGameState.scene == (s32)&SceneId_RunpaJo4)
	{
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
		Engine_ActorSetSpriteFlags(Actor_Get(12), 0);
		OBJ_DIR(12, 0, 0);
		OBJ_MODE(12, 0);
		ActivateFiveActorGroupFromFlags();
		actor = Actor_Get(8);
		if (actor != 0)
			Engine_ActorSetSpriteFlags(actor, 0);
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Actor_Get(9);
		if (actor != 0)
			Engine_ActorSetSpriteFlags(actor, 0);
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor = Actor_Get(10);
		if (actor != 0)
			Engine_ActorSetSpriteFlags(actor, 0);
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		Engine_MapSetWorkFlagBits9To11(0xe00);
		if (gGameState.entrance == 4)
		{
			Engine_MapSetWorkFlagBits9To11(0xc00);
			FieldScene_RunMainScriptSequence();
		}
		if (gGameState.entrance == 3)
		{
			Engine_MapSetWorkFlagBits9To11(0xc00);
			if (FLAG_CHK(0x941) != 0)
			{
				OBJ_POS_SET(12, 0, 0);
				OBJ_POS_SET(16, 432, 344);
				OBJ_DIR(16, 0x5000, 0);
				OBJ_POS_SET(13, 456, 288);
				OBJ_DIR(13, 0x5000, 0);
				OBJ_POS_SET(17, 456, 320);
				Engine_ActorSetSpriteFlags(Actor_Get(17), 0);
			}
		}
		actor = Actor_Get(15);
		if (actor != 0)
			Engine_ActorSetSpriteFlags(actor, 0);
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
		actor->scale_x = 0xcccc;
	}
	return 0;
}

void FieldScene_InstallSceneTasks(void)
{
	FieldScene_ActivateThreeActorGroup();
	switch (gGameState.entrance)
	{
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Task_AddCallback((s32)TriggerSceneStage95FromActor12, 3200);
		Task_AddCallback((s32)FieldScene_RunScene3bfSequenceB, 3200);
		Task_AddCallback((s32)FieldScene_RunScene3bfSequenceC, 3200);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		break;
	case 12:
	case 19:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		Engine_MapSetWorkFlagBits9To11(0xc00);
		break;
	case 16:
	case 17:
	case 18:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Task_AddCallback((s32)FieldScene_UpdateActorEighteenInteraction, 3200);
		Task_AddCallback((s32)TriggerScene41AtVillagePath, 3200);
		Engine_TaskWait(1);
		Engine_MapRedraw();
		Engine_TaskWait(1);
		Map_CopyCells(101, 9, 10, 8, 110, 9);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		break;
	case 13:
	case 14:
	case 15:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Task_AddCallback((s32)FieldScene_RunScene3bfSequenceA, 3200);
		break;
	default:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		break;
	}
	Engine_ActorSetChildValue(18, 1);
	Engine_ActorSetChildValue(17, 1);
	Engine_ActorSetChildValue(21, 1);
	Engine_ActorSetChildValue(12, 1);
	Engine_ActorSetChildValue(13, 1);
	Engine_TaskWait(1);
}

void FieldScene_SetupActorsForScene(void)
{
	struct FieldActor *actor;

	FieldScene_ActivateTwoActorGroup();
	Engine_ActorSetChildValue(9, 1);
	Engine_ActorSetChildValue(10, 1);
	Engine_ActorSetChildValue(17, 1);
	if (FLAG_CHK(0x94c))
	{
		OBJ_POS_SET(15, 0, 0);
	}
	if (FLAG_CHK(0x949))
	{
		OBJ_POS_SET(11, 0, 0);
	}
	if (FLAG_CHK(0x94b))
	{
		OBJ_POS_SET(16, 0, 0);
	}
	if (FLAG_CHK(0xf2e))
	{
		OBJ_POS_SET(8, 0, 0);
	}
	switch (gGameState.entrance)
	{
	case 1:
	case 2:
	case 3:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		Task_AddCallback(FieldScene_UpdateActorPairInteraction, 3200);
		Engine_TaskWait(1);
		Engine_MapRedraw();
		Engine_TaskWait(1);
		break;
	case 10:
	case 13:
	case 20:
	case 23:
	case 24:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		Engine_MapSetWorkFlagBits9To11(0xc00);
		Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
		if (FLAG_CHK(0x314))
		{
			OBJ_POS_SET(25, 872, 120);
		}
		break;
	case 21:
	case 22:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		Task_AddCallback(FieldScene_UpdateActorSeventeenInteraction, 3200);
		Engine_TaskWait(1);
		Engine_MapRedraw();
		Engine_TaskWait(1);
		break;
	case 11:
	case 12:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		if (FLAG_CHK(0x94a))
		{
			FieldScene_RunSequenceTail();
		}
		break;
	case 31:
		gEventWork->start_transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, RUNPA_RETURN_TRANSITION_VARIANT);
		FieldScene_RunSequenceTail();
		break;
	case 14:
	case 15:
	case 16:
		Task_AddCallback(TriggerScene40AtVillagePath, 3200);
		break;
	default:
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		Engine_MapSetWorkFlagBits9To11(0xe00);
		break;
	}
	actor = Actor_Get(8);
	Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
	Engine_ActorSetSpritePriority(8, 1);
	actor->scale_x = 0xc000;
	actor->scale_y = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
	struct FieldActor *actor;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	FieldScene_ActivateAlternateActorGroup();
	if (FLAG_CHK(0x943))
	{
		PlaceActorTwelveAndFinishScene();
	}
	FLAG_SET(0x217);
	FLAG_SET(0x218);
	if (FLAG_CHK(0x944))
	{
		OBJ_POS_SET(8, 0, 0);
		Engine_GameFlagClear(0x217);
	}
	if (FLAG_CHK(0x945))
	{
		OBJ_POS_SET(9, 0, 0);
		ConfigureInteractionRegionC();
	}
	if (FLAG_CHK(0x946))
	{
		OBJ_POS_SET(10, 0, 0);
		Engine_GameFlagClear(0x218);
	}
	if (FLAG_CHK(0x947))
	{
		ConfigureInteractionRegionA();
	}
	if (FLAG_CHK(0x948))
	{
		ConfigureInteractionRegionB();
	}
	EV_BEGIN();
	actor = Actor_Get(8);
	if (actor != 0)
	{
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	}
	actor = Actor_Get(9);
	if (actor != 0)
	{
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	}
	actor = Actor_Get(10);
	if (actor != 0)
	{
		actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	}
	actor = Actor_Get(11);
	if (actor != 0)
	{
		Engine_ActorSetSpriteFlags(actor, 0);
	}
	actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
	actor = Actor_Get(12);
	if (actor != 0)
	{
		actor->collision_flags |= 0x10;
	}
	Engine_ActorSetSpriteFlags(Actor_Get(11), 0);
	EV_END();
	Engine_MapSetWorkFlagBits9To11(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
	if (Engine_GameFlagIsSet(0x35a))
	{
		PlaceSceneObjectPairFromTableC(0);
	}
	if (Engine_GameFlagIsSet(0x35b))
	{
		PlaceSceneObjectPairFromTableC(1);
	}
	if (Engine_GameFlagIsSet(0x35c))
	{
		PlaceSceneObjectPairFromTableC(2);
	}
}

void FieldScene_ActivateTwoActorGroup(void)
{
	if (Engine_GameFlagIsSet(0x358))
	{
		PlaceSceneObjectPairFromTableB(0);
	}
	if (Engine_GameFlagIsSet(0x359))
	{
		PlaceSceneObjectPairFromTableB(1);
	}
}

void FieldScene_ActivateAlternateActorGroup(void)
{
	if (Engine_GameFlagIsSet(0x355))
	{
		FieldScene_SetPositionPairs(0);
	}
	if (Engine_GameFlagIsSet(0x356))
	{
		FieldScene_SetPositionPairs(1);
	}
	if (Engine_GameFlagIsSet(0x357))
	{
		FieldScene_SetPositionPairs(2);
	}
}

void ActivateFiveActorGroupFromFlags(void)
{
	if (Engine_GameFlagIsSet(0x350))
	{
		PlaceSceneObjectPairFromTableA(0);
	}
	if (Engine_GameFlagIsSet(0x351))
	{
		PlaceSceneObjectPairFromTableA(1);
	}
	if (Engine_GameFlagIsSet(0x352))
	{
		PlaceSceneObjectPairFromTableA(2);
	}
	if (Engine_GameFlagIsSet(0x353))
	{
		PlaceSceneObjectPairFromTableA(3);
	}
	if (Engine_GameFlagIsSet(0x354))
	{
		PlaceSceneObjectPairFromTableA(4);
	}
}
