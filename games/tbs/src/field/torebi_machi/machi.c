#include "unit/field/torebi_machi/machi.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/graphics/window/window.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtim13.h"
#include "machi.h"
#include "iwram_call.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void EV09_PSYNERGY(void);

static void EV31_TALK(void)
{
	BattleFx_RunPageEffectForSlot(0x1F, 2, 4);
}

static s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

static s32 SceneActor_UpdatePlayerProximity(
	struct SceneActor *actor, struct SceneActor *target, s32 range, s32 force)
{
	s32	 result = 0;
	s32 *targetPos = &target->x;
	s32 *actorPos = &actor->x;

	if (FixedPoint_Distance(targetPos, actorPos) < range || force != 0)
	{
		u32 angle = (u16)ArcTan2(target->z - actor->z, *targetPos - *actorPos);
		u32 farLeft = (angle - 0x2000) & 0xf000;
		u32 farRight = (angle + 0x2000) & 0xf000;
		u32 left = (angle - 0x1000) & 0xf000;
		u32 right = (angle + 0x1000) & 0xf000;
		u32 forward = angle & 0xf000;
		u32 facing = actor->facing & 0xf000;

		if (forward == facing || right == facing || left == facing || force != 0)
		{
			actor->active = 1;
			Object_SetMode(actor, 1);
			result = 1;
		}
		if (target == Object_GetById(0) && (farRight == facing || farLeft == facing))
		{
			actor->active = 1;
			Object_SetMode(actor, 1);
			result = 1;
		}
	}
	else
	{
		actor->active = 0;
		Object_SetMode(actor, 2);
	}
	return result;
}

static s32 SceneActor_UpdatePartnerProximity(u8 *self)
{
	u8					  **globals = (u8 **)gWindowWork;
	struct TorebiSceneWork *scene = (struct TorebiSceneWork *)globals[0];
	u8					   *work = globals[12];
	u16					   *flags = (u16 *)(self + 100);
	s32						force = 0;
	s32						range = 18;
	u8					   *partner;
	u8					   *player;

	if ((*flags & 1) != 0)
	{
		partner = Object_GetById(17);
	}
	else
	{
		partner = Object_GetById(16);
	}
	if (SceneActor_UpdatePlayerProximity(self, partner, 32, 0) != 0)
	{
		return 0;
	}

	player = Object_GetById(0);

	if (*(s16 *)(work + 376) != 0 || scene->widen_proximity != 0)
	{
		range = 26;
		if ((*flags & 2) != 0)
		{
			force = 1;
		}
	}

	SceneActor_UpdatePlayerProximity(self, player, range, force);
	return 0;
}

u8 *TorebiMachi_GetEntrances(void)
{
	return (u8 *)gTorebiMachiEntrances;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *TorebiMachi_GetExits(void)
{
	return (u8 *)gTorebiMachiExits;
}

u8 *TorebiMachi_GetPlacements(void)
{
	return (u8 *)gTorebiMachiPlacements;
}

static void EV08_PSYNERGY(void)
{

	struct FieldActor *record;

	record = Object_GetById(8);
	if ((s32)record != 0)
	{
		record->collision_flags = 0;
	}
	record = Object_GetById(8);
	Engine_ActorSetSpriteFlags((s32)record, 0);
	Field_SetMapCellCollision(0, 0x2200000, 0x1200000, 253);
	Engine_GameFlagSet(0x200);
}

static void ConfigureAndPlaceActorOneHundredTwo(void)
{
	s32 a = 3, b = 26;
	Engine_MapCopyCellAttributes(3, 32, 1, 1, a, b);
	MapObject_SetPosition(102, 0x00380000, 0x01a80000);
}

static void HideActorOneHundredTwo(void)
{
	s32 a = 3, b = 26;
	Engine_MapCopyCellAttributes(2, 25, 1, 1, a, b);
	MapObject_SetPosition(102, -1, -1);
}

static void EV30_SEARCH(void)
{
	Engine_EventSetMessage((s32)MsgTorebiFaceAwayTolbi);
	Engine_EventShowMessage(-1, 0);
}

static void EV31_SEARCH(void)
{
	Engine_EventSetMessage((s32)MsgTorebiTossLuckyMedal);
	Engine_EventShowMessage(-1, 0);
}

static void EV16_SCRIPT(void)
{
	struct EventWork  *work = gEventWork;
	struct SceneActor *actor;
	s16				   facing;
	s32				   msg;

	actor = Object_GetById(16);
	facing = actor->facing;
	Engine_EventBegin();
	actor->proximity_flags |= 2;
	if (work->psynergy_request == 0)
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			msg = (s32)MsgTorebiYayEasyRun;
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			msg = (s32)MsgTorebiHoorayFinalsSeven;
		}
		else
		{
			msg = (s32)MsgTorebiWheeFestivalColosso;
		}
	}
	else
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			msg = (s32)MsgTorebiMainStreetTolbi;
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			msg = (s32)MsgTorebiWinBigUsed;
		}
		else
		{
			msg = (s32)MsgTorebiOldestShouldntShare;
		}
	}
	Engine_EventSetMessage(msg);
	Engine_ActorSetAnimation(16, 0);
	Engine_ActorFaceEachOther(16, 0, 2);
	Engine_EventShowMessageAndWait(16, 0, 10);
	actor->facing = facing;
	Engine_TaskWait(1);
	actor->proximity_flags &= 1;
	Engine_EventEnd();
}

static void EV17_SCRIPT(void)
{
	struct EventWork  *work = gEventWork;
	struct SceneActor *actor;
	s16				   facing;
	s32				   msg;

	actor = Object_GetById(17);
	facing = actor->facing;
	Engine_EventBegin();
	actor->proximity_flags |= 2;
	if (work->psynergy_request == 0)
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			msg = (s32)MsgTorebiHaHaHa;
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			msg = (s32)MsgTorebiWaahBigBrother;
		}
		else
		{
			msg = (s32)MsgTorebiWaahSaidMoney;
		}
	}
	else
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			msg = (s32)MsgTorebiBrotherMeanDuring;
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			msg = (s32)MsgTorebiMamaWhyListen;
		}
		else
		{
			msg = (s32)MsgTorebiMamaToldShare;
		}
	}
	Engine_EventSetMessage(msg);
	Engine_ActorSetAnimation(17, 0);
	Engine_ActorFaceEachOther(17, 0, 2);
	Engine_EventShowMessageAndWait(17, 0, 10);
	actor->facing = facing;
	Engine_TaskWait(1);
	actor->proximity_flags &= 1;
	Engine_EventEnd();
}

u8 *TorebiMachi_SelectEvents(void)
{
	if (Engine_GameFlagIsSet(0x950) != 0)
	{
		return (u8 *)gTorebiMachiEvents3;
	}
	if (Engine_GameFlagIsSet(0x962) != 0)
	{
		return (u8 *)gTorebiMachiEvents2;
	}
	return (u8 *)gTorebiMachiEvents;
}

static void EV15_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiSomeoneLiveMore);
	Engine_EventAskYesNo(15, 0);
	Engine_EventEnd();
}

static void EV24_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiChildrenLoveSouvenirs);
	Engine_EventAskYesNo(24, 0);
	Engine_EventEnd();
}

static void EV25_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiPatientLittleGuy);
	Actor_FaceDirection(25, 0xc000, 0);
	Engine_EventShowMessage(25, 0);
	Actor_FaceDirection(25, 0x8000, 0);
	Engine_EventShowMessage(25, 0);
	Engine_EventEnd();
}

static void EV26_TALK(void)
{
	Engine_EventBegin();
	Actor_FaceDirection(26, 0x4000, 0);
	Engine_ActorStartRepeatedMotion(26, 2);
	Engine_EventSetMessage((s32)MsgTorebiWaahBuySweets);
	Engine_EventShowMessage(26, 0);
	Engine_EventEnd();
}

static void EV27_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiInnsFullStaying);
	Engine_EventShowMessage(0x1B, 0);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiFinalsWerent);
	Engine_EventAskYesNo(24, 0);
	Engine_EventEnd();
}

static void EV19_TALK(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x8bf) == 0)
	{
		Engine_GameFlagSet(0x8bf);
		Engine_EventSetMessage((s32)MsgTorebiLeftovers);
		Engine_EventShowMessage(19, 0);
		Engine_ItemShowFound(233, 3);
		Engine_EventShowMessage(19, 0);
		Engine_ActorSetAnimation(0, 1);
		Engine_PartyGiveItem(233, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgTorebiFestivalLongerUsual);
		Engine_EventShowMessage(19, 0);
	}
	Engine_EventEnd();
}

static void SceneScript_SetupActors(void)
{

	struct EventWork *work = gEventWork;
	u32				  no;
	s32				  index;

	Engine_EventBegin();
	for (no = 8; no <= 65; no++)
	{
		u8 *actor = Object_GetById(no);
		if (actor != NULL)
		{
			actor[85] = 0;
		}
	}
	index = work->touched_trigger - 1;
	Engine_AudioPlayCue(158);
	Map_AnimateCells((s32)gTorebiMachiCellSteps[index].steps, gTorebiMachiCellSteps[index].x,
		gTorebiMachiCellSteps[index].y);
	Actor_SetSpeed(0, 0x8000, 0x4000);
	Object_GetById(0)->motion_flags = 0;
	Engine_ActorSetAnimation(0, 2);
	if (index != 6)
	{
		Actor_CenterAndWalk(0, 2, -8);
		Engine_EventWait(10);
	}
	Engine_EventRequestExit(work->touched_trigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void SceneActor_CopyPlayerModeToActor(union FieldObject *object)
{
	s32 priority;

	if (object != NULL)
	{
		priority = Object_GetById(0)->sprite->priority;
		object->actor.priority_flags = 0;
		object->actor.sprite->priority = priority;
		((struct SceneSprite *)object->actor.sprite)->priority_15 = priority;
	}
}

s32 TorebiMachi_ApplyEntryState(s32 a0)
{
	s32 record;
	s32 handler;
	s32 hidden;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Actor_SetPosition(16, 0x1600000, 0x1600000);
	Engine_ActorEnableActionCallback(16, gTorebiMachiActor16Action);
	record = Object_GetById(16);
	handler = (s32)SceneActor_UpdatePartnerProximity;
	((struct SceneActor *)record)->proximity_flags = 1;
	*(s32 *)(record + 108) = handler;
	hidden = 0;
	Engine_ActorSetPosition(17, 0x1700000, 0x1400000);
	Engine_ActorEnableActionCallback(17, gTorebiMachiActor17Action);
	record = Object_GetById(17);
	((struct SceneActor *)record)->proximity_flags = hidden;
	*(s32 *)(record + 108) = handler;
	record = Object_GetById(14);
	*(s32 *)(record + 108) = (s32)SceneActor_CopyPlayerModeToActor;
	if (Engine_GameFlagIsSet(0x8c1) != 0)
	{
		Actor_SetPosition(28, 0x13c0000, 0x1480000);
	}
	if (Engine_GameFlagIsSet(0x201) != 0)
	{
		EV09_PSYNERGY();
	}
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		EV08_PSYNERGY();
		Engine_ActorSetAnimation(8, 4);
	}
	if (Engine_GameFlagIsSet(0x950) != 0)
	{
		Actor_SetPosition(20, 0x2080000, 0x2300000);
		Actor_SetPosition(21, 0x2080000, 0x2300000);
		Actor_SetPosition(22, 0x2080000, 0x2300000);
		Actor_SetPosition(24, 0x2080000, 0x2300000);
		Actor_SetPosition(25, 0x2080000, 0x2300000);
		Actor_SetPosition(26, 0x2080000, 0x2300000);
		Actor_SetPosition(27, 0x2080000, 0x2300000);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x962) != 0)
		{
			Actor_SetPosition(27, 0x1180000, 0x500000);
			Actor_FaceDirection(27, 0x2000, 0);
			Engine_ActorSetAnimation(27, 1);
		}
	}
	return 0;
}

static void EV28_TALK(void)
{
	Engine_EventBegin();
	Actor_WalkToAndWait(0, 0x130, 0x138);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Engine_ActorFaceDirection(28, 0x4000, 0);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgTorebiWantTestLuck);
	Event_OpenMessage(28, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		gEventWork->message++;
		Engine_EventShowMessage(28, 0);
		Actor_SetSpeed(28, 0x10000, 0x8000);
		Actor_WalkToAndWait(28, 0x140, 0x130);
		Actor_WalkToAndWait(28, 0x13c, 0x148);
		Engine_ActorFaceDirection(28, 0xa000, 0);
		Engine_GameFlagSet(0x8c1);
	}
	else
	{
		Engine_EventShowMessage(28, 0);
	}
	Engine_EventEnd();
}

static void EV239_SCRIPT(void)
{
	Engine_EventRequestExit(30);
	Engine_EventEnd();
}

static void TorebiMachi_RequestTouchedExit(void)
{

	s16 *cnt = &gEventWork->touched_trigger;

	Engine_EventRequestExit(*cnt);
}

static void EV20_TOUCH(void)
{
	s32 base;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Actor_SetSpeed(29, 0x10000, 0x8000);
	Engine_ActorSetSpeed(30, 0x10000, 0x8000);
	base = (s32)MsgTorebiSeenAnyoneWho;
	Engine_EventSetMessage(base);
	Actor_SetPosition(29, 0x480000, 0xd00000);
	Actor_SetPosition(30, 0x380000, 0xd00000);
	Engine_ActorSetChildValue(32, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(32), 0);
	Actor_SetPosition(32, 0x5f0000, 0x280000);
	Engine_ActorWalkTo(29, 72, 248);
	Engine_ActorWalkTo(30, 56, 248);
	Actor_WalkToAndWait(0, 64, 0x108);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Engine_ActorWaitForMove(29);
	Engine_ActorSetAnimation(29, 1);
	Engine_ActorSetAnimation(30, 1);
	Engine_ActorSetAnimation(0, 1);
	Engine_ActorFaceActor(29, 0, 0);
	Engine_ActorFaceActor(30, 0, 0);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(29, 0x102);
	Actor_SetAttachedEffect(30, 0x102);
	Engine_ActorStartRepeatedMotion(29, 2);
	Engine_ActorRunRepeatedMotion(30, 2);
	Engine_EventWait(20);
	Event_OpenMessage(29, 0);
	Engine_EventWait(25);
	UiWindow_CreateWithSideObject(52, 0, 12, 7);
	UiText_OpenMessageWindow((base + 3), 11, 12, 2);
	SCENE_OBJECT_ID = 32;
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(30, 2);
		Engine_EventWait(30);
		Engine_ActorFaceDirection(30, 0, 0);
		Engine_EventWait(30);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(29, 3);
		Engine_EventWait(20);
		Engine_ActorFaceDirection(29, 0, 0);
		Engine_EventWait(30);
		Engine_EventShowMessage(29, 0);
		Engine_EventWait(20);
		Actor_FaceDirection(29, 0x4000, 0);
		Engine_ActorFaceDirection(30, 0x4000, 0);
		Engine_EventWait(30);
		Engine_ActorSetAnimation(29, 3);
		Engine_ActorSetAnimationAndWait(30, 3);
		Engine_EventWait(20);
		Actor_SetSpeed(29, 0x1cccc, 0xe666);
		Actor_SetSpeed(30, 0x1cccc, 0xe666);
		Actor_WalkTo(29, 232, 248);
		Engine_EventWait(2);
		Actor_WalkTo(30, 232, 248);
		Engine_ActorWaitForMove(29);
		Engine_ActorWalkTo(29, 248, 248);
		Engine_ActorWalkToAndWait(30, 248, 248);
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(30, 2);
		Engine_EventWait(30);
		Engine_ActorFaceDirection(30, 0, 0);
		Engine_EventWait(30);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(29, 4);
		Engine_EventWait(20);
		Engine_ActorFaceDirection(29, 0, 0);
		Engine_EventWait(30);
		gEventWork->message += 1;
		Engine_EventShowMessage(29, 0);
		Engine_EventWait(20);
		Actor_FaceDirection(29, 0x4000, 0);
		Actor_FaceDirection(30, 0x4000, 0);
		Engine_EventWait(30);
		Engine_ActorSetAnimation(29, 3);
		Engine_ActorSetAnimationAndWait(30, 3);
		Engine_EventWait(20);
		Actor_SetSpeed(29, 0x19999, 0xcccc);
		Actor_SetSpeed(30, 0x19999, 0xcccc);
		Engine_ActorWalkTo(29, 72, 184);
		Engine_ActorWalkToAndWait(30, 56, 184);
	}
	Engine_ActorSetPosition(29, 0, 0);
	Engine_ActorSetPosition(30, 0, 0);
	Engine_ActorSetPosition(32, 0, 0);
	Engine_GameFlagSet(0x8c0);
	Engine_EventEnd();
}

static void EV09_PSYNERGY(void)
{
	struct Actor *actor = Object_GetById(9);
	if (actor != 0)
	{
		Engine_ActorSetSpriteFlags(actor, 0);
		actor->field23 = 2;
		actor->field55 = 0;
	}
	Engine_ActorSetAnimation(9, 5);
	{
		s32 v5 = 34;
		s32 v6 = 16;
		Engine_MapCopyCellAttributes(36, 16, 1, 1, v5, v6);
	}
	Engine_GameFlagSet(0x201);
}

s32 gTorebiMachiPlacement2[] = {
	SC_SET(SW_SCALE_X, -0x10000),
	SC_ARG(7),
	SC_END,
};

s32 gTorebiMachiActor16Action[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(352), 0, PIXELS(352)),
	SC_SYNC,
	SC_MOVE(PIXELS(368), 0, PIXELS(352)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(344)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(328)),
	SC_SYNC,
	SC_MOVE(PIXELS(368), 0, PIXELS(320)),
	SC_SYNC,
	SC_MOVE(PIXELS(352), 0, PIXELS(320)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(328)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(344)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gTorebiMachiActor17Action[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(368), 0, PIXELS(320)),
	SC_SYNC,
	SC_MOVE(PIXELS(352), 0, PIXELS(320)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(328)),
	SC_SYNC,
	SC_MOVE(PIXELS(344), 0, PIXELS(344)),
	SC_SYNC,
	SC_MOVE(PIXELS(352), 0, PIXELS(352)),
	SC_SYNC,
	SC_MOVE(PIXELS(368), 0, PIXELS(352)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(344)),
	SC_SYNC,
	SC_MOVE(PIXELS(376), 0, PIXELS(328)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gTorebiMachiEntrances[] = {
	{ 0, CONDITION_ALWAYS, 296, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 304, 0, 448, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 536, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 536, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 456, 0, 376, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 368, 0, 416, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 200, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 88, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 64, 0, 112, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 304, 0, 64, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 424, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 20, CONDITION_ALWAYS, 304, 0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 30, CONDITION_ALWAYS, 568, 0, 296, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gTorebiMachiExits[] = {
	SCENE_EXITS(135),
	SCENE_EXIT(1, 136, 4),
	SCENE_EXIT(2, 136, 2),
	SCENE_EXIT(3, 136, 9),
	SCENE_EXIT(4, 137, 11),
	SCENE_EXIT(5, 136, 1),
	SCENE_EXIT(6, 136, 3),
	SCENE_EXIT(7, 136, 6),
	SCENE_EXIT(8, 136, 5),
	SCENE_EXIT(9, 136, 8),
	SCENE_EXIT(10, 139, 1),
	SCENE_EXIT(11, 142, 1),
	SCENE_EXIT(12, 2, 23),
	SCENE_EXIT(13, 137, 10),
	SCENE_EXIT(30, 189, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gTorebiMachiPlacements[] = {
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(552), 0, PIXELS(288), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(552), 0, PIXELS(264), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(264), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(208), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(224), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(152), 0, PIXELS(304), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(24), 0, PIXELS(456), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, (s32)gTorebiMachiActor16Action,
		PIXELS(344), 0, PIXELS(328), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, (s32)gTorebiMachiActor17Action,
		PIXELS(376), 0, PIXELS(328), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x82, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(400), 0, PIXELS(432), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x82, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(184), 0, PIXELS(472), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(256), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(184), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(272), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, (s32)gTorebiMachiPlacement2,
		PIXELS(312), 0, PIXELS(120), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(302), 0, PIXELS(296), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x5c, 0x5c, ACTOR_STAND,
		PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x34, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(-9), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gTorebiMachiEvents[] = {
	{ PSYNERGY_ON_ACTOR(0xc), 8, 0x200, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ 0xc402, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ EVENT_TOUCH, 20, 0x8c0, EVENT_SCRIPT(EV20_TOUCH) },
	{ 0xc402, 239, IF_FLAG_SET(0x8c1), EVENT_SCRIPT(EV239_SCRIPT) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTolbisAlwaysReallyBustlingDuringColosso) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiColossoIsHeldInTheColosseum) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTryoutsStartedAtTheColosseumToday) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiColossoShouldHaveStartedTodayWhat) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiBabiWasAlreadyPrettyOldBack) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiGetYourColossoSouvenirsTheyMake) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiGetYourSnacksAndDrinksBefore) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIveDecidedNotToFightAt) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWowTolbisFestivalIsHumongousJust) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiRatsWeOversleptAndNowThe) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_TALK) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_TALK) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_TALK) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TALK) },
	{ EVENT_TALK, 28, 0x8c1, EVENT_SCRIPT(EV28_TALK) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiAhIGuessIJustDont) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTouristsArentTheOnlyOnesWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheTrialsAreStartingAtThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheTrialsAreOkayButThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiItsNotLikeThereHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiEveryYearIGetOlderAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWhatIfBabiIsActuallyA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWeHaveToSellAllOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheyreSellingTheseAtTheColosseum) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiISawThePeopleWhoAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTolbiIsNotLikeXianIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiICantSayAnythingCauseIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheStuffTheyreSellingAtThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWhyDoKidsWantSuchAwful) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiThoseSweetsSmellGreatImNot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIveBeenGuardingOurSpotBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIKeepLosingCoinsButI) },
	{ EVENT_TALK, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_SEARCH) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf97, FIND_COINS(9) },
#if !EDITION_INTERNATIONAL
	{ SEARCH(SEARCH_JAR), 101, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
#else
	{ 0xc423, 101, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
#endif
	{ SEARCH(SEARCH_CHEST), 102, 0xf99, FIND_ITEM(0xbf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorOneHundredTwo) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(HideActorOneHundredTwo) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTorebiMachiEvents2[] = {
	{ PSYNERGY_ON_ACTOR(0xc), 8, 0x200, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ 0xc402, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 239, IF_FLAG_SET(0x8c1), EVENT_SCRIPT(EV239_SCRIPT) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiColossoWillReachItsApexAny) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiAreYouPlanningToWatchThe) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiMyFamilySavedMeASeat) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIThoughtSomethingHappenedToBabi) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheColosseumWasSoFullThat) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiNobodyElseHasNoticedButThis) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiHowAboutSomeColossoSouvenirsThis) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheFinalsAreSoLongIts) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheTrialsWereSoRivetingThe) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIHaveSeatsToTheFinals) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIOversleptAgainSoICant) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_TALK) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_TALK) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_TALK) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheFinalsStartTodayIWonder) },
	{ EVENT_TALK, 28, 0x8c1, EVENT_SCRIPT(EV28_TALK) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiAhIGuessIJustDont) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIveGotToGetToThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiABattleToTheDeathWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheFinalsStartOnBabisSignal) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiSoWhyWereTheSoldiersAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiICouldStillBuySeatsFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiNoMatterHowIAddIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWeCantSellAnythingOnceThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiThisIsTheFinalPushAnything) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiFightingInColossoRequiresALot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheyreSellingFinalsSeatsRightAt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiItsSupposedToBeFreeHaving) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiOnceColossoEndsOurLivesCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiFestivalsAreSoExpensiveMyWallets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiMomYouSaidYouWereAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTodayIsABigDayFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIKeepLosingCoinsButI) },
	{ EVENT_TALK, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_SEARCH) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf97, FIND_COINS(9) },
	{ SEARCH(SEARCH_JAR), 101, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 102, 0xf99, FIND_ITEM(0xbf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorOneHundredTwo) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(HideActorOneHundredTwo) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTorebiMachiEvents3[] = {
	{ PSYNERGY_ON_ACTOR(0xc), 8, 0x200, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneScript_SetupActors) },
	{ 0xc402, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TorebiMachi_RequestTouchedExit) },
	{ 0xc402, 239, IF_FLAG_SET(0x8c1), EVENT_SCRIPT(EV239_SCRIPT) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWellThisYearsColossoHasEnded) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiItsBackToOurNormalQuiet) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiWithAllThoseRevelersGoneThe) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIHadNoLuckInThe) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiItsSoQuietTheHustleAnd) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIllBeBackToDoBusiness) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 28, 0x8c1, EVENT_SCRIPT(EV28_TALK) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiAhIGuessIJustDont) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheSpectatorsHaveAllLeftSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiThinkHowFunItWouldBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiAllTheWarriorsWhoGainFame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiComeToThinkOfItNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiHmmSinceColossoEndedSoDramatically) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiTheCalmAfterTheStormIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiEveryYearITryToSell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiHowCanWeGetHomeIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiMachiIKeepLosingCoinsButI) },
	{ EVENT_TALK, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TALK) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_SEARCH) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf97, FIND_COINS(9) },
	{ SEARCH(SEARCH_JAR), 101, 0xf98, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 102, 0xf99, FIND_ITEM(0xbf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorOneHundredTwo) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(HideActorOneHundredTwo) },
	{ SCENE_EVENTS_END },
};

u16 gTorebiMachiCellStepData[] = {
	MAP_CELL_STEP(39, 32, 1, 2, 6), MAP_CELL_STEP(40, 32, 1, 2, 6), MAP_CELL_STEPS_END,
};

struct MapCellAnim gTorebiMachiCellSteps[] = {
	{ gTorebiMachiCellStepData, 48, 6 },
	{ gTorebiMachiCellStepData, 63, 4 },
	{ gTorebiMachiCellStepData, 71, 8 },
	{ gTorebiMachiCellStepData, 71, 4 },
	{ gTorebiMachiCellStepData, 66, 21 },
	{ gTorebiMachiCellStepData, 61, 24 },
	{ gTorebiMachiCellStepData, 50, 26 },
	{ gTorebiMachiCellStepData, 50, 22 },
	{ gTorebiMachiCellStepData, 43, 20 },
};
