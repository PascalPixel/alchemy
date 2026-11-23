#ifndef ALCHEMY_MENU_H
#define ALCHEMY_MENU_H

/// @file
/// The menus' list orders, sign strings and routines.

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

extern u8 gStatusScreen_CursorWidths[];
extern u8 gMenu_ListOrderItemsFirst[];
extern u8 gMenu_ListOrderEquippedThenItems[];
extern u8 gMenu_ListOrderEquippedThenEquipment[];
extern u8 gMenu_ListOrderEquipmentFirst[];
/// `-`, before a stat change that is not a gain (ItemMenu_DrawStat).
extern u8 gMenu_MinusSignString;
/// `+`, before a stat gain.
extern u8 gMenu_PlusSignString;
extern s8 gMenu_TopEntryCommandByPosition[];
extern s8 gMenu_TopEntryPositionByCommand[];
/// The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price
/// sprite's tiles (Shop_CopyGlyphs).
extern u8 gShop_GlyphBytes[];
extern const u8 gStatusMenu_LevelLetterString[];
void *IconMenu_Open(void);
void ItemMenu_PositionCategory(void);
void IconMenu_AddIcon(s32 icon);
void IconMenu_Close(void);
void Resource_ResetPendingTransfer(void);
s32 Sanctum_NeedsService(s32 entry_id, s32 kind);
void Text_DrawMessageFar(s32 message, s32 window, s32 x, s32 y);

struct Obj;
struct Window;
struct WindowSprite;
struct DjinnMenuBackdrop;

/// One 4-bit tile (backdrop.4bpp) the Start prompt blinks while a menu watches
/// for Start (MENU_START_PROMPT_TILE), every four frames.
extern u8 gMenu_StartPromptTile[];
void DjinnMenu_DrawPage(struct DjinnMenuBackdrop *backdrop, s32 first);
void Event_FinishScreenTransitionFar(void);
void Scene_TalkToActorFar(s32, s32);
void PaletteFade_SetSourceFar(s32);
void PaletteFade_ReleaseFar(void);
void PaletteFade_SetBackdropFar(void);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
void DebugFlagEditor_DrawTable(s32, s32);
s32 DebugFlagEditor_HandleInput(s32, s32 *, s32 *);
void IconMenu_LayoutIcons(s32 x, s32 y, s32 w, s32 h);
s32 IconMenu_Run(s32);
void Party_AdjustCoinsFar(s32);
s32 Party_ListMembersFar(s16 *);
s32 Party_SumDjinnCountsFar(s32);
#ifndef OWN_WindowSprite_CreateFromResourceFar
struct WindowSprite *WindowSprite_CreateFromResourceFar(s32 mode, s32 resource,
                                                        struct Window *window,
                                                        s32 x, s32 y);
#endif
void SelectionMenu_StopDrawing(void);
void *String_FormatPlayTime(s32, u8 *);
void Window_HighlightRectFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Icon_PrepareActor(struct WindowSprite *icon);
#ifndef OWN_UiTextResource_Initialize
struct MenuSprite;
void MenuArrow_Create(struct MenuSprite *sprite, s32 *slot);
#endif
#ifndef OWN_UiTextResource_SetPosition
void MenuArrow_SetPosition(struct Obj *obj, s32 x, s32 y);
#endif
void Window_SetTilemapEntryFar(s32 window, s32 value, s32 x, s32 y, u32 mode);

extern const u8 gMenu_ColonString[];
extern const u8 gMenu_HexDigitsString[];
extern u8 gMenu_SelectionStepDelays[];

#endif
