#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

/* @file */
/* What the menus share with the window module, the shops and the debug rooms:
 */
/* the item list's sort orders, the strings and tables the menus draw from, */
/* and the functions each side calls of the other. */

#include "types.h"
#include "edition.h"

/* The item menu's sort orders (Menu_ListOrder*): one byte per group, an */
/* item type, with LIST_ORDER_EQUIPPED for only the equipped items of that */
/* type, ending with LIST_ORDER_END. */
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED	 0x80
#define LIST_ORDER_END		 255

/* The item menu's four sort orders (mnItemMenuSort), which */
/* L + A cycles through in this order. */
typedef enum LIST_SORT {
	/* The equipped items, the items that are not equipment, then the other */
	/* equipment (Menu_ListOrderEquippedThenItems). */
	LIST_SORT_EQUIPPED_THEN_ITEMS = 0,
	/* The equipped items, the other equipment, then the rest */
	/* (Menu_ListOrderEquippedThenEquipment). */
	LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
	LIST_SORT_EQUIPMENT_FIRST = 2, /* Menu_ListOrderEquipmentFirst */
	/* The items that are not equipment first (Menu_ListOrderItemsFirst). */
	LIST_SORT_ITEMS_FIRST = 3,
	LIST_SORT_COUNT = 4
} LIST_SORT;

/* The rows a page of the item and Psynergy lists shows. */
#define LIST_PAGE_SIZE 5

/* The y of the menus' row of party figures, and of the chosen member's, */
/* lowered below the rest. */
#define MENU_MEMBER_Y		 26
#define MENU_MEMBER_CHOSEN_Y 30

/* The width in tiles of each status row's highlight on the status page (enum */
/* UNIT_STAT). */
extern u8 StatusScreen_CursorWidths[];
extern u8 Menu_ListOrderItemsFirst[];
extern u8 Menu_ListOrderEquippedThenItems[];
extern u8 Menu_ListOrderEquippedThenEquipment[];
extern u8 Menu_ListOrderEquipmentFirst[];
/* `-`, before a stat change that is not a gain (mnItemMenuDrawStat). */
extern u8 Menu_MinusSignString;
/* `+`, before a stat gain. */
extern u8 Menu_PlusSignString;
/* The field menu's four icons: the command at each place, and the place of */
/* each command. */
/* @{ */
extern s8 Menu_TopEntryCommandByPosition[];
extern s8 Menu_TopEntryPositionByCommand[];
/* @} */
/* `L`, before a level in the file list (screens_save.c). */
extern const u8 StatusMenu_LevelLetterString[];
void mnHideEntryIcons(void);

struct WINDOW;
struct WINDOW_SPR;
struct DJINN_MENU_BACKDROP;

/* The down arrow from the class now to the class an equipment would give */
/* (backdrop.4bpp), which mnWatchStart blinks every four frames at */
/* CLASS_CHANGE_ARROW_TILE. */
extern u8 Menu_ClassChangeArrowTile[];
void mnDjinnMenuDrawPage(struct DJINN_MENU_BACKDROP *backdrop, s32 first);
void fdEventFinishScrTransitionStub(void);
void mnItemMenuArrangeCategoryItemIcons(u16 *items);
void mnItemMenuDrawEquippedItemNames(s32 window, u16 *items);
void gmPartyAdjustCoinsStub(s32 amount);
s32 gmPartyListMembersStub(s16 *unitIds);
s32 gmPartySumDjinnCountsStub(s32 element);
void *mnStringFormatPlayTime(s32 frames, u8 *out);

/* `:` and the sixteen hex digits, for the debug rooms' flag and destination */
/* lists. */
/* @{ */
extern const u8 Menu_ColonString[];
extern const u8 Menu_HexDigitsString[];
/* @} */
/* The frames a selection made for the player to watch waits at each step, by */
/* the icons still to go (mnIconMenuAnimateTo). */
extern u8 Menu_SelectionStepDelays[];

/* mnCreditsRun's modes: the title's story crawl over the slideshow, or the */
/* staff credits from a page, CREDITS_MODE_FIRST_PAGE being their first. */
typedef enum CREDITS_MODE {
	CREDITS_MODE_STORY_CRAWL = 1,
	CREDITS_MODE_FIRST_PAGE = 2
} CREDITS_MODE;

struct WINDOW;
struct WINDOW_SPR;

/* The menu module's functions in the order of its stub table. */
/* The field menu's Item screen: choose a party member, one of their items and
 */
/* a command. Owns the menu state while it runs and frees it before returning.
 */
/* Returns mnItemMenuRunCommands' result: 1 when an item is to be used on the */
/* field, -1 when the player backed out or pressed Start. */
s32 mnItemMenuRun(void);
/* The field menu's Psynergy screen (mnFieldMenuRun, FIELD_MENU_PSYNERGY), over
 */
/* the field, which stops meanwhile. A Psynergy chosen to use on the field goes
 */
/* to FIELD_STATE.psynergy with the party member who uses it. Returns */
/* mnPsynergyMenuRunSteps' result. */
s32 mnPsynergyMenuRun(void);
/* The Status screen, the field menu's Status, over the field: saves the */
/* background palettes and the window tiles it draws over, sets up the party */
/* row, the five command icons and, when the party has Djinn, the Djinn */
/* figures, runs the status pages, then puts everything back. */
s32 mnStatScrRun(void);
/* Lets the player choose one party member, over the field. Returns the chosen
 */
/* party member's id, or -1 when the player backs out. Owns the menu state */
/* (HEAP_SLOT_MENU) while it runs and frees it before returning. */
s32 mnPartyMemberChooserOpen(void);
/* Creates a hand cursor sprite in window at (x, y), loading the hand's tiles */
/* into a free VRAM block. */
s32 mnIconCreateHandCursor(struct WINDOW *window, s32 x, s32 y);
/* Shows an icon at its place, unscaled. */
void mnIconShow(struct WINDOW_SPR *icon);
/* Runs the Djinn menu over the field: the field menu's Djinn screen */
/* (mnFieldMenuRun, FIELD_MENU_DJINN), and the Djinn tutorial. Types messages */
/* at MESSAGE_SPEED_FAST while it runs (the tutorial's steps set */
/* MESSAGE_SPEED_NORMAL for their messages, which stays until the menu closes)
 */
/* and puts the player's speed back. Takes the menu state (HEAP_SLOT_MENU) and
 */
/* the backdrop (DJINN_MENU_BACKDROP, from the EWRAM pool), and frees */
/* both before returning, after putting back the field tiles and palettes the */
/* backdrop saved. Returns 1, which makes the field menu open again. */
s32 mnDjinnMenuRun(void);
/* Describes item in window with the entry's own count, through */
/* mnItemMenuDrawItemDetailsWithCount. */
void mnItemMenuDrawItemDetails(s32 window, s32 item);
/* Describes an item (an inventory entry) in window, one 8-pixel row a line: */
/* its equip bonuses and effects, a curse, the Psynergy it bestows, its uses */
/* and its count, or failing all of these whether it is rare or important. */
/* count is the count to show, which only Japanese reads; */
/* mnItemMenuDrawItemDetails passes -1, the entry's own. */
void mnItemMenuDrawItemDetailsWithCount(s32 window, s32 item, s32 count);
/* Lends HIGHLIGHT_PALETTE to the element icons: copies the first sprite */
/* palette, the icons' colours, into it, then WINDOW_BACKGROUND_COLOR over its
 */
/* TEXT_BACKGROUND_COLOR. */
void mnPaletteLoadIconsIntoHighlight(void);
/* A task while the party figures show: places each party member's */
/* figure at its menu position and scale through the projected placement. */
void mnUpdatePartyFigures(void);
/* Replaces the menu's party figures with up to four party members' figures, */
/* from party place first on, standing (ANIM_STAND), and counts the party. */
void mnCreatePartyFigures(s32 first);
/* Creates the first four party members' figures in a row from tile (originX, */
/* originY) inside window, three tiles apart, and starts placing them each */
/* frame (mnUpdatePartyFigures). */
void mnShowPartyFigures(struct WINDOW *window, s32 originX, s32 originY);
/* Destroys the menu's party figures and stops placing them. */
void mnReleasePartyFigures(void);
/* Replaces the four Djinn figures with one Djinni of each element */
/* (DjinnFigures_SpriteIds) walking (ANIM_WALK), at FIGURE_START_X and */
/* MENU_FIGURE_HIDDEN_Y until the screen moves them, and starts placing them */
/* each frame (mnDjinnFiguresUpdateTopRow). */
void mnDjinnFiguresInitTopRow(void);
/* Destroys the four Djinn figures and stops placing them. */
void mnDjinnFiguresReleaseTopRow(void);
/* Shows the Djinn figures of the top row (nonzero) or hides them (0). */
void mnDjinnFiguresSetShown(s32 shown);
/* Replaces Djinn figure index with a Djinni of element kind */
/* (DjinnFigures_SpriteIds) playing animation. */
void mnDjinnFiguresReplaceSlot(s32 index, s32 kind, s32 animation);
/* Moves Djinn figure index to screen pixel (x, y), if it exists. */
void mnDjinnFiguresSetSlotPos(s32 index, s32 x, s32 y);
/* Turns Djinn figure index to face facing (FACING), if it exists. */
void mnDjinnFiguresSetSlotFacing(s32 index, s32 facing);
/* Sets Djinn figure index's scale, if it exists: positive grows towards full */
/* size, negative holds at its size. */
void mnDjinnFiguresSetSlotPhase(s32 index, s32 scale);

/* The title module's functions in the order of its stub table. */
/* Shows the title screen from white: the Golden Sun logo over the valley and */
/* its lighthouse, with, when `prompt` is set, the "Press Start" prompt */
/* pulsing. It holds 90 seconds with the prompt and 11 without, and A or Start
 */
/* ends it early, returning 1 (else 0); except in the Japanese edition, the */
/* prompt and the keys wait two seconds. */
s32 mnTitleShowTitleScr(s32 prompt);
/* Unused: an empty entry the stub table keeps. */
void mnTitleAfterIntroNoOp(void);
/* Unused: an empty entry the stub table keeps. */
void mnTitleBeforeLogosNoOp(void);
/* Shows the publisher's logo with its chime. With mode 0, which is all the */
/* title passes, it appears at once and holds two seconds; any other mode, */
/* unused, waits a second, fades in, holds three seconds and fades out, faster
 */
/* once skipped. Returns -1 when A or Start skipped it. */
s32 mnTitleShowPublisherLogo(s32 mode);
/* Shows the Camelot logo for two seconds, cycling its four frames of tiles. A
 */
/* or Start ends it early. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_Title_ShowCamelotLogo and declares that form itself. */
#ifndef OWN_Title_ShowCamelotLogo
s32 mnTitleShowCamelotLogo(void);
#endif
/* The intro (menu/title_intro.c): it takes a flag, 1 from the title and 0 */
/* before the prologue, that it never reads. */
s32 mnTitleIntroRun(s32 unused);

/* The credits module's functions in the order of its stub table. */
/* Runs the title's story crawl or the staff credits over the greyed battle */
/* backdrops. The story crawl (CREDITS_MODE_STORY_CRAWL) cross-fades a new */
/* backdrop each time the crawl stops for one, until Start or the crawl's end;
 */
/* any other mode scrolls the staff credits from page mode - */
/* CREDITS_MODE_FIRST_PAGE and returns once they have run out. */
s32 mnCreditsRun(s32 mode);
/* Starts the ending's images (the epilogue scene calls it first): resets */
/* the task table and the scroll, adds mnEndingImageScroll, sets up BG2, BG3 */
/* and blending, and builds the border strips. */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_EndingImage_Start and declares that form itself. */
#ifndef OWN_EndingImage_Start
void mnEndingImageStart(void);
#endif
/* Shows a scrolling image on the hidden one of the two BG character blocks. */
/* With no resource it fills the map buffer with a plain tile word and clears */
/* the palette half; otherwise it clears the block's first 2 KB, expands */
/* resource's map into the buffer with EndingImage_DecodeCode copied to the */
/* heap (the fill word marks the second block's tiles), and queues the */
/* resource's 128-colour palette. Then it copies the map at position into the */
/* tilemap (CopyScrollMap), flips the block and restarts the fade-in counter. */
void mnEndingImageShow(s32 resource, u8 *map, s32 position);
/* Sets the row the ending's scrolling image moves to, in pixels from its */
/* top, at most EndingImage_Speed a frame. */
void mnEndingImageSetTargetRow(u16 row);
/* Waits, for at most 300 frames, until the scroll reaches its row. */
void mnEndingImageWaitForScroll(void);
/* Moves the top strip's eight sprites to y 120 when enable is set, and to */
/* y 0 otherwise. */
void mnEndingImageLowerTopStrip(s32 enable);

#endif
