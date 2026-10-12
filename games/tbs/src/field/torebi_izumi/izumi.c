#include "keystate.h"
#include "unit/field/torebi_izumi/izumi.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/graphics/window/window.h"
#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/system/scheduler/runtime.h"
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
#include "call.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "global_cells.h"
#include "edition.h"
#include "field_service.h"
#include "game_state.h"
#include "inventory.h"
#include "menu_list.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "iowrite.h"
#include "frame.h"

// 前方宣言
static void TorebiIzumi_RiseAndFadeIn(void);
static void TorebiIzumi_OfferLuckyWheels(s32 spoken);
static s32 TorebiIzumi_RunSpringGame(void);
static s32 SceneDialogue_PickTopicVariantId(s32 topic);
static void SceneState_InitFourActorRecordsAndInstallTask(void);
static s32 TorebiIzumi_RunSpringRide(s32 side);

struct SpringPoint
{
	s32 x;
	s32 y;
	s32 z;
};

struct SpringRide
{
	u8				   unknown_00[2];
	s16				   hold;
	struct SpringPoint trail[4];
	struct SpringPoint shadow;
	struct SpringPoint velocity;
	s32				   frames;
};

struct SpringActor
{
	s32 x;
	s32 y;
	s32 z;
	s16 heading;
	s16 hold;
	s16 mode;
	s16 react;
	s16 cool;
	u8	unknown_16[2];
};

static __inline__ void Io_SetBlendControl(s32 value)
{
	REG_BLDCNT = value;
}

static __inline__ void Io_SetBlendAlpha(s32 value)
{
	REG_BLDALPHA = value;
}

u8 *SceneData_GetSceneTableA(void)
{
	return TorebiIzumi_SceneTableA;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSceneTableB(void)
{
	return TorebiIzumi_SceneTableB;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_TorebiIzumi2)
	{
		return gTorebiIzumiPlacements2;
	}
	return gTorebiIzumiPlacementsOther;
}

static void EV27_TALK(s32 object)
{
	s32 question = (s32)MsgTorebiLuckyMedal;
	Engine_EventSetMessage(question);
	Engine_EventOpenMessage(object, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(question + 1);
	}
	else
	{
		Engine_EventSetMessage(question + 2);
	}
	Engine_EventShowMessage(object, 0);
}

static void EV11_TALK(s32 object)
{
	s32 question = (s32)MsgTorebiYerFirstTime;
	Engine_EventSetMessage(question);
	Engine_EventOpenMessage(object, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_EventSetMessage(question + 1);
	}
	else
	{
		Engine_EventSetMessage(question + 2);
	}
	Engine_EventShowMessage(object, 0);
}

static void EV80_SEARCH(void)
{
	s32 leader = gGameState.selected_actor;

	if (GameFlag_IsSet(0x200) == 0)
	{
		Engine_GameFlagSet(0x200);
		TorebiIzumi_RiseAndFadeIn();
	}
	Engine_EventBegin();
	Engine_ActorWalkToAndWait(leader, 120, 152);
	Engine_ActorFaceDirection(leader, 0x4000, 0);
	TorebiIzumi_RunSpringGame();
	Engine_EventEnd();
}

static void EV15_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiLuckyWheelsRulesPullLever);
	Engine_EventOpenMessage(-1, 0);
	Engine_EventEnd();
}

static void EV14_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgTorebiLuckyWheelsPrizesPrizesDetermined);
	Engine_EventOpenMessage(-1, 0);
	Engine_EventEnd();
}

static void EV08_TALK(void)
{
	TorebiIzumi_OfferLuckyWheels(0);
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_TorebiIzumi2)
	{
		return gTorebiIzumiEvents2;
	}
	return gTorebiIzumiEventsOther;
}

s32 TorebiIzumi_OpenScene(void)
{
	s32 diff;
	s8 *list;
	s32 i;
	s32 item;

	if (gGameState.scene == (s32)&SceneId_TorebiIzumi2)
	{
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		Io_Write16(0x3f42, &REG_BLDCNT);
		Io_Write16(0x80c, &REG_BLDALPHA);
		Engine_ActorSetAnimation(24, 2);
		Engine_ActorSetAnimation(25, 2);
		Object_GetById(24)->scale_x = -0x10000;
		Object_GetById(25)->scale_x = -0x10000;
		Object_GetById(24)->priority_flags = 2;
		Object_GetById(25)->priority_flags = 2;
		Engine_EventOpenScreen();
		if (gGameState.entrance == 1)
		{
			SceneState_InitFourActorRecordsAndInstallTask();
			if (Engine_GameFlagIsSet(0x200))
			{
				Io_SetBlendControl(0x3f42);
				Io_SetBlendAlpha(0x1000);
			}
		}
	}
	else
	{
		if (Engine_GameFlagIsSet(0x950))
		{
			Engine_ActorSetPosition(17, 0, 0);
		}
		gOamCopyEnabled = 1;
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
		if (gGameState.entrance == 10)
		{
			Engine_ActorSetChildValue(8, 1);
			Engine_ActorSetChildValue(9, 2);
		}
		if (gGameState.entrance == 13 && !Engine_GameFlagIsSet(0x109))
		{
			Engine_EventBegin();
			Engine_ActorSetChildValue(8, 1);
			Engine_ActorSetChildValue(9, 2);
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			Engine_EventWait(10);
			Engine_ActorWalkToAndWait(0, 120, 112);
			Engine_EventWait(20);
			diff = gGameState.coins - *(s32 *)gSceneState;
			if (diff > 0)
			{
				if (diff > 19999)
				{
					Engine_AudioPlayCue(93);
				}
				else if (diff > 4999)
				{
					Engine_AudioPlayCue(92);
				}
				else
				{
					Engine_AudioPlayCue(91);
				}
				Engine_EventWait(20);
				Engine_EventSetMessage((s32)MsgTorebiYaWonNumber);
				UiWork_PushValueSlot(diff, 5);
				Engine_EventShowMessage(9, 0);
				AudioCommand_WaitForStateByteClear();
			}
			else if (diff < 0)
			{
				Engine_EventSetMessage((s32)MsgTorebiYaLostNumber);
				UiWork_PushValueSlot(-diff, 5);
				Engine_EventShowMessage(9, 0);
			}
			Engine_EventEnd();
		}
		if (gGameState.entrance == 12 && !Engine_GameFlagIsSet(0x109))
		{
			list = gGameState.won_prizes;
			Engine_EventBegin();
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			Engine_EventWait(10);
			if (list[0] == -1)
			{
				TorebiIzumi_OfferLuckyWheels(1);
			}
			else if (list[0] != -2)
			{
				Engine_EventSetMessage((s32)MsgTorebiCongratulations);
				Engine_EventShowMessage(8, 0);
				if (*list != -1)
				{
					for (i = 0; list[i] != -1; i++)
					{
						if (i == 0)
						{
							Engine_EventSetMessage((s32)MsgTorebiWonItemGo);
						}
						else
						{
							Engine_EventSetMessage((s32)MsgTorebiWonItemGo2);
						}
						item = SceneDialogue_PickTopicVariantId(list[i]);
						UiWork_PushValueSlot(item, 2);
						Engine_EventShowMessage(8, 0);
						Engine_ItemShowFound(item, 3);
						Engine_PartyGiveItem(item, 0);
						Engine_EventWait(10);
						Engine_ActorFaceDirection(0, 0xc000, 0);
						Engine_EventWait(30);
					}
				}
				gGameState.won_prizes[0] = -2;
				Engine_EventSetMessage((s32)MsgTorebiComeAgain);
				Engine_EventShowMessage(8, 0);
			}
			Engine_EventEnd();
		}
	}
	return 0;
}

static void TorebiIzumi_RevealPrize(s32 item)
{
	Engine_EventBegin();
	Engine_EventWait(30);
	Engine_AudioPlayCue(148);
	Engine_EventWait(100);
	Actor_FaceDirection(0, 0xc000, 0);
	Engine_EventWait(40);

	Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
	Engine_EventWait(3);
	Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);

	Map_CopyCellsTo(79, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(100, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);

	Engine_EventWait(70);
	Engine_AudioPlayCue(126);
	Engine_ItemShowFound(item, 3);
	Engine_PartyGiveItem(item, 0);
	Engine_EventWait(20);

	Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Map_CopyCellsTo(79, 20, 70, 0, 3, 8);
	Engine_AudioPlayCue(154);
	Engine_EventWait(8);
	Engine_EventEnd();
}

static void TorebiIzumi_RiseAndFadeIn(void)
{
	u8		  *p = gMapWork[0];
	const u16 *step;
	s32		   i;

	Engine_AudioPlayCue(216);
	p += 356;
	for (i = 0; i < 16; i++)
	{
		((struct FieldActor *)p)->y.fixed -= 0x10000;
		Engine_TaskWait(4);
	}
	step = TorebiIzumi_AlphaSteps;
	for (i = 0; i < 8; i++)
	{
		Io_Write16(0x3f42, &REG_BLDCNT);
		Io_Write16(*step++, &REG_BLDALPHA);
		Engine_TaskWait(8);
	}
}

static void TorebiIzumi_OfferLuckyWheels(s32 spoken)
{
	s32 tickets = PartyInventory_CountItem(228);
	s32 room;

	ObjectTable_Snapshot();
	if (spoken == 0)
	{
		s32 message = (s32)MsgTorebiStepRightTry;

		Engine_EventSetMessage(message);
		Engine_EventShowMessage(8, 0);
		if (tickets == 0)
		{
			Engine_EventShowMessage(8, 0);
			return;
		}
		Engine_EventSetMessage(message + 2);
		UiWork_PushValueSlot(tickets, 5);
		Engine_EventOpenMessage(8, 0);
		if (Engine_EventChooseYesNo(0, 0) != 0)
		{
			Engine_EventShowMessage(8, 0);
			return;
		}
		room = PartyInventory_CountFreeSlots();
		if (room == 0)
		{
			Engine_EventSetMessage(message + 4);
			Engine_EventOpenMessage(8, 0);
		}
		else
		{
			if (room > 6)
			{
				goto play;
			}
			Engine_EventSetMessage(message + 5);
			Engine_EventOpenMessage(8, 0);
		}
		if (room > 6 || Engine_EventChooseYesNo(0, 0) == 0)
		{
			goto play;
		}
		Engine_EventSetMessage((s32)MsgTorebiGetRidItems);
		Engine_EventShowMessage(8, 0);
		return;
	}
	if (tickets == 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiBadComeBack);
		Engine_EventShowMessage(8, 0);
		return;
	}
	Engine_EventSetMessage((s32)MsgTorebiBadLikeUse);
	Engine_EventOpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		Engine_EventSetMessage((s32)MsgTorebiComeAgain);
		Engine_EventShowMessage(8, 0);
		return;
	}
play:
	Engine_EventSetMessage((s32)MsgTorebiStepRightEnjoy);
	Engine_EventShowMessage(8, 0);
	Event_RequestDestination(508, 0);
	Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 12);
}

static void EV09_TALK(void)
{
	u32 cost;
	u32 coins;

	cost = Party_GetAverageLevel() * 10;
	Engine_EventBegin();
	coins = gGameState.coins;
	if (coins < cost)
	{
		Engine_EventSetMessage((s32)MsgTorebiYaDontEnough);
		Engine_EventOpenMessage(9, 0);
		return;
	}
	*(u32 *)gSceneState = coins;
	ObjectTable_Snapshot();
	Engine_EventSetMessage((s32)MsgTorebiBuddyWannaTry);
	UiWork_PushValueSlot(cost, 5);
	Engine_EventOpenMessage(9, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventShowMessage(9, 0);
		Engine_ActorWalkToAndWait(0, 120, 128);
		Engine_ActorWalkToAndWait(0, 120, 152);
		Engine_ActorFaceDirection(0, 0x8000, 0);
		Engine_EventWait(20);
		Event_RequestDestination(0x1fd, 0);
		Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 13);
	}
	else
	{
		gEventWork->message++;
		Engine_EventShowMessage(9, 0);
	}
	Engine_EventEnd();
}

#if defined(TBS_EDITION_EN) || !EDITION_INTERNATIONAL
#define ASK_WIDTH 17
#else
#define ASK_WIDTH 19
#endif
#if !EDITION_INTERNATIONAL
#define NUMBER_X 56
#define TOSS_X 22
#define TOSS_WIDTH 7
#elif defined(TBS_EDITION_EN)
#define NUMBER_X 72
#define TOSS_X 20
#define TOSS_WIDTH 9
#else
#define NUMBER_X 88
#define TOSS_X 19
#define TOSS_WIDTH 10
#endif

static s32 TorebiIzumi_RunSpringGame(void)
{
	s32				  choice = 0;
	s32				  text;
	s32				  cancel = -1;
	s32				  coins;
	s32				  tickets;
	struct UiWindow	 *window;
	s32				  result;
	s32				  sum;
	s32				  i;
	s32				  roll;
	s32				  message;
	struct GameState *state;

	Event_Begin();
	Battle_ResetEffectCounter();
	message = (s32)MsgTorebiThrow;
#if EDITION_INTERNATIONAL
	state = &gGameState;
prompt:
	coins = state->coins;
	tickets = PartyInventory_CountItem(229);
	Event_SetMessage(message);
	Event_OpenMessage(-1, 0);
	window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
	{
		text = (s32)MsgTorebiCoins;
		UiText_DrawCharacterAtOffset(text, window, 0, 0);
		UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
		UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
		UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
	}
	choice = Menu_SelectEntry20To21(choice);
	UiWork_Finalize(window, 2);
	UiWork_FinalizePendingCore();
	if (choice == cancel)
	{
		goto end;
	}
	if (choice == 0)
	{
		if (coins != 0)
		{
			goto play;
		}
		Event_SetMessage(message + 1);
		Event_ShowMessage(-1, 0);
		Task_Wait(1);
		goto prompt;
	}
	if (choice != 1)
	{
		goto play;
	}
	if (tickets != 0)
	{
		goto room;
	}
	Event_SetMessage(message + 2);
	Event_ShowMessage(-1, 0);
	Task_Wait(1);
	goto prompt;
room:
	if (PartyInventory_CountFreeSlots() == 0)
	{
		Event_SetMessage(message + 4);
		Event_OpenMessage(-1, 0);
		if (Event_ChooseYesNo(0, 0) != 0)
		{
			goto end;
		}
	}
play:
	window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
	{
		text = (s32)MsgTorebiToss;
		UiText_DrawCharacterAtOffset(text, window, 0, 0);
		UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
	}
	Task_Wait(5);
	Event_PlayCue(116);
	for (;;)
	{
		if (gKeyState & KEY_A)
		{
			Event_PlayCue(112);
			result = 0;
			break;
		}
		if (gKeyState & KEY_B)
		{
			Event_PlayCue(113);
			result = -1;
			break;
		}
		Task_Wait(1);
	}
	UiWork_Finalize(window, 2);
#else
	/* FAKEMATCH: whole marker ids swap message/label registers; keep message in r10 and label in r9. 2026-10-11: writing the marker as text + 2 matches in tbs-ja (the pool word becomes MsgTorebiCoins+2) but the id gate refuses an id with an addend (O3), so both barriers stay. */
	asm("" : "+r"(message));
	for (;;)
	{
		text = (s32)MsgTorebiCoins;
		/* FAKEMATCH: keep the coin label in r9; otherwise the repeated marker takes fp and moves state into r9. */
		asm("" : "+r"(text));
		state = &gGameState;
		coins = state->coins;
		tickets = PartyInventory_CountItem(229);
		Event_SetMessage(message);
		Event_OpenMessage(-1, 0);
		window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
		UiText_DrawCharacterAtOffset(text, window, 0, 0);
		UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
		UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 0);
		UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
		UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
		UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 8);
		choice = Menu_SelectEntry20To21(choice);
		UiWork_Finalize(window, 2);
		UiWork_FinalizePendingCore();
		if (choice == cancel)
		{
			goto end;
		}
		if (choice == 0)
		{
			if (coins == 0)
			{
				Event_SetMessage(message + 1);
				Event_ShowMessage(-1, 0);
				Task_Wait(1);
				continue;
			}
		}
		else if (choice == 1)
		{
			if (tickets == 0)
			{
				Event_SetMessage(message + 2);
				Event_ShowMessage(-1, 0);
				Task_Wait(1);
				continue;
			}
			if (PartyInventory_CountFreeSlots() == 0)
			{
				Event_SetMessage(message + 4);
				Event_OpenMessage(-1, 0);
				if (Event_ChooseYesNo(0, 0) != 0)
				{
					goto end;
				}
			}
		}
		window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
		result = (s32)MsgTorebiToss;
		UiText_DrawCharacterAtOffset(result, window, 0, 0);
		UiText_DrawCharacterAtOffset(result + 1, window, 0, 8);
		Task_Wait(5);
		Event_PlayCue(116);
		for (;;)
		{
			if (gKeyState & KEY_A)
			{
				Event_PlayCue(112);
				result = 0;
				break;
			}
			if (gKeyState & KEY_B)
			{
				Event_PlayCue(113);
				result = -1;
				break;
			}
			Task_Wait(1);
		}
		UiWork_Finalize(window, 2);
		break;
	}
#endif
	if (result == cancel)
	{
		goto end;
	}
	if (choice == 0)
	{
		Party_AdjustCoins(-1);
	}
	else if (choice == 1)
	{
		PartyInventory_Remove(229);
	}
	result = TorebiIzumi_RunSpringRide(choice);
	if (choice == 0)
	{
		if (result != 4)
		{
			Party_AdjustCoins(((struct { u16 coins[1]; } *)Data_0200200c)->coins[result]);
			Event_PlayCue(91);
			UiWork_PushValueSlot(((struct { u16 coins[1]; } *)Data_0200200c)->coins[result], 5);
			Event_SetMessage((s32)MsgTorebiWonNumberCoin);
			Event_ShowMessage(-1, 0);
		}
		else
		{
			Event_PlayCue(113);
			Event_Wait(10);
		}
	}
	else
	{
		sum = 0;
		i = 0;
		if (i < result * 3 + 3)
		{
			do
			{
				u8 w = ((u8 *)state)[i + 285];
				sum += (s8)w;
				i++;
			} while (i < result * 3 + 3);
		}
		roll = (u32)(Random_Next() * sum) >> 16;
		for (i = 0; i < 15; i++)
		{
			roll -= state->prize_weights[i];
			if (roll < 0)
			{
				break;
			}
		}
		if (i == 15)
		{
			i = 14;
		}
		TorebiIzumi_RevealPrize(((struct { s32 prize[1]; } *)Data_02001fd0)->prize[i]);

#if EDITION_INTERNATIONAL
		/* FAKEMATCH: the pointer pair keeps the native prize-weight base
         * recomputation in r0 after the call. Without it, the earlier
         * roulette base survives in r5 (280 -> 278 instructions, 42 rows
         * differ in the international editions). */
		state++;
		state--;
		{

			/* FAKEMATCH: keeps the slot in r1 and state + 1 in r0. 2026-10-11: plain forms share state + 1 with the roulette's peeled first iteration through GCSE PRE (struct field 42/96, s8 pointer 57/107, direct global 38/100 rows in EN/JA); the ++/-- pair breaks the sharing, but then global allocation gives the later-defined pointer r1 before the slot (differ 10) or, pointer first, sched2 keeps adds r0, #1 ahead of adds r1, r6, r3 by RTL order (differ 2). */
			register s32 slot asm("r1") = i + 284;
			s8			*w = (s8 *)state + 1;

			if (w[slot] > 1)
			{
				w[slot] /= 2;
			}
		}
#else
		{
			/* FAKEMATCH: keeps state + 1 in r0 as the native does. */
			register s8 *w asm("r0");
			s32			 slot = i + 284;

			/* FAKEMATCH: the native recomputes state + 1 after the call. */
			__asm__("" : "=r"(w) : "0"(state));
			w++;
			if (w[slot] > 1)
			{
				w[slot] /= 2;
			}
		}
#endif
	}
end:
	Event_End();
	return 0;
}

struct Half
{
	u16 v;
};

static s32 SceneDialogue_PickTopicVariantId(s32 topic)
{
	s32 cursor;
	s32 variant;

	if (topic < 0)
	{
		return 0;
	}

	if (topic == 5)
	{
		topic = ((unsigned int)(Engine_RandomNext() * 5) >> 16);
	}

	cursor = gGameState.won_prizes[8 + topic];

	variant = (cursor + (s32)((unsigned int)(Engine_RandomNext() * 2) >> 16) + 4) % 3;

	gGameState.won_prizes[8 + topic] = variant;

	return TorebiIzumi_TopicIds[topic * 3 + variant];
}

static void TorebiIzumi_PlaceActor(s32 id, s32 *pos, s32 dir, s32 palette, s32 value)
{
	u8 *actor = (u8 *)Object_GetById(id);
	u8 *sprite;
	u32 n;

	if (actor != 0)
	{
		struct Half zero;

		((struct FieldActor *)actor)->x.fixed = *pos++;
		((struct FieldActor *)actor)->y.fixed = *pos++;
		((struct FieldActor *)actor)->z.fixed = *pos;
		((struct FieldActor *)actor)->facing = dir;
		zero.v = 0;
		((struct FieldActor *)actor)->motion_flags = zero.v;
		((struct FieldActor *)actor)->sprite->flags = zero.v;
		ObjectDispatch_ApplyValueToChildren(actor, value);
	}
	sprite = (u8 *)((struct FieldActor *)actor)->sprite;
	n = sprite[39];
	if (n != 0)
	{
		u32	 mask = 255;
		u8 **list = (u8 **)(sprite + 40);
		u32	 left = n;

		do
		{
			u8 *cell = *list++;

			if (cell[5] != palette)
			{
				cell[5] = palette;
				{
					u32 v = cell[22];

					v |= mask;
					cell[22] = v;
				}
			}
		} while (--left != 0);
	}
}

static void TorebiIzumi_SetActorActive(s32 arg0, s32 arg1)
{
	struct FieldActor *entry = Object_GetById(arg0);

	if (entry != 0)
	{
		u8 *field = &entry->active;

		*field = arg1;
	}
}

static struct SpringRide TorebiIzumi_Ride;

static s32 TorebiIzumi_RideSide;

static s32 TorebiIzumi_RideUnknown[3];

static struct SpringActor TorebiIzumi_ActorRecords[4];

static s32 TorebiIzumi_RideFrame;
static s32 TorebiIzumi_RideEnded;
static s32 TorebiIzumi_RideResult;

static void FieldScene_RunSecondaryScript(void)
{
	struct SpringRide  *ride;
	struct SpringActor *rec;
	s32					i;
	s32					y;
	s32					tmp;
	s32					step;
	s32					mode;
	s32					phase;
	s32					xlo;
	s32					xhi;
	s32					zlo;
	s32					zhi;
	/* FAKEMATCH: never used; the reference's frame keeps twelve bytes that no instruction touches. */
	struct SpringPoint spare;

	ride = &TorebiIzumi_Ride;
	i = 3;
	do
	{
		ride->trail[i].x = ride->trail[i - 1].x;
		ride->trail[i].y = ride->trail[i - 1].y;
		ride->trail[i].z = ride->trail[i - 1].z;
		i--;
	} while (i != 0);

	if (ride->hold > 31)
	{
		ride->trail[0].x += ride->velocity.x;
		y = ride->trail[0].y + ride->velocity.y;
		ride->trail[0].y = y;
		ride->trail[0].z += ride->velocity.z;

		if (y <= 0)
		{
			ride->trail[0].y = 0;
			if (ride->velocity.y != 0)
			{
				ride->velocity.y = 0;
				if (TorebiIzumi_RideSide == 1)
				{
					Object_SetMode(Object_GetById(17), 1);
				}
				else
				{
					Object_SetMode(Object_GetById(12), 1);
				}
			}

			if (ride->frames > 0)
			{
				s32 dx;
				s32 dz;
				s32 len;

				dx = (0x780000 - ride->trail[0].x) >> 8;
				dz = (0x470000 - ride->trail[0].z) >> 8;
				len = Iwram_Sqrt(dx * dx + dz * dz);
				ride->velocity.x += 6553 * dx / len;
				ride->velocity.z += 6553 * dz / len;
				ride->velocity.x = ride->velocity.x * 253 / 256;
				ride->velocity.z = ride->velocity.z * 253 / 256;
				ride->frames = ride->frames - 1;
			}
			else
			{
				ride->velocity.x = ride->velocity.x * 220 / 256;
				ride->velocity.z = ride->velocity.z * 220 / 256;
				if (ride->velocity.x > -1024 && ride->velocity.x < 1024)
				{
					ride->velocity.x = 0;
				}
				if (ride->velocity.z > -1024 && ride->velocity.z < 1024)
				{
					ride->velocity.z = 0;
				}
				if (ride->velocity.x == 0 && ride->velocity.z == 0)
				{
					if (TorebiIzumi_RideSide == 1)
					{
						Object_SetMode(Object_GetById(17), 2);
						TorebiIzumi_SetActorActive(15, 0);
						TorebiIzumi_SetActorActive(14, 0);
						TorebiIzumi_SetActorActive(13, 0);
					}
					else
					{
						Object_SetMode(Object_GetById(12), 2);
						TorebiIzumi_SetActorActive(10, 0);
						TorebiIzumi_SetActorActive(9, 0);
						TorebiIzumi_SetActorActive(8, 0);
					}
					{
						s32 dx = (0x780000 - ride->trail[0].x) >> 16;
						s32 dz = (0x470000 - ride->trail[0].z) >> 16;
						s32 dist = dx * dx + dz * dz;

						TorebiIzumi_RideEnded = 1;
						if (dist <= 224)
						{
							TorebiIzumi_RideResult = 0;
						}
						else if (dist <= 624)
						{
							TorebiIzumi_RideResult = 1;
						}
						else if (dist <= 1088)
						{
							TorebiIzumi_RideResult = 2;
						}
						else if (dist <= 1680)
						{
							TorebiIzumi_RideResult = 3;
						}
						else
						{
							TorebiIzumi_RideResult = 4;
						}
					}
				}
			}

			xlo = 0x300000;
			xhi = 0xc00000;
			zlo = 0x180000;
			zhi = 0x780000;
			if (ride->trail[0].z < 0x2a0000)
			{
				tmp = (0x2a0000 - ride->trail[0].z) * 42 / 18;
				xlo = 0x300000 + tmp;
				if (xlo > 0x5a0000)
				{
					xlo = 0x5a0000;
				}
				xhi = 0xc00000 - tmp;
				if (xhi < 0x960000)
				{
					xhi = 0x960000;
				}
			}
			if (ride->trail[0].z > 0x660000)
			{
				tmp = (ride->trail[0].z * 42 - 0x10bc0000) / 18;
				xlo = 0x300000 + tmp;
				if (xlo > 0x5a0000)
				{
					xlo = 0x5a0000;
				}
				xhi = 0xc00000 - tmp;
				if (xhi < 0x960000)
				{
					xhi = 0x960000;
				}
			}
			if (ride->trail[0].x < 0x5a0000)
			{
				tmp = (0x5a0000 - ride->trail[0].x) * 18 / 42;
				zlo = 0x180000 + tmp;
				if (zlo > 0x2a0000)
				{
					zlo = 0x2a0000;
				}
				zhi = 0x780000 - tmp;
				if (zhi < 0x660000)
				{
					zhi = 0x660000;
				}
			}
			if (ride->trail[0].x > 0x960000)
			{
				tmp = (ride->trail[0].x * 18 - 0xa8c0000) / 42;
				zlo = 0x180000 + tmp;
				if (zlo > 0x2a0000)
				{
					zlo = 0x2a0000;
				}
				zhi = 0x780000 - tmp;
				if (zhi < 0x660000)
				{
					zhi = 0x660000;
				}
			}

			if (ride->trail[0].x < xlo)
			{
				ride->trail[0].x = xlo;
				if (ride->velocity.x < 0)
				{
					ride->velocity.x = -ride->velocity.x / 2;
				}
			}
			if (ride->trail[0].x > xhi)
			{
				ride->trail[0].x = xhi;
				if (ride->velocity.x > 0)
				{
					ride->velocity.x = -ride->velocity.x / 2;
				}
			}
			if (ride->trail[0].z < zlo)
			{
				ride->trail[0].z = zlo;
				if (ride->velocity.z < 0)
				{
					ride->velocity.z = -ride->velocity.z / 2;
				}
			}
			if (ride->trail[0].z > zhi)
			{
				ride->trail[0].z = zhi;
				if (ride->velocity.z > 0)
				{
					ride->velocity.z = -ride->velocity.z / 2;
				}
			}
		}
		else
		{
			ride->velocity.y -= 0x4000;
		}
	}

	i = 0;
	do
	{
		rec = &TorebiIzumi_ActorRecords[i];

		if (rec->react > 0)
		{
			rec->react = rec->react - 1;
		}
		if (rec->cool > 0)
		{
			rec->cool = rec->cool - 1;
		}

		if (i <= 1)
		{
			mode = rec->mode;
			step = 0x10000;
			if (mode == 1)
			{
				step = step * 2;
			}
			if (mode == 2)
			{
				step = step * 3;
			}
			if (rec->react > 0)
			{
				if (i == 0)
				{
					Object_SetMode(Object_GetById(18), 3);
				}
				else
				{
					Object_SetMode(Object_GetById(19), 3);
				}
			}
			else
			{
				if (i == 0)
				{
					Object_SetMode(Object_GetById(18), 1);
				}
				else
				{
					Object_SetMode(Object_GetById(19), 1);
				}
				if (rec->hold == 0)
				{
					if (rec->heading == 0)
					{
						rec->x = rec->x + step;
					}
					else
					{
						rec->x = rec->x - step;
					}
					if (rec->x <= 0x400000)
					{
						rec->heading = 0;
						if (i == 1)
						{
							rec->hold = 30;
						}
					}
					if (rec->x > 0xafffff)
					{
						rec->heading = 1;
						if (i == 1)
						{
							rec->hold = 30;
						}
					}
				}
				else
				{
					rec->hold = rec->hold - 1;
				}
			}
		}
		else if (i == 2)
		{
			mode = rec->mode;
			step = -64;
			if (mode == 1)
			{
				step = step * 2;
			}
			if (mode == 2)
			{
				step = step * 3;
			}
			if (rec->react > 0)
			{
				Object_SetMode(Object_GetById(20), 3);
			}
			else
			{
				Object_SetMode(Object_GetById(20), 2);
				rec->x = Engine_MathSin(rec->heading) * 48 + 0x700000;
				rec->z = Engine_MathCos(rec->heading) * 40 + 0x480000;
				phase = (u16)rec->heading + step;
				rec->heading = phase;
				rec->hold = rec->hold + 1;
			}
		}
		else
		{
			phase = rec->hold & 511;
			mode = rec->mode;
			step = 64;
			if (mode == 1)
			{
				step = step * 2;
			}
			if (mode == 2)
			{
				step = step * 3;
			}
			if (rec->react > 0)
			{
				Object_SetMode(Object_GetById(21), 3);
			}
			else if (phase <= 383)
			{
				rec->x = Engine_MathSin(rec->heading) * 52 + 0x700000;
				rec->z = Engine_MathCos(rec->heading) * 24 + 0x480000;
				rec->heading = rec->heading + step;
				Object_SetMode(Object_GetById(21), 2);
			}
			else
			{
				Object_SetMode(Object_GetById(21), 3);
			}
			rec->hold = rec->hold + 1;
		}

		if (rec->cool == 0 && ride->trail[0].y == 0)
		{
			s32 dx;
			s32 dz;
			s32 dist;

			dx = (rec->x - ride->trail[0].x) >> 16;
			dz = (rec->z - ride->trail[0].z) >> 16;
			dist = dx * dx + dz * dz;
			if (dist <= 119 && ride->frames > 30)
			{
				s32 speed;
				s32 next_mode;

				speed = 0x30000;
				if (i <= 1)
				{
					if (rec->heading == 0)
					{
						if (ride->velocity.x < speed)
						{
							ride->velocity.x = speed;
							ride->frames = ride->frames - 100;
						}
					}
					else
					{
						if (ride->velocity.x > -speed)
						{
							ride->velocity.x = -speed;
							ride->frames = ride->frames - 100;
						}
					}
				}
				else
				{
					s32 len;

					len = Iwram_Sqrt(dist);
					ride->velocity.x = -dx * speed / len;
					ride->velocity.z = -dz * speed / len;
					ride->frames = ride->frames - 100;
				}
				Engine_AudioPlayCue(301);
				next_mode = (rec->mode + 1) % 3;
				rec->react = 36;
				rec->mode = next_mode;
				rec->cool = 30;
			}
		}

		switch (i)
		{
		case 0:
			TorebiIzumi_PlaceActor(
				18, &rec->x, 0, TorebiIzumi_SliderFrames[rec->mode], (rec->mode << 4) + 16);
			break;
		case 1:
			TorebiIzumi_PlaceActor(
				19, &rec->x, 0, TorebiIzumi_SliderFrames[rec->mode], (rec->mode << 4) + 16);
			break;
		case 2:
			TorebiIzumi_PlaceActor(20, &rec->x, 0x8000 - rec->heading,
				TorebiIzumi_CircleFrames[rec->mode], (rec->mode << 4) + 16);
			break;
		case 3:
			TorebiIzumi_PlaceActor(21, &rec->x, 0xffff - rec->heading,
				TorebiIzumi_CircleFrames[rec->mode], (rec->mode << 4) + 16);
			break;
		}

		i++;
	} while (i != 4);

	ride->shadow.x = ride->trail[0].x;
	ride->shadow.y = 0;
	ride->shadow.z = ride->trail[0].z;

	if (TorebiIzumi_RideSide == 1)
	{
		TorebiIzumi_PlaceActor(17, &ride->trail[0].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(16, &ride->shadow.x, 0, 0, 16);
		TorebiIzumi_PlaceActor(15, &ride->trail[1].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(14, &ride->trail[2].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(13, &ride->trail[3].x, 0, 0, 16);
		Object_SetMode(Object_GetById(15), 4);
		Object_SetMode(Object_GetById(14), 4);
		Object_SetMode(Object_GetById(13), 4);
	}
	else
	{
		TorebiIzumi_PlaceActor(12, &ride->trail[0].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(11, &ride->shadow.x, 0, 0, 16);
		TorebiIzumi_PlaceActor(10, &ride->trail[1].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(9, &ride->trail[2].x, 0, 0, 16);
		TorebiIzumi_PlaceActor(8, &ride->trail[3].x, 0, 0, 16);
		Object_SetMode(Object_GetById(10), 4);
		Object_SetMode(Object_GetById(9), 4);
		Object_SetMode(Object_GetById(8), 4);
	}

	if (ride->hold != -1)
	{
		ride->hold = ride->hold + 1;
	}
}

static void SceneState_InitFourActorRecordsAndInstallTask(void)
{
	struct SpringRide *ride = &TorebiIzumi_Ride;
	s32				   i = 0;
	u8				  *xtbl;
	u16				  *htbl;
	u8				  *rec;
	u8				  *ztbl;

	xtbl = (u8 *)TorebiIzumi_ActorTileX;
	rec = (u8 *)TorebiIzumi_ActorRecords;
	htbl = (u16 *)TorebiIzumi_ActorHeadings;
	ztbl = (u8 *)TorebiIzumi_ActorTileZ;

	do
	{
		*(s32 *)(rec + 0) = *xtbl << 16;
		*(s32 *)(rec + 8) = *ztbl << 16;
		*(s32 *)(rec + 4) = 0;
		*(u16 *)(rec + 12) = *htbl;
		*(u16 *)(rec + 14) = 0;
		*(u16 *)(rec + 16) = 0;
		*(u16 *)(rec + 18) = 0;
		*(u16 *)(rec + 20) = 0;

		i++;
		xtbl++;
		ztbl++;
		htbl++;
		rec += 24;
	} while (i != 4);

	ride->trail[0].x = 0xffe20000;
	ride->trail[0].y = 0;
	ride->trail[0].z = 0x640000;
	ride->velocity.x = 0;
	ride->velocity.y = 0;
	ride->velocity.z = 0;
	ride->frames = 0;

	Object_SetMode(Object_GetById(20), 2);
	Object_SetMode(Object_GetById(21), 2);

	{
		s32 budget = 0xc83;
		void (*task)(void) = FieldScene_RunSecondaryScript;

		Engine_TaskAddCallback(task, budget);
	}
}

static s32 TorebiIzumi_RunSpringRide(s32 side)
{
	struct SpringRide *ride;

	ride = &TorebiIzumi_Ride;
	ride->trail[0].y = 0;
	ride->trail[1].y = 0;
	ride->trail[2].y = 0;
	ride->trail[3].y = 0;
	TorebiIzumi_RideSide = side;
	TorebiIzumi_RideEnded = 0;
	ride->hold = 0xffff;
	for (TorebiIzumi_RideFrame = 0;; TorebiIzumi_RideFrame++)
	{
		if (TorebiIzumi_RideFrame == 50)
		{
			Engine_AudioPlayCue(300);
		}
		if (TorebiIzumi_RideFrame == 16)
		{
			Engine_ActorSetAnimation(gGameState.selected_actor, 29);
			ride->hold = 0;
			ride->velocity.x = 0x14ccc;
			ride->velocity.y = 0x40000;
			ride->velocity.z = -0x20000;
			ride->trail[0].x = 0x780000;
			ride->trail[0].y = 0x100000;
			ride->trail[0].z = 0x980000;
			ride->frames = 300;
			if (TorebiIzumi_RideSide == 1)
			{
				Object_SetMode(Object_GetById(16), 3);
				Object_SetMode(Object_GetById(17), 0);
				TorebiIzumi_SetActorActive(15, 1);
				TorebiIzumi_SetActorActive(14, 1);
				TorebiIzumi_SetActorActive(13, 1);
			}
			else
			{
				Object_SetMode(Object_GetById(11), 3);
				Object_SetMode(Object_GetById(12), 0);
				TorebiIzumi_SetActorActive(10, 1);
				TorebiIzumi_SetActorActive(9, 1);
				TorebiIzumi_SetActorActive(8, 1);
			}
		}
		Engine_TaskWait(1);
		if (TorebiIzumi_RideEnded == 1)
		{
			break;
		}
	}
	return TorebiIzumi_RideResult;
}

const struct SceneEntrance TorebiIzumi_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 128, 0, 176, FACING_NORTH, 0, 8, 8, 248, 200, 0 },
	{ 11, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, 8, 288, 248, 456, 0 },
	{ 12, CONDITION_ALWAYS, 144, 0, 390, FACING_NORTH, 0, 8, 288, 248, 456, 0 },
	{ 13, CONDITION_ALWAYS, 120, 0, 152, FACING_WEST, 0, 8, 8, 248, 200, 0 },
	{ SCENE_TABLE_END },
};

const u32 TorebiIzumi_SceneTableB[] = {
	SCENE_EXITS(189),
	SCENE_EXIT_IF(1, 135, 20, 0xffffffff),
	SCENE_EXIT(0, 137, 0),
	SCENE_EXIT_IF(1, 135, 13, 0xffffffff),
	SCENE_EXIT_IF(2, 135, 5, 0xffffffff),
	SCENE_EXITS_END,
};

const struct ScenePlacement gTorebiIzumiPlacements2[] = {
	{ 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18c, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18c, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x18d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x112, CONDITION_ALWAYS, 7,
		PIXELS(176), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x112, CONDITION_ALWAYS, 7,
		PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x112, CONDITION_ALWAYS, 7,
		PIXELS(176), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x112, CONDITION_ALWAYS, 7,
		PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(48), 0, PIXELS(136), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(24), 0, PIXELS(72), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTorebiIzumiPlacementsOther[] = {
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(360), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(128), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(96), 0, PIXELS(168), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(104), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(96), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(160), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(152), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(80), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

const struct SceneEvent gTorebiIzumiEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiHmmIWonderIfTheseKids) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiReadTheSignIfYaDont) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiWhoaGettingTripleDigitsIsReal) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiIDontWantToStopPlaying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiWinningOrLosingIsPureLuck) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiTheRulesForLuckyDiceAint) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiSometimesPeopleWhoHaveNeverPlayed) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiThinkingWontHelpYouWinThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiMyLuckHasRunOutFor) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiLuckyDiceThrowTheDiceOn) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiLuckyDiceIfTwoNumbersMatch) },
	{ EVENT_TALK, 17, 0x962, MSGNO(MsgTorebiIzumiYeahThatDiceGameLooksKinda) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x962, MSGNO(MsgTorebiIzumiILikePlayingLuckyDiceBut) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiOhThereAreAlwaysScalpersAt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiTheresNoWayPoorSoldiersLike) },
	{ SCENE_EVENTS_END },
};

const struct SceneEvent gTorebiIzumiEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTorebiTossLuckyMedal) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTorebiFaceAwayTolbi) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiTolbiSpringHasMysteriousPowersStand) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TALK) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiTooBadIOnlyHaveOne) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiIGetNervousWhenIToss) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiWhyWontAnyoneGiveMeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTorebiIzumiThisIsALuckyMedalSo) },
	{ SEARCH(SEARCH_UNNAMED), 80, CONDITION_ALWAYS, EVENT_SCRIPT(EV80_SEARCH) },
	{ SCENE_EVENTS_END },
};

// BLDALPHA の段階
const u16 TorebiIzumi_AlphaSteps[] = {
	0x90a, 0xa08, 0xb06, 0xc04, 0xd03, 0xe02, 0xf01, 0x1000,
};

const u32 Data_02001fd0[] = {
	23, 124, 81, 152, 37, 108, 171, 142,
	48, 160, 133, 97, 183, 186, 189,
};

const u32 Data_0200200c[] = {
	0xa0014, 0x10002, 0,
};

const u32 TorebiIzumi_TopicIds[] = {
	250, 251, 252, 256, 257, 258, 262, 263,
	264, 183, 182, 181, 189, 186, 188,
};

const u8 TorebiIzumi_SliderFrames[] = {
	0, 1, 9,
};

const u8 TorebiIzumi_CircleFrames[] = {
	0, 3, 4,
};

const u16 TorebiIzumi_ActorTileX[] = {
	0xa050, 0x4850,
};

const u16 TorebiIzumi_ActorTileZ[] = {
	0x6820, 0x4844,
};

const u16 TorebiIzumi_ActorHeadings[] = {
	FACING_EAST, 0x1, FACING_EAST, FACING_WEST,
};
