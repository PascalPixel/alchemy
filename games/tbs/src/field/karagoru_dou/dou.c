#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/karagoru_dou/dou.h"
#include "unit/field/common/effect/move.h"
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
#include "staged_actor_pair_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "fieldobj.h"

// 前方宣言
static void EV09_PSYNERGY_3(void);
static void StagedActorPairScene_RunSpinningLeap(s32 id);

static void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

static void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 0;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 14);
		Engine_ObjectSetBlendMode(object, 1);
		return object;
	}
	return 0;
}

static void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 1;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 15);
		object->priority_flags = (object->priority_flags & 0xfe) | 2;
		return object;
	}
	return 0;
}

union SpinningActor
{
	union FieldObject  object;
	struct StagedActor staged;
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"
#include "../common/object/staged_actor.c"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KaragoruDou1)
	{
		return gKaragoruDouEntrances1;
	}
	if (scene == (s32)&SceneId_KaragoruDou2)
	{
		return gKaragoruDouEntrances2;
	}
	if (scene == (s32)&SceneId_KaragoruDou3)
	{
		return gKaragoruDouEntrances3;
	}
	return gKaragoruDouEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gKaragoruDouExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KaragoruDou1)
	{
		if (Engine_GameFlagIsSet(0x96f) != 0)
		{
			return gKaragoruDouPlacements1Flag96f;
		}
		return gKaragoruDouPlacements1;
	}
	if (scene == (s32)&SceneId_KaragoruDou2)
	{
		return gKaragoruDouPlacements2;
	}
	if (scene == (s32)&SceneId_KaragoruDou3)
	{
		return gKaragoruDouPlacements3;
	}
	return gKaragoruDouPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KaragoruDou1)
	{
		if (Engine_GameFlagIsSet(0x96f) != 0)
		{
			return gKaragoruDouEvents1Flag96f;
		}
		return gKaragoruDouEvents1;
	}
	if (scene == (s32)&SceneId_KaragoruDou2)
	{
		return gKaragoruDouEvents2;
	}
	if (scene == (s32)&SceneId_KaragoruDou3)
	{
		return gKaragoruDouEvents3;
	}
	return gKaragoruDouEventsOther;
}

static void EV10_TOUCH(void)
{
	struct FieldActor *leader;

	if (GameFlag_IsSet(0x98a) == 0 && GameFlag_IsSet(0x9a0) != 0)
	{
		Engine_EventBegin();
		Engine_ActorSetSpeed(11, 0x10000, 0x8000);
		leader = Actor_Get(ACTOR_PARTY_LEADER);
		if (leader != 0)
		{
			Actor_SetPosition(11, leader->x.fixed, leader->z.fixed);
		}
		Engine_ActorSetDestinationOffset(11, -8, 16);
		Engine_ActorWaitForMove(11);
		Actor_FaceDirection(11, 0xd000, 0);
		Engine_EventWait(10);
		Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
		Engine_EventSetMessage((s32)MsgKaragoruWhyGoingBackRobinDo);
		Event_OpenMessage(11, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Event_ShowMessage(11, 0);
			Actor_WalkTo(11, 152, 232);
			GameFlag_Clear(0x9a0);
			Engine_ActorWaitForMove(11);
			Engine_ActorSetAnimation(11, 1);
			gGameState.saved_scene = (s32)&SceneId_TorebiHeya;
			gGameState.saved_entrance = 30;
		}
		else
		{
			gEventWork->message += 1;
			Event_ShowMessage(11, 0);
			Engine_ActorSetAnimation(11, 2);
			leader = Actor_Get(ACTOR_PARTY_LEADER);
			if (leader != 0)
			{
				Actor_SetDestination(11, leader->x.part.pixel, leader->z.part.pixel);
			}
			Engine_ActorWaitForMove(11);
			Actor_SetPosition(11, 0, 0);
			Engine_EventWait(30);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
			Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
			Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
		}
		Engine_EventEnd();
	}
}

static void EV11_SCRIPT(void)
{
	Engine_EventBegin();
	Engine_ActorFaceActor(11, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 11, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(10);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
	Engine_EventSetMessage((s32)MsgKaragoruIveBeenWaitingForRobin);
	Engine_EventShowMessage(11, 0);
	Engine_ActorSetAnimation(11, 2);
	{
		s16 *position = Object_GetById(ACTOR_PARTY_LEADER);

		if (position != 0)
			Engine_ActorSetDestination(11, position[5], position[9]);
	}
	Engine_ActorWaitForMove(11);
	Engine_ActorSetPosition(11, 0, 0);
	Engine_EventWait(20);
	Engine_GameFlagSet(2464);
	Engine_EventEnd();
}

static void EV08_TALK(void)
{
	s32 base;

	base = (s32)MsgKaragoruDoYouWishCrossInto;
	Engine_EventSetMessage(base);
	Engine_EventOpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			if (Engine_GameFlagIsSet(0x96f) == 0)
			{
				Engine_EventSetMessage((base + 8));
			}
		}
		Engine_EventShowMessage(8, 0);
	}
	else
	{
		gEventWork->message++;
		Engine_EventShowMessage(8, 0);
	}
}

static void EV09_PSYNERGY_1(void)
{
	if (Engine_GameFlagIsSet(2384) != 0 && Engine_GameFlagIsSet(2415) == 0)
		Engine_EventSetMessage((s32)MsgKaragoruWeMissedColossoBecauseWe);
	else
		Engine_EventSetMessage((s32)MsgKaragoruWarriorsHaveBeenFightingWhile);
	Engine_EventShowMessage(9, 0);
}

static void EV01_TOUCH(void)
{
	struct EventWork *work;

	work = gEventWork;
	Engine_EventBegin();
	if (GameFlag_IsSet(0x204) != 0)
	{
		GameFlag_Clear(0x9a3);
		GameFlag_Clear(0x9a5);
		GameFlag_Clear(0x9a4);
		GameFlag_Clear(0x9a6);
		GameFlag_Set(0x9a5);
		GameFlag_Set(0x9a4);
	}
	Engine_EventRequestExit(work->touched_trigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void StagedActorPairScene_RunStep(void)
{
	Engine_LeaderCheckAhead();
}

static void EV08_PSYNERGY(void)
{
	Object_GetById(8);
	Engine_EventBegin();
	{
		struct FieldActor *actor = Object_GetById(8);

		if ((actor->x.fixed >> 20) <= 30)
		{
			StagedActorPairScene_RunSpinningLeap(8);
			{
				s32 x = 27;
				s32 y = 19;

				Engine_MapCopyCellAttributes(29, 19, 1, 1, x, y);
			}
			Engine_GameFlagSet(2466);
		}
	}
	Engine_EventEnd();
}

static void EV50_SCRIPT(void)
{
	StagedActor_AdvancePair();
	EV09_PSYNERGY_3();
}

static void EV09_PSYNERGY_3(void)
{
	Engine_EventBegin();
	if ((Object_GetById(9)->x.fixed >> 20) > 42)
	{
		s32 x = 107;
		s32 y = 17;

		Map_CopyCellAttributes(108, 17, 1, 1, x, y);
		Engine_EventWait(8);
		Actor_SetPosition(9, 0, 0);
		Actor_SetPosition(10, 45613056, 18874368);
		Engine_ActorSetAnimation(10, 3);
		Event_PlayCue(154);
		GameFlag_Set(2469);
	}
	Engine_EventEnd();
}

static void StagedActorPairScene_NoopActorCallback(void)
{
}

static void StagedActorPairScene_RotateActorPart(u8 *actor)
{
	u8 *sprite_part = *(u8 **)(actor + 80);

	*(u16 *)(sprite_part + 30) -= 0x400;
}

static void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object, s32 limit)
{
	s32 frames = 40;

	while (frames != 0)
	{
		Engine_TaskWait(1);
		frames--;
		if (object->height <= limit)
		{
			break;
		}
	}
}

static void StagedActorPairScene_RunSpinningLeap(s32 id)
{
	union SpinningActor *work;
	u32					 cnt;
	s32					 velocity[3];
	s32					 angle;

	work = (union SpinningActor *)Object_GetById(id);
	work->object.actor.motion_flags = 0;
	for (cnt = 0; cnt < 9; cnt++)
	{
		Engine_TaskWait(1);
		work->object.actor.sprite->rotation -= 0x100;
		work->object.actor.x.fixed -= Engine_MathCos(work->object.actor.sprite->rotation) / 2;
		work->staged.target_x = 0x80000000;
	}
	work->object.actor.update = (void (*)(union FieldObject *))StagedActorPairScene_RotateActorPart;
	Engine_AudioPlayCue(136);
	Actor_SetSpeed(id, 0x20000, 0x10000);
	Actor_SetDestination(id, 472, 288);
	work->object.effect.velocity_y = 0xcccc;
	work->object.actor.motion_flags = 3;
	work->object.actor.terrain_id = 0;
	Engine_ActorWaitForMove(id);
	StagedActorPairScene_WaitForHeight(work, 0x200000);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	for (cnt = 0; cnt < 17; cnt++)
	{
		angle = cnt << 12;
		velocity[0] = Engine_MathCos(angle);
		velocity[1] = 0;
		velocity[2] = Engine_MathSin(angle);
		velocity[0] -= velocity[0] / 4;
		velocity[2] -= velocity[2] / 2;
		Effect_Spawn(work->object.actor.x.fixed, work->object.actor.y.fixed,
			work->object.actor.z.fixed, velocity[0], velocity[1], velocity[2], 0, 0);
	}
	Actor_SetDestination(id, 440, 308);
	Engine_ActorWaitForMove(id);
	StagedActorPairScene_WaitForHeight(work, 0x200000);
	work->object.actor.update = 0;
	work->object.actor.sprite->rotation = 0x1000;
	Engine_AudioPlayCue(154);
	Engine_ActorSetAnimation(id, 3);
	Map_WaitWorkValuesBelow256();
	Engine_EventWait(10);
	Engine_ActorSetAnimation(id, 2);
}

static void StagedActorPairScene_NoopSceneCallback(void)
{
}

static void EV11_PSYNERGY(void)
{
	Engine_ActorSetPosition(12, 0, 0);
}

s32 Scene_Initialize(void)
{
	struct FieldActor *actor;

	if (gGameState.scene == (s32)&SceneId_KaragoruDou1)
	{
		GameFlag_Set(0x144);
		if (GameFlag_IsSet(0x9a0) != 0)
		{
			Actor_SetPosition(11, 0, 0);
		}
	}
	if (gGameState.scene == (s32)&SceneId_KaragoruDou3)
	{
		if (gGameState.entrance == 1)
		{
			Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
		}
		if (GameFlag_IsSet(0x9a2) != 0)
		{
			Actor_SetPosition(8, 0x1b80000, 0x1340000);
			Engine_ActorSetAnimation(8, 2);
			Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
		}
		if (GameFlag_IsSet(0x9a5) != 0)
		{
			Actor_SetPosition(9, 0, 0);
			Actor_SetPosition(10, 0x2b80000, 0x1200000);
			Engine_ActorSetAnimation(10, 2);
		}
		actor = Actor_Get(12);
		Engine_ActorSetSpriteFlags(actor, 0);
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

struct SceneEntrance gKaragoruDouEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
	{ 3, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
	{ 4, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
	{ 2, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
	{ 3, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
	{ 4, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
	{ 5, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
	{ 6, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
	{ SCENE_TABLE_END },
};

u32 gKaragoruDouExits[] = {
	SCENE_EXITS(152),
	SCENE_EXIT(1, 2, 25),
	SCENE_EXIT(2, 2, 54),
	SCENE_EXIT(0, 157, 0),
	SCENE_EXIT(1, 158, 2),
	SCENE_EXIT(2, 157, 3),
	SCENE_EXIT(3, 157, 2),
	SCENE_EXIT(4, 158, 5),
	SCENE_EXIT(0, 158, 0),
	SCENE_EXIT(1, 2, 26),
	SCENE_EXIT(2, 157, 1),
	SCENE_EXIT(3, 158, 4),
	SCENE_EXIT(4, 158, 3),
	SCENE_EXIT(5, 157, 4),
	SCENE_EXIT(6, 2, 41),
	SCENE_EXITS_END,
};

struct ScenePlacement gKaragoruDouPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements1[] = {
	{ 0x96, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements1Flag96f[] = {
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements2[] = {
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements3[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKaragoruDouEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouUsWereTheSoldiersWhoGuard) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEvenIfYouHeadEastYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouThoseWhoGoToGondowanRarely) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_PSYNERGY_1) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouYoudThinkYouCouldGetThrough) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents1Flag96f[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 11, 0x9a0, EVENT_SCRIPT(EV11_SCRIPT) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWhyWouldIodemChooseToWait) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWeHaventSeenAnybodyUnusualCrossing) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEvenIfYouGoEastFrom) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouIodemIsGoingToBabiLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouItsALotOfStressBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEveryoneComesDownThisWayWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWeHaveToHeadForThe) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents3[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ PSYNERGY_ON_ACTOR(0x8f), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x9a2, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x205, EVENT_SCRIPT(EV09_PSYNERGY_3) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunStep) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunStep) },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SCRIPT) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};
