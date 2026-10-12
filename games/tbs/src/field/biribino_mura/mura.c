#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/biribino_mura/mura.h"
#include "unit/field/common/object/facing.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
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
#include "mura.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

struct SpawnCounter
{
	s16 frames;
};

struct SpawnCounter gCornerSpawnCounter;

// 向きごとの一歩: 表は s16 の (z, x) 組、コードは一語で読む
struct FacingCellStepWords
{
	s32 step[16];
};

#define FACING_CELL_STEP(dir) \
	(((struct FacingCellStepWords *)BiribinoMura_FacingCellSteps)->step[dir])

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/facing.c"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_BiribinoMura1)
	{
		return gBiribinoMuraEntrances1;
	}
	if (scene == (s32)&SceneId_BiribinoMura3)
	{
		return gBiribinoMuraEntrances3;
	}
	if (scene == (s32)&SceneId_BiribinoMura2)
	{
		return gBiribinoMuraEntrances2;
	}
	return (const struct SceneEntrance *)gBiribinoMuraEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_BiribinoMura2)
	{
		return (const struct SceneRegion *)gBiribinoMuraRegions2;
	}
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

void SceneState_SetValues9_3_0(void)
{
	BattleFx_RunPageEffectForSlot(9, 3, 0);
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_BiribinoMura1)
	{
		return gBiribinoMuraPlacements1;
	}
	if (scene == (s32)&SceneId_BiribinoMura3)
	{
		return gBiribinoMuraPlacements3;
	}
	if (scene == (s32)&SceneId_BiribinoMura2)
	{
		return gBiribinoMuraPlacements2;
	}
	return gBiribinoMuraPlacementsOther;
}

void FieldScene_RunScriptedSteps947And29DD(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
	Engine_MessageShowCentered((s32)MsgBiribinoBottomNotVisibleLooksVery, 1);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_BiribinoMura1)
	{
		return gBiribinoMuraEvents1;
	}
	if (scene == (s32)&SceneId_BiribinoMura3)
	{
		return gBiribinoMuraEvents3;
	}
	if (scene == (s32)&SceneId_BiribinoMura2)
	{
		return gBiribinoMuraEvents2;
	}
	return gBiribinoMuraEventsOther;
}

void FieldScene_RunScriptedStep1472(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgBiribinoAreaOffLimitsThoseWithout, 1);
	Engine_EventEnd();
}

void FieldScene_RunScriptedStep146E(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgBiribinoMccoysHiddenWarehouseDoNot, 1);
	Engine_EventEnd();
}

void SceneDialogue_RunLine1470(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgBiribinoThereTreeLooksLikePerson, 1);
	Engine_EventEnd();
}

void BiribinoMura_TalkAboutHumanTree(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoItsTreeButAlmostLooks);
	if (GameFlag_IsSet(0x301) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessage(9, 0);
	GameFlag_Set(0x301);
	Engine_EventEnd();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoDidSeeTreeAtEntrance);
	Engine_EventAskYesNo(12, 0);
	Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoHaveTriedHeadingSoutheastFrom);
	Engine_EventAskYesNo(14, 0);
	Engine_EventEnd();
}

void SceneDialogue_ShowLine16BF(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoWasTurnedIntoTreeFor);
	Engine_EventAskYesNo(21, 0);
	Engine_EventEnd();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoCurseWasBrokenThanksEfforts);
	Engine_EventAskYesNo(16, 0);
	Engine_EventEnd();
}

void SceneDialogue_ShowLine16CC(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoHaveEverBeenVillageImil);
	Engine_EventAskYesNo(18, 0);
	Engine_EventEnd();
}

void FieldScene_RunEarlySequence(void)
{

	u32				   i;
	struct FieldActor *record;
	s32				   v5;
	u8				  *tbl;
	u8				  *tbl2;
	s32				   off;
	s32				   off2;
	s32				   a1;
	s32				   a2;
	u8				  *p7;

	p7 = *(u8 **)&gEventWork;
	Engine_EventBegin();
	for (i = 8; i < 66; i++)
	{
		record = Object_GetById(i);
		if (record != 0)
		{
			record->motion_flags = 0;
		}
	}
	v5 = ((*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
	if (v5 == 6)
	{
		Event_PlayCue(188);
	}
	else
	{
		Engine_AudioPlayCue(158);
	}
	off = v5 << 2;
	tbl = (u8 *)Mura_DoorCellOrigins;
	a1 = *(s16 *)(tbl + off);
	off2 = off + 2;
	a2 = *(s16 *)(tbl + off2);
	tbl2 = (u8 *)Mura_DoorCellSteps;
	Engine_MapAnimateCells(*(s32 *)(tbl2 + off), a1, a2);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Object_GetById(0)->motion_flags = 0;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if (v5 == 6)
	{
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
		Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
	}
	else
	{
		Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	}
	if (v5 == 4)
	{
		Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	}
	else
	{
		Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	}
	Engine_EventWait(16);
	Engine_EventRequestExit(v5 + 3);
	Engine_EventEnd();
}

void FieldScene_RunScene38bSequenceC(void)
{
	struct FieldActor *rec;
	struct FieldActor *rec7;

	rec = Object_GetById(ACTOR_PARTY_LEADER);
	rec7 = Object_GetById(11);
	if ((rec7->x.fixed >> 20) == 6)
	{
		Engine_EventBegin();
		Engine_ActorSetSpritePriority(11, 1);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_EventWait(20);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
		Actor_SetSpeed(11, 0x3333, 0x1999);
		Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
		rec7->motion_flags = 0;
		rec->scale_x = -0x10000;
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
		Actor_MoveToAndWait(11, 111, 196);
		rec->scale_x = 0x10000;
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
		Engine_EventWait(20);
		rec->scale_x = -0x10000;
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
		Actor_MoveToAndWait(11, 121, 190);
		rec->scale_x = 0x10000;
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
		Engine_EventWait(20);
		rec->scale_x = -0x10000;
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
		Actor_MoveToAndWait(11, 132, 186);
		rec->scale_x = 0x10000;
		Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
		Engine_ActorSetSpritePriority(11, 2);
		RunSceneTransitionEffect(0, 11);
		Engine_TaskWait(10);
		Engine_EventSetMessage((s32)MsgBiribinoThankSavedMeFromBeing);
		Event_ShowMessage(11, 0);
		Engine_PsynergyCancel();
		Engine_TaskWait(10);
		GameFlag_Set(0x848);
		Engine_EventEnd();
	}
}

void BiribinoMura_CheckLeaderAhead(void)
{
	Engine_LeaderCheckAhead();
}

void BiribinoMura_ReceiveJillsGift(void)
{
	u32				   i;
	struct FieldActor *rec7;
	struct FieldActor *record;

	rec7 = Object_GetById(ACTOR_PARTY_LEADER);
	if (GameFlag_IsSet(0x845) == 0)
	{
	}
	else
	{
		if (GameFlag_IsSet(0x848) == 0)
		{
		}
		else
		{
			Engine_EventBegin();
			Camera_SetSpeed(0x26666, 0x4ccc);
			Camera_MoveTo(0x1070000, -1, 0xad0000, 1);
			Engine_CameraWaitForMove();
			record = Actor_Get(12);
			if (record->x.fixed > rec7->x.fixed)
			{
				Actor_FaceDirection(13, 0x5000, 20);
				Actor_ShowEmote(13, 0x100, 20);
				Engine_EventSetMessage((s32)MsgBiribinoYoureGuy);
				Event_ShowMessageAndWait(13, 0, 10);
				Actor_ShowEmote(12, 0x100, 0);
			}
			else
			{
				Actor_FaceDirection(12, 0x3000, 20);
				Actor_ShowEmote(12, 0x100, 20);
				Engine_EventSetMessage((s32)MsgBiribinoYoureGuy);
				Event_ShowMessageAndWait(12, 0, 10);
				Actor_ShowEmote(13, 0x100, 0);
			}
			Actor_ShowEmote(14, 0x100, 0);
			Actor_FaceDirection(14, 0x3000, 0);
			Actor_FaceDirection(12, 0x5000, 0);
			Actor_FaceDirection(13, 0x3000, 0);
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x10c, 184);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
			Engine_ActorRunRepeatedMotion(13, 2);
			Event_ShowMessageAndWait(13, 0, 10);
			Actor_FaceDirection(13, 0, 0);
			Actor_FaceDirection(14, 0x3000, 20);
			Actor_FaceDirection(12, 0x8000, 20);
			Engine_ActorSetAnimationAndWait(12, 3);
			Actor_SetAttachedEffect(14, 0x102);
			Engine_EventWait(40);
			Actor_FaceDirection(14, 0x3000, 10);
			Actor_FaceDirection(12, 0x5000, 0);
			Actor_FaceDirection(13, 0x3000, 10);
			Engine_ActorRunRepeatedMotion(14, 1);
			Event_ShowMessageAndWait(14, 0, 10);
			Engine_ActorSetAnimation(12, 3);
			Engine_ActorSetAnimationAndWait(13, 3);
			Engine_EventWait(20);
			Event_ShowMessage(14, 0);
			Actor_SetSpeed(14, 0x9999, 0x4ccc);
			Object_GetById(14)->steering_flags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(14, 0x10a, 172);
			Engine_EventWait(1);
			Object_GetById(14)->steering_flags |= ACTOR_FACES_MOTION;
			Engine_EventWait(10);
			Engine_ActorSetAnimationAndWait(14, 3);
			Event_ShowMessageAndWait(14, 0, 10);
			Engine_MessageShowCentered((s32)MsgBiribinoJillGaveRobinSpecialGift, 1);
			gEventWork->message++;
			Engine_ItemShowFound(ITEM_HARD_NUT, 3);
			Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
			Engine_ActorSetAnimationAndWait(14, 3);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
			Actor_SetSpeed(14, 0x10000, 0x8000);
			Object_GetById(14)->steering_flags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(14, 0x106, 156);
			Engine_EventWait(1);
			{
				u8 *record = Actor_Get(14);
				u8	value = record[90] | 1;

				record[90] = value;
			}
			Engine_EventWait(20);
			Engine_ActorRunRepeatedMotion(12, 2);
			Event_ShowMessageAndWait(12, 0, 10);
			Engine_ActorSetAnimation(12, 3);
			Engine_ActorSetAnimation(13, 3);
			Engine_ActorSetAnimationAndWait(14, 3);
			FieldObject_SetTargetAndCallback(12, 0x10000, Mura_VillagerActions);
			FieldObject_SetTargetAndCallback(13, 0x10000, Mura_VillagerActions);
			FieldObject_SetTargetAndCallback(14, 0x10000, Mura_VillagerActions);
			GameFlag_Set(0x849);
			Engine_EventEnd();
		}
	}
}

s32 Scene_Initialize(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if (gGameState.scene == (s32)&SceneId_BiribinoMura1)
	{
		BiribinoMura_RestoreVillageActors();
	}
	else
	{
		if (gGameState.scene == (s32)&SceneId_BiribinoMura3)
		{
			FieldScene_RunScene38bSequenceA();
			Engine_TaskAddCallback(BiribinoMura_UpdateCornerSpawn, 0xc80);
		}
		else
		{
			if (gGameState.scene == (s32)&SceneId_BiribinoMura2)
			{
				BiribinoMura_RestoreRaisedEntrance();
			}
		}
	}
	return 0;
}

void BiribinoMura_RestoreVillageActors(void)
{

	struct FieldActor *record;
	s16				   sub_state;

	if (GameFlag_IsSet(0x845) != 0)
	{
		Engine_ActorSetPosition(9, 0, 0);
		Actor_FaceDirection(14, 0x3000, 0);
		Actor_FaceDirection(15, 0x5000, 0);
	}
	else
	{
		record = Actor_Get(9);
		Engine_ActorSetSpriteFlags(record, 0);
		Actor_SetPosition(21, 0, 0);
	}
	record = Actor_Get(8);
	record->scale_y = 0x18000;
	{
		s32 off = 450;
		sub_state = *(s16 *)((u8 *)&gGameState + off);
	}
	if (sub_state == 10)
	{
		Actor_SetPosition(8, 0, 0);
	}
	else
	{
		if (sub_state == 9)
		{
			GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
		}
	}
	if (GameFlag_IsSet(0x109) == 0)
	{
		{
			s32 off = 450;
			sub_state = *(s16 *)((u8 *)&gGameState + off);
		}
		if (sub_state == 11)
		{
			Actor_SetPosition(20, 0xf80000, 0xd80000);
		}
	}
	Scene_UpdatePuzzleActors();
	if (GameFlag_IsSet(0x84a) != 0)
	{
		if (GameFlag_IsSet(0x84b) == 0)
		{
			GameFlag_Set(0x304);
		}
	}
}

void Scene_UpdatePuzzleActors(void)
{
	s32				   p10;
	s32				   p9;
	struct FieldActor *rec7;
	struct FieldActor *record;
	s32				   p6;
	s32				   row;

	rec7 = Object_GetById(ACTOR_PARTY_LEADER);
	record = Object_GetById(20);
	row = record->z.fixed >> 20;
	p9 = (rec7->x.fixed >> 20);
	p10 = (rec7->z.fixed >> 20);
	p6 = record->x.fixed;
	Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
	Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
	Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
	Map_CopyCellAttributes(1, 0, 1, 1, (p6 >> 20), row);
	if ((p6 >> 20) == 16)
	{
		if (row == 13)
		{
			goto L_02000a60;
		}
	}
	Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
L_02000a60:;
	if (p9 == 16)
	{
		if (p10 == 13)
		{
			Engine_EventBegin();
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
			Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
			if (row == 13)
			{
				Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x106, 196);
				Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
			}
			else
			{
				Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x11e, 218);
				Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
			}
			Engine_EventEnd();
		}
	}
}

void FieldScene_RunScene38bSequenceA(void)
{
	u32				   i;
	struct FieldActor *rec;
	s32				   rec7;
	struct FieldActor *rec8;
	s32				   record;

	rec8 = Object_GetById(10);
	rec = Object_GetById(11);
	record = Actor_Get(8);
	Engine_ActorSetSpriteFlags(record, 0);
	rec7 = GameFlag_IsSet(0x845);
	if (rec7 != 0)
	{
		Actor_SetPosition(9, 0, 0);
		Actor_SetPosition(10, 0, 0);
		Actor_SetPosition(11, 0, 0);
		Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
		Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
		if (GameFlag_IsSet(0x849) == 0)
		{
			if (GameFlag_IsSet(0x848) != 0)
			{
				goto L_02000c92;
			}
			Engine_ActorSetPosition(14, 0, 0);
		}
		Actor_FaceDirection(12, 0xd000, 0);
		Actor_FaceDirection(13, 0xb000, 0);
	}
	else
	{
		Actor_SetPosition(12, 0, 0);
		Actor_SetPosition(13, 0, 0);
		Actor_SetPosition(14, 0, 0);
		record = Actor_Get(9);
		Engine_ActorSetSpriteFlags(record, 0);
		record = Actor_Get(10);
		Engine_ActorSetSpriteFlags(record, 0);
		record = Actor_Get(11);
		Engine_ActorSetSpriteFlags(record, 0);
		rec8->motion_flags = rec7;
		record = GameFlag_IsSet(0x881);
		if (record != 0)
		{
			Object_GetById(9)->collision_flags |= 16;
			Object_GetById(16)->collision_flags |= 16;
			Object_GetById(11)->collision_flags |= 16;
			Actor_SetPosition(16, 0x8e0000, 0x9c0000);
			record = Actor_Get(16);
			Engine_ActorSetSpriteFlags(record, 0);
			Actor_SetPosition(10, 0x8e0000, 0x9c0000);
			rec8->sprite->rotation = 0x4000;
			rec8->y.fixed += -0x80000;
			if (GameFlag_IsSet(0x848) != 0)
			{
				Actor_SetPosition(11, 0x840000, 0xba0000);
				goto L_02000c92;
			}
			Actor_SetPosition(11, 0x580000, 0xc40000);
			Engine_ActorSetSpritePriority(11, 3);
			rec->collision_flags |= 4;
		}
		else
		{
			rec8->y.fixed = 0x200000;
			rec->motion_flags = record;
			rec->y.fixed = 0x300000;
		}
	}
L_02000c92:;
	ActorPresentation_RepaintTenCellsAndActorEightCell();
}

void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
	struct FieldActor *actor;
	s32				   tx;
	s32				   tz;
	u32				   i;

	actor = Object_GetById(8);
	tx = actor->x.fixed >> 20;
	tz = actor->z.fixed >> 20;

	for (i = 0; i < 20; i += 2)
	{
		s32 x = ((u8 *)Mura_RepaintCells)[i];
		s32 z = ((u8 *)Mura_RepaintCells)[i + 1];
		Engine_MapCopyCellAttributes(1, 0, 1, 1, x, z);
	}

	Engine_MapCopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void BiribinoMura_RestoreRaisedEntrance(void)
{
	struct FieldActor *leader;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	FieldScene_DrawTilesByActor8Row();
	if (gGameState.entrance == 3 || gGameState.entrance == 4)
	{
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			leader = Object_GetById(ACTOR_PARTY_LEADER);
			Engine_EventBegin();
			leader->y.fixed = 0x100000;
			Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
			Engine_MapRedraw();
			Engine_EventEnd();
			Engine_TaskWait(1);
		}
	}
}

struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z)
{
	struct FieldActor **actors = gEventWork->placed_actors - 8;
	u32					i;

	for (i = 8; i <= 65; i++)
	{
		struct FieldActor *actor = actors[i];

		if (x == actor->x.fixed >> 20 && z == actor->z.fixed >> 20)
		{
			return actor;
		}
	}
	return 0;
}

void BiribinoMura_PushFacedBlock(void)
{
	struct FieldActor	  *leader;
	struct FieldActor	  *block;
	s32					   step;
	u32					   dir;
	union FieldCoordinate  pos[3];
	union FieldCoordinate *p;

	/* FAKEMATCH: the step table is s16 pairs read as one word each; only an array member view keeps its base and index apart, a plain s32 pointer cast adds them first (59 instructions differ, measured). */
	leader = Object_GetById(0);
	dir = leader->facing >> 12;
	block = SceneActor_FindActorAtCell((leader->x.part.pixel + (FACING_CELL_STEP(dir) >> 16)) >> 4,
		(leader->z.part.pixel + (s16)FACING_CELL_STEP(dir)) >> 4);
	if (block != NULL)
	{
		block->terrain_id = 2;
		/* FAKEMATCH: the probe is filled through a pointer to it; indexing pos directly drops a saved register and moves the step table address (measured). */
		p = pos;
		step = FACING_CELL_STEP(dir);
		p[0].fixed = block->x.fixed + (step & -0x10000);
		p[1].fixed = block->y.fixed;
		p[2].fixed = block->z.fixed + (step << 16);
		if (Object_CheckMovementCollision(block, p) <= 0)
		{
			Object_SetMode(leader, 8);
			Engine_TaskWait(15);
			Engine_AudioPlayCue(185);
			block->speed = 0x3333;
			block->acceleration = 0x3333;
			Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
			leader->speed = 0x3333;
			leader->acceleration = 0x3333;
			Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
			Engine_ObjectCommitPosition(block);
			block->x.fixed = p[0].fixed;
			block->z.fixed = p[2].fixed;
			block->velocity_x = 0;
			block->velocity_z = 0;
			Object_SetMode(leader, 1);
			if (gGameState.scene == (s32)&SceneId_BiribinoMura3)
				ActorPresentation_RepaintTenCellsAndActorEightCell();
			else if (gGameState.scene == (s32)&SceneId_BiribinoMura1)
				Scene_UpdatePuzzleActors();
			else if (gGameState.scene == (s32)&SceneId_BiribinoMura2)
				FieldScene_DrawTilesByActor8Row();
		}
	}
}

void BiribinoMura_UpdateCornerSpawn(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(gGameState.selected_actor);
	if (actor->x.fixed < 0x8e0000)
	{
		if (actor->y.fixed < 0x80000)
		{
			if (gCornerSpawnCounter.frames == 0)
				OverlayObject_SpawnKind24AtActor(actor);
			if (++gCornerSpawnCounter.frames == 30)
				gCornerSpawnCounter.frames = 0;
		}
		else
		{
			gCornerSpawnCounter.frames = 0;
		}
	}
}

void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor)
{
	struct FieldActor  *effect;
	struct FieldSprite *sprite;

	effect = Engine_ObjectCreate(24, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if (effect == NULL)
		return;

	sprite = effect->sprite;
	Engine_ObjectSetScript(effect, Mura_SpawnScript);
	effect->motion_flags = 0;
	effect->terrain_id = 1;
	effect->priority_flags = 2;
	if (sprite == NULL)
		return;

	AnimationObjects_SelectAnimation(sprite, 2);
	sprite->flags = 0;
	sprite->blend_mode = 1;
	sprite->priority = 3;
}

void FieldScene_DrawTilesByActor8Row(void)
{
	struct FieldActor *actor;

	actor = Actor_Get(8);
	if (actor == NULL)
		return;

	if (actor->z.fixed >> 20 == 6)
		Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
	else
		Map_CopyCellAttributes(0, 0, 1, 1, 14, 6);

	if (actor->z.fixed >> 20 == 9)
		Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
	else
		Map_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

s32 Mura_VillagerActions[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gBiribinoMuraEntrancesOther[] = {
	MAP_CELL_STEP(0, 65535, 168, 0, 168), MAP_CELL_STEP(16384, 0, 65535, 65535, 65535), MAP_CELL_STEPS_END,
};

struct SceneEntrance gBiribinoMuraEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 240, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 224, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 200, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 104, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 72, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 296, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 400, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 40, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 264, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gBiribinoMuraEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 56, 0, 376, FACING_EAST, 0, 16, 16, 400, 400, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 184, FACING_WEST, 0, 16, 16, 400, 400, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gBiribinoMuraEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 232, 0, 144, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 408, FACING_NORTH, 0, 0, 288, 240, 448, 0 },
	{ 2, CONDITION_ALWAYS, 120, 0, 384, FACING_SOUTH, 0, 0, 288, 240, 448, 0 },
	{ 3, CONDITION_ALWAYS, 72, 4, 128, FACING_NORTH, 0, 32, 32, 384, 304, 0 },
	{ 4, CONDITION_ALWAYS, 296, 4, 224, FACING_NORTH, 0, 32, 32, 384, 304, 0 },
	{ SCENE_TABLE_END },
};

s16 gBiribinoMuraRegions2[] = {
	66, 26, 128, 78, 38, 140, -1, 3,
	290, 26, 224, 302, 38, 236, -1, 4,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 Placement_Messages[] = {
	SCENE_EXITS(30),
	SCENE_EXIT(1, 2, 5),
	SCENE_EXIT(2, 34, 1),
	SCENE_EXIT(3, 31, 6),
	SCENE_EXIT(4, 31, 1),
	SCENE_EXIT(5, 31, 2),
	SCENE_EXIT(6, 31, 3),
	SCENE_EXIT(7, 31, 5),
	SCENE_EXIT(8, 31, 4),
	SCENE_EXIT(9, 9, 35),
	SCENE_EXIT(10, 32, 1),
	SCENE_EXIT(11, 32, 4),
	SCENE_EXIT(0, 32, 0),
	SCENE_EXIT(1, 30, 10),
	SCENE_EXIT(2, 32, 3),
	SCENE_EXIT(3, 32, 2),
	SCENE_EXIT(4, 30, 11),
	SCENE_EXIT(0, 35, 0),
	SCENE_EXIT(1, 2, 6),
	SCENE_EXIT(2, 2, 47),
	SCENE_EXITS_END,
};

struct ScenePlacement gBiribinoMuraPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gBiribinoMuraPlacements1[] = {
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(40), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x69, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(256), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(328), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(240), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(176), 0, PIXELS(328), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(128), 0, PIXELS(224), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(176), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(232), 0, PIXELS(296), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
	{ 0x87, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xf3, CONDITION_ALWAYS, 7,
		PIXELS(264), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(432), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gBiribinoMuraPlacements3[] = {
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x69, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x69, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x69, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(282), 0, PIXELS(170), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(246), 0, PIXELS(170), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(262), 0, PIXELS(156), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x17, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gBiribinoMuraPlacements2[] = {
	{ 0xf3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(100), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5d, 0x6c, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gBiribinoMuraEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gBiribinoMuraEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_TalkAboutHumanTree) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine16BF) },
	{ EVENT_TALK, 10, 0x845, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
	{ EVENT_TALK, 11, 0x845, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
	{ EVENT_TALK, 12, 0x845, EVENT_SCRIPT(SceneDialogue_RunActorTwelveDialogue) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
	{ EVENT_TALK, 13, 0x845, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
	{ EVENT_TALK, 14, 0x845, EVENT_SCRIPT(SceneDialogue_RunActorFourteenDialogue) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
	{ EVENT_TALK, 15, 0x845, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
	{ EVENT_TALK, 16, 0x845, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorSixteenDialogue) },
	{ EVENT_TALK, 17, 0x845, MSGNO(MsgBilibinIsItTrueThatAMountain) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
	{ EVENT_TALK, 18, 0x845, MSGNO(MsgBilibinTheresATowerAtTheTip) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine16CC) },
	{ EVENT_TALK, 19, 0x845, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinHelpMeSomebody) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgBilibinINeverWantToBeTurned) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x845, MSGNO(MsgBilibinYouGuysAreSoUnluckyArriving) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinNotManyLumberjacksAreWillingTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x845, MSGNO(MsgBilibinYouMustBeMadItsAlmost) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinOneOfTheTwoPeopleThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x845, MSGNO(MsgBilibinWeEndedUpLikeThisBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinThatGuyMightTurnIntoA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x845, MSGNO(MsgBilibinEveryoneGetsWorriedWhenTownLeaders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoySeemsToBeHaving) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x845, MSGNO(MsgBilibinWhereDidTheseKidsComeFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinApparentlyJustAboutAnyoneCouldJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x845, MSGNO(MsgBilibinNoOneWouldDareGoAfter) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBilibinTheGangOfThreeHaTheyre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x845, MSGNO(MsgBilibinHeBeggedForHelpAsHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinImSureLordMccoyWillGive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x845, MSGNO(MsgBilibinIWantToSeeThatPretty) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyMustBeReportingYourDeeds) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x845, MSGNO(MsgBilibinOnlyGreatAdeptsCanEnterThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBilibinIWantToShareWithYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x845, MSGNO(MsgBilibinThatExplosionMustHaveBeenA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBilibinAsLongAsImOkayI) },
	{ PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_UpdatePuzzleActors) },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ PSYNERGY_ON_ACTOR(0x4e), 8, CONDITION_ALWAYS, 0 },
	{ SEARCH(SEARCH_JAR), 100, 0xf50, FIND_ITEM(0xb4) },
	{ SEARCH(SEARCH_JAR), 101, 0xf51, FIND_COINS(5) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedSteps947And29DD) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gBiribinoMuraEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine1470) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine1470) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine1470) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine1470) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x881), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x848, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x848, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x848, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBiribinoThankSavedMeFromBeing) },
	{ EVENT_TALK, 12, 0x848, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
	{ EVENT_TALK, 13, 0x848, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x848, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x848, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
	{ SEARCH(SEARCH_UNNAMED), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedStep1472) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
	{ EVENT_TOUCH, 13, 0x849, EVENT_SCRIPT(BiribinoMura_ReceiveJillsGift) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x848, EVENT_SCRIPT(FieldScene_RunScene38bSequenceC) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ 0x202, 12, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RepaintTenCellsAndActorEightCell) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gBiribinoMuraEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedStep146E) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValues9_3_0) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DrawTilesByActor8Row) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ 0x202, 13, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_PushFacedBlock) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoMura_CheckLeaderAhead) },
	{ SCENE_EVENTS_END },
};

u16 Mura_DoorCellStep6[] = {
	MAP_CELL_STEP(37, 33, 4, 2, 5), MAP_CELL_STEP(33, 33, 4, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep4[] = {
	MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep5[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 5), MAP_CELL_STEP(33, 52, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep1[] = {
	MAP_CELL_STEP(35, 54, 2, 2, 5), MAP_CELL_STEP(33, 54, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep0[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 5), MAP_CELL_STEP(33, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep2[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 5), MAP_CELL_STEP(33, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 Mura_DoorCellStep3[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 5), MAP_CELL_STEP(33, 50, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *Mura_DoorCellSteps[] = {
	Mura_DoorCellStep0,
	Mura_DoorCellStep1,
	Mura_DoorCellStep2,
	Mura_DoorCellStep3,
	Mura_DoorCellStep4,
	Mura_DoorCellStep5,
	Mura_DoorCellStep6,
};

u32 Mura_DoorCellOrigins[] = {
	0x140036, 0x12002b, 0x130025, 0xb0029, 0xb0023, 0x90031, 0x60037,
};

u32 Mura_RepaintCells[] = {
	0x12091208, 0x1308120a, 0x130a1309, 0x1409130b, 0x140b140a,
};

s16 BiribinoMura_FacingCellSteps[] = {
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

s32 Mura_SpawnScript[] = {
	SC_WAIT(30),
	SC_FREE,
};
