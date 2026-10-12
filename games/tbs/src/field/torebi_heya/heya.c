#include "game_state.h"
#include "unit/field/torebi_heya/heya.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "torebi.h"
#include "call.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

u8 *TorebiHeya_GetEntrances(void)
{
	return Data_02008bb4;
}

s32 TorebiHeya_GetDefaultResult(void)
{
	return 0;
}

u8 *TorebiHeya_GetExits(void)
{
	return Data_02008dac;
}

s32 TorebiHeya_SelectPlacements(void)
{
	if (Engine_GameFlagIsSet(0x950) != 0)
	{
		return (s32)Data_02009040;
	}
	return (s32)Data_02008e00;
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
static void SceneDialogue_AskBabiWasMissing(s32 subject)
{
	s32 msg;

	msg = (s32)MsgTorebiHeyaBabiShowedHimselfAtFinals;
	Engine_EventSetMessage(msg);
	Engine_EventOpenMessage(subject, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(msg + 1);
	}
	else
	{
		Engine_EventSetMessage(msg + 2);
	}
	Engine_EventShowMessage(subject, 0);
}
#endif

u8 *TorebiHeya_SelectEvents(void)
{
	if (Engine_GameFlagIsSet(0x950) != 0)
	{
		return Data_020099d0;
	}
	if (Engine_GameFlagIsSet(0x962) != 0)
	{
		return Data_02009670;
	}
	return Data_02009310;
}

static void TorebiHeya_StartWindowTransition(void)
{
	struct EventWork *state = gEventWork;

	state->start_transition = 0x201;
	state->transition_frames = 24;
	((void (*)(void))Engine_EventRequestExit)();
}

static void FieldScene_RunPrimarySequence(void)
{
	u32				   i;
	struct FieldActor *record;
	u8				  *p6;
	s32				   n;

	p6 = *(u8 **)&gEventWork;
	for (i = 8; i < 66; i++)
	{
		record = Object_GetById(i);
		if (record != 0)
		{
			record->motion_flags = 0;
		}
	}
	p6 += 0x16c;
	n = *(s16 *)p6 - 14;
	Engine_AudioPlayCue(158);
	Map_AnimateCells(Data_02009dcc[n].steps, Data_02009dcc[n].x, Data_02009dcc[n].y);
	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Object_GetById(0)->motion_flags = 0;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_EventRequestExit(*(s16 *)p6);
}

static void FieldScene_RunScene3b6SequenceA(void)
{
	s32 rec7;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiHeyWhatsThis);
	Engine_EventWait(40);
	rec7 = Engine_ObjectCreate(0x11c, 0x2580000, 0, 0x3380000);
	Engine_ActorSetSpriteFlags(rec7, 0);
	Object_SetMode(rec7, 6);
	Engine_EventWait(10);
	Object_SetMode(rec7, 1);
	Engine_EventWait(40);
	Engine_ObjectDispatchRelease(rec7);
	Engine_EventWait(2);
	Actor_ShowEmote(25, 0x100, 50);
	Actor_SetSpeed(25, 0x10000, 0x8000);
	Actor_WalkToAndWait(25, 0x258, 0x350);
	Actor_FaceDirection(25, 0xc000, 0);
	Engine_EventWait(40);
	Event_ShowMessage(25, 0);
	Engine_ActorRunRepeatedMotion(25, 2);
	Engine_EventWait(30);
	Actor_WalkToAndWait(25, 0x238, 0x350);
	Actor_FaceDirection(25, 0xc000, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(25, 0x108, 50);
	Engine_EventWait(20);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
	Engine_EventWait(20);
	Actor_FaceDirection(25, 0x3000, 0);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(25, 2);
	Engine_EventWait(20);
	Event_ShowMessage(25, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 50);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(25, 4);
	Engine_EventWait(20);
	Event_ShowMessage(25, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(25, 0x102, 50);
	Event_ShowMessage(25, 0);
	Actor_SetSpeed(25, 0x16666, 0xb333);
	Actor_WalkByAndWait(25, 16, 0);
	Actor_WalkByAndWait(25, 0, 32);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(25, 3);
	Engine_EventWait(20);
	Event_ShowMessage(25, 0);
	Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
	Engine_EventWait(20);
	Actor_SetSpeed(25, 0x1cccc, 0xe666);
	Actor_WalkByAndWait(25, 0, 48);
	Actor_SetPosition(25, 0, 0);
	Engine_EventEnd();
}

static void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiHeWontSailShipEven);
	Engine_EventWait(30);
	Engine_ActorJump(31, 4, 13);
	Engine_ActorJump(31, 4, 30);
	Engine_EventShowMessage(31, 0);
	Engine_EventWait(10);

	Engine_ActorShowEmote(32, 0x102, 50);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(32, 3);
	Engine_EventWait(30);
	Engine_EventShowMessage(32, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(33, 4);
	Engine_EventWait(20);
	Engine_EventShowMessage(33, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(31, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(31, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(32, 3);
	Engine_EventWait(30);
	Engine_EventEnd();
}

s32 Scene_InitFacingActors(void)
{
	u8 *record;
	s32 none;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	if (Engine_GameFlagIsSet(0x950) != 0)
	{
		Map_CopyCellAttributes(51, 47, 3, 1, 51, 45);
		record = Object_GetById(31);
		record[35] = none = 0;
		(*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
		record = Object_GetById(32);
		record[35] = none;
		(*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
		if (GameFlag_IsSet(0x8bc) != 0)
		{
			Actor_SetPosition(25, 0x2300000, 0x2a80000);
			Actor_FaceDirection(25, 0x8000, 0);
		}
		if (gGameState.entrance == 19)
		{
			if (GameFlag_IsSet(0x8bc) == 0)
			{
				Engine_GameFlagSet(0x8bc);
				Engine_EventOpenScreen();
				FieldScene_RunScene3b6SequenceA();
			}
		}
		if (gGameState.entrance == 16)
		{
			if (GameFlag_IsSet(0x300) == 0)
			{
				Engine_GameFlagSet(0x300);
				Engine_EventOpenScreen();
				FieldScene_RunActorsThirtyOneToThirtyThreeChoreography();
			}
		}
		if (Engine_GameFlagIsSet(0x8ab) != 0)
		{
			Engine_ActorSetPosition(35, 0, 0);
			Engine_ActorSetPosition(36, 0, 0);
		}
	}
	return 0;
}

static void EV21_TALK(s32 no)
{
	struct FieldActor *actor = Object_GetById(0);
	s32				   msg;
	if ((u16)((actor->facing + 0x2000) & ~0x3fff) == 0x8000)
	{
		Engine_ShopOpen(28, no);
	}
	else if (Engine_GameFlagIsSet(0x950) != 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiRightOneWell);
		Engine_EventShowMessage(no, 0);
	}
	else if (Engine_GameFlagIsSet(0x962) != 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiRequireLotHealing);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		msg = (s32)MsgTorebiFirstTimeTolbi;
		Engine_EventSetMessage(msg);
		Engine_EventOpenMessage(no, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(10);
			Engine_EventSetMessage(msg + 1);
		}
		else
		{
			Engine_EventSetMessage(msg + 2);
		}
		Engine_EventShowMessage(no, 0);
	}
}

static void EV19_TALK(s32 no)
{
	struct FieldActor *actor = Object_GetById(0);
	s32				   msg;
	if ((u16)((actor->facing + 0x2000) & ~0x3fff) == 0xc000)
	{
		Engine_ShopOpen(26, no);
	}
	else if (Engine_GameFlagIsSet(0x950) != 0)
	{
		msg = (s32)MsgTorebiComeWayKalay;
		Engine_EventSetMessage(msg);
		Engine_EventOpenMessage(no, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(10);
			Engine_EventSetMessage(msg + 1);
		}
		else
		{
			Engine_EventSetMessage(msg + 2);
		}
		Engine_EventShowMessage(no, 0);
	}
	else if (Engine_GameFlagIsSet(0x962) != 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiColossoFinalsFinally);
		Engine_EventShowMessage(no, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgTorebiLookStrongGo);
		Engine_EventShowMessage(no, 0);
		Engine_ActorShowEmote(no, 0x106, 0);
		Engine_EventWait(40);
		Engine_EventShowMessage(no, 0);
	}
}

static void EV20_TALK(s32 no)
{
	struct FieldActor *actor = Object_GetById(0);
	if ((u16)((actor->facing + 0x2000) & ~0x3fff) == 0xc000)
	{
		Engine_ShopOpen(27, no);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			Engine_EventSetMessage((s32)MsgTorebiWasntAbleWatch);
			Engine_EventShowMessage(no, 0);
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			Engine_EventSetMessage((s32)MsgTorebiMaybeCloseShop);
			Engine_EventShowMessage(no, 0);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgTorebiIfCanMakeNameFor);
			Engine_EventShowMessage(no, 0);
		}
	}
}

static void EV26_TALK(s32 subject)
{
	s32 message;

	Engine_EventBegin();

	message = (s32)MsgTorebiLookLikeWarrior;
	Engine_EventSetMessage(message);
	Engine_EventOpenMessage(subject, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(message + 1);
	}
	else
	{
		Engine_EventSetMessage(message + 2);
	}

	Engine_EventShowMessage(subject, 0);
	Engine_EventEnd();
}

static void EV27_TALK(s32 no)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiGrrrChefInBadMood);
	Engine_EventShowMessage(no, 0);
	Engine_EventEnd();
}

static void EV09_TALK(s32 subject)
{
	s32 msg;

	Engine_EventBegin();

	msg = (s32)MsgTorebiWantStay;
	Engine_EventSetMessage(msg);
	Engine_EventOpenMessage(subject, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(msg + 1);
	}
	else
	{
		Engine_EventSetMessage(msg + 2);
	}

	Engine_EventShowMessage(subject, 0);
	Engine_EventEnd();
}

static void EV25_TALK(s32 a0)
{
	s32 msg;

	Engine_EventBegin();
	if (GameFlag_IsSet(0x8bd) == 0)
	{
		msg = (s32)MsgTorebiHeeHeeLook;
		Engine_EventSetMessage(msg);
		Event_OpenMessage(a0, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			Engine_EventWait(10);
			Engine_EventSetMessage(msg + 1);
		}
		else
		{
			Engine_EventSetMessage(msg + 2);
		}
		Event_ShowMessage(a0, 0);
	}
	else
	{
		if (GameFlag_IsSet(0x8be) == 0)
		{
			GameFlag_Set(0x8be);
			Engine_EventSetMessage((s32)MsgTorebiHello);
			Event_ShowMessage(a0, 0);
			Engine_EventWait(10);
			Engine_ActorRunRepeatedMotion(a0, 2);
			Engine_EventWait(20);
		}
		Engine_EventSetMessage((s32)MsgTorebiHoHumFine);
		Event_ShowMessage(a0, 0);
	}
	Engine_EventEnd();
}

static void EV25_PSYNERGY(void)
{

	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x8BE) == 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiHehHehSheJustHid);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgTorebiThingFoundDefinitelySameAs);
	}
	Engine_EventShowMessage(25, 0);
	Engine_EventEnd();
}

static void EV31_TALK(s32 a0)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiGrrrScamWhyWontThey);
	Actor_ShowEmote(31, 0x103, 40);
	Event_ShowMessage(a0, 0);
	Engine_EventEnd();
}

static void EV35_TALK(s32 subject)
{
	s32 message;

	Engine_EventBegin();

	message = (s32)MsgTorebiWarriorRemember;
	Engine_EventSetMessage(message);
	Engine_EventOpenMessage(subject, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(message + 1);
	}
	else
	{
		Engine_EventSetMessage(message + 2);
	}

	Engine_EventShowMessage(subject, 0);
	Engine_EventEnd();
}

static void EV30_TALK(s32 no)
{

	struct FieldActor *actor = Object_GetById(0);
	if ((u16)((actor->facing + 0x2000) & ~0x3fff) == 0xc000)
	{
		Engine_SanctumOpen(no);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x950) != 0)
		{
			Engine_EventSetMessage((s32)MsgTorebiShipsArentGoing);
			Engine_EventShowMessage(no, 0);
		}
		else if (Engine_GameFlagIsSet(0x962) != 0)
		{
			Engine_EventSetMessage((s32)MsgTorebiMissFinalsTolbis);
			Engine_EventShowMessage(no, 0);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgTorebiWasteStuckHereWhenSuch);
			Engine_EventShowMessage(no, 0);
		}
	}
}

s32 Data_02009040_Data[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_ADD(SW_SCALE_X, -0x5000),
	SC_ADD(SW_SCALE_Y, 0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x2800),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, -0x5000),
	SC_ADD(SW_SCALE_Y, 0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x2800),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Data_02008bb4[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 160, 0, 216, FACING_NORTH, 0, 0, 0, 256, 256, 0 },
	{ 2, CONDITION_ALWAYS, 336, 0, 200, FACING_NORTH, 0, 248, 64, 488, 216, 0 },
	{ 3, CONDITION_ALWAYS, 544, 0, 200, FACING_NORTH, 0, 520, 48, 696, 216, 0 },
	{ 4, CONDITION_ALWAYS, 792, 0, 216, FACING_NORTH, 0, 688, 0, 960, 240, 0 },
	{ 5, CONDITION_ALWAYS, 544, 0, 424, FACING_NORTH, 0, 488, 264, 696, 448, 0 },
	{ 6, CONDITION_ALWAYS, 312, 0, 472, FACING_NORTH, 0, 208, 272, 456, 488, 0 },
	{ 8, CONDITION_ALWAYS, 824, 0, 528, FACING_NORTH, 0, 688, 248, 944, 568, 0 },
	{ 9, CONDITION_ALWAYS, 128, 0, 768, FACING_NORTH, 0, 0, 536, 304, 800, 0 },
	{ 10, CONDITION_ALWAYS, 56, 0, 608, FACING_NORTH, 0, 0, 536, 304, 800, 0 },
#if !EDITION_INTERNATIONAL
	{ 11, CONDITION_ALWAYS, 464, 0, 624, FACING_SOUTH, 0, 320, 568, 616, 736, 0 },
	{ 12, CONDITION_ALWAYS, 392, 0, 728, FACING_NORTH, 0, 320, 568, 616, 736, 0 },
	{ 13, CONDITION_ALWAYS, 536, 0, 728, FACING_NORTH, 0, 320, 568, 616, 736, 0 },
	{ 14, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 320, 568, 616, 736, 0 },
	{ 15, CONDITION_ALWAYS, 536, 0, 680, FACING_SOUTH, 0, 320, 568, 616, 736, 0 },
	{ 16, CONDITION_ALWAYS, 808, 0, 664, FACING_SOUTH, 0, 704, 608, 944, 768, 0 },
	{ 17, CONDITION_ALWAYS, 840, 0, 824, FACING_SOUTH, 0, 704, 768, 944, 928, 0 },
	{ 18, CONDITION_ALWAYS, 440, 0, 920, FACING_NORTH, 0, 288, 768, 528, 928, 0 },
	{ 19, CONDITION_ALWAYS, 584, 0, 920, FACING_NORTH, 0, 496, 768, 736, 928, 0 },
#else
	{ 11, CONDITION_ALWAYS, 464, 0, 624, FACING_SOUTH, 0, 320, 568, 616, 744, 0 },
	{ 12, CONDITION_ALWAYS, 392, 0, 728, FACING_NORTH, 0, 320, 568, 616, 744, 0 },
	{ 13, CONDITION_ALWAYS, 536, 0, 728, FACING_NORTH, 0, 320, 568, 616, 744, 0 },
	{ 14, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 320, 568, 616, 744, 0 },
	{ 15, CONDITION_ALWAYS, 536, 0, 680, FACING_SOUTH, 0, 320, 568, 616, 744, 0 },
	{ 16, CONDITION_ALWAYS, 808, 0, 664, FACING_SOUTH, 0, 704, 608, 952, 768, 0 },
	{ 17, CONDITION_ALWAYS, 840, 0, 824, FACING_SOUTH, 0, 704, 768, 952, 928, 0 },
	{ 18, CONDITION_ALWAYS, 440, 0, 920, FACING_NORTH, 0, 288, 768, 528, 936, 0 },
	{ 19, CONDITION_ALWAYS, 584, 0, 920, FACING_NORTH, 0, 496, 768, 736, 936, 0 },
#endif
	{ 30, CONDITION_ALWAYS, 824, 0, 352, FACING_NORTH, 0, 688, 248, 944, 568, 0 },
	{ SCENE_TABLE_END },
};

u32 Data_02008dac[] = {
	SCENE_EXITS(136),
	SCENE_EXIT(1, 135, 2),
	SCENE_EXIT(2, 135, 3),
	SCENE_EXIT(3, 135, 4),
	SCENE_EXIT(4, 135, 5),
	SCENE_EXIT(5, 135, 6),
	SCENE_EXIT(6, 135, 7),
	SCENE_EXIT(7, 135, 8),
	SCENE_EXIT(8, 135, 9),
	SCENE_EXIT(9, 135, 10),
	SCENE_EXIT(10, 136, 11),
	SCENE_EXIT(11, 136, 10),
	SCENE_EXIT(12, 136, 17),
	SCENE_EXIT(13, 136, 16),
	SCENE_EXIT(14, 136, 18),
	SCENE_EXIT(15, 136, 19),
	SCENE_EXIT(16, 136, 13),
	SCENE_EXIT(17, 136, 12),
	SCENE_EXIT(18, 136, 14),
	SCENE_EXIT(19, 136, 15),
	SCENE_EXITS_END,
};

struct ScenePlacement Data_02008e00[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(104), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(144), 0, PIXELS(104), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(832), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(584), 0, PIXELS(848), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x84, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(230), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x43, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement Data_02009040[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(392), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(520), 0, PIXELS(520), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x76, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x77, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x80, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(112), 0, PIXELS(600), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(848), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x84, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x94, CONDITION_ALWAYS, (s32)Data_02009040_Data,
		PIXELS(230), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x43, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(712), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(816), 0, PIXELS(712), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(688), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(672), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(864), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Data_02009310[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTolbiIsPackedWithTouristsDuring) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaICouldntSleepAtAllLast) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheCrowdsAtTheColosseumMade) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWePaidGoodMoneyToRent) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIReallyWantToSeeColosso) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWahhhIWantToSeeColosso) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWonderIfTheyReallyCant) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBabiWasntAtColossoAccordingTo) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWereGoingToMakeSomeMoney) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIHeardBabiIsUsingThe) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWereFullyBookedWithReservationsDuring) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaColossoIsTheBusiestTimeOf) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWereSoBusyRunningTheInn) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaATourGroupCameFromKalay) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWeArentGoingToGetTo) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImNotVeryInterestedInColosso) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOhNoWeBarelyMadeIt) },
	{ EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfWedGottenHereJustA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOnceColossoStartsWereGoingTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaItsTooLateToStartLooking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfICouldHaveSleptLast) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHuhUhCrowdsAreADefinite) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWeGetToStayHereUntil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWasStubbornISaidIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIKeepOnCryingButDaddy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaItsStrangeThatBabisSoldiersAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaItsStrangeBabiLovesColossoBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHeJustTakesACutOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaNamingTheLighthouseHeBuiltAfter) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIGuessINeverWouldHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheyHaveSomeNiceThingsFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBabiChoosesColossosVictorsToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheTourGroupIsFeelingCramped) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWeCantHandleTooManyMore) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImGoingToGoToThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThereArentEnoughBedsForThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheInnkeeperToldMeToCook) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIHaveAHeadacheJustThinking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaEvenThoughICantEnterColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfWeCouldHaveEnteredColosso) },
	{ EVENT_TALK, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWonderHowTheTryoutsAre) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf9a, FIND_ITEM(ITEM_VIAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaShesMakingTheWorldRenownedTolbi)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaHeIsBoilingThePastaIt)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaItsTolbiSeafoodPastaWithLots)) },
	{ SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaRobinCheckedTheSignInnFull)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Data_02009670[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSoColossoHasAlreadyReachedThe) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheFestivalScheduleWasDelayedFor) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThatsStrangeIWonderWhatHappened) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIJustSawBabiTheTowns) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHereIAmInTolbiFor) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImGoingToWatchTheFinals) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHoorayWereGoingToColossoNow) },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_AskBabiWasMissing) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBabiDecidedShowUpForFinals) },
#else
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWellIGuessSoSinceHes) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHuhThenTheyFoundHimIn) },
#endif
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBabiApparentlyHasSomeReasonFor) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIHeardBabiIsSearchingFor) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOnceTheFinalsBeginEveryoneTries) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThisYearImNotLettingAnyone) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheWifeHasACloseEye) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHmmIThoughtAllMenLoved) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheMomentIHearTheFinals) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSoColossoIsEndingSoonIts) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheColossoFinalsAreStartingSoon) },
	{ EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaMaybeIllDoSomeResearchOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaAreWeMakingMoreMoneyNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSoTheyDidExtendTheFestival) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaICantAffordToStayHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheAirInTheColosseumIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfSomebodyElseWouldWatchThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaColossoIsAlreadyAtTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSinceWeDidntGetToSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSoWhatWereBabisSoldiersDoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaMaybeWatchingTheTrialsIsToo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBeforeHeStartedTheLighthouseBabi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaBabiHasBeenMakingStrangeReferences) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheTouristsAllBuyWeaponsAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaYouCouldSayTheWarriorsIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfMomFindsOutIWent) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImTheOneWhoReallyWants) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOnceThingsSettleDownImSure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfTheMasterSlipsOutThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWeWomenAreTheOnlyOnes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHasntTheMasterGoneToColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIDontHaveAnyParticularInterest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWantToSeeTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheTrialBattlesWerentQuiteLike) },
	{ EVENT_TALK, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfYoureNotInterestedInThe) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf9a, FIND_ITEM(ITEM_VIAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaShesMakingTheWorldRenownedTolbi)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaHeIsBoilingThePastaIt)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaItsTolbiSeafoodPastaWithLots)) },
	{ SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaRobinCheckedTheSignInnFull)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Data_020099d0[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaNowThatColossoIsOverWeve) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaWeHadALotOfGuests) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIRentedMyHouseToA) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheTouristsAreAllInA) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWasSoMovedByThis) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThatLittleGuyJumpedStraightInto) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaDidYouKnowMtAlephErupted) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaMaybeAlephsEruptionCausedThoseSea) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheWarriorsSaidTheyWouldRush) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThereWontBeManyCustomersUntil) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIThoughtWedHaveSomeFree) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheSpectatorsFromTheEastCant) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaItsBusyAllDayEveryDay) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_TALK) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_TALK) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TALK) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWentToThePortBut) },
	{ EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaColossoHasEndedButWeStill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSighMakingTheBedsHasWorn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIdLoveAVacationButWhere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOnceTheShipsStartSailingAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIHeardOfACaveThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheWarriorsWhoFoughtInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaDaringAndBraveryAreNiceAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheSparklingStonesThatRainedFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIllBetTheMonstersWereFrightened) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaItBothersMeABitThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThereArentManyMerchantsAtThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaEvenWithoutTheShipsICould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaDidAllTheWarriorsWhoFought) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImGonnaWatchTheFinalsNext) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaSinceTheShipsArentSailingThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHeeHeeItsGreatTheInns) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaEveryoneIsWorkingOnBabiLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaGrrrIDidntGetToSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaHeyChefSoWhatIfYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaITriedGoingToTheEastern) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaYoudThinkYouCouldGetThrough) },
	{ EVENT_TALK, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaYouAllGotToSeeColosso) },
	{ EVENT_TALK, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EV31_TALK) },
	{ EVENT_TALK, 32, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaOnceThisGuyGetsMadTheres) },
	{ EVENT_TALK, 33, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIWonderIfThisReallyIs) },
	{ EVENT_TALK, 34, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTolbiGetsSoBoringOnceThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaYoureSayingIfWeRowWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaTheCrewIsSayingThatIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaImSickOfStayingAtThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIfMomHadJustBoughtMe) },
	{ EVENT_TALK, 35, CONDITION_ALWAYS, EVENT_SCRIPT(EV35_TALK) },
	{ EVENT_TALK, 36, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaIGotToSeeColossoSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaThatVoyageAcrossTheSeaWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgTorebiHeyaNoMatterWhatHappensImComing) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf9a, FIND_ITEM(ITEM_VIAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaShesMakingTheWorldRenownedTolbi)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaHeIsBoilingThePastaIt)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaItsTolbiSeafoodPastaWithLots)) },
	{ SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTorebiHeyaRobinCheckedTheSignInnFull)) },
	{ SEARCH(SEARCH_BARREL), 102, 0x8bd, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

u16 Data_02009dccData[] = {
	MAP_CELL_STEP(2, 123, 1, 2, 6), MAP_CELL_STEP(4, 123, 1, 2, 6), MAP_CELL_STEPS_END,
};

struct MapCellAnim Data_02009dcc[] = {
	{ Data_02009dccData, 24, 100 },
	{ Data_02009dccData, 33, 100 },
	{ Data_02009dccData, 50, 99 },
	{ Data_02009dccData, 52, 109 },
};
