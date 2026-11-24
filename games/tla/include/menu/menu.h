#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

/// @file
/// What the menus share with the window bank, the shops and the debug rooms:
/// the item list's sort orders, the strings and tables the menus draw from,
/// and the routines each side calls of the other.

#include "types.h"
#include "edition.h"

/// The item menu's sort orders (gMenu_ListOrder*): one byte per group, an
/// item type, with LIST_ORDER_EQUIPPED for only the equipped items of that
/// type, ending with LIST_ORDER_END.
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED 0x80
#define LIST_ORDER_END 255

/// The item menu's four sort orders (ItemMenu_Sort), which
/// L + A cycles through in this order.
enum ListSort {
  /// The equipped items, the items that are not equipment, then the other
  /// equipment (gMenu_ListOrderEquippedThenItems).
  LIST_SORT_EQUIPPED_THEN_ITEMS = 0,
  /// The equipped items, the other equipment, then the rest
  /// (gMenu_ListOrderEquippedThenEquipment).
  LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
  LIST_SORT_EQUIPMENT_FIRST = 2, ///< gMenu_ListOrderEquipmentFirst
  /// The items that are not equipment first (gMenu_ListOrderItemsFirst).
  LIST_SORT_ITEMS_FIRST = 3,
  LIST_SORT_COUNT = 4
};

/// The rows a page of the item and Psynergy lists shows.
#define LIST_PAGE_SIZE 5

/// The y of the menus' row of party figures, and of the chosen member's,
/// lowered below the rest.
#define MENU_MEMBER_Y 26
#define MENU_MEMBER_CHOSEN_Y 30

/// The width in tiles of each status row's highlight on the status page (enum
/// UnitStatus).
extern u8 gStatusScreen_CursorWidths[];
extern u8 gMenu_ListOrderItemsFirst[];
extern u8 gMenu_ListOrderEquippedThenItems[];
extern u8 gMenu_ListOrderEquippedThenEquipment[];
extern u8 gMenu_ListOrderEquipmentFirst[];
/// `-`, before a stat change that is not a gain (ItemMenu_DrawStat).
extern u8 gMenu_MinusSignString;
/// `+`, before a stat gain.
extern u8 gMenu_PlusSignString;
/// The field menu's four icons: the command at each place, and the place of
/// each command.
///@{
extern s8 gMenu_TopEntryCommandByPosition[];
extern s8 gMenu_TopEntryPositionByCommand[];
///@}
/// `L`, before a level in the file list (screens_save.c).
extern const u8 gStatusMenu_LevelLetterString[];
void Menu_HideEntryIcons(void);

struct Window;
struct WindowSprite;
struct DjinnMenuBackdrop;

/// The down arrow from the class now to the class an equipment would give
/// (backdrop.4bpp), which Menu_WatchStart blinks every four frames at
/// CLASS_CHANGE_ARROW_TILE.
extern u8 gMenu_ClassChangeArrowTile[];
void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 first);
void Event_FinishScreenTransitionFar(void);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
void Party_AdjustCoinsFar(s32 amount);
s32 Party_ListMembersFar(s16 *unitIds);
s32 Party_SumDjinnCountsFar(s32 element);
void *String_FormatPlayTime(s32 frames, u8 *out);
void Icon_Show(struct WindowSprite *icon);

/// `:` and the sixteen hex digits, for the debug rooms' flag and destination
/// lists.
///@{
extern const u8 gMenu_ColonString[];
extern const u8 gMenu_HexDigitsString[];
///@}
/// The frames a selection made for the player to watch waits at each step, by
/// the icons still to go (IconMenu_AnimateTo).
extern u8 gMenu_SelectionStepDelays[];

/// Credits_Run's modes: the title's story crawl over the slideshow, or the
/// staff credits from a page, CREDITS_MODE_FIRST_PAGE being their first.
enum CreditsMode { CREDITS_MODE_STORY_CRAWL = 1, CREDITS_MODE_FIRST_PAGE = 2 };

#endif
