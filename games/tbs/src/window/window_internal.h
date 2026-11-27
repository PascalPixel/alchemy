#ifndef ALCHEMY_WINDOW_INTERNAL_H
#define ALCHEMY_WINDOW_INTERNAL_H

/* @file */
/* What every part of the window object shares: its includes, edition values, */
/* macros and the codes of decoded text. window.c includes it before the */
/* parts, and each part includes it too, so that a part read on its own still */
/* resolves. */

#include "ability_ids.h"
#include "enemy_ids.h"
#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle_side.h"
#include "battle/battle_state.h"
#include "field/event.h"
#include "field/particle.h"
#include "graphics/graphics.h"
#include "window/articles.h"
#include "window/glyph_arm.h"
#include "window/message_reader.h"
#include "window/message_speed.h"
#include "window/scales.h"
#include "window/select.h"
#include "window/settings_menu.h"
#include "window/text_font.h"
#include "text/msg_ids.h"

/* Writes code at position in the ring of decoded text and steps position on, */
/* wrapping at the ring's end. */
#define TEXT_APPEND(entries, position, code)                                   \
	((entries)[position] = (code),                                             \
	 (position) = ((position) + 1) & TEXT_BUFFER_MASK)

/* Edition-specific text and window layout values. */
/* The possessives an item's name can take: two in Spanish, four in the other */
/* languages. */
#if LANGUAGE == LANGUAGE_SPANISH
#define ITEM_POSSESSIVE_TEXT_COUNT 2
#else
#define ITEM_POSSESSIVE_TEXT_COUNT 4
#endif

/* What a message's em dash (MESSAGE_EM_DASH) draws before its hyphen. In */
/* English it is the font's code 143, a 9-pixel line at the hyphen's height */
/* that runs into it as one long dash; German, Spanish, French and Italian */
/* write a space, and a space after the hyphen. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define TEXT_EM_DASH_LEAD ' '
#else
#define TEXT_EM_DASH_LEAD 143
#endif

/* A space's width in sprite text, in pixels. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define WINDOW_SPRITE_SPACE_WIDTH 7
#else
#define WINDOW_SPRITE_SPACE_WIDTH 5
#endif

#ifdef MODERN
/* The window code calls these before any declaration of them. */
void mnWindowPutGlyph(WINDOW *win, u32 tile, u32 x, u32 y, s32 mode);
void mnWindowClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);
#endif

/* WIN_STATE owns three messages, each with four line gaps. */
#define MESSAGE_COUNT	   3
#define MESSAGE_LINE_COUNT 4 /* lines a page; spacing has one gap each */
#define TEXT_DEFAULT_COLOR                                                     \
	TEXT_COLOR_WHITE				 /* a message's text colour at start */
#define TEXT_DEFAULT_LINE_SPACING 10 /* a message's line spacing at start */
/* The pixels a line of text takes in a window fitted to it. */
#define TEXT_LINE_HEIGHT 15

/* Window animation lengths. */
#define WINDOW_OPEN_FRAMES	8
#define WINDOW_CLOSE_FRAMES 4

/* Scratch capacities are in bytes unless the name says halfwords. */
#define TEXT_NUMBER_BUFFER_SIZE 16 /* mnStringFormatNum writes 14 of them */
/* A byte string widened to codes: 255 bytes and the terminator. */
#define TEXT_WIDE_STRING_BUFFER_SIZE 512
/* A name the decoder writes out into a message, decoded first into a buffer */
/* of this many halfwords. */
#define MESSAGE_NAME_HALFWORDS	   24
#define MESSAGE_ARGUMENT_COUNT	   8  /* the message argument table's places */
#define SELECTION_MENU_ENTRY_LIMIT 16 /* entries a selection row can list */
#define AREA_NAME_WINDOW_FRAMES	   90 /* how long the area's name shows */

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
	/* TEXT_DEFAULT_COLOR, TEXT_STYLE_PLAIN and TEXT_DEFAULT_LINE_SPACING again
	 */
	/* (the catalogs' reset_text_style). */
	TEXT_RESET_STYLE = 7,
	TEXT_COLOR = 8, /* the next entry is the text colour */
	/* The next entry is the text style (TXT_STYLE). */
	TEXT_STYLE = 9,
	/* The next entry is the line spacing; a glyph sprite sits half of it below
	 */
	/* its line's top. */
	TEXT_LINE_SPACING = 10,
	/* XXX (no evidence): what codes 11 and 12 (one operand) and 14 (two) are */
	/* for, which drawing and measuring skip with their operands; no message of
	 */
	/* The Broken Seal uses them and neither game's decoder names them, so */
	/* nothing the game shows settles it. */
	TEXT_UNUSED_11 = 11,
	TEXT_UNUSED_12 = 12,
	TEXT_UNUSED_14 = 14,
	TEXT_WINDOW_ANIMATION =
		15, /* the next entry is the window animation to run */
	/* MESSAGE_ACTOR_NAME as mnMessageDecodeRaw keeps it, unexpanded: the next
	 */
	/* entry names the unit, and drawing and measuring skip both. */
	TEXT_ACTOR_NAME = 17,
	/* The first letters of a unit's name, a stammer, as The Lost Age's decoder
	 */
	/* writes its MESSAGE_ACTOR_NAME_START: the next two entries are the unit
	   and */
	/* how many letters, and drawing and measuring skip all three, as The Lost
	 */
	/* Age's do. No message of The Broken Seal uses it. */
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
	/* Characters the decoder and the measure test by code, as the fonts draw */
	/* them: the Japanese one by charmap_ja.tsv, the others by Latin-1 and its
	 */
	/* CP1252 extension (tools/ags-text). */
	TEXT_OPENING_QUOTE = 142,			  /* “, outside Japanese */
	TEXT_INVERTED_EXCLAMATION_MARK = 161, /* ¡, outside Japanese */
	TEXT_JAPANESE_FULL_STOP = 161,		  /* 。 */
	TEXT_JAPANESE_COMMA = 164,			  /* 、 */
	TEXT_JAPANESE_MIDDLE_DOT = 165,		  /* ・ */
	TEXT_INVERTED_QUESTION_MARK = 191	  /* ¿, outside Japanese */
};

/* The kana mark drawn with a character, in the top two bits of its code (the */
/* index into Font_MarkGlyphCodes). */
#define TEXT_CODE_MARK_SHIFT 14
#define TEXT_CODE_VOICED	 (1 << TEXT_CODE_MARK_SHIFT)
#define TEXT_CODE_SEMIVOICED (2 << TEXT_CODE_MARK_SHIFT)

/* Which selection icon records mnSelectionMenuFindFreeIcon looks in. */
typedef enum SELECTION_ICON_GROUP {
	SELECTION_ICON_ROW = 0,	  /* the seven icons of the row */
	SELECTION_ICON_CHOSEN = 1 /* the five icons already chosen; any nonzero */
} SELECTION_ICON_GROUP;

/* The style of sprite text (WIN_STATE.outline, set by TEXT_STYLE): its */
/* glyphs outlined, or a motion of the glyph sprites (mnGlyphDraw). */
typedef enum TXT_STYLE {
	TEXT_STYLE_PLAIN = 0,
	TEXT_STYLE_OUTLINED = 1, /* drawn a pixel wider (mnTextDrawGlyphPair) */
	TEXT_STYLE_WOBBLE = 2,	 /* WINDOW_SPRITE_MOTION_WOBBLE */
	TEXT_STYLE_SHAKE = 3,	 /* WINDOW_SPRITE_MOTION_SHAKE */
	TEXT_STYLE_POP = 4,		 /* WINDOW_SPRITE_MOTION_POP */
	TEXT_STYLE_SPIN = 5		 /* WINDOW_SPRITE_MOTION_SPIN, a pixel wider too */
} TXT_STYLE;

/* Outside Japanese, two narrow characters whose glyphs come to this many */
/* pixels at most are drawn together in one 16-pixel cell (mnMessageType, */
/* mnTextDrawWideStringAtOffset). */
#define GLYPH_PAIR_WIDTH_MAX 15
/* As GLYPH_PAIR_WIDTH_MAX, in Spanish, in the outlined style, which widens */
/* each glyph by one. */
#define GLYPH_PAIR_OUTLINED_WIDTH_MAX 14

/* The window animation a message can start (TEXT_WINDOW_ANIMATION): the */
/* window's frame grows by a tile and its text sprites pulse, for the message's
 */
/* countdown. */
#define WINDOW_ANIMATION_SWELL 4

/* A window's WINDOW.state while a message types into it, and once its text */
/* has ended. */
enum {
	WINDOW_TYPING = 0,
	WINDOW_TYPED = 1,
};

/* The pixels between a battle message's lines: the first line is 10 pixels */
/* down its canvas, the second this far below it, and the text then scrolls */
/* up a line at a time. */
#define BATTLE_MESSAGE_LINE_HEIGHT 13

/* How mnWindowFitOnScr sizes a window (its flags). */
enum {
	/* Measure every page and take the widest and tallest
	   (mnTextMeasureAllPages) */
	/* rather than the first page alone. */
	WINDOW_FIT_ALL_PAGES = 1,
	/* Take the width and height given, in tiles, instead of measuring. */
	WINDOW_FIT_GIVEN_SIZE = 2,
};

/* The icon frames of Icon_BasePointerTable (graphics/ui/marks.s) an icon is */
/* drawn over. */
typedef enum ICON_FRAME {
	ICON_FRAME_COMMAND = 0,
	ICON_FRAME_COMMAND_RED = 1,
	ICON_FRAME_ITEM = 2, /* under items and Psynergy alike */
} ICON_FRAME;

/* The marks of Icon_MarkPointers drawn over an item's icon. */
typedef enum ICON_MARK {
	ICON_MARK_EQUIPPED = 0, /* the green E of an equipped item */
	ICON_MARK_BROKEN = 1,	/* the red cross */
	ICON_MARK_CURSED = 2,	/* the white skull of a cursed item worn */
} ICON_MARK;

/* Icon_DigitPointers' first tens digit, 1: the digits 0 to 9 come before it. */
#define ICON_DIGIT_TENS_FIRST 10

/* A sprite cell's affine field (OAM attribute 0's bits 8 and 9) and its shape
 */
/* field, as the window and selection sprites set them. */
enum {
	SPRITE_AFFINE_OFF = 0,
	SPRITE_AFFINE_ON = 1,
	/* Affine in a box twice its size: the sprite stands 8 pixels up and left of
	 */
	/* its plain place for a 16x16 sprite, 16 for a 32x32 one. */
	SPRITE_AFFINE_DOUBLE_SIZE = 3,
};
enum {
	SPRITE_SHAPE_SQUARE = 0, /* size 0 is 8x8, 1 16x16, 2 32x32 */
	SPRITE_SHAPE_WIDE = 1,
	SPRITE_SHAPE_TALL = 2, /* size 0 is 8x16 */
};

/* HEAP_SLOT_ICON_TRANSFER's pointer, kept in IWRAM (system/iwram.s, after */
/* WindowStateWork and PartyStatusState): the glyph transfer buffer icons are */
/* built in. Each icon builder allocates the slot for its build and frees it */
/* after. */
#define GlyphState (*(ICON_TRANSFER **)&HeapSlots[HEAP_SLOT_ICON_TRANSFER])

/* Each message context bank's Huffman models and their offsets, which the */
/* message catalog built from games/tbs/text defines. */
extern const void *const Text_MessageContexts[][2];

#endif
