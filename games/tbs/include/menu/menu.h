#ifndef MENU_H
#define MENU_H

/// @file
/// The menus' list orders, sign strings and routines.

#include "types.h"
#include "edition.h"
#include "farcall.h"

/// The summary screen highlight's width in tiles on each status row: Down,
/// Poison, Venom, Curse and Haunt.
extern u8 gSummaryScreen_CursorWidths[];
extern u8 gMenu_ListOrderItemsFirst[];
extern u8 gMenu_ListOrderEquippedThenItems[];
extern u8 gMenu_ListOrderEquippedThenEquipment[];
extern u8 gMenu_ListOrderEquipmentFirst[];
/// `-`, before a stat change that is not a gain (ItemMenu_DrawStat).
extern u8 gMenu_MinusSignString;
/// `+`, before a stat gain.
extern u8 gMenu_PlusSignString;
/// The field menu's command by icon position plus one, a row of 6 with the
/// Djinn icon and one without: 1 Psynergy, 2 Djinn, 3 Item, 4 Status
/// (FieldMenu_SelectCommand).
extern s8 gFieldMenu_CommandByPosition[];
/// The inverse: each command's icon position plus one, in the same two rows.
extern s8 gFieldMenu_PositionByCommand[];
/// The price digits 0 to 9, 32 bytes each (digits.4bpp), copied into a price
/// sprite's tiles (Shop_CopyGlyphs).
extern u8 gShop_GlyphBytes[];
/// The level label of a save summary: `L`; `Nv.` in Spanish, `Niv` in French.
extern u8 gSummaryScreen_LevelLetterString[];

struct Sprite;
struct AbilityDefinition;
struct DjinnListTable;
struct FieldEffectSprite;
struct PagedList;
struct ObjectSystemWork;
struct PsynergyEntry;
struct PlacementDescriptor;
struct RenderInput;
struct WindowSprite;
struct SelectionScreenWork;
struct MenuCursor;
struct Window;
struct Work;

/// A background's horizontal and vertical scroll offsets.
struct ScrollPair {
  u16 x; ///< pixels
  u16 y; ///< pixels
};
/// The small down arrow, 16 by 16 (sdown): the item menu's stat drop.
extern u8 gMenu_SmallDownArrowTiles[];
/// The small up arrow, then the up and down arrows, 16 by 16 each; the item
/// menu's stat rise loads the first (Resource_LoadPairedBlocks).
extern u8 gMenu_ArrowTiles[];
/// The Djinn menu's up arrow, 16 by 16 (djinn/arrowup).
extern u8 gMenu_PairSpriteATiles[];
/// The Djinn menu's down arrow, 16 by 16 (djinn/arrowdn).
extern u8 gMenu_PairSpriteBTiles[];
/// The item menu's quantity picture (quantity.4bpp).
extern const u8 gMenu_QuantityTiles[];
/// The Djinn menu's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const char gDjinnMenu_TextLevel[];
/// `/`, between the Djinn menu's numbers.
extern const char gDjinnMenu_TextSlash[];
#ifndef OWN_gObjectSystemWork
/// Heap slot 6: the object system's work (struct ObjectSystemWork).
extern struct ObjectSystemWork *gObjectSystemWork;
#endif
/// The number text buffer: String_FormatHex writes eight digits and
/// String_FormatDecimal eleven characters, right-aligned, each with a
/// terminator. The debug text printers and the Djinn preview's numbers
/// (String_FormatNumberToHalfwords) read it.
extern u8 gNumberBuffer[];
/// Heap slot 18: the selection screen while it runs.
extern struct SelectionScreenWork *gSelectionScreenWork;
#ifndef OWN_gTitleExtraOptionEnabled
/// Read as a halfword: nonzero offers Battle on the main menu, when a file
/// has any of game flags 48 to 127 set.
extern u8 gTitleExtraOptionEnabled[];
#endif
/// The Japanese item menu's help: `L+A:`, before the arrange help.
extern u8 gItemMenu_ArrangeKeysString[];
extern s32 gItemMenu_CommandColumnXTable[];
/// The Japanese item menu's help: `R:`.
extern u8 gItemMenu_EquipmentKeyString[];
/// One 4-bit tile (backdrop.4bpp) the Djinn menus copy into the backdrop's
/// frame; the Djinn preview blinks it, drawing it and a fill of colour 4
/// (0x44444444) in turn every four frames.
extern const u8 gMenu_BackdropFrameTile[];
#ifndef OWN_Menu_ColonString
/// The colon the debug menus' map and flag rows draw
/// (window/screens/debug.c): `:`.
extern const u8 gMenu_ColonString[];
#endif
/// The selection screen's left arrow, 8 by 16 at 256 colours (cursor_l).
extern u8 gMenu_CursorLeftObjectTiles[];
/// `0123456789ABCDEF`, drawn by a debug window.
extern const u8 gMenu_HexDigitsString[];
/// The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in
/// Italian.
extern const u8 gMenu_HpString[];
#ifndef OWN_Menu_LvString
/// The summary screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const u8 gMenu_LvString[];
#endif
/// The four Djinn sprites, one per element from Venus (90 to 93), that
/// FileMenu_ShowDjinnSprites shows in a row.
extern const s32 gMenu_PartySpriteResourceIds[4];
/// `.`, the decimal point of an item's stat multiplier (`x1.2`).
extern u8 gMenu_PointString[];
/// The PP label: `PP`; `EP` in Japanese.
extern const u8 gMenu_PpString[];
/// The title menu's action by icon position, a row of 6 for each layout:
/// 0 New Game, 1 Continue, 2 Copy, 3 Erase, 4 Battle, 5 Send.
extern s8 gFileMenu_ActionByPosition[];
/// Frames IconMenu_AnimateTo waits at each icon on its way, by icons still to
/// go.
extern u8 gMenu_SelectionStepDelays[];
/// `L:`, the Japanese Psynergy menu's first shortcut label.
extern const u8 gMenu_ShortcutLColonString[];
/// `L`, the first shortcut's label beside an ability name wider than 11
/// (Japanese only).
extern const u8 gMenu_ShortcutLString[];
/// `R:`, the second shortcut's label (Japanese only).
extern const u8 gMenu_ShortcutRColonString[];
/// `R`, the second shortcut's label beside an ability name wider than 11
/// (Japanese only).
extern const u8 gMenu_ShortcutRString[];
/// `/`, between a current and a maximum.
extern const u8 gMenu_SlashString[];
/// `x`, before an item's stat multiplier (not in Japanese).
extern u8 gMenu_TimesString[];
extern s8 gOptionMenu_SleepFrames[];
extern s8 gOptionMenu_SpeechFrames[];
extern s8 gOptionMenu_SpeedFrames[];
/// The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern u8 gUi_LvString[];
extern u8 gMenuCursor_BobX[];
extern u8 gMenuCursor_BobY[];
/// A debug window's title: `ENERGY`.
extern u8 gWorkspace_EnergyTitle[];
/// A debug window's title: `ITEM`.
extern u8 gWorkspace_ItemTitle[];
/// A debug window's label: `PAGE:`.
extern u8 gWorkspace_PageLabel[];
/// `-`, between the first and last entry numbers of a debug window's page
/// (32 a page).
extern u8 gWorkspace_RangeDash[];
/// A debug window's title: `STATUS`.
extern u8 gWorkspace_StatusTitle[];
/// The option menu's slider colours (slidpal).
extern const u16 gOptionMenu_SliderPalette[];
/// The option menu's slider, 32 by 8 (slider.4bpp32x8).
extern const u8 gOptionMenu_SliderTiles[];
#ifndef OWN_Ability_GetData
struct AbilityDefinition *Ability_GetData(s32 actionId);
#endif
s32 Item_FindSlot(s32 unitId, s32 item_id);
void TitleFar_Entry20(s32 mode);
void CreditsFar_Entry00(s32 mode);
void Text_DrawAt(s32, s32, s32, s32);
void Window_Clear(s32 window);
void Window_Commit(s32 window);

#endif
