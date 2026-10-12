#include "game_state.h"
#include "unit/debug/item_level/level.h"
#include "unit/battle/effect/object.h"
#include "unit/game/character/chara.h"
#include "unit/graphics/window/window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "level.h"
#include "fieldobj.h"
#include "call.h"
#include "party_state.h"
#include "types.h"
#include "io_reg.h"
#include "inventory.h"
#include "dma.h"
#include "text_render_runtime.h"
#include "item.h"
#include "edition.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "field_scene.h"

void Engine_AudioPlayCue(s32 cue);
void Engine_TaskWait(s32 frames);
#define ITEM_COUNT 270
#define ABILITY_COUNT 270

u8 *ItemLevel_GetEntrances(void)
{
	return (u8 *)gItemLevelEntrances;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *ItemLevel_GetExits(void)
{
	return (u8 *)gItemLevelExits;
}

u8 *ItemLevel_GetPlacements(void)
{
	return (u8 *)gItemLevelPlacements;
}

#if EDITION_INTERNATIONAL
void FieldScene_RunActor13Mode102Step(void)
{
	Engine_EventSetMessage(MsgDebugWontStopYou);
	Call3(Engine_ActorShowEmote, 13, 0x102, 0);
	Engine_EventShowMessage(13, 0);
}

void FieldScene_RunActor13Mode105Step(void)
{
	Engine_ActorShowEmote(13, 0x105, 0);
	Engine_EventSetMessage(MsgDebugWontStopYou);
	Engine_EventShowMessage(13, 0);
}
#endif

u8 *ItemLevel_GetEvents(void)
{
	return (u8 *)gItemLevelEvents;
}

void SceneState_AddToRecordCount(s32 id, s32 amount)
{
	u8 *entry = UnitGet(id);

	Party_AdvanceOwnerCountToTarget(id, entry[15] + amount);
	UnitCalc(id);
}

void Scene_AddToListedRecordCounts(s32 amount)
{
	u16	 list[16];
	u16 *p;
	s32	 n;

	n = Value1((s32 (*)(u16 *))Party_ListActiveOwners, (s32)list);
	if (n > 0)
	{
		s32 count;
		p = list;
		count = n;
		do
		{
			SceneState_AddToRecordCount(*p++, amount);
		} while (--count != 0);
	}
}

void FieldScene_RunCountAdjustPanel(void)
{
	u8 *record;
	s32 win;
	s32 flag;
	s32 msg;

	record = UnitGet(gGameState.selected_actor);
	win = UiWindow_Create(0, 0, 30, 9, 2);

	msg = (s32)MsgDebugRaiseEveryonesLevel;
	UiText_DrawResource(msg, win, 0, 0);
	UiText_DrawResource(msg + 1, win, 0, 16);
	msg += 2;
	flag = 1;
	UiText_DrawResource(msg, win, 0, 32);

	for (;;)
	{
		if (flag != 0)
		{
			RenderOutput_RedrawSavedRect(win);
			UiText_DrawStringAtOffset(record, win, 0, 48);
			UiText_DrawStringInWindow(gItemLevelLvLabel, win, 48, 48);
			flag = 0;
			UiText_DrawNumberInWindow(record[15], 0, win, 72, 48);
		}

		if ((gKeyState & 8) != 0 || (gKeyState & 4) != 0)
		{
			Scene_AddToListedRecordCounts(5);
			Engine_AudioPlayCue(93);
			flag = 1;
		}

		if ((gKeyState & 1) != 0)
		{
			Scene_AddToListedRecordCounts(1);
			Engine_AudioPlayCue(91);
			flag = 1;
		}

		if ((gKeyState & 2) != 0)
		{
			Engine_AudioPlayCue(113);
			RenderOutput_RedrawSavedRect(win);
			Engine_TaskWait(1);
			UiWork_Finalize(win, 1);

			UnitCalc(0);
			UnitCalc(1);
			UnitCalc(3);
			UnitCalc(2);
			return;
		}

		Engine_TaskWait(1);
	}
}

void ItemLevel_SelectItem(void)
{
	struct RenderInput *window;
	struct RenderInput *details;
	s32					item;
	s32					redraw;
	s32					index;
	s32					count;
	s8					step;

	count = ITEM_COUNT;
	step = 0;
	item = 1;
	Engine_AudioPlayCue(112);
	redraw = 1;
	window = UiWindow_Create(0, 0, 30, 7, 2);
#if EDITION_INTERNATIONAL
	details = UiWindow_Create(0, 8, 28, 10, 2);
#else
	details = UiWindow_Create(0, 8, 13, 10, 2);
#endif
	Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
	Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
	Engine_TaskWait(1);
	for (;;)
	{
		if (redraw)
		{
			redraw = 0;
			item = (item + ITEM_COUNT) % count;
			RenderOutput_RedrawSavedRect(window);
			Engine_DebugClearWindow(window);
			UiText_DrawStringInWindow(gItemLevelItemPrompt, window, 0, 0);
			UiText_DrawNumberAtOffset(item, 0, window, 80, 0);
			if (PartyInventory_HasSpace())
			{
				UiText_DrawStringInWindow(gItemLevelItemHelp, window, 0, 32);
				index = item & 0x1ff;
				Engine_DebugGetItem(index);
				UiText_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
#if EDITION_INTERNATIONAL
				UiText_DrawCharacterAtOffset(index + (s32)MsgItemPlainName, window, 0, 16);
#else
				UiText_DrawResource(index + (s32)MsgItemPlainName, window, 0, 16);
#endif
				RenderOutput_RedrawSavedRect(details);
				Engine_DebugDrawItemDetails(details, item);
			}
			else
			{
				UiText_DrawStringInWindow(gItemLevelItemFull, window, 0, 32);
			}
		}
		if (gKeyState & KEY_A)
		{
			if (PartyInventory_Add(item) == -1)
			{
				Engine_AudioPlayCue(113);
				goto done;
			}
			Engine_AudioPlayCue(175);
		}
		if (gKeyState & KEY_B)
		{
			Engine_AudioPlayCue(113);
			goto done;
		}
		if (gKeysRepeat & KEY_UP)
		{
			step = -1;
			item--;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			step = 1;
			item++;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			step = 1;
			item += 10;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			step = -1;
			item -= 10;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_R)
		{
			step = 1;
			item += 30;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_L)
		{
			step = -1;
			item -= 30;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (step == -1)
		{
			item = (item + ITEM_COUNT) % count;
			goto check_back;
		back:
			item = (item + ITEM_COUNT - 1) % count;
		check_back:
			if (Engine_DebugGetItem(item & 0x1ff)->icon == 0)
				goto back;
		}
		if (step == 1)
		{
			item = (item + ITEM_COUNT) % count;
			goto check_ahead;
		ahead:
			item = (item + ITEM_COUNT + 1) % count;
		check_ahead:
			if (Engine_DebugGetItem(item & 0x1ff)->icon == 0)
				goto ahead;
		}
		step = 0;
		Engine_TaskWait(1);
	}
done:
	RenderOutput_RedrawSavedRect(window);
	Engine_TaskWait(1);
	UiWork_Finalize(window, 1);
	UiWork_Finalize(details, 1);
}

#if EDITION_INTERNATIONAL
void ItemLevel_SelectAbility(void)
{
	struct RenderInput *window;
	struct RenderInput *details;
	s32					ability;
	s32					redraw;
	s32					index;
	s32					count;
	s8					step;

	count = ABILITY_COUNT;
	step = 0;
	ability = 1;
	Engine_AudioPlayCue(112);
	redraw = 1;
	window = UiWindow_Create(0, 0, 30, 12, 2);
	Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
	Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
	Engine_TaskWait(1);
	for (;;)
	{
		if (redraw)
		{
			redraw = 0;
			ability = (ability + ABILITY_COUNT) % count;
			RenderOutput_RedrawSavedRect(window);
			Engine_DebugClearWindow(window);
			UiText_DrawStringInWindow(gItemLevelPsyPrompt, window, 0, 0);
			UiText_DrawNumberAtOffset(ability, 0, window, 80, 0);
			UiText_DrawStringInWindow(gItemLevelPsyHelp, window, 0, 72);
			index = ability & 0x3fff;
			UiText_DrawCharacterAtOffset(index + (s32)MsgAbilityName, window, 120, 0);
			index += (s32)MsgAbilityDescription;
			UiText_DrawCharacterAtOffset(index, window, 0, 24);
			UiText_DrawResource(index, window, 0, 48);
		}
		if (gKeyState & KEY_B)
		{
			Engine_AudioPlayCue(113);
			goto done;
		}
		if (gKeysRepeat & KEY_UP)
		{
			step = -1;
			ability--;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			step = 1;
			ability++;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			step = 1;
			ability += 10;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			step = -1;
			ability -= 10;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_R)
		{
			step = 1;
			ability += 30;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (gKeysRepeat & KEY_L)
		{
			step = -1;
			ability -= 30;
			redraw = 1;
			Engine_AudioPlayCue(111);
		}
		if (step == -1)
		{
			ability = (ability + ABILITY_COUNT) % count;
			goto check_back;
		back:
			ability = (ability + ABILITY_COUNT - 1) % count;
		check_back:
			if (Engine_DebugGetAbility(ability & 0x3fff)[4] == 0)
				goto back;
		}
		if (step == 1)
		{
			ability = (ability + ABILITY_COUNT) % count;
			goto check_ahead;
		ahead:
			ability = (ability + ABILITY_COUNT + 1) % count;
		check_ahead:
			if (Engine_DebugGetAbility(ability & 0x3fff)[4] == 0)
				goto ahead;
		}
		step = 0;
		Engine_TaskWait(1);
	}
done:
	RenderOutput_RedrawSavedRect(window);
	Engine_TaskWait(1);
	UiWork_Finalize(window, 1);
	UiWork_Finalize(details, 1);
}
#endif

void SceneState_SetWorkWords1c0And1c8(void)
{
	gEventWork->start_transition = 0x201;
	gEventWork->transition_frames = 24;
	Engine_EventRequestExit();
}

s32 FieldScene_RunEntrySetup(void)
{
	gEventWork->start_transition = 516;
	gEventWork->transition_frames = 24;
#if EDITION_INTERNATIONAL
	Object_GetById(11)->scale_y = 0x19999;
	Object_GetById(11)->scale_x = 0x19999;
	Engine_ActorSetAnimation(13, 5);
	Engine_ActorSetAnimation(14, 2);
#else
	Object_GetById(11)->scale_y = 0x1b333;
	Object_GetById(11)->scale_x = 0x1b333;
#endif
	return 0;
}

#if !EDITION_INTERNATIONAL
void ItemLevel_RunMotionTest(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(10, 2);
	Engine_ActorSetAnimation(11, 4);
	ObjectMotion_WaitForAnimationChange(10);
	Call3(Engine_ActorSetPosition, 11, 312 << 16, 416 << 16);
	Engine_EventEnd();
}
#endif

#if EDITION_INTERNATIONAL
void FieldScene_DrawThreeCaptionWindow(void)
{
	struct RenderInput *window;
	/* FAKEMATCH: an unread 32-byte text buffer, the native frame's 32 extra bytes. Without it: differ 4 (sp moves 4 for 36). */
	u8 text[32];

	window = (struct RenderInput *)UiWindow_Create(0, 13, 30, 6, 2);
	UiText_DrawStringInWindow(gItemLevelGlyphsUpper, window, 0, 0);
	UiText_DrawStringInWindow(gItemLevelGlyphsLower, window, 0, 8);
	UiText_DrawStringInWindow(gItemLevelGlyphsMarks, window, 0, 16);
}
#endif

void SceneState_SetRecordFlag53(void)
{
	u8 *record = *(u8 **)gEffectWork;

	record[53] = 1;
}

int SceneState_GetFarResult100c(void)
{
	return DebugMenu_BrowseIcons();
}

int SceneState_GetFarResult1020(void)
{
	return DebugMenu_BrowseEntryGlyphs();
}

char gItemLevelLvLabel[4] = "Lv";

char gItemLevelItemPrompt[12] = "Item No:";

char gItemLevelItemHelp[24] = "A:Get Item  B:Return";

char gItemLevelItemFull[20] = "ITEM FULL........";

#if EDITION_INTERNATIONAL
char gItemLevelPsyPrompt[8] = "Psy No:";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelPsyHelp[12] = "B:Return";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsMarks[12] = "! ? # $ %";
#endif

struct SceneEntrance gItemLevelEntrances[] = {
	{0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
	{SCENE_TABLE_END},
};

u32 gItemLevelExits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement gItemLevelPlacements[] = {
	{0x1f4, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(864), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(480), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x21, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(504), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
#if !EDITION_INTERNATIONAL
	{0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
#elif defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
		0},
	{0x8f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
		TALK_KEEP_FACING, 0},
	{0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
		TALK_KEEP_FACING, 0},
#else
	{0x88, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY,
		0},
	{0x8f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(864), FACING_SOUTH + FACING_STEP,
		TALK_FACE_PARTY_AND_BACK, 0},
	{0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
		TALK_KEEP_FACING, 0},
	{0x15, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP,
		TALK_KEEP_FACING, 0},
#endif
	{SCENE_TABLE_END},
};

struct SceneEvent gItemLevelEvents[] = {
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectItem)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult100c)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult1020)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunCountAdjustPanel)},
#if !EDITION_INTERNATIONAL
	{PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_RunMotionTest)},
#elif defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectAbility)},
	{EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode102Step)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode105Step)},
	{PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_DrawThreeCaptionWindow)},
#else
	{EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode102Step)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode105Step)},
	{PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_DrawThreeCaptionWindow)},
#endif
	{0x10008e15, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetRecordFlag53)},
	{SCENE_EVENTS_END},
};
