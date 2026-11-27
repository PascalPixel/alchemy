#ifndef ALCHEMY_WINDOW_INTERNAL_H
#define ALCHEMY_WINDOW_INTERNAL_H

/* @file */
/* What the parts of the window object share: their includes, the codes of */
/* decoded message text and the sizes several parts read. window.c includes it
 */
/* before the parts, and each part includes it too, so that a part read on its
 */
/* own still resolves. */

/* The window and screens objects call syOamAddSprAtDepth in a form of their */
/* own, declared below. */
#define OWN_Oam_AddSpriteAtDepth

#include "battle/battle.h"
#include "debug/debug.h"
#include "djinn_ids.h"
#include "enemy_ids.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "types.h"
#include "edition.h"
#include "window/window.h"
#include "graphics/uiwindow.h"
#include "graphics/window_close.h"
#include "graphics/render_input.h"
#include "graphics/resource.h"
#include "graphics/glyph.h"
#include "version.h"
#include "tla_edition.h"
#include "graphics/text_font.h"
#include "graphics/text_render.h"
#include "fixed_math.h"
#include "graphics/text_reader.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "menu/select.h"
#include "text/msg_ids.h"
#include "menu/selection_cursor.h"
#include "static_assert.h"
#include "macros.h"
#include "graphics/oam.h"
#include "graphics/vram_tab.h"
#include "stddef.h"
#include "battle/battle_state.h"
#include "battle/battle_side.h"
#include "game/class_djinn.h"
#include "field/field_sprite.h"
#include "battle/battle_summon.h"
#include "battle/battle_system.h"
#include "field/event.h"
#include "window/screens.h"
#include "window/item_icon.h"
#include "battle/stubs.h"
#include "shop/stubs.h"
#include "field/actor.h"
#include "battle/battle_action.h"
#include "window/icon_menu.h"
#include "graphics/display_transition.h"
#include "field/sprite.h"

/* Queues a sprite at a depth slot of this frame's OAM list. The window and */
/* screens objects' ROM code is compiled with no declaration in scope, as */
/* against this form: no parameter list and an s32 result, where the */
/* definition takes the entry and the slot and returns nothing. */
s32 syOamAddSprAtDepth();

/* The window animation a message can start (its window-state code): the */
/* window's frame flashes as it swells, for the message's countdown. */
#define WINDOW_ANIMATION_SWELL 4

/* Window animation lengths. */
#define WINDOW_OPEN_FRAMES	8
#define WINDOW_CLOSE_FRAMES 4

/* Writes code at position in the ring of decoded text and steps position on, */
/* wrapping at the ring's end. */
#define TEXT_APPEND(entries, position, code)                                   \
	((entries)[position] = (code),                                             \
	 (position) = ((position) + 1) & TEXT_BUFFER_MASK)

/* The space a decoded em dash opens with: a plain space in the European */
/* editions, code 143 in the others. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define TEXT_ARGUMENT_SPACE ' '
#else
#define TEXT_ARGUMENT_SPACE 143
#endif

#define MESSAGE_ARGUMENT_COUNT 8 /* the message argument table's places */
/* A name the decoder writes out into a message, decoded first into a buffer */
/* of this many halfwords. */
#define MESSAGE_NAME_HALFWORDS	24
#define TEXT_NUMBER_BUFFER_SIZE 16 /* mnStringFormatNum writes 14 of them */
/* A byte string widened to codes: 255 bytes and the terminator. */
#define TEXT_WIDE_STRING_BUFFER_SIZE 512
#define SELECTION_MENU_ENTRY_LIMIT	 16 /* entries a selection row can list */
#define AREA_NAME_WINDOW_FRAMES		 90 /* how long the area's name shows */

/* WIN_STATE owns three messages, each with four line gaps. */
#define MESSAGE_COUNT			  3
#define MESSAGE_LINE_COUNT		  4 /* lines a page; spacing has one gap each */
#define TEXT_DEFAULT_LINE_SPACING 10 /* a message's line spacing at start */
/* The glyph spacing text starts with (WIN_STATE.letterSpacing): a glyph */
/* sprite sits half of it right of its place in the line. */
#define TEXT_DEFAULT_GLYPH_SPACING 9
/* The glyph tiles' size in sprite VRAM: the four tiles of a 16x16 glyph for */
/* each window sprite. */
#define GLYPH_TILES_SIZE (WINDOW_SPRITE_COUNT * 4 * TILE_SIZE_4BPP)
/* The pixels a line of text takes in a window fitted to it. */
#define TEXT_LINE_HEIGHT 15

/* The control codes of decoded message text (WIN_STATE.textBuf). Codes */
/* from TEXT_SPACE up are characters. */
enum {
	TEXT_END = 0,  /* the text ends; the window stays open */
	TEXT_PAGE = 1, /* wait for a button, then clear the window for a new page */
	TEXT_WAIT_KEY = 2, /* wait for a button, then close the window */
	TEXT_NEWLINE = 3,
	TEXT_PAUSE_LONG = 4,	/* a 60-frame pause */
	TEXT_PAUSE_SHORT = 5,	/* a 20-frame pause a button can cut short */
	TEXT_PAUSE_LONGEST = 6, /* a 120-frame pause a button can cut short */
	TEXT_COLORS_RESET = 7, /* the colour, style and line spacing set at start */
	TEXT_COLOR = 8,		   /* the next entry is the text colour */
	TEXT_OUTLINE = 9,	   /* the next entry is the text style */
	/* The next entry is the line spacing; a glyph sprite sits half of it below
	 */
	/* its line's top. */
	TEXT_LINE_SPACING = 10,
	TEXT_WINDOW_ANIMATION =
		15, /* the next entry is the window animation to run */
	/* MESSAGE_ACTOR_NAME as mnMessageDecodeRaw keeps it, unexpanded: the next
	 */
	/* entry names the unit, and drawing and measuring skip both. */
	TEXT_ACTOR_NAME = 17,
	/* MESSAGE_ACTOR_NAME_START as mnMessageDecodeRaw keeps it, unexpanded: the
	 */
	/* next two entries name the unit and its letters, and drawing and measuring
	 */
	/* skip all three. */
	TEXT_ACTOR_NAME_START = 28,
	/* MESSAGE_ARTICLE_CLASS as mnMessageDecodeRaw keeps it outside Japanese, at
	 */
	/* the head of an item or class name: drawing skips it and its class. */
	TEXT_ARTICLE_CLASS = 29,
	TEXT_END_ALT = 30, /* ends the text as TEXT_END does */
	/* The message runs the codes below it as control codes and draws the rest.
	 */
	TEXT_COMMANDS = 31,
	TEXT_SPACE = 32,
	/* Characters the decoder tests by code, as the fonts draw them: the
	   Japanese */
	/* one by charmap_ja.tsv, the others by Latin-1 and its CP1252 extension. */
	TEXT_OPENING_QUOTE = 142,			  /* “, outside Japanese */
	TEXT_INVERTED_EXCLAMATION_MARK = 161, /* ¡, outside Japanese */
	TEXT_JAPANESE_FULL_STOP = 161,		  /* 。 */
	TEXT_JAPANESE_COMMA = 164,			  /* 、 */
	TEXT_JAPANESE_MIDDLE_DOT = 165,		  /* ・ */
	/* °, outside Japanese the em dash's message code (MESSAGE_EM_DASH) where */
	/* it is left in the text: drawing and measuring skip it. */
	TEXT_EM_DASH = 176,
	TEXT_INVERTED_QUESTION_MARK = 191 /* ¿, outside Japanese */
};

/* The window system's state, as ☀️'s WINDOW_STATE. */
#define WINDOW_STATE ((WIN_STATE *)Ram_HeapSlots->winState)

/* mnWindowFitOnScr's flags. */
typedef enum WINDOW_FIT_FLAG {
	WINDOW_FIT_ALL_PAGES =
		1, /* size the window for every page, not the first */
	/* take the size given, in tiles, rather than measuring the text */
	WINDOW_FIT_SIZE_GIVEN = 2
} WINDOW_FIT_FLAG;

/* The window object's functions its parts call before their definitions, or */
/* with declarations of their own that disagree. */
void mnTextResetStyle();
void mnWindowFitOnScr(s32 message, s32 *windowX, s32 *windowY, u32 *windowWidth,
					  u32 *windowHeight, u16 *spacing, u32 flags);
s32 mnGlyphDraw(WINDOW *window, s32 character, s32 x, s32 y, s32 mode);
s32 mnDebugSceneSelectHandleRowInput(s32 arg, s16 value, s16 *sub, s16 *mode);

/* The stub calls the window object makes into the field, menu and shop modules.
 */
/* Writes an actor's position on screen, in pixels, to out[0] and out[1]; */
/* returns -1 when the actor does not exist. */
s32 fdActorGetScrPosStub(s32 object, s32 *out);
/* The party leader's actor id. */
s32 gmPartyGetLeaderActorStub(void);
/* The cell behavior 16 pixels ahead of the leader when there is something to */
/* search there, else 0. */
s32 fdActorGetTriggerCellAheadOfCurrentStub(void);
/* The field menu's Djinn screen; returns 1 to open the field menu again. */
s32 mnDjinnMenuRunStub(void);
/* The field menu's Psynergy screen. */
s32 mnPsynergyMenuRunStub(void);
/* The field menu's Item screen; returns 1 when an item is to be used. */
s32 mnItemMenuRunStub(void);
/* The field menu's Status screen. */
s32 mnStatScrRunStub(void);
/* Moves a menu cursor to (x, y), gliding as mode says. */
void mnCursorSetStub(void *cursor, s32 x, s32 y, s32 frames);
/* Places a menu cursor at (x, y) at once. */
void mnCursorSetPosImmediateStub(void *cursor, s32 x, s32 y);

/* Tiles the battle lists draw over their window's top edge beside the page */
/* numbers (PAGE_NUMBER_TILE and PAGE_NUMBER_LIT_TILE, window/window.h): the */
/* arrows. */
enum {
	/* Left arrow, tile 820, which mnPageArrowsDraw slides. */
	LIST_TILE_PAGES_LEFT = WINDOW_TILE(820),
	LIST_TILE_PAGES_RIGHT = WINDOW_TILE(821), /* Right arrow, tile 821 */
	/* The Status screen's own two arrows, tiles 822 and 823, which */
	/* mnBattleStatDrawPageArrows slides. */
	STATUS_TILE_PAGES_LEFT = WINDOW_TILE(822),
	STATUS_TILE_PAGES_RIGHT = WINDOW_TILE(823)
};

/* The battle lists' two page arrow tiles in VRAM, which mnPageArrowsDraw */
/* draws each frame (LIST_TILE_PAGES_LEFT and LIST_TILE_PAGES_RIGHT). */
#define LIST_PAGE_ARROW_VRAM                                                   \
	((s32)BG_CHAR_BLOCK(0) +                                                   \
	 (LIST_TILE_PAGES_LEFT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP)
/* The Status screen's arrow tiles in VRAM. */
#define STATUS_PAGE_ARROW_VRAM                                                 \
	((s32)BG_CHAR_BLOCK(0) +                                                   \
	 (STATUS_TILE_PAGES_LEFT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP)

/* Which selection icon records mnSelectionMenuFindFreeIcon looks in. */
typedef enum SELECTION_ICON_GROUP {
	SELECTION_ICON_ROW = 0,	  /* the seven icons of the row */
	SELECTION_ICON_CHOSEN = 1 /* the five icons already chosen; any nonzero */
} SELECTION_ICON_GROUP;

/* The style of sprite text (WIN_STATE.outline, set by TEXT_OUTLINE): its */
/* glyphs outlined, or a motion of the glyph sprites (mnGlyphDraw). */
typedef enum TXT_STYLE {
	TEXT_STYLE_PLAIN = 0,
	TEXT_STYLE_OUTLINED = 1, /* drawn a pixel wider (mnTextDrawGlyphPair) */
	TEXT_STYLE_WOBBLE = 2,
	TEXT_STYLE_SHAKE = 3,
	TEXT_STYLE_POP = 4,
	TEXT_STYLE_SPIN = 5 /* a pixel wider too */
} TXT_STYLE;

/* The two battle command rows mnBattleCommandRowRun runs. */
typedef enum COMMAND_ROW_MODE {
	COMMAND_ROW_PARTY = 0, /* the start of a turn: Fight, Flee and Status */
	COMMAND_ROW_MEMBER =
		1 /* a party member's commands; any mode but 0 is this row */
} COMMAND_ROW_MODE;

/* What mnBattleTargetRunSelection shows on a party member for the action */
/* being aimed (mnBattleClassifyEntryKind), and which party member it starts */
/* on. */
typedef enum BATTLE_TARGET_HINT {
	TARGET_HINT_NONE = 0,
	TARGET_HINT_HP = 1, /* heals HP: the party member's HP */
	TARGET_HINT_PP = 2, /* restores PP: the party member's PP */
	/* Cures poison (ABILITY_EFFECT_CURE_POISON): starts on a poisoned party */
	/* member. */
	TARGET_HINT_CURE_POISON = 3,
	/* Cures what Restore cures (ABILITY_EFFECT_RESTORE): starts on a party */
	/* member who has one of those statuses, and lists them. */
	TARGET_HINT_RESTORE = 4,
	/* Revives: starts on a downed party member. */
	TARGET_HINT_REVIVE = 5,
	/* Cures Restore's statuses, poison and the evil spirit, as Tonic does */
	/* (ABILITY_EFFECT_CURE_ALL). */
	TARGET_HINT_CURE_ALL = 6,
	TARGET_HINT_SET_DJINNI = 7, /* setting a Djinni: "Set Djinni" */
	/* Switch: starts on the party member the last command was aimed at */
	/* (BATTLE_MENU_STATE.lastTgt). */
	TARGET_HINT_LAST_TARGET = 8
} BATTLE_TARGET_HINT;

/* A code carrying the kana mark drawn with its character, in the top bits */
/* (TEXT_MARK_SHIFT, the index into Font_MarkGlyphCodes). */
#define TEXT_CODE_VOICED	 (1 << TEXT_MARK_SHIFT)
#define TEXT_CODE_SEMIVOICED (2 << TEXT_MARK_SHIFT)

/* The command window's top tile row: it runs along the bottom three rows. */
#define COMMAND_ROW_TOP 17
/* A command icon's pixels: 32x32 at a byte a pixel. */
#define COMMAND_ICON_BYTES (32 * 32)

/* A space's width in pixels. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define WINDOW_SPRITE_SPACE_WIDTH 7
#else
#define WINDOW_SPRITE_SPACE_WIDTH 5
#endif

/* A sprite speaker's portrait is numbered PORTRAIT_FIRST_SPRITE_PORTRAIT more
 */
/* than its entry in Portrait_SpriteMap: mnIconLoadWithPalette takes portrait */
/* PORTRAIT_FIRST_SPRITE_PORTRAIT + n from icon PORTRAIT_FIRST_SPRITE_ICON + n.
 */
#define PORTRAIT_FIRST_SPRITE_PORTRAIT 128

/* A battle Djinn list entry, as the battle Status screen's Djinn page and the
 */
/* battle Djinn menu pack it: bits 21..17 the recovery turns left, 16 set for a
 */
/* Djinni not set (on standby or recovering), 11..8 its element and 7..0 its */
/* number (DJINN_COMMAND); BATTLE_DJINN_END ends the list. */
#define BATTLE_DJINN_ELEMENT_SHIFT 8
#define BATTLE_DJINN_ELEMENT	   (0xf << BATTLE_DJINN_ELEMENT_SHIFT)
#define BATTLE_DJINN_NUMBER		   0xff
#define BATTLE_DJINN_UNSET		   0x10000
#define BATTLE_DJINN_TURNS_SHIFT   17
#define BATTLE_DJINN_TURNS		   (0x1f << BATTLE_DJINN_TURNS_SHIFT)
#define BATTLE_DJINN_END		   0x80000000
/* An entry's element. */
#define BATTLE_DJINN_ELEMENT_OF(entry)                                         \
	(((entry) & BATTLE_DJINN_ELEMENT) >> BATTLE_DJINN_ELEMENT_SHIFT)
/* An entry's Djinni, for its name and effect messages. */
#define BATTLE_DJINNI(entry)                                                   \
	(BATTLE_DJINN_ELEMENT_OF(entry) * DJINN_PER_ELEMENT +                      \
	 ((entry) & BATTLE_DJINN_NUMBER))

/* What mnSaveScanFileSummaries adds to its count of files when every file is */
/* cleared (its sendFlg set by saving at the game's end). */
#define SAVE_SCAN_ALL_CLEARED 100

/* Places a battle menu's pointing hand at its list window's selected row: */
/* `position` takes the hand's spot, `margin` pixels left of the window and */
/* 20 below the row's top, and `sprite` becomes a plain 16x16 sprite, its */
/* position still to come. The Djinn, item, summon and Psynergy battle menus */
/* each place their cursor so. */
#define MENU_PLACE_CURSOR(position, window, row, margin, sprite)               \
	do {                                                                       \
		(position).x = (window)->x * 8 - (margin);                             \
		(position).y = ((row) * 2 + (window)->y) * 8 + 20;                     \
		(sprite)->oam.word.attr01 = OBJ_SIZE_16X16;                            \
		(sprite)->oam.word.attr23 = 0;                                         \
	} while(0)

#endif
