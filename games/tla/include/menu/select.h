#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

// The selection screen: the row of icons a command, item or Psynergy is picked from, its icons and
// their sprites.

#include "types.h"
#include "static_assert.h"
#include "window/window.h"
#include "system.h"

// A selection screen sprite: a list link and its OAM attributes.
typedef struct SELECTION_SPR
{
	struct SELECTION_SPR *next;
	Uint8                 y;
	Uint8                 affine   : 2;
	Uint8                 mode     : 2;
	Uint8                 mosaic   : 1;
	Uint8                 colors   : 1;
	Uint8                 shape    : 2;
	Uint16                x        : 9;
	Uint16                matrix   : 5; // the affine matrix
	Uint16                size     : 2;
	Uint16                tile     : 10;
	Uint16                priority : 2;
	Uint16                palette  : 4;
} SELECTION_SPR;

// An icon of the selection screen: its list links, the entry it shows, its place and its slide
// toward a target, its zoom and its sprite.
typedef struct SELECTION_NODE
{
	struct SELECTION_NODE *prev;    // the icon before it in its row
	struct SELECTION_NODE *next;    // the icon after it in its row
	Uint16                 base;    // the entry's id, as listed in SELECTION_MENU_STATE.bases
	Uint16                 kind;    // the entry's kind, as listed in SELECTION_MENU_STATE.kinds
	Uint16                 slot;
	Uint16                 tile;
	Sint16                 x;       // screen pixels
	Sint16                 y;       // screen pixels
	Sint16                 dx;      // pixels a tick
	Sint16                 dy;      // pixels a tick
	Sint16                 targetX; // screen pixels
	Sint16                 targetY; // screen pixels
	Sint16                 homeX;
	Sint16                 homeY;
	Uint16                 msg;     // its name, shown in the screen's window
	Sint16                 scale;
	Sint16                 scaleStep;
	Sint16                 scaleEnd;
	SELECTION_SPR          sprite;
} SELECTION_NODE;

// The kinds of icon the selection screen lists (SELECTION_NODE.kind) and mnSelectionIconLoad loads.
typedef enum ICON_KIND
{
	ICON_KIND_NONE           = 0, // a free selection icon entry
	ICON_KIND_COMMAND        = 1, // a command's icon, from its number
	ICON_KIND_ITEM           = 2, // an item's icon
	ICON_KIND_ABILITY        = 4, // a Psynergy's icon, from its ability number
	ICON_KIND_YES_NO         = 6, // a yes or no command, drawn as a command
	// An item's icon without the broken mark, for the equipment view.
	ICON_KIND_EQUIPMENT_ITEM = 7,
	ICON_KIND_PATTERN        = 8 // one of the other icons
} ICON_KIND;

// The entries of SELECTION_MENU_STATE.records.
typedef enum SELECTION_RECORD
{
	SELECTION_RECORD_ROW        = 2,  // the first of the row's seven icons
	SELECTION_RECORD_CHOSEN     = 9,  // the first of the five icons chosen
	SELECTION_RECORD_HAND       = 14, // the pointing hand
	SELECTION_RECORD_LARGE_ICON = 15,
	SELECTION_RECORD_COUNT      = 16
} SELECTION_RECORD;

// The selection screen (HEAP_SLOT_SELECTION_MENU): the row of icons the player picks from, with
// arrows at each end and a pointing hand over the chosen target.
typedef struct SELECTION_MENU_STATE
{
	// 0 and 1 the arrows at the row's ends, 2 to 8 the row's icons, 9 to 13 the icons chosen so
	// far, 14 the pointing hand and 15 the large icon.
	SELECTION_NODE   records[SELECTION_RECORD_COUNT];
	AFFINE_TRANSFORM transform; // a zooming icon's scale
	SELECTION_NODE  *head;      // the first icon of the row
	SELECTION_NODE  *path;      // the icons chosen so far
	WINDOW          *window;    // the window naming the chosen entry
	// The listed entries, up to 16.
	Uint16           kinds[16];
	Uint16           bases[16];
	Uint16           count;
	Uint16           baseX; // screen pixels
	Uint16           baseY; // screen pixels
	Uint16
		// cleared with the list and on each step down; nothing reads it
		   unread;
	Uint16 top; // the first listed entry shown
	// The entry under the hand, from top; SELECTION_CURSOR_RESET asks the next list to start again
	// from its first entry.
	Uint16 cursorIndex;
	Uint16 verticalScroll; // pixels added to the icons' y; 32 at setup
	// SELECTION_STATUS_SCROLLING while the cursor steps, SELECTION_STATUS_SHOWN after
	// (window_selection.c); each tick drawn adds 1.
	Uint16 status;
	Uint16 pathTop[5];    // top, kept for each icon chosen on the path
	Uint16 pathCursor[5]; // cursorIndex, kept for each icon chosen on the path
	// 999 from setup and once a yes or no window opens, 1 from mnRunSelectionForValue. Nothing sets
	// 0, which would open a yes or no row and its window at the top of the screen.
	Uint16 locked;
	Uint8  unused[42]; // no code names it
} SELECTION_MENU_STATE;

// SELECTION_MENU_STATE.status: the cursor stepping, or the row shown.
#define SELECTION_STATUS_SCROLLING 33
#define SELECTION_STATUS_SHOWN     1

STATIC_ASSERT(sizeof(SELECTION_SPR) == 12);
STATIC_ASSERT(sizeof(SELECTION_NODE) == 52);
STATIC_ASSERT(offsetof(SELECTION_NODE, msg) == 32);
STATIC_ASSERT(offsetof(SELECTION_NODE, sprite) == 40);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, head) == 840);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, kinds) == 852);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, cursorIndex) == 926);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, locked) == 952);
STATIC_ASSERT(sizeof(SELECTION_MENU_STATE) == 996);

// The bit of SELECTION_MENU_STATE.cursorIndex set as an entry is chosen, on which
// mnSelectionMenuClear puts the next list's top and cursor back to its first entry.
#define SELECTION_CURSOR_RESET 0x80

void  *mnIconMenuOpen(void);
void   mnIconMenuAddIcon(Sint32 icon);
void   mnIconMenuClose(void);
void   mnSelectionMenuFreeLargeIcon(void);
void   mnIconMenuLayoutIcons(Sint32 x, Sint32 y, Sint32 w, Sint32 h);
Sint32 mnIconMenuRun(Sint32 init);

// Loads the icon of a selection entry of kind mode (ICON_KIND) into a free sprite tile slot and
// returns the slot, or -1.
Sint32 mnSelectionIconLoad(Uint32 mode, Sint32 value);

#endif
