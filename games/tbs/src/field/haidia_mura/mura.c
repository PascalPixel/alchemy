#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/field/haidia_mura/mura.h"
#include "unit/field/world_map/linked_effects.h"
#include "unit/battle/effect/object.h"
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
#include "unit/graphics/window/window.h"
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
#include "mem.h"
#include "global_cells.h"
#include "edition.h"
#include "staged_motion.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fieldobj.h"
#include "field_effect.h"

// 前方宣言
static void FieldScene_RunScene373SequenceC(void);
static void HaidiaMura_AskAboutSneakingToAleph(s32 a0, s32 a1);
static void HaidiaMura_MoveActorAlongLowerZPath(s32 a0, s32 a1, s32 a2, s32 a3);
static void HaidiaMura_MoveActorAlongHigherZPath(s32 a0, s32 a1, s32 a2, s32 a3);
static void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);
static void FieldScene_RunLargeStagingSequence(void);
static void Scene_RepairTheHouse(void);
static void FieldScene_RunStep8C(void);
static void FieldScene_RunSingleStep(void);
static void SceneState_SetValue1ThenCall(void);
static void SceneState_SetValue0ThenCall(void);
static void FieldScene_RunStep9(void);
static void FieldScene_RunStep17(void);
static void SceneState_SetValue24ThenCall(void);
static void SceneState_SetValue25ThenCall(void);
static s32 SceneActor_RunStep18WhenTargetSet(s32 *p);
static void Effect_ConfigureSpawnedParticle(struct SourceEntity *source);
static void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter);
static void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);
static void HaidiaMura_OpenVillagerLane(void);
static void Effect_PlayStepSound(void);
static void FieldScene_RunScene373SequenceB(void);
static void SceneActor_SetFlagByteBySlotZeroPosition(void);
static void InitializeStagedActorSceneOrbitingEffect(s32 id);
static void OverlayObject_UpdateOnFrameBit1(s32 p);
static void SceneEffect_UpdateObjectOnOddFrames(s32 p);
static void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p);

typedef struct OrbitingSceneObjectSprite
{
	u8 padding_00[5];
	u8 flags_05_low : 5;
	u8 flags_05_bit_5 : 1;
	u8 flags_05_high : 2;
	u8 padding_06[3];
	u8 flags_09_low : 2;
	u8 flags_09_mode : 2;
	u8 flags_09_high : 4;
	u8 padding_0a[18];
	u8 palette;
	u8 padding_1d[10];
	u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
	u8						   padding_00[8];
	s32						   x;
	s32						   y;
	s32						   z;
	u8						   padding_14[15];
	u8						   flags_23;
	u8						   padding_24[12];
	s32						   orbit_angle;
	u8						   padding_34[4];
	s32						   orbit_center_x;
	s32						   orbit_center_y;
	s32						   orbit_center_z;
	u8						   padding_44[12];
	OrbitingSceneObjectSprite *sprite;
	u8						   padding_54;
	u8						   mode;
	u8						   state;
	u8						   padding_57[5];
	u8						   active;
	u8						   padding_5d[4];
	u8						   visible;
	u8						   padding_62[10];
	u32						   callback;
} OrbitingSceneObject;

enum HouseActor
{
	ACTOR_BOARD = 23,
	ACTOR_LAST_BOARD = 24
};

enum
{
	ANIM_HAMMER = 11
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"

s32 MapStagedScene_SelectPrimaryData(void)
{
	return (s32)gHaidiaMuraEntrances;
}

s32 MapStagedScene_GetEmptyData(void)
{
	return 0;
}

s32 MapStagedScene_SelectSecondaryData(void)
{
	return (s32)gHaidiaMuraExits;
}

s32 MapStagedScene_SelectTertiaryData(void)
{
	u8 *scene_state = (u8 *)&gGameState;
	if (((struct GameState *)scene_state)->entrance == 16)
		return (s32)gHaidiaMuraPlacements4;
	if (Engine_GameFlagIsSet(0x87a) != 0)
		return (s32)gHaidiaMuraPlacements3;
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
		return (s32)gHaidiaMuraPlacements2;
	return (s32)gHaidiaMuraPlacements;
}

static void EV26_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(26, 0, 0);
	Engine_GameFlagSet(0xfd0);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

static void EV20_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(20, 0, 0);
	Engine_GameFlagSet(0xfd0);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

s32 MapStagedScene_SelectQuaternaryData(void)
{
	if (Engine_GameFlagIsSet(0x87a) != 0)
		return (s32)gHaidiaMuraEvents3;
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
		return (s32)gHaidiaMuraEvents2;
	return (s32)gHaidiaMuraEvents;
}

static void EV10_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaTheGroundStillShakes);
		Engine_EventShowMessage(10, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaYourGrandpaIsTheMayor);
		Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 4);
		Engine_EventAskYesNo(10, 0);
	}
	Engine_EventEnd();
}

static void EV14_TALK(void)
{
	s32 flag = 0x806;
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaSukuretaHasntComeBack);
		Engine_EventShowMessage(14, 0);
	}
	else if (Engine_GameFlagIsSet(flag) == 0)
	{
		Engine_GameFlagSet(flag);
		Engine_EventSetMessage((s32)MsgHaidiaDoYouNeedToGo);
		Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
		Engine_EventAskYesNo(14, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaDontGoBeyondSukuretasCottage);
		Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
		Engine_EventShowMessage(14, 0);
	}
	Engine_EventEnd();
}

static void EV18_TALK(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x807) == 0)
	{
		GameFlag_Set(0x807);
		Engine_EventSetMessage((s32)MsgHaidiaYouMakeMeSoMad);
		Actor_ShowEmote(18, 0x103, 0);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 18, 20);
		Event_ShowMessageAndWait(18, 0, 6);
		Actor_FaceDirection(18, FACING_WEST, 30);
		Engine_ActorJump(18, 2, 20);
		Event_ShowMessageAndWait(18, 0, 6);
		Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 10);
		Actor_ShowEmote(18, 0x103, 0);
		Event_ShowMessageAndWait(18, 0, 10);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	}
	else
	{
		Actor_ShowEmote(18, 0x103, 0);
		Engine_EventSetMessage((s32)MsgHaidiaIllGetYouForMy);
		Event_ShowMessageAndWait(18, 0, 20);
	}
	Engine_EventEnd();
}

static void EV21_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x202) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaIToldGeraldItWas);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgHaidiaItWontRainForSome);
	}
	Engine_EventShowMessage(21, 0);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor10LineAndFlag81f(void)
{
	Engine_EventBegin();
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 10, 20);
	Engine_EventSetMessage((s32)MsgHaidiaIsJasmineBackYet);
	Engine_EventShowMessage(10, 0);
	Engine_GameFlagSet(0x81f);
	Engine_EventEnd();
}

static void EV17_TALK(void)
{
	Engine_EventBegin();
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Engine_TaskWait(10);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventSetMessage((s32)MsgHaidiaIFeltAnotherOne);
	Event_ShowMessageAndWait(17, 0, 20);
	Actor_FaceActor(17, ACTOR_PARTY_LEADER, 20);
	Event_ShowMessage(17, 0);
	Engine_EventEnd();
}

static void SceneDialogue_RunActorNineteenDialogue(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(19, 2);
	Engine_EventWait(20);
	Engine_ActorFaceActor(19, ACTOR_PARTY_LEADER, 20);
	Engine_EventSetMessage((s32)MsgHaidiaNoTravelersSinceTheEruption);
	Engine_EventAskYesNo(19, 0);
	Engine_GameFlagSet(0x307);
	Engine_EventEnd();
}

static void EV15_TOUCH(void)
{
	s32 m, n;
	Engine_GameFlagSet(0x210);
	m = 10;
	n = 84;
	Engine_MapCopyCellAttributes(40, 84, 7, 4, m, n);
}

static void EV14_TOUCH(void)
{
	s32 m, n;
	Engine_GameFlagClear(0x210);
	m = 10;
	n = 84;
	Engine_MapCopyCellAttributes(40, 89, 7, 4, m, n);
}

static void EV01_TOUCH(void)
{
	Event_PlayCue(188);
	Map_AnimateCells((s32)gHaidiaMuraCellAnimC, 45, 11);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x101, 0x1a4);
	Engine_EventRequestExit(11);
}

static void EV02_TOUCH(void)
{
	if (Engine_GameFlagIsSet(0x801) == 0)
	{
		FieldScene_RunScene373SequenceC();
	}
	else
	{
		Engine_AudioPlayCue(123);
		Engine_EventRequestExit(1);
	}
}

static void EV03_TOUCH(void)
{
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(3);
}

static void EV04_TOUCH(void)
{
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(4);
}

static void EV05_TOUCH(void)
{
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(2);
}

static void EV06_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells((u32)gHaidiaMuraCellAnimA, 54, 32);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x196, 0x2d7);
	Engine_EventRequestExit(5);
}

static void EV07_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells((u32)gHaidiaMuraCellAnimB, 45, 39);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x106, 0x325);
	Engine_EventRequestExit(6);
}

static void EV08_TOUCH(void)
{
	if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0 && Engine_GameFlagIsSet(0x87a) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgHaidiaYouCantBeRobin);
		Engine_EventOpenMessage(21, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventShowMessageAndWait(21, 0, 60);
			Engine_EventShowMessage(21, 0);
		}
		else
		{
			u16 *h = &gEventWork->message;

			*h = *h + 2;
			Engine_EventWait(40);
			Engine_EventShowMessage(21, 0);
		}
		Engine_EventEnd();
	}
	else
	{
		Engine_AudioPlayCue(0x9e);
		Engine_MapAnimateCells((s32)gHaidiaMuraCellAnimA, 50, 44);
		Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x154, 0x378);
		Engine_EventRequestExit(7);
	}
}

static void EV09_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells((s32)gHaidiaMuraCellAnimB, 49, 69);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 0x466);
	Engine_EventRequestExit(8);
}

static void EV10_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells((s32)gHaidiaMuraCellAnimD, 52, 76);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x176, 0x4d6);
	Engine_EventRequestExit(9);
}

static void EV11_TOUCH(void)
{
	Event_PlayCue(158);
	Map_AnimateCells((s32)gHaidiaMuraCellAnimA, 35, 74);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x4b6);
	Engine_EventRequestExit(10);
}

static void FieldScene_RunScene373SequenceC(void)
{
	struct FieldActor *leader;
	struct FieldActor *jasmine;
	struct FieldActor *actor;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	jasmine = Object_GetById(JASMINE);
	Engine_EventBegin();
	jasmine->x.fixed = leader->x.fixed;
	jasmine->y.fixed = leader->y.fixed;
	jasmine->z.fixed = leader->z.fixed;
	jasmine->target_x = ACTOR_NO_TARGET;
	jasmine->target_y = ACTOR_NO_TARGET;
	jasmine->target_z = ACTOR_NO_TARGET;
	jasmine->velocity_x = 0;
	jasmine->velocity_y = 0;
	jasmine->velocity_z = 0;
	jasmine->terrain_height = leader->y.fixed;
	Engine_TaskWait(1);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_WalkToAndWait(JASMINE, 110, 0x11b);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 2);
	Engine_EventSetMessage((s32)MsgHaidiaSukuretaIsWaitingForUs);
	if (leader->x.fixed < jasmine->x.fixed)
	{
		Event_ShowMessageAndWait(0xa005, 0, 2);
	}
	else
	{
		Event_ShowMessageAndWait(0x8005, 0, 2);
	}
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(2);
	Engine_ActorSetAnimation(JASMINE, 2);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetDestination(JASMINE, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorWaitForMove(JASMINE);
	Actor_SetPosition(JASMINE, 0, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 0x12f);
	Engine_EventEnd();
}

static void EV20_TOUCH(void)
{
	s32 **cam;
	s32	 *saved;
	s32	 *rec;
	s32	  n;
	s32	  pos[3];

	if (Engine_GameFlagIsSet(0x808) == 0)
	{
		cam = gMapWork[0];
		Engine_EventBegin();
		Actor_SetSpeed(0, 0x10000, 0x8000);
		Engine_ActorSetAnimation(0, 1);
		Engine_EventWait(2);
		Engine_EventSetMessage((s32)MsgHaidiaRrruffRrrruff);
		Engine_EventShowMessageAndWait(15, 0, 2);
		Engine_EventShowMessageAndWait(16, 0, 2);
		rec = (s32 *)Object_GetById(0);
		pos[0] = rec[2];
		pos[1] = rec[3];
		pos[2] = rec[4];
		saved = *cam;
		*cam = pos;
		n = 0;
		do
		{
			pos[2] += 0x20000;
			Engine_EventWait(1);
			n++;
			Engine_MapRedraw();
		} while (n != 40);
		Engine_EventWait(60);
		Engine_MessageShowCentered((s32)MsgHaidiaPuppiesPlayingOver, 1);
		n = 0;
		Engine_EventWait(6);
		do
		{
			pos[2] -= 0x20000;
			Engine_EventWait(1);
			n++;
			Engine_MapRedraw();
		} while (n != 40);
		*cam = saved;
		Engine_EventWait(60);
		Actor_WalkToAndWait(0, 70, 0x2e5);
		Engine_EventEnd();
	}
}

static void EV21_TOUCH(void)
{
	s32 msg;

	if (GameFlag_IsSet(0x808) == 0)
	{
		Engine_EventBegin();
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
		msg = (s32)MsgHaidiaRrruffRrrruff;
		Engine_EventSetMessage(msg);
		Event_ShowMessageAndWait(15, 0, 2);
		Event_ShowMessageAndWait(16, 0, 2);
		Engine_MessageShowCentered(msg + 2, 1);
		Engine_EventWait(6);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
		Engine_EventEnd();
	}
}

static void EV100_TOUCH(void)
{
	s32 m, n;
	Engine_EventBegin();
	m = 20;
	n = 50;
	Engine_MapCopyCellAttributes(49, 53, 8, 4, m, n);
	HaidiaMura_MoveActorAlongLowerZPath(0, 10, 11, 1);
	Engine_GameFlagSet(0x204);
	Engine_EventEnd();
}

static void EV101_TOUCH(void)
{
	s32 p5, p6;
	Engine_EventBegin();
	HaidiaMura_MoveActorAlongHigherZPath(0, 13, 10, 1);
	Engine_GameFlagClear(0x204);
	p5 = 20;
	p6 = 50;
	Engine_MapCopyCellAttributes(49, 46, 8, 4, p5, p6);
	Engine_EventEnd();
}

static void EV50_TOUCH(void)
{
	struct FieldActor *rec7;
	struct FieldActor *record;

	rec7 = Object_GetById(22);
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 5, 0);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 0x193);
	rec7->steering_flags |= ACTOR_FACES_MOTION;
	Actor_SetPosition(22, 0xa60000, 0x1770000);
	Actor_FaceDirection(22, FACING_SOUTHEAST, 20);
	rec7->steering_flags = (rec7->steering_flags ^ 1);
	Actor_SetSpeed(22, 0x28000, 0x28000);
	Engine_ActorJump(22, 4, 0);
	Actor_WalkToAndWait(22, 202, 0x18b);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(22, 0x3000, 24);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Actor_SetSpeed(22, 0x18000, 0x10000);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaMuraLeaderWalkActions);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x103, 0);
	Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22WalkActions);
	Object_RefreshSelectorById(0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x1da);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Object_RefreshSelectorById(22);
	Actor_WalkToAndWait(22, 0x100, 0x1c8);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_FaceDirection(22, FACING_SOUTH, 20);
	Engine_ActorStartRepeatedMotion(22, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
	Event_AskYesNo(22, 0);
	record = Actor_Get(22);
	record->update = (void (*)(union FieldObject *))SceneActor_RunStep18WhenTargetSet;
	Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
	GameFlag_Set(0x823);
	Engine_EventEnd();
}

static void EV51_TOUCH(void)
{
	u8 *p = Object_GetById(22);
	if (Engine_GameFlagIsSet(0x823) != 0)
	{
		u8 *q = p;
		q += 100;
		if (*(s16 *)q == 1)
		{
			HaidiaMura_AskAboutSneakingToAleph(
				(s32)gHaidiaMuraPairTableA, (s32)gHaidiaMuraPairTableB);
		}
	}
}

static void HaidiaMura_AskAboutSneakingToAleph(s32 a0, s32 a1)
{
	s32				   p8;
	struct FieldActor *rec8;
	struct FieldActor *record;

	p8 = a1;
	rec8 = Object_GetById(22);
	Engine_EventBegin();
	Engine_ActorStartRepeatedMotion(22, 2);
	Actor_ShowEmote(22, 0x100, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, a0);
	Engine_EventWait(10);
	Actor_ShowEmote(22, 0x103, 0);
	Object_SetActionCallbackAndRefreshById(22, p8);
	Object_RefreshSelectorById(0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(22, 2);
	rec8->scale_x = 0x10000;
	rec8->scale_y = 0x10000;
	record = Object_GetById(ACTOR_PARTY_LEADER);
	record->scale_x = 0x10000;
	record->scale_y = 0x10000;
	Engine_EventSetMessage((s32)MsgHaidiaNotSneakingUpMtAleph);
	Event_AskYesNo(22, 0);
	record = Actor_Get(22);
	record->update = (void (*)(union FieldObject *))SceneActor_RunStep18WhenTargetSet;
	Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
	Engine_EventEnd();
}

static void EV52_TOUCH(void)
{
	u8 *p = Object_GetById(22);
	if (Engine_GameFlagIsSet(0x823) != 0)
	{
		u8 *q = p;
		q += 100;
		if (*(s16 *)q == 2)
		{
			HaidiaMura_AskAboutSneakingToAleph(
				(s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
		}
	}
}

static void EV53_TOUCH(void)
{
	u8 *rec = Object_GetById(22);
	if (Engine_GameFlagIsSet(0x823) != 0)
	{
		u8 *q = rec;
		s32 v;
		q += 100;
		v = *(s16 *)q;
		if (v == 1)
		{
			HaidiaMura_AskAboutSneakingToAleph(
				(s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableB);
		}
		else if (v == 2)
		{
			HaidiaMura_AskAboutSneakingToAleph(
				(s32)gHaidiaMuraPairTableC, (s32)gHaidiaMuraPairTableD);
		}
	}
}

static void HouseScene_RunRepairMorning(void)
{
	struct FieldActor *leader;
	struct FieldActor *actor;
	u8				   leader_motion_flags;
	u8				  *leader_motion;
	s32				   i;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	actor = Engine_EventGetViewCenter();
	actor->motion_flags = 0;
	Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
	Engine_EventWait(1);
	Engine_MapRedraw();
	Map_CopyCellAttributes(49, 41, 7, 3, 20, 50);
	Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
	Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
	Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	leader_motion = &leader->motion_flags;
	leader_motion_flags = *leader_motion;
	*leader_motion = 0;
	Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(407), PIXELS(690));
	Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
	Actor_SetPosition(GERALD, PIXELS(298), PIXELS(736));
	Actor_SetPosition(JASMINE, PIXELS(298), PIXELS(760));
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
	SceneActor_ResetActorRun(ACTOR_BOARD, 2, 1);
	gEventWork->start_transition = 0;
	gEventWork->transition_frames = 32;
	Engine_EventOpenScreen();
	Actor_SetSpeed(JASMINE, 0x8000, 0x4000);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Engine_ActorEnableActionCallback(JASMINE, gJasmineAction);
	Engine_ActorEnableActionCallback(GERALD, gGeraldAction);

	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
	leader->scale_x = 0x10000;
	leader->scale_y = 0x10000;
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
	Actor_SetChildValue(ACTOR_BOARD, 2);
	Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
	Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
	Engine_EventWait(80);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(ACTOR_BOARD, 402, 828);
	Engine_EventWait(80);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(ACTOR_BOARD, 0);
	Actor_SetPosition(ACTOR_BOARD, PIXELS(390), PIXELS(842));
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
	Engine_EventWait(200);
	Map_CopyCellsTo(7, 102, 84, 41, 2, 1);

	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
	leader->scale_x = 0x10000;
	leader->scale_y = 0x10000;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(20);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
	Actor_SetChildValue(ACTOR_BOARD, 2);
	Actor_SetSpeed(ACTOR_BOARD, 0x4ccc, 0x2666);
	Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
	Engine_EventWait(80);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(ACTOR_BOARD, 377, 828);
	Engine_EventWait(80);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(ACTOR_BOARD, 0);
	Actor_SetPosition(ACTOR_BOARD, 0, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
	Engine_EventWait(200);
	Map_CopyCellsTo(6, 102, 83, 41, 1, 1);

	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
	leader->scale_x = 0x10000;
	leader->scale_y = 0x10000;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(20);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
	Actor_SetChildValue(ACTOR_LAST_BOARD, 2);
	Actor_SetSpeed(ACTOR_LAST_BOARD, 0x4ccc, 0x2666);
	Actor_MoveToAndWait(ACTOR_LAST_BOARD, 390, 832);
	Engine_EventWait(80);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(ACTOR_LAST_BOARD, 360, 842);
	Engine_EventWait(80);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(ACTOR_LAST_BOARD, 0);
	Actor_SetPosition(ACTOR_LAST_BOARD, 0, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gLeaderHammerAction);
	Engine_EventWait(200);
	Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, ACTION_TABLE_STOP);
	leader->scale_x = 0x10000;
	leader->scale_y = 0x10000;

	Engine_EventSetMessage((s32)MsgHaidiaGoodJob);
	Engine_ActorJump(ACTOR_DORA, 2, 20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
	HaidiaMura_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
	Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
	Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Event_OpenMessage(ACTOR_DORA, 0);
	if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
	{
		gEventWork->message++;
	}
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_EventSetMessage((s32)MsgHaidiaWorkingYourselvesBone);
	Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
	Event_OpenMessage(ACTOR_DORA, 0);
	if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1)
	{
		gEventWork->message++;
	}
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgHaidiaDevastatedWhenKyle);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);

	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_WalkTo(GERALD, 369, 904);
	Actor_WalkToAndWait(JASMINE, 392, 904);
	Engine_ActorSetAnimation(GERALD, ANIM_STAND);
	HaidiaMura_MoveActorAlongLowerZPath(JASMINE, 10, 11, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
	Engine_ActorJump(JASMINE, 4, 0);
	Actor_WalkToAndWait(JASMINE, 392, 843);
	Actor_FaceDirection(JASMINE, FACING_WEST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	HaidiaMura_MoveActorAlongLowerZPath(GERALD, 10, 11, 0);
	Actor_SetSpeed(JASMINE, 0x4ccc, 0x2666);
	Actor_SetSpeed(GERALD, 0x4ccc, 0x2666);
	Actor_WalkTo(GERALD, 392, 843);
	Actor_Get(JASMINE)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(JASMINE, 408, 843);
	Engine_EventWait(1);
	Actor_Get(JASMINE)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Engine_ActorWaitForMove(GERALD);
	Engine_ActorSetAnimation(GERALD, ANIM_STAND);
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorJump(ACTOR_DORA, 4, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, ANIM_NOD);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, ANIM_NOD);
	Engine_EventWait(100);
	Engine_ActorFaceEachOther(JASMINE, GERALD, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
	Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
	Engine_ActorFaceEachOther(JASMINE, GERALD, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(30);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(JASMINE, FACING_WEST, 30);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(10);
	Event_OpenMessage(ACTOR_DORA, 0);
	if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1)
	{
		gEventWork->message++;
	}
	Engine_EventWait(40);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);

	Engine_EventSetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 3, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorJump(ACTOR_DORA, 4, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Engine_ActorSetAnimation(ACTOR_DORA, 7);
	Engine_EventWait(5);
	Event_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, GERALD, 10, 14, 4, 14, 0);
	actor = Actor_Get(ACTOR_DORA);
	actor->sprite->flags = 0;
	actor->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
	Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
	Engine_EventWait(4);
	for (i = 0; i != 4; i++)
	{
		actor->z.fixed += 0x18000;
		actor->scale_y -= 0x1999;
		Engine_EventWait(1);
	}
	Actor_SetPosition(ACTOR_DORA, 0, 0);
	Actor_SetSpeed(GERALD, 0x30000, 0x18000);
	Engine_ActorJump(GERALD, 6, 0);
	Actor_WalkToAndWait(GERALD, 374, 827);
	Event_ShowMessage(JASMINE, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST + FACING_STEP, 0);
	Actor_ShowEmote(JASMINE, EMOTE_IN_FRONT | 0, 0);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 0, 10);
	Engine_ActorSetAnimation(GERALD, 13);
	Engine_ActorJump(GERALD, 2, 5);
	MapRender_SetValues(0, 0x40000, 0x10000);
	Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
	Engine_ActorStartRepeatedMotion(GERALD, 3);
	MapRender_SetValues(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 80);
	Engine_ActorSetAnimation(ACTOR_DORA, 8);
	actor->scale_y = 0x8000;
	Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
	for (i = 0; i != 5; i++)
	{
		actor->scale_y += 0x1999;
		Engine_EventWait(1);
	}
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Camera_SetSpeed(0x4ccc, 0x999);
	Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
	Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
	Engine_ActorJump(ACTOR_DORA, 6, 0);
	Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(30);
	actor = Actor_Get(ACTOR_DORA);
	actor->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_SetAttachedEffect(ACTOR_DORA, EMOTE_IN_FRONT | 2);
	Engine_EventWait(80);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 30);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 80);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 1);
	Engine_ActorJump(GERALD, 6, 0);
	Engine_ActorSetAnimation(GERALD, ANIM_STAND);
	Actor_SetSpeed(GERALD, 0x40000, 0x20000);
	actor = Actor_Get(GERALD);
	actor->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetDestination(GERALD, 403, 827);
	Actor_SetAttachedEffect(JASMINE, EMOTE_IN_FRONT | 2);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 20);
	Event_ShowMessageAndWait(JASMINE, 0, 1);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 0, 0);
	Engine_ActorSetAnimation(GERALD, 13);
	Engine_ActorJump(GERALD, 2, 5);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
	MapRender_SetValues(0, 0x40000, 0x10000);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	MapRender_SetValues(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 30);
	Actor_SetSpeed(JASMINE, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(JASMINE, 408, 855);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
	Engine_ActorStartRepeatedMotion(JASMINE, 3);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(80);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_SHAKE_HEAD);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 80);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
	Actor_ShowEmote(JASMINE, EMOTE_IN_FRONT | 1, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 1, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 70);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_WEST, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, ANIM_NOD);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
	Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Engine_ActorJump(GERALD, 4, 0);
	Actor_WalkToAndWait(GERALD, 398, 828);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	Engine_EventWait(60);

	actor = Actor_Get(GERALD);
	actor->steering_flags |= ACTOR_FACES_MOTION;
	actor = Actor_Get(JASMINE);
	actor->steering_flags |= ACTOR_FACES_MOTION;
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_WalkTo(JASMINE, actor->x.part.pixel + 16, actor->z.part.pixel);
	Actor_WalkToAndWait(GERALD, actor->x.part.pixel + 16, actor->z.part.pixel - 16);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 30);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(JASMINE, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(40);
	Actor_WalkToAndWait(JASMINE, actor->x.part.pixel, actor->z.part.pixel);
	Actor_SetPosition(JASMINE, 0, 0);
	Actor_WalkToAndWait(GERALD, actor->x.part.pixel, actor->z.part.pixel);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_PartyAddMembers(GERALD, JASMINE);
	Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
	HaidiaMura_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER, 13, 10, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_Get(ACTOR_DORA)->steering_flags |= ACTOR_FACES_MOTION;
	HaidiaMura_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
	Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(20);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(100);
	GameFlag_Set(0x202);
	GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	*leader_motion = leader_motion_flags;
	Engine_EventEnd();
}

s32 HaidiaMura_ApplyEntryState(void)
{
	struct FieldActor *actor;
	s32				   set;

	if (gGameState.entrance == 16)
	{
		PaletteGlow_Update(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
		FieldScene_RunLargeStagingSequence();
	}
	else
	{
		if (!Engine_GameFlagIsSet(0xfd0))
		{
			if (!Engine_GameFlagIsSet(0x87a))
			{
				InitializeStagedActorSceneOrbitingEffect(26);
			}
			else
			{
				InitializeStagedActorSceneOrbitingEffect(20);
			}
		}
		Engine_MapCopyCellsTo(2, 102, 84, 41, 2, 1);
		Engine_MapCopyCellsTo(1, 102, 83, 41, 1, 1);
		actor = Object_GetById((Engine_GameFlagIsSet(0x87a) != 0) + 20);
		Engine_ActorSetSpriteFlags(actor, 0);
		if (Engine_GameFlagIsSet(0x314))
		{
			actor->x.fixed = 181 << 17;
		}
		else if (Engine_GameFlagIsSet(0x316))
		{
			actor->x.fixed = 197 << 17;
		}
		else
		{
			actor->x.fixed = 189 << 17;
		}
		actor->z.fixed = 0x2480000;
		actor->y.fixed = 0xc00000;
		HaidiaMura_OpenVillagerLane();
		actor->terrain_id = 3;
		actor->motion_flags = 0;
		Engine_TaskAddCallback(SceneActor_SetFlagByteBySlotZeroPosition, 0xc80);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
		if (Engine_GameFlagIsSet(0x87a))
		{
			if (Engine_GameFlagIsSet(0x109))
			{
				if (Engine_GameFlagIsSet(0x204))
				{
					Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
				}
				if (Engine_GameFlagIsSet(0x210))
				{
					EV15_TOUCH();
				}
			}
		}
		else
		{
#else
		if (!Engine_GameFlagIsSet(0x87a))
		{
#endif
			if (GameFlag_IsSet(0x815))
			{
				actor = Object_GetById(21);
				Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
				actor->scale_x = 0x28f;
				actor->scale_y = 0x28f;
			}
			if (Engine_GameFlagIsSet(0x808))
			{
				Engine_ActorSetPosition(15, 0, 0);
				Engine_ActorSetPosition(16, 0, 0);
				Engine_ActorSetPosition(17, 0, 0);
			}
			set = Engine_GameFlagIsSet(0x815);
			if (set == 0)
			{
				if (!Engine_GameFlagIsSet(0x109))
				{
					if (Engine_GameFlagIsSet(0x823))
					{
						Actor_SetPosition(22, 0x1000000, 0x1c80000);
						Object_GetById(22)->update = (void *)SceneActor_RunStep18WhenTargetSet;
						Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
					}
				}
				else
				{
					Object_GetById(22)->paused = set;
					Engine_GameFlagClear(0x241);
				}
				if (gGameState.entrance != 16 && !Engine_GameFlagIsSet(0x87a))
				{
					Engine_TaskAddCallback(FieldScene_RunScene373SequenceB, 0xc80);
				}
			}
			if (!GameFlag_IsSet(0x308) && gGameState.entrance == 17)
			{
				Scene_RepairTheHouse();
				Engine_GameFlagSet(0x308);
			}
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			if (Engine_GameFlagIsSet(0x109))
			{
				if (Engine_GameFlagIsSet(0x204))
				{
					Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
				}
				if (Engine_GameFlagIsSet(0x210))
				{
					EV15_TOUCH();
				}
			}
			{
				s32 priority;
				s32 actor;

				/* FAKEMATCH: the cancelled byte keeps priority's constant before the actor's; a plain 2, even in a one-pass block, copies actor first (2 aligned differences). */
				do
				{
					priority = ((u32)(u8)set >> 8) + 2;
				} while (0);
				actor = ACTOR_DORA;
				Engine_ActorSetSpritePriority(actor, priority);
			}
#endif
		}
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_IT)
		if (Engine_GameFlagIsSet(0x109))
		{
			if (Engine_GameFlagIsSet(0x204))
			{
				Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
			}
			if (Engine_GameFlagIsSet(0x210))
			{
				EV15_TOUCH();
			}
		}
#endif
		BattleFx_SetQueuedSoundAndPlay(170);
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
	return 0;
}

static void FieldScene_RunSecondaryActorSequence(void)
{
	struct FieldActor *record;
	s32				   msg;

	Engine_EventBegin();
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
	msg = (s32)MsgHaidiaAh;
	Engine_EventSetMessage(msg);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 20);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x18e0000, -1, 0x2460000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a4, 0x260);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(8, 0x3000, 0);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	Actor_WalkToAndWait(GERALD, 0x192, 0x260);
	Actor_FaceDirection(GERALD, 0xd000, 20);
	Event_ShowMessage(0x1001, 0);
	Actor_FaceDirection(8, 0x5000, 20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Event_ShowMessage(0x4008, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_OpenMessage(0x4008, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
		Engine_ActorStartRepeatedMotion(8, 1);
	}
	Event_ShowMessageAndWait(0x4008, 0, 40);
	Actor_ShowEmote(8, 0x105, 60);
	Engine_EventSetMessage(msg + 6);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(0x1001, 0, 40);
	Engine_ActorRunRepeatedMotion(8, 1);
	Actor_FaceDirection(8, 0xd000, 20);
	Event_ShowMessage(0x4008, 0);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Event_ShowMessageAndWait(0x1001, 0, 120);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Actor_ShowEmote(GERALD, 0x105, 40);
	Event_ShowMessageAndWait(0x1001, 0, 40);
	Engine_ActorSetAnimationAndWait(8, 4);
	Event_ShowMessageAndWait(0x4008, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(40);
	Actor_FaceDirection(8, 0x5000, 20);
	Event_ShowMessageAndWait(0x4008, 0, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, 3);
	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	GameFlag_Set(0x303);
	Engine_EventEnd();
}

static void FieldScene_RunPrimaryActorSequence(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Camera_MoveTo(0x1650000, -1, 0x2e20000, 1);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16f, 0x2e9);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	Actor_WalkToAndWait(GERALD, 0x15a, 0x2e9);
	Actor_FaceDirection(GERALD, 0xd000, 20);
	Engine_EventSetMessage((s32)MsgHaidiaHowHaveYouBeen);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorRunRepeatedMotion(9, 2);
	Actor_ShowEmote(9, 0x100, 0);
	Actor_FaceDirection(9, 0x3000, 10);
	Actor_FaceDirection(9, 0x5000, 10);
	Actor_FaceDirection(9, 0x3000, 40);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorRunRepeatedMotion(9, 1);
	Actor_FaceDirection(9, 0x5000, 10);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Actor_ShowEmote(GERALD, 0x103, 40);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorSetAnimationAndWait(9, 3);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, 0x1000, 40);
	Engine_ActorSetAnimationAndWait(9, 4);
	Event_ShowMessage(9, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(GERALD, 0xd000, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorSetAnimationAndWait(9, 3);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 80);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(9, 0, 20);
	Actor_FaceDirection(GERALD, 0x1000, 20);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Actor_ShowEmote(GERALD, 0x105, 60);
	}
	else
	{
		gEventWork->message++;
	}
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(GERALD, 0xd000, 10);
	Engine_EventSetMessage((s32)MsgHaidiaEverProtectFamily);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorSetAnimationAndWait(9, 3);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x1000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	GameFlag_Set(0x304);
	Engine_EventEnd();
}

static void FieldScene_RunCompanionActorSequence(void)
{
	struct FieldActor *actor_data;
	s32				   shared_data;

	Engine_EventBegin();
	actor_data = Actor_Get(ACTOR_A);
	Engine_ActorSetSpriteFlags(actor_data, 0);
	actor_data = Actor_Get(ACTOR_B);
	Engine_ActorSetSpriteFlags(actor_data, 0);
	actor_data = Actor_Get(ACTOR_C);
	Engine_ActorSetSpriteFlags(actor_data, 0);
	Engine_ActorSetAnimation(ACTOR_A, 0);
	Engine_ActorSetAnimation(ACTOR_B, 0);
	Engine_ActorSetAnimation(ACTOR_C, 0);
	Engine_TaskWait(20);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	shared_data = ((s32)gVillagerAction);
	Engine_ActorEnableActionCallback(ACTOR_A, shared_data);
	Engine_TaskWait(10);
	Engine_ActorEnableActionCallback(ACTOR_B, shared_data);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_TaskWait(20);
	Object_SetActionCallbackAndRefreshById(ACTOR_C, shared_data);
	Actor_ShowEmote(ACTOR_D, 0x100, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_D, 2);
	Actor_FaceDirection(ACTOR_D, 0xd000, 10);
	Engine_EventSetMessage((s32)MsgHaidiaIUsedToPlayHere);
	Event_ShowMessageAndWait(ACTOR_D, 0, 40);
	Actor_FaceActor(ACTOR_D, ACTOR_PARTY_LEADER, 20);
	Event_ShowMessage(ACTOR_D, 0);
	Actor_FaceDirection(ACTOR_D, FACING_WEST, 10);
	GameFlag_Set(0x305);
	Engine_EventEnd();
}

static void HaidiaMura_MoveActorAlongLowerZPath(s32 a0, s32 a1, s32 a2, s32 a3)
{
	s32				   p10;
	s32				   p8;
	s32				   p9;
	struct FieldActor *rec8;
	u8				  *p5;

	p9 = a3;
	p8 = a1;
	p10 = a2;
	rec8 = Object_GetById(a0);
	p5 = (u8 *)rec8->sprite;
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_WalkToAndWait(a0, 0x188, 0x376);
	Actor_FaceDirection(0, FACING_NORTH, 10);
	rec8->motion_flags = 0;
	((struct FieldSprite *)p5)->flags = 0;
	Engine_ActorSetAnimation(a0, p8);
	Actor_SetSpeed(a0, 0x4ccc, 0x2666);
	Actor_MoveToAndWait(a0, 0x188, 0x36b);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(a0, p10);
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_MoveToAndWait(a0, 0x188, 0x35b);
	p5[38] = 1;
	if (p9 != 0)
	{
		rec8->motion_flags = 3;
	}
	Engine_EventWait(10);
	Engine_ActorSetAnimation(a0, 1);
}

static void HaidiaMura_MoveActorAlongHigherZPath(s32 a0, s32 a1, s32 a2, s32 a3)
{
	s32				   p10;
	s32				   p8;
	s32				   p9;
	struct FieldActor *rec8;
	u8				  *p5;

	p9 = a3;
	p8 = a1;
	p10 = a2;
	rec8 = Object_GetById(a0);
	p5 = (u8 *)rec8->sprite;
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_WalkToAndWait(a0, 0x188, 0x35b);
	Actor_FaceDirection(a0, FACING_NORTH, 10);
	rec8->motion_flags = 0;
	((struct FieldSprite *)p5)->flags = 0;
	Engine_ActorSetAnimation(a0, p8);
	Actor_SetSpeed(a0, 0x10000, 0x8000);
	Actor_MoveToAndWait(a0, 0x188, 0x36b);
	Engine_EventWait(10);
	Actor_SetSpeed(a0, 0x4ccc, 0x2666);
	Engine_ActorSetAnimation(a0, p10);
	Actor_MoveToAndWait(a0, 0x188, 0x37a);
	rec8->velocity_y = 0x20000;
	p5[38] = 1;
	if (p9 != 0)
	{
		rec8->motion_flags = 3;
	}
	Engine_ActorSetAnimation(a0, 1);
}

static void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
	s32 selector = first;
	u32 i;

	if (mode == 0)
	{
		for (i = 0; i < count; i++)
		{
			struct Resource373Actor *actor = Object_GetById(selector);

			actor->flag55 = 0;
			Engine_ActorSetSpriteFlags(actor, 0);
			actor->field08 = 0x01860000;
			actor->field0c = 0x00a00000;
			actor->field10 = 0x034a0000;
			selector++;
		}
		return;
	}

	for (i = 0; i < count; i++)
	{
		Engine_ActorSetPosition(selector, 0, 0);
		selector++;
	}
}

static void FieldScene_RunLargeStagingSequence(void)
{
	u32 i;
	/* FAKEMATCH: rec's dead zero initializer and the none2 temporary preserve register allocation throughout the scene. Without the initializer 170 instructions differ; a plain 0 for none2 is born two instructions later, outranks the callback address and exchanges r9 and r10 with it (14 differ). */
	s32				   rec = 0;
	struct FieldActor *rec3;
	struct FieldActor *rec8;
	u8				  *record;
	s32				   none2;
	s32				   msg;

	rec3 = Object_GetById(ACTOR_PARTY_LEADER);
	rec8 = Object_GetById(14);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	((struct FieldActor *)Engine_EventGetViewCenter())->motion_flags = 0;
	Engine_TaskWait(1);
	Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
	Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
	Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
	Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
	rec = Object_GetById(11);
	((struct FieldActor *)rec)->motion_flags = 0;
	((struct FieldActor *)rec)->x.fixed = 0x1840000;
	((struct FieldActor *)rec)->y.fixed = 0xa00000;
	((struct FieldActor *)rec)->z.fixed = 0x3480000;
	Engine_ActorSetSpriteFlags(rec, 0);
	rec = Object_GetById(12);
	((struct FieldActor *)rec)->motion_flags = 0;
	((struct FieldActor *)rec)->y.fixed = 0xa00000;
	((struct FieldActor *)rec)->z.fixed = 0x34c0000;
	((struct FieldActor *)rec)->x.fixed = 0x1840000;
	Engine_ActorSetSpriteFlags(rec, 0);
	rec = Object_GetById(13);
	((struct FieldActor *)rec)->motion_flags = 0;
	((struct FieldActor *)rec)->z.fixed = 0x3500000;
	((struct FieldActor *)rec)->y.fixed = 0xa00000;
	((struct FieldActor *)rec)->x.fixed = 0x1840000;
	Engine_ActorSetSpriteFlags(rec, 0);
	record = Object_GetById(11);
	Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
	record = Object_GetById(12);
	Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, (s32)gLeaderHammerAction);
	Graphics_EnableObjLayerAndCallbacks();
	msg = (s32)MsgHaidiaRepairCaption;
	UiText_ShowCenteredMessage(msg, 0, 0);
	ObjectDispatch_StopCallbacksAndHideLayers();
	Camera_MoveTo(0x1530000, 0xa00000, 0x4950000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Camera_SetSpeed(0x547a, 0xa8f);
	Camera_MoveTo(0x1280000, 0xa00000, 0x3990000, 1);
	Actor_SetPosition(JASMINE, 0x1990000, 0x46e0000);
	Engine_TaskWait(1);
	Actor_SetSpeed(JASMINE, 0xb333, 0x5999);
	Actor_WalkTo(JASMINE, 0x1a4, 0x42c);
	Audio_PlayCueFromEventWork();
	gEventWork->transition_frames = 60;
	Engine_EventOpenScreen();
	Engine_ActorWaitForMove(JASMINE);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_WalkToAndWait(JASMINE, 0x155, 0x428);
	Actor_SetSpeed(JASMINE, 0xcccc, 0x6666);
	Actor_WalkToAndWait(JASMINE, 0x167, 0x409);
	Actor_SetSpeed(8, 0x8000, 0x4000);
	Actor_WalkTo(8, 0x13e, 0x3b3);
	Engine_ActorSetAnimation(8, 2);
	Actor_WalkToAndWait(JASMINE, 0x19c, 0x409);
	Actor_WalkToAndWait(JASMINE, 0x19c, 0x3fb);
	Actor_WalkToAndWait(JASMINE, 0x176, 0x3f0);
	Actor_WalkToAndWait(JASMINE, 0x15b, 0x3bb);
	Actor_WalkToAndWait(8, 0x13e, 0x3b3);
	Engine_ActorFaceEachOther(JASMINE, 8, 40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(10);
	Actor_WalkTo(8, 0x17b, 0x3f9);
	Camera_SetSpeed(0x8000, 0x1000);
	Actor_WalkToAndWait(JASMINE, 0x14d, 0x398);
	Actor_WalkToAndWait(JASMINE, 0x12b, 0x39c);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Actor_FaceDirection(JASMINE, 0xf000, 30);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x1830000, 0xa00000, 0x3620000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorJump(10, 2, 20);
	Engine_EventSetMessage(msg + 1);
	Event_ShowMessageAndWait(0x100a, 0, 10);
	rec3->scale_x = 0x10000;
	rec3->scale_y = 0x10000;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
	Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 40);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(0x100a, 0, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, (s32)gLeaderHammerAction);
	Engine_CameraFollowActor(JASMINE, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Actor_FaceDirection(JASMINE, 0xd000, 10);
	Actor_WalkToAndWait(JASMINE, 0x138, 0x2f7);
	Actor_WalkToAndWait(JASMINE, 0x169, 0x2f8);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_WEST, 40);
	Actor_FaceDirection(JASMINE, FACING_EAST, 40);
	Event_ShowMessageAndWait(0x6001, 0, 10);
	Actor_ShowEmote(JASMINE, 0x100, 0);
	Engine_ActorJump(JASMINE, 4, 40);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 30);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(JASMINE, 0, 30);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(10);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0x18c0000, -1, 0x24c0000, 1);
	Actor_WalkToAndWait(JASMINE, 0x1c8, 0x2e3);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Actor_ShowEmote(GERALD, 0x105, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	FieldScene_RunStep8C();
	Engine_ActorSetAnimation(GERALD, 17);
	Event_ShowMessageAndWait(0x2001, 0, 20);
	Event_PlayCue(131);
	for (i = 0; i < 60; i++)
	{
		OverlayObject_UpdateOnFrameBit1(Object_GetById(GERALD));
		Engine_TaskWait(1);
	}
	Engine_ActorSetSpritePriority(GERALD, 1);
	Task_AddCallback(SceneState_SetValue1ThenCall, 0xc80);
	Task_AddCallback(FieldScene_RunStep9, 0xc80);
	record = Object_GetById(14);
	Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
	none2 = 0;
	rec8->motion_flags = none2;
	rec8->x.fixed = 0x1ac0000;
	rec8->y.fixed = 0xd00000;
	rec8->z.fixed = 0x2480000;
	rec8->facing = 0x8000;
	rec8->update = (void (*)(union FieldObject *))Effect_ConfigureSpawnedParticle;
	Engine_EventWait(4);
	Actor_SetSpeed(14, 0x20000, 0x20000);
	FieldObject_SetPosition(rec8, 0x1980000, 0xd00000, 0x2480000);
	Engine_EventWait(40);
	Actor_SetSpeed(9, 0x2666, 0x1333);
	Actor_SetSpeed(14, 0x2666, 0x1333);
	// 戻り値は使わない
	Object_GetById(9);
	Engine_ObjectSetPosition((s32)rec8, 0x1880000, 0xd00000, 0x2480000);
	Actor_MoveToAndWait(9, 0x17a, 0x248);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x2005, 0, 10);
	rec8->update = (void (*)(union FieldObject *))none2;
	Engine_ActorSetSpritePriority(GERALD, 2);
	Actor_Get(GERALD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Scheduler_RemoveCallback((s32)SceneState_SetValue1ThenCall);
	Scheduler_RemoveCallback((s32)FieldScene_RunStep9);
	Engine_TaskWait(1);
	Actor_SetChildValue(GERALD, 0);
	Actor_SetChildValue(9, 0);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	Engine_ActorSetAnimation(GERALD, 1);
	Effect_SpawnRisingDustBurst((s32)rec8);
	FieldScene_RunSingleStep();
	Engine_EventWait(10);
	Actor_WalkToAndWait(JASMINE, 0x1a8, 0x270);
	Engine_ActorFaceEachOther(GERALD, JASMINE, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x6001, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_ShowEmote(GERALD, 0x102, 40);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_ShowEmote(GERALD, 0x102, 80);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(10);
	Actor_WalkToAndWait(JASMINE, 0x184, 0x25c);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Actor_ShowEmote(GERALD, 0x105, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, 0x8000, 40);
	Event_ShowTwoMessagesAndWait(1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
	Engine_EventWait(40);
	Actor_SetAttachedEffect(JASMINE, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(80);
	Actor_ShowEmote(JASMINE, 0x101, 40);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(10);
	Actor_ShowEmote(JASMINE, 0x101, 40);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(20);
	Actor_ShowEmote(JASMINE, 0x101, 80);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x1005, 0, 10);
	Actor_FaceDirection(JASMINE, 0x1000, 40);
	Actor_FaceActor(JASMINE, GERALD, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Actor_WalkToAndWait(JASMINE, 0x17c, 0x26c);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x1005, 0, 10);
	Actor_FaceDirection(GERALD, 0x5000, 30);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x6001, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_WalkToAndWait(GERALD, 0x19c, 0x25c);
	Actor_FaceDirection(GERALD, 0x5000, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Engine_ActorJump(JASMINE, 4, 30);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x6001, 0, 20);
	Engine_EventWait(30);
	Actor_FaceDirection(JASMINE, FACING_NORTHEAST, 40);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Actor_ShowEmote(JASMINE, 0x103, 40);
	Actor_SetSpeed(JASMINE, 0xcccc, 0x6666);
	Actor_WalkTo(JASMINE, 0x1ac, 0x274);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	Engine_ActorWaitForMove(JASMINE);
	Event_ShowMessage(0x5001, 0);
	Engine_ActorSetAnimation(JASMINE, 1);
	Engine_EventWait(60);
	Actor_FaceDirection(JASMINE, 0xb000, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_WalkToAndWait(JASMINE, 0x1ac, 0x274);
	Actor_FaceDirection(JASMINE, 0xb000, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(0x2005, 0, 20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(0x5001, 0, 20);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(10);
	Actor_SetSpeed(JASMINE, 0x8000, 0x4000);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_WalkTo(JASMINE, 0x1c2, 0x2ee);
	Actor_WalkTo(GERALD, 0x1c2, 0x2ee);
	Engine_EventWait(60);
	gEventWork->transition_frames = 60;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(12);
	Engine_EventEnd();
}

static void Scene_RepairTheHouse(void)
{
	struct FieldActor *scene;
	struct FieldActor *rec;
	u32				   i;
	s32				   turn_back;
	s32				   turn_side;
	struct FieldActor *turned;
	s32				   none;
	s32				   flag;
	s32				   callback_a;
	s32				   callback_b;
	s32				   callback_c;
	s32				   callback_d;
	s32				   callback_e;
	s32				   callback_f;

	scene = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
	Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
	Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
	Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
	Actor_SetPosition(ACTOR_DORA, 0x1880000, 0x3800000);
	turned = Actor_Get(ACTOR_DORA);

	turned->facing = 0;
	turn_back = 0xc000;
	turned->facing = turn_back;
	Actor_SetPosition(GERALD, 0x12a0000, 0x2e00000);
	turned = Actor_Get(GERALD);
	turn_side = 0x4000;
	turned->facing = turn_side;
	Actor_SetPosition(JASMINE, 0x12a0000, 0x2f80000);
	turned = Actor_Get(JASMINE);
	turned->facing = turn_side;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
	rec = Actor_Get(23);
	rec->motion_flags = 0;
	rec->x.fixed = 0x1840000;
	rec->y.fixed = 0xa00000;
	rec->z.fixed = 0x3480000;
	Engine_ActorSetSpriteFlags(rec, 0);
	rec = Actor_Get(24);
	rec->motion_flags = 0;
	rec->x.fixed = 0x1840000;
	rec->y.fixed = 0xa00000;
	rec->z.fixed = 0x34c0000;
	Engine_ActorSetSpriteFlags(rec, 0);
	rec = Actor_Get(25);
	rec->motion_flags = 0;
	rec->x.fixed = 0x1840000;
	rec->y.fixed = 0xa00000;
	rec->z.fixed = 0x3500000;
	Engine_ActorSetSpriteFlags(rec, 0);
	((u8 *)Engine_EventGetViewCenter())[85] = 0;
	Engine_TaskWait(1);
	Camera_MoveTo(0x17f0000, 0xa00000, 0x36d0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	gEventWork->transition_frames = 32;
	Engine_EventOpenScreen();
	Actor_SetSpeed(JASMINE, 0x8000, turn_side);
	Actor_SetSpeed(GERALD, 0x8000, turn_side);
	Engine_ActorEnableActionCallback(5, (u32)gJasmineAction);
	Engine_ActorEnableActionCallback(1, (u32)gGeraldAction);
	Engine_EventWait(40);
	Engine_ActorEnableActionCallback(0, 1);
	scene->scale_x = 0x10000;
	scene->scale_y = 0x10000;
	Actor_FaceDirection(0, 0xb000, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, turn_back, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	FieldScene_RunStep8C();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
	Engine_TaskAddCallback((void (*)(void))Effect_PlayStepSound, 3200);
	for (i = 0; i < 40; i++)
	{
		OverlayObject_UpdateOnFrameBit1(scene);
		Engine_TaskWait(1);
	}
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);

	callback_a = (s32)SceneState_SetValue0ThenCall;
	Task_AddCallback((void (*)(void))callback_a, 3200);
	callback_b = (s32)FieldScene_RunStep17;
	Task_AddCallback((void (*)(void))callback_b, 3200);
	Actor_SetSpeed(23, 0x3333, 0x1999);
	Actor_MoveToAndWait(23, 390, 832);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(23, 400, 826);
	Engine_EventWait(20);
	{
		struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

		record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
	}
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
	Scheduler_RemoveCallback(callback_a);
	Scheduler_RemoveCallback(callback_b);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(23, 0);
	Actor_SetPosition(23, 0, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
	Engine_EventWait(120);
	Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
	Engine_ActorEnableActionCallback(0, 1);
	scene->scale_x = 0x10000;
	scene->scale_y = 0x10000;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
	Task_AddCallback(Effect_PlayStepSound, 3200);
	for (i = 0; i < 40; i++)
	{
		OverlayObject_UpdateOnFrameBit1(scene);
		Engine_TaskWait(1);
	}
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	callback_c = (s32)SceneState_SetValue0ThenCall;
	Task_AddCallback((void (*)(void))callback_c, 3200);
	callback_d = (s32)SceneState_SetValue24ThenCall;
	Task_AddCallback((void (*)(void))callback_d, 3200);
	Actor_SetSpeed(24, 0x3333, 0x1999);
	Actor_MoveToAndWait(24, 390, 832);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(24, 377, 828);
	Engine_EventWait(20);
	{
		struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

		record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
	}
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
	Scheduler_RemoveCallback(callback_c);
	Scheduler_RemoveCallback(callback_d);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(24, 0);
	Actor_SetPosition(24, 0, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
	Engine_EventWait(120);
	Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
	Engine_ActorEnableActionCallback(0, 1);
	scene->scale_x = 0x10000;
	scene->scale_y = 0x10000;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
	Actor_FaceDirection(ACTOR_DORA, 0xb000, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
	Task_AddCallback(Effect_PlayStepSound, 3200);
	for (i = 0; i < 40; i++)
	{
		OverlayObject_UpdateOnFrameBit1(scene);
		Engine_TaskWait(1);
	}
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	callback_e = (s32)SceneState_SetValue0ThenCall;
	Task_AddCallback((void (*)(void))callback_e, 3200);
	callback_f = (s32)SceneState_SetValue25ThenCall;
	Engine_TaskAddCallback(callback_f, 3200);
	Actor_SetSpeed(25, 0x3333, 0x1999);
	Actor_MoveToAndWait(25, 390, 832);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_MoveToAndWait(25, 360, 837);
	Engine_EventWait(20);
	{
		struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

		record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
	}
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Scheduler_RemoveCallback((s32)Effect_PlayStepSound);
	Scheduler_RemoveCallback(callback_e);
	Scheduler_RemoveCallback(callback_f);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	Actor_SetChildValue(25, 0);
	Actor_SetPosition(25, 0, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
	Engine_ActorEnableActionCallback(0, (u32)gLeaderHammerAction);
	Engine_EventWait(120);
	FieldScene_RunSingleStep();
	Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
	Engine_ActorEnableActionCallback(0, 1);
	scene->scale_x = 0x10000;
	scene->scale_y = 0x10000;
	Engine_ActorJump(ACTOR_DORA, 2, 20);
	Engine_EventSetMessage((s32)MsgHaidiaGoodJob);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 10);
	HaidiaMura_MoveActorAlongLowerZPath(21, 5, 6, 0);
	Actor_SetSpeed(ACTOR_DORA, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
	Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Event_OpenMessage(ACTOR_DORA, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
		gEventWork->message++;
	}
	else
	{
		Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	}
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_EventSetMessage((s32)MsgHaidiaWorkingYourselvesBone);
	Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
	Event_OpenMessage(ACTOR_DORA, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, 0xd000, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventSetMessage((s32)MsgHaidiaDevastatedWhenKyle);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Camera_SetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x1790000, 0xa00000, 0x35c0000, 1);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_WalkTo(GERALD, 369, 904);
	Actor_WalkToAndWait(JASMINE, 392, 904);
	Engine_ActorSetAnimation(GERALD, 1);
	HaidiaMura_MoveActorAlongLowerZPath(5, 10, 11, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x1000, 20);
	Engine_ActorJump(JASMINE, 4, 0);
	Actor_WalkToAndWait(JASMINE, 392, 843);
	Actor_FaceDirection(JASMINE, 0x9000, 0);
	Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(21, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Engine_ActorSetAnimation(ACTOR_DORA, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	HaidiaMura_MoveActorAlongLowerZPath(1, 10, 11, 0);
	Actor_SetSpeed(JASMINE, 0x4ccc, 0x2666);
	Actor_SetSpeed(GERALD, 0x4ccc, 0x2666);
	Actor_WalkTo(GERALD, 392, 843);
	Object_GetById(5)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(JASMINE, 408, 843);
	Engine_EventWait(1);
	{
		struct FieldActor *record = Actor_Get(JASMINE);

		record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
	}
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Engine_ActorWaitForMove(GERALD);
	Engine_ActorSetAnimation(GERALD, 1);
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorJump(ACTOR_DORA, 4, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 258);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, 0x3000, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(ACTOR_DORA, 257, 80);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 30);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
	Actor_FaceDirection(ACTOR_DORA, 0x3000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(80);
	Engine_ActorFaceEachOther(JASMINE, GERALD, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_DORA, 261, 60);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 10);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(JASMINE, 0, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(0x1750000, 0xa00000, 0x3450000, 1);
	Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Actor_FaceDirection(ACTOR_DORA, 0x3000, 10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
	Engine_ActorFaceEachOther(JASMINE, GERALD, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(5, 0x8000, 20);
	Actor_FaceDirection(ACTOR_DORA, 0x5000, 20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Event_OpenMessage(ACTOR_DORA, 0);
	none = 0;

	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_EventSetMessage((s32)MsgHaidiaAsStubbornAsYourFather);
	Actor_ShowEmote(ACTOR_DORA, 259, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Engine_ActorJump(ACTOR_DORA, 4, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Engine_ActorSetAnimation(ACTOR_DORA, 7);
	Engine_EventWait(5);
	Event_ShowTwoMessagesAndWait(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
	Event_PlayCue(161);
	rec = Actor_Get(ACTOR_DORA);
	rec->sprite->flags = none;
	rec->steering_flags &= 0xfe;
	Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
	Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
	Engine_EventWait(4);
	for (i = 0; i != 4; i++)
	{
		rec->z.fixed += 0x18000;
		rec->scale_y += -0x1999;
		Engine_EventWait(1);
	}
	Actor_SetPosition(ACTOR_DORA, 0, 0);
	Actor_SetSpeed(GERALD, 0x30000, 0x18000);
	Engine_ActorJump(GERALD, 6, 0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	Actor_WalkToAndWait(GERALD, 374, 829);
	Actor_SetPosition(GERALD, PIXELS(374), PIXELS(829));
#else
	Actor_WalkToAndWait(GERALD, 374, 827);
#endif
	Event_ShowMessage(JASMINE, 0);
	Actor_FaceDirection(GERALD, 0xb000, 0);
	Actor_ShowEmote(JASMINE, 256, 0);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Actor_ShowEmote(GERALD, 256, 10);
	Engine_ActorSetAnimation(GERALD, 13);
	Engine_ActorJump(GERALD, 2, 5);
	Event_PlayCue(143);
	Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
	Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 10);
	Engine_ActorStartRepeatedMotion(GERALD, 3);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_ShowEmote(GERALD, 258, 80);
	Engine_ActorSetAnimation(ACTOR_DORA, 8);
	rec->scale_y = 0x8000;
	Actor_SetPosition(ACTOR_DORA, 0x16c0000, 0x32b0000);
	for (i = 0; i != 5; i++)
	{
		rec->scale_y += 0x1999;
		Engine_EventWait(1);
	}
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_FaceDirection(GERALD, 0x5000, 30);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Camera_SetSpeed(0x4ccc, 0x999);
	Camera_MoveTo(0x1740000, 0xa00000, 0x35b0000, 1);
	Actor_SetSpeed(ACTOR_DORA, 0x30000, 0x18000);
	Engine_ActorJump(ACTOR_DORA, 6, 0);
	Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
	rec->priority_flags &= 0xfe;
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
	Actor_ShowEmote(ACTOR_DORA, 257, 80);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_SetAttachedEffect(ACTOR_DORA, 258);
	Engine_EventWait(80);
	Actor_FaceDirection(GERALD, 0x5000, 40);
	Actor_ShowEmote(GERALD, 258, 80);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(GERALD, FACING_WEST, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 1);
	Engine_ActorJump(GERALD, 6, 0);
	Engine_ActorSetAnimation(GERALD, 1);
	Actor_SetSpeed(GERALD, 0x40000, 0x20000);
	rec = Actor_Get(GERALD);
	rec->steering_flags &= ~ACTOR_FACES_MOTION;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	Actor_SetDestination(GERALD, 403, 829);
#else
	Actor_SetDestination(GERALD, 403, 827);
#endif
	Actor_SetAttachedEffect(JASMINE, 258);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 20);
	Event_ShowMessageAndWait(JASMINE, 0, 1);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, 0x5000, 20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_ShowEmote(GERALD, 256, 0);
	Engine_ActorSetAnimation(GERALD, 13);
	Engine_ActorJump(GERALD, 2, 5);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
	Event_PlayCue(143);
	Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Actor_ShowEmote(GERALD, 258, 30);
	Actor_SetSpeed(JASMINE, 0x4ccc, 0x2666);
	Actor_WalkToAndWait(JASMINE, 408, 855);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_ShowEmote(ACTOR_DORA, 261, 60);
	Engine_ActorStartRepeatedMotion(JASMINE, 3);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(80);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
	Actor_FaceDirection(JASMINE, 0xb000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
	Actor_ShowEmote(ACTOR_DORA, 261, 80);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
	Actor_ShowEmote(JASMINE, 257, 0);
	Actor_ShowEmote(GERALD, 257, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_WEST, 60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Actor_ShowEmote(ACTOR_DORA, 256, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
	Engine_EventWait(30);
	Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Engine_ActorJump(GERALD, 4, 0);
	Engine_ActorWalkToAndWait(1, 398, 828);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
	Engine_EventWait(60);
	rec = Actor_Get(GERALD);
	flag = 1;
	rec->steering_flags |= flag;
	rec = Actor_Get(JASMINE);
	rec->steering_flags |= flag;
	rec = Actor_Get(ACTOR_PARTY_LEADER);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(JASMINE, 0x10000, 0x8000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_WalkTo(JASMINE, rec->x.part.pixel + 16, rec->z.part.pixel);
	Actor_WalkToAndWait(GERALD, rec->x.part.pixel + 16, rec->z.part.pixel - 16);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, 0x5000, 30);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(JASMINE, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(40);
	Actor_WalkToAndWait(JASMINE, rec->x.part.pixel, rec->z.part.pixel);
	Actor_SetPosition(JASMINE, 0, 0);
	Actor_WalkToAndWait(GERALD, rec->x.part.pixel, rec->z.part.pixel);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_PartyAddMembers(1, 5);
	Camera_MoveTo(0x1790000, 0xa00000, 0x3770000, 1);
	HaidiaMura_MoveActorAlongHigherZPath(0, 13, 10, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	{
		struct FieldActor *record = Actor_Get(ACTOR_DORA);

		record->steering_flags = flag | record->steering_flags;
	}
	HaidiaMura_MoveActorAlongHigherZPath(21, 6, 5, 0);
	Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
	Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	Engine_ActorSetAnimation(ACTOR_DORA, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(100);
	Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
	GameFlag_Set(514);
	GameFlag_Clear(303);
#if EDITION_INTERNATIONAL
	scene->motion_flags = 3;
	scene->y.fixed = 0xa00000;
	scene->target_y = 0x80000000;
	scene->velocity_y = 0;
#else
	Object_GetById(ACTOR_PARTY_LEADER)->motion_flags = 3;
#endif
	Engine_EventEnd();
}

static void FieldScene_RunStep8C(void)
{
	Engine_PsynergyBegin(0x8c, 0);
}

static void FieldScene_RunSingleStep(void)
{
	BattleEffect_CleanupSceneObjects();
}

static void SceneState_SetValue1ThenCall(void)
{
	SceneEffect_UpdateObjectOnOddFrames((s32)Object_GetById(GERALD));
}

static void SceneState_SetValue0ThenCall(void)
{
	SceneEffect_UpdateObjectOnOddFrames((s32)Object_GetById(ACTOR_PARTY_LEADER));
}

static void FieldScene_RunStep9(void)
{
	SceneEffect_UpdateObjectOnOddFramesOnly((s32)Object_GetById(9));
}

static void FieldScene_RunStep17(void)
{
	SceneEffect_UpdateObjectOnOddFramesOnly((s32)Object_GetById(0x17));
}

static void SceneState_SetValue24ThenCall(void)
{
	SceneEffect_UpdateObjectOnOddFramesOnly((s32)Object_GetById(0x18));
}

static void SceneState_SetValue25ThenCall(void)
{
	SceneEffect_UpdateObjectOnOddFramesOnly((s32)Object_GetById(0x19));
}

static s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

static s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force)
{
	s32 result;
	u32 angle;
	u32 left;
	u32 right;
	u32 dir;

	result = 0;
	if (obj->paused == 1)
	{
		if (obj->rise_counter == 0)
		{
			Object_SetMode(obj, 1);
			return 1;
		}
	}
	if (Runtime_ComputeFixedPointDistance(&target->x.fixed, &obj->x.fixed) < range || force != 0)
	{
		angle = (u16)ArcTan2(target->z.fixed - obj->z.fixed, target->x.fixed - obj->x.fixed);
		left = (angle - 0x1000) & 0xf000;
		right = (angle + 0x1000) & 0xf000;
		angle &= 0xf000;
		dir = obj->facing & 0xf000;
		if (angle == dir || right == dir || left == dir || force != 0)
		{
			obj->paused = 1;
			Object_SetMode(obj, 1);
			result = 1;
			obj->rise_counter = result;
		}
		else
		{
			obj->paused = 0;
			Object_SetMode(obj, 2);
			obj->rise_counter = 0;
		}
	}
	else
	{
		obj->paused = 0;
		Object_SetMode(obj, 2);
		obj->rise_counter = 0;
	}
	return result;
}

static s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
	struct FieldActor *t = Object_GetById(ACTOR_PARTY_LEADER);
	if (p[14] == 0x80000000 && p[16] == 0x80000000)
		return 0;
	HaidiaMura_TestFacing((struct FieldActor *)p, t, 18, 0);
	return 0;
}

static void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
	s32					   spawn_position[3];
	s32					   particle_index;
	struct StagedParticle *particle;
	spawn_position[0] = source->f08;
	spawn_position[1] = source->f0c - (((s32 (*)())Engine_RandomNext)(source) << 4) - 0x80000;
	spawn_position[2] = source->f10;
	particle_index = Engine_RandomNext();
	Vector_AddPolarOffset(
		((particle_index << 1) + particle_index) << 4, Engine_RandomNext(), spawn_position);
	particle = Engine_ObjectCreate(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
	if (particle != 0)
	{
		particle->f55 = 2;
		particle->f48 = 0x1999;
		particle->f5e = 12;
		Engine_ActorSetSpriteFlags(particle, 0);
		Object_SetMode(particle, 0);
		Engine_ObjectSetScript(particle, (s32)gDustBurstScript);
		{
			struct ParticleRecord *record = particle->f50;
			s32					   record_flags = ~12;
			record_flags &= record->f09;
			record_flags |= 4;
			record->f09 = record_flags;
		}
	}
	Engine_AudioPlayCue(0x8a);
}

static void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
	s32 frame_countdown;

	Engine_AudioPlayCue(154);

	for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--)
	{
		emitter->y += 0x10000;
		emitter->field06 = (u16)(emitter->field06 + 0x2000);
		emitter->field18 += -2048;
		emitter->field1c += -2048;
		Engine_TaskWait(1);
	}

	for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--)
	{
		struct Resource373Particle *particle =
			Engine_ObjectCreate(0x11d, emitter->x, emitter->y, emitter->z);

		if (particle != 0)
		{
			s32 vertical_speed;

			Engine_ActorSetSpriteFlags(particle, 0);
			Engine_ObjectSetScript(particle, (const void *)&gDustBurstScript[1]);

			vertical_speed = Engine_RandomNext() + 0x10000;
			particle->field34 = 0x10000;
			particle->field30 = vertical_speed;
			particle->field55 = 2;
			particle->field48 = 0x0a3d;

			particle->lifetime = Engine_RandomNext() - Engine_RandomNext();

			Effect_UpdateParticlePosition(
				particle, ((Engine_RandomNext() * 3) << 3) + 0x80000, Engine_RandomNext());
		}
	}

	Engine_AudioPlayCue(131);

	emitter->x = 0;
	emitter->y = 0;
	emitter->z = 0;
	emitter->field38 = 0x80000000;
	emitter->field3c = 0x80000000;
	emitter->field40 = 0x80000000;
	emitter->field24 = 0;
	emitter->field28 = 0;
	emitter->field2c = 0;
}

static void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
	s32 position[3];
	if (particle != 0)
	{
		position[0] = particle[2];
		position[1] = particle[3];
		position[2] = particle[4];
		Vector_AddPolarOffset(delta_x, delta_z, position);
		Engine_ObjectSetPosition((s32)particle, position[0], position[1], position[2]);
	}
}

static void SceneState_ApplyRectAndRunTwo(void)
{
	s32 e;
	s32 f;
#if !EDITION_INTERNATIONAL
	// 日本版のみ: 村人を引くが戻り値は使わない
	if (Engine_GameFlagIsSet(0x87a))
		Object_GetById(ACTOR_DORA);
	else
		Object_GetById(20);
#endif
	e = 22;
	f = 36;
	Engine_MapCopyCellAttributes(17, 0, 3, 1, e, f);
	StagedActor_AdvancePair();
	HaidiaMura_OpenVillagerLane();
}

static void HaidiaMura_OpenVillagerLane(void)
{
	struct FieldActor *actor;
	s32				   column;

	Map_CopyCellAttributes(17, 0, 3, 1, 22, 36);
	if (Engine_GameFlagIsSet(0x87a))
		actor = Object_GetById(21);
	else
		actor = Object_GetById(20);
	if (actor == NULL)
		return;
	GameFlag_Clear(0x314);
	GameFlag_Clear(0x315);
	GameFlag_Clear(0x316);
	column = actor->x.fixed >> 20;
	if (column == 22)
	{
		Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
		Engine_GameFlagSet(0x314);
	}
	else if (column == 23)
	{
		Engine_MapCopyCellAttributes(17, 1, 1, 1, column, 36);
		Engine_GameFlagSet(0x315);
	}
	else
	{
		Map_CopyCellAttributes(17, 1, 1, 1, 24, 36);
		Engine_GameFlagSet(0x316);
	}
}

static void Effect_PlayStepSound(void)
{
	if ((*(u32 *)&gFrameCount & 15) == 0)
		Engine_AudioPlayCue(0x83);
}

static void EV24_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
	Engine_EventEnd();
}

static void FieldScene_RunScene373SequenceB(void)
{
	s32 rec7;

	if (GameFlag_IsSet(0x241) != 0)
	{
		rec7 = GameFlag_IsSet(0x106);
		if (rec7 != 0)
		{
			goto L_02005a8a;
		}
		Object_GetById(22)->paused = rec7;
		GameFlag_Clear(0x241);
	}
	else
	{
		if (GameFlag_IsSet(0x106) != 0)
		{
			Object_GetById(22)->paused = 1;
			GameFlag_Set(0x241);
		}
	}
L_02005a8a:;
}

static void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
	s32				  *g = Object_GetById(ACTOR_PARTY_LEADER);
	struct FieldActor *q;
	if (Engine_GameFlagIsSet(0x87a) != 0)
		q = Object_GetById(21);
	else
		q = Object_GetById(20);
	if (q != 0)
	{
		if (g[3] > 0xc80000)
			q->priority_flags = 3;
		else
			q->priority_flags = 1;
	}
}

static s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
	s16 *q = (s16 *)p[20];
	s32	 a, b;
	s32	 d = Engine_MathSin(p[12]) * 2;
	if (d > 0)
		d = -d;
	p[2] = p[14] + Engine_MathCos(p[12]) * 2;
	p[3] = p[15] + d;
	q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
	a = Engine_RandomNext();
	b = Engine_RandomNext();
	p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
	return 0;
}

static void InitializeStagedActorSceneOrbitingEffect(s32 id)
{
	OrbitingSceneObject		  *actor;
	OrbitingSceneObjectSprite *sprite;
	u8						  *transfer;
	s32						   zero;

	actor = (OrbitingSceneObject *)Object_GetById(id);
	sprite = actor->sprite;
	sprite->flags_09_mode = 1;
	sprite->flags_05_bit_5 = 0;
	sprite->flags_09_high = 0;

	zero = 0;
	sprite->state = zero;
	Engine_ActorSetSpriteFlags(actor, zero);
	actor->active = zero;
	actor->mode = zero;

	if (Engine_GameFlagIsSet(0x109) == 0)
		actor->y += 0x200000;

	actor->flags_23 &= 0xfe;
	actor->visible = 1;

	transfer = Engine_HeapAllocate(17, 0x608);
	Engine_ItemLoadIcon(ITEM_NUT);
	transfer += 0x400;
	Engine_VramLoad(sprite->palette, 128, transfer);
	Engine_HeapRelease(17);

	actor->orbit_center_x = actor->x;
	actor->orbit_angle = zero;
	actor->orbit_center_y = actor->y;
	actor->active = 1;
	actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
	actor->state = zero;
}

static void OverlayObject_UpdateOnFrameBit1(s32 p)
{

	if ((gFrameCount & 2) != 0)
		Engine_ObjectSetPartPalettes(p, 7);
	else
		Engine_ObjectSetPartPalettes(p, 0);
	if ((gFrameCount & 0xf) == 0)
		WorldMap_CreateLinkedEffects((void *)p);
}

static void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
	if ((gFrameCount & 1) != 0)
		Engine_ObjectSetPartPalettes(p, (gFrameCount >> 1) % 6);
	if ((gFrameCount & 0xf) == 0)
		WorldMap_CreateLinkedEffects((void *)p);
}

static void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
	if ((gFrameCount & 1) != 0)
		Engine_ObjectSetPartPalettes(p, (gFrameCount >> 1) % 6);
}

static void Effect_AnimateVerticalPositive(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed + (0x10000 - amplitude) * 5 + 0x80000;
}

static void Effect_AnimateVerticalNegative(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = -amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed - (0x10000 - amplitude) * 5 + 0x100000;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../world_map/linked_effects.c"

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

s32 gHaidiaMuraActor22Actions[] = {
	SC_SET(SW_SPEED, 0x8000),
	SC_SET(SW_ACCEL, 0x4000),
	SC_SET(0x23, 2),
	SC_LABEL(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(296), 0, PIXELS(456)),
	SC_SYNC,
	SC_SET(0x23, 3),
	SC_MOVE(PIXELS(296), 0, PIXELS(420)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_ARG(2),
	SC_MOVE(PIXELS(296), 0, PIXELS(456)),
	SC_SYNC,
	SC_SET(0x23, 2),
	SC_MOVE(PIXELS(256), 0, PIXELS(456)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(15),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(60),
	SC_ARG(2),
	SC_MOVE(PIXELS(212), 0, PIXELS(456)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_MOVE(PIXELS(212), 0, PIXELS(388)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(40),
	SC_ARG(2),
	SC_MOVE(PIXELS(212), 0, PIXELS(429)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(30),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_ARG(2),
	SC_MOVE(PIXELS(212), 0, PIXELS(456)),
	SC_SYNC,
	SC_SET(0x23, 2),
	SC_MOVE(PIXELS(256), 0, PIXELS(456)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(40),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gLeaderHammerAction[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, 1966),
	SC_WAIT(12),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(12),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 gGeraldAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(300), 0, PIXELS(880)),
	SC_SYNC,
	SC_MOVE(PIXELS(300), 0, PIXELS(904)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_END,
};

s32 gJasmineAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(300), 0, PIXELS(904)),
	SC_SYNC,
	SC_MOVE(PIXELS(321), 0, PIXELS(904)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_END,
};

s32 gVillagerAction[] = {
	SC_ARG(1),
	SC_POS(PIXELS(266), 0, PIXELS(1230)),
	SC_SET(SW_SCALE_X, 0x1999),
	SC_SET(SW_SCALE_Y, 0x1999),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 3932),
	SC_ADD(SW_SCALE_Y, 0x147a),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 gDustBurstScript[] = {
	SC_FREE,
	SC_SYNC,
	SC_SET(SW_SPEED, 0x1000),
	SC_ARG(0),
	SC_WAIT(10),
	SC_FREE,
};

struct SceneEntrance gHaidiaMuraEntrances[] = {
	{ 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 16, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 17, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gHaidiaMuraExits[] = {
	SCENE_EXITS(4),
	SCENE_EXIT(1, 19, 1),
	SCENE_EXIT(2, 5, 8),
	SCENE_EXIT(3, 6, 1),
	SCENE_EXIT(4, 6, 2),
	SCENE_EXIT(5, 7, 6),
	SCENE_EXIT(6, 7, 5),
	SCENE_EXIT(7, 8, 6),
	SCENE_EXIT(8, 7, 2),
	SCENE_EXIT(9, 7, 1),
	SCENE_EXIT(10, 7, 2),
	SCENE_EXIT(11, 9, 1),
	SCENE_EXIT(12, 4, 17),
	SCENE_EXITS_END,
};

struct ScenePlacement gHaidiaMuraPlacements[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, 3,
		PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(412), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x25, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(398), 0, PIXELS(616), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(373), 0, PIXELS(887), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#else
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(373), 0, PIXELS(889), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#endif
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x16, 0xfd0, 7,
		PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaMuraPlacements2[] = {
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, 3,
		PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, 4,
		PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(412), 0, PIXELS(586), FACING_WEST, 3, 0 },
	{ 0x25, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(398), 0, PIXELS(616), FACING_NORTH, 3, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(335), 0, PIXELS(869), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x16, 0xfd0, 7,
		PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaMuraPlacements3[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(412), 0, PIXELS(586), FACING_NORTH + FACING_STEP, 3, 0 },
	{ 0x25, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(356), 0, PIXELS(727), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, 3,
		PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0xfd0, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaMuraPlacements4[] = {
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(409), 0, PIXELS(1134), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(428), 0, PIXELS(588), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(204), 0, PIXELS(924), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(394), 0, PIXELS(584), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(373), 0, PIXELS(887), FACING_NORTH, TALK_FACE_PARTY, 0 },
#else
	{ 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(373), 0, PIXELS(889), FACING_NORTH, TALK_FACE_PARTY, 0 },
#endif
	{ 0xce, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xce, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xd7, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gHaidiaMuraEvents[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaMtAlephLooksBeautifulTodayIts) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaValeStaysPristineBecauseOfMt) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaThisPlaceLooksJustLikeIt) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaRobinWatchOutDontGetToo) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaValeMustRemainHiddenPsynergysSecrets) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaYoureInTroubleNowGeraldYour) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgHaidiaMtAlephIsASacredPeak) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TOUCH) },
	{ EVENT_TOUCH, 50, 0x823, EVENT_SCRIPT(EV50_TOUCH) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EV51_TOUCH) },
	{ EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EV52_TOUCH) },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EV100_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EV101_TOUCH) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0xfd0, EVENT_SCRIPT(EV26_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaMura_OpenVillagerLane) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SEARCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaMuraEvents2[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaValeIsSavedThanksToThe) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaEverSinceTheTremorsStartedMt) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaSighNoLongerWillISee) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaIDontBlameGarsiaIfOnly) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheMayorWasTooKindTo) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaYouGuysAreSoLuckyGerald) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaPleaseTakeCareOfMyBrother) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgHaidiaItIsVeryDangerousToGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgHaidiaIAmAllAloneInThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaGeeIWishICouldSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaMyBrothersStrengthIsHisBest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaImGladThatGarsiaIsAlive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaGarsiaWasAGoodPersonI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaPoorKradenWasTakenToSome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaValeWasSavedBecausePeopleLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaICantBelieveAnyoneWouldClimb) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaIJustDontFeelSafeKnowing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgHaidiaThePeopleOfValeWillNever) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TOUCH) },
	{ EVENT_TOUCH, 50, 0x823, EVENT_SCRIPT(EV50_TOUCH) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(EV51_TOUCH) },
	{ EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(EV52_TOUCH) },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(EV53_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EV100_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EV101_TOUCH) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0xfd0, EVENT_SCRIPT(EV26_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaMura_OpenVillagerLane) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SEARCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaMuraEvents3[] = {
	{ EVENT_TALK, 8, 0x303, EVENT_SCRIPT(FieldScene_RunSecondaryActorSequence) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaICanTellJustByLooking) },
	{ EVENT_TALK, 9, 0x304, EVENT_SCRIPT(FieldScene_RunPrimaryActorSequence) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaIWillDoMyBestTo) },
	{ EVENT_TALK, 10, 0x81f, EVENT_SCRIPT(SceneDialogue_RunActor10LineAndFlag81f) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaPromiseMeRobinPromiseMeYoull) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaIUsedToPlayHere) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaMtAlephsEruptionWasSoPowerful) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheySayTheDangerIsGone) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorNineteenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x303, EVENT_SCRIPT(FieldScene_RunSecondaryActorSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgHaidiaYouGuysAreWorkingSoHard) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x304, EVENT_SCRIPT(FieldScene_RunPrimaryActorSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaICouldGoWithYouIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, 0x81f, EVENT_SCRIPT(SceneDialogue_RunActor10LineAndFlag81f) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaYouDidntBringJasminWithYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgHaidiaGarsiaMustBeProtectingJasminIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheEarthquakesArentCausedByEruptions) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgHaidiaAhhICantStandTheseEarthquakes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgHaidiaTheSmokeFromMtAlephIsnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x413, 0x307, EVENT_SCRIPT(SceneDialogue_RunActorNineteenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgHaidiaHavingNoTravelersIsNoFun) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EV100_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EV101_TOUCH) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 20, 0xfd0, EVENT_SCRIPT(EV20_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 21, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaMura_OpenVillagerLane) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndRunTwo) },
	{ SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SEARCH) },
	{ SCENE_EVENTS_END },
};

u16 gHaidiaMuraCellAnimC[] = {
	MAP_CELL_STEP(0, 98, 2, 2, 2), MAP_CELL_STEP(2, 98, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 gHaidiaMuraCellAnimA[] = {
	MAP_CELL_STEP(0, 96, 2, 2, 2), MAP_CELL_STEP(2, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 gHaidiaMuraCellAnimB[] = {
	MAP_CELL_STEP(4, 98, 2, 2, 2), MAP_CELL_STEP(4, 98, 2, 2, 2), MAP_CELL_STEPS_END,
};

u16 gHaidiaMuraCellAnimD[] = {
	MAP_CELL_STEP(4, 96, 2, 2, 2), MAP_CELL_STEP(4, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};

s32 gHaidiaMuraLeaderWalkActions[] = {
	SC_ARG(5),
	SC_MOVE(PIXELS(199), 0, PIXELS(446)),
	SC_SYNC,
	SC_MOVE(PIXELS(215), 0, PIXELS(472)),
	SC_SYNC,
	SC_MOVE(PIXELS(239), 0, PIXELS(461)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gHaidiaMuraActor22WalkActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(199), 0, PIXELS(446)),
	SC_SYNC,
	SC_MOVE(PIXELS(215), 0, PIXELS(472)),
	SC_SYNC,
	SC_MOVE(PIXELS(239), 0, PIXELS(461)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gHaidiaMuraPairTableA[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_ARG(5),
	SC_MOVE(PIXELS(224), 0, PIXELS(458)),
	SC_SYNC,
	SC_MOVE(PIXELS(256), 0, PIXELS(458)),
	SC_SYNC,
	SC_MOVE(PIXELS(256), 0, PIXELS(474)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

s32 gHaidiaMuraPairTableB[] = {
	SC_SET(SW_SPEED, 0x18000),
	SC_SET(SW_ACCEL, 0x10000),
	SC_ARG(2),
	SC_MOVE(PIXELS(216), 0, PIXELS(458)),
	SC_SYNC,
	SC_MOVE(PIXELS(256), 0, PIXELS(456)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};

s32 gHaidiaMuraPairTableC[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_ARG(5),
	SC_MOVE(PIXELS(256), 0, PIXELS(458)),
	SC_SYNC,
	SC_MOVE(PIXELS(256), 0, PIXELS(474)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

s32 gHaidiaMuraPairTableD[] = {
	SC_SET(SW_SPEED, 0x18000),
	SC_SET(SW_ACCEL, 0x10000),
	SC_ARG(2),
	SC_MOVE(PIXELS(256), 0, PIXELS(456)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};
