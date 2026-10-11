#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/karei_machi/machi.h"
#include "unit/battle/effect/slots.h"
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
#include "types.h"
#include "edition.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "resource_3a8_effect.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

#define NULL ((void *)0)

enum StagedGroupMessage
{
	MSG_ROBIN_PEERED_INTO = 0x947,
	MSG_CAME_KALAY_BECAUSE_DIDNT_LIKE = 0x1a7c,
	MSG_WHY_WE_STOPPING_AT_PLACE = 0x1a92,
	MSG_THATS_WE_CANT_WAIT_ANY = 0x1ab2,
	MSG_LORD_HAMMET_SELLS_HIS_BEST = 0x1acf,
	MSG_DID_FIND_NEEDED_IN_WEAPON = 0x1ad1,
	MSG_LADY_LAYANA_SHARED_IN_LORD = 0x1ad5,
	MSG_LORD_HAMMETS_PALACE_LORD_AWAY = 0x1b05,
	MSG_WEVE_ARRIVED_HAMMET = 0x256f,
	MSG_DO_WANT_GO_CAVE_UP = 0x2584,
	MSG_LAYANA_WAS_VERY_HARD_ON = 0x25b3,
	MSG_VERY_CLEAN_MAINTAINED = 0x29df
};

struct Obj
{
	u8	filler00[6];
	u16 f06;
};

struct Obj_02000040
{
	u8	filler00[100];
	u16 f64;
	u16 f66;
};

struct Object
{
	u8	filler00[6];
	u16 x;
	u8	filler08[94];
	s16 cnt;
};

typedef struct Effect
{
	unsigned char pad00[0xC];
	s32			  y;
	unsigned char pad10[0x13];
	s8			  state23;
} Effect;

struct Obj_020036f8
{
	u8	filler00[8];
	s32 f08;
	s32 f0c;
	u8	filler10[8];
	s32 f18;
	s32 f1c;
	s32 f20;
	s32 f24;
	s32 f28;
	u8	filler2c[56];
	s16 f64;
};

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

extern u8 *gActorEffectWork;

struct SpriteBits
{
	u8 pad0[5];
	u8 b5_lo : 5;
	u8 hidden : 1;
	u8 b5_hi : 2;
	u8 pad6[3];
	u8 b9_lo : 2;
	u8 mode : 2;
	u8 b9_hi : 4;
};

union GameStateRows
{
	s16 halves[512][1];
};

static __inline__ void ClearStep(u8 *a)
{
	s32 z = 0;

	*(u16 *)(a + 100) = z;
}

void SceneState_ApplyValues12And2And3(void)
{
	BattleFx_RunPageEffectForSlot(0xC, 2, 3);
}

s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect)
{
	u32 action;
	u32 next_timer;

	if (effect->action_timer == 0)
	{
		action = (u32)(Engine_RandomNext() * 8) >> 0x10;
		switch (action)
		{
		case 0:
			Object_SetMode(effect, 3);
			break;
		case 1:
			Object_SetMode(effect, 4);
			break;
		case 3:
		case 4:
			effect->unknown_06 += (u32)(Engine_RandomNext() << 0xF) >> 0x10;
			break;
		}
		next_timer = (u32)(Engine_RandomNext() * 0x50) >> 0x10;
		effect->action_timer = next_timer;
		if (next_timer != 0)
		{
			goto decrement_timer;
		}
	}
	else
	{
	decrement_timer:
		--effect->action_timer;
	}
	return 1;
}

s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj)
{
	s32 cnt = *(u16 *)&obj->cnt;
	s32 loaded = obj->cnt;
	s32 amount;

	if (loaded == 0)
	{
		obj->x += (u32)(Engine_RandomNext() << 15) >> 16;
		amount = (u32)(Engine_RandomNext() * 80) >> 16;
		obj->cnt = amount;
		if (amount == 0)
		{
			goto done;
		}
		cnt = amount;
	}
	obj->cnt = cnt - 1;
done:
	return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KareiMachi1)
	{
		return gKareiMachiEntrances1;
	}
	if (scene == (s32)&SceneId_KareiMachi2)
	{
		return gKareiMachiEntrances2;
	}
	if (scene == (s32)&SceneId_KareiMachi3)
	{
		return gKareiMachiEntrances3;
	}
	if (scene == (s32)&SceneId_KareiMachi4)
	{
		return gKareiMachiEntrances4;
	}
	if (scene == (s32)&SceneId_KareiMachi5)
	{
		return gKareiMachiEntrances5;
	}
	if (scene == (s32)&SceneId_KareiMachi6)
	{
		return gKareiMachiEntrances6;
	}
	return gKareiMachiEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *KareiMachi_GetExits(void)
{
	return (u8 *)KareiMachi_SceneTable01;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KareiMachi1)
	{
		return gKareiMachiPlacements1;
	}
	if (scene == (s32)&SceneId_KareiMachi2)
	{
		return gKareiMachiPlacements2;
	}
	if (scene == (s32)&SceneId_KareiMachi3)
	{
		return gKareiMachiPlacements3;
	}
	if (scene == (s32)&SceneId_KareiMachi6)
	{
		return gKareiMachiPlacements6;
	}
	return gKareiMachiPlacementsOther;
}

void FieldScene_RunStepWithValue29df(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
	Engine_MessageShowCentered((s32)MsgKareiVeryCleanMaintained, 1);
	Engine_EventEnd();
}

void FieldScene_RunStepWithValueFd6(void)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(12, 0, 0);
	Engine_GameFlagSet(0xfd6);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KareiMachi1)
	{
		return gKareiMachiEvents1;
	}
	if (scene == (s32)&SceneId_KareiMachi2)
	{
		return gKareiMachiEvents2;
	}
	if (scene == (s32)&SceneId_KareiMachi3)
	{
		return gKareiMachiEvents3;
	}
	if (scene == (s32)&SceneId_KareiMachi4)
	{
		return gKareiMachiEvents4;
	}
	if (scene == (s32)&SceneId_KareiMachi5)
	{
		return gKareiMachiEvents5;
	}
	if (scene == (s32)&SceneId_KareiMachi6)
	{
		return gKareiMachiEvents6;
	}
	return gKareiMachiEventsOther;
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKareiCameKalayBecauseDidntLike);
	Engine_EventAskYesNo(19, 0);
	Engine_EventEnd();
}

void FieldScene_RunSlotZeroFacingSequence(void)
{
	struct FieldActor *leader;
	u32				   v;
	u16				  *message;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	v = (leader->facing + 0xfffff000) << 16;
	if (v > 0x60000000)
	{
		Engine_EventBegin();
		Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgKareiDoWantGoCaveUp);
		Engine_EventOpenMessage(8, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_ActorSetAnimationAndWait(8, 4);
			Engine_EventShowMessage(8, 0);
		}
		else
		{
			message = &gEventWork->message;
			*message = *message + 1;
			Engine_ActorSetAnimationAndWait(8, 3);
			Engine_EventShowMessage(8, 0);
		}
		Engine_EventEnd();
	}
}

void SceneDialogue_RunActorTenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKareiLayanaWasVeryHardOn);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

void SceneState_BranchOnSlotZeroFacing(void)
{
	struct Obj *o;
	u32			v;

	o = Object_GetById(ACTOR_PARTY_LEADER);
	v = (o->f06 - 0x2000) << 16;
	if (v > 0x80000000)
	{
		Engine_ShopOpen(22, 22);
	}
	else
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgKareiLordHammetSellsHisBest);
		Engine_EventShowMessage(22, 0);
		Engine_EventEnd();
	}
}

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void)
{
	struct Obj *o;
	u32			v;

	o = Object_GetById(ACTOR_PARTY_LEADER);
	v = (o->f06 - 0x6001) << 16;
	if (v <= 0x7ffe0000)
	{
		Engine_ShopOpen(23, 23);
	}
	else
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgKareiDidFindNeededInWeapon);
		Engine_EventAskYesNo(23, 0);
		Engine_EventEnd();
	}
}

void FieldScene_RunActorTwentyFourAngleDialogue(void)
{
	struct Obj *o;
	u32			v;

	o = Object_GetById(ACTOR_PARTY_LEADER);
	v = (o->f06 - 0x2000) << 16;
	if (v > 0xC0000000)
	{
		Engine_ShopOpen(24, 24);
	}
	else
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgKareiLadyLayanaSharedInLord);
		Engine_EventShowMessage(24, 0);
		Engine_EventEnd();
	}
}

void KareiMachi_RunExitDoor(void)
{
	struct EventWork  *event;
	struct FieldActor *actor;
	s16				  *trigger;
	u32				   i;
	s32				   door;

	event = gEventWork;
	Engine_EventBegin();
	for (i = 8; i <= 65; i++)
	{
		actor = Object_GetById(i);
		if (actor != NULL)
		{
			actor->motion_flags = 0;
		}
	}
	trigger = &event->touched_trigger;
	door = *trigger - 1;
	Engine_AudioPlayCue(158);
	{
		s32 x = KareiMachi_DoorCells[door].x;
		s32 y = KareiMachi_DoorCells[door].y;

		Engine_MapAnimateCells(KareiMachi_DoorCells[door].steps, x, y);
	}
	Engine_ActorSetSpeed(0, 0x8000, 0x4000);
	Object_GetById(0)->motion_flags = 0;
	Engine_ActorSetAnimation(0, 2);
	Actor_CenterAndWalk(0, 2, -8);
	Engine_EventWait(10);
	Engine_EventRequestExit(*trigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

void RunSceneArrivalSetup(void)
{
	s32 two = 2;

	Engine_EventBegin();
	Event_PlayCue(188);
	Map_CopyCellsTo(36, 23, 43, 12, two, two);
	Engine_TaskWait(5);
	Map_CopyCellsTo(39, 23, 43, 12, two, two);
	Engine_TaskWait(5);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Object_GetById(ACTOR_PARTY_LEADER)->motion_flags = 0;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_SetDestinationOffset(0, 0, -8);
	Engine_EventWait(10);
	Engine_EventRequestExit(2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

void FieldScene_RunStagedGroupSequence(void)
{
	s32				   message;
	struct FieldActor *head_wait;
	struct FieldActor *last_wait;
	struct FieldActor *closing;
	struct FieldActor *late;
	struct FieldActor *head_end;
	struct FieldActor *last_end;
	struct FieldActor *leader;
	struct FieldActor *last;
	struct FieldActor *head;
	struct FieldActor *head_back;
	struct FieldActor *head_again;
	struct FieldActor *head_home;
	struct EventWork  *work;

	work = gEventWork;
	Engine_EventBegin();
	if (GameFlag_IsSet(0x910) != 0 && GameFlag_IsSet(0x911) == 0)
	{
		Event_CallWithLastActiveObjectId((s32)KareiMachi_StagedGroupScript);
		Actor_SetPosition(20, PIXELS(252), PIXELS(272));
		Actor_SetPosition(27, PIXELS(284), PIXELS(264));
		Actor_SetPosition(28, PIXELS(284), PIXELS(280));
		Actor_SetPosition(29, PIXELS(300), PIXELS(264));
		Actor_SetPosition(30, PIXELS(300), PIXELS(280));
		Actor_SetPosition(32, PIXELS(316), PIXELS(264));
		Actor_SetPosition(31, PIXELS(316), PIXELS(280));
		Actor_SetPosition(33, PIXELS(332), PIXELS(264));
		Actor_SetPosition(34, PIXELS(332), PIXELS(280));
		Actor_SetPosition(21, PIXELS(364), PIXELS(272));
		Event_PlayCue(17);
		Ui_SetRenderResultFromObject(20);
		UiText_ShowCenteredMessage((s32)MsgKareiOkListeningLets, 1, 0);
		Event_PlayCue(9);
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		if (work->touched_trigger == 9)
		{
			Camera_SetSpeed(0x26666, 0x4ccc);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
		}
		else
		{
			Camera_SetSpeed(0x13333, 0x2666);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
		}
		Actor_SetSpeed(20, 0x11999, 0x8ccc);
		Actor_SetSpeed(27, 0x10000, 0x8000);
		Actor_SetSpeed(28, 0x10000, 0x8000);
		Actor_SetSpeed(29, 0xe666, 0x7333);
		Actor_SetSpeed(30, 0xe666, 0x7333);
		Actor_SetSpeed(32, 0xcccc, 0x6666);
		Actor_SetSpeed(31, 0xcccc, 0x6666);
		Actor_SetSpeed(33, 0xb333, 0x5999);
		Actor_SetSpeed(34, 0xb333, 0x5999);
		Actor_SetSpeed(21, 0x9999, 0x4ccc);
		Engine_ActorEnableActionCallback(20, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(27, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(28, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(29, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(30, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(32, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(31, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(33, (const u8 *)SceneAction_GroupMotion);
		Engine_ActorEnableActionCallback(34, (const u8 *)SceneAction_GroupMotion);
		last = Actor_Get(21);
		last->timer = 0;
		Engine_ActorEnableActionCallback(21, (const u8 *)SceneAction_GroupMotion);
		Camera_MoveTo(PIXELS(186), -1, PIXELS(272), 1);
		Object_RefreshSelectorById(20);
		Actor_FaceDirection(20, FACING_EAST, 0);
		do
		{
			Engine_TaskWait(1);
		} while (last->timer == 0);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(27, 2);
		Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 20);
		Engine_EventSetMessage((s32)MsgKareiWhyWeStoppingAtPlace);
		Event_ShowMessageAndWait(27, 0, 10);
		Engine_ActorRunRepeatedMotion(28, 2);
		Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 10);
		Engine_ActorSetAnimation(28, ANIM_NOD);
		Event_ShowMessageAndWait(28, 0, 10);
		Actor_SetAttachedEffect(32, EMOTE_IN_FRONT | 2);
		Engine_EventWait(40);
		Event_ShowMessageAndWait(32, 0, 10);
		Actor_ShowEmote(31, EMOTE_IN_FRONT, 40);
		Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 10);
		Event_ShowMessageAndWait(31, 0, 10);
		Actor_FaceDirection(31, FACING_WEST, 10);
		Engine_ActorRunRepeatedMotion(31, 2);
		Engine_ActorSetAnimation(31, ANIM_SHAKE_HEAD);
		Event_ShowMessageAndWait(31, 0, 10);
		Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 0);
		Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 20);
		Engine_ActorSetAnimation(31, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(32, ANIM_NOD);
		Engine_ActorRunRepeatedMotion(20, 2);
		Actor_SetAttachedEffect(20, EMOTE_IN_FRONT | 2);
		Engine_EventWait(40);
		Event_ShowMessageAndWait(20, 0, 10);
		Engine_ActorRunRepeatedMotion(20, 2);
		head = Actor_Get(20);
		head->steering_flags &= ~ACTOR_FACES_MOTION;
		Actor_WalkToAndWait(20, 172, 264);
		Engine_EventWait(1);
		head_back = Actor_Get(20);
		head_back->steering_flags |= ACTOR_FACES_MOTION;
		Actor_FaceDirection(27, FACING_WEST, 0);
		Actor_FaceDirection(28, FACING_WEST, 0);
		Actor_FaceDirection(32, FACING_WEST, 0);
		Actor_FaceDirection(31, FACING_WEST, 20);
		Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
		Engine_EventWait(20);
		head_again = Actor_Get(20);
		head_again->steering_flags &= ~ACTOR_FACES_MOTION;
		Actor_WalkToAndWait(20, 172, 272);
		Engine_EventWait(1);
		head_home = Actor_Get(20);
		head_home->steering_flags |= ACTOR_FACES_MOTION;
		Actor_WalkToAndWait(20, 180, 272);
		Actor_FaceDirection(20, FACING_EAST, 0);
		Event_ShowMessageAndWait(20, 0, 10);
		Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 0);
		Engine_ActorRunRepeatedMotion(34, 1);
		Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
		Event_ShowMessageAndWait(34, 0, 10);
		Engine_ActorRunRepeatedMotion(33, 1);
		Event_ShowMessageAndWait(33, 0, 10);
		Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
		Event_ShowMessageAndWait(33, 0, 10);
		Engine_ActorRunRepeatedMotion(21, 2);
		Actor_ShowEmote(21, EMOTE_IN_FRONT | 2, 0);
		Event_ShowMessageAndWait(21, 0, 10);
		Engine_ActorJump(20, 2, 20);
		Engine_ActorJump(20, 4, 40);
		Engine_ActorRunRepeatedMotion(20, 2);
		Event_ShowMessageAndWait(20, 0, 10);
		Actor_SetSpeed(21, 0x19999, 0xcccc);
		Actor_WalkToAndWait(21, 265, 282);
		Actor_WalkToAndWait(21, 251, 284);
		Actor_WalkToAndWait(21, 246, 296);
		Actor_FaceDirection(21, FACING_NORTH, 0);
		SceneState_SetValues27Through34();
		Engine_EventWait(40);
		Actor_SetSpeed(21, 0x19999, 0xcccc);
		Actor_WalkToAndWait(21, 228, 296);
		Actor_FaceDirection(21, FACING_NORTH, 40);
		Actor_WalkToAndWait(21, 212, 296);
		Actor_FaceDirection(21, FACING_NORTH, 40);
		Actor_WalkToAndWait(21, 192, 296);
		Actor_FaceDirection(21, FACING_NORTH, 40);
		Engine_ActorStartRepeatedMotion(21, 2);
		Actor_ShowEmote(21, EMOTE_IN_FRONT, 60);
		Actor_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 0);
		Actor_WalkToAndWait(21, 184, 286);
		Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 10);
		Actor_SetAttachedEffect(21, EMOTE_IN_FRONT | 2);
		Engine_EventWait(40);
		Engine_ActorSetAnimationAndWait(21, ANIM_SHAKE_HEAD);
		Actor_ShowEmote(20, EMOTE_IN_FRONT | 1, 40);
		Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
		Engine_ActorRunRepeatedMotion(20, 2);
		Actor_FaceDirection(20, FACING_EAST, 0);
		Actor_FaceDirection(21, FACING_EAST, 60);
		Actor_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 0);
		Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 10);
		Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
		Actor_FaceDirection(21, FACING_EAST, 0);
		Actor_SetSpeed(20, 0x19999, 0xcccc);
		Object_SetActionCallbackAndRefreshById(20, (s32)KareiMachi_DanceScriptB);
		Actor_WalkToAndWait(20, 228, 296);
		Actor_FaceDirection(20, FACING_NORTH, 40);
		Actor_WalkToAndWait(20, 212, 296);
		Actor_FaceDirection(20, FACING_NORTH, 40);
		Actor_WalkToAndWait(20, 192, 296);
		Actor_FaceDirection(20, FACING_NORTH, 40);
		Actor_FaceDirection(20, FACING_NORTHWEST + FACING_STEP, 0);
		Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 10);
		Actor_SetAttachedEffect(20, EMOTE_IN_FRONT | 2);
		Engine_EventWait(60);
		Engine_ActorSetAnimationAndWait(20, ANIM_SHAKE_HEAD);
		message = (s32)MsgKareiShortOnePerson;
		Engine_EventSetMessage(message);
		Event_ShowMessageAndWait(20, 0, 40);
		KareiMachi_SpeedUpActors();
		Actor_WalkToAndWait(20, 178, 272);
		Actor_FaceDirection(20, FACING_EAST, 0);
		Engine_EventWait(240);
		Engine_ActorStop(27);
		Engine_TaskWait(1);
		Actor_FaceDirection(27, FACING_WEST, 10);
		Actor_ShowEmote(27, EMOTE_IN_FRONT | 1, 60);
		Event_ShowMessageAndWait(27, 0, 10);
		SceneEffect_SetSlotVariantAndDescriptor(27);
		Engine_EventWait(80);
		Engine_ActorStop(28);
		Engine_TaskWait(1);
		Actor_FaceDirection(28, FACING_NORTH + FACING_STEP, 20);
		Engine_ActorRunRepeatedMotion(28, 2);
		Event_ShowMessageAndWait(28, 0, 10);
		SceneEffect_SetSlotVariantAndDescriptor(28);
		Engine_EventWait(160);
		Engine_ActorStop(32);
		Engine_TaskWait(1);
		Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 10);
		Actor_ShowEmote(32, EMOTE_IN_FRONT | 1, 60);
		Event_ShowMessageAndWait(32, 0, 10);
		SceneEffect_SetSlotVariantAndDescriptor(32);
		Engine_EventWait(80);
		Engine_ActorStop(30);
		Engine_TaskWait(1);
		Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 10);
		Engine_ActorRunRepeatedMotion(30, 1);
		Engine_EventSetMessage(message + 6);
		Event_ShowMessageAndWait(30, 0, 10);
		Scheduler_RemoveCallback(((s32)KareiMachi_AlternateDance));
		Engine_ActorStop(20);
		Engine_ActorStop(21);
		Engine_TaskWait(1);
		head_wait = Actor_Get(20);
		head_wait->timer = 0;
		last_wait = Actor_Get(21);
		last_wait->timer = 0;
		Actor_SetSpeed(20, 0xcccc, 0x6666);
		Actor_SetSpeed(21, 0xcccc, 0x6666);
		Engine_ActorEnableActionCallback(20, (const u8 *)KareiMachi_DanceScriptC);
		Engine_ActorEnableActionCallback(21, (const u8 *)KareiMachi_DanceScriptD);
		Engine_ActorStop(29);
		Engine_TaskWait(1);
		Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 10);
		message += 5;
		Engine_ActorRunRepeatedMotion(29, 2);
		Engine_EventSetMessage(message);
		Event_ShowMessageAndWait(29, 0, 20);
		SceneEffect_SetSlotVariantAndDescriptor(29);
		SceneEffect_SetSlotVariantAndDescriptor(30);
		do
		{
			Engine_TaskWait(1);
		} while (Actor_Get(20)->timer == 0 || Actor_Get(21)->timer != 1);
		Engine_ActorEnableActionCallback(20, (const u8 *)KareiMachi_DanceScriptE);
		Engine_ActorEnableActionCallback(21, (const u8 *)KareiMachi_DanceScriptF);
		Engine_ActorStop(31);
		Engine_TaskWait(1);
		Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 10);
		Engine_ActorRunRepeatedMotion(31, 1);
		Engine_ActorSetAnimationAndWait(31, ANIM_SHAKE_HEAD);
		message = (s32)MsgKareiDecideEnteringColosso;
		Engine_EventSetMessage(message);
		Event_ShowMessageAndWait(31, 0, 10);
		SceneEffect_SetSlotVariantAndDescriptor(31);
		Engine_ActorStop(34);
		Engine_ActorStop(33);
		Engine_TaskWait(1);
		Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 40);
		Actor_ShowEmote(33, EMOTE_IN_FRONT | 5, 60);
		Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 10);
		Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 10);
		message += 3;
		Engine_ActorSetAnimationAndWait(34, ANIM_SHAKE_HEAD);
		Engine_EventSetMessage(message);
		Event_ShowMessageAndWait(34, 0, 10);
		Engine_ActorRunRepeatedMotion(33, 1);
		Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
		Event_ShowMessageAndWait(33, 0, 10);
		Actor_ShowEmote(34, EMOTE_IN_FRONT | 2, 60);
		Actor_ShowEmote(20, EMOTE_IN_FRONT | 3, 0);
		Engine_ActorRunRepeatedMotion(20, 2);
		Engine_EventSetMessage((s32)MsgKareiThatsWeCantWaitAny);
		Event_ShowMessageAndWait(20, 0, 10);
		Engine_ActorStop(27);
		Engine_ActorStop(28);
		Engine_ActorStop(29);
		Engine_ActorStop(30);
		Engine_ActorStop(32);
		Engine_ActorStop(31);
		Engine_ActorStop(33);
		Engine_ActorStop(34);
		Engine_ActorStop(20);
		Engine_ActorStop(21);
		Engine_TaskWait(1);
		Engine_ActorJump(27, 2, 0);
		Engine_ActorJump(28, 2, 0);
		Engine_ActorJump(29, 2, 0);
		Engine_ActorJump(30, 2, 0);
		Engine_ActorJump(32, 2, 0);
		Engine_ActorJump(31, 2, 0);
		Engine_ActorJump(33, 2, 0);
		Engine_ActorJump(34, 2, 0);
		Engine_ActorJump(21, 2, 40);
		Actor_FaceDirection(27, FACING_WEST, 0);
		Actor_FaceDirection(28, FACING_WEST, 0);
		Actor_FaceDirection(29, FACING_WEST, 0);
		Actor_FaceDirection(30, FACING_WEST, 0);
		Actor_FaceDirection(32, FACING_WEST, 0);
		Actor_FaceDirection(31, FACING_WEST, 0);
		Actor_FaceDirection(33, FACING_WEST, 0);
		Actor_FaceDirection(34, FACING_WEST, 40);
		Engine_ActorJump(21, 4, 40);
		Event_ShowMessageAndWait(21, 0, 10);
		Engine_ActorRunRepeatedMotion(20, 1);
		Event_ShowMessageAndWait(20, 0, 10);
		Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
		Event_ShowMessageAndWait(21, 0, 10);
		Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
		Event_ShowMessageAndWait(20, 0, 10);
		Actor_ShowEmote(27, EMOTE_IN_FRONT | 2, 40);
		Engine_ActorStartRepeatedMotion(27, 1);
		Event_ShowMessageAndWait(27, 0, 10);
		Actor_ShowEmote(28, EMOTE_IN_FRONT | 2, 40);
		Event_ShowMessageAndWait(28, 0, 10);
		Engine_ActorSetAnimationAndWait(21, ANIM_SHAKE_HEAD);
		Engine_EventWait(40);
		Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
		Event_ShowMessageAndWait(21, 0, 20);
		Engine_ActorSetAnimationAndWait(20, ANIM_NOD);
		Event_ShowMessageAndWait(20, 0, 10);
		Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 0);
		Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 4);
		Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 0);
		Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 4);
		Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 0);
		Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 4);
		Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 0);
		Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 4);
		Engine_ActorSetAnimation(27, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(28, ANIM_NOD);
		Engine_ActorSetAnimation(29, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(30, ANIM_NOD);
		Engine_ActorSetAnimation(32, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(31, ANIM_NOD);
		Engine_ActorSetAnimation(33, ANIM_NOD);
		Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
		Engine_ActorJump(20, 2, 40);
		Event_ShowMessageAndWait(20, 0, 10);
		Actor_FaceDirection(27, FACING_WEST, 0);
		Actor_FaceDirection(28, FACING_WEST, 4);
		Actor_FaceDirection(29, FACING_WEST, 0);
		Actor_FaceDirection(30, FACING_WEST, 4);
		Actor_FaceDirection(32, FACING_WEST, 0);
		Actor_FaceDirection(31, FACING_WEST, 4);
		Actor_FaceDirection(33, FACING_WEST, 0);
		Actor_FaceDirection(34, FACING_WEST, 4);
		Actor_SetSpeed(20, 0x11999, 0x8ccc);
		Actor_SetSpeed(27, 0x10ccc, 0x8666);
		Actor_SetSpeed(28, 0x10ccc, 0x8666);
		Actor_SetSpeed(29, 0x10000, 0x8000);
		Actor_SetSpeed(30, 0x10000, 0x8000);
		Actor_SetSpeed(32, 0xf333, 0x7999);
		Actor_SetSpeed(31, 0xf333, 0x7999);
		Actor_SetSpeed(33, 0xe666, 0x7333);
		Actor_SetSpeed(34, 0xe666, 0x7333);
		Actor_SetSpeed(21, 0xd999, 0x6ccc);
		Engine_ActorSetSpritePriority(27, 1);
		Engine_ActorSetSpritePriority(28, 1);
		Engine_ActorSetSpritePriority(29, 1);
		Engine_ActorSetSpritePriority(30, 1);
		Engine_ActorSetSpritePriority(32, 1);
		Engine_ActorSetSpritePriority(31, 1);
		Engine_ActorSetSpritePriority(33, 1);
		Engine_ActorSetSpritePriority(34, 1);
		Engine_ActorSetSpritePriority(20, 1);
		Engine_ActorSetSpritePriority(21, 1);
		Engine_ActorStop(27);
		Engine_ActorStop(28);
		Engine_ActorStop(29);
		Engine_ActorStop(30);
		Engine_ActorStop(32);
		Engine_ActorStop(31);
		Engine_ActorStop(33);
		Engine_ActorStop(34);
		Engine_ActorStop(20);
		Engine_ActorStop(21);
		Engine_TaskWait(1);
		Engine_ActorEnableActionCallback(20, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(27, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(28, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(29, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(30, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(32, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(31, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(33, (const u8 *)SceneAction_GroupOffsetMotion);
		Engine_ActorEnableActionCallback(34, (const u8 *)SceneAction_GroupOffsetMotion);
		closing = Actor_Get(21);
		closing->timer = 0;
		Engine_ActorEnableActionCallback(21, (const u8 *)SceneAction_GroupOffsetMotion);
		do
		{
			Engine_TaskWait(1);
		} while (Actor_Get(21)->timer != 1);
		Engine_EventWait(80);
		Actor_SetPosition(14, PIXELS(340), PIXELS(274));
		Engine_TaskWait(1);
		Actor_SetSpeed(14, 0x10000, 0x8000);
		Actor_WalkToAndWait(14, 224, 274);
		Actor_FaceDirection(14, FACING_EAST, 40);
		Actor_FaceDirection(14, FACING_WEST, 40);
		Actor_FaceDirection(14, FACING_NORTH, 40);
		Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 40);
		Actor_ShowEmote(14, EMOTE_IN_FRONT | 1, 60);
		Event_ShowMessageAndWait(14, 0, 10);
		Actor_FaceDirection(14, FACING_EAST, 40);
		Actor_FaceDirection(14, FACING_NORTH, 40);
		Actor_FaceDirection(14, FACING_WEST, 40);
		Actor_SetAttachedEffect(14, EMOTE_IN_FRONT | 2);
		Engine_ActorJump(14, 4, 40);
		Event_ShowMessageAndWait(14, 0, 20);
		Engine_ActorRunRepeatedMotion(14, 2);
		Event_ShowMessageAndWait(14, 0, 10);
		Engine_ActorJump(14, 4, 40);
		Actor_SetSpeed(14, 0x13333, 0x9999);
		closing = Actor_Get(14);
		closing->timer = 0;
		Engine_ActorEnableActionCallback(14, (const u8 *)SceneAction_GroupOffsetMotion);
		do
		{
			Engine_TaskWait(1);
		} while (Actor_Get(14)->timer != 1);
		Actor_SetPosition(14, PIXELS(359), PIXELS(314));
		late = Actor_Get(14);
		late->facing = FACING_NORTH + FACING_STEP;
		Actor_SetPosition(20, PIXELS(455), PIXELS(434));
		head_end = Actor_Get(20);
		head_end->facing = FACING_NORTH + FACING_STEP;
		Actor_SetPosition(21, PIXELS(464), PIXELS(416));
		last_end = Actor_Get(21);
		last_end->facing = FACING_SOUTH + FACING_STEP;
		Engine_ActorDestroy(27);
		Engine_ActorDestroy(28);
		Engine_ActorDestroy(29);
		Engine_ActorDestroy(30);
		Engine_ActorDestroy(31);
		Engine_ActorDestroy(32);
		Engine_ActorDestroy(33);
		Engine_ActorDestroy(34);
		Event_PlayCue(17);
		if (work->touched_trigger == 9)
		{
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
			leader = Actor_Get(ACTOR_PARTY_LEADER);
			leader->facing = FACING_NORTH;
		}
		else
		{
			Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
			leader = Actor_Get(ACTOR_PARTY_LEADER);
			leader->facing = FACING_SOUTH;
		}
		Audio_PlayCueFromEventWork();
		GameFlag_Set(0x911);
	}
	else
	{
		Event_PlayCue(123);
		Engine_EventRequestExit(work->touched_trigger);
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
	}
	Engine_EventEnd();
}

void KareiMachi_ReleaseEffectLeader(void)
{
	if (GameFlag_IsSet(0x200) != 0)
	{
		*(s32 *)(gActorEffectWork + 24) = 0;
		GameFlag_Clear(0x200);
	}
}

void SceneState_LinkRecordZeroWhenFlag200Clear(void)
{
	u8 *work;

	if (GameFlag_IsSet(0x200) == 0)
	{
		work = gActorEffectWork;
		*(s32 *)(work + 24) = (s32)Object_GetById(ACTOR_PARTY_LEADER);
		GameFlag_Set(0x200);
	}
}

void SceneActor_SetLeaderPriorityAutomatic(void)
{
	u8 *p;
	s32 v;

	p = &Object_GetById(ACTOR_PARTY_LEADER)->priority_flags;
	v = ACTOR_PRIORITY_AUTOMATIC;
	v |= *p;
	*p = v;
}

void SceneState_ClearLeaderPriorityAutomatic(void)
{
	u8 *p;
	s32 v;

	p = &Object_GetById(ACTOR_PARTY_LEADER)->priority_flags;
	v = (u8)~ACTOR_PRIORITY_AUTOMATIC;
	v &= *p;
	*p = v;
}

void SceneDialogue_RunActor181Line916(void)
{
	Scheduler_RemoveCallback((u32)SceneEffect_UpdateLobeOrbitEffect26);
	Engine_TaskWait(1);
	Engine_ActorSetPosition(26, 0, 0);
	Engine_GameFlagSet(0x916);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
}

s32 Scene_Initialize(void)
{
	s32 v;

	Engine_GameFlagSet(0x87a);
	v = gGameState.scene;
	if (v == (s32)&SceneId_KareiMachi1)
	{
		KareiMachi_SetupEntryActors();
	}
	else if (v == (s32)&SceneId_KareiMachi2)
	{
		SceneState_CheckFlags941And940();
	}
	else if (v == (s32)&SceneId_KareiMachi3)
	{
		SceneState_ApplyFlagGatedActorEightSetup();
	}
	else if (v == (s32)&SceneId_KareiMachi5)
	{
		KareiMachi_StartWindowTransition();
	}
	else if (v == (s32)&SceneId_KareiMachi6)
	{
		FieldScene_RunMiddleSequence();
	}
	return 0;
}

void KareiMachi_SetupEntryActors(void)
{
	OrbitingSceneObject		  *nut;
	s32						   found;
	u8						  *heap;
	OrbitingSceneObjectSprite *sprite;

	if (Engine_GameFlagIsSet(0x941) != 0)
	{
		Engine_GameFlagSet(0x321);
		Engine_GameFlagSet(0x913);
		Engine_GameFlagSet(0x912);
		Engine_GameFlagSet(0x915);
	}
	if (Engine_GameFlagIsSet(0x940) != 0)
	{
		Engine_GameFlagSet(0x321);
	}
	if (gGameState.entrance == 14)
	{
		Actor_SetPosition(25, 0x1a80000, 0x580000);
	}
	Engine_ActorSetChildValue(21, 2);
	found = Engine_GameFlagIsSet(0x916);
	if (found != 0)
	{
		Engine_ActorSetPosition(26, 0, 0);
	}
	else
	{
		nut = (OrbitingSceneObject *)Object_GetById(26);
		sprite = nut->sprite;
		sprite->flags_09_mode = 1;
		sprite->flags_05_bit_5 = 0;
		sprite->flags_09_high = 0;
		sprite->state = found;
		nut->active = 1;
		nut->mode = found;
		nut->y = 0xa0000;
		nut->visible = 1;
		heap = Engine_HeapAllocate(17, 0x608);
		Engine_ItemLoadIcon(ITEM_NUT);
		Engine_VramLoad(sprite->palette, 128, heap + 0x400);
		Engine_HeapRelease(17);
		nut->orbit_angle = found;
		nut->orbit_center_x = nut->x;
		nut->orbit_center_y = nut->y;
		nut->orbit_center_z = nut->z;
		Engine_TaskAddCallback(SceneEffect_UpdateLobeOrbitEffect26, 0xc80);
	}
}

void SceneState_CheckFlags941And940(void)
{
	if (Engine_GameFlagIsSet(0x941) != 0)
	{
		Engine_GameFlagSet(0x321);
		Engine_GameFlagSet(0x913);
		Engine_GameFlagSet(0x912);
		Engine_GameFlagSet(0x915);
	}
	if (Engine_GameFlagIsSet(0x940) != 0)
	{
		Engine_GameFlagSet(0x321);
	}
	if (gGameState.entrance != 0)
	{
		if (Engine_GameFlagIsSet(0x912) == 0)
		{
			FieldScene_RunTwoActorCutsceneSequence();
		}
	}
}

void SceneState_ApplyFlagGatedActorEightSetup(void)
{
	struct Obj *o;

	if (Engine_GameFlagIsSet(0xfd6) == 0)
	{
		InitializeOrbitingRenderEffect(12);
	}
	if (Engine_GameFlagIsSet(0x915) != 0)
	{
		o = Object_GetById(8);
		o->f06 = 0;
	}
	if (gGameState.entrance == 10)
	{
		FieldScene_RunSecondaryGroupSequence();
	}
}

void KareiMachi_StartWindowTransition(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (Engine_GameFlagIsSet(0x915) != 0)
	{
		s32 k = 2;
		Engine_MapCopyCellsTo(58, 5, 58, 8, k, 3);
		{
			s32 a = 8, b = 10;
			Engine_MapCopyCellAttributes(8, 11, 2, 1, a, b);
		}
		Engine_MapCopyCellsTo(8, 12, 8, 11, k, 1);
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
	if (gGameState.entrance <= 3)
	{
		BattleFx_SetQueuedSoundAndPlay(170);
	}
}

void FieldScene_RunMiddleSequence(void)
{
	u32				   i;
	s32				   rec7;
	struct FieldActor *record;
	s32				   r0;
	s32				   v5;
	u8				  *p5;
	u8				 **base = (u8 **)&gEventWork;

	*(s32 *)(base[0] + 0x1c0) = 0x204;
	BattleFx_StartFadeOverlay(0);
#if EDITION_INTERNATIONAL
	rec7 = GameFlag_IsSet(0x109);
	if (rec7 != 0)
	{
		p5 = base[9];
		r0 = GameFlag_IsSet(0x200);
		if (r0 != 0)
		{
			r0 = Object_GetById(ACTOR_PARTY_LEADER);
		}
		*(s32 *)(p5 + 24) = r0;
	}
	else
	{
		GameFlag_Set(0x200);
		if (((union GameStateRows *)&gGameState)->halves[225][0] == 4)
		{
			*(s32 *)(base[9] + 24) = rec7;
			GameFlag_Clear(0x200);
		}
	}
#else
	GameFlag_Set(0x200);
	if (GameFlag_IsSet(0x109) != 0)
	{
		p5 = base[9];
		r0 = GameFlag_IsSet(0x200);
		if (r0 != 0)
		{
			r0 = Object_GetById(ACTOR_PARTY_LEADER);
		}
		*(s32 *)(p5 + 24) = r0;
	}
	else if (((union GameStateRows *)&gGameState)->halves[225][0] == 4)
	{
		*(s32 *)(base[9] + 24) = 0;
		GameFlag_Clear(0x200);
	}
#endif
	if (GameFlag_IsSet(0x302) != 0)
	{
		Actor_SetPosition(11, 0x960000, 0x2d80000);
		if (GameFlag_IsSet(0x201) != 0)
		{
			Actor_Get(11);
			v5 = 9;
			Engine_ActorSetAnimation(11, 5);
			Map_CopyCellAttributes(0, 0, 1, 1, v5, 14);
			Map_CopyCellAttributes(0, 0, 1, 1, v5, 45);
			{
				u8 *p = &Object_GetById(11)->priority_flags;
				s32 v = ACTOR_PRIORITY_UNDERFOOT;

				v |= *p;
				*p = v;
			}
		}
	}
	record = Actor_Get(8);
	Engine_ActorSetSpriteFlags((s32)record, 0);
	record = Actor_Get(9);
	Engine_ActorSetSpriteFlags((s32)record, 0);
	Engine_TaskAddCallback((s32)FieldScene_RunSupplementalSequenceOne, 0xc80);
	if (GameFlag_IsSet(0x915) != 0)
	{
		Actor_SetPosition(10, 0x1aa0000, 0x2da0000);
		record = Actor_Get(10);
		{
			u16 shown = 0x5000;

			record->facing = shown;
		}
		Map_CopyCellsTo(88, 48, 88, 45, 2, 3);
		Map_CopyCellsTo(24, 49, 24, 48, 2, 1);
		Map_CopyCellsTo(25, 42, 25, 47, 1, 1);
		Map_CopyCellAttributes(22, 50, 2, 1, 24, 49);
	}
	if (GameFlag_IsSet(0x302) == 0)
	{
	}
	else
	{
		Actor_SetPosition(8, 0xe80000, 0x2dc0000);
		Map_CopyCellAttributes(7, 44, 1, 1, 0, 1);
		Map_CopyCellsTo(74, 58, 78, 41, 1, 5);
		Map_CopyCellsTo(16, 109, 13, 109, 3, 2);
		Map_CopyCellsTo(67, 64, 71, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 72, 44, 1, 2);
		Map_CopyCellsTo(67, 68, 73, 43, 1, 2);
		Map_CopyCellsTo(67, 68, 74, 43, 1, 2);
		Map_CopyCellsTo(67, 64, 75, 44, 1, 2);
		Map_CopyCellsTo(67, 66, 76, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 77, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 78, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 79, 44, 1, 2);
		Map_CopyCellsTo(67, 66, 80, 44, 1, 2);
		Map_CopyCellsTo(2, 0, 9, 42, 2, 2);
		Map_CopyCellsTo(68, 64, 71, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 72, 44, 1, 2);
		Map_CopyCellsTo(68, 68, 73, 43, 1, 2);
		Map_CopyCellsTo(68, 68, 74, 43, 1, 2);
		Map_CopyCellsTo(68, 64, 75, 44, 1, 2);
		Map_CopyCellsTo(68, 66, 76, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 77, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 78, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 79, 44, 1, 2);
		Map_CopyCellsTo(68, 66, 80, 44, 1, 2);
		Map_CopyCellsTo(4, 0, 9, 42, 2, 2);
		Map_CopyCellsTo(7, 11, 7, 42, 10, 8);
		Map_CopyCellsTo(71, 12, 71, 43, 10, 13);
		Map_CopyCellAttributes(6, 13, 12, 12, 6, 44);
		Map_CopyCellAttributes(0, 1, 1, 1, 7, 44);
		goto L_02001cc2;
	}
	switch (((union GameStateRows *)&gGameState)->halves[225][0])
	{
	case 1:
	case 2:
		BattleFx_SetQueuedSoundAndPlay(170);
		break;
	}
L_02001cc2:;
	if (GameFlag_IsSet(0x303) == 0)
	{
	}
	else
	{
		Actor_SetPosition(9, 0x2b80000, 0x2dc0000);
		Map_CopyCellsTo(74, 58, 107, 41, 1, 5);
		Map_CopyCellsTo(45, 109, 42, 109, 3, 2);
		Map_CopyCellsTo(67, 64, 102, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 103, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 104, 44, 1, 2);
		Map_CopyCellsTo(67, 66, 105, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 106, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 107, 44, 1, 2);
		Map_CopyCellsTo(67, 64, 108, 44, 1, 2);
		Map_CopyCellsTo(67, 66, 109, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 102, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 103, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 104, 44, 1, 2);
		Map_CopyCellsTo(68, 66, 105, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 106, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 107, 44, 1, 2);
		Map_CopyCellsTo(68, 64, 108, 44, 1, 2);
		Map_CopyCellsTo(68, 66, 109, 44, 1, 2);
		Map_CopyCellsTo(38, 14, 38, 44, 8, 4);
		Map_CopyCellsTo(102, 14, 102, 44, 8, 12);
		Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
		goto L_02001e5a;
	}
	switch (((union GameStateRows *)&gGameState)->halves[225][0])
	{
	case 3:
	case 4:
		BattleFx_SetQueuedSoundAndPlay(170);
		break;
	}
L_02001e5a:;
}

void SceneState_SetValues27Through34(void)
{
	SceneEffect_SetSlotVariantAndDescriptor(27);
	SceneEffect_SetSlotVariantAndDescriptor(28);
	SceneEffect_SetSlotVariantAndDescriptor(29);
	SceneEffect_SetSlotVariantAndDescriptor(30);
	SceneEffect_SetSlotVariantAndDescriptor(32);
	SceneEffect_SetSlotVariantAndDescriptor(31);
	SceneEffect_SetSlotVariantAndDescriptor(33);
	SceneEffect_SetSlotVariantAndDescriptor(34);
}

void SceneEffect_SetSlotVariantAndDescriptor(s32 a)
{
	struct Obj_02000040 *p;
	u32					 t;

	p = Object_GetById(a);
	p->f64 = a;
	t = Engine_RandomNext();
	p->f66 = (t * 5) >> 12;
	Engine_ObjectSetScript(p, (s32)KareiMachi_Script01);
}

void KareiMachi_SpeedUpActors(void)
{

	{
		s32 zero = 0;

		KareiMachi_DanceStep = zero;
	}
	Actor_SetSpeed(20, 0x19999, 0xcccc);
	Actor_SetSpeed(21, 0x19999, 0xcccc);
	Engine_TaskAddCallback(KareiMachi_AlternateDance, 0xc80);
}

void KareiMachi_AlternateDance(void)
{
	switch (KareiMachi_DanceStep)
	{
	case 0:
		ClearStep((u8 *)Object_GetById(21));
		Engine_ActorEnableActionCallback(21, (const u8 *)KareiMachi_DanceScriptA);
		KareiMachi_DanceStep++;
		break;
	case 1:
		if (Object_GetById(21)->timer != 0)
		{
			ClearStep((u8 *)Object_GetById(20));
			Engine_ActorEnableActionCallback(20, (const u8 *)KareiMachi_DanceScriptB);
			KareiMachi_DanceStep++;
		}
		break;
	case 2:
		if (Object_GetById(20)->timer != 0)
		{
			ClearStep((u8 *)Object_GetById(20));
			Engine_ActorEnableActionCallback(20, (const u8 *)KareiMachi_DanceScriptA);
			KareiMachi_DanceStep++;
		}
		break;
	case 3:
		if (Object_GetById(20)->timer != 0)
		{
			ClearStep((u8 *)Object_GetById(21));
			Engine_ActorEnableActionCallback(21, (const u8 *)KareiMachi_DanceScriptB);
			KareiMachi_DanceStep++;
		}
		break;
	case 4:
		if (Object_GetById(21)->timer != 0)
		{
			KareiMachi_DanceStep = 0;
		}
		break;
	}
}

void FieldScene_RunTwoActorCutsceneSequence(void)
{
	struct FieldActor *actor;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xc00000, 0x1560000);
	Engine_TaskWait(1);
	Camera_SetSpeed(0x3333, 0x666);
	Camera_MoveTo(0xc00000, -1, 0xfc0000, 1);
	gEventWork->transition_frames = 40;
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 0x116);
	Actor_SetSpeed(8, 0x10000, 0x8000);
	Actor_SetSpeed(9, 0x10000, 0x8000);
	actor = Actor_Get(8);
	actor->facing = 0x3000;
	actor = Actor_Get(9);
	actor->facing = 0x5000;
	Engine_TaskWait(1);
	Actor_Get(8)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_Get(9)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkTo(8, 184, 232);
	Actor_WalkToAndWait(9, 198, 232);
	Engine_ActorSetAnimation(8, ANIM_STAND);
	Engine_EventWait(20);
	Actor_Get(8)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_Get(9)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage((s32)MsgKareiLordHammetsPalaceLordAway);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_FaceDirection(9, 0x5000, 10);
	Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
	Engine_EventWait(60);
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
	}
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Actor_WalkToAndWait(IWAN, 212, 0x10c);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 20);
	Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IWAN, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_WalkToAndWait(IWAN, 202, 254);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(IWAN, 0, 10);
	Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 0);
	Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
	Actor_FaceDirection(8, FACING_EAST, 0);
	Actor_FaceDirection(9, FACING_WEST, 40);
	Actor_FaceDirection(8, 0x3000, 0);
	Actor_FaceDirection(9, 0x3000, 20);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 2, 60);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Event_ShowMessageAndWait(IWAN, 0, 10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 60);
	Engine_ActorStartRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(9, 0, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(10);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 10);
	Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 1, 40);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 60);
	Actor_FaceDirection(IWAN, FACING_NORTH, 10);
	Event_ShowMessageAndWait(IWAN, 0, 10);
	Actor_FaceDirection(8, FACING_EAST, 0);
	Actor_FaceDirection(9, FACING_WEST, 40);
	Actor_FaceDirection(8, 0x3000, 0);
	Actor_FaceDirection(9, 0x3000, 10);
	Engine_ActorRunRepeatedMotion(8, 1);
	Event_OpenMessage(8, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorRunRepeatedMotion(IWAN, 2);
		Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IWAN, 0, 10);
		gEventWork->message++;
	}
	else
	{
		gEventWork->message++;
		Engine_ActorSetAnimationAndWait(IWAN, ANIM_SHAKE_HEAD);
		Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IWAN, 0, 10);
	}
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(IWAN, FACING_NORTH, 10);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_SHAKE_HEAD);
	Event_ShowMessageAndWait(IWAN, 0, 10);
	Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(8, 0, 10);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 10);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Event_ShowMessageAndWait(IWAN, 0, 10);
	Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
	Actor_SetAttachedEffect(9, EMOTE_IN_FRONT | 2);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_ShowMessageAndWait(8, 0, 10);
	Engine_ActorRunRepeatedMotion(9, 2);
	Actor_FaceDirection(9, FACING_WEST, 10);
	Event_ShowMessageAndWait(9, 0, 10);
	Actor_FaceDirection(8, FACING_EAST, 10);
	Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
	Actor_FaceDirection(8, 0x3000, 10);
	Event_ShowMessage(8, 0);
	Actor_FaceDirection(9, 0x3000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 10);
	Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IWAN, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Actor_FaceDirection(IWAN, FACING_NORTH, 10);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Engine_ActorSetSpeed(9, 0xcccc, 0x6666);
	actor = Actor_Get(8);
	actor->facing = 0;
	actor = Actor_Get(9);
	actor->facing = 0x8000;
	Actor_Get(8)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_Get(9)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkTo(8, 168, 232);
	Actor_WalkToAndWait(9, 212, 232);
	Engine_ActorSetAnimation(8, ANIM_STAND);
	Engine_EventWait(20);
	Actor_Get(8)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_Get(9)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(IWAN, 192, 232);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Event_PlayCue(188);
	Map_CopyCellsTo(36, 23, 43, 12, 2, 2);
	Engine_TaskWait(5);
	Map_CopyCellsTo(39, 23, 43, 12, 2, 2);
	Engine_TaskWait(5);
	Actor_WalkToAndWait(IWAN, 192, 222);
	Actor_SetPosition(IWAN, 0, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Engine_ActorSetAnimation(8, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
	Actor_WalkTo(8, 184, 232);
	Actor_WalkToAndWait(9, 198, 232);
	Actor_WalkTo(8, 188, 212);
	Actor_WalkToAndWait(9, 194, 212);
	Actor_SetPosition(8, 0, 0);
	Actor_SetPosition(9, 0, 0);
	gEventWork->transition_frames = 24;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(5);
	Engine_EventEnd();
}

void FieldScene_RunSecondaryGroupSequence(void)
{

	u8				  *fieldActor;
	struct FieldActor *object;
	struct EventWork  *work;
	u32				   random;
	u32				   motionPhase;
	const u8		  *motionActions;
	s32				   scale;
	const u8		  *exitActions;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(9, 0x1b80000, 0x20a0000);
	object = Actor_Get(9);
	Engine_ActorSetSpriteFlags(object, 0);
	Camera_MoveTo(0x1b80000, -1, 0x20a0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Camera_SetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x1b80000, -1, 0x1900000, 1);
	Event_PlayCue(141);
	Actor_SetSpeed(9, 0x19999, 0xcccc);
	Actor_WalkToAndWait(9, 0x1b8, 0x190);
	Camera_SetSpeed(0xc000, 0x1800);
	Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
	Actor_SetSpeed(9, 0xcccc, 0x6666);
	Actor_WalkToAndWait(9, 0x1b8, 0x12c);
	Engine_ActorSetAnimation(9, 0);
	Event_PlayCue(0x121);
	Engine_EventWait(40);
	Actor_SetSpeed(11, 0xcccc, 0x6666);
	Actor_SetPosition(11, 0x1b70000, 0x1320000);
	Engine_ActorJump(11, 4, 0);
	Actor_WalkToAndWait(11, 0x1b7, 0x138);
	Actor_WalkToAndWait(11, 0x1a0, 0x138);
	Actor_WalkToAndWait(11, 0x190, 0x100);
	Actor_FaceDirection(11, 0x3000, 40);
	Camera_MoveTo(0x19a0000, -1, 0x1180000, 1);
	Actor_SetSpeed(10, 0x9999, 0x4ccc);
	Actor_SetPosition(10, 0x1b70000, 0x1320000);
	Engine_ActorJump(10, 4, 0);
	Actor_WalkToAndWait(10, 0x1b7, 0x138);
	Actor_WalkToAndWait(10, 0x1a0, 0x138);
	Actor_WalkToAndWait(10, 0x184, 0x10e);
	Actor_FaceDirection(10, 0xd000, 10);
	Actor_FaceDirection(11, 0x5000, 10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b70000, 0x1320000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x138);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a0, 0x138);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x184, 0x12c);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
	fieldActor = Actor_Get(ACTOR_PARTY_LEADER);
	random = Random_Next();
	motionPhase = random * 5;
	motionActions = (const u8 *)KareiMachi_Data02;
	fieldActor += 102;
	*(u16 *)fieldActor = motionPhase >> 12;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, motionActions);
	Engine_ActorJump(11, 2, 20);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventSetMessage((s32)MsgKareiWeveArrivedHammet);
	Event_ShowMessageAndWait(11, 0, 10);
	Engine_ActorSetAnimationAndWait(10, 3);
	object = Actor_Get(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(GERALD, object->x.fixed, object->z.fixed);
	}
	object = Actor_Get(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(IWAN, object->x.fixed, object->z.fixed);
	}
	object = Actor_Get(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(MEARI, object->x.fixed, object->z.fixed);
	}
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
	Actor_WalkTo(GERALD, 0x17a, 0x136);
	Actor_WalkTo(IWAN, 0x190, 0x120);
	Actor_WalkToAndWait(MEARI, 0x19a, 0x134);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 40);
	fieldActor = Actor_Get(GERALD);
	random = Random_Next();
	fieldActor += 102;
	*(u16 *)fieldActor = ((random * 5) >> 12);
	fieldActor = Actor_Get(IWAN);
	random = Random_Next();
	fieldActor += 102;
	*(u16 *)fieldActor = ((random * 5) >> 12);
	fieldActor = Actor_Get(MEARI);
	random = Random_Next();
	fieldActor += 102;
	*(u16 *)fieldActor = ((random * 5) >> 12);
	Engine_ActorEnableActionCallback(GERALD, motionActions);
	Engine_ActorEnableActionCallback(IWAN, motionActions);
	Engine_ActorEnableActionCallback(MEARI, motionActions);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Event_ShowMessageAndWait(0x2002, 0, 10);
	Actor_ShowEmote(10, 0x100, 40);
	Actor_FaceDirection(10, 0x3000, 20);
	Event_ShowMessageAndWait(10, 0, 10);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(GERALD);
	Engine_ActorStop(IWAN);
	Engine_ActorStop(MEARI);
	Engine_TaskWait(1);
	Actor_FaceDirection(IWAN, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 20);
	Engine_ActorSetAnimationAndWait(10, 3);
	Event_ShowMessageAndWait(10, 0, 10);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessageAndWait(0x2003, 0, 10);
	Engine_ActorSetAnimationAndWait(10, 3);
	Event_ShowMessageAndWait(10, 0, 10);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Actor_FaceDirection(10, 0x5000, 0);
	Engine_ActorRunRepeatedMotion(11, 2);
	Event_ShowMessageAndWait(11, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Actor_FaceDirection(GERALD, 0xe000, 0);
	Actor_FaceDirection(IWAN, 0x2000, 0);
	Actor_FaceDirection(MEARI, 0xa000, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 10);
	Actor_FaceDirection(10, 0xd000, 10);
	Engine_ActorSetAnimationAndWait(10, 3);
	Event_ShowMessageAndWait(10, 0, 10);
	Engine_ActorSetAnimationAndWait(11, 3);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Event_ShowMessageAndWait(0x2002, 0, 10);
	Actor_FaceDirection(11, 0x3000, 10);
	Engine_ActorSetAnimation(11, 4);
	Event_ShowMessageAndWait(11, 0, 10);
	Actor_ShowEmote(MEARI, 0x101, 40);
	Event_ShowMessageAndWait(0x2003, 0, 10);
	Actor_FaceDirection(10, 0x3000, 10);
	Actor_ShowEmote(10, 0x108, 20);
	Event_ShowMessageAndWait(10, 0, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Actor_FaceDirection(11, 0x5000, 0);
	Actor_FaceDirection(10, 0x5000, 10);
	Engine_ActorSetAnimation(11, 4);
	Event_ShowMessageAndWait(11, 0, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
	Actor_FaceDirection(GERALD, 0xe000, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(11, 4);
	Event_ShowMessageAndWait(11, 0, 10);
	Engine_ActorSetAnimationAndWait(10, 4);
	Event_ShowMessageAndWait(10, 0, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
	Actor_ShowEmote(GERALD, 0x105, 0);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Actor_ShowEmote(MEARI, 0x105, 60);
	Actor_FaceDirection(10, 0x3000, 10);
	Engine_ActorSetAnimation(10, 3);
	Event_OpenMessage(10, 0);
	Actor_FaceDirection(GERALD, 0xe000, 0);
	Actor_FaceDirection(IWAN, 0x6000, 0);
	Actor_FaceDirection(MEARI, 0xa000, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		gEventWork->message += 3;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(11, 2);
		Event_ShowMessageAndWait(11, 0, 40);
		Event_ShowMessageAndWait(11, 0, 10);
		Actor_ShowEmote(MEARI, 0x106, 40);
		Actor_FaceDirection(MEARI, 0xc000, 20);
		Event_ShowMessageAndWait(0x2003, 0, 10);
	}
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(8, 0x3000, 0);
	Engine_ActorSetAnimationAndWait(10, 3);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x1180000, -1, 0xc80000, 1);
	Actor_WalkToAndWait(10, 0x14d, 222);
	Actor_WalkToAndWait(10, 0x11c, 198);
	Actor_FaceDirection(10, 0x8000, 10);
	Engine_ActorSetAnimationAndWait(10, 3);
	Actor_FaceDirection(8, 0, 10);
	Engine_ActorSetAnimationAndWait(8, 3);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_WalkToAndWait(8, 0x10c, 198);
	Actor_FaceDirection(8, 0xc000, 10);
	Engine_ActorRunRepeatedMotion(8, 2);
	Event_PlayCue(125);
	Map_CopyCellsTo(71, 60, 76, 11, 2, 1);
	Map_CopyCellAttributes(71, 60, 2, 1, 16, 11);
	Engine_EventWait(20);
	Actor_WalkToAndWait(8, 246, 198);
	Actor_FaceDirection(8, 0, 20);
	((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(0xf80000, -1, 0xaa0000, 1);
	Actor_WalkToAndWait(10, 0x10e, 198);
	Actor_WalkToAndWait(10, 0x10e, 174);
	Actor_WalkToAndWait(10, 224, 170);
	Actor_WalkToAndWait(10, 210, 158);
	Actor_WalkToAndWait(10, 246, 148);
	Actor_WalkToAndWait(10, 246, 142);
	Actor_SetPosition(10, 0, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Actor_SetPosition(9, 0x1b80000, 0x1540000);
	object = Actor_Get(9);
	scale = 0x4000;
	object->facing = scale;
	Camera_MoveTo(0x17c0000, -1, 0x1180000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(10);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(3, 3);
	Actor_WalkToAndWait(11, 0x1a4, 0x11a);
	Actor_WalkToAndWait(11, 0x1a4, 0x138);
	Actor_WalkToAndWait(11, 0x1b7, 0x138);
	Actor_WalkToAndWait(11, 0x1b7, 0x132);
	Actor_SetPosition(11, 0, 0);
	exitActions = (const u8 *)KareiMachi_Data03;
	Engine_ActorEnableActionCallback(GERALD, exitActions);
	Engine_ActorEnableActionCallback(IWAN, (s32)exitActions);
	Object_SetActionCallbackAndRefreshById(3, (s32)exitActions);
	Camera_MoveTo(0x19a0000, -1, 0x12c0000, 1);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a0, 0x138);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x138);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b7, 0x132);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Event_PlayCue(141);
	Actor_SetSpeed(9, 0xcccc, 0x6666);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x1b80000, -1, 0x1a40000, 1);
	Actor_WalkToAndWait(9, 0x1b8, 0x1a4);
	Camera_SetSpeed(0x20000, scale);
	Camera_MoveTo(0x1b80000, -1, 0x2580000, 1);
	Actor_SetSpeed(9, 0x19999, 0xcccc);
	Actor_WalkToAndWait(9, 0x1b8, 0x1f4);
	Actor_WalkTo(9, 0x1b8, 0x258);
	Event_PlayCue(0x121);
	work = gEventWork;
	work->transition_frames = 24;
	work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(10);
	Engine_EventEnd();
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

void FieldScene_RunLateSequence(void)
{
	s32				   tmp[3];
	struct FieldActor *leader;
	struct FieldActor *block;
	s32				   idx;
	s32				   w;
	s32				   w2;
	s32				   a;
	s32				  *dst;
	s32				   zero;
	s32				   k;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	idx = (leader->facing >> 12);
	a = leader->x.part.pixel;
	w = KareiMachi_Data01[idx];
	block = SceneActor_FindActorAtCell((a + (w >> 16)) >> 4, (leader->z.part.pixel + (s16)w) >> 4);
	if (block != NULL)
	{
		zero = 0;
		block->terrain_id = 2;
		dst = tmp;
		w2 = KareiMachi_Data01[idx];
		dst[0] = block->x.fixed + (w2 & -0x10000);
		dst[1] = block->y.fixed;
		dst[2] = block->z.fixed + (w2 << 16);
		if (Object_CheckMovementCollision(block, dst) <= 0)
		{
			Object_SetMode(leader, 8);
			k = 0x3333;
			Engine_TaskWait(15);
			Engine_AudioPlayCue(185);
			block->speed = k;
			block->acceleration = k;
			Engine_ObjectSetPosition(block, dst[0], dst[1], dst[2]);
			leader->speed = k;
			leader->acceleration = k;
			Engine_ObjectSetPosition(leader, dst[0], dst[1], dst[2]);
			Object_CommitPosition(block);
			BattleFx_PlayQueuedSound();
			block->x.fixed = dst[0];
			block->z.fixed = dst[2];
			block->velocity_x = zero;
			block->velocity_z = zero;
			Object_SetMode(leader, 1);
			FieldScene_RunScene3a8SequenceB();
		}
	}
}

void FieldScene_RunScene3a8SequenceB(void)
{
	struct FieldActor *rec8;
	struct FieldActor *rec4;
	s32				   rec7;
	struct FieldActor *rec2;
	u32				   i;
	s32				   v5;
	s32				   v6;
	s32				   v7;

	rec8 = Object_GetById(8);
	rec4 = Object_GetById(9);
	rec7 = GameFlag_IsSet(0x302);
	if (rec7 != 0)
	{
	}
	else if ((rec8->x.fixed >> 19) > 29)
	{
	}
	else
	{
		rec2 = Object_GetById(11);
		Engine_EventBegin();
		Map_CopyCellAttributes(7, 44, 1, 1, rec7, 1);
		i = 67;
		v7 = 1;
		v6 = 5;
		do
		{
			Map_CopyCellsTo(i, 58, 78, 41, v7, v6);
			Engine_EventWait(4);
			if (i == 70)
			{
				GameFlag_Set(0x302);
			}
			i++;
		} while (i <= 74);
		v5 = 2;
		Map_CopyCellsTo(16, 109, 13, 109, 3, v5);
		Engine_EventWait(40);
		rec2->scale_x = 0x1999;
		rec2->scale_y = 0x1999;
		Actor_SetPosition(11, 0x960000, 0x2d80000);
		Engine_ActorEnableActionCallback(11, (const u8 *)KareiMachi_ActionScript01);
		v6 = 1;
		Map_CopyCellsTo(67, 64, 71, 44, v6, v5);
		Map_CopyCellsTo(67, 64, 72, 44, v6, v5);
		Map_CopyCellsTo(67, 68, 73, 43, v6, v5);
		Map_CopyCellsTo(67, 68, 74, 43, v6, v5);
		Map_CopyCellsTo(67, 64, 75, 44, v6, v5);
		Map_CopyCellsTo(67, 66, 76, 44, v6, v5);
		Map_CopyCellsTo(67, 64, 77, 44, v6, v5);
		Map_CopyCellsTo(67, 64, 78, 44, v6, v5);
		Map_CopyCellsTo(67, 64, 79, 44, v6, v5);
		Map_CopyCellsTo(67, 66, 80, 44, v6, v5);
		Map_CopyCellsTo(2, 0, 9, 42, v5, v5);
		Engine_EventWait(40);
		Map_CopyCellsTo(68, 64, 71, 44, v6, v5);
		Map_CopyCellsTo(68, 64, 72, 44, v6, v5);
		Map_CopyCellsTo(68, 68, 73, 43, v6, v5);
		Map_CopyCellsTo(68, 68, 74, 43, v6, v5);
		Map_CopyCellsTo(68, 64, 75, 44, v6, v5);
		Map_CopyCellsTo(68, 66, 76, 44, v6, v5);
		Map_CopyCellsTo(68, 64, 77, 44, v6, v5);
		Map_CopyCellsTo(68, 64, 78, 44, v6, v5);
		Map_CopyCellsTo(68, 64, 79, 44, v6, v5);
		Map_CopyCellsTo(68, 66, 80, 44, v6, v5);
		Map_CopyCellsTo(4, 0, 9, 42, v5, v5);
		Engine_EventWait(40);
		v5 = 10;
		Map_CopyCellsTo(7, 11, 7, 42, v5, 8);
		Map_CopyCellsTo(71, 12, 71, 43, v5, 13);
		v5 = 44;
		Map_CopyCellAttributes(6, 13, 12, 12, 6, v5);
		Engine_EventWait(40);
		SceneState_LinkRecordZeroWhenFlag200Clear();
		Map_CopyCellAttributes(0, 1, 1, 1, 7, v5);
		Engine_EventEnd();
	}
	if (GameFlag_IsSet(0x303) != 0)
	{
	}
	else if ((rec4->x.fixed >> 19) > 87)
	{
	}
	else
	{
		Engine_EventBegin();
		i = 67;
		v7 = 1;
		v6 = 5;
		do
		{
			Map_CopyCellsTo(i, 58, 107, 41, v7, v6);
			Engine_EventWait(4);
			if (i == 70)
			{
				GameFlag_Set(0x303);
			}
			i++;
		} while (i <= 74);
		v6 = 2;
		Map_CopyCellsTo(45, 109, 42, 109, 3, v6);
		Engine_EventWait(40);
		v5 = 1;
		Map_CopyCellsTo(67, 64, 102, 44, v5, v6);
		Map_CopyCellsTo(67, 64, 103, 44, v5, v6);
		Map_CopyCellsTo(67, 64, 104, 44, v5, v6);
		Map_CopyCellsTo(67, 66, 105, 44, v5, v6);
		Map_CopyCellsTo(67, 64, 106, 44, v5, v6);
		Map_CopyCellsTo(67, 64, 107, 44, v5, v6);
		Map_CopyCellsTo(67, 64, 108, 44, v5, v6);
		Map_CopyCellsTo(67, 66, 109, 44, v5, v6);
		Engine_EventWait(40);
		Map_CopyCellsTo(68, 64, 102, 44, v5, v6);
		Map_CopyCellsTo(68, 64, 103, 44, v5, v6);
		Map_CopyCellsTo(68, 64, 104, 44, v5, v6);
		Map_CopyCellsTo(68, 66, 105, 44, v5, v6);
		Map_CopyCellsTo(68, 64, 106, 44, v5, v6);
		Map_CopyCellsTo(68, 64, 107, 44, v5, v6);
		Map_CopyCellsTo(68, 64, 108, 44, v5, v6);
		Map_CopyCellsTo(68, 66, 109, 44, v5, v6);
		Engine_EventWait(40);
		v5 = 8;
		Map_CopyCellsTo(38, 14, 38, 44, v5, 4);
		Map_CopyCellsTo(102, 14, 102, 44, v5, 12);
		Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
		Engine_EventWait(40);
		SceneState_LinkRecordZeroWhenFlag200Clear();
		Engine_EventEnd();
	}
}

void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p)
{
	s16 *h;
	s32	 v;
	s32	 a;
	s32	 b;

	h = &p->f64;
	v = *h;
	if (v == 0)
	{
		/* FAKEMATCH: Object_Destroy is called as void (*)(void); as void (*)(struct DispatchObject *), its prototype, SceneEffect_UpdateMotionWithDamping does not compile */
		((void (*)(void))Object_Destroy)();
	}
	else if (v == 1)
	{
		p->f24 = 0;
		p->f28 = 0;
		p->f08 = 0;
		p->f0c = 0;
	}
	else
	{
		p->f18 += 0x800;
		p->f1c += 0x800;
	}
	p->f08 += p->f24;
	p->f0c += p->f28;
	a = p->f24;
	b = p->f28;
	p->f24 = a - a / 256;
	p->f28 = b - b / 16;
	{
		s32 t = *(u16 *)h;
		t -= 1;
		*(u16 *)h = t;
	}
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
	struct FieldActor *a;
	struct FieldActor *b;

	a = Actor_Get(8);
	b = Actor_Get(9);
	if ((u32)(a->x.part.pixel - 381) <= 12 && a->z.part.pixel > 777)
	{
		a->sprite->priority = Actor_Get(ACTOR_PARTY_LEADER)->sprite->priority;
	}
	else if (!GameFlag_IsSet(0x302) && a->x.part.pixel <= 245 && (gFrameCount & 1) == 0)
	{
		if (!GameFlag_IsSet(0x202))
		{
			BattleFx_SetQueuedSoundAndPlay(-1);
			Event_PlayCue(230);
			GameFlag_Set(0x202);
		}
		FieldScene_RunScene3a8SequenceA(a->x.fixed, a->y.fixed, a->z.fixed);
	}
	if (!GameFlag_IsSet(0x303) && b->x.part.pixel <= 709 && (gFrameCount & 1) == 0)
	{
		if (!GameFlag_IsSet(0x203))
		{
			BattleFx_SetQueuedSoundAndPlay(-1);
			Event_PlayCue(230);
			GameFlag_Set(0x203);
		}
		FieldScene_RunScene3a8SequenceA(b->x.fixed, b->y.fixed, b->z.fixed);
	}
}

void FieldScene_RunScene3a8SequenceA(s32 a0, s32 a1, s32 a2)
{
	s32 p8;
	u8 *rec7;
	s32 value;
	s32 mask;
	u8 *link;

	p8 = a2;
	value = Engine_RandomNext();
	rec7 = (u8 *)Engine_ObjectCreate(
		222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
	if ((s32)rec7 != 0)
	{
		rec7[85] = (mask = 0);
		link = (u8 *)*(s32 *)((s32)rec7 + 80);
		mask -= 13;
		link[9] = ((link[9] & mask) | 8);
		ObjectGroup_SetChildValue((s32)rec7, 9);
		Engine_ActorSetSpriteFlags((s32)rec7, 0);
		value = Random_Next();
		*(s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
		value = Random_Next();
		*(s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
		{
			u16 *target = (u16 *)((s32)rec7 + 100);
			u16	 shown = 20;

			*target = shown;
			*((u8 *)target - 3) = 1;
		}
		Object_SetMode((s32)rec7, 1);
		Engine_ObjectSetScript((s32)rec7, (s32)KareiMachi_Script02);
	}
}

void SceneEffect_UpdateLobeOrbitEffect26(void)
{
	Effect_0200390c *effect = Object_GetById(26);
	RenderData		*render = effect->render;
	s32				 offset = Engine_MathSin(effect->angle) * 2;
	s32				 first;

	if (offset > 0)
	{
		offset = -offset;
	}
	effect->x = effect->base_x + Engine_MathCos(effect->angle) * 2;
	effect->y = effect->base_y + offset;
	render->rotation = Engine_MathCos(effect->angle + 0x8000) >> 3;
	first = Engine_RandomNext();
	effect->angle += ((u32)(first << 9) >> 16) + ((u32)(Engine_RandomNext() << 9) >> 16) + 0x400;
}

void FieldScene_DrawTilesAndRaiseActor11(void)
{
	Effect *effect = Object_GetById(11);

	Engine_MapCopyCellAttributes(0, 0, 1, 1, 9, 14);
	Engine_MapCopyCellAttributes(0, 0, 1, 1, 9, 45);
	if (effect != 0)
	{
		Engine_ActorSetSpriteFlags(effect, 0);
		effect->y -= 0x200000;
		effect->state23 = 2;
	}
	Engine_GameFlagSet(0x201);
}

s32 SceneEffect_UpdateOrbitingEffect(Effect_0200390c *effect)
{
	RenderData *render = effect->render;
	s32			ofs = Engine_MathSin(effect->angle) * 2;
	s32			first;

	if (ofs > 0)
	{
		ofs = -ofs;
	}
	effect->x = effect->base_x + Engine_MathCos(effect->angle) * 2;
	effect->y = effect->base_y + ofs;
	render->rotation = Engine_MathCos(effect->angle + 0x8000) / 8;
	first = Engine_RandomNext();
	effect->angle += ((u32)(first << 9) >> 16) + ((u32)(Engine_RandomNext() << 9) >> 16) + 0x400;
	return 0;
}

void InitializeOrbitingRenderEffect(s32 id)
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
	actor->callback = (u32)SceneEffect_UpdateOrbitingEffect;
	actor->state = zero;
}

// 向きごとの 1 マス先: 上位 16 ビットが x、下位 16 ビットが z
#define FACING_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 KareiMachi_Data01[] = {
	FACING_STEP(16, 0),
	FACING_STEP(16, 0),
	FACING_STEP(16, 0),
	FACING_STEP(0, 16),
	FACING_STEP(0, 16),
	FACING_STEP(0, 16),
	FACING_STEP(0, 16),
	FACING_STEP(-16, 0),
	FACING_STEP(-16, 0),
	FACING_STEP(-16, 0),
	FACING_STEP(-16, 0),
	FACING_STEP(0, -16),
	FACING_STEP(0, -16),
	FACING_STEP(0, -16),
	FACING_STEP(0, -16),
	FACING_STEP(16, 0),
};

s32 SceneAction_GroupMotion[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-80), 0, 0),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(0x23, 1),
	SC_WAIT(1),
	SC_END,
};

s32 SceneAction_GroupOffsetMotion[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-72), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(PIXELS(-100), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(PIXELS(-100), 0, 0),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(0x23, 1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
	SC_ARG(2),
	SC_MOVE(PIXELS(192), 0, PIXELS(222)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_Script01[] = {
	SC_CALL(SceneEffect_UpdateRandomAction),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KareiMachi_DanceScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(228), 0, PIXELS(296)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_ARG(2),
	SC_MOVE(PIXELS(212), 0, PIXELS(296)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_ARG(2),
	SC_MOVE(PIXELS(192), 0, PIXELS(296)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_SET(0x23, 1),
	SC_ARG(2),
	SC_MOVE(PIXELS(178), 0, PIXELS(272)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_DanceScriptB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(164), 0, PIXELS(278)),
	SC_SYNC,
	SC_MOVE(PIXELS(182), 0, PIXELS(304)),
	SC_SYNC,
	SC_MOVE(PIXELS(238), 0, PIXELS(304)),
	SC_SYNC,
	SC_MOVE(PIXELS(246), 0, PIXELS(296)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(40),
	SC_SET(0x23, 1),
	SC_END,
};

s32 KareiMachi_DanceScriptC[] = {
	SC_SET(SW_TARGET_X, PIXELS(-32768)),
	SC_SET(SW_TARGET_Y, PIXELS(-32768)),
	SC_SET(SW_TARGET_Z, PIXELS(-32768)),
	SC_SET(SW_VELOCITY_X, 0),
	SC_SET(SW_VELOCITY_Y, 0),
	SC_SET(SW_VELOCITY_Z, 0),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(184), 0, PIXELS(294)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(0x23, 1),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_DanceScriptD[] = {
	SC_SET(SW_TARGET_X, PIXELS(-32768)),
	SC_SET(SW_TARGET_Y, PIXELS(-32768)),
	SC_SET(SW_TARGET_Z, PIXELS(-32768)),
	SC_SET(SW_VELOCITY_X, 0),
	SC_SET(SW_VELOCITY_Y, 0),
	SC_SET(SW_VELOCITY_Z, 0),
	SC_WAIT(1),
	SC_ARG(2),
	SC_MOVE(PIXELS(198), 0, PIXELS(300)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(0x23, 1),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_DanceScriptE[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(178), 0, PIXELS(272)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(0x23, 0),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_DanceScriptF[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(255), 0, PIXELS(289)),
	SC_SYNC,
	SC_MOVE(PIXELS(265), 0, PIXELS(280)),
	SC_SYNC,
	SC_MOVE(PIXELS(284), 0, PIXELS(272)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_SET(0x23, 0),
	SC_WAIT(1),
	SC_END,
};

s32 KareiMachi_Data02[] = {
	SC_CALL(OverlayObject_AdvanceXWhenCounterExpires),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KareiMachi_Data03[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(388), 0, PIXELS(300)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 KareiMachi_ActionScript01[] = {
	SC_ADD(SW_SCALE_X, 546),
	SC_ADD(SW_SCALE_Y, 546),
	SC_WAIT(1),
	SC_LOOP(108, SC_TOP),
	SC_END,
};

struct SceneEntrance gKareiMachiEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 72, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 408, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 408, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 328, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 120, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 312, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 152, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 224, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 56, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 224, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 296, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 440, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 192, 0, 280, FACING_NORTH, 0, 0, 0, 400, 352, 0 },
	{ 2, CONDITION_ALWAYS, 192, 0, 232, FACING_SOUTH, 0, 0, 0, 400, 352, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 248, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 424, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 440, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances4[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 296, 0, 104, FACING_SOUTH, 0, 100, 50, 440, 445, 0 },
	{ 2, CONDITION_ALWAYS, 280, 0, 392, FACING_NORTH, 0, 100, 50, 440, 445, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances5[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 296, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 632, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 152, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 568, 0, 152, FACING_SOUTH, 0, 390, 0, 630, 220, 0 },
	{ 5, CONDITION_ALWAYS, 472, 0, 72, FACING_SOUTH, 0, 390, 0, 630, 220, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKareiMachiEntrances6[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 408, 0, 632, FACING_SOUTH, 0, 85, 400, 465, 880, 0 },
	{ 2, CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0 },
	{ 3, CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0 },
	{ 4, CONDITION_ALWAYS, 856, 0, 856, FACING_NORTH, 0, 595, 400, 930, 880, 0 },
	{ SCENE_TABLE_END },
};

u32 KareiMachi_SceneTable01[] = {
	SCENE_EXITS(99),
	SCENE_EXIT(1, 100, 1),
	SCENE_EXIT(2, 100, 2),
	SCENE_EXIT(3, 100, 3),
	SCENE_EXIT(4, 100, 4),
	SCENE_EXIT(5, 100, 6),
	SCENE_EXIT(6, 100, 7),
	SCENE_EXIT(7, 100, 10),
	SCENE_EXIT(8, 101, 1),
	SCENE_EXIT(9, 2, 20),
	SCENE_EXIT(10, 2, 19),
	SCENE_EXIT(11, 102, 1),
	SCENE_EXIT(12, 100, 5),
	SCENE_EXIT(13, 100, 8),
	SCENE_EXIT(14, 156, 4),
	SCENE_EXIT(0, 102, 0),
	SCENE_EXIT(1, 99, 11),
	SCENE_EXIT(2, 103, 1),
	SCENE_EXIT(5, 103, 20),
	SCENE_EXIT(0, 153, 0),
	SCENE_EXIT(1, 154, 2),
	SCENE_EXIT(2, 2, 60),
	SCENE_EXIT(10, 2, 71),
	SCENE_EXIT(0, 154, 0),
	SCENE_EXIT(1, 155, 5),
	SCENE_EXIT(2, 153, 1),
	SCENE_EXIT(0, 155, 0),
	SCENE_EXIT(1, 156, 2),
	SCENE_EXIT(2, 156, 3),
	SCENE_EXIT(3, 155, 4),
	SCENE_EXIT(4, 155, 3),
	SCENE_EXIT(5, 154, 1),
	SCENE_EXIT(0, 156, 0),
	SCENE_EXIT(1, 103, 11),
	SCENE_EXIT(2, 155, 1),
	SCENE_EXIT(3, 155, 2),
	SCENE_EXIT(4, 99, 14),
	SCENE_EXITS_END,
};

struct ScenePlacement gKareiMachiPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKareiMachiPlacements1[] = {
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(188), 0, PIXELS(476), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(260), 0, PIXELS(476), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(222), 0, PIXELS(382), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(134), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(115), 0, PIXELS(453), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(484), 0, PIXELS(258), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(359), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(404), 0, PIXELS(268), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(154), 0, PIXELS(119), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(198), 0, PIXELS(84), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(199), 0, PIXELS(233), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(46), 0, PIXELS(217), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(468), 0, PIXELS(434), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(464), 0, PIXELS(416), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(408), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(278), 0, PIXELS(318), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(56), 0, PIXELS(332), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement KareiMachi_StagedGroupScript[] = {
	{ 0x84, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x43, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKareiMachiPlacements2[] = {
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(212), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKareiMachiPlacements3[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(246), 0, PIXELS(198), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x111, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x31, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x44, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0xfd6, 7,
		PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKareiMachiPlacements6[] = {
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(826), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(732), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x44, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5c, 0x5b, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKareiMachiEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents1[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_RunExitDoor) },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStagedGroupSequence) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStagedGroupSequence) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKareiWelcomeInPeace) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKareiSoldiersProtectPeace) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKareiHammetGenerousMerchant) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKareiHammetOnJourney) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKareiNorthernBridgeRebuilt) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKareiRamakanTrouble) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKareiElderTownNews) },
	{ EVENT_TALK, 15, 0x911, MSGNO(MsgKareiInnFestivalTourists) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKareiFormerInnFestivalTourists) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKareiChildWantsFestival) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKareiTraderTownNews) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKareiHammetBusinessSkill) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorNineteenDialogue) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgKareiHammetCapturedRumor) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgKareiHammetWentNorth) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_BranchOnSlotZeroFacing) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorTwentyThreeByLeaderHeading) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTwentyFourAngleDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKareiWarriorHonestEyes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKareiGuardsJudgeVisitors) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKareiHammetPeaceAndLiberty) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKareiHammetCanBeRash) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKareiSoldiersWentToKuupuappu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKareiStrangeEvents) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKareiElderThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKareiWishSavedForFestival) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKareiChildFestivalDream) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKareiTraderThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKareiHammetValuesTrust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKareiTrustAgainstForce) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKareiRunpaNorthOfKuupuappu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKareiRumorsHideTruth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKareiWishBetterWeapons) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKareiWeaponsHaveSoul) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgKareiPalaceWaterSupply) },
	{ PSYNERGY_ON_ACTOR(0x8c), 25, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0x916, EVENT_SCRIPT(SceneDialogue_RunActor181Line916) },
	{ SEARCH(SEARCH_JAR), 100, 0xf81, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_JAR), 101, 0xf82, FIND_ITEM(ITEM_VIAL) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWithValue29df) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(RunSceneArrivalSetup) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKareiSoldiersHandleAffairs) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKareiSoldiersRescueHammet) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKareiPrayForHammetSafety) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKareiWarriorsWillSaveHammet) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSlotZeroFacingSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKareiPassageClosedByOrders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, 0xfd6, EVENT_SCRIPT(FieldScene_RunStepWithValueFd6) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents4[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf22, FIND_ITEM(0xc1) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents5[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ 0x31, 4, CONDITION_ALWAYS, 4 },
	{ 0x21, 5, CONDITION_ALWAYS, 5 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKareiMachiEvents6[] = {
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorTenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKareiTreasureBeyondDoors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x302, EVENT_SCRIPT(FieldScene_RunScene3a8SequenceB) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x303, EVENT_SCRIPT(FieldScene_RunScene3a8SequenceB) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateSequence) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateSequence) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x201, EVENT_SCRIPT(FieldScene_DrawTilesAndRaiseActor11) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(KareiMachi_ReleaseEffectLeader) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_LinkRecordZeroWhenFlag200Clear) },
	{ EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearLeaderPriorityAutomatic) },
	{ EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetLeaderPriorityAutomatic) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf1c, FIND_ITEM(0x84) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues12And2And3) },
	{ SEARCH(SEARCH_CHEST), 102, 0xf1e, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_CHEST), 103, 0xf1f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 104, 0xf20, FIND_COINS(200) },
	{ SEARCH(SEARCH_CHEST), 105, 0xf21, FIND_ITEM(0xb7) },
	{ SCENE_EVENTS_END },
};

u16 KareiMachi_DoorCellData[] = {
	MAP_CELL_STEP(35, 33, 1, 2, 5), MAP_CELL_STEP(37, 33, 1, 2, 5), MAP_CELL_STEPS_END,
};

struct MapCellAnim KareiMachi_DoorCells[] = {
	{ KareiMachi_DoorCellData, 36, 10 },
	{ KareiMachi_DoorCellData, 57, 7 },
	{ KareiMachi_DoorCellData, 57, 11 },
	{ KareiMachi_DoorCellData, 57, 23 },
	{ KareiMachi_DoorCellData, 52, 25 },
	{ KareiMachi_DoorCellData, 39, 14 },
	{ KareiMachi_DoorCellData, 51, 14 },
	{ KareiMachi_DoorCellData, 41, 25 },
};

s32 KareiMachi_Script02[] = {
	SC_CALL(SceneEffect_UpdateMotionWithDamping),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
