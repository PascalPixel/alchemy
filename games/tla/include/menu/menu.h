#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

// What the menus share with the window module, the shops and the debug rooms: the item list's sort
// orders, the strings and tables the menus draw from, and the functions each side calls of the
// other.

#include "types.h"
#include "edition.h"

// The item menu's sort orders (MenuListOrder*): one byte per group, an item type, with
// LIST_ORDER_EQUIPPED for only the equipped items of that type, ending with LIST_ORDER_END.
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED  0x80
#define LIST_ORDER_END       255

// The item menu's four sort orders (mnItemMenuSort), which L + A cycles through in this order.
typedef enum LIST_SORT
{
	// The equipped items, the items that are not equipment, then the other equipment
	// (MenuListOrderEquippedThenItems).
	LIST_SORT_EQUIPPED_THEN_ITEMS     = 0,
	// The equipped items, the other equipment, then the rest (MenuListOrderEquippedThenEquipment).
	LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
	LIST_SORT_EQUIPMENT_FIRST =
		2, // MenuListOrderEquipmentFirst
	       // The items that are not equipment first (MenuListOrderItemsFirst).
	LIST_SORT_ITEMS_FIRST = 3,
	LIST_SORT_COUNT       = 4
} LIST_SORT;

// The rows a page of the item and Psynergy lists shows.
#define LIST_PAGE_SIZE       5

// The y of the menus' row of party figures, and of the chosen member's, lowered below the rest.
#define MENU_MEMBER_Y        26
#define MENU_MEMBER_CHOSEN_Y 30

// The width in tiles of each status row's highlight on the status page (enum UNIT_STAT).
extern Uint8       StatusScreenCursorWidths[];
extern Uint8       MenuListOrderItemsFirst[];
extern Uint8       MenuListOrderEquippedThenItems[];
extern Uint8       MenuListOrderEquippedThenEquipment[];
extern Uint8       MenuListOrderEquipmentFirst[];
// `-`, before a stat change that is not a gain (mnItemMenuDrawStat).
extern Uint8       MenuMinusSignString;
// `+`, before a stat gain.
extern Uint8       MenuPlusSignString;
// The field menu's four icons: the command at each place, and the place of each command.
extern Sint8       MenuTopEntryCommandByPosition[];
extern Sint8       MenuTopEntryPositionByCommand[];
// `L`, before a level in the file list (screens_save.c).
extern const Uint8 StatusMenuLevelLetterString[];
void               mnHideEntryIcons(void);

struct WINDOW;
struct WINDOW_SPR;
struct DJINN_MENU_BACKDROP;

// The down arrow from the class now to the class an equipment would give (backdrop.4bpp), which
// mnWatchStart blinks every four ticks at CLASS_CHANGE_ARROW_TILE.
extern Uint8       MenuClassChangeArrowTile[];
void               mnDjinnMenuDrawPage(struct DJINN_MENU_BACKDROP *backdrop, Sint32 first);
void               fdScriptFinishScreenTransitionStub(void);
void               mnItemMenuArrangeCategoryItemIcons(Uint16 *items);
void               mnItemMenuDrawEquippedItemNames(Sint32 window, Uint16 *items);
void               gmPartyAdjustCoinsStub(Sint32 amount);
Sint32             gmPartyListMembersStub(Sint16 *unitIds);
Sint32             gmPartySumDjinnCountsStub(Sint32 element);
void              *mnStringFormatPlayTime(Sint32 ticks, Uint8 *out);

// `:` and the sixteen hex digits, for the debug rooms' flag and destination lists.
extern const Uint8 MenuColonString[];
extern const Uint8 MenuHexDigitsString[];
// The ticks a selection made for the player to watch waits at each step, by the icons still to go
// (mnIconMenuAnimateTo).
extern Uint8       MenuSelectionStepDelays[];

// mnCreditsRun's modes: the title's story crawl over the slideshow, or the staff credits from a
// page, CREDITS_MODE_FIRST_PAGE being their first.
typedef enum CREDITS_MODE
{
	CREDITS_MODE_STORY_CRAWL = 1,
	CREDITS_MODE_FIRST_PAGE  = 2
} CREDITS_MODE;

struct WINDOW;
struct WINDOW_SPR;

// The menu module's functions in the order of its stub table. The field menu's Item screen: choose
// a party member, one of their items and a command. Owns the menu state while it runs and frees it
// before returning. Returns mnItemMenuRunCommands' result: 1 when an item is to be used on the
// field, -1 when the player backed out or pressed Start.
Sint32 mnItemMenuRun(void);
// The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over the field, which
// stops meanwhile. A Psynergy chosen to use on the field goes to FIELD_STATE.psynergy with the
// party member who uses it. Returns mnPsynergyMenuRunSteps' result.
Sint32 mnPsynergyMenuRun(void);
// The Status screen, the field menu's Status, over the field: saves the background palettes and the
// window tiles it draws over, sets up the party row, the five command icons and, when the party has
// Djinn, the Djinn figures, runs the status pages, then puts everything back.
Sint32 mnStatusScreenRun(void);
// Lets the player choose one party member, over the field. Returns the chosen party member's id, or
// -1 when the player backs out. Owns the menu state (HEAP_SLOT_MENU) while it runs and frees it
// before returning.
Sint32 mnPartyMemberChooserOpen(void);
// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles into a free VRAM
// block.
Sint32 mnIconCreateHandCursor(struct WINDOW *window, Sint32 x, Sint32 y);
// Shows an icon at its place, unscaled.
void   mnIconShow(struct WINDOW_SPR *icon);
// Runs the Djinn menu over the field: the field menu's Djinn screen (mnFieldMenuRun,
// FIELD_MENU_DJINN), and the Djinn tutorial. Types messages at MESSAGE_SPEED_FAST while it runs
// (the tutorial's steps set MESSAGE_SPEED_NORMAL for their messages, which stays until the menu
// closes) and puts the player's speed back. Takes the menu state (HEAP_SLOT_MENU) and the backdrop
// (DJINN_MENU_BACKDROP, from the EWRAM pool), and frees both before returning, after putting back
// the field tiles and palettes the backdrop saved. Returns 1, which makes the field menu open
// again.
Sint32 mnDjinnMenuRun(void);
// Describes item in window with the entry's own count, through mnItemMenuDrawItemDetailsWithCount.
void   mnItemMenuDrawItemDetails(Sint32 window, Sint32 item);
// Describes an item (an inventory entry) in window, one 8-pixel row a line: its equip bonuses and
// effects, a curse, the Psynergy it bestows, its uses and its count, or failing all of these
// whether it is rare or important. count is the count to show, which only Japanese reads;
// mnItemMenuDrawItemDetails passes -1, the entry's own.
void   mnItemMenuDrawItemDetailsWithCount(Sint32 window, Sint32 item, Sint32 count);
// Lends HIGHLIGHT_PALETTE to the element icons: copies the first sprite palette, the icons'
// colours, into it, then WINDOW_BACKGROUND_COLOR over its TEXT_BACKGROUND_COLOR.
void   mnPaletteLoadIconsIntoHighlight(void);
// An event while the party figures show: places each party member's figure at its menu position and
// scale through the projected placement.
void   mnUpdatePartyFigures(void);
// Replaces the menu's party figures with up to four party members' figures, from party place first
// on, standing (ANIM_STAND), and counts the party.
void   mnCreatePartyFigures(Sint32 first);
// Creates the first four party members' figures in a row from tile (originX, originY) inside
// window, three tiles apart, and starts placing them each tick (mnUpdatePartyFigures).
void   mnShowPartyFigures(struct WINDOW *window, Sint32 originX, Sint32 originY);
// Destroys the menu's party figures and stops placing them.
void   mnReleasePartyFigures(void);
// Replaces the four Djinn figures with one Djinni of each element (DjinnFiguresSpriteIds) walking
// (ANIM_WALK), at FIGURE_START_X and MENU_FIGURE_HIDDEN_Y until the screen moves them, and starts
// placing them each tick (mnDjinnFiguresUpdateTopRow).
void   mnDjinnFiguresInitTopRow(void);
// Destroys the four Djinn figures and stops placing them.
void   mnDjinnFiguresReleaseTopRow(void);
// Shows the Djinn figures of the top row (nonzero) or hides them (0).
void   mnDjinnFiguresSetShown(Sint32 shown);
// Replaces Djinn figure index with a Djinni of element kind (DjinnFiguresSpriteIds) playing
// animation.
void   mnDjinnFiguresReplaceSlot(Sint32 index, Sint32 kind, Sint32 animation);
// Moves Djinn figure index to screen pixel (x, y), if it exists.
void   mnDjinnFiguresSetSlotPos(Sint32 index, Sint32 x, Sint32 y);
// Turns Djinn figure index to face facing (FACING), if it exists.
void   mnDjinnFiguresSetSlotFacing(Sint32 index, Sint32 facing);
// Sets Djinn figure index's scale, if it exists: positive grows towards full size, negative holds
// at its size.
void   mnDjinnFiguresSetSlotPhase(Sint32 index, Sint32 scale);

// The title module's functions in the order of its stub table. Shows the title screen from white:
// the Golden Sun logo over the valley and its lighthouse, with, when `prompt` is set, the "Press
// Start" prompt pulsing. It holds 90 seconds with the prompt and 11 without, and A or Start ends it
// early, returning 1 (else 0); except in the Japanese edition, the prompt and the keys wait two
// seconds.
Sint32 mnTitleShowTitleScreen(Sint32 prompt);
// Unused: an empty entry the stub table keeps.
void   mnTitleAfterIntroNoOperation(void);
// Unused: an empty entry the stub table keeps.
void   mnTitleBeforeLogosNoOperation(void);
// Shows the publisher's logo with its chime. With mode 0, which is all the title passes, it appears
// at once and holds two seconds; any other mode, unused, waits a second, fades in, holds three
// seconds and fades out, faster once skipped. Returns -1 when A or Start skipped it.
Sint32 mnTitleShowPublisherLogo(Sint32 mode);
// Shows the Camelot logo for two seconds, cycling its four ticks of tiles. A or Start ends it
// early. A unit whose ROM code is compiled against another form defines OWN_mnTitleShowCamelotLogo
// and declares that form itself.
#ifndef OWN_mnTitleShowCamelotLogo
Sint32 mnTitleShowCamelotLogo(void);
#endif
// The intro (menu/title_intro.c): it takes a flag, 1 from the title and 0 before the prologue, that
// it never reads.
Sint32 mnTitleIntroRun(Sint32 unused);

// The credits module's functions in the order of its stub table. Runs the title's story crawl or
// the staff credits over the greyed battle backdrops. The story crawl (CREDITS_MODE_STORY_CRAWL)
// cross-fades a new backdrop each time the crawl stops for one, until Start or the crawl's end; any
// other mode scrolls the staff credits from page mode - CREDITS_MODE_FIRST_PAGE and returns once
// they have run out.
Sint32 mnCreditsRun(Sint32 mode);
// Starts the ending's images (the epilogue scene calls it first): resets the event table and the
// scroll, adds mnEndingImageScroll, sets up BG2, BG3 and blending, and builds the border strips. A
// unit whose ROM code is compiled against another form defines OWN_mnEndingImageStart and declares
// that form itself.
#ifndef OWN_mnEndingImageStart
void mnEndingImageStart(void);
#endif
// Shows a scrolling image on the hidden one of the two BG character blocks. With no resource it
// fills the map buffer with a plain tile word and clears the palette half; otherwise it clears the
// block's first 2 KB, expands resource's map into the buffer with EndingImageDecodeCode copied to
// the heap (the fill word marks the second block's tiles), and queues the resource's 128-colour
// palette. Then it copies the map at position into the tilemap (CopyScrollMap), flips the block and
// restarts the fade-in counter.
void mnEndingImageShow(Sint32 resource, Uint8 *map, Sint32 pos);
// Sets the row the ending's scrolling image moves to, in pixels from its top, at most
// EndingImageSpeed a tick.
void mnEndingImageSetTargetRow(Uint16 row);
// Waits, for at most 300 ticks, until the scroll reaches its row.
void mnEndingImageWaitForScroll(void);
// Moves the top strip's eight sprites to y 120 when enable is set, and to y 0 otherwise.
void mnEndingImageLowerTopStrip(Sint32 enable);

#endif
