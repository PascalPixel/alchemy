#include "game_state.h"
#include "unit/import.h"
#include "unit/field/haidia_sukureta/sukureta.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
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
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
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
#include "sukureta.h"
#include "call.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"

static s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 delta;
	u16					 old;
	s32					 angle;
	struct FacingObject *target;

	target = obj->facing_target;
	if (target != 0)
	{
		obj->facing_flags = (0xFE & obj->facing_flags);
		angle = (u16)ArcTan2(
			target->position_z - obj->position_z, target->position_x - obj->position_x);
		old = obj->facing;
		delta = (s16)(angle - old);
		if (delta != 0)
		{
			if (delta > 0x1000)
			{
				delta = 0x1000;
			}

			if (delta < -0x1000)
			{
				delta = -0x1000;
			}
			obj->facing = (old + delta);
		}
	}
	return 1;
}

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

u8 *SceneData_GetActorTable(void)
{
	return (u8 *)Placement_Actors;
}

u8 *SceneData_SelectEffectTable(void)
{
	if (Engine_GameFlagIsSet(0x87A) != 0)
	{
		return (u8 *)Placement_Effects87a;
	}
	if (Engine_GameFlagIsSet(0x834) != 0)
	{
		return (u8 *)Placement_Effects834;
	}
	return (u8 *)Placement_Effects;
}

static void EV12_TALK(void)
{
	s32 base;

	Engine_EventBegin();
	if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0)
	{
		Engine_EventSetMessage((s32)MsgHaidiaIllClimbTheFenceSomeday);
		Event_ShowMessage(12, 0);
	}
	else
	{
		base = (s32)MsgHaidiaHearAwfulGrowls;
		Engine_EventSetMessage(base);
		Actor_FaceActor(12, ACTOR_PARTY_LEADER, 10);
		Engine_ActorRunRepeatedMotion(12, 2);
		Engine_EventWait(6);
		Event_OpenMessage(12, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventSetMessage(base + 1);
		}
		else
		{
			Engine_EventSetMessage(base + 2);
		}
		Engine_ActorStartRepeatedMotion(12, 3);
		Event_ShowMessage(12, 0);
		Actor_FaceDirection(12, 49152, 10);
	}
	Engine_EventEnd();
}

static void EV13_TALK(void)
{
	struct FieldActor *record;
	s32				   x, y;
	s32				   sneak;

	if (GameFlag_IsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0)
	{
		Engine_EventBegin();
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
		Engine_EventSetMessage((s32)MsgHaidiaSukuretaOhRobin);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 0x108);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SUKURETA, 20);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		record = Object_GetById(0);
		x = record->x.part.pixel;
		y = record->z.part.pixel;
		Actor_SetPosition(JASMINE, x << 16, y << 16);
		Actor_SetPosition(GERALD, x << 16, y << 16);
		Actor_SetSpeed(JASMINE, 0x8000, 0x4000);
		Actor_SetSpeed(GERALD, 0x8000, 0x4000);
		Actor_WalkTo(JASMINE, 248, 0x108);
		Actor_WalkToAndWait(GERALD, 216, 0x108);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
		Engine_ActorSetAnimation(JASMINE, 1);
		Engine_ActorSetAnimation(GERALD, 1);
		Engine_EventWait(4);
		Actor_FaceDirection(JASMINE, 0xb000, 0);
		Actor_FaceDirection(GERALD, 0xd000, 20);
		Engine_ActorSetAnimationAndWait(JASMINE, 4);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(JASMINE, 0, 20);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
		Engine_EventWait(10);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		Actor_FaceDirection(GERALD, 0x3000, 40);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Event_ShowMessageAndWait(GERALD, 0, 10);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
		Engine_ActorSetAnimation(ACTOR_SUKURETA, 3);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 8);
		Actor_FaceDirection(JASMINE, 0x3000, 20);
		Event_ShowMessageAndWait(JASMINE, 0, 10);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
		Actor_ShowEmote(GERALD, 0x101, 0);
		Actor_ShowEmote(JASMINE, 0x101, 60);
		Actor_FaceDirection(GERALD, 0xd000, 0);
		Actor_FaceDirection(JASMINE, 0xb000, 20);
		Engine_ActorRunRepeatedMotion(JASMINE, 2);
		Event_ShowMessageAndWait(JASMINE, 0, 10);
		Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		Actor_FaceDirection(GERALD, FACING_EAST, 0);
		Actor_FaceDirection(JASMINE, FACING_WEST, 20);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 60);
		Actor_FaceDirection(GERALD, 0xd000, 0);
		Actor_FaceDirection(JASMINE, 0xb000, 0);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
		Event_OpenMessage(ACTOR_SUKURETA, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventSetMessage((s32)MsgHaidiaYouCannotEnterMtAleph);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgHaidiaSukuretaIWaitedYearsFor);
		}
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		Engine_EventSetMessage((s32)MsgHaidiaJasmineTheyMightBeThieves);
		Engine_ActorRunRepeatedMotion(JASMINE, 2);
		Actor_FaceDirection(JASMINE, FACING_WEST, 10);
		Event_ShowMessageAndWait(JASMINE, 0, 6);
		Actor_ShowEmote(GERALD, 0x103, 30);
		Engine_ActorJump(GERALD, 4, 30);
		Actor_FaceDirection(GERALD, FACING_EAST, 10);
		Event_ShowMessageAndWait(GERALD, 0, 6);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 10);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 0);
		Actor_FaceActor(ACTOR_SUKURETA, GERALD, 10);
		Actor_FaceActor(ACTOR_SUKURETA, JASMINE, 10);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimation(GERALD, 3);
		Engine_ActorSetAnimationAndWait(JASMINE, 3);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(JASMINE, 1);
		Engine_ActorSetAnimation(GERALD, 1);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
		Actor_FaceDirection(JASMINE, FACING_SOUTH, 16);
		Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
		Actor_ShowEmote(GERALD, 0x100, 0);
		Actor_ShowEmote(JASMINE, 0x100, 40);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
		Actor_FaceDirection(JASMINE, 0xb000, 10);
		Event_ShowMessageAndWait(JASMINE, 0, 6);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
		Engine_EventWait(6);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
		Engine_ActorJump(GERALD, 2, 0);
		Engine_ActorJump(JASMINE, 2, 10);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(GERALD, 0xd000, 20);
		Event_ShowMessageAndWait(GERALD, 0, 6);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
		Engine_EventWait(16);
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 40);
		Event_ShowMessageAndWait(JASMINE, 0, 10);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
		Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(JASMINE, 0xb000, 30);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
		Actor_ShowEmote(GERALD, 0x105, 0);
		Actor_ShowEmote(JASMINE, 0x105, 80);
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
		Event_OpenMessage(ACTOR_SUKURETA, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventSetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckThe);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckMt);
		}
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
		sneak = (s32)MsgHaidiaFineIfTheyDontSee;
		Engine_EventSetMessage(sneak);
		Actor_FaceDirection(GERALD, FACING_EAST, 10);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Event_OpenMessage(GERALD, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventSetMessage((sneak + 1));
		}
		else
		{
			Engine_EventSetMessage((sneak + 2));
		}
		Event_ShowMessageAndWait(GERALD, 0, 6);
		Engine_EventSetMessage((s32)MsgHaidiaJasmineOurSecret);
		Actor_FaceDirection(JASMINE, FACING_WEST, 10);
		Engine_ActorRunRepeatedMotion(JASMINE, 1);
		Event_OpenMessage(JASMINE, 0);
		Engine_EventWait(4);
		if (Engine_EventChooseYesNo(0, 0) == 1)
		{
			Engine_ActorJump(JASMINE, 2, 20);
			Event_ShowMessageAndWait(JASMINE, 0, 10);
		}
		else
		{
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
			Engine_ActorSetAnimation(GERALD, 3);
			Engine_ActorSetAnimationAndWait(JASMINE, 3);
			Engine_EventWait(8);
			Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
			gEventWork->message++;
		}
		Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
		Engine_EventWait(10);
		Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(GERALD, 0xd000, 0);
		Actor_FaceDirection(JASMINE, 0xb000, 10);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimation(GERALD, 3);
		Engine_ActorSetAnimationAndWait(JASMINE, 3);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
		Event_OpenMessage(ACTOR_SUKURETA, 0);
		Engine_EventWait(4);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventSetMessage((s32)MsgHaidiaGeraldYouCanHandleThe);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgHaidiaGeraldIllTakeOverIf);
		}
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Actor_FaceDirection(GERALD, FACING_EAST, 10);
		Event_ShowMessageAndWait(GERALD, 0, 6);
		Actor_FaceDirection(JASMINE, FACING_WEST, 10);
		Engine_ActorSetAnimationAndWait(JASMINE, 4);
		Event_ShowMessageAndWait(JASMINE, 0, 6);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Actor_FaceDirection(GERALD, FACING_WEST, 10);
		Actor_ShowEmote(GERALD, 0x103, 30);
		Event_ShowMessageAndWait(GERALD, 0, 10);
		Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 40);
		Engine_ActorJump(ACTOR_SUKURETA, 4, 40);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(GERALD, FACING_NORTH, 0);
		Actor_FaceDirection(JASMINE, 0xb000, 20);
		Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
		Event_PlayCue(158);
		Map_AnimateCells(Sukureta_GateCells, 43, 8);
		Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
		Actor_WalkToAndWait(ACTOR_SUKURETA, 232, 218);
		Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
		Actor_ShowEmote(GERALD, 0x101, 0);
		Actor_ShowEmote(JASMINE, 0x101, 60);
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
		Engine_EventRequestExit(13);
		Engine_EventEnd();
	}
}

static void Scene_LeaveForMtAleph(void)
{
	struct FieldActor *record;
	s32				   facing;

	Engine_EventBegin();
	Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
	Actor_SetPosition(GERALD, 0xd80000, 0x1080000);
	Actor_SetPosition(JASMINE, 0xf80000, 0x1080000);
	record = Object_GetById(1);
	facing = 0xc000;
	record->facing = facing;
	record = Object_GetById(5);
	record->facing = facing;
	Map_AnimateCells(Sukureta_GateCells, 43, 8);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
	Actor_SetPosition(ACTOR_SUKURETA, 0xe60000, 0xdc0000);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 230, 232);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventSetMessage((s32)MsgHaidiaSukuretaOurBestBet);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
	Actor_FaceDirection(GERALD, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(JASMINE, FACING_WEST, 10);
	Engine_ActorSetAnimation(JASMINE, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_SetSpeed(JASMINE, 0xcccc, 0x6666);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
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
	Actor_SetPosition(ACTOR_SATUROS, 0, 0);
	Actor_SetPosition(ACTOR_MENARDI, 0, 0);
	GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	ColorBuffer_ApplySource(0x10000, 0);
	GameFlag_Set(0x242);
	Engine_EventEnd();
}

static void EV03_TOUCH(void)
{
	if (Engine_GameFlagIsSet(0x242) == 0)
	{
		Engine_AudioPlayCue(0x9E);
		Engine_MapAnimateCells(Sukureta_GateCells, 0x2B, 8);
	}
	Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0xE5, 0xD9);
	Engine_EventRequestExit(3);
}

static void EV07_TOUCH(void)
{
	struct FieldActor *record;
	s32				   x, z;
	s32				   evt;
	s32				   evt2;

	if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0)
	{
		return;
	}

	Engine_EventBegin();
	Event_PlayCue(17);
	GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

	evt = (s32)MsgHaidiaTheyKnowLittleOfThe;
	Engine_EventSetMessage(evt);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);

	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 30);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x148);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);

	record = Object_GetById(0);
	x = record->x.part.pixel;
	z = record->z.part.pixel;
	Actor_SetPosition(JASMINE, x << 16, z << 16);
	Actor_SetPosition(GERALD, x << 16, z << 16);

	Actor_SetSpeed(JASMINE, 0x13333, 0x9999);
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_WalkTo(JASMINE, 0x178, 0x148);
	Actor_WalkToAndWait(GERALD, 0x198, 0x148);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimation(JASMINE, 0);
	Engine_ActorSetAnimation(GERALD, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(JASMINE, FACING_EAST, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(JASMINE, 0x101, 30);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTH, 0);
	Camera_SetSpeed(0x60000, 0xc000);
	Camera_MoveTo(0xd70000, -1, 0x1590000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Event_PlayCue(61);

	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
	Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Actor_FaceDirection(ACTOR_MENARDI, FACING_SOUTH, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 6);
	Actor_ShowEmote(ACTOR_SATUROS, 0x100, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
	Actor_ShowEmote(ACTOR_MENARDI, 0x101, 40);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 60);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
	Actor_WalkToAndWait(ACTOR_SATUROS, 232, 0x168);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Engine_EventWait(10);
	Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
	Actor_WalkToAndWait(ACTOR_MENARDI, 216, 0x168);
	Actor_WalkTo(ACTOR_MENARDI, 0x178, 0x168);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTH, 0);
	Camera_MoveTo(0x1890000, -1, 0x1530000, 1);
	Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x168);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
	Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Actor_ShowEmote(GERALD, 258, 60);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
	Event_OpenMessage(0x100f, 0);

	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventSetMessage(evt + 10);
	}
	else
	{
		Engine_EventSetMessage(evt + 11);
	}

	Event_ShowMessageAndWait(0x100f, 0, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);

	evt2 = (s32)MsgHaidiaYoureTheOnesSneakingAround;
	Engine_EventSetMessage(evt2);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
	Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Event_ShowMessageAndWait(0x100f, 0, 10);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Event_ShowMessageAndWait(0x1005, 0, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST, 20);
	Event_OpenMessage(ACTOR_SATUROS, 0);

	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventSetMessage(evt2 + 5);
	}
	else
	{
		Engine_EventSetMessage(evt2 + 6);
	}

	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Actor_FaceActor(ACTOR_SATUROS, GERALD, 30);
	Actor_FaceActor(ACTOR_SATUROS, JASMINE, 30);
	Actor_ShowEmote(ACTOR_SATUROS, 0x105, 80);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
	Engine_EventSetMessage((s32)MsgHaidiaSaturosGo);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 6);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(JASMINE, 0x100, 0);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorStartRepeatedMotion(JASMINE, 1);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTH, 10);
	Event_ShowMessageAndWait(0x1005, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
	Event_ShowMessageAndWait(0x100f, 0, 10);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
	Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
	Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);

	record = Object_GetById(14);
	record->steering_flags &= ~ACTOR_FACES_MOTION;
	record = Object_GetById(15);
	record->steering_flags &= ~ACTOR_FACES_MOTION;

	Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x178);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x178);
	Engine_EventWait(6);

	record = Object_GetById(14);
	record->steering_flags |= ACTOR_FACES_MOTION;
	record = Object_GetById(15);
	record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;

	Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
	Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Event_PlayCue(17);

	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);

	Engine_ActorSetAnimation(JASMINE, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(JASMINE, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(JASMINE);
	Actor_SetPosition(JASMINE, 0, 0);
	FieldObject_SetTargetAndCallback(14, 0x10000, Sukureta_StrangerActions);
	FieldObject_SetTargetAndCallback(15, 0x10000, Sukureta_StrangerActions);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
}

static void EV05_TOUCH(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x800) != 0)
	{
	}
	else
	{
		Engine_EventBegin();
		Actor_SetSpeed(0, 0x10000, 0x8000);
		Engine_EventSetMessage((s32)MsgHaidiaWho);
		Engine_EventShowMessage(13, 0);
		record = Object_GetById(0);
		{
			s32 x = record->x.part.pixel;
			s32 y = record->z.part.pixel;

			Engine_ActorSetPosition(5, x << 16, y << 16);
			Engine_ActorSetPosition(1, x << 16, y << 16);
		}
		Actor_SetSpeed(5, 0xcccc, 0x6666);
		Actor_SetSpeed(1, 0xcccc, 0x6666);
		Actor_WalkTo(5, 0x128, 0x148);
		Actor_WalkToAndWait(1, 0x118, 0x148);
		Engine_ActorSetAnimation(0, 0);
		Engine_ActorSetAnimation(5, 0);
		Engine_ActorSetAnimation(1, 0);
		Actor_FaceDirection(5, 0xb000, 0);
		Actor_FaceDirection(1, 0xb000, 0);
		Actor_FaceDirection(0, 0xb000, 0);
		Engine_CameraMoveTo(0xe80000, -1, 0xf00000, 1);
		Engine_CameraWaitForMove();
		Actor_FaceDirection(13, FACING_WEST, 20);
		Engine_ActorRunRepeatedMotion(13, 2);
		Engine_EventShowMessage(13, 0);
		Actor_SetSpeed(13, 0x3333, 0x1999);
		Engine_ActorWalkToAndWait(13, 216, 232);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(13, 1);
		Engine_EventWait(10);
		Engine_EventShowMessageAndWait(13, 0, 40);
		Engine_ActorJump(13, 2, 10);
		Engine_ActorFaceDirection(13, FACING_EAST, 10);
		Engine_EventShowMessageAndWait(13, 0, 10);
		Engine_ActorWalkToAndWait(13, 248, 232);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(13, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(13, 0);
		Actor_ShowEmote(13, 0x101, 60);
		Engine_ActorRunRepeatedMotion(13, 1);
		Actor_FaceDirection(13, FACING_WEST, 20);
		Engine_EventShowMessageAndWait(13, 0, 2);
		Engine_ActorWalkToAndWait(13, 232, 232);
		Engine_EventWait(2);
		Actor_FaceDirection(13, FACING_SOUTH, 4);
		Engine_ActorRunRepeatedMotion(13, 2);
		Engine_EventWait(4);
		Engine_EventShowMessageAndWait(13, 0, 2);
		Engine_ActorSetAnimationAndWait(13, 4);
		Engine_EventWait(20);
		Engine_CameraFollowActor(0, 1);
		Engine_CameraWaitForMove();
		Engine_ActorSetAnimation(1, 2);
		record = Object_GetById(0);
		if (record != 0)
		{
			Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(1);
		Engine_ActorSetPosition(1, 0, 0);
		Engine_ActorSetAnimation(5, 2);
		record = Object_GetById(0);
		if (record != 0)
		{
			Engine_ActorSetDestination(5, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(5);
		Engine_ActorSetPosition(5, 0, 0);
		Engine_GameFlagSet(0x800);
		Engine_EventEnd();
	}
}

s32 HaidiaSukureta_RestoreEntryState(void)
{
	struct FieldActor *record;
	s32				   v5;

	if (gGameState.entrance == 5 || gGameState.entrance == 6)
	{
		Engine_GameFlagClear(0x12f);
	}
	if (GameFlag_IsSet(0x109) != 0)
	{
		Engine_GameFlagClear(0x242);
	}
	if (Engine_GameFlagIsSet(0x834) != 0)
	{
		Effect_SoundAndFlash();
		BattleFx_StartTwelveFrameBlend();
		Engine_EventBegin();
		Engine_ActorSetPosition(12, 0, 0);
		Engine_ActorSetPosition(13, 0, 0);
		Engine_ActorSetPosition(14, 0, 0);
		Engine_ActorSetPosition(15, 0, 0);
		Engine_ActorSetPosition(5, 0, 0);
		{
			struct FieldActor *record = Object_GetById(8);

			record->collision_flags = 8 | record->collision_flags;
		}
		Actor_SetPosition(11, 0x530000, 0x1090000);
		Engine_ActorWalkToAndWait(11, 83, 0x111);
		Engine_ActorSetAnimation(11, 5);
		record = Object_GetById(11);
		{
			s32 shown = 12;

			record->radius = shown;
		}
		Engine_ActorEnableActionCallback(11, Sukureta_Actor11Actions);
		if (Engine_GameFlagIsSet(0x839) != 0)
		{
			Engine_ActorSetPosition(11, 0, 0);
		}
		v5 = 21;
		Engine_EventEnd();
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 14, v5);
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 15, v5);
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 19);
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 19);
		v5 = 20;
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 20);
		Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 20);
		goto L_0200171e;
	}
	Engine_ActorSetPosition(9, 0, 0);
	Engine_ActorSetPosition(10, 0, 0);
	Engine_ActorSetPosition(1, 0, 0);
	Engine_ActorSetPosition(11, 0, 0);
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		if (gGameState.entrance == 10)
		{
			Scene_LeaveForMtAleph();
		}
	}
	if (Engine_GameFlagIsSet(0x801) != 0)
	{
		Engine_ActorSetPosition(13, 0, 0);
		Engine_ActorSetPosition(14, 0, 0);
		Engine_ActorSetPosition(15, 0, 0);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x808) != 0)
		{
			Actor_SetPosition(14, 0x1880000, 0x1780000);
			Actor_SetPosition(15, 0x1780000, 0x1780000);
			FieldObject_SetTargetAndCallback(14, 0x10000, Sukureta_StrangerActions);
			FieldObject_SetTargetAndCallback(15, 0x10000, Sukureta_StrangerActions);
		}
	}
	if (Engine_GameFlagIsSet(0x87a) != 0)
	{
		Actor_SetPosition(16, 0x840000, 0x1080000);
	}
	Engine_EventEnd();
L_0200171e:;
	return 0;
}

static void HaidiaSukureta_RunActorSequence(void)
{
	struct FieldActor *record;
	s32				   base;

	if (GameFlag_IsSet(0x839) == 0)
	{
		if (GameFlag_IsSet(0x82f) != 0)
		{
			Engine_EventBegin();
			Engine_ActorRunRepeatedMotion(11, 2);
			Engine_EventSetMessage((s32)MsgHaidiaSaveYourselves);
			Engine_EventShowMessage(11, 0);
			Engine_EventEnd();
		}
		else
		{
			Engine_EventBegin();
			Engine_ActorStop(11);
			Engine_ActorRunRepeatedMotion(11, 1);
			base = (s32)MsgHaidiaRockslideDestroyedFence;
			Engine_EventSetMessage(base);
			Engine_EventShowMessageAndWait(11, 0, 20);
			Actor_ShowEmote(0, 0x100, 30);
			Engine_CameraMoveTo(0x620000, -1, 0x11b0000, 1);
			Actor_WalkToAndWait(0, 94, 0x125);
			Actor_FaceDirection(0, FACING_NORTHWEST, 0);
			record = Object_GetById(0);
			if (record != 0)
			{
				Engine_ActorSetPosition(1, record->x.fixed, record->z.fixed);
			}
			Actor_WalkToAndWait(1, 110, 0x117);
			Actor_FaceDirection(1, FACING_NORTHWEST, 40);
			Engine_ActorRunRepeatedMotion(11, 2);
			Engine_EventWait(40);
			Engine_EventOpenMessage(11, 0);
			if (Engine_EventChooseYesNo(0, 0) == 0)
			{
				Engine_ActorRunRepeatedMotion(11, 2);
				Engine_EventWait(20);
				Engine_EventSetMessage((base + 2));
				Engine_EventShowMessage(11, 0);
				Engine_GameFlagSet(0x82f);
			}
			else
			{
				Engine_ActorRunRepeatedMotion(11, 2);
				Engine_EventWait(20);
				Engine_EventSetMessage((base + 3));
				Engine_EventShowMessageAndWait(11, 0, 40);
				Engine_ActorFaceActor(11, 0, 0);
				Engine_ActorSetAnimation(11, 1);
				Engine_ActorJump(11, 4, 40);
				Engine_ActorSetAnimation(11, 6);
				Actor_ShowEmote(11, 0x101, 40);
				Engine_EventShowMessageAndWait(11, 0, 10);
				Engine_ActorSetAnimation(11, 1);
				Engine_EventWait(10);
				Engine_ActorSetAnimationAndWait(11, 3);
				Engine_EventShowMessageAndWait(11, 0, 10);
				Engine_ActorSetAnimationAndWait(11, 3);
				FieldObject_SetTargetAndCallback(0, 0x1000b, Sukureta_StrangerActions);
				FieldObject_SetTargetAndCallback(1, 0x1000b, Sukureta_StrangerActions);
				Object_SetActionCallbackAndRefreshById(11, (s32)Sukureta_Actor11RockslideActions);
				Engine_ActorStop(0);
				Engine_ActorStop(1);
				Actor_FaceDirection(0, FACING_SOUTH, 0);
				Actor_FaceDirection(1, FACING_SOUTH, 60);
				Actor_ShowEmote(0, 0x105, 0);
				Engine_ActorShowEmote(1, 0x105, 120);
				Engine_GameFlagSet(0x839);
			}
			Engine_ActorSetAnimation(1, 2);
			record = Object_GetById(0);
			if (record != 0)
			{
				Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
			}
			Engine_ActorWaitForMove(1);
			Engine_ActorSetPosition(1, 0, 0);
			Engine_EventEnd();
		}
	}
}

static void EV10_TOUCH(void)
{
	struct EventWork *work;

	Engine_EventBegin();

	work = gEventWork;
	work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	work->transition_frames = 64;

	Engine_GameFlagSet(0x87D);
	BattleFx_SetWeightedResult(12, 0);
	Engine_GameFlagSet(0x900);
	Engine_EventEnd();
}

static void EV11_TOUCH(void)
{
	struct EventWork *work;

	Engine_EventBegin();

	work = gEventWork;
	work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	work->transition_frames = 64;

	Engine_GameFlagSet(0x87E);
	BattleFx_SetWeightedResult(12, 1);
	Engine_GameFlagSet(0x900);
	Engine_EventEnd();
}

static void EV16_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgHaidiaMemoriesOfThisCottage);
	Engine_EventAskYesNo(16, 0);
	Engine_EventEnd();
}

s32 Sukureta_Actor11RockslideActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(294)),
	SC_SYNC,
	SC_MOVE(PIXELS(77), 0, PIXELS(329)),
	SC_SYNC,
	SC_MOVE(PIXELS(70), 0, PIXELS(415)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 Sukureta_Actor11Actions[] = {
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 2048),
	SC_ADD(SW_SCALE_X, -1024),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 1024),
	SC_ADD(SW_SCALE_X, -512),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 1024),
	SC_ADD(SW_SCALE_X, -512),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Sukureta_StrangerActions[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 454, 0, 290, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 454, 0, 443, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 229, 0, 233, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 72, 0, 80, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 104, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 232, 0, 264, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(6),
	SCENE_EXIT(1, 4, 3),
	SCENE_EXIT(2, 4, 4),
	SCENE_EXIT(3, 8, 2),
	SCENE_EXIT(4, 93, 1),
	SCENE_EXIT(6, 93, 3),
	SCENE_EXIT(11, 3, 3),
	SCENE_EXIT(12, 3, 4),
	SCENE_EXIT(13, 8, 22),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(240), 0, PIXELS(353), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(384), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, (s32)Sukureta_Actor11Actions,
		PIXELS(83), 0, PIXELS(273), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(268), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 3, 0 },
	{ 0x21, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Placement_Effects[] = {
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaDoNotKeepSukuretaWaiting) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaLeaveBeforeWeChangeMinds) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaCuriousBeyondFence) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
	{ SEARCH(SEARCH_JAR), 100, 0xf41, FIND_ITEM(ITEM_ELIXIR) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Placement_Effects834[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 12 },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaSukureta_RunActorSequence) },
	{ EVENT_TOUCH, 20, 0x82f, EVENT_SCRIPT(HaidiaSukureta_RunActorSequence) },
	{ EVENT_TOUCH, 10, 0x87d, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 11, 0x87e, EVENT_SCRIPT(EV11_TOUCH) },
	{ SEARCH(SEARCH_JAR), 100, CONDITION_ALWAYS, 0 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Placement_Effects87a[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EV03_TOUCH) },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaWonderBeyondFence) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaAdventuresBeyondFence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaLonelyWithoutSukureta) },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	{ SEARCH(SEARCH_JAR), 100, 0xf41, FIND_ITEM(ITEM_ELIXIR) },
#endif
	{ SCENE_EVENTS_END },
};

u16 Sukureta_GateCells[] = {
	MAP_CELL_STEP(2, 36, 2, 2, 1), MAP_CELL_STEP(4, 36, 2, 2, 1), MAP_CELL_STEPS_END,
};
