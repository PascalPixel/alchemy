/* @file */
/* The debug room Increase item and level, whose actors raise the party's */
/* levels, give any item and show the game's icons and portraits. */

#include "ability_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/field_psynergy_state.h"
#include "text/msg_ids.h"

const SCENE_ENTRANCE *scGetEntrances(void);
const u32 *scGetExits(void);
const ACTOR_PLACE *scGetActorPlacements(void);
const TRIGGER *scGetTriggers(void);
s32 scInit(void);
extern char IncreaseItemAndLevel_LvLabel[4];
extern char IncreaseItemAndLevel_ItemPrompt[12];
extern char IncreaseItemAndLevel_ItemHelp[24];
extern char IncreaseItemAndLevel_ItemFull[20];
extern char IncreaseItemAndLevel_PsyPrompt[8];
extern char IncreaseItemAndLevel_PsyHelp[12];
extern char IncreaseItemAndLevel_GlyphsUpper[28];
extern char IncreaseItemAndLevel_GlyphsLower[28];
extern char IncreaseItemAndLevel_GlyphsMarks[12];
extern SCENE_ENTRANCE IncreaseItemAndLevel_Entrances[];
extern u32 IncreaseItemAndLevel_Exits[];
extern ACTOR_PLACE IncreaseItemAndLevel_ActorPlacements[];
extern TRIGGER IncreaseItemAndLevel_Triggers[];

/* The item browser's details window, 28 tiles wide outside Japanese and 13 in
 */
/* it, and how it draws the item's plain name: as tiles outside Japanese, into
 */
/* the window's canvas in it. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define ITEM_DEBUG_DETAILS_WIDTH  28
#define ITEM_DEBUG_DRAW_ITEM_NAME mnTextDrawMessageAsTiles
#else
#define ITEM_DEBUG_DETAILS_WIDTH  13
#define ITEM_DEBUG_DRAW_ITEM_NAME mnTextDrawMessage
#endif

#define DEBUG_ABILITY_COUNT 270 /* The ability browser's numbers, 0 to 269. */

/* ACTOR_LEVEL_EDITOR's scale in 16.16, which the international editions */
/* changed (scInit). */
#if TBS_PATCH_1
#define LEVEL_EDITOR_SCALE (FIXED_ONE * 8 / 5)
#else
#define LEVEL_EDITOR_SCALE (FIXED_ONE * 17 / 10)
#endif

/* The scene's actors, numbering the rows of */
/* IncreaseItemAndLevel_ActorPlacements; the game names none of them, so */
/* each is named for what talking to it opens (IncreaseItemAndLevel_Triggers).
 */
typedef enum INCREASE_ITEM_AND_LEVEL_ACTOR_ID {
	/* dbIncreaseItemAndLevelSelectItem: `Item No:` over an item's name and what
	 */
	/* equipping it does, with `A:Get Item  B:Return`. */
	ACTOR_ITEM_BROWSER = ACTOR_FIRST_PLACED,
	/* dbIncreaseItemAndLevelOpenIconBrowser: pages of 32 item icons headed */
	/* `ITEM PAGE:0`. */
	ACTOR_ICON_BROWSER,
	/* dbIncreaseItemAndLevelOpenPortraitBrowser: the portraits, Isaac's first,
	 */
	/* stepped through with Left and Right; Force cast at it runs the Force */
	/* tests. */
	ACTOR_PORTRAIT_BROWSER,
	/* dbIncreaseItemAndLevelRunLevelEditor: `Raise everyone's level.` over the
	 */
	/* leader's name and level, A raising it one and Start five. */
	ACTOR_LEVEL_EDITOR,
	/* dbIncreaseItemAndLevelSelectAbility: `Psy No:` over an ability's name, */
	/* with */
	/* `B:Return`. */
	ACTOR_ABILITY_BROWSER,
	/* An emote bubble standing on its own, showing "...": */
	/* dbIncreaseItemAndLevelWinceAndSpeak. */
	ACTOR_ELLIPSIS_BUBBLE,
	/* An emote bubble standing on its own, showing the wince: */
	/* dbIncreaseItemAndLevelEllipsisAndSpeak. */
	ACTOR_WINCE_BUBBLE
} INCREASE_ITEM_AND_LEVEL_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void) {
	return IncreaseItemAndLevel_Entrances;
}

/* The scene has no exit regions: its regions entry returns none. */
const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return IncreaseItemAndLevel_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	return IncreaseItemAndLevel_ActorPlacements;
}

#if TBS_PATCH_1
/* Talking to ACTOR_ELLIPSIS_BUBBLE: a wince pops up over it and it says "I */
/* won't stop you, but I wouldn't go unless I had business there." The line is
 */
/* chosen before the bubble, where dbIncreaseItemAndLevelEllipsisAndSpeak */
/* chooses it after: the one way the two differ. */
static void dbIncreaseItemAndLevelWinceAndSpeak(void) {
	fdEventSetMessageId(MsgDebugWontStopYou);
	fdActorShowEmote(ACTOR_ELLIPSIS_BUBBLE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventShowMessageAndWait(ACTOR_ELLIPSIS_BUBBLE, 0);
}

/* Talking to ACTOR_WINCE_BUBBLE: a "..." pops up over the other bubble, which
 */
/* says the same line. */
static void dbIncreaseItemAndLevelEllipsisAndSpeak(void) {
	fdActorShowEmote(ACTOR_ELLIPSIS_BUBBLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdEventSetMessageId(MsgDebugWontStopYou);
	fdEventShowMessageAndWait(ACTOR_ELLIPSIS_BUBBLE, 0);
}
#endif

const TRIGGER *scGetTriggers(void) { return IncreaseItemAndLevel_Triggers; }

/* Raises one party member the given number of levels and works out their new */
/* stats. */
static void dbIncreaseItemAndLevelRaiseMemberLevel(s32 memberId, s32 levels) {
	UNIT *unit = gmUnitGet(memberId);

	gmUnitLevelUpTo(memberId, unit->level + levels);
	gmUnitCalc(memberId);
}

/* Raises everyone in the party the given number of levels. */
static void dbIncreaseItemAndLevelRaisePartyLevels(s32 levels) {
	u16 memberIds[16];
	u16 *member;
	s32 memberCount;

	memberCount = syFieldPartyListMembers(memberIds);
	if(memberCount > 0) {
		s32 remaining;
		member = memberIds;
		remaining = memberCount;
		do {
			dbIncreaseItemAndLevelRaiseMemberLevel(*member++, levels);
		} while(--remaining != 0);
	}
}

/* Talking to ACTOR_LEVEL_EDITOR: `Raise everyone's level.` over the leader's */
/* name and level. A raises the whole party one level, Start or Select five, */
/* and B closes the window and works out the four's stats again. */
static void dbIncreaseItemAndLevelRunLevelEditor(void) {
	UNIT *unit;
	WINDOW *window;
	s32 redraw;
	s32 messageId;

	unit = gmUnitGet(MainRoutineStateWork.leaderActorId);
	window = mnWindowCreate(0, 0, 30, 9, WINDOW_FLAG_INSTANT);

	/* "Raise everyone's level.", the A and Start help, and "B Button: End". */
	messageId = MsgDebugRaiseEveryonesLevel;
	mnTextDrawMessage(messageId, window, 0, 0);
	mnTextDrawMessage(messageId + 1, window, 0, 16);
	messageId += 2;
	redraw = 1;
	mnTextDrawMessage(messageId, window, 0, 32);

	for(;;) {
		if(redraw != 0) {
			mnWindowSprRedrawSavedRect(window);
			mnTextDrawStringAtOffset(unit->name, window, 0, 48);
			mnTextDrawStringInWindow(IncreaseItemAndLevel_LvLabel, window, 48,
									 48);
			redraw = 0;
			mnTextDrawNumInWindow(unit->level, 0, window, 72, 48);
		}

		/* Start or Select adds five levels, A one. */
		if((KeysPressed & KEY_START) != 0 || (KeysPressed & KEY_SELECT) != 0) {
			dbIncreaseItemAndLevelRaisePartyLevels(5);
			sySndPlay(MUS_SUPER_LUCKY);
			redraw = 1;
		}

		if((KeysPressed & KEY_A) != 0) {
			dbIncreaseItemAndLevelRaisePartyLevels(1);
			sySndPlay(MUS_LUCKY);
			redraw = 1;
		}

		if((KeysPressed & KEY_B) != 0) {
			sySndPlay(SE_MENU_CANCEL);
			mnWindowSprRedrawSavedRect(window);
			sySynch(1);
			mnWindowClose(window, 1);

			gmUnitCalc(ISAAC);
			gmUnitCalc(GARET);
			gmUnitCalc(MIA);
			gmUnitCalc(IVAN);
			return;
		}

		sySynch(1);
	}
}

/* Talking to ACTOR_ITEM_BROWSER: `Item No:` over an item's name, its icon and
 */
/* what equipping it does, stepping over the numbers with no item. A gives it */
/* to the party, and B, or A once the bags are full, leaves. */
static void dbIncreaseItemAndLevelSelectItem(void) {
	WINDOW *window;
	WINDOW *details;
	s32 selectedItemId;
	s32 redraw;
	s32 itemId;
	s32 entryCount;
	s8 seekDirection;

	entryCount = DEBUG_ITEM_COUNT;
	seekDirection = 0;
	selectedItemId = 1;
	sySndPlay(SE_MENU_CONFIRM);
	redraw = 1;
	window = mnWindowCreate(0, 0, 30, 7, WINDOW_FLAG_INSTANT);
	details =
		mnWindowCreate(0, 8, ITEM_DEBUG_DETAILS_WIDTH, 10, WINDOW_FLAG_INSTANT);
	/* Lend HIGHLIGHT_PALETTE to the item icons, as */
	/* mnPaletteLoadIconsIntoHighlight does: sprite palette 0 into it, then the
	 */
	/* window's background colour over its TEXT_BACKGROUND_COLOR. */
	syDMAXCopy((const void *)OBJ_PLTT,
			   (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
		DMA_COPY_HALFWORDS(1), REG_DMA3);
	sySynch(1);
	while(1) {
		if(redraw) {
			redraw = 0;
			/* Wrap the number into 0 to 269 (a step back from 0 is 269). */
			selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % entryCount;
			mnWindowSprRedrawSavedRect(window);
			mnWindowSprClearList(window);
			mnTextDrawStringInWindow(IncreaseItemAndLevel_ItemPrompt, window, 0,
									 0);
			mnTextDrawNumAtOffset(selectedItemId, 0, window, 80, 0);
			if(gmPartyInventoryHasSpace()) {
				mnTextDrawStringInWindow(IncreaseItemAndLevel_ItemHelp, window,
										 0, 32);
				itemId = selectedItemId & INVENTORY_ITEM_ID_MASK;
				/* DEAD: the item's definition is looked up and not used; the
				 * ROM keeps */
				/* the call. */
				gmItemGetDirect(itemId);
				mnTextDrawMessageAsTiles(itemId + MsgItemName, window, 120, 0);
				ITEM_DEBUG_DRAW_ITEM_NAME(itemId + MsgItemPlainName, window, 0,
										  16);
				mnWindowSprRedrawSavedRect(details);
				mnItemMenuDrawItemDetails(details, selectedItemId);
			}
			else {
				mnTextDrawStringInWindow(IncreaseItemAndLevel_ItemFull, window,
										 0, 32);
			}
		}
		if(KeysPressed & KEY_A) {
			if(gmPartyInventoryAdd(selectedItemId) == -1) {
				sySndPlay(SE_MENU_CANCEL);
				break;
			}
			sySndPlay(SE_NOTE_HIGH);
		}
		if(KeysPressed & KEY_B) {
			sySndPlay(SE_MENU_CANCEL);
			break;
		}
		if(KeysRepeat & KEY_UP) {
			seekDirection = -1;
			selectedItemId--;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_DOWN) {
			seekDirection = 1;
			selectedItemId++;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_RIGHT) {
			seekDirection = 1;
			selectedItemId += DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_LEFT) {
			seekDirection = -1;
			selectedItemId -= DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_R) {
			seekDirection = 1;
			selectedItemId += DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_L) {
			seekDirection = -1;
			selectedItemId -= DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		/* Step on past numbers with no item, in the direction moved. */
		if(seekDirection == -1) {
			selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % entryCount;
			goto check_back;
		back:
			selectedItemId =
				(selectedItemId + DEBUG_ITEM_COUNT - 1) % entryCount;
		check_back:
			if(gmItemGetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon ==
			   0)
				goto back;
		}
		if(seekDirection == 1) {
			selectedItemId = (selectedItemId + DEBUG_ITEM_COUNT) % entryCount;
			goto check_ahead;
		ahead:
			selectedItemId =
				(selectedItemId + DEBUG_ITEM_COUNT + 1) % entryCount;
		check_ahead:
			if(gmItemGetDirect(selectedItemId & INVENTORY_ITEM_ID_MASK)->icon ==
			   0)
				goto ahead;
		}
		seekDirection = 0;
		sySynch(1);
	}
	mnWindowSprRedrawSavedRect(window);
	sySynch(1);
	mnWindowClose(window, 1);
	mnWindowClose(details, 1);
}

#if TBS_PATCH_1
/* Talking to ACTOR_ABILITY_BROWSER in English and German: `Psy No:` over an */
/* ability's name and its description, stepping over the numbers with no */
/* ability; B leaves. */
UNUSED static void dbIncreaseItemAndLevelSelectAbility(void) {
	WINDOW *window;
#ifndef UBFIX
	WINDOW *details;
#endif
	s32 selectedAbilityId;
	s32 redraw;
	s32 abilityId;
	s32 entryCount;
	s8 seekDirection;

	entryCount = DEBUG_ABILITY_COUNT;
	seekDirection = 0;
	selectedAbilityId = 1;
	sySndPlay(SE_MENU_CONFIRM);
	redraw = 1;
	window = mnWindowCreate(0, 0, 30, 12, WINDOW_FLAG_INSTANT);
	/* Lend HIGHLIGHT_PALETTE to the icons, as the item browser does. */
	syDMAXCopy((const void *)OBJ_PLTT,
			   (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
		DMA_COPY_HALFWORDS(1), REG_DMA3);
	sySynch(1);
	while(1) {
		if(redraw) {
			redraw = 0;
			selectedAbilityId =
				(selectedAbilityId + DEBUG_ABILITY_COUNT) % entryCount;
			mnWindowSprRedrawSavedRect(window);
			mnWindowSprClearList(window);
			mnTextDrawStringInWindow(IncreaseItemAndLevel_PsyPrompt, window, 0,
									 0);
			mnTextDrawNumAtOffset(selectedAbilityId, 0, window, 80, 0);
			mnTextDrawStringInWindow(IncreaseItemAndLevel_PsyHelp, window, 0,
									 72);
			abilityId = selectedAbilityId & ACTION_ID_MASK;
			mnTextDrawMessageAsTiles(abilityId + MsgAbilityName, window, 120,
									 0);
			/* The description twice: as tiles, and below it into the canvas. */
			mnTextDrawMessageAsTiles(abilityId + MsgAbilityDescription, window,
									 0, 24);
			mnTextDrawMessage(abilityId + MsgAbilityDescription, window, 0, 48);
		}
		if(KeysPressed & KEY_B) {
			sySndPlay(SE_MENU_CANCEL);
			break;
		}
		if(KeysRepeat & KEY_UP) {
			seekDirection = -1;
			selectedAbilityId--;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_DOWN) {
			seekDirection = 1;
			selectedAbilityId++;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_RIGHT) {
			seekDirection = 1;
			selectedAbilityId += DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_LEFT) {
			seekDirection = -1;
			selectedAbilityId -= DEBUG_BROWSER_ROW_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_R) {
			seekDirection = 1;
			selectedAbilityId += DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		if(KeysRepeat & KEY_L) {
			seekDirection = -1;
			selectedAbilityId -= DEBUG_BROWSER_PAGE_STEP;
			redraw = 1;
			sySndPlay(SE_MENU_CURSOR_MOVE);
		}
		/* Step on past numbers with no ability, in the direction moved. */
		if(seekDirection == -1) {
			selectedAbilityId =
				(selectedAbilityId + DEBUG_ABILITY_COUNT) % entryCount;
			goto check_back;
		back:
			selectedAbilityId =
				(selectedAbilityId + DEBUG_ABILITY_COUNT - 1) % entryCount;
		check_back:
			if(gmAbilityGetDirect(selectedAbilityId & ACTION_ID_MASK)->icon ==
			   0)
				goto back;
		}
		if(seekDirection == 1) {
			selectedAbilityId =
				(selectedAbilityId + DEBUG_ABILITY_COUNT) % entryCount;
			goto check_ahead;
		ahead:
			selectedAbilityId =
				(selectedAbilityId + DEBUG_ABILITY_COUNT + 1) % entryCount;
		check_ahead:
			if(gmAbilityGetDirect(selectedAbilityId & ACTION_ID_MASK)->icon ==
			   0)
				goto ahead;
		}
		seekDirection = 0;
		sySynch(1);
	}
	mnWindowSprRedrawSavedRect(window);
	sySynch(1);
	mnWindowClose(window, 1);
#ifndef UBFIX
	/* UB: details is never created, but the browser closes it. */
	mnWindowClose(details, 1);
#endif
}
#endif

/* Unused: requests an exit through TRANSITION_WINDOW over 24 frames; no event
 */
/* row names it. */
UNUSED static void dbIncreaseItemAndLevelRequestExit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	FieldStateWork->transitionFrm = 24;
	syImportEventSetExitRequest();
}

/* Sets up the room: the window transition in, the level editor's size, and */
/* what each emote bubble shows. */
s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	FieldStateWork->transitionFrm = 24;
	fdActorGetById(ACTOR_LEVEL_EDITOR)->scaleY = LEVEL_EDITOR_SCALE;
	fdActorGetById(ACTOR_LEVEL_EDITOR)->scaleX = LEVEL_EDITOR_SCALE;
#if TBS_PATCH_1
	syImportActorSetAnimation(ACTOR_ELLIPSIS_BUBBLE, EMOTE_ELLIPSIS);
	syImportActorSetAnimation(ACTOR_WINCE_BUBBLE, EMOTE_WINCE);
#endif
	return 0;
}

#if !TBS_PATCH_1
/* Force cast at ACTOR_PORTRAIT_BROWSER in Japanese: Menardi starts walking on
 */
/* the spot and the cat shaking its head, and once Menardi's animation moves on
 */
/* the cat is put at 312x416 on the map. */
static void dbIncreaseItemAndLevelRunMotionTest(void) {
	fdEventStart();
	syImportActorSetAnimation(ACTOR_PORTRAIT_BROWSER, ANIM_WALK);
	syImportActorSetAnimation(ACTOR_LEVEL_EDITOR, ANIM_SHAKE_HEAD);
	fdActorWaitForAnimationChange(ACTOR_PORTRAIT_BROWSER);
	fdActorSetPos(ACTOR_LEVEL_EDITOR, 312, 416);
	fdEventEnd();
}
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/* Force cast at ACTOR_PORTRAIT_BROWSER outside Japanese: a window at the foot
 */
/* of the screen with three rows of the font's letters and marks, left open. */
static void dbIncreaseItemAndLevelShowGlyphCaptions(void) {
	WINDOW *window;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED u8 text[32];

	window = mnWindowCreate(0, 13, 30, 6, WINDOW_FLAG_INSTANT);
	mnTextDrawStringInWindow(IncreaseItemAndLevel_GlyphsUpper, window, 0, 0);
	mnTextDrawStringInWindow(IncreaseItemAndLevel_GlyphsLower, window, 0, 8);
	mnTextDrawStringInWindow(IncreaseItemAndLevel_GlyphsMarks, window, 0, 16);
}
#endif

/* Keeps Force from pushing ACTOR_PORTRAIT_BROWSER: run as Force is cast at it,
 */
/* before the shot, this sets the effect's leaveTgt, and the shot then */
/* leaves its target where it is (field/event_commands_force.c). */
static void dbIncreaseItemAndLevelKeepPortraitBrowserInPlace(void) {
	FIELD_EFFECT_STATE *effectState =
		(FIELD_EFFECT_STATE *)FieldEffectStateWork;

	effectState->leaveTgt = 1;
}

/* Talking to ACTOR_ICON_BROWSER: the icon browser. */
static int dbIncreaseItemAndLevelOpenIconBrowser(void) {
	return mnDebugBrowserIcons();
}

/* Talking to ACTOR_PORTRAIT_BROWSER: the portrait browser. */
static int dbIncreaseItemAndLevelOpenPortraitBrowser(void) {
	return mnDebugBrowserPortraits();
}

char IncreaseItemAndLevel_LvLabel[4] = "Lv";

char IncreaseItemAndLevel_ItemPrompt[12] = "Item No:";

char IncreaseItemAndLevel_ItemHelp[24] = "A:Get Item  B:Return";

char IncreaseItemAndLevel_ItemFull[20] = "ITEM FULL........";

#if LANGUAGE != LANGUAGE_JAPANESE
char IncreaseItemAndLevel_PsyPrompt[8] = "Psy No:";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char IncreaseItemAndLevel_PsyHelp[12] = "B:Return";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
/* The glyph captions' first row; it has no I. */
char IncreaseItemAndLevel_GlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char IncreaseItemAndLevel_GlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
char IncreaseItemAndLevel_GlyphsMarks[12] = "! ? # $ %";
#endif

SCENE_ENTRANCE IncreaseItemAndLevel_Entrances[] = {
	{0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0},
	{SCENE_TABLE_END},
};

/* No exits. */
u32 IncreaseItemAndLevel_Exits[] = {
	SCENE_EXITS_END,
};

/* The scene's actors, in the order of INCREASE_ITEM_AND_LEVEL_ACTOR_ID: */
/* the item, icon and portrait browsers and the level editor; outside Japanese
 */
/* also the ability browser (reachable in English and German only) and the two
 */
/* emote bubbles. */
/* clang-format off */
ACTOR_PLACE IncreaseItemAndLevel_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 864, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 480, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 504, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
#if !TBS_PATCH_1
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_SOUTHEAST,
        TALK_FACING_PARTY, 0),
#elif TBS_PATCH_1 && !TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 864,
        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 816, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
#else
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 800, FACING_EAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 864,
        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 848, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMOTE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 816, 0, 688, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_KEEP, 0),
#endif
    {SCENE_TABLE_END},
};
/* clang-format on */

/* Talking to each actor opens its tool; Force cast at Menardi runs a test of */
/* its own. */
TRIGGER IncreaseItemAndLevel_Triggers[] = {
	{TRIGGER_TALK, ACTOR_ITEM_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelSelectItem)},
	{TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelOpenIconBrowser)},
	{TRIGGER_TALK, ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelOpenPortraitBrowser)},
	{TRIGGER_TALK, ACTOR_LEVEL_EDITOR, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelRunLevelEditor)},
#if !TBS_PATCH_1
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelRunMotionTest)},
#else
#if !TBS_PATCH_3
	{TRIGGER_TALK, ACTOR_ABILITY_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelSelectAbility)},
#endif
	{TRIGGER_TALK, ACTOR_ELLIPSIS_BUBBLE, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelWinceAndSpeak)},
	{TRIGGER_TALK, ACTOR_WINCE_BUBBLE, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelEllipsisAndSpeak)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelShowGlyphCaptions)},
#endif
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_PORTRAIT_BROWSER, CONDITION_ALWAYS,
	 EVENT(dbIncreaseItemAndLevelKeepPortraitBrowserInPlace)},
	{TRIGGER_TABLE_END},
};
