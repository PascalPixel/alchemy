#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

/* @file */
/* Windows, the messages that type into them, and the window renderer's */
/* state. */

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
	/* In battle, while mnBattleMessageOpen decodes a battle message: the */
	/* Japanese decoder then sets names off with spaces and style codes. */
	WINDOW_BATTLE_MESSAGE = 2,
} WINDOW_BATTLE_MODE;

/* A window: a bordered rectangle of the tilemap with its window sprites, its */
/* opening or closing animation and the rectangle it last drew. */
typedef struct WINDOW {
	WINDOW_SPR_LIST sprs; /* the sprites the window carries */
	u16 width;			  /* tiles */
	u16 height;			  /* tiles */
	u16 x;				  /* tiles */
	u16 y;				  /* tiles */
	u16 opened;			  /* 1 once opened, 0 once freed; nothing reads it */
	/* The animation a message's TEXT_WINDOW_ANIMATION code sets. */
	u16 anm;
	/* 0 while a message is typing into the window; 1 once its text has */
	/* ended or closed the window, which frees the message for the next text. */
	u16 state;
	u16 flg; /* WINDOW_FLAG_ flags; 0 once closing or free */
	/* The zoom's stage: counts down from 7 to 0 while opening, and up from 0 to
	 */
	/* duration while closing. */
	s16 zoomStage;
	s16 duration;	/* frames of the zoom under way; 0 when none */
	s16 prevX;		/* tiles */
	s16 prevY;		/* tiles */
	s16 prevWidth;	/* tiles */
	s16 prevHeight; /* tiles */
} WINDOW;

STATIC_ASSERT(sizeof(WINDOW) == 36);
STATIC_ASSERT(offsetof(WINDOW, width) == 8);
STATIC_ASSERT(offsetof(WINDOW, zoomStage) == 24);
STATIC_ASSERT(offsetof(WINDOW, prevHeight) == 34);

/* A message, one of three, which types decoded text into a window a few */
/* characters a frame and runs its control codes. */
typedef struct MESSAGE {
	WINDOW *win; /* the window typed into; NULL when free */
	u16 x;		 /* pen, 8.8 fixed-point pixels */
	u16 y;		 /* pen, 8.8 fixed-point pixels */
	/* Extra advance per line, in 1/256 pixels: after every character in */
	/* Japanese, after a space in the other editions. */
	u16 spacing[4];
	u16 line;		 /* line of the page, indexes spacing; at most 3 */
	u16 textIdx;	 /* next code in WIN_STATE.textBuf */
	u16 countdown;	 /* frames left of a pause, a page wait or a swell */
	u16 colour;		 /* text colour; 15 by default */
	u16 outline;	 /* outline colour; 0 for none */
	u16 lineSpacing; /* 10 by default */
	u16 transition;	 /* frames left of the canvas scrolling up a line */
	u16 startX;		/* pen x each new line returns to, 8.8 fixed-point pixels */
	u16 ended;		/* 1 once a TEXT_END code is reached */
	u16 glyphDelay; /* frames to wait after a glyph while no button is held */
	u16 flg;		/* nonzero keeps the window's size from page to page */
	u16 unused;
} MESSAGE;

STATIC_ASSERT(sizeof(MESSAGE) == 40);
STATIC_ASSERT(offsetof(MESSAGE, textIdx) == 18);
STATIC_ASSERT(offsetof(MESSAGE, countdown) == 20);
STATIC_ASSERT(offsetof(MESSAGE, transition) == 28);
STATIC_ASSERT(offsetof(MESSAGE, flg) == 36);

/* Requests transfer of the four-row tilemap band containing window row y. */
#define mnWindowMarkTilemapRow(render, y)                                      \
	do {                                                                       \
		(render)->dirty |= 2 << ((u32)(y) >> 2);                               \
	} while(0)

/* Window sprites in the shared pool: every sprite the windows show at once, */
/* glyphs, icons and portraits together. */
enum { WINDOW_SPRITE_COUNT = 64 };

/* WIN_STATE.portraitPictures of a free portrait slot, as */
/* mnPortraitClearSlots writes it. */
#define PORTRAIT_SLOT_EMPTY 999

/* The sprite palette of portrait slot 0; slot 1 uses the next. */
#define PORTRAIT_PALETTE 14

/* The window tiles text is drawn into: TEXT_TILES from TEXT_TILE_FIRST, and */
/* with WIN_STATE.extraTextTiles as many more from TEXT_EXTRA_TILE_FIRST. */
#define TEXT_TILE_FIRST		  128
#define TEXT_TILES			  128
#define TEXT_EXTRA_TILE_FIRST 512

/* WIN_STATE.glyphResource when no glyph resource is loaded. */
#define NO_GLYPH_RESOURCE 99

/* The last code the main font draws (Font_Glyphs, from TEXT_SPACE) and the */
/* window tiles cover; a code above it is a kanji (Font_KanjiGlyphs). */
#define TEXT_LAST_FONT_CODE 255
/* In Japanese, the first kanji's code: the codes after the font's are kanji */
/* (Font_KanjiGlyphs). */
#define KANJI_FIRST_CODE (TEXT_LAST_FONT_CODE + 1)

/* The window system's state: the windows' tilemap, the windows, the three text
 */
/* messages, the 64 window sprites, and the decoded message and its arguments.
 */
typedef struct WIN_STATE {
	u16 tilemap[640];		   /* the windows' BG tilemap, 32 by 20 tiles */
	WINDOW wins[WINDOW_COUNT]; /* 12 in Japanese, 8 in the others */
	MESSAGE msgs[3];		   /* the messages, a NULL window when free */
	WINDOW_SPR sprs[WINDOW_SPRITE_COUNT];
	WINDOW_SPR_LIST freeSprs; /* the sprites not in any window */
	/* Per text tile (tiles 128 to 255, then 512 to 639): bit 0 once allocated,
	 */
	/* bit 1 while the tilemap shows it, 0 when free. */
	u8 textTileState[2 * TEXT_TILES];
	u16 nextTextTile;  /* the text tile the next search for a free one starts at
						*/
	u8 extraTextTiles; /* 1 lets text use 256 tiles, adding tiles 512 to 639 */
	/* The tilemap's 4-row bands to upload: bit 0 for all, bit n + 1 for band n.
	 */
	u8 dirty;
	/* Set while Mind Read shows thoughts: round border corners, colour 8 text
	 */
	/* with no shadow, and latched button presses to dismiss text. */
	u8 mindRead;
	u8 battleMode;		  /* WINDOW_BATTLE_MODE */
	u8 holdTilemapUpload; /* while set, the tilemap is not uploaded */
	u8 textPal; /* text tiles' palette; 15 by default, 14 or 5 for low HP */
	u16 lineSpacing;			/* the current message's, copied in to draw */
	u16 unusedAfterLineSpacing; /* set to 1 with the text style; not read */
	u16 outline;				/* the current message's, copied in to draw */
	u16 colour;					/* the current message's, copied in to draw */
	u16 textBuf[TEXT_BUFFER_MASK + 1]; /* decoded text, a ring of codes */
	u16 letterSpacing;				   /* pixels between sprite glyphs; 9 */
	u16 textBufEnd;					   /* where the next code goes in textBuf */
	u16 lastMsgStart; /* where the last decoded message began in textBuf
					   */
	/* VRAM cache slot of the page arrow; NO_GLYPH_RESOURCE when none. */
	u16 glyphResource;
	u16 glyphTiles; /* first of the sprite tiles letters are drawn into */
	/* (VRAM_CACHE_ENTRY_GLYPH_TILES) */
	/* Message argument values (mnMessageArgumentPush). */
	u32 argumentValues[8];
	u16 argumentKinds[8]; /* the MESSAGE_ARGUMENT_ kind of each value; 0 for a
						   */
						  /* free slot */
	u16 portraitPictures[2]; /* picture shown in each portrait slot; */
							 /* PORTRAIT_SLOT_EMPTY when free */
	u16 portraitSprs[2];	 /* VRAM cache slot of each portrait slot's picture
							  */
	u16 voiceSnd;			 /* first of four voice blip sounds; 0 for none */
	u16 voiceCountdown;		 /* characters until the next voice blip */
	u8 pageArrowShown;		 /* 1 while the next-page arrow is shown */
	/* Set while a message waits on its sound: the text ends its pauses and */
	/* pages once the sound is over. */
	u8 waitingForSnd;
	/* 1 for halting speech: a pause before each character, broken off with an
	 */
	/* ellipsis after the first word (MESSAGE_DECODE_HALTING_SPEECH). */
	u8 haltingSpeech;
	/* 1 puts code 222 after certain characters in English and Japanese */
	/* (MESSAGE_DECODE_SPACING_AFTER). */
	u8 spacingAfter;
} WIN_STATE;

/* mnMessageSetDecodeFlags' flags, which set WIN_STATE.haltingSpeech and */
/* spacingAfter for the messages decoded next. */
typedef enum MESSAGE_DECODE_FLAG {
	MESSAGE_DECODE_HALTING_SPEECH = 1, /* a pause before each character */
	MESSAGE_DECODE_SPACING_AFTER = 2   /* a wide gap after some characters */
} MESSAGE_DECODE_FLAG;

/* The byte of WIN_STATE that Mind Read sets, for the scenes that read it */
/* through a byte pointer. */
#define WINDOW_STATE_MIND_READ_OFFSET offsetof(WIN_STATE, mindRead)

/* The window renderer's state, heap slot HEAP_SLOT_WINDOW under its own name.
 */
#define WindowStateWork (*(u8 *(*)[]) & HeapSlots[HEAP_SLOT_WINDOW])

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

/* The flags of a UI window (WINDOW.flg), which mnWindowCreate */
/* takes as its attributes. */
enum {
	WINDOW_FLAG_OPEN = 0x1,	   /* the window slot is in use */
	WINDOW_FLAG_INSTANT = 0x2, /* opens and closes at once, with no zoom */
	/* Passed with WINDOW_FLAG_INSTANT by the menus (MENU_WINDOW_FLAGS); */
	/* mnWindowCreate does not keep it. */
	WINDOW_FLAG_UNREAD = 0x4,
	/* text is drawn into a canvas of bitmap tiles (mnWindowMapTextCanvasTiles)
	 */
	WINDOW_FLAG_TEXT_CANVAS = 0x8,
	/* the canvas has a border and a filled background; without it the canvas is
	 */
	/* transparent and borderless */
	WINDOW_FLAG_BORDERED_CANVAS = 0x20,
	WINDOW_FLAG_OUTLINED_TEXT = 0x40, /* the canvas text gets an outline */
	/* when a new page resizes the window, its bottom edge stays put */
	WINDOW_FLAG_ANCHOR_BOTTOM = 0x80,
	/* when a new page resizes the window, its left edge stays put */
	WINDOW_FLAG_FIXED_X = 0x100,
};

/* The text canvas tiles in VRAM: 120 4bpp tiles from tile 296, enough for the
 */
/* interior of a full-width window six tiles tall. */
#define TEXT_CANVAS_VRAM	   ((void *)0x06002500)
#define TEXT_CANVAS_SIZE	   (120 * TILE_SIZE_4BPP)
#define TEXT_CANVAS_FIRST_TILE 296

/* The UI tilemap in VRAM (BG screen block 4), which */
/* mnWindowSystemUploadTilemap copies the state block's tilemap to. */
#define WINDOW_TILEMAP_VRAM ((u8 *)0x06002000)

/* The BG palette of the windows and their text, the UI palette. */
#define WINDOW_PALETTE 15

/* The window colour settings (MAIN_ROUTINE_STATE.winHue and
 * winBrightness),
 */
/* which mnWindowColApply turns into WINDOW_PALETTE's colours 4 to 10. */
#define WINDOW_COLOR_COUNT 24 /* "Window color" takes 0 to 23 */
/* "Window brightness" takes 0 to 14. */
#define WINDOW_BRIGHTNESS_COUNT 15
/* The brightness that adds nothing to the hue's colour. */
#define WINDOW_BRIGHTNESS_NEUTRAL 7
/* Steps round WindowColor_WaveTable's colour wheel; each hue is four. */
#define WINDOW_COLOR_WHEEL_STEPS 96

/* The BG palette the menus keep for their selected row or page: a copy of */
/* WINDOW_PALETTE with its background lightened (mnPaletteLightenHighlight), */
/* which the item icons borrow (mnPaletteLoadIconsIntoHighlight). */
#define HIGHLIGHT_PALETTE 14
/* How many levels HIGHLIGHT_PALETTE lightens each channel of */
/* WINDOW_BACKGROUND_COLOR by (mnPaletteLightenHighlight). */
#define HIGHLIGHT_LIGHTEN_STEPS 9
/* The window palette's background colour, behind its text; in */
/* HIGHLIGHT_PALETTE, the selected row's. */
#define WINDOW_BACKGROUND_COLOR 4
/* The window palette's white and black, of the font's and the border's tiles,
 */
/* and the two greys only the border's tiles use. */
#define WINDOW_WHITE_COLOR		  1
#define WINDOW_BLACK_COLOR		  3
#define WINDOW_BORDER_DARK_COLOR  11
#define WINDOW_BORDER_LIGHT_COLOR 12
/* Eight 4bpp pixels of WINDOW_BACKGROUND_COLOR, for filling the text canvas */
/* or a tile a word at a time. */
#define TEXT_CANVAS_BACKGROUND_WORD 0x44444444
/* The colour that text drawn in a palette other than WINDOW_PALETTE stands */
/* on (mnTextDrawStringAsTiles); the menus copy WINDOW_BACKGROUND_COLOR into */
/* it. */
#define TEXT_BACKGROUND_COLOR 14
/* The BG palette that battle and the Djinn menu fill with the first basic */
/* sprite palette, its TEXT_BACKGROUND_COLOR the window background: the element
 */
/* icons and the coloured numbers are drawn in it (WIN_STATE.textPal). */
#define BASIC_COLORS_PALETTE 5

/* Text colours (mnTextSetCol), named as both the window palette (through */
/* Font_ColorPens) and the basic sprite palette show them. */
typedef enum TEXT_COL {
	TEXT_COLOR_RED = 2,
	TEXT_COLOR_YELLOW = 4,
	/* In BASIC_COLORS_PALETTE; WINDOW_PALETTE shows it white. */
	TEXT_COLOR_PINK = 9,
	TEXT_COLOR_GREY = 14,
	TEXT_COLOR_WHITE = 15, /* a message's colour at start */
} TEXT_COL;

/* The OAM shape and size a window sprite is created with */
/* (mnWindowSprCreate): attribute 0's shape in the low half, attribute 1's */
/* size in the high half. */
#define WINDOW_SPRITE_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32		 0x80000000
#define OUTPUT_SIZE_32X8		 0x40004000
#define OUTPUT_SIZE_32X16		 0x80004000
#define OUTPUT_SIZE_16X32		 0x80008000

/* A tilemap entry of WINDOW_PALETTE. */
#define WINDOW_TILE(tile) (0xf000 | (tile))
/* Two copies of a tilemap entry, for filling a run of entries a word at a */
/* time. */
#define WINDOW_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

/* The two page arrows beside a paged list's page numbers, in the menus and */
/* the File Screen: tiles 296 and 297 of character block 0, which */
/* mnPageArrowsDraw slides back and forth each frame, and their */
/* tilemap entries. */
#define PAGE_ARROW_TILE 296
#define PAGE_ARROW_VRAM                                                        \
	((s32)BG_CHAR_BLOCK(0) + PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define PAGE_ARROW_LEFT	 WINDOW_TILE(PAGE_ARROW_TILE)
#define PAGE_ARROW_RIGHT WINDOW_TILE(PAGE_ARROW_TILE + 1)

/* The page numbers between the arrows: 20 tiles from this one of character */
/* block 0, which mnGraphicsExpandVramTilesByColTable builds from the UI's */
/* digits '0' to '9', first with colour 1 drawn as 10, then in their own */
/* colours; and their tilemap entries, + n giving page n + 1's number, the */
/* page shown lit. */
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
	/* Divider lines (mnWindowDrawDividerLine) and their joins. */
	WINDOW_TILE_DIVIDER_TEE_DOWN =
		WINDOW_TILE(9), /* a vertical line leaving below */
	WINDOW_TILE_DIVIDER_TEE_UP =
		WINDOW_TILE(10), /* a vertical line leaving above */
	/* a horizontal line leaving right */
	WINDOW_TILE_DIVIDER_TEE_RIGHT = WINDOW_TILE(11),
	WINDOW_TILE_DIVIDER_TEE_LEFT =
		WINDOW_TILE(12), /* a horizontal line leaving left */
	WINDOW_TILE_DIVIDER_CROSS = WINDOW_TILE(13),
	WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14),
	WINDOW_TILE_DIVIDER_VERTICAL = WINDOW_TILE(15),
	WINDOW_TILE_BORDER_TOP_TEE =
		WINDOW_TILE(24), /* the top edge meeting a line */
	WINDOW_TILE_BORDER_BOTTOM_TEE =
		WINDOW_TILE(25), /* the bottom edge meeting a line */
	WINDOW_TILE_BORDER_LEFT_TEE =
		WINDOW_TILE(26), /* the left edge meeting a line */
	WINDOW_TILE_BORDER_RIGHT_TEE =
		WINDOW_TILE(27), /* the right edge meeting a line */
	/* In battle, the bottom border tiles' lower rows, which */
	/* mnWindowSystemInitBattle copies to tiles 128 to 130. */
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT = WINDOW_TILE(128),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM = WINDOW_TILE(129),
	WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT = WINDOW_TILE(130),
	/* In battle, the tile the command row fills its empty panels with */
	/* (screens_battle_command_row.c), and mnWindowEraseBorderRect below row 16.
	 */
	WINDOW_TILE_COMMAND_PANEL = WINDOW_TILE(127),
	/* The HP and PP labels before a number (mnTextDrawPrefixedNumAtOffset) */
	/* or a Psynergy's cost: each label's first tile, and the second tile the */
	/* HP label always takes. */
	WINDOW_TILE_HP_LABEL = WINDOW_TILE(29),
	WINDOW_TILE_HP_LABEL_END = WINDOW_TILE(30),
	WINDOW_TILE_PP_LABEL = WINDOW_TILE(31),
};

/* The PP label's second tile: the HP label's in English, German and */
/* Japanese, a tile of its own in Spanish, French and Italian. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
	LANGUAGE == LANGUAGE_ITALIAN
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE(5)
#else
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE_HP_LABEL_END
#endif

/* A tilemap entry's tile number, the bits below its flip bits and palette */
/* number in the hardware's screen entry. */
#define TILE_NUMBER_MASK 0x3ff
/* The tilemap entry flip bits. */
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800

/* How a window sprite moves (WINDOW_SPR.motion), run each frame */
/* by mnWindowSprUpdateAll. 0 is a free window sprite. */
enum {
	WINDOW_SPRITE_MOTION_STILL = 1,
	WINDOW_SPRITE_MOTION_PAGE_ARROW =
		2,							 /* the animated next-page arrow, bobbing */
	WINDOW_SPRITE_MOTION_WOBBLE = 4, /* swings along a small arc */
	WINDOW_SPRITE_MOTION_SHAKE = 5,	 /* jitters by a pixel every other frame */
	WINDOW_SPRITE_MOTION_POP = 6,	/* shown at double size, then normal size */
	WINDOW_SPRITE_MOTION_SPIN = 7,	/* turns while circling its place */
	WINDOW_SPRITE_MOTION_PULSE = 8, /* shown at 1.25 times, then normal size */
	WINDOW_SPRITE_MOTION_SWELL =
		9, /* pulses between 1 and 1.5 times its size */
	WINDOW_SPRITE_MOTION_SWELL_HALF = 10, /* the same at half the scale */
	WINDOW_SPRITE_MOTION_SHRINK =
		11, /* shrinks from 1.5 times to its size, once */
	WINDOW_SPRITE_MOTION_SHRINK_HALF = 12, /* the same at half the scale */
	WINDOW_SPRITE_MOTION_HIDDEN = 13,	   /* not drawn */
	/* Three motions mnWindowSprUpdateAll runs alike: a hop down and back. */
	WINDOW_SPRITE_MOTION_HOP_DOWN = 14,
	WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE = 15,
	WINDOW_SPRITE_MOTION_HOP_DOWN_UNUSED = 16, /* Unused: nothing sets it */
	WINDOW_SPRITE_MOTION_HOP_UP = 17,
	WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT = 18,
};

/* The command icons (ResourceId_CommandIcons), each drawn with the command */
/* name of the same number (MsgCommandName + icon). */
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
	/* The settings menu's choices and the Pause menu's entries */
	/* (SettingsMenu_SpeedIcons to PauseMenu_Icons), whose command names are */
	/* placeholders. */
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
	FIELD_MESSAGE_WAITING_FOR_SOUND =
		2,						 /* the window state is busy while it shows */
	FIELD_MESSAGE_KEEP_OPEN = 4, /* the window is left open */
	FIELD_MESSAGE_LOWER = 8,	 /* 4 tiles below the top rows' centre */
	FIELD_MESSAGE_HOLD_UPLOAD = 0x20, /* the tilemap upload is held off */
	FIELD_MESSAGE_LOWEST = 0x40		  /* 12 tiles below the top rows' centre */
} FIELD_MESSAGE_FLAG;

/* The Pause menu's entries, as mnPauseMenuChoose returns them */
/* (counted from Save your quest even when only Change settings shows). */
typedef enum PAUSE_MENU_ENTRY {
	PAUSE_MENU_SAVE_QUEST = 0,
	PAUSE_MENU_SLEEP = 1,
	PAUSE_MENU_CHANGE_SETTINGS = 2,
	/* The debug entries. */
	PAUSE_MENU_ROUGH_MENU = 3,
	PAUSE_MENU_RECOVERY_TEST = 4,
	PAUSE_MENU_RESTART_QUEST = 5
} PAUSE_MENU_ENTRY;

/* The kinds of icon the selection screen lists (SELECTION_NODE.kind) and */
/* mnSelectionIconLoad, mnSelectionIconLoadIntoSlot and */
/* mnWindowSprCreateFromResource load. */
typedef enum ICON_KIND {
	ICON_KIND_NONE = 0,	   /* a free selection icon entry */
	ICON_KIND_COMMAND = 1, /* a command's icon, from its number */
	/* An item's icon, with its count above one and its equipped and broken */
	/* marks (and the artifact mark where a slot is given). */
	ICON_KIND_ITEM = 2,
	ICON_KIND_ABILITY = 4, /* a Psynergy's icon, from its ability number */
	ICON_KIND_YES_NO = 6,  /* a yes or no command, drawn as a command */
	/* An item's icon without the broken mark, for the equipment view. */
	ICON_KIND_EQUIPMENT_ITEM = 7,
	ICON_KIND_PATTERN =
		8, /* one of the other icons (mnIconBuildMiscIconTiles) */
	/* The one tile mnTitleLoadFirstTile loads. */
	ICON_KIND_TABLE_ENTRY = 9
} ICON_KIND;

/* The layers mnItemIconCompose draws over an item's icon. */
#define ICON_BASE			 1 /* the base under the icon */
#define ICON_COUNT_ABOVE_ONE 2 /* the count, unless it is 1 */
#define ICON_COUNT			 4 /* the count, always */
/* Mark 1, the cross, on an item with INVENTORY_BROKEN. */
#define ICON_BROKEN 8
/* Mark 0, the equip mark, on an item with INVENTORY_EQUIPPED. */
#define ICON_EQUIPPED 16
/* Mark 2, Icon_MarkCursed, on a cursed item that cannot be unequipped */
/* (ITEM_CURSED and ITEM_CANNOT_UNEQUIP) when its code has INVENTORY_EQUIPPED.
 */
#define ICON_CURSED 32

/* The kinds of message argument (mnMessageArgumentPush), which the message */
/* codes such as {argument_item_name} read. */
enum {
	MESSAGE_ARGUMENT_NONE = 0, /* an empty place in the argument table */
	MESSAGE_ARGUMENT_ACTOR = 1,
	MESSAGE_ARGUMENT_ITEM = 2,
	MESSAGE_ARGUMENT_CLASS = 3,
	MESSAGE_ARGUMENT_ABILITY = 4,
	MESSAGE_ARGUMENT_NUMBER = 5,
	MESSAGE_ARGUMENT_LOCATION = 6,
};

/* The window module's functions in the order of its stub table */
/* (src/window/stubs.s). */
void mnWindowSystemInitField(void);
void mnWindowSystemInitBattle(s32 loadGlyphTiles);
WINDOW *mnWindowCreate(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void mnWindowClose(WINDOW *window, s32 instant);
void mnMessageDecodeRawUnbounded(s32 message, u16 *text);
void mnMessageDecodeRaw(s32 message, u16 *text, s32 capacity);
s32 mnMessageCopyExpanded(s32 message, u16 *destination, u32 capacity);
WINDOW *mnMessageOpenWindow(s32 message, s32 x, s32 y, u32 packed);
void mnFieldMessageShow(s32 message, s32 flags);
s32 mnMessageAllTyped(void);
s32 mnWindowIsClosed(WINDOW *window);
void mnMessageSetDecodeFlags(s32 flags);
void mnWindowSprPrepareForRedraw(WINDOW *window);
void mnWindowClearInteriorTiles(const WINDOW *window, u32 left, u32 top,
								u32 right, u32 bottom);
void mnWindowDrawDividerLine(WINDOW *win, u32 x1, u32 y1, u32 x2, u32 y2);
void mnTextDrawMessage(s32 message, WINDOW *window, s32 x, s32 y);
void mnTextDrawMessageAsTiles(s32 message, const WINDOW *window, u32 offsetX,
							  u32 offsetY);
void mnTextDrawString(const u8 *text, WINDOW *window, s32 x, s32 y);
void mnTextDrawStringAtOffset(const u8 *text, const WINDOW *window, s32 offsetX,
							  s32 offsetY);
void mnTextDrawStringInWindow(const u8 *text, WINDOW *window, u32 x, u32 y);
void mnTextDrawNum(s32 value, s32 width, WINDOW *window, s32 x, s32 y);
void mnTextDrawNumAtOffset(s32 value, s32 width, const WINDOW *window, s32 x,
						   s32 y);
void mnTextDrawNumInWindow(s32 value, s32 width, WINDOW *window, s32 x, s32 y);
void mnTextSetCol(s32 color);
void mnTextSetStyle(u16 style);
WINDOW_SPR *mnWindowSprCreate(s32 slot, s32 oamFlags, WINDOW *window,
							  s32 offsetX, s32 offsetY);
WINDOW_SPR *mnWindowSprCreateFromResource(s32 mode, s32 resource,
										  WINDOW *window, s32 x, s32 y);
WINDOW_SPR *mnPortraitCreate(s32 character, s32 unused, s32 side,
							 WINDOW *window, s32 x, s32 y);
void mnPortraitReload(u32 side, s32 character, s32 unused);
void mnWindowSprSetMotion(WINDOW_SPR *sprite, s32 mode);
void mnWindowSprSetPriority(void *sprite, s32 priority);
WINDOW *mnPortraitOpenWindow(s32 speaker, s32 portraitOption, s32 x, s32 y);
void mnPortraitCloseWindow(u32 speaker);
s32 mnTextGetMessageSize(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 mnTextGetMessageSizeAllPages(s32 no, s32 *x, s32 *y, u32 *width,
								 u32 *height);
void mnMessageArgumentClearAll(void);
void mnMessageArgumentPush(u32 value, u32 kind);
void mnPartyStatCreateWindow(s32 flags);
void mnPartyStatDraw(s32 flags);
void mnPartyStatClose(void);
void mnMessageCloseFinishedWindows(void);
void mnWindowCloseAll(s32 unused);
void mnSelectionMenuCreate(void);
void mnSelectionMenuClear(void);
void mnSelectionMenuAddEntry(u32 kind, u32 base);
void mnSelectionMenuStartDrawing(void);
void mnSelectionMenuStopDrawing(void);
void mnSelectionMenuOpenNameWindow(s32 mode, u32 count);
void mnSelectionMenuClose(void);
void mnSetupSelectionBothSides(void);
s32 mnSelectionMenuRun(s32 mode);
u32 mnSelectionMenuRunWithSounds(u32 mode);
void mnArrowNoOpCallbackBank(void);
void mnArrowNoOpCallbackA(void);
void mnSelectionMenuBuildEntries(u32 unused);
void mnSelectionMenuReservedNoOp(void);
s32 mnBattleCollectPartyCommands(struct BATTLE_ACTION *commands, u16 *actors,
								 s32 commandCount);
void mnBattleMessageShowAndWait(s32 message);
void mnBattleMessageOpen(s32 message);
void mnArrowNoOpCallbackD(void);
s32 mnPortraitGetForSpeaker(u32 speaker);
s32 mnFieldMenuRun(void);
s32 mnFileListSelectAction(void);
s32 mnIconMenuChooseSendOption(s32 mode);
void mnAreaNameWindowShow(void);
void mnAreaNameWindowHide(void);
void mnFieldMessageShowCentered(s32 message, s32 mode, s32 yOffset);
void mnBattleMessageClose(void);
void mnMessageResetIdle(void);
void mnArrowCreate(struct MENU_SPR *sprite, s32 *slot);
void mnArrowSetPos(struct MENU_SPR *sprite, s32 x, s32 y);
void mnArrowRelease(s32 slot);
void mnRunSelectionHook(void);
s32 mnItemRemoveFromInventory(s32 unused, s32 itemId);
void mnIconDrawItem(s32 code);
void mnTextDrawWideStringAtOffset(u16 *text, WINDOW *window, s32 x, s32 y);
void mnSettingsMenuSelectionNoOp(void);
void mnScanlineWindowFillSpans(void *spans);
void mnWindowSprRedrawSavedRect(WINDOW *window);
void mnWindowSprClearList(void *sprites);
void mnWindowSetTilemapEntry(WINDOW *window, s32 value, s32 x, s32 y, u32 mode);
#ifdef UBFIX
void mnDebugSceneSelectRun(void);
#else
s16 mnDebugSceneSelectRun(void);
#endif
s32 mnDebugFlagEditorRun(void);
s32 mnSelectionIconLoadIntoSlot(s32 kind, s32 entry, s32 slot, s32 baseIcon);
void mnDebugSelectShortcuts(void);
void mnWindowMarkVisibleTextTiles(void);
s32 mnItemIconCompose(u32 code, u32 layers);
s32 mnPartyStatHighlightMembers(u16 *ids);
void mnIconBuildItemIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
							  s32 *tileOut, s32 reuse);
void mnIconBuildAbilityIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
								 s32 *tileOut, s32 reuse);
WINDOW_SPR *mnWindowSprCreateLoaded(s32 icon, s32 layers, WINDOW *window, s32 x,
									s32 y);
WINDOW_SPR *mnWindowSprCreateFromTable(s32 tableEntry, WINDOW *window, s32 x,
									   s32 y);
s16 mnSaveWriteCurrentFile(void);
s32 mnSaveToChosenFile(void);
s32 mnSaveChooseAndLoadFile(s32 mode);
s32 mnSaveCopyFile(void);
s32 mnSaveEraseSelectedFile(void);
s32 mnSaveWriteLinkTallies(void);
s32 mnSaveWriteFile(s32 file);
s32 mnSaveScanFileSummaries(void);
s32 mnNameEntryEditMemberName(s32 member);
void mnNameEntryLoadPointingHandTiles(s32 slot);
void mnNameEntryLoadCaretTiles(s32 slot);
void mnSelectionMenuSetPos(u32 x, u32 y);
s32 mnDebugBrowserIcons(void);
s32 mnDebugBrowserPortraits(void);
s32 mnItemReturnTrue(void);
void mnDjinnShowJoinedMessage(s32 member, s32 element, s32 djinn);
void mnWindowColApply(s32 phase, s32 brightness);
s32 mnSettingsMenuRun(void);
s32 mnPauseMenuRun(void);
void mnBgScrFillRect(s32 x, s32 y, u32 width, u32 height, s32 value);
s32 mnSelectShopCommand(s32 selection);
s32 mnSelectSanctumService(s32 selection);
s32 mnYesNoMenuRun(s32 bottom, s32 message, s32 labelWidth, s32 selection);
s32 mnSelectCoinsOrMedals(s32 selection);
s32 mnColossoMenuChoose(s32 unused);
s32 mnTitleLoadFirstTile(u32 entry, s32 unused, u32 slot);
s32 mnYesNoMenuRunAt(s32 x, s32 y, s32 selection);
s32 mnPointingHandLoadIntoFreeSlot(void);
void mnTextMeasureMessage(s32 message, s32 *x, s32 *y);
void mnWindowHighlightRect(s32 x, s32 y, s32 width, s32 height, s32 paletteBit);
void mnAbilityBuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
							 s32 reuse);
void mnGraphicsExpandVramTilesByColTable(u16 *dst);
void mnTextUseExtraTiles(s32 allowAlternateTiles);
s32 mnAnimateSelectionToEntry(s32 start, s32 command);
s32 mnWindowSprGetPointerTiles(s32 index);
s32 mnMessageOpenDecoded(WINDOW *window, s32 message);
s32 mnSaveCountFilesExcludingSent(s32 excludeSent);
void mnWindowDrawBorder(s32 x, s32 y, u32 width, u32 height);
void mnWindowEraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void mnPageArrowsDraw(s32 offset);
void mnPartyShowJoinedMessage(s32 member);
void mnPartyShowPairJoinedMessage(s32 first, s32 second);

/* Some of the window object's functions have no prototype here: the menu */
/* object (screens.c) calls them unprototyped, with the old argument */
/* promotions, and screens_internal.h declares them for a current GCC. They are
 */
/* mnWindowSprAppendToList, mnMessageStart (the joined-party messages pass */
/* five of its six arguments), mnTextDrawWideStringInWindow, mnWindowPutGlyph */
/* (mnPartyStatDraw passes four of its five) and */
/* mnTextDrawPrefixedNumAtOffset; mnWindowClearTextTilesInRect is called */
/* before its definition. */
WINDOW_SPR *mnWindowSprAcquireFree(void);
void mnWindowSprReleaseFree(WINDOW_SPR *out);
void mnWindowSprInitFreeList(void);
void mnWindowSystemUploadTilemap(void);
void mnWindowDrawOpened(WINDOW *window);
void mnWindowWaitZoom(WINDOW *window);
void mnWindowSprUpdateListTail(WINDOW_SPR_LIST *list);
void mnWindowSprRelease(WINDOW_SPR *entry);
MESSAGE *mnBattleMessageCont(WINDOW *window, s32 entry, s32 keepPosition);
s32 mnTextClearCanvas(void);
s32 mnTextFillCanvasBackground(void);
void mnMessageApplyStyle(MESSAGE *message);
void mnTextStartCanvasScroll(MESSAGE *message);
void mnTextScrollCanvasUp(s32 step);
void mnMessageUpdateAll(void);
s32 mnMessageType(MESSAGE *message);
void mnWindowUpdateZooms(void);
void mnWindowDrawZoomBorder(void *windowArg, s32 savePosition);
u16 *mnMemoryFillHalfwordsDma(u16 *dst, s32 value, s32 count);
void mnWindowMapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
								s32 fullWidth);
/* Declared without a prototype, so that mnWindowCreate can pass it an argument
 */
/* it never reads. */
void mnTextResetStyle();
void mnWindowSystemStartFieldUpdate(void);
void mnWindowSystemStartBattleUpdate(s32 loadGlyphTiles);
/* The bob of a prompt or cursor: up and down by up to 2 pixels, a sine every */
/* 16 frames. */
#define CURSOR_BOB_Y() (sySin(FrameCount * (TRIG_TURN / 16)) / (FIXED_ONE / 2))

/* mnMessageOpenWindow's packed argument: the speaker's voice in bits 16 to 27,
 */
/* above the window options. */
#define MESSAGE_WINDOW_VOICE(voice) ((voice) << 16)
/* The speaker's voice in a packed argument, as mnMessageOpenWindow takes it */
/* out. */
#define MESSAGE_WINDOW_VOICE_OF(packed) (((u32)(packed) << 4) >> 20)
/* The window options of a packed argument: the bits below the speaker's */
/* voice. */
#define MESSAGE_WINDOW_OPTIONS_MASK (MESSAGE_WINDOW_VOICE(1) - 1)

/* mnMessageOpenWindow's window options, in packed's low bits. */
typedef enum MESSAGE_WINDOW_OPTION {
	MESSAGE_WINDOW_ZOOM = 0x01, /* zoom open rather than appear at once */
	/* Set by the shopkeepers' windows and some others; mnMessageOpenWindow
	   never */
	/* reads it. */
	MESSAGE_WINDOW_UNREAD = 0x02,
	MESSAGE_WINDOW_TEXT_CANVAS = 0x08,	 /* WINDOW_FLAG_TEXT_CANVAS */
	MESSAGE_WINDOW_ANCHOR_BOTTOM = 0x10, /* WINDOW_FLAG_ANCHOR_BOTTOM */
	MESSAGE_WINDOW_FIXED_X = 0x20		 /* WINDOW_FLAG_FIXED_X */
} MESSAGE_WINDOW_OPTION;
void mnWindowSystemUpdate(void);
#if LANGUAGE != LANGUAGE_JAPANESE
s32 mnTextDrawGlyphPair(s32 code, u32 *out);
#else
s32 mnTextDrawGlyphPair(u32 code, u32 *out);
#endif
s32 mnTextGetWideStringWidth(u16 *text);
void mnTextDrawWideStringAsTiles(u16 *buffer, const WINDOW *window, s32 offsetX,
								 s32 offsetY);
u8 *mnStringFormatNum(u8 *buffer, s32 input, s32 width);
u32 mnMessageAppendArticleName(s32 inBattleMessage, u16 *name,
							   u32 writePosition, u16 *entries, s32 articleForm,
							   s32 plural, s32 *lastLetter);
s32 mnMessageDecode(s32 script, s32 clear);
void mnWindowFitOnScr(s32 entryIndex, s32 *windowX, s32 *windowY,
					  u32 *windowWidth, u32 *windowHeight, u16 *spacing,
					  u32 flags);
void mnTextMeasureDecoded(s32 entryIndex, u32 *outWidth, u32 *outHeight,
						  u16 *spacing);
void mnTextMeasureAllPages(s32 entryIndex, u32 *outWidth, u32 *outHeight,
						   u16 *spacing);
s32 mnGlyphDraw(WINDOW *window, s32 character, s32 x, s32 y, s32 mode);

/* How mnWindowSetTilemapEntry writes its value (its mode argument): as given,
 */
/* or as a tile number in one of the palettes the menus keep for their */
/* windows. */
typedef enum TILEMAP_ENTRY_MODE {
	TILEMAP_ENTRY_PLAIN, /* as given, palette bits included */
	TILEMAP_ENTRY_NONE,	 /* nothing */
	/* In HIGHLIGHT_PALETTE, the highlight mnPaletteLightenHighlight makes for
	 */
	/* the selected row or page. */
	TILEMAP_ENTRY_HIGHLIGHT,
	TILEMAP_ENTRY_WINDOW, /* in WINDOW_PALETTE */
	/* In palette 1, which the Status screen and the party rearranging fill */
	/* with the element icons' colours for the rows that are not highlighted */
	/* (mnStatScrRun, mnStatScrRunPartyRow). */
	TILEMAP_ENTRY_ICON
} TILEMAP_ENTRY_MODE;
void mnWindowSprUpdateScaleAnimation(WINDOW_SPR *windowSprite);
void mnWindowSprUpdateAll(void);
s32 *mnMemoryClearWordIfSet(s32 *p);
void mnWindowStepSwellEffect(MESSAGE *message);
u32 mnMessageArgumentLookup(u32 kind, u32 clear);
s32 mnTextCheckPauseSkipped(MESSAGE *message);
s32 mnTextCheckTextDismissed(MESSAGE *message);
s32 mnMessageDecodeAndClearArguments(s32 message);
void mnMessageFind(struct MESSAGE_READER *reader, s32 message);
s32 mnTextDecodeSymbolInC(struct MESSAGE_READER *reader);
void mnPortraitClearSlots(void);

#define WINDOW_STATE ((WIN_STATE *)WindowStateWork[0])

/* The action id in a PSYNERGY_ENTRY's encodedAction, below the marks for an */
/* action the class or an equipped item gave (PSYNERGY_ABILITY_MASK). */
#define ACTION_ID_MASK 0x3fff

/* Defines name(no, x, y, width, height): decodes message no and fits a window
 */
/* for it on the screen near tile (x, y) (mnWindowFitOnScr with flags), */
/* returning its place and size in tiles. Returns 0 for an empty message. */
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                            \
	s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height) {                \
		WIN_STATE *state;                                                      \
		s32 idx;                                                               \
                                                                               \
		state = WINDOW_STATE;                                                  \
		idx = mnMessageDecode(no, 0);                                          \
		if(state->textBuf[idx] == 0) {                                         \
			return 0;                                                          \
		}                                                                      \
		mnWindowFitOnScr(idx, x, y, width, height, 0, flags);                  \
		return 1;                                                              \
	}

s32 mnIconCountItemIcons(void);
s32 mnIconCountPsynergyIcons(void);
void mnIconBuildCommandIconTiles(s32 overlayIcon, s32 frame, s32 *slot,
								 s32 *tileOut, s32 reuse);
void mnAbilityBuildItemIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
								 s32 reuse);
s32 mnItemIconLoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot);
s32 mnIconBuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot);
void mnIconDrawMisc(u32 iconIndex);
void mnIconLoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
						   s32 palette, s32 reuse);
void mnIconReservedNoOp(void);
void mnIconDecode(ICON_TRANSFER *glyph, s32 overlay);

struct SELECTION_NODE *mnSelectionMenuFindFreeIcon(s32 kind);
void mnSelectionMenuDraw(void);
void mnSelectionMenuDrawArrow(struct SELECTION_MENU_STATE *state, s32 side);
void mnSelectionMenuSetupArrow(struct SELECTION_MENU_STATE *state, s32 side);
struct SELECTION_NODE *
mnSelectionMenuGetCursorNode(struct SELECTION_MENU_STATE *state);
void mnMoveSelectionForward(struct SELECTION_MENU_STATE *state);
void mnMoveSelectionBackward(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuStepRight(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuStepLeft(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuRedrawIcon(struct SELECTION_MENU_STATE *state,
							   u32 rowIndex);
void mnSelectionMenuHighlightIcon(struct SELECTION_MENU_STATE *state,
								  u32 rowIndex);
void mnClearBattleHighlight(struct SELECTION_MENU_STATE *state);
void mnSelectionMenuScroll(struct SELECTION_MENU_STATE *state, s32 forward);
s32 mnSelectionIconLoad(s32 mode, s32 value);
void mnSelectionMenuSetupIcon(u32 kind, s32 base, struct SELECTION_NODE *node,
							  s32 reuse);
u32 mnConfirmSelection(struct SELECTION_MENU_STATE *state);
void mnArrowNoOpCallback(void);
void mnArrowNoOpCallbackB(void);
void mnArrowNoOpCallbackC(void);
void mnSelectionMenuLoadLargeIcon(void);
void mnSelectionMenuFreeLargeIcon(void);
void mnPartyStatOpenAndWait(void);
/* UB: mnFieldMenuRun calls it with no prototype and uses the result, though it
 */
/* returns nothing. */
#ifdef UBFIX
void mnPartyStatCloseWithMenu(void);
#endif
void mnSelectionMenuCreateEmpty(void);
u32 mnRunSelectionForValue(u32 initialIndex);

void mnAreaNameWindowUpdate(void);
void mnSettingsMenuStepWindowHue(s32 keys);
s32 mnPartyListPsynergy(struct ABILITY_LIST_ENTRY *output);
void mnFindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
						   const struct ABILITY_LIST_ENTRY *entries);

void mnAllocateSelectionState(void);
void mnReleaseSelectionState(void);
void mnSettingsMenuReservedNoOpA(void);
void mnSettingsMenuReservedNoOpB(void);
void mnSettingsMenuReservedNoOpC(void);
void mnSettingsMenuResetEntries(struct SETTINGS_MENU_STATE *optionMenuState);
void mnWindowColCalcTint(struct SETTINGS_MENU_STATE *state);
void mnWindowColWritePalette(struct SETTINGS_MENU_STATE *state);
u16 mnWindowColScaleTint(struct SETTINGS_MENU_STATE *state, FIXED redScale,
						 FIXED greenScale, FIXED blueScale);
u16 mnColScaleComponents(s16 *rgb, FIXED redScale, FIXED greenScale,
						 FIXED blueScale);
void mnSettingsMenuPreviousRow(struct SETTINGS_MENU_STATE *state);
void mnSettingsMenuNextRow(struct SETTINGS_MENU_STATE *state);
void mnSettingsMenuLowerSetting(struct SETTINGS_MENU_STATE *state);
void mnSettingsMenuRaiseSetting(struct SETTINGS_MENU_STATE *state);
void mnSettingsMenuAnimateCursors(void);
void mnSettingsMenuInitState(void);
void mnSettingsMenuRelease(void);
WINDOW *mnSettingsMenuOpen(void);
void mnPauseMenuAnimateCursor(void);
void mnPauseMenuInit(void);
void mnPauseMenuRelease(void);
WINDOW *mnPauseMenuOpen(void);
s32 mnPauseMenuChoose(void);

void mnWindowOverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
								u8 *remap);
s32 mnTextDrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x);
void mnWindowReservedNoOp(void);

/* The menus' compressed 64x16 headings, Save to Setup (menu_titles.s). */
extern const u8 *MenuTitlePictures[];

#endif
