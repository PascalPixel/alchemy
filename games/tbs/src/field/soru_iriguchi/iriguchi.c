#include "game_state.h"
#include "unit/import.h"
#include "unit/field/soru_iriguchi/iriguchi.h"
#include "unit/battle/effect/object.h"
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
#include "field_scene.h"
#include "field_service.h"
#include "soru.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "fieldobj.h"

// 前方宣言
static void SoruIriguchi_ApplyEntryState(void);
static void FieldScene_RunSceneEntryHook(void);
static void Scene_EnterSolSanctum(void);
static void Scene_SukuretaSuspectsHiddenPassage(void);
static void Scene_RunTransitionCue(void);
static void Scene_RunActorFormation(s32 a0);
static void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

s32 SoruIriguchi_CueTimer __attribute__((section(".bss")));

void InitializeSceneRecordBuffer();

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_SoruIriguchi2)
	{
		return gSoruIriguchiEntrances2;
	}
	if (scene == (s32)&SceneId_SoruIriguchi1)
	{
		return gSoruIriguchiEntrances1;
	}
	return gSoruIriguchiEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return SoruIriguchi_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s32 table;
	s32 lo = 11;

	if (gGameState.scene == (s32)&SceneId_SoruIriguchi1)
	{
		if (gGameState.entrance >= lo)
		{
			if (gGameState.entrance > 13)
			{
				if (gGameState.entrance > 16)
				{
					goto other_entrance;
				}
				return gSoruIriguchiPlacements1Entrances14To16;
			}
			return gSoruIriguchiPlacements1Entrances11To13;
		}
	other_entrance:;
		table = (s32)gSoruIriguchiPlacements1;
		FieldScene_PrepareActors(table);
		return (const struct ScenePlacement *)table;
	}
	if (gGameState.scene == (s32)&SceneId_SoruIriguchi2)
	{
		return gSoruIriguchiPlacements2;
	}
	return gSoruIriguchiPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s32 lo = 11;

	if (gGameState.scene == (s32)&SceneId_SoruIriguchi2)
	{
		return gSoruIriguchiEvents2;
	}
	if (gGameState.scene == (s32)&SceneId_SoruIriguchi1)
	{
		if (gGameState.entrance >= lo)
		{
			if (gGameState.entrance > 13)
			{
				if (gGameState.entrance > 16)
				{
					goto other_entrance;
				}
				return gSoruIriguchiEvents1Entrances14To16;
			}
			return gSoruIriguchiEvents1Entrances11To13;
		}
	other_entrance:;
		return gSoruIriguchiEvents1;
	}
	else
	{
	}
	return gSoruIriguchiEventsOther;
}

static void EV02_TOUCH(void)
{
	s32 v5;
	s32 v6;

	Engine_EventBegin();
	v5 = 3;
	v6 = 2;
	Event_PlayCue(181);
	Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
	Engine_TaskWait(10);
	Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
	Engine_TaskWait(10);
	Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
	Engine_TaskWait(10);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	Engine_EventWait(10);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(2);
	Engine_EventEnd();
}

static void EV20_SEARCH(void)
{

	Engine_EventBegin();

	if (Engine_GameFlagIsSet(0x81a) != 0)
	{
		Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
	}
	else
	{
		Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
		if (Engine_GameFlagIsSet(0xf01) != 0)
		{
			u16 *p = &gEventWork->unknown_172;
			u16	 val = 1;
			*p = val;
		}
	}

	Engine_EventEnd();
}

static void EV1044_SCRIPT(void)
{
	s32 id;
	s32 v5;
	s32 v6;

	if (GameFlag_IsSet(0xf01) == 0)
	{
	}
	else
	{
		if (GameFlag_IsSet(0x81a) != 0)
		{
		}
		else
		{
			Engine_EventBegin();
			Battle_ResetEffectCounter();
			v5 = 1;
			Event_PlayCue(182);
			Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
			Engine_MapRedraw();
			Engine_EventWait(40);
			id = (s32)MsgSoruSetSmallGem;
			Engine_MessageShowCentered(id, 1);
			Engine_EventWait(20);
			v6 = 3;
			Event_PlayCue(183);
			Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
			Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
			Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
			Engine_MapRedraw();
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
			Engine_EventWait(20);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
			Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
			Engine_EventWait(20);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
			Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
			Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_EventWait(40);
			Engine_MessageShowCentered(id + 1, 1);
			GameFlag_Set(0x143);
			GameFlag_Set(0x81a);
			Engine_EventEnd();
		}
	}
}

static void EV21_SEARCH(void)
{

	struct EventWork *work;

	Engine_EventBegin();

	if (Engine_GameFlagIsSet(0x821) != 0)
	{
		Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
	}
	else if (Engine_GameFlagIsSet(0xf02) != 0)
	{
		work = gEventWork;
		Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
		{

			u16 *frame = &work->unknown_172;
			s32	 one = 1;
			*frame = one;
		}
	}
	else
	{
		Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
	}

	Engine_EventEnd();
}

static void EV1045_SCRIPT(void)
{
	s32 message;

	if (GameFlag_IsSet(3842) != 0 && GameFlag_IsSet(GATE_ID) == 0)
	{
		Engine_EventBegin();
		Battle_ResetEffectCounter();
		Engine_AudioPlayCue(182);
		Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
		Engine_MapRedraw();
		Engine_EventWait(40);
		message = (s32)MsgSoruSetSmallGem;
		Engine_MessageShowCentered(message, 1);
		Engine_EventWait(20);
		Engine_AudioPlayCue(183);
		Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
		Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
		Engine_MapRedraw();
		Work_SetValuesIfNonNegative(65536, 65536, 65536);
		Engine_EventWait(20);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
		Work_SetValuesIfNonNegative(131072, 131072, 65536);
		Engine_EventWait(20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
		Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
		Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
		Work_SetValuesIfNonNegative(-1, -1, 58982);
		Engine_EventWait(40);
		Engine_MessageShowCentered(message + 1, 1);
		GameFlag_Set(0x143);
		GameFlag_Set(GATE_ID);
		Engine_EventEnd();
	}
}

static void Scene_UpdateOuterActor9Flags(void)
{
	s32 *work = Object_GetById(9);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x302);
	GameFlag_Clear(0x303);
	if (pos == 93)
	{
		Engine_GameFlagSet(0x303);
	}
	else if (pos == 95)
	{
		Engine_GameFlagSet(0x302);
	}
}

static void Scene_UpdateOuterActor10Flags(void)
{
	s32 *work = Object_GetById(10);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x300);
	GameFlag_Clear(0x301);
	if (pos == 115)
	{
		Engine_GameFlagSet(0x300);
	}
	else if (pos == 113)
	{
		Engine_GameFlagSet(0x301);
	}
}

static void Scene_UpdateFormationActor9Flags(void)
{
	s32 *work = Object_GetById(9);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x310);
	GameFlag_Clear(0x311);
	if (pos == 99)
	{
		Engine_GameFlagSet(0x311);
	}
	else if (pos == 101)
	{
		Engine_GameFlagSet(0x310);
	}
	Scene_RunActorFormation(0);
}

static void Scene_UpdateFormationActor10Flags(void)
{
	s32 *work = Object_GetById(10);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x312);
	GameFlag_Clear(0x313);
	if (pos == 103)
	{
		Engine_GameFlagSet(0x313);
	}
	else if (pos == 105)
	{
		Engine_GameFlagSet(0x312);
	}
	Scene_RunActorFormation(0);
}

static void EV11_PSYNERGY(void)
{
	s32 *work = Object_GetById(11);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x314);
	GameFlag_Clear(0x315);
	if (pos == 107)
	{
		Engine_GameFlagSet(0x315);
	}
	else if (pos == 109)
	{
		Engine_GameFlagSet(0x314);
	}
	Scene_RunActorFormation(0);
}

static void EV12_PSYNERGY(void)
{
	s32 *work = Object_GetById(12);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x316);
	GameFlag_Clear(0x317);
	if (pos == 111)
	{
		Engine_GameFlagSet(0x317);
	}
	else if (pos == 113)
	{
		Engine_GameFlagSet(0x316);
	}
	Scene_RunActorFormation(0);
}

static void EV13_PSYNERGY(void)
{
	s32 *work = Object_GetById(13);
	s32	 pos;

	if (work == 0)
	{
		return;
	}
	pos = work[2] >> 20;
	GameFlag_Clear(0x318);
	GameFlag_Clear(0x319);
	if (pos == 115)
	{
		Engine_GameFlagSet(0x319);
	}
	else if (pos == 117)
	{
		Engine_GameFlagSet(0x318);
	}
	Scene_RunActorFormation(0);
}

static void EV14_PSYNERGY(void)
{
	s32 *work = Object_GetById(14);
	s32	 pos;

	if (work == 0)
		return;
	pos = work[2] >> 20;
	GameFlag_Clear(0x31a);
	GameFlag_Clear(0x31b);
	if (pos == 119)
	{
		Engine_GameFlagSet(0x31b);
	}
	else if (pos == 121)
	{
		Engine_GameFlagSet(0x31a);
	}
	Scene_RunActorFormation(0);
}

static struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z)
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

static void SoruIriguchi_PushFacedBlock(void)
{
	struct FieldActor	  *leader;
	struct FieldActor	  *block;
	s32					   zero;
	s32					   step;
	u32					   dir;
	union FieldCoordinate  pos[3];
	union FieldCoordinate *p;

	leader = Object_GetById(0);
	dir = leader->facing >> 12;
	block = SceneActor_FindActorAtCell((leader->x.part.pixel + (gSoruPushSteps[dir] >> 16)) >> 4,
		(leader->z.part.pixel + (s16)gSoruPushSteps[dir]) >> 4);
	if (block != NULL)
	{
		zero = 0;
		block->terrain_id = 2;
		p = pos;
		step = gSoruPushSteps[dir];
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
			block->velocity_x = zero;
			block->velocity_z = zero;
			Object_SetMode(leader, 1);
			switch (gGameState.entrance)
			{
			case 11:
			case 12:
			case 13:
				Scene_UpdateOuterActor9Flags();
				Scene_UpdateOuterActor10Flags();
				break;
			case 14:
			case 15:
			case 16:
				Scene_UpdateFormationActor9Flags();
				Scene_UpdateFormationActor10Flags();
				EV11_PSYNERGY();
				EV12_PSYNERGY();
				EV13_PSYNERGY();
				EV14_PSYNERGY();
				break;
			}
		}
	}
}

s32 Scene_Initialize(void)
{
	s32 scenario = gGameState.scene;

	if (scenario == (s32)&SceneId_SoruIriguchi2)
	{
		SoruIriguchi_ApplyEntryState();
	}
	else if (scenario == (s32)&SceneId_SoruIriguchi1)
	{
		FieldScene_RunSceneEntryHook();
	}
	return 0;
}

static void SoruIriguchi_ApplyEntryState(void)
{
	GameFlag_Set(0x144);
	gEventWork->start_transition = 0x100;
	if (GameFlag_IsSet(0x814) != 0)
	{
		SoruIriguchi_CueTimer = 0;
		Task_AddCallback((void (*)(void))Scene_UpdateCueTimer, 0xc80);
	}
	if (GameFlag_IsSet(0x879) != 0)
	{
		Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
		Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
		Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
		Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
	}
	if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
	{
		Actor_SetPosition(8, 0x780000, 0xe80000);
		Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
		Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
		Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
	}
}

static void FieldScene_RunSceneEntryHook(void)
{
	s32 entrance;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (Engine_GameFlagIsSet(0x814) != 0)
	{
		BattleFx_SetQueuedSoundAndPlay(141);
		Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		InitializeSceneRecordBuffer();
	}

	entrance = gGameState.entrance;
	switch (entrance)
	{
	case 1:
	case 2:
		if (Engine_GameFlagIsSet(0x81a) != 0)
		{
			Engine_MapCopyCellsTo(1, 109, 4, 81, 1, 1);
			Engine_MapCopyCellsTo(0, 70, 30, 42, 1, 1);
			Engine_MapCopyCellsTo(0, 29, 3, 1, 3, 2);
			Engine_MapCopyCellAttributes(0, 29, 3, 2, 3, 1);
			Engine_MapRedraw();
		}
		break;

	case 3:
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		break;

	case 8:
		gGameState.retreat_scene = (s32)&SceneId_SoruIriguchi1;
		gGameState.retreat_entrance = 8;
		if (GameFlag_IsSet(0x802) == 0)
			Scene_EnterSolSanctum();
		break;

	case 11:
	case 12:
	case 13:
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
		if (Engine_GameFlagIsSet(0x804) == 0)
			Scene_SukuretaSuspectsHiddenPassage();
		if (Engine_GameFlagIsSet(0x303) != 0)
			Actor_SetPosition(9, 0x5d80000, 0x880000);
		else if (GameFlag_IsSet(0x302) != 0)
			Actor_SetPosition(9, 0x5f80000, 0x880000);
		if (Engine_GameFlagIsSet(0x301) != 0)
			Actor_SetPosition(10, 0x7180000, 0x880000);
		else if (Engine_GameFlagIsSet(0x300) != 0)
			Actor_SetPosition(10, 0x7380000, 0x880000);
		break;

	case 14:
	case 15:
	case 16:
		Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
		if (Engine_GameFlagIsSet(0x825) == 0)
			Scene_RunTransitionCue();
		Scene_RunActorFormation(1);
		Engine_GameFlagSet(0x234);
		if (GameFlag_IsSet(0x821) != 0)
		{
			Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
			Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
			Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
			Engine_MapRedraw();
		}
		break;
	}
}

static void Scene_EnterSolSanctum(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_ActorSetAnimation(0, 0);
	Engine_EventWait(4);
	Camera_MoveTo(-1, -1, -1, 0);
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(0x4c80000, -1, 0x880000, 1);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(ACTOR_SUKURETA, record->x.fixed, record->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(JASMINE, record->x.fixed, record->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
	Actor_SetSpeed(JASMINE, 0x9999, 0x4ccc);
	Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
	Engine_ActorSetAnimation(GERALD, 2);
	Engine_ActorSetAnimation(JASMINE, 2);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	Actor_SetDestinationOffset(GERALD, -16, 0);
	Actor_SetDestinationOffset(JASMINE, 16, 0);
	Actor_SetDestinationOffset(ACTOR_SUKURETA, 0, -32);
	Engine_ActorWaitForMove(GERALD);
	Engine_ActorSetAnimation(GERALD, 0);
	Engine_ActorSetAnimation(JASMINE, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 0);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 20);
	Engine_EventSetMessage((s32)MsgSoruSukuretaFirstTimeAtSol);
	Event_AskYesNo(0x4008, 0);
	Engine_EventWait(20);
	Camera_MoveTo(0x4c80000, -1, 0x940000, 1);
	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorSetAnimation(JASMINE, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(JASMINE, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(ACTOR_SUKURETA, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Actor_SetPosition(JASMINE, 0, 0);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(JASMINE, 1);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
	GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	Engine_EventEnd();
}

static void Scene_SukuretaSuspectsHiddenPassage(void)
{
	u8 *record;

	Engine_EventBegin();
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();

	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetPosition(8, RECORD_A32(record), RECORD_B32(record));
	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetPosition(5, RECORD_A32(record), RECORD_B32(record));
	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetPosition(1, RECORD_A32(record), RECORD_B32(record));

	Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
	Actor_SetSpeed(JASMINE, 0x9999, 0x4ccc);
	Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
	Engine_ActorSetAnimation(GERALD, 2);
	Engine_ActorSetAnimation(JASMINE, 2);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	Actor_SetDestinationOffset(GERALD, -16, 0);
	Actor_SetDestinationOffset(JASMINE, 16, 0);
	Actor_SetDestinationOffset(8, 0, -16);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimation(GERALD, 0);
	Engine_ActorSetAnimation(JASMINE, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 30);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 30);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 30);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	Engine_ActorSetDestinationOffset(8, 0, -16);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(6);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_EAST, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	Engine_ActorSetDestinationOffset(8, 0, -32);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);

	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Camera_SetSpeed(0x13333, 0x2666);
	Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
	Engine_CameraWaitForMove();
	Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
	Camera_MoveTo(0x06d80000, -1, 0x00960000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Camera_SetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_EventWait(10);

	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 10);
	Actor_ShowEmote(GERALD, 0x101, 20);
	Engine_EventSetMessage((s32)MsgSoruWrongSukureta);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
	Engine_ActorStartRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(5, 0x102);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	Event_ShowMessage(ACTOR_SUKURETA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Engine_ActorFaceEachOther(0, 5, 0);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorRunRepeatedMotion(5, 1);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 20);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(10);
	Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_EAST, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 60);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 20);
	Engine_ActorJump(ACTOR_SUKURETA, 2, 20);

	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
	Actor_FaceDirection(GERALD, FACING_EAST, 20);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventSetMessage((s32)MsgSoruDangerousSplitStay);
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventShowMessageAndWait(1, 0, 10);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgSoruCantSeriouslyWant);
		Actor_FaceDirection(JASMINE, FACING_WEST, 20);
		Event_ShowMessageAndWait(JASMINE, 0, 10);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
		Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 60);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
		Engine_ActorRunRepeatedMotion(1, 1);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(1, 3);
		Engine_EventWait(10);
		Actor_FaceDirection(GERALD, FACING_EAST, 0);
		Actor_FaceDirection(JASMINE, FACING_WEST, 30);
		Engine_ActorRunRepeatedMotion(1, 1);
		Engine_EventWait(10);
		Engine_EventShowMessageAndWait(1, 0, 10);
	}

	Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
	Engine_ActorSetDestinationOffset(8, 0, 48);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(6);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Engine_ActorFaceDirection(5, FACING_NORTHWEST, 0);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(JASMINE, 3);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(1, 2);

	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetDestination(1, RECORD_A16(record), RECORD_B16(record));
	Engine_ActorSetAnimation(5, 2);
	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetDestination(5, RECORD_A16(record), RECORD_B16(record));
	Engine_ActorSetAnimation(8, 2);
	record = (u8 *)Object_GetById(0);
	if (record != 0)
		Engine_ActorSetDestination(8, RECORD_A16(record), RECORD_B16(record));

	Engine_ActorWaitForMove(8);
	Actor_SetPosition(GERALD, 0, 0);
	Actor_SetPosition(JASMINE, 0, 0);
	Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(5, 1);
	GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
	Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
	Engine_EventEnd();
}

static void Scene_RunTransitionCue(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(8, record->x.fixed, record->z.fixed);
	}
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Engine_ActorSetAnimation(8, 2);
	Actor_SetDestinationOffset(8, 24, -10);
	Engine_ActorWaitForMove(8);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(6);
	Actor_FaceDirection(8, 0xb000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	Camera_SetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x6880000, -1, 0x20c0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Camera_SetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x7580000, -1, 0x20c0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Camera_SetSpeed(0x33333, 0x6666);
	Camera_MoveTo(0x6e90000, -1, 0x2240000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Actor_FaceDirection(8, FACING_EAST, 30);
	Engine_EventSetMessage((s32)MsgSoruMoreStatuesOutOfReach);
	Event_ShowMessageAndWait(0x4008, 0, 10);
	Actor_ShowEmote(8, 0x100, 40);
	Engine_ActorRunRepeatedMotion(8, 1);
	Actor_FaceDirection(8, 0x5000, 20);
	Event_ShowMessageAndWait(0x4008, 0, 10);
	Engine_ActorSetAnimation(8, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(8, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(8);
	Actor_SetPosition(8, 0, 0);
	GameFlag_Set(0x825);
	Engine_EventEnd();
}

static void Scene_RunActorFormation(s32 a0)
{
	Engine_MapCopyCellAttributes(122, 20, 1, 1, 100, 32);
	Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
	Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
	Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
	Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
	Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
	if (GameFlag_IsSet(0x311) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
		if (a0 == 0)
		{
			goto L_02001890;
		}
		Actor_SetPosition(9, 0x6380000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x310) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(9, 0x6580000, 0x2080000);
			}
		}
	}
L_02001890:;
	if (GameFlag_IsSet(0x313) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
		if (a0 == 0)
		{
			goto L_020018f2;
		}
		Actor_SetPosition(10, 0x6780000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x312) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(10, 0x6980000, 0x2080000);
			}
		}
	}
L_020018f2:;
	if (GameFlag_IsSet(0x315) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
		if (a0 == 0)
		{
			goto L_02001956;
		}
		Actor_SetPosition(11, 0x6b80000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x314) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(11, 0x6d80000, 0x2080000);
			}
		}
	}
L_02001956:;
	if (GameFlag_IsSet(0x317) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
		if (a0 == 0)
		{
			goto L_020019b8;
		}
		Actor_SetPosition(12, 0x6f80000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x316) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(12, 0x7180000, 0x2080000);
			}
		}
	}
L_020019b8:;
	if (GameFlag_IsSet(0x319) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
		if (a0 == 0)
		{
			goto L_02001a1c;
		}
		Actor_SetPosition(13, 0x7380000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x318) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(13, 0x7580000, 0x2080000);
			}
		}
	}
L_02001a1c:;
	if (GameFlag_IsSet(0x31b) != 0)
	{
		Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
		if (a0 == 0)
		{
			goto L_02001a7e;
		}
		Actor_SetPosition(14, 0x7780000, 0x2080000);
	}
	else
	{
		if (GameFlag_IsSet(0x31a) != 0)
		{
			Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
			if (a0 != 0)
			{
				Actor_SetPosition(14, 0x7980000, 0x2080000);
			}
		}
	}
L_02001a7e:;
}

static void EV10_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
	Engine_EventEnd();
}

static void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
	s32	 value;
	s32 *timer;
	s32	 v3;

	timer = &SoruIriguchi_CueTimer;
	if (*timer != 0)
	{
		v3 = (*timer - 1);
		*timer = (*timer - 1);
		if (v3 != 40)
		{
			goto L_02001b14;
		}
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	}
	else
	{
		value = Engine_RandomNext();
		if (((u32)(((value << 4) - value) << 3) >> 16) == 0)
		{
			Event_PlayCue(138);
			Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
			*timer = 80;
		}
	}
L_02001b14:;
}

struct SceneEntrance gSoruIriguchiEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSoruIriguchiEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 120, 0, 112, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSoruIriguchiEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 280, 0, 472, FACING_NORTH, 0, 5, 0, 555, 510, 0 },
	{ 2, CONDITION_ALWAYS, 72, 0, 56, FACING_SOUTH, 0, 5, 0, 555, 510, 0 },
	{ 3, CONDITION_ALWAYS, 744, 0, 152, FACING_NORTH, 0, 620, 0, 860, 195, 0 },
	{ 4, CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0 },
	{ 5, CONDITION_ALWAYS, 696, 0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0 },
	{ 6, CONDITION_ALWAYS, 984, 0, 152, FACING_NORTH, 0, 860, 0, 1100, 195, 0 },
	{ 7, CONDITION_ALWAYS, 984, 0, 56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0 },
	{ 8, CONDITION_ALWAYS, 1224, 0, 184, FACING_NORTH, 0, 1100, 20, 1340, 220, 0 },
	{ 9, CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220, 0 },
	{ 10, CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490, 0 },
	{ 11, CONDITION_ALWAYS, 1672, 0, 280, FACING_NORTH, 0, 1455, 0, 1890, 310, 0 },
	{ 12, CONDITION_ALWAYS, 1512, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0 },
	{ 13, CONDITION_ALWAYS, 1832, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0 },
	{ 14, CONDITION_ALWAYS, 1768, 0, 568, FACING_NORTH, 0, 1540, 440, 1995, 600, 0 },
	{ 15, CONDITION_ALWAYS, 1672, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600, 0 },
	{ 16, CONDITION_ALWAYS, 1928, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600, 0 },
	{ SCENE_TABLE_END },
};

u32 SoruIriguchi_Exits[] = {
	SCENE_EXITS(19),
	SCENE_EXIT_IF(1, 4, 2, 0x814),
	SCENE_EXIT_IF(1, 10, 15, 0x815),
	SCENE_EXIT_IF(1, 4, 2, 0xffffffff),
	SCENE_EXIT_IF(2, 16, 8, 0xffffffff),
	SCENE_EXIT(0, 16, 0),
	SCENE_EXIT_IF(1, 15, 2, 0xffffffff),
	SCENE_EXIT_IF(2, 16, 11, 0xffffffff),
	SCENE_EXIT_IF(3, 16, 12, 0xffffffff),
	SCENE_EXIT_IF(4, 16, 9, 0xffffffff),
	SCENE_EXIT_IF(5, 15, 1, 0xffffffff),
	SCENE_EXIT_IF(6, 16, 13, 0xffffffff),
	SCENE_EXIT_IF(7, 14, 1, 0xffffffff),
	SCENE_EXIT_IF(8, 19, 2, 0xffffffff),
	SCENE_EXIT_IF(9, 16, 4, 0xffffffff),
	SCENE_EXIT_IF(10, 16, 15, 0xffffffff),
	SCENE_EXIT_IF(11, 16, 2, 0xffffffff),
	SCENE_EXIT_IF(12, 16, 3, 0xffffffff),
	SCENE_EXIT_IF(13, 16, 6, 0xffffffff),
	SCENE_EXIT_IF(14, 14, 2, 0xffffffff),
	SCENE_EXIT_IF(15, 16, 10, 0xffffffff),
	SCENE_EXIT_IF(16, 11, 1, 0xffffffff),
	SCENE_EXITS_END,
};

struct ScenePlacement gSoruIriguchiPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSoruIriguchiPlacements1[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, 0x0, ACTOR_STAND,
		PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSoruIriguchiPlacements1Entrances11To13[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSoruIriguchiPlacements1Entrances14To16[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSoruIriguchiPlacements2[] = {
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gSoruIriguchiEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSoruIriguchiEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SEARCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaItIsVeryDangerousToGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaIAmAllAloneInThis) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSoruIriguchiEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ 0x21, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf01, FIND_ITEM(0xe1) },
	{ 0xe104, 0x414, CONDITION_ALWAYS, EVENT_SCRIPT(EV1044_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSoruIriguchiEvents1Entrances11To13[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ 0x21, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x234, EVENT_SCRIPT(Scene_UpdateOuterActor9Flags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x234, EVENT_SCRIPT(Scene_UpdateOuterActor10Flags) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SoruIriguchi_PushFacedBlock) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(SoruIriguchi_PushFacedBlock) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(SoruIriguchi_PushFacedBlock) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSoruIriguchiEvents1Entrances14To16[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ 0x21, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_UpdateFormationActor9Flags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_UpdateFormationActor10Flags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_PSYNERGY) },
	{ 0x202, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SoruIriguchi_PushFacedBlock) },
	{ 0xe104, 0x415, CONDITION_ALWAYS, EVENT_SCRIPT(EV1045_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_SEARCH) },
	{ SCENE_EVENTS_END },
};

// 向きごとの押す一歩: 上位16ビットがx、下位16ビットがz (ピクセル)
#define PUSH_STEP(x, z) (((x) << 16) | ((z) & 0xffff))

s32 gSoruPushSteps[] = {
	PUSH_STEP(16, 0),
	PUSH_STEP(16, 0),
	PUSH_STEP(16, 0),
	PUSH_STEP(0, 16),
	PUSH_STEP(0, 16),
	PUSH_STEP(0, 16),
	PUSH_STEP(0, 16),
	PUSH_STEP(-16, 0),
	PUSH_STEP(-16, 0),
	PUSH_STEP(-16, 0),
	PUSH_STEP(-16, 0),
	PUSH_STEP(0, -16),
	PUSH_STEP(0, -16),
	PUSH_STEP(0, -16),
	PUSH_STEP(0, -16),
	PUSH_STEP(16, 0),
};
