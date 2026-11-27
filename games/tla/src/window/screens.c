/* The screens object: the screens built on the windows, from the window
   sprites, the party status window and the file list to the battle menus, the
   icon menus and the Pause menu, as in The Broken Seal's screens.c. Its code
   calls several window functions with no declaration in scope, as The Broken
   Seal's does.

   window_internal.h holds what both objects share; the code is in the
   screens_*.c files under window/, included here in the order it has in the
   ROM. */

#include "window_internal.h"
#include "field/scene_tables.h"

/* A bounce of 32 steps, 8.8 fixed-point scales: the selected icons pulse by
   it. */
extern const Uint16 WindowSpriteBounceScales[];
/* The portraits of the characters and of the other speakers, {who, portrait}
   pairs ending in -1. */
extern const Sint16 PortraitCharacterMap[];
extern const Sint16 PortraitSpriteMap[];

#ifdef MODERN
/* The screens object calls the window sprite list's functions with no
   declaration in scope, as its bytes need; a current GCC gets them here. */
#include "graphics/window_sprite_list.h"
#endif

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

/* The pulsing icon's transform: its scale, 8.8 fixed point, and angle. */
typedef struct COMMAND_ROW_AFFINE {
	Uint16 scaleX;
	Uint16 scaleY;
	Uint16 ang;
} COMMAND_ROW_AFFINE;

/* The command row's state: the icons' shared palette, up to six icons, the
   label window, the pulsing icon's transform and the selection. */
typedef struct BATTLE_COMMAND_ROW_STATE {
	/* BG palette slots for the icons' 256-colour pixels, filled as each icon is
	   loaded (mnBattleCommandRowLoadIconTiles). */
	ICON_PALETTE			pal;
	BATTLE_COMMAND_ROW_ICON entries[6];
	void				   *win;
	Sint32					iconCnt;
	COMMAND_ROW_AFFINE		affine;
	Sint32					affineIdx;
	Sint32					selectedIdx;
	Sint32					drawnIdx;
	Sint32					commandKinds[6];
} BATTLE_COMMAND_ROW_STATE;

/* A sprite as the summary screen queues it (syOAMAddSprAtDepth): the depth
   queue's link, then its OAM attributes as words or as fields. */
typedef struct SUMMARY_OAM {
	Uint8 link[4];

	union {
		struct {
			Uint32 attr01;
			Uint32 attr23;
		} word;

		struct {
			Uint8  y;
			Uint8  attr0high;
			Uint16 x : 9;
			Uint16 affineIdx : 5;
			Uint16 size : 2;
			Uint16 tile : 10;
			Uint16 pri : 2;
			Uint16 pal : 4;
			Uint16 high;
		} f;
	} oam;
} SUMMARY_OAM;

/* Where the Status screen's cursor points, in pixels. */
typedef struct SUMMARY_POINT {
	Sint32 x;
	Sint32 y;
} SUMMARY_POINT;

/* The Status screen's cursor: its place in each group, the help entry it shows
   and where it points. */
typedef struct STAT_CURSOR {
	Sint8  groupPositions[8];
	Sint32 helpEntry;
	Sint32 drawnHelpEntry;
	Sint32 group;
	Sint32 groupSize;
	Sint32 column;
	Sint32 row;
} STAT_CURSOR;

/* The name entry screen's cursors: the hand on the keyboard and the caret under
   the name, each a window sprite moved as a MENU_CURSOR. */
typedef union NAME_CURSOR {
	WINDOW_SPR *spr;
	MENU_CURSOR cursor;
} NAME_CURSOR;

/* The menus' state (heap slot HEAP_SLOT_MENU) as the file list reads it: the
   party row of the shop's SHOP_STATE (shop/shop_internal.h), holding the party
   sprites of the file under the cursor, where each stands and its scale. */
typedef struct FILE_LIST_STATE {
	Uint8 menuScreens[248]; /* the windows and lists before the party row */
	/* Up to four party sprites, null for none; the row has room for eight. */
	void  *partyMemberIcons[8];
	Sint16 partyMemberX[8];		/* pixels */
	Sint16 partyMemberY[8];		/* pixels */
	FIXED  partyMemberScale[4]; /* 16.16 fixed point, toFIXED(1) full size */
} FILE_LIST_STATE;

/* The battle menus' message buffers, in halfwords, and the copier that fills
   them: the Japanese edition decodes the message (mnMessageDecodeRaw), the
   others copy its text (mnMessageCopyExpanded). */
#define BATTLE_MENU_MESSAGE_CAPACITY JAPANESE_OR(32, 52)
#define BATTLE_MENU_MESSAGE_COPY \
	JAPANESE_OR(mnMessageDecodeRaw, mnMessageCopyExpanded)

/* The two BG tiles of the Djinn menu's arrow: DJINN_MENU_ARROW_TILE blinks, the
   one after it stays (DjinnMenu_BlinkTile). */
#define DJINN_MENU_ARROW_TILE 808

/* The arrow tile mnBattleDjinnMenuRun copies to the two BG tiles from
   DJINN_MENU_ARROW_TILE each tick: the first blinks (four ticks shown, four
   blank), the second stays. */
extern const Uint8 DjinnMenu_BlinkTile[32];

/* Where each element's icon and standby count sit in the party status window's
   Djinn column, in tiles, for the Djinn menu to blink them. */
extern Uint8 DjinnMenuElementXOffsets[4];
extern Uint8 DjinnMenuElementYOffsets[4];

/* The Status screen's cursor stops: the stats, the equipment and the
   conditions. */
extern const BATTLE_SUMMARY_STAT_CURSOR OwnerStatusStatCursors[];
extern const BATTLE_SUMMARY_CURSOR		OwnerStatusEquipmentCursors[];
extern const BATTLE_SUMMARY_CURSOR		OwnerStatusConditionCursors[];
/* "Lv", before the level on the Status screen. */
extern const Uint8 OwnerStatusLevelString[];

/* The ability of each summon, by its number. */
extern const Uint16 SummonAbilities[];

/* The heading pair's bob (mnTitleBob), in pixels, a step every four */
/* Frames. */
extern const Uint8 MenuTitleBobOffsets[];

/* The rows the range icons are marked with (mnRangeIconBuildTiles). */
extern const BAR_PATTERN_ROW MenuBarPatternRows[];

/* The standby Djinn counts 10 to 18, one tile each, from BG tile
   DJINN_COUNT_TENS_TILE: the party status window loads them
   (PartyStatus_DigitTiles) when a count passes 9. */
extern const Uint8 PartyStatus_DigitTiles[];
/* That tile, and its place in VRAM. */
#define DJINN_COUNT_TENS_TILE 416
#define DJINN_COUNT_TENS_VRAM \
	((void *)(BG_CHAR_BLOCK(0) + DJINN_COUNT_TENS_TILE * TILE_SIZE_4BPP))
/* The window tilemap entry of standby count `n`: a digit, or a tile from
   DJINN_COUNT_TENS_TILE for 10 on. */
#define DJINN_COUNT_TILE(n) \
	((n) > 9 ? (n) + WINDOW_TILE(DJINN_COUNT_TENS_TILE - 10) \
			 : WINDOW_TILE((n) + '0'))

/* The saving screen's progress bar: its seven stages, a row of tile numbers
   each. */
extern const Uint8 WindowProgressBarRows[];
/* The progress bar's tiles, compressed. */
extern const Uint8 Window_ProgressBarTiles[];

/* The ARM function that reads the windows' edges off the UI tilemap for each
   scan line (window_spans.c), copied to RAM before it runs. */
extern Uint8 mnScanlineWindowBuildSpansCode[];

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
   parts"). */
#include "screens_window_sprites.c"
#include "screens_party_status.c"
#include "screens_save.c"
#include "screens_name_entry.c"
#include "screens_joined.c"
#include "screens_menu_titles.c"
#include "screens_battle_command_row.c"
#include "screens_battle_djinn_preview.c"
#include "screens_battle_status.c"
#include "screens_battle_djinn_menu.c"
#include "screens_battle_summon_menu.c"
#include "screens_battle_item_menu.c"
#include "screens_battle_psynergy_menu.c"

#undef mnCursorSetPosImmediateStub

/* A unit's place projected onto the screen, as the target cursor reads it. */
typedef struct TARGET_POINT {
	Sint32 x;
	Sint32 y;
	Sint32 z;
} TARGET_POINT;

/* One of the target cursor's markers: where it stands, whether it shows and
   which listed unit it marks. */
typedef struct TARGET_TRACK {
	Uint8 x;
	Uint8 y;
	Uint8 flg;
	Sint8 idx;
} TARGET_TRACK;

/* One line's horizontal window edges, each a WIN_RANGE value. */
typedef struct WINDOW_LINE {
	Uint16 win0; /* for WIN0H */
	Uint16 win1; /* for WIN1H */
} WINDOW_LINE;

/* A page of the icon menu's window shape: one WINDOW_LINE for each of the
   screen's 160 lines. */
typedef struct WINDOW_BUFFER {
	Uint8		winInOut[4]; /* WININ and WINOUT, not touched here */
	WINDOW_LINE lines[160];
} WINDOW_BUFFER;

/* The debug icon browser's headings and page label. */
extern const Uint8 IconBrowserItemTitle[];
extern const Uint8 IconBrowserEnergyTitle[];
extern const Uint8 IconBrowserStatusTitle[];
extern const Uint8 IconBrowserPageLabel[];
extern const Uint8 IconBrowserRangeDash[];

/* The File Screen's icon menu: the action at each icon position. */
extern Sint8 MenuSaveSlotActionByPosition[];

/* The parts, in ROM order. */
#include "screens_battle_targets.c"
#include "screens_link_timer.c"
#include "screens_battle_party_commands.c"
#include "screens_icon_menu.c"
#include "screens_debug_destinations.c"
#include "screens_debug_flags.c"
#include "screens_pause_menu.c"
