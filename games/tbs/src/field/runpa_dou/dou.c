#include "game_state.h"
#include "scene_ids.h"
#include "field_event.h"
#include "unit/field/runpa_dou/dou.h"
#include "unit/battle/effect/object.h"
#include "cave.h"
#include "text/msg_ids.h"

// 前方宣言
static void Reunion_Converse(void);
static void WagonChoice_Run(void);
static u8 Leader_AnswersYes(void);
static u8 Gerald_AsksIfNotRiding(void);
static u8 Gerald_AsksAboutUnfinishedBusiness(void);
static u8 Party_ConfirmsStaying(void);
static u8 Bunza_AsksAboutUnfinishedBusiness(void);
static u8 Gerald_ChecksNothingLeftToDo(void);
static u8 Mia_AsksIfRidingAfterAll(void);
static u8 Bunza_CannotWait(void);
static u8 Gerald_AsksAboutThingsToDo(void);
static void Party_StaysBehind(void);
static void Party_RidesWagon(void);

const struct SceneEntrance *Scene_GetEntrances(void)
{
	return gCaveEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return NULL;
}

const u32 *Scene_GetExits(void)
{
	return gCaveExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_RunpaDou)
	{
		return gCavePlacements;
	}
	return gCaveNoPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return gCaveEvents;
}

static void EV08_PSYNERGY(void)
{
	struct FieldActor *puddle;

	puddle = Object_GetById(ACTOR_HIDDEN_PUDDLE);
	if (puddle != NULL)
	{
		Engine_ActorSetSpriteFlags(puddle, 0);
	}
}

static void EV09_PSYNERGY(void)
{
	struct FieldActor *puddle;

	puddle = Actor_Get(ACTOR_SOUTH_PUDDLE);
	if (puddle != NULL)
	{
		puddle->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
		puddle->motion_flags = 0;
	}
	Map_CopyCellAttributes(7, 32, 1, 1, 8, 32);
	GameFlag_Set(FLAG_CAVE_SOUTH_PILLAR);
}

static void Gate_DrawPropped(void)
{
	struct FieldActor *pillar;

	pillar = Actor_Get(ACTOR_GATE_PUDDLE);
	Engine_ActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
	if (pillar != NULL)
	{
		Engine_ActorSetSpriteFlags(pillar, 0);
		pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
	}
	Map_CopyCells(41, 87, 2, 5, 21, 59);
	Engine_TaskWait(4);
	Map_CopyCells(3, 93, 1, 1, 24, 62);
	Map_CopyCells(1, 94, 1, 1, 21, 55);
	Map_CopyCells(43, 87, 2, 5, 21, 58);
	Engine_TaskWait(4);
	Map_CopyCells(41, 87, 2, 5, 21, 58);
	Engine_TaskWait(4);
	Engine_TaskWait(4);
	Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
	Map_CopyCellAttributes(21, 11, 1, 1, 22, 15);
	Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

static void EV10_PSYNERGY(void)
{
	struct FieldActor *pillar;

	pillar = Actor_Get(ACTOR_GATE_PUDDLE);
	GameFlag_Set(FLAG_CAVE_GATE_PROPPED);
	if (pillar != NULL)
	{
		Engine_ActorSetSpriteFlags(pillar, 0);
		pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
	}
	if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0)
	{
		Event_PlayCue(SOUND_GATE_MOVE);
		Gate_DrawPropped();
		Event_PlayCue(SOUND_PUZZLE_SOLVED);
		GameFlag_Set(FLAG_CAVE_GATE_RAISED);
	}
}

static void EV11_PSYNERGY(void)
{
	Engine_GameFlagSet(FLAG_CAVE_NORTH_PILLAR);
}

static void Gate_Lower(void)
{
	if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) != 0)
	{
		Map_CopyCells(41, 86, 2, 6, 21, 57);
		Engine_TaskWait(4);
		Map_CopyCells(43, 86, 2, 6, 21, 57);
		Engine_TaskWait(4);
		Map_CopyCells(41, 86, 2, 6, 21, 58);
		Engine_TaskWait(4);
		Map_CopyCells(43, 86, 2, 6, 21, 58);
		Engine_TaskWait(4);
	}
	Map_CopyCells(2, 93, 1, 1, 24, 62);
	Map_CopyCells(2, 94, 1, 1, 21, 55);
	Map_CopyCells(41, 86, 2, 6, 21, 59);
	Engine_TaskWait(4);
	Map_CopyCells(1, 93, 1, 1, 24, 62);
	Map_CopyCells(3, 94, 1, 1, 21, 55);
	Map_CopyCells(43, 86, 2, 6, 21, 59);
	Engine_TaskWait(4);
	Engine_ActorSetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
	Map_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

static void EV03_SEARCH(void)
{
	if (GameFlag_IsSet(FLAG_CAVE_GATE_LOWERED) == 0)
	{
		if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0)
		{
			Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
			Event_PlayCue(SOUND_GATE_MOVE);
			Gate_Lower();
			GameFlag_Set(FLAG_CAVE_GATE_LOWERED);
			GameFlag_Clear(FLAG_CAVE_GATE_RAISED);
		}
	}
}

static void Gate_Raise(void)
{
	Map_CopyCells(41, 87, 2, 5, 21, 59);
	Engine_TaskWait(4);
	Map_CopyCells(2, 93, 1, 1, 24, 62);
	Map_CopyCells(2, 94, 1, 1, 21, 55);
	Map_CopyCells(43, 87, 2, 5, 21, 58);
	Engine_TaskWait(4);
	Map_CopyCells(3, 93, 1, 1, 24, 62);
	Map_CopyCells(1, 94, 1, 1, 21, 55);
	Map_CopyCells(41, 87, 2, 5, 21, 58);
	Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
	Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

static void EV04_SEARCH(void)
{
	if (GameFlag_IsSet(FLAG_CAVE_GATE_PROPPED) == 0)
	{
		if (GameFlag_IsSet(FLAG_CAVE_GATE_RAISED) == 0)
		{
			Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
			Event_PlayCue(SOUND_GATE_MOVE);
			Gate_Raise();
			GameFlag_Set(FLAG_CAVE_GATE_RAISED);
			GameFlag_Clear(FLAG_CAVE_GATE_LOWERED);
		}
	}
}

static void EV10_TOUCH(void)
{
	struct FieldActor *leader;
	s32				   sighting;
	s32				   recognition;

	if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0)
	{
		GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
		Engine_EventBegin();
		Actor_SetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
		Actor_SetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
		Actor_WalkTo(ACTOR_BUNZA, 184, 400);
		Engine_ActorWaitForMove(ACTOR_BUNZA);
		Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
		Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Engine_EventWait(30);
		Camera_SetSpeed(0x8000, 0x1000);
		Camera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
		leader = Actor_Get(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			Actor_SetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
		}
		Actor_SetSpeed(ACTOR_HAMMET, 0x14ccc, 0xa666);
		Actor_WalkTo(ACTOR_HAMMET, 168, 464);
		Engine_ActorWaitForMove(ACTOR_HAMMET);
		Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
		leader = Actor_Get(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
		}
		Actor_SetSpeed(IWAN, 0x14ccc, 0xa666);
		Actor_WalkTo(IWAN, 152, 488);
		Engine_ActorWaitForMove(IWAN);
		Actor_FaceDirection(IWAN, FACING_NORTH, 0);
		leader = Actor_Get(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
		}
		Actor_SetSpeed(MEARI, 0x14ccc, 0xa666);
		Actor_WalkTo(MEARI, 168, 488);
		Engine_ActorWaitForMove(MEARI);
		Actor_FaceDirection(MEARI, FACING_NORTH, 0);
		leader = Actor_Get(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
		}
		Actor_SetSpeed(GERALD, 0x14ccc, 0xa666);
		Actor_WalkTo(GERALD, 184, 488);
		Engine_ActorWaitForMove(GERALD);
		Actor_FaceDirection(GERALD, FACING_NORTH, 0);
		Engine_EventWait(30);

		Engine_ActorRunRepeatedMotion(GERALD, 1);
		sighting = (s32)MsgRunpaThinkSawSomeone;
		Engine_EventSetMessage(sighting + SIGHTING_GERALD_SAW_SOMEONE);
		Event_ShowMessage(GERALD, 0);
		Engine_EventWait(30);
		Engine_ActorSetAnimationAndWait(MEARI, ANIM_NOD);
		Engine_EventWait(10);
		Engine_EventSetMessage(sighting + SIGHTING_MIA_SAW_SOMETHING);
		Event_ShowMessage(MEARI, 0);
		Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 2, 70);
		Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
		Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
		Engine_EventSetMessage(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
		Event_OpenMessage(IWAN, 0);
		Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
		Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(30);
		if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
		{
			Engine_EventSetMessage(sighting + SIGHTING_GERALD_WILL_FIGHT);
			Event_ShowMessage(GERALD, 0);
		}
		else
		{
			Engine_EventSetMessage(sighting + SIGHTING_GERALD_ASKS_WHAT_ELSE);
			Event_ShowMessage(GERALD, 0);
		}

		Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
		recognition = (s32)MsgRunpaMoment;
		Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_CALLS_OUT);
		Event_ShowMessage(ACTOR_HAMMET, 0);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Engine_ActorStartRepeatedMotion(IWAN, 2);
		Engine_ActorStartRepeatedMotion(MEARI, 2);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		Actor_FaceDirection(GERALD, FACING_NORTH, 0);
		Actor_FaceDirection(IWAN, FACING_NORTH, 0);
		Actor_FaceDirection(MEARI, FACING_NORTH, 0);
		Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
		Actor_WalkBy(ACTOR_HAMMET, 0, -16);
		Engine_ActorWaitForMove(ACTOR_HAMMET);
		Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
		Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
		Event_ShowMessage(ACTOR_HAMMET, 0);
		Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 65);
		Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
		Event_ShowMessage(ACTOR_BUNZA, 0);
		Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
		Engine_EventWait(80);
		Actor_SetSpeed(ACTOR_BUNZA, 0x6666, 0x3333);
		Actor_WalkBy(ACTOR_BUNZA, -13, 0);
		Engine_ActorWaitForMove(ACTOR_BUNZA);
		Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
		Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
		Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
		Event_ShowMessage(ACTOR_BUNZA, 0);
		Actor_WalkTo(ACTOR_BUNZA, 168, 432);
		Engine_EventWait(40);
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(20);
		Engine_EventEnd();
		Reunion_Converse();
	}
}

static void Reunion_Converse(void)
{
	s32 reunion;
	s32 plan;

	Engine_EventBegin();
	Actor_SetPosition(GERALD, PIXELS(200), PIXELS(544));
	Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(184), PIXELS(544));
	Actor_SetPosition(MEARI, PIXELS(168), PIXELS(544));
	Actor_SetPosition(IWAN, PIXELS(212), PIXELS(528));
	Actor_SetPosition(ACTOR_HAMMET, PIXELS(200), PIXELS(512));
	Actor_SetPosition(ACTOR_BUNZA, PIXELS(168), PIXELS(512));
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
	Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);

	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventWait(30);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
	reunion = (s32)MsgRunpaDodonpaNeverIntention;
	Engine_EventSetMessage(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_CREDITS_IVAN);
	Event_ShowMessage(ACTOR_HAMMET, 0);
	Actor_FaceActor(IWAN, ACTOR_HAMMET, 0);
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(40);
	Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_LEADER);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 7, 80);
	Engine_EventSetMessage(reunion + REUNION_GERALD_QUESTIONS_IVAN);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 0, 70);
	Actor_FaceActor(IWAN, GERALD, 0);
	Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_EVERYONE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(60);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_PARTY);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_EventWait(120);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 8, 80);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_IVAN);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Actor_FaceActor(IWAN, ACTOR_HAMMET, 0);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 8, 60);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventWait(30);

	Actor_ShowEmote(MEARI, EMOTE_IN_FRONT | 1, 40);
	Actor_FaceActor(MEARI, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(reunion + REUNION_MIA_PRAISES_BUNZA);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 60);
	Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_PRAISES_BUNZA);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 70);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
	Engine_EventWait(20);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorFaceEachOther(ACTOR_HAMMET, IWAN, 0);
	Engine_EventWait(60);
	Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
	Actor_FaceActor(IWAN, ACTOR_BUNZA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	Actor_FaceActor(GERALD, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_WHY_BUNZA_CAME);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	Actor_FaceActor(ACTOR_BUNZA, GERALD, 0);
	Engine_EventWait(20);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_RECALLS_ADVICE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_FaceActor(MEARI, ACTOR_BUNZA, 0);
	Actor_ShowEmote(MEARI, EMOTE_IN_FRONT | 1, 70);
	Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(40);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_MEANS_WISDOM);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 70);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_APPEARANCES);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_SELLING);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 70);
	Engine_ActorStartRepeatedMotion(IWAN, 1);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Actor_FaceActor(ACTOR_BUNZA, IWAN, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_IVAN_ON_FATE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_ENTRY);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_WAS_REFUSED);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Actor_FaceActor(ACTOR_BUNZA, MEARI, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorFaceEachOther(IWAN, ACTOR_HAMMET, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
	Actor_FaceActor(IWAN, ACTOR_BUNZA, 0);
	Engine_EventWait(20);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_LINKS_COMMOTION);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_SetAttachedEffect(ACTOR_BUNZA, EMOTE_IN_FRONT | 2);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_HAD_TO_KNOW);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_FaceActor(GERALD, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_ABOUT_CAVE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 5, 70);
	Engine_ActorSetAnimationAndWait(MEARI, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(reunion + REUNION_MIA_ON_GATE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_HID);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
	Engine_EventSetMessage(reunion + REUNION_HAMMET_STARTLED_BUNZA);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Engine_EventWait(30);
	Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_IS_GLAD);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Engine_EventWait(30);

	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 0, 60);
	Actor_SetSpeed(ACTOR_BUNZA, 0x18000, 0xc000);
	Actor_WalkTo(ACTOR_BUNZA, 144, 528);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Actor_WalkTo(ACTOR_BUNZA, 168, 560);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_WalkTo(ACTOR_BUNZA, 144, 528);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Actor_WalkTo(ACTOR_BUNZA, 168, 488);
	Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_WHY);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventWait(20);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Engine_EventWait(20);
	Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 2, 60);
	Engine_EventSetMessage(reunion + REUNION_BUNZA_FEARS_CAPTURE);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventSetMessage(reunion + REUNION_MIA_URGES_ESCAPE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 2, 60);
	Engine_EventSetMessage(reunion + REUNION_IVAN_WANTS_STEALTH);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 2, 40);
	Engine_EventSetMessage(reunion + REUNION_GERALD_ASKS_TO_FIGHT);
	Event_OpenMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
	{
		Engine_EventSetMessage(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
		Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	}
	else
	{
		Engine_EventSetMessage(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
		Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	}

	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
	Actor_FaceActor(GERALD, ACTOR_BUNZA, 0);
	Actor_FaceActor(MEARI, ACTOR_BUNZA, 0);
	Actor_FaceActor(IWAN, ACTOR_BUNZA, 0);
	Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
	Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 1, 60);
	plan = (s32)MsgRunpaBunza;
	Engine_EventSetMessage(plan + PLAN_HAMMET_ASKS_PLAN);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | 8, 60);
	Engine_EventSetMessage(plan + PLAN_BUNZA_HAS_WAGON);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_ShowEmote(MEARI, EMOTE_IN_FRONT | 1, 60);
	Engine_EventSetMessage(plan + PLAN_MIA_ASKS_ABOUT_WAGON);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MEARI, 0);
	Actor_FaceActor(ACTOR_BUNZA, MEARI, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventSetMessage(plan + PLAN_BUNZA_OFFERS_RIDE);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Actor_FaceActor(IWAN, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(plan + PLAN_IVAN_DOUBTS_WAGON);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IWAN, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(plan + PLAN_BUNZA_REASSURES);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
	Engine_EventWait(60);
	Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
	Actor_FaceActor(GERALD, ACTOR_BUNZA, 0);
	Engine_EventSetMessage(plan + PLAN_GERALD_AGREES);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GERALD, 0);
	Actor_FaceActor(ACTOR_BUNZA, GERALD, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(20);
	Engine_EventSetMessage(plan + PLAN_BUNZA_IS_UNSUSPECTED);
	Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
	Engine_ActorFaceEachOther(ACTOR_HAMMET, IWAN, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
	Engine_ActorSetAnimation(IWAN, ANIM_NOD);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(60);
	WagonChoice_Run();
}

static void WagonChoice_Run(void)
{
	s32 wagon;
	s32 insisted;
	s32 konran;

	wagon = (s32)MsgRunpaLetsTakeWagon;
	Engine_EventSetMessage(wagon + WAGON_BUNZA_LEADS_THE_WAY);
	Engine_EventShowMessage(ACTOR_BUNZA, 0);
	Engine_ActorFaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_EventSetMessage(wagon + WAGON_GERALD_ASKS_TO_RIDE);
	Engine_EventOpenMessage(GERALD, 0);
	Engine_ActorFaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
	Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
	if (Leader_AnswersYes())
	{
	ask_about_business:
		if (!Bunza_AsksAboutUnfinishedBusiness())
		{
			goto ride;
		}
		insisted = FALSE;
		if (!Gerald_AsksAboutThingsToDo())
		{
		insist_nothing_left:
			insisted = TRUE;
		check_nothing_left:
			Gerald_ChecksNothingLeftToDo();
			if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0)
			{
				goto ride;
			}
		}
		if (Bunza_CannotWait())
		{
			goto stay;
		}
		if (!insisted)
		{
			goto stay;
		}
		goto check_nothing_left;
	}
	else
	{
		if (Gerald_AsksIfNotRiding())
		{
			if (Gerald_AsksAboutUnfinishedBusiness())
			{
				goto stay;
			}
			goto insist_nothing_left;
		}
		if (Mia_AsksIfRidingAfterAll())
		{
			goto ask_about_business;
		}
		konran = (s32)MsgRunpaTotallyConfusedChanged;
		Engine_EventSetMessage(konran + CONFUSION_IVAN_IS_CONFUSED);
		Engine_EventShowMessage(IWAN, 0);
		Engine_EventSetMessage(konran + CONFUSION_GERALD_ASKS_AGAIN);
		Engine_EventOpenMessage(GERALD, 0);
		goto ask_to_ride;
	ride:
		Party_RidesWagon();
		goto done;
	}
stay:
	Party_ConfirmsStaying();
	Party_StaysBehind();
done:;
}

static u8 Leader_AnswersYes(void)
{
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Gerald_AsksIfNotRiding(void)
{
	Engine_EventSetMessage((s32)MsgRunpaGeraldAsksIfNotRiding);
	Engine_EventOpenMessage(GERALD, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Gerald_AsksAboutUnfinishedBusiness(void)
{
	Engine_EventSetMessage((s32)MsgRunpaGeraldAsksAboutUnfinishedBusiness);
	Engine_EventOpenMessage(GERALD, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Party_ConfirmsStaying(void)
{
	return TRUE;
}

static u8 Bunza_AsksAboutUnfinishedBusiness(void)
{
	Engine_EventSetMessage((s32)MsgRunpaBunzaAsksAboutUnfinishedBusiness);
	Engine_EventOpenMessage(ACTOR_BUNZA, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Gerald_ChecksNothingLeftToDo(void)
{
	Engine_EventSetMessage((s32)MsgRunpaGeraldChecksNothingLeft);
	Engine_EventOpenMessage(GERALD, 0);
	return TRUE;
}

static u8 Mia_AsksIfRidingAfterAll(void)
{
	Engine_EventSetMessage((s32)MsgRunpaMiaAsksIfRidingAfter);
	Engine_EventOpenMessage(MEARI, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Bunza_CannotWait(void)
{
	s32 warning;

	Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
	Actor_ShowEmote(IWAN, EMOTE_IN_FRONT | 2, 60);
	warning = (s32)MsgRunpaMeaningWontRide;
	Engine_EventSetMessage(warning + WARNING_IVAN_ASKS_IF_STAYING);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(warning + WARNING_BUNZA_CANNOT_WAIT);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Actor_ShowEmote(MEARI, EMOTE_IN_FRONT | 2, 60);
	Engine_EventSetMessage(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
	Event_OpenMessage(MEARI, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static u8 Gerald_AsksAboutThingsToDo(void)
{
	Engine_EventSetMessage((s32)MsgRunpaGeraldAsksAboutThingsTo);
	Engine_EventOpenMessage(GERALD, 0);
	return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

static void Party_StaysBehind(void)
{
	struct FieldActor *leader;
	s32				   farewell;

	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 5, 60);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	farewell = (s32)MsgRunpaInsistStick;
	Engine_EventSetMessage(farewell + FAREWELL_GERALD_STAYS);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Engine_EventSetMessage(farewell + FAREWELL_MIA_STAYS);
	Event_ShowMessage(MEARI, 0);
	Actor_FaceActor(IWAN, ACTOR_HAMMET, 0);
	Engine_EventWait(60);
	Engine_EventSetMessage(farewell + FAREWELL_IVAN_STAYS);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceActor(ACTOR_HAMMET, IWAN, 0);
	Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 5, 70);
	Engine_EventSetMessage(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
	Event_ShowMessage(ACTOR_HAMMET, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	Engine_EventSetMessage(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(IWAN, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Engine_EventWait(60);
	Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
	Engine_ActorWaitForMove(ACTOR_HAMMET);
	Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(IWAN, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Actor_WalkTo(ACTOR_BUNZA, 156, 528);
	Engine_EventWait(20);
	Actor_WalkTo(ACTOR_HAMMET, 164, 528);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Actor_WalkTo(ACTOR_BUNZA, 168, 640);
	Engine_ActorWaitForMove(ACTOR_HAMMET);
	Actor_WalkTo(ACTOR_HAMMET, 168, 640);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Engine_EventWait(60);
	Actor_SetPosition(ACTOR_HAMMET, 0, 0);
	Actor_SetPosition(ACTOR_BUNZA, 0, 0);
	Engine_EventWait(110);

	Engine_EventSetMessage(farewell + FAREWELL_GERALD_SIGHS);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorSetAnimationAndWait(MEARI, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
	Event_ShowMessage(MEARI, 0);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventSetMessage(farewell + FAREWELL_IVAN_REASSURES);
	Event_ShowMessage(IWAN, 0);
	Engine_EventWait(140);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_EventSetMessage(farewell + FAREWELL_GERALD_MOVES_ON);
	Event_ShowMessage(GERALD, 0);

	Engine_ActorSetAnimation(GERALD, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorSetAnimation(IWAN, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(IWAN, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(IWAN);
	Actor_SetPosition(IWAN, 0, 0);
	Engine_ActorSetAnimation(MEARI, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(MEARI, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(MEARI);
	Actor_SetPosition(MEARI, 0, 0);
	Engine_EventWait(30);
	Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
	Engine_CameraWaitForMove();
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	GameFlag_Set(FLAG_PARTY_STAYED_IN_LUNPA);
}

static void Party_RidesWagon(void)
{
	struct FieldActor *leader;
	s32				   departure;

	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	departure = (s32)MsgRunpaDontUnfinishedBusiness;
	Engine_EventSetMessage(departure + DEPARTURE_GERALD_HEADS_FOR_KALAY);
	Event_ShowMessage(GERALD, 0);
	Engine_ActorSetAnimationAndWait(IWAN, ANIM_NOD);
	Engine_EventSetMessage(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceActor(ACTOR_HAMMET, IWAN, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Engine_EventWait(20);
	Engine_EventSetMessage(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
	Event_ShowMessage(ACTOR_HAMMET, 0);
	Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	Engine_EventWait(30);
	Engine_EventSetMessage(departure + DEPARTURE_BUNZA_SETS_OFF);
	Event_ShowMessage(ACTOR_BUNZA, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(IWAN, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Engine_EventWait(80);
	Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
	Engine_ActorWaitForMove(ACTOR_HAMMET);
	Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimation(GERALD, ANIM_NOD);
	Engine_ActorSetAnimation(IWAN, ANIM_NOD);
	Engine_ActorSetAnimation(MEARI, ANIM_NOD);
	Actor_WalkTo(ACTOR_BUNZA, 152, 528);
	Engine_EventWait(20);
	Actor_WalkTo(ACTOR_HAMMET, 160, 528);
	Engine_ActorWaitForMove(ACTOR_BUNZA);
	Actor_WalkTo(ACTOR_BUNZA, 168, 640);
	Engine_ActorWaitForMove(ACTOR_HAMMET);
	Actor_WalkTo(ACTOR_HAMMET, 168, 640);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Engine_EventWait(200);
	Actor_SetPosition(ACTOR_HAMMET, 0, 0);
	Actor_SetPosition(ACTOR_BUNZA, 0, 0);

	Engine_ActorSetAnimation(GERALD, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorSetAnimation(IWAN, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(IWAN, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(IWAN);
	Actor_SetPosition(IWAN, 0, 0);
	Engine_ActorSetAnimation(MEARI, ANIM_WALK);
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(MEARI, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(MEARI);
	Actor_SetPosition(MEARI, 0, 0);
	Engine_EventWait(30);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 640);
	Engine_EventWait(60);
	Engine_EventCloseScreen();
	Engine_EventRequestExit(CAVE_EXIT_BY_WAGON);
}

s32 Scene_Initialize(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_RunpaDou)
	{
		Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_HIDDEN_PUDDLE), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SOUTH_PUDDLE), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_GATE_PUDDLE), 0);
		Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_NORTH_PUDDLE), 0);
		Object_GetById(ACTOR_NORTH_PUDDLE)->scale_y = 0xf333;
		if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_LOWERED) != 0)
		{
			Gate_Lower();
		}
		if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_RAISED) != 0)
		{
			Gate_Raise();
		}
		if (Engine_GameFlagIsSet(FLAG_CAVE_GATE_PROPPED) != 0)
		{
			Gate_DrawPropped();
		}
		if (Engine_GameFlagIsSet(FLAG_CAVE_NORTH_PILLAR) != 0)
		{
			Engine_ActorSetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIM_FROZEN);
		}
		if (Engine_GameFlagIsSet(FLAG_CAVE_SOUTH_PILLAR) != 0)
		{
			Engine_ActorSetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIM_FROZEN);
		}
	}
	return 0;
}

const struct SceneEntrance gCaveEntrances[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
	{ CAVE_ENTRANCE_FROM_LUNPA, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
	{ CAVE_ENTRANCE_THIRD, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 488, 0 },
	{ SCENE_TABLE_END },
};

const u32 gCaveExits[] = {
	SCENE_EXITS(SCENE_RUNPA_DOU),
	SCENE_EXIT(CAVE_EXIT_TO_WORLD_MAP, SCENE_WORLD_MAP, WORLD_MAP_ENTRANCE_FROM_CAVE),
	SCENE_EXIT(CAVE_EXIT_TO_LUNPA, SCENE_RUNPA_MURA, LUNPA_ENTRANCE_FROM_CAVE),
	SCENE_EXIT(CAVE_EXIT_BY_WAGON, SCENE_WORLD_MAP, WORLD_MAP_ENTRANCE_FROM_CAVE),
	SCENE_EXITS_END,
};

const struct ScenePlacement gCaveNoPlacements[] = {
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gCavePlacements[] = {
	{ SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(136), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(344), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(424), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SPRITE_BUNZA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

const struct SceneEvent gCaveNoEvents[] = {
	{ SCENE_EVENTS_END },
};

const struct SceneEvent gCaveEvents[] = {
	{ EVENT_EXIT, TRIGGER_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, CAVE_EXIT_TO_WORLD_MAP },
	{ EVENT_EXIT, TRIGGER_LUNPA_PASSAGE, CONDITION_ALWAYS, CAVE_EXIT_TO_LUNPA },
	{ EVENT_TOUCH, TRIGGER_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      EVENT_SCRIPT(EV10_TOUCH) },
	{ SEARCH(SEARCH_UNNAMED), TRIGGER_LOWERING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV03_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), TRIGGER_RAISING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV04_SEARCH) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV08_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GATE_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EV11_PSYNERGY) },
	{ SCENE_EVENTS_END },
};
