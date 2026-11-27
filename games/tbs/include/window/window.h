#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/* Windows, the messages that type into them, and the window renderer's
   state. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "tbs_edition.h"
#include "types.h"
#include "graphics/glyph.h"
#include "system/globals.h"
#include "window/screens.h"
#include "window/window_sprite.h"

/* Where the windows are drawn (WIN_STATE.battleMode). */
typedef enum WINDOW_BATTLE_MODE {
	WINDOW_FIELD = 0,  /* on the field; the field setup clears the block */
	WINDOW_BATTLE = 1, /* in battle, from mnWindowSystemInitBattle */
	/* In battle, while mnBattleMessageOpen decodes a battle message: the
	   Japanese decoder then sets names off with spaces and style codes. */
	WINDOW_BATTLE_MESSAGE = 2,
} WINDOW_BATTLE_MODE;

/* A window: a bordered rectangle of the tilemap with its window sprites, its
   opening or closing animation and the rectangle it last drew. */
typedef struct WINDOW {
	WINDOW_SPR_LIST sprs;	/* the sprites the window carries */
	Uint16			width;	/* tiles */
	Uint16			height; /* tiles */
	Uint16			x;		/* tiles */
	Uint16			y;		/* tiles */
	Uint16			opened; /* 1 once opened, 0 once freed; nothing reads it */
	/* The animation a message's TEXT_WINDOW_ANIMATION code sets. */
	Uint16 anm;
	/* 0 while a message is typing into the window; 1 once its text has ended or
	   closed the window, which frees the message for the next text. */
	Uint16 state;
	Uint16 flg; /* WINDOW_FLAG_ flags; 0 once closing or free */
	/* The zoom's stage: counts down from 7 to 0 while opening, and up from 0 to
	   duration while closing. */
	Sint16 zoomStage;
	Sint16 duration;   /* frames of the zoom under way; 0 when none */
	Sint16 prevX;	   /* tiles */
	Sint16 prevY;	   /* tiles */
	Sint16 prevWidth;  /* tiles */
	Sint16 prevHeight; /* tiles */
} WINDOW;

STATIC_ASSERT(sizeof(WINDOW) == 36);
STATIC_ASSERT(offsetof(WINDOW, width) == 8);
STATIC_ASSERT(offsetof(WINDOW, zoomStage) == 24);
STATIC_ASSERT(offsetof(WINDOW, prevHeight) == 34);

/* A message, one of three, which types decoded text into a window a few
   characters a frame and runs its control codes. */
typedef struct MESSAGE {
	WINDOW *win; /* the window typed into; NULL when free */
	Uint16	x;	 /* pen, 8.8 fixed-point pixels */
	Uint16	y;	 /* pen, 8.8 fixed-point pixels */
	/* Extra advance per line, in 1/256 pixels: after every character in
	   Japanese, after a space in the other editions. */
	Uint16 spacing[4];
	Uint16 line;		/* line of the page, indexes spacing; at most 3 */
	Uint16 textIdx;		/* next code in WIN_STATE.textBuf */
	Uint16 countdown;	/* frames left of a pause, a page wait or a swell */
	Uint16 colour;		/* text colour; 15 by default */
	Uint16 outline;		/* outline colour; 0 for none */
	Uint16 lineSpacing; /* 10 by default */
	Uint16 transition;	/* frames left of the canvas scrolling up a line */
	Uint16 startX; /* pen x each new line returns to, 8.8 fixed-point pixels */
	Uint16 ended;  /* 1 once a TEXT_END code is reached */
	/* frames to wait after a glyph while no button is held */
	Uint16 glyphDelay;
	Uint16 flg; /* nonzero keeps the window's size from page to page */
	Uint16 unused;
} MESSAGE;

STATIC_ASSERT(sizeof(MESSAGE) == 40);
STATIC_ASSERT(offsetof(MESSAGE, textIdx) == 18);
STATIC_ASSERT(offsetof(MESSAGE, countdown) == 20);
STATIC_ASSERT(offsetof(MESSAGE, transition) == 28);
STATIC_ASSERT(offsetof(MESSAGE, flg) == 36);

/* Requests transfer of the four-row tilemap band containing window row y. */
#define mnWindowMarkTilemapRow(render, y) \
	do { \
		(render)->dirty |= 2 << ((Uint32)(y) >> 2); \
	} while(0)

/* Window sprites in the shared pool: every sprite the windows show at once,
   glyphs, icons and portraits together. */
enum { WINDOW_SPRITE_COUNT = 64 };

/* WIN_STATE.portraitPictures of a free portrait slot, as mnPortraitClearSlots
   writes it. */
#define PORTRAIT_SLOT_EMPTY 999

/* The sprite palette of portrait slot 0; slot 1 uses the next. */
#define PORTRAIT_PALETTE 14

/* The window tiles text is drawn into: TEXT_TILES from TEXT_TILE_FIRST, and
   with WIN_STATE.extraTextTiles as many more from TEXT_EXTRA_TILE_FIRST. */
#define TEXT_TILE_FIRST		  128
#define TEXT_TILES			  128
#define TEXT_EXTRA_TILE_FIRST 512

/* WIN_STATE.glyphResource when no glyph resource is loaded. */
#define NO_GLYPH_RESOURCE 99

/* The last code the main font draws (Font_Glyphs, from TEXT_SPACE) and the
   window tiles cover; a code above it is a kanji (Font_KanjiGlyphs). */
#define TEXT_LAST_FONT_CODE 255
/* In Japanese, the first kanji's code: the codes after the font's are kanji
   (Font_KanjiGlyphs). */
#define KANJI_FIRST_CODE	(TEXT_LAST_FONT_CODE + 1)

/* The window system's state: the windows' tilemap, the windows, the three text
   messages, the 64 window sprites, and the decoded message and its
   arguments. */
typedef struct WIN_STATE {
	Uint16			tilemap[640]; /* the windows' BG tilemap, 32 by 20 tiles */
	WINDOW			wins[WINDOW_COUNT]; /* 12 in Japanese, 8 in the others */
	MESSAGE			msgs[3]; /* the messages, a NULL window when free */
	WINDOW_SPR		sprs[WINDOW_SPRITE_COUNT];
	WINDOW_SPR_LIST freeSprs; /* the sprites not in any window */
	/* Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once allocated,
	   bit 1 while the tilemap shows it, 0 when free. */
	Uint8 textTileState[2 * TEXT_TILES];
	/* the text tile the next search for a free one starts at */
	Uint16 nextTextTile;
	/* 1 lets text use 256 tiles, adding tiles 512 to 639 */
	Uint8 extraTextTiles;
	/* The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band
	   n. */
	Uint8 dirty;
	/* Set while Mind Read shows thoughts: round border corners, colour 8 text
	   with no shadow, and latched button presses to dismiss text. */
	Uint8  mindRead;
	Uint8  battleMode;		  /* WINDOW_BATTLE_MODE */
	Uint8  holdTilemapUpload; /* while set, the tilemap is not uploaded */
	Uint8  textPal; /* text tiles' palette; 15 by default, 14 or 5 for low HP */
	Uint16 lineSpacing; /* the current message's, copied in to draw */
	Uint16 unusedAfterLineSpacing; /* set to 1 with the text style; not read */
	Uint16 outline; /* the current message's, copied in to draw */
	Uint16 colour;	/* the current message's, copied in to draw */
	Uint16 textBuf[TEXT_BUFFER_MASK + 1]; /* decoded text, a ring of codes */
	Uint16 letterSpacing;				  /* pixels between sprite glyphs; 9 */
	Uint16 textBufEnd; /* where the next code goes in textBuf */
	/* where the last decoded message began in textBuf */
	Uint16 lastMsgStart;
	/* VRAM cache slot of the page arrow; NO_GLYPH_RESOURCE when none. */
	Uint16 glyphResource;
	Uint16 glyphTiles; /* first of the sprite tiles letters are drawn into */
	/* (VRAM_CACHE_ENTRY_GLYPH_TILES) Message argument values
	   (mnMessageArgumentPush). */
	Uint32 argumentValues[8];
	/* the MESSAGE_ARGUMENT_ kind of each value; 0 for a free slot */
	Uint16 argumentKinds[8];
	/* picture shown in each portrait slot; PORTRAIT_SLOT_EMPTY when free */
	Uint16 portraitPictures[2];
	/* VRAM cache slot of each portrait slot's picture */
	Uint16 portraitSprs[2];
	Uint16 voiceSnd;	   /* first of four voice blip sounds; 0 for none */
	Uint16 voiceCountdown; /* characters until the next voice blip */
	Uint8  pageArrowShown; /* 1 while the next-page arrow is shown */
	/* Set while a message waits on its sound: the text ends its pauses and
	   pages once the sound is over. */
	Uint8 waitingForSnd;
	/* 1 for halting speech: a pause before each character, broken off with an
	   ellipsis after the first word (MESSAGE_DECODE_HALTING_SPEECH). */
	Uint8 haltingSpeech;
	/* 1 puts code 222 after certain characters in English and Japanese
	   (MESSAGE_DECODE_SPACING_AFTER). */
	Uint8 spacingAfter;
} WIN_STATE;

/* mnMessageSetDecodeFlags' flags, which set WIN_STATE.haltingSpeech and
   spacingAfter for the messages decoded next. */
typedef enum MESSAGE_DECODE_FLAG {
	MESSAGE_DECODE_HALTING_SPEECH = 1, /* a pause before each character */
	MESSAGE_DECODE_SPACING_AFTER = 2   /* a wide gap after some characters */
} MESSAGE_DECODE_FLAG;

/* The byte of WIN_STATE that Mind Read sets, for the scenes that read it
   through a byte pointer. */
#define WINDOW_STATE_MIND_READ_OFFSET offsetof(WIN_STATE, mindRead)

/* The window renderer's state, heap slot HEAP_SLOT_WINDOW under its own
   name. */
#define WindowStateWork (*(Uint8 *(*)[]) & HeapSlots[HEAP_SLOT_WINDOW])

struct ABILITY_LIST_ENTRY;
struct BATTLE_ACTION;
struct MENU_SPR;
struct WINDOW;
struct WINDOW_SPR;
struct WINDOW_SPR_LIST;
struct SELECTION_NODE;
struct SELECTION_MENU_STATE;
struct MESSAGE_READER;
struct MESSAGE;
struct SETTINGS_MENU_STATE;

/* The flags of a UI window (WINDOW.flg), which mnWindowCreate takes as its
   attributes. */
enum {
	WINDOW_FLAG_OPEN = 0x1,	   /* the window slot is in use */
	WINDOW_FLAG_INSTANT = 0x2, /* opens and closes at once, with no zoom */
	/* Passed with WINDOW_FLAG_INSTANT by the menus (MENU_WINDOW_FLAGS);
	   mnWindowCreate does not keep it. */
	WINDOW_FLAG_UNREAD = 0x4,
	/* text is drawn into a canvas of bitmap tiles
	   (mnWindowMapTextCanvasTiles) */
	WINDOW_FLAG_TEXT_CANVAS = 0x8,
	/* the canvas has a border and a filled background; without it the canvas is
	   transparent and borderless */
	WINDOW_FLAG_BORDERED_CANVAS = 0x20,
	WINDOW_FLAG_OUTLINED_TEXT = 0x40, /* the canvas text gets an outline */
	/* when a new page resizes the window, its bottom edge stays put */
	WINDOW_FLAG_ANCHOR_BOTTOM = 0x80,
	/* when a new page resizes the window, its left edge stays put */
	WINDOW_FLAG_FIXED_X = 0x100,
};

/* The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
   interior of a full-width window six tiles tall. */
#define TEXT_CANVAS_VRAM	   ((void *)0x06002500)
#define TEXT_CANVAS_SIZE	   (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/* The UI tilemap in VRAM (BG screen block 4), which mnWindowSystemUploadTilemap
   copies the state block's tilemap to. */
#define WINDOW_TILEMAP_VRAM ((Uint8 *)0x06002000)

/* The BG palette of the windows and their text, the UI palette. */
#define WINDOW_PALETTE 15

/* The window colour settings (MAIN_ROUTINE_STATE.winHue and winBrightness),
   which mnWindowColApply turns into WINDOW_PALETTE's colours 4 to 10. */
#define WINDOW_COLOR_COUNT		  24 /* "Window color" takes 0 to 23 */
/* "Window brightness" takes 0 to 14. */
#define WINDOW_BRIGHTNESS_COUNT	  15
/* The brightness that adds nothing to the hue's colour. */
#define WINDOW_BRIGHTNESS_NEUTRAL 7
/* Steps round WindowColor_WaveTable's colour wheel; each hue is four. */
#define WINDOW_COLOR_WHEEL_STEPS  96

/* The BG palette the menus keep for their selected row or page: a copy of
   WINDOW_PALETTE with its background lightened (mnPaletteLightenHighlight),
   which the item icons borrow (mnPaletteLoadIconsIntoHighlight). */
#define HIGHLIGHT_PALETTE			14
/* How many levels HIGHLIGHT_PALETTE lightens each channel of
   WINDOW_BACKGROUND_COLOR by (mnPaletteLightenHighlight). */
#define HIGHLIGHT_LIGHTEN_STEPS		9
/* The window palette's background colour, behind its text; in
   HIGHLIGHT_PALETTE, the selected row's. */
#define WINDOW_BACKGROUND_COLOR		4
/* The window palette's white and black, of the font's and the border's tiles,
   and the two greys only the border's tiles use. */
#define WINDOW_WHITE_COLOR			1
#define WINDOW_BLACK_COLOR			3
#define WINDOW_BORDER_DARK_COLOR	11
#define WINDOW_BORDER_LIGHT_COLOR	12
/* Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas or
   a tile a word at a time. */
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444
/* The colour that text drawn in a palette other than WINDOW_PALETTE stands on
   (mnTextDrawStringAsTiles); the menus copy WINDOW_BACKGROUND_COLOR into it. */
#define TEXT_BACKGROUND_COLOR		14
/* The BG palette that battle and the Djinn menu fill with the first basic
   sprite palette, its TEXT_BACKGROUND_COLOR the window background: the element
   icons and the coloured numbers are drawn in it (WIN_STATE.textPal). */
#define BASIC_COLORS_PALETTE		5

/* Text colours (mnTextSetCol), named as both the window palette (through
   Font_ColorPens) and the basic sprite palette show them. */
typedef enum TEXT_COL {
	TEXT_COLOR_RED = 2,
	TEXT_COLOR_YELLOW = 4,
	/* In BASIC_COLORS_PALETTE; WINDOW_PALETTE shows it white. */
	TEXT_COLOR_PINK = 9,
	TEXT_COLOR_GREY = 14,
	TEXT_COLOR_WHITE = 15, /* a message's colour at start */
} TEXT_COL;

/* The OAM shape and size a window sprite is created with (mnWindowSprCreate):
   attribute 0's shape in the low half, attribute 1's size in the high half. */
#define WINDOW_SPRITE_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32		 0x80000000
#define OUTPUT_SIZE_32X8		 0x40004000
#define OUTPUT_SIZE_32X16		 0x80004000
#define OUTPUT_SIZE_16X32		 0x80008000

/* A tilemap entry of WINDOW_PALETTE. */
#define WINDOW_TILE(tile)		(0xf000 | (tile))
/* Two copies of a tilemap entry, for filling a run of entries a word at a
   time. */
#define WINDOW_TILE_PAIR(entry) ((Uint32)(entry) << 16 | (entry))

/* The two page arrows beside a paged list's page numbers, in the menus and the
   File Screen: tiles 296 and 297 of character block 0, which mnPageArrowsDraw
   slides back and forth each frame, and their tilemap entries. */
#define PAGE_ARROW_TILE 296
#define PAGE_ARROW_VRAM \
	((Sint32)BG_CHAR_BLOCK(0) + PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define PAGE_ARROW_LEFT	 WINDOW_TILE(PAGE_ARROW_TILE)
#define PAGE_ARROW_RIGHT WINDOW_TILE(PAGE_ARROW_TILE + 1)

/* The page numbers between the arrows: 20 tiles from this one of character
   block 0, which mnGraphicsExpandVRAMTilesByColTable builds from the UI's
   digits '0' to '9', first with colour 1 drawn as 10, then in their own
   colours; and their tilemap entries, + n giving page n + 1's number, the page
   shown lit. */
#define PAGE_DIGITS_TILE	 768
#define PAGE_NUMBER_TILE	 WINDOW_TILE(PAGE_DIGITS_TILE + 1)
#define PAGE_NUMBER_LIT_TILE WINDOW_TILE(PAGE_DIGITS_TILE + 10 + 1)

/* The window border's tiles. */
enum {
	WINDOW_TILE_BLANK = WINDOW_TILE(0),
	WINDOW_TILE_BORDER_TOP_LEFT = WINDOW_TILE(16),
	WINDOW_TILE_BORDER_TOP = WINDOW_TILE(17),
	WINDOW_TILE_BORDER_TOP_RIGHT = WINDOW_TILE(18),
	WINDOW_TILE_BORDER_BOTTOM_LEFT = WINDOW_TILE(19),
	WINDOW_TILE_BORDER_BOTTOM = WINDOW_TILE(20),
	WINDOW_TILE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(21),
	WINDOW_TILE_BORDER_LEFT = WINDOW_TILE(22),
	WINDOW_TILE_BORDER_RIGHT = WINDOW_TILE(23),
	WINDOW_TILE_BORDER_ROUND_CORNER = WINDOW_TILE(28),
	WINDOW_TILE_WINDOW_FILL = WINDOW_TILE(32),
	/* Divider lines (mnWindowDrawDividerLine) and their joins. a vertical line
	   leaving below */
	WINDOW_TILE_DIVIDER_TEE_DOWN = WINDOW_TILE(9),
	/* a vertical line leaving above */
	WINDOW_TILE_DIVIDER_TEE_UP = WINDOW_TILE(10),
	/* a horizontal line leaving right */
	WINDOW_TILE_DIVIDER_TEE_RIGHT = WINDOW_TILE(11),
	/* a horizontal line leaving left */
	WINDOW_TILE_DIVIDER_TEE_LEFT = WINDOW_TILE(12),
	WINDOW_TILE_DIVIDER_CROSS = WINDOW_TILE(13),
	WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14),
	WINDOW_TILE_DIVIDER_VERTICAL = WINDOW_TILE(15),
	/* the top edge meeting a line */
	WINDOW_TILE_BORDER_TOP_TEE = WINDOW_TILE(24),
	/* the bottom edge meeting a line */
	WINDOW_TILE_BORDER_BOTTOM_TEE = WINDOW_TILE(25),
	/* the left edge meeting a line */
	WINDOW_TILE_BORDER_LEFT_TEE = WINDOW_TILE(26),
	/* the right edge meeting a line */
	WINDOW_TILE_BORDER_RIGHT_TEE = WINDOW_TILE(27),
	/* In battle, the bottom border tiles' lower rows, which
	   mnWindowSystemInitBattle copies to tiles 128 to 130. */
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT = WINDOW_TILE(128),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM = WINDOW_TILE(129),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(130),
	/* In battle, the tile the command row fills its empty panels with
	   (screens_battle_command_row.c), and mnWindowEraseBorderRect below row
	   16. */
	WINDOW_TILE_COMMAND_PANEL = WINDOW_TILE(127),
	/* The HP and PP labels before a number (mnTextDrawPrefixedNumAtOffset) or a
	   Psynergy's cost: each label's first tile, and the second tile the HP
	   label always takes. */
	WINDOW_TILE_HP_LABEL = WINDOW_TILE(29),
	WINDOW_TILE_HP_LABEL_END = WINDOW_TILE(30),
	WINDOW_TILE_PP_LABEL = WINDOW_TILE(31),
};

/* The PP label's second tile: the HP label's in English, German and Japanese, a
   tile of its own in Spanish, French and Italian. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE(5)
#else
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE_HP_LABEL_END
#endif

/* A tilemap entry's tile number, the bits below its flip bits and palette
   number in the hardware's screen entry. */
#define TILE_NUMBER_MASK 0x3ff
/* The tilemap entry flip bits. */
#define TILE_HFLIP		 0x400
#define TILE_VFLIP		 0x800

/* How a window sprite moves (WINDOW_SPR.motion), run each frame by
   mnWindowSprUpdateAll. 0 is a free window sprite. */
enum {
	WINDOW_SPRITE_MOTION_STILL = 1,
	/* the animated next-page arrow, bobbing */
	WINDOW_SPRITE_MOTION_PAGE_ARROW = 2,
	WINDOW_SPRITE_MOTION_WOBBLE = 4, /* swings along a small arc */
	WINDOW_SPRITE_MOTION_SHAKE = 5,	 /* jitters by a pixel every other frame */
	WINDOW_SPRITE_MOTION_POP = 6,	/* shown at double size, then normal size */
	WINDOW_SPRITE_MOTION_SPIN = 7,	/* turns while circling its place */
	WINDOW_SPRITE_MOTION_PULSE = 8, /* shown at 1.25 times, then normal size */
	/* pulses between 1 and 1.5 times its size */
	WINDOW_SPRITE_MOTION_SWELL = 9,
	WINDOW_SPRITE_MOTION_SWELL_HALF = 10, /* the same at half the scale */
	/* shrinks from 1.5 times to its size, once */
	WINDOW_SPRITE_MOTION_SHRINK = 11,
	WINDOW_SPRITE_MOTION_SHRINK_HALF = 12, /* the same at half the scale */
	WINDOW_SPRITE_MOTION_HIDDEN = 13,	   /* not drawn */
	/* Three motions mnWindowSprUpdateAll runs alike: a hop down and back. */
	WINDOW_SPRITE_MOTION_HOP_DOWN = 14,
	WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE = 15,
	WINDOW_SPRITE_MOTION_HOP_DOWN_UNUSED = 16, /* Unused: nothing sets it */
	WINDOW_SPRITE_MOTION_HOP_UP = 17,
	WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT = 18,
};

/* The command icons (ResourceId_CommandIcons), each drawn with the command name
   of the same number (MsgCommandName + icon). */
typedef enum COMMAND_ICON {
	COMMAND_ICON_ATTACK = 0,
	COMMAND_ICON_PSYNERGY = 1,
	COMMAND_ICON_ITEM = 2,
	COMMAND_ICON_DEFEND = 3,
	COMMAND_ICON_FLEE = 4,
	COMMAND_ICON_YES = 5,
	COMMAND_ICON_NO = 6,
	COMMAND_ICON_STATUS = 7,
	COMMAND_ICON_CHECK = 8,
	COMMAND_ICON_USE = 9,
	COMMAND_ICON_GIVE = 10,
	COMMAND_ICON_DROP = 11,
	COMMAND_ICON_SPEAK = 12,
	COMMAND_ICON_EQUIP = 13,
	COMMAND_ICON_FIGHT = 14,
	COMMAND_ICON_DJINN = 15,
	COMMAND_ICON_SUMMON = 16,
	COMMAND_ICON_BUY = 17,
	COMMAND_ICON_SELL = 18,
	COMMAND_ICON_ARTIFACTS = 19,
	COMMAND_ICON_REPAIR = 20,
	COMMAND_ICON_NEW_GAME = 21,
	COMMAND_ICON_CONTINUE = 22,
	COMMAND_ICON_COPY = 23,
	COMMAND_ICON_ERASE = 24,
	COMMAND_ICON_REVIVE = 25,
	COMMAND_ICON_CURE_POISON = 26,
	COMMAND_ICON_REPEL_EVIL = 27,
	COMMAND_ICON_REMOVE_CURSE = 28,
	COMMAND_ICON_BATTLE = 29,
	COMMAND_ICON_SEND = 30,
	COMMAND_ICON_CHANGE = 31,
	COMMAND_ICON_COINS = 32,
	COMMAND_ICON_LUCKY_MEDALS = 33,
	/* The settings menu's choices and the Pause menu's entries
	   (SettingsMenu_SpeedIcons to PauseMenu_Icons), whose command names are
	   placeholders. */
	COMMAND_ICON_SPEED_SLOW = 34,
	COMMAND_ICON_SPEED_NORMAL = 35,
	COMMAND_ICON_SPEED_FAST = 36,
	COMMAND_ICON_SPEECH_OFF = 37,
	COMMAND_ICON_SPEECH_ON = 38,
	COMMAND_ICON_AUTO_SLEEP_OFF = 39,
	COMMAND_ICON_AUTO_SLEEP_ON = 40,
	COMMAND_ICON_SAVE_QUEST = 41,
	COMMAND_ICON_SLEEP = 42,
	COMMAND_ICON_CHANGE_SETTINGS = 43,
	COMMAND_ICON_PASSWORD = 44,
	COMMAND_ICON_CABLE = 45,
	COMMAND_ICON_GOLD = 46,
	COMMAND_ICON_SILVER = 47,
	COMMAND_ICON_BRONZE = 48
} COMMAND_ICON;

/* What mnGlyphDraw and mnWindowPutGlyph draw. */
typedef enum GLYPH_DRAW_MODE {
	GLYPH_DRAW_CHARACTER = 0, /* the character given */
	GLYPH_DRAW_PAGE_ARROW = 1 /* the next-page arrow, in the window's corner */
} GLYPH_DRAW_MODE;

/* The window mnFieldMessageShowCentered shows its message in. */
typedef enum CENTERED_MESSAGE_MODE {
	/* No frame or size: it opens and closes at once. */
	CENTERED_MESSAGE_BARE = 0,
	/* A zooming window sized to the message, waited for as it closes. */
	CENTERED_MESSAGE_WINDOW = 1
} CENTERED_MESSAGE_MODE;

/* The flags mnFieldMessageShow takes. */
typedef enum FIELD_MESSAGE_FLAG {
	FIELD_MESSAGE_ZOOM = 1, /* the window zooms open and shut */
	/* the window state is busy while it shows */
	FIELD_MESSAGE_WAITING_FOR_SOUND = 2,
	FIELD_MESSAGE_KEEP_OPEN = 4,	  /* the window is left open */
	FIELD_MESSAGE_LOWER = 8,		  /* 4 tiles below the top rows' centre */
	FIELD_MESSAGE_HOLD_UPLOAD = 0x20, /* the tilemap upload is held off */
	FIELD_MESSAGE_LOWEST = 0x40		  /* 12 tiles below the top rows' centre */
} FIELD_MESSAGE_FLAG;

/* The Pause menu's entries, as mnPauseMenuChoose returns them (counted from
   Save your quest even when only Change settings shows). */
typedef enum PAUSE_MENU_ENTRY {
	PAUSE_MENU_SAVE_QUEST = 0,
	PAUSE_MENU_SLEEP = 1,
	PAUSE_MENU_CHANGE_SETTINGS = 2,
	/* The debug entries. */
	PAUSE_MENU_ROUGH_MENU = 3,
	PAUSE_MENU_RECOVERY_TEST = 4,
	PAUSE_MENU_RESTART_QUEST = 5
} PAUSE_MENU_ENTRY;

/* The kinds of icon the selection screen lists (SELECTION_NODE.kind) and
   mnSelectionIconLoad, mnSelectionIconLoadIntoSlot and
   mnWindowSprCreateFromResource load. */
typedef enum ICON_KIND {
	ICON_KIND_NONE = 0,	   /* a free selection icon entry */
	ICON_KIND_COMMAND = 1, /* a command's icon, from its number */
	/* An item's icon, with its count above one and its equipped and broken
	   marks (and the artifact mark where a slot is given). */
	ICON_KIND_ITEM = 2,
	ICON_KIND_ABILITY = 4, /* a Psynergy's icon, from its ability number */
	ICON_KIND_YES_NO = 6,  /* a yes or no command, drawn as a command */
	/* An item's icon without the broken mark, for the equipment view. */
	ICON_KIND_EQUIPMENT_ITEM = 7,
	/* one of the other icons (mnIconBuildMiscIconTiles) */
	ICON_KIND_PATTERN = 8,
	/* The one tile mnTitleLoadFirstTile loads. */
	ICON_KIND_TABLE_ENTRY = 9
} ICON_KIND;

/* The layers mnItemIconCompose draws over an item's icon. */
#define ICON_BASE			 1 /* the base under the icon */
#define ICON_COUNT_ABOVE_ONE 2 /* the count, unless it is 1 */
#define ICON_COUNT			 4 /* the count, always */
/* Mark 1, the cross, on an item with INVENTORY_BROKEN. */
#define ICON_BROKEN			 8
/* Mark 0, the equip mark, on an item with INVENTORY_EQUIPPED. */
#define ICON_EQUIPPED		 16
/* Mark 2, Icon_MarkCursed, on a cursed item that cannot be unequipped
   (ITEM_CURSED and ITEM_CANNOT_UNEQUIP) when its code has
   INVENTORY_EQUIPPED. */
#define ICON_CURSED			 32

/* The kinds of message argument (mnMessageArgumentPush), which the message
   codes such as {argument_item_name} read. */
enum {
	MESSAGE_ARGUMENT_NONE = 0, /* an empty place in the argument table */
	MESSAGE_ARGUMENT_ACTOR = 1,
	MESSAGE_ARGUMENT_ITEM = 2,
	MESSAGE_ARGUMENT_CLASS = 3,
	MESSAGE_ARGUMENT_ABILITY = 4,
	MESSAGE_ARGUMENT_NUMBER = 5,
	MESSAGE_ARGUMENT_LOCATION = 6,
};

/* The window module's functions in the order of its stub table
   (src/window/stubs.s). */
void	mnWindowSystemInitField(void);
void	mnWindowSystemInitBattle(Sint32 loadGlyphTiles);
WINDOW *mnWindowCreate(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 attrs);
void   mnWindowClose(WINDOW *window, Sint32 instant);
void   mnMessageDecodeRawUnbounded(Sint32 message, Uint16 *text);
void   mnMessageDecodeRaw(Sint32 message, Uint16 *text, Sint32 capacity);
Sint32 mnMessageCopyExpanded(
	Sint32 message, Uint16 *destination, Uint32 capacity);
WINDOW *mnMessageOpenWindow(Sint32 message, Sint32 x, Sint32 y, Uint32 packed);
void	mnFieldMessageShow(Sint32 message, Sint32 flags);
Bool	mnMessageAllTyped(void);
Bool	mnWindowIsClosed(WINDOW *window);
void	mnMessageSetDecodeFlags(Sint32 flags);
void	mnWindowSprPrepareForRedraw(WINDOW *window);
void	mnWindowClearInteriorTiles(
	   const WINDOW *window, Uint32 left, Uint32 top, Uint32 right, Uint32 bottom);
void mnWindowDrawDividerLine(
	WINDOW *win, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2);
void mnTextDrawMessage(Sint32 message, WINDOW *window, Sint32 x, Sint32 y);
void mnTextDrawMessageAsTiles(
	Sint32 message, const WINDOW *window, Uint32 offsetX, Uint32 offsetY);
void mnTextDrawString(const Uint8 *text, WINDOW *window, Sint32 x, Sint32 y);
void mnTextDrawStringAtOffset(
	const Uint8 *text, const WINDOW *window, Sint32 offsetX, Sint32 offsetY);
void mnTextDrawStringInWindow(
	const Uint8 *text, WINDOW *window, Uint32 x, Uint32 y);
void mnTextDrawNum(
	Sint32 value, Sint32 width, WINDOW *window, Sint32 x, Sint32 y);
void mnTextDrawNumAtOffset(
	Sint32 value, Sint32 width, const WINDOW *window, Sint32 x, Sint32 y);
void mnTextDrawNumInWindow(
	Sint32 value, Sint32 width, WINDOW *window, Sint32 x, Sint32 y);
void		mnTextSetCol(Sint32 color);
void		mnTextSetStyle(Uint16 style);
WINDOW_SPR *mnWindowSprCreate(Sint32 slot, Sint32 oamFlags, WINDOW *window,
	Sint32 offsetX, Sint32 offsetY);
WINDOW_SPR *mnWindowSprCreateFromResource(
	Sint32 mode, Sint32 resource, WINDOW *window, Sint32 x, Sint32 y);
WINDOW_SPR *mnPortraitCreate(Sint32 character, Sint32 unused, Sint32 side,
	WINDOW *window, Sint32 x, Sint32 y);
void		mnPortraitReload(Uint32 side, Sint32 character, Sint32 unused);
void		mnWindowSprSetMotion(WINDOW_SPR *sprite, Sint32 mode);
void		mnWindowSprSetPriority(void *sprite, Sint32 priority);
WINDOW	   *mnPortraitOpenWindow(
		Sint32 speaker, Sint32 portraitOption, Sint32 x, Sint32 y);
void   mnPortraitCloseWindow(Uint32 speaker);
Sint32 mnTextGetMessageSize(
	Sint32 no, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height);
Sint32 mnTextGetMessageSizeAllPages(
	Sint32 no, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height);
void   mnMessageArgumentClearAll(void);
void   mnMessageArgumentPush(Uint32 value, Uint32 kind);
void   mnPartyStatCreateWindow(Sint32 flags);
void   mnPartyStatDraw(Sint32 flags);
void   mnPartyStatClose(void);
void   mnMessageCloseFinishedWindows(void);
void   mnWindowCloseAll(Sint32 unused);
void   mnSelectionMenuCreate(void);
void   mnSelectionMenuClear(void);
void   mnSelectionMenuAddEntry(Uint32 kind, Uint32 base);
void   mnSelectionMenuStartDrawing(void);
void   mnSelectionMenuStopDrawing(void);
void   mnSelectionMenuOpenNameWindow(Sint32 mode, Uint32 count);
void   mnSelectionMenuClose(void);
void   mnSetupSelectionBothSides(void);
Sint32 mnSelectionMenuRun(Sint32 mode);
Uint32 mnSelectionMenuRunWithSounds(Uint32 mode);
void   mnArrowNoOperationCallbackBank(void);
void   mnArrowNoOperationCallbackA(void);
void   mnSelectionMenuBuildEntries(Uint32 unused);
void   mnSelectionMenuReservedNoOperation(void);
Sint32 mnBattleCollectPartyCommands(
	struct BATTLE_ACTION *commands, Uint16 *actors, Sint32 commandCount);
void   mnBattleMessageShowAndWait(Sint32 message);
void   mnBattleMessageOpen(Sint32 message);
void   mnArrowNoOperationCallbackD(void);
Sint32 mnPortraitGetForSpeaker(Uint32 speaker);
Sint32 mnFieldMenuRun(void);
Sint32 mnFileListSelectAction(void);
Sint32 mnIconMenuChooseSendOption(Sint32 mode);
void   mnAreaNameWindowShow(void);
void   mnAreaNameWindowHide(void);
void   mnFieldMessageShowCentered(Sint32 message, Sint32 mode, Sint32 yOffset);
void   mnBattleMessageClose(void);
void   mnMessageResetIdle(void);
void   mnArrowCreate(struct MENU_SPR *sprite, Sint32 *slot);
void   mnArrowSetPos(struct MENU_SPR *sprite, Sint32 x, Sint32 y);
void   mnArrowRelease(Sint32 slot);
void   mnRunSelectionHook(void);
Sint32 mnItemRemoveFromInventory(Sint32 unused, Sint32 itemId);
void   mnIconDrawItem(Sint32 code);
void   mnTextDrawWideStringAtOffset(
	  Uint16 *text, WINDOW *window, Sint32 x, Sint32 y);
void mnSettingsMenuSelectionNoOperation(void);
void mnScanlineWindowFillSpans(void *spans);
void mnWindowSprRedrawSavedRect(WINDOW *window);
void mnWindowSprClearList(void *sprites);
void mnWindowSetTilemapEntry(
	WINDOW *window, Sint32 value, Sint32 x, Sint32 y, Uint32 mode);
#if FIX_FUNCTION_SIGNATURES
void mnDebugSceneSelectRun(void);
#else
Sint16 mnDebugSceneSelectRun(void);
#endif
Sint32 mnDebugFlagEditorRun(void);
Sint32 mnSelectionIconLoadIntoSlot(
	Sint32 kind, Sint32 entry, Sint32 slot, Sint32 baseIcon);
void   mnDebugSelectShortcuts(void);
void   mnWindowMarkVisibleTextTiles(void);
Sint32 mnItemIconCompose(Uint32 code, Uint32 layers);
Sint32 mnPartyStatHighlightMembers(Uint16 *ids);
void   mnIconBuildItemIconTiles(Uint32 icon, Sint32 withBase, Sint32 *cacheSlot,
	  Sint32 *tileOut, Sint32 reuse);
void   mnIconBuildAbilityIconTiles(Uint32 icon, Sint32 withBase,
	  Sint32 *cacheSlot, Sint32 *tileOut, Sint32 reuse);
WINDOW_SPR *mnWindowSprCreateLoaded(
	Sint32 icon, Sint32 layers, WINDOW *window, Sint32 x, Sint32 y);
WINDOW_SPR *mnWindowSprCreateFromTable(
	Sint32 tableEntry, WINDOW *window, Sint32 x, Sint32 y);
Sint16 mnSaveWriteCurrentFile(void);
Sint32 mnSaveToChosenFile(void);
Sint32 mnSaveChooseAndLoadFile(Sint32 mode);
Sint32 mnSaveCopyFile(void);
Sint32 mnSaveEraseSelectedFile(void);
Sint32 mnSaveWriteLinkTallies(void);
Sint32 mnSaveWriteFile(Sint32 file);
Sint32 mnSaveScanFileSummaries(void);
Sint32 mnNameEntryEditMemberName(Sint32 member);
void   mnNameEntryLoadPointingHandTiles(Sint32 slot);
void   mnNameEntryLoadCaretTiles(Sint32 slot);
void   mnSelectionMenuSetPos(Uint32 x, Uint32 y);
Sint32 mnDebugBrowserIcons(void);
Sint32 mnDebugBrowserPortraits(void);
Sint32 mnItemReturnTrue(void);
void   mnDjinnShowJoinedMessage(Sint32 member, Sint32 element, Sint32 djinn);
void   mnWindowColApply(Sint32 phase, Sint32 brightness);
Sint32 mnSettingsMenuRun(void);
Sint32 mnPauseMenuRun(void);
void   mnBgScrFillRect(
	  Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 value);
Sint32 mnSelectShopCommand(Sint32 selection);
Sint32 mnSelectSanctumService(Sint32 selection);
Sint32 mnYesNoMenuRun(
	Sint32 bottom, Sint32 message, Sint32 labelWidth, Sint32 selection);
Sint32 mnSelectCoinsOrMedals(Sint32 selection);
Sint32 mnColossoMenuChoose(Sint32 unused);
Sint32 mnTitleLoadFirstTile(Uint32 entry, Sint32 unused, Uint32 slot);
Sint32 mnYesNoMenuRunAt(Sint32 x, Sint32 y, Sint32 selection);
Sint32 mnPointingHandLoadIntoFreeSlot(void);
void   mnTextMeasureMessage(Sint32 message, Sint32 *x, Sint32 *y);
void   mnWindowHighlightRect(
	  Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 paletteBit);
void mnAbilityBuildIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse);
void   mnGraphicsExpandVRAMTilesByColTable(Uint16 *dst);
void   mnTextUseExtraTiles(Sint32 allowAlternateTiles);
Sint32 mnAnimateSelectionToEntry(Sint32 start, Sint32 command);
Sint32 mnWindowSprGetPointerTiles(Sint32 index);
Sint32 mnMessageOpenDecoded(WINDOW *window, Sint32 message);
Sint32 mnSaveCountFilesExcludingSent(Sint32 excludeSent);
void   mnWindowDrawBorder(Sint32 x, Sint32 y, Uint32 width, Uint32 height);
void   mnWindowEraseBorderRect(Sint32 x, Sint32 y, Uint32 width, Uint32 height);
void   mnPageArrowsDraw(Sint32 offset);
void   mnPartyShowJoinedMessage(Sint32 member);
void   mnPartyShowPairJoinedMessage(Sint32 first, Sint32 second);

/* Some of the window object's functions have no prototype here: the menu object
   (screens.c) calls them unprototyped, with the old argument promotions, and
   screens_internal.h declares them for a current GCC. They are
   mnWindowSprAppendToList, mnMessageStart (the joined-party messages pass five
   of its six arguments), mnTextDrawWideStringInWindow, mnWindowPutGlyph
   (mnPartyStatDraw passes four of its five) and mnTextDrawPrefixedNumAtOffset;
   mnWindowClearTextTilesInRect is called before its definition. */
WINDOW_SPR *mnWindowSprAcquireFree(void);
void		mnWindowSprReleaseFree(WINDOW_SPR *out);
void		mnWindowSprInitFreeList(void);
void		mnWindowSystemUploadTilemap(void);
void		mnWindowDrawOpened(WINDOW *window);
void		mnWindowWaitZoom(WINDOW *window);
void		mnWindowSprUpdateListTail(WINDOW_SPR_LIST *list);
void		mnWindowSprRelease(WINDOW_SPR *entry);
MESSAGE *mnBattleMessageCont(WINDOW *window, Sint32 entry, Sint32 keepPosition);
Sint32	 mnTextClearCanvas(void);
Sint32	 mnTextFillCanvasBackground(void);
void	 mnMessageApplyStyle(MESSAGE *message);
void	 mnTextStartCanvasScroll(MESSAGE *message);
void	 mnTextScrollCanvasUp(Sint32 step);
void	 mnMessageUpdateAll(void);
Sint32	 mnMessageType(MESSAGE *message);
void	 mnWindowUpdateZooms(void);
void	 mnWindowDrawZoomBorder(void *windowArg, Sint32 savePosition);
Uint16	*mnMemoryFillHalfwordsDMA(Uint16 *dst, Sint32 value, Sint32 count);
void	 mnWindowMapTextCanvasTiles(
		Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 fullWidth);
/* Declared without a prototype, so that mnWindowCreate can pass it an argument
   it never reads. */
void mnTextResetStyle();
void mnWindowSystemStartFieldUpdate(void);
void mnWindowSystemStartBattleUpdate(Sint32 loadGlyphTiles);
/* The bob of a prompt or cursor: up and down by up to 2 pixels, a sine every 16
   frames. */
#define CURSOR_BOB_Y() (sySin(FrameCount * (TRIG_TURN / 16)) / (toFIXED(1) / 2))

/* mnMessageOpenWindow's packed argument: the speaker's voice in bits 16 to 27,
   above the window options. */
#define MESSAGE_WINDOW_VOICE(voice)		((voice) << 16)
/* The speaker's voice in a packed argument, as mnMessageOpenWindow takes it
   out. */
#define MESSAGE_WINDOW_VOICE_OF(packed) (((Uint32)(packed) << 4) >> 20)
/* The window options of a packed argument: the bits below the speaker's
   voice. */
#define MESSAGE_WINDOW_OPTIONS_MASK		(MESSAGE_WINDOW_VOICE(1) - 1)

/* mnMessageOpenWindow's window options, in packed's low bits. */
typedef enum MESSAGE_WINDOW_OPTION {
	MESSAGE_WINDOW_ZOOM = 0x01, /* zoom open rather than appear at once */
	/* Set by the shopkeepers' windows and some others; mnMessageOpenWindow
	   never reads it. */
	MESSAGE_WINDOW_UNREAD = 0x02,
	MESSAGE_WINDOW_TEXT_CANVAS = 0x08,	 /* WINDOW_FLAG_TEXT_CANVAS */
	MESSAGE_WINDOW_ANCHOR_BOTTOM = 0x10, /* WINDOW_FLAG_ANCHOR_BOTTOM */
	MESSAGE_WINDOW_FIXED_X = 0x20		 /* WINDOW_FLAG_FIXED_X */
} MESSAGE_WINDOW_OPTION;

void mnWindowSystemUpdate(void);
#if LANGUAGE != LANGUAGE_JAPANESE
Sint32 mnTextDrawGlyphPair(Sint32 code, Uint32 *out);
#else
Sint32 mnTextDrawGlyphPair(Uint32 code, Uint32 *out);
#endif
Sint32 mnTextGetWideStringWidth(Uint16 *text);
void   mnTextDrawWideStringAsTiles(
	  Uint16 *buffer, const WINDOW *window, Sint32 offsetX, Sint32 offsetY);
Uint8 *mnStringFormatNum(Uint8 *buffer, Sint32 input, Sint32 width);
Uint32 mnMessageAppendArticleName(Sint32 inBattleMessage, Uint16 *name,
	Uint32 writePosition, Uint16 *entries, Sint32 articleForm, Sint32 plural,
	Sint32 *lastLetter);
Sint32 mnMessageDecode(Sint32 script, Sint32 clear);
void   mnWindowFitOnScreen(Sint32 entryIndex, Sint32 *windowX, Sint32 *windowY,
	  Uint32 *windowWidth, Uint32 *windowHeight, Uint16 *spacing, Uint32 flags);
void   mnTextMeasureDecoded(
	  Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing);
void mnTextMeasureAllPages(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing);
Sint32 mnGlyphDraw(
	WINDOW *window, Sint32 character, Sint32 x, Sint32 y, Sint32 mode);

/* How mnWindowSetTilemapEntry writes its value (its mode argument): as given,
   or as a tile number in one of the palettes the menus keep for their
   windows. */
typedef enum TILEMAP_ENTRY_MODE {
	TILEMAP_ENTRY_PLAIN, /* as given, palette bits included */
	TILEMAP_ENTRY_NONE,	 /* nothing */
	/* In HIGHLIGHT_PALETTE, the highlight mnPaletteLightenHighlight makes for
	   the selected row or page. */
	TILEMAP_ENTRY_HIGHLIGHT,
	TILEMAP_ENTRY_WINDOW, /* in WINDOW_PALETTE */
	/* In palette 1, which the Status screen and the party rearranging fill with
	   the element icons' colours for the rows that are not highlighted
	   (mnStatScreenRun, mnStatScreenRunPartyRow). */
	TILEMAP_ENTRY_ICON
} TILEMAP_ENTRY_MODE;

void	mnWindowSprUpdateScaleAnimation(WINDOW_SPR *windowSprite);
void	mnWindowSprUpdateAll(void);
Sint32 *mnMemoryClearWordIfSet(Sint32 *p);
void	mnWindowStepSwellEffect(MESSAGE *message);
Uint32	mnMessageArgumentLookup(Uint32 kind, Uint32 clear);
Bool	mnTextCheckPauseSkipped(MESSAGE *message);
Bool	mnTextCheckTextDismissed(MESSAGE *message);
Sint32	mnMessageDecodeAndClearArguments(Sint32 message);
void	mnMessageFind(struct MESSAGE_READER *reader, Sint32 message);
Sint32	mnTextDecodeSymbolInC(struct MESSAGE_READER *reader);
void	mnPortraitClearSlots(void);

#define WINDOW_STATE ((WIN_STATE *)WindowStateWork[0])

/* The action id in a PSYNERGY_ENTRY's encodedAction, below the marks for an
   action the class or an equipped item gave (PSYNERGY_ABILITY_MASK). */
#define ACTION_ID_MASK 0x3fff

/* Defines name(no, x, y, width, height): decodes message no and fits a window
   for it on the screen near tile (x, y) (mnWindowFitOnScreen with flags),
   returning its place and size in tiles. Returns 0 for an empty message. */
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags) \
	Sint32 name( \
		Sint32 no, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height) \
	{ \
		WIN_STATE *state; \
		Sint32	   idx; \
\
		state = WINDOW_STATE; \
		idx = mnMessageDecode(no, 0); \
		if(state->textBuf[idx] == 0) { \
			return 0; \
		} \
		mnWindowFitOnScreen(idx, x, y, width, height, 0, flags); \
		return 1; \
	}

Sint32 mnIconCountItemIcons(void);
Sint32 mnIconCountPsynergyIcons(void);
void mnIconBuildCommandIconTiles(Sint32 overlayIcon, Sint32 frame, Sint32 *slot,
	Sint32 *tileOut, Sint32 reuse);
void mnAbilityBuildItemIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse);
Sint32 mnItemIconLoadLayersIntoSlot(
	Sint32 inventoryCode, Sint32 layers, Sint32 cacheSlot);
Sint32 mnIconBuildMiscIconTiles(Sint32 icon, Sint32 unused, Sint32 cacheSlot);
void   mnIconDrawMisc(Uint32 iconIndex);
void   mnIconLoadWithPalette(Uint32 icon, Sint32 unused, Sint32 *slot,
	  Sint32 *tile, Sint32 palette, Sint32 reuse);
void   mnIconReservedNoOperation(void);
void   mnIconDecode(ICON_TRANSFER *glyph, Sint32 overlay);

struct SELECTION_NODE *mnSelectionMenuFindFreeIcon(Sint32 kind);
void				   mnSelectionMenuDraw(void);
void mnSelectionMenuDrawArrow(struct SELECTION_MENU_STATE *state, Sint32 side);
void mnSelectionMenuSetupArrow(struct SELECTION_MENU_STATE *state, Sint32 side);
struct SELECTION_NODE *mnSelectionMenuGetCursorNode(
	struct SELECTION_MENU_STATE *state);
void mnMoveSelectionForward(struct SELECTION_MENU_STATE *state);
void mnMoveSelectionBackward(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuStepRight(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuStepLeft(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuRedrawIcon(
	struct SELECTION_MENU_STATE *state, Uint32 rowIndex);
void mnSelectionMenuHighlightIcon(
	struct SELECTION_MENU_STATE *state, Uint32 rowIndex);
void mnClearBattleHighlight(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuScroll(struct SELECTION_MENU_STATE *state, Sint32 forward);
Sint32 mnSelectionIconLoad(Sint32 mode, Sint32 value);
void   mnSelectionMenuSetupIcon(
	  Uint32 kind, Sint32 base, struct SELECTION_NODE *node, Sint32 reuse);
Uint32 mnConfirmSelection(struct SELECTION_MENU_STATE *state);
void   mnArrowNoOperationCallback(void);
void   mnArrowNoOperationCallbackB(void);
void   mnArrowNoOperationCallbackC(void);
void   mnSelectionMenuLoadLargeIcon(void);
void   mnSelectionMenuFreeLargeIcon(void);
void   mnPartyStatOpenAndWait(void);
/* UB: mnFieldMenuRun calls it with no prototype and uses the result, though it
   returns nothing. */
#if FIX_FUNCTION_SIGNATURES
void mnPartyStatCloseWithMenu(void);
#endif
void   mnSelectionMenuCreateEmpty(void);
Uint32 mnRunSelectionForValue(Uint32 initialIndex);

void   mnAreaNameWindowUpdate(void);
void   mnSettingsMenuStepWindowHue(Sint32 keys);
Sint32 mnPartyListPsynergy(struct ABILITY_LIST_ENTRY *output);
void   mnFindShortcutEntries(Uint32 *firstIndex, Uint32 *secondIndex,
	  const struct ABILITY_LIST_ENTRY *entries);

void   mnAllocateSelectionState(void);
void   mnReleaseSelectionState(void);
void   mnSettingsMenuReservedNoOperationA(void);
void   mnSettingsMenuReservedNoOperationB(void);
void   mnSettingsMenuReservedNoOperationC(void);
void   mnSettingsMenuResetEntries(struct SETTINGS_MENU_STATE *optionMenuState);
void   mnWindowColCalcTint(struct SETTINGS_MENU_STATE *state);
void   mnWindowColWritePalette(struct SETTINGS_MENU_STATE *state);
Uint16 mnWindowColScaleTint(struct SETTINGS_MENU_STATE *state, FIXED redScale,
	FIXED greenScale, FIXED blueScale);
Uint16 mnColScaleComponents(
	Sint16 *rgb, FIXED redScale, FIXED greenScale, FIXED blueScale);
void	mnSettingsMenuPreviousRow(struct SETTINGS_MENU_STATE *state);
void	mnSettingsMenuNextRow(struct SETTINGS_MENU_STATE *state);
void	mnSettingsMenuLowerSetting(struct SETTINGS_MENU_STATE *state);
void	mnSettingsMenuRaiseSetting(struct SETTINGS_MENU_STATE *state);
void	mnSettingsMenuAnimateCursors(void);
void	mnSettingsMenuInitState(void);
void	mnSettingsMenuRelease(void);
WINDOW *mnSettingsMenuOpen(void);
void	mnPauseMenuAnimateCursor(void);
void	mnPauseMenuInit(void);
void	mnPauseMenuRelease(void);
WINDOW *mnPauseMenuOpen(void);
Sint32	mnPauseMenuChoose(void);

void mnWindowOverlayTilePattern(
	Uint16 *entry, Uint16 *mirror, Sint32 pattern, Uint8 *remap);
Sint32 mnTextDrawStringAsTiles(
	Uint16 *text, Uint16 *tiles, Uint16 *mirror, Sint32 x);
void mnWindowReservedNoOperation(void);

/* The menus' compressed 64x16 headings, Save to Setup (menu_titles.s). */
extern const Uint8 *MenuTitlePictures[];

#endif
