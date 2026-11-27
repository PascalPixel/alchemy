#ifndef ALCHEMY_SCREENS_INTERNAL_H
#define ALCHEMY_SCREENS_INTERNAL_H

/* What every part of the menu object shares: its includes, edition values,
   macros and the link turn state. screens.c includes it before the parts, and
   each part includes it too, so that a part read on its own still resolves. */

#include "ability_ids.h"
#include "enemy_ids.h"
#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle_side.h"
#include "battle/battle_state.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "field/particle.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "menu/item_menu.h"
#include "system/bup.h"
#include "window/battle_status.h"
#include "window/djinn_preview.h"
#include "window/glyph_arm.h"
#include "window/icon_menu.h"
#include "window/scales.h"
#include "window/select.h"
#include "window/text_font.h"

#ifdef MODERN
/* The menu object calls these window functions with no prototype in scope, so
   their arguments take the old promotions (window/window.h lists them); a
   current GCC, which refuses implicit declarations, gets them here. */
void	 mnWindowSprAppendToList(WINDOW_SPR_LIST *list, WINDOW_SPR *node);
MESSAGE *mnMessageStart(WINDOW *window, Sint32 entry, Sint32 x, Sint32 y,
	Uint16 *spacing, Sint32 flags);
void	 mnTextDrawWideStringInWindow(
		Sint16 *text, WINDOW *window, Sint32 x, Sint32 y);
void mnWindowPutGlyph(
	WINDOW *win, Uint32 tile, Uint32 x, Uint32 y, Sint32 mode);
void mnTextDrawPrefixedNumAtOffset(Sint32 value, const WINDOW *window,
	Sint32 offsetX, Sint32 offsetY, Sint32 variant);
#endif

/* Edition-specific text and window layout values: places and widths in tiles,
   except where a note says pixels. The column of the old HP and PP in the
   Djinni preview. */
#define DJINN_PREVIEW_VITALS_X JAPANESE_OR(4, 5)

/* Where a widened number's last four characters start, room for 9999, and its
   last three, room for 999 (the Djinni preview's numberText). */
#define NUMBER_TEXT_FOUR_PLACES	 7
#define NUMBER_TEXT_THREE_PLACES 8

/* Where the Djinni preview's old stats start in their number text, and their
   column. */
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET \
	JAPANESE_OR(NUMBER_TEXT_FOUR_PLACES, NUMBER_TEXT_THREE_PLACES)
#define DJINN_PREVIEW_STAT_X JAPANESE_OR(4, 6)

/* The x of the preview's ":Psynergy Gained" prompt, in pixels. */
#define DJINN_PREVIEW_HELP_X JAPANESE_OR(40, 24)

/* The two UI tiles of the PP label before a Psynergy's cost in the lists. */
#define PSYNERGY_COST_FIRST_TILE  WINDOW_TILE_PP_LABEL
#define PSYNERGY_COST_SECOND_TILE WINDOW_TILE_PP_LABEL_END

/* The Status screen's level label, after the party member's name, in pixels. */
#define OWNER_STATUS_LEVEL_X FRENCH_OR(51, 56)

/* The summon list's window. */
#define SUMMON_MENU_WINDOW_X	 JAPANESE_OR(16, 13)
#define SUMMON_MENU_WINDOW_WIDTH JAPANESE_OR(14, 17)

/* While a party member's commands are chosen (mnBattleCollectPartyCommands):
   the screen x of the party member's portrait sprite, in pixels. */
#define BATTLE_PORTRAIT_X JAPANESE_OR(4, 0)

/* Where "Attack" is written in its window once chosen, in pixels. */
#define BATTLE_COMMAND_NAME_X FRENCH_OR(14, 16)

/* The screen x of the chosen command's icon, in pixels, while the summon list
   is open. */
#define BATTLE_SUMMON_ICON_X JAPANESE_OR(104, 80)

/* The chosen summon's window, in tiles. */
#define BATTLE_SUMMON_WINDOW_WIDTH JAPANESE_OR(14, 17)

/* The tile columns of a summon's first Djinn cost in the battle's summon
   command: its element icon, then its count. */
#define BATTLE_SUMMON_ELEMENT_COLUMN JAPANESE_OR(10, 13)
#define BATTLE_SUMMON_COST_COLUMN	 JAPANESE_OR(11, 14)

/* How the menus open their windows: at once, with no zoom. */
#define MENU_WINDOW_FLAGS (WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD)

/* BG1's tile number of the command row's first icon tile. */
#define COMMAND_ROW_ICON_TILE (BG_CHAR_SIZE / (8 * 8))
/* The command window's top tile row: it runs along the bottom three rows. */
#define COMMAND_ROW_TOP		  17
/* A command icon's pixels: 32x32 at a byte a pixel. */
#define COMMAND_ICON_BYTES	  (32 * 32)

/* Places a battle menu's pointing hand at its list window's selected row:
   `position` takes the hand's spot, `margin` pixels left of the window and 20
   below the row's top, and `sprite` becomes a plain 16x16 sprite, its position
   still to come. */
#define MENU_PLACE_CURSOR(position, window, row, margin, sprite) \
	do { \
		(position).x = (window)->x * 8 - (margin); \
		(position).y = ((row) * 2 + (window)->y) * 8 + 20; \
		((Uint32 *)(sprite))[1] = OBJ_SIZE_16X16; \
		((Uint32 *)(sprite))[2] = 0; \
	} while(0)

/* The chosen item's window. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define BATTLE_ITEM_WINDOW_WIDTH 16
#else
#define BATTLE_ITEM_WINDOW_WIDTH 15
#endif

/* The password level menu (mnIconMenuChooseSendOption,
   SEND_OPTION_PASSWORD_LEVEL): its heading window ("Password Selection") and,
   outside the Japanese edition, the explanation window under it. */
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN || \
	LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_LEVEL_WINDOW_X		7
#define PASSWORD_LEVEL_WINDOW_WIDTH 17
#elif LANGUAGE == LANGUAGE_SPANISH
#define PASSWORD_LEVEL_WINDOW_X		5
#define PASSWORD_LEVEL_WINDOW_WIDTH 21
#elif LANGUAGE == LANGUAGE_FRENCH
#define PASSWORD_LEVEL_WINDOW_X		5
#define PASSWORD_LEVEL_WINDOW_WIDTH 22
#else
#define PASSWORD_LEVEL_WINDOW_X		9
#define PASSWORD_LEVEL_WINDOW_WIDTH 13
#endif

/* The password level menu's explanation window ("The data stored in your
   password depends on which level you select:"), in tiles. */
#define PASSWORD_HELP_WINDOW_X	   GERMAN_OR(1, 3)
#define PASSWORD_HELP_WINDOW_WIDTH GERMAN_OR(28, 25)

/* The transfer method menu (mnIconMenuChooseSendOption,
   SEND_OPTION_TRANSFER_METHOD): its heading window ("Transfer Method"), the
   heading's x in it in pixels, and the help window. */
#define TRANSFER_METHOD_WINDOW_X	 BY_LANGUAGE(9, 6, 6, 4, 5, 5)
#define TRANSFER_METHOD_WINDOW_WIDTH BY_LANGUAGE(13, 18, 18, 21, 20, 20)

/* The x of "Transfer Method" in its heading window, in pixels. */
#define TRANSFER_METHOD_LABEL_X JAPANESE_OR(0, 2)

/* The transfer method menu's help window, in tiles. */
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define TRANSFER_HELP_WINDOW_X	   1
#define TRANSFER_HELP_WINDOW_WIDTH 28
#elif LANGUAGE == LANGUAGE_GERMAN
#define TRANSFER_HELP_WINDOW_X	   0
#define TRANSFER_HELP_WINDOW_WIDTH 30
#else
#define TRANSFER_HELP_WINDOW_X	   5
#define TRANSFER_HELP_WINDOW_WIDTH 21
#endif

/* The width inside the border of the name window under the shop's commands and
   the Sanctum's services. */
#define SHOP_COMMAND_LABEL_WIDTH BY_LANGUAGE(6, 7, 8, 7, 7, 7)

/* The width of the Sanctum's service label window, in tiles. */
#define SANCTUM_SERVICE_LABEL_WIDTH JAPANESE_OR(8, 10)

/* The battle menus' message buffers, in halfwords, and the copier that fills
   them: the Japanese edition decodes the message (mnMessageDecodeRaw), the
   others copy its text (mnMessageCopyExpanded). */
#define BATTLE_MENU_MESSAGE_CAPACITY JAPANESE_OR(32, 52)
#define BATTLE_MENU_MESSAGE_COPY \
	JAPANESE_OR(mnMessageDecodeRaw, mnMessageCopyExpanded)

/* Tiles the battle lists draw over their window's top edge beside the page
   numbers (PAGE_NUMBER_TILE): the page arrows, and the summon list's element
   icon. */
enum {
	/* Element icon: + ELEMENT, in BASIC_COLORS_PALETTE. */
	LIST_TILE_SUMMON_ELEMENT = BG_TILE_PALETTE(BASIC_COLORS_PALETTE) | 1,
	/* Left arrow, tile 820, which mnPageArrowsDraw slides. */
	LIST_TILE_PAGES_LEFT = WINDOW_TILE(820),
	LIST_TILE_PAGES_RIGHT = WINDOW_TILE(821) /* Right arrow, tile 821 */
};

/* The battle lists' two page arrow tiles in VRAM, which mnPageArrowsDraw draws
   each tick (LIST_TILE_PAGES_LEFT and LIST_TILE_PAGES_RIGHT). */
#define LIST_PAGE_ARROW_VRAM \
	((Sint32)BG_CHAR_BLOCK(0) + \
		(LIST_TILE_PAGES_LEFT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP)

/* What mnBattleCommandRowRun returns besides the command icon chosen. */
#define COMMAND_ROW_CANCELLED -1 /* B, in a party member's row */
/* L or R in a party member's row: the party's Status screens. */
#define COMMAND_ROW_STATUS	  -2

/* The two battle command rows mnBattleCommandRowRun runs. */
typedef enum COMMAND_ROW_MODE {
	COMMAND_ROW_PARTY = 0, /* the start of a turn: Fight, Flee and Status */
	/* a party member's commands; any mode but 0 is this row */
	COMMAND_ROW_MEMBER = 1
} COMMAND_ROW_MODE;

/* What mnBattleTargetRunSelection shows on a party member for the action being
   aimed (mnBattleClassifyEntryKind), and which party member it starts on. */
typedef enum BATTLE_TARGET_HINT {
	TARGET_HINT_NONE = 0,
	TARGET_HINT_HP = 1, /* heals HP: the party member's HP */
	TARGET_HINT_PP = 2, /* restores PP: the party member's PP */
	/* Cures poison (ABILITY_EFFECT_CURE_POISON): starts on a poisoned party
	   member. */
	TARGET_HINT_CURE_POISON = 3,
	/* Cures what Restore cures (ABILITY_EFFECT_RESTORE): starts on a party
	   member who has one of those statuses, and lists them. */
	TARGET_HINT_RESTORE = 4,
	/* Revives (gmBattleIsReviveEffectStub): starts on a downed party member. */
	TARGET_HINT_REVIVE = 5,
	/* Cures Restore's statuses, poison and the evil spirit, as Tonic does
	   (ABILITY_EFFECT_CURE_ALL). */
	TARGET_HINT_CURE_ALL = 6,
	TARGET_HINT_SET_DJINNI = 7 /* setting a Djinni: "Set Djinni" */
} BATTLE_TARGET_HINT;

#define BATTLE_OK 0x80 /* ABILITY_DEFINITION.flg: usable in battle */

/* Whether a party member can use an item in battle (mnItemCheckBattleUse). */
typedef enum BATTLE_ITEM_USE {
	BATTLE_ITEM_USABLE = 0,
	/* "Using this will have no effect.": no item, one that only teaches
	   Psynergy while equipped, one with no action, or equipment the party
	   member cannot equip. */
	BATTLE_ITEM_NO_EFFECT = 1,
	/* "Cannot be used during battle.": its action lacks
	   ABILITY_USABLE_IN_BATTLE. */
	BATTLE_ITEM_NOT_IN_BATTLE = 2
} BATTLE_ITEM_USE;

/* What mnSaveScanFileSummaries adds to its count of files when every file is
   cleared (its sendFlg set by saving at the game's end). */
#define SAVE_SCAN_ALL_CLEARED 100

/* One icon of the command row (BATTLE_COMMAND_ROW_STATE): its sprite, where it
   stands and the command it stands for. screens_menu_titles.c loads it,
   screens_battle_command_row.c runs the row. */
typedef struct BATTLE_COMMAND_ROW_ICON {
	/* The icon as a 32x32 sprite, drawn only while it is selected (pulsing);
	   the others show as tiles of BG1. */
	MENU_SPR spr;
	/* resource: the VRAM cache entry its sprite's tiles go into, one 1 KB entry
	   that every icon of the row shares, reloaded with the selected icon.
	   unused is never read. */
	Uint16 resource, unused;
	/* Screen pixels: the sprite's top left before the pulse moves it. */
	Sint32 posX, posY;
	Sint32 kind; /* its command: MsgCommandName + kind names it */
} BATTLE_COMMAND_ROW_ICON;

/* The command row's state block: the icons' shared palette, up to six icons,
   the label window, the pulsing icon's transform and the selection. */
typedef struct BATTLE_COMMAND_ROW_STATE {
	/* BG palette slots for the icons' 256-colour pixels, filled as each icon is
	   loaded (mnBattleCommandRowLoadIconTiles). */
	ICON_PALETTE			pal;
	BATTLE_COMMAND_ROW_ICON entries[6];
	WINDOW				   *win; /* shows the selected command's name */
	Sint32					iconCnt;
	/* The selected icon's scale, 8.8 fixed point: it pulses between 304 and 336
	   (1.19 and 1.31 times) every 16 ticks. */
	AFFINE_TRANSFORM affine;
	/* affineIndex: the OAM matrix built from affine this tick. selectedIndex:
	   the icon under the cursor. drawnIdx: the icon whose name the window
	   shows, -1 before the first is drawn. */
	Sint32 affineIdx, selectedIdx, drawnIdx;
	/* The commands in row order, ended by -1 when fewer than six (see
	   mnBattleCommandRowRun for the kinds). */
	Sint32 commandKinds[6];
} BATTLE_COMMAND_ROW_STATE;

/* The Djinn menu's blinking arrow (djinn_blink.s), and where each element's
   standby count sits in the party status window (window_labels.s). The arrow
   tile mnBattleDjinnMenuRun copies to the two BG tiles from
   DJINN_MENU_ARROW_TILE each tick: the first blinks (four ticks shown, four
   blank), the second stays. */
extern const Uint8 DjinnMenuBlinkTile[32];
/* The two BG tiles of the Djinn menu's arrow: DJINN_MENU_ARROW_TILE blinks, the
   one after it stays. */
#define DJINN_MENU_ARROW_TILE 808
#define DJINN_MENU_ARROW_VRAM \
	(BG_CHAR_BLOCK(0) + DJINN_MENU_ARROW_TILE * TILE_SIZE_4BPP)
/* Where each element's standby Djinn count sits in the party status window's
   Djinn column, a two-by-two grid of tiles: it blinks while the Djinni under
   the Djinn menu's cursor is of that element, on standby and ready to
   summon. */
extern const Uint8 DjinnMenuElementXOffsets[4];
extern const Uint8 DjinnMenuElementYOffsets[4];

#endif
