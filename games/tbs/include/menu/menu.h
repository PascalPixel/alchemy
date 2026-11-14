#ifndef MENU_H
#define MENU_H

/// @file
/// The menus' list orders, sign strings and routines.

#include "types.h"
#include "edition.h"
#include "gamepak/far_call.h"
#include "system/heap.h"

/// The Status screen highlight's width in tiles on each status row: Down,
/// Poison, Venom, Curse and Haunt.
extern u8 gStatusScreen_CursorWidths[];
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
extern u8 gStatusScreen_LevelLetterString[];

struct Sprite;
struct ActorSystemWork;
struct SelectionMenuWork;
struct MenuCursor;

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
/// The Djinn menu's up arrow, 16 by 16 (graphics/ui/djinn_arrow_up).
extern u8 gMenu_PairSpriteATiles[];
/// The Djinn menu's down arrow, 16 by 16 (graphics/ui/djinn_arrow_down).
extern u8 gMenu_PairSpriteBTiles[];
/// The item menu's quantity picture (quantity.4bpp).
extern const u8 gMenu_QuantityTiles[];
/// The Djinn menu's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const char gDjinnMenu_TextLevel[];
/// `/`, between the Djinn menu's numbers.
extern const char gDjinnMenu_TextSlash[];
/// Heap slot 6: the actor system's work (struct ActorSystemWork).
#define gActorSystemWork                                                       \
  (*(struct ActorSystemWork **)&gHeapSlots[HEAP_SLOT_ACTOR_SYSTEM])
/// The number text buffer: String_FormatHex writes eight digits and
/// String_FormatDecimal eleven characters, right-aligned, each with a
/// terminator.
extern u8 gNumberBuffer[];
/// Heap slot 18: the selection screen while it runs.
#define gSelectionMenuWork                                                     \
  (*(struct SelectionMenuWork **)&gHeapSlots[HEAP_SLOT_SELECTION_MENU])
/// Read as a halfword: nonzero offers Battle on the File Screen, when a file
/// has any of game flags 48 to 127 set.
extern u8 gTitleBattleOptionEnabled[];
/// The Japanese item menu's help: `L+A:`, before the arrange help.
extern u8 gItemMenu_ArrangeKeysString[];
extern s32 gItemMenu_CommandColumnXTable[];
/// The Japanese item menu's help: `R:`.
extern u8 gItemMenu_EquipmentKeyString[];
/// One 4-bit tile (backdrop.4bpp) the Djinn menus copy into the backdrop's
/// frame; the Djinn preview blinks it, drawing it and a fill of the window
/// background (TEXT_CANVAS_BACKGROUND_WORD) in turn every four frames.
extern const u8 gMenu_BackdropBorderTile[];
/// The colon the debug menus' scene and flag rows draw
/// (window/screens_debug.c): `:`.
extern const u8 gMenu_ColonString[];
/// The selection screen's left arrow, 8 by 16 at 256 colours
/// (graphics/ui/sprite_cursors/left).
extern u8 gMenu_CursorLeftActorTiles[];
/// `0123456789ABCDEF`, drawn by a debug window.
extern const u8 gMenu_HexDigitsString[];
/// The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in
/// Italian.
extern const u8 gMenu_HpString[];
/// The Status screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const u8 gMenu_LvString[];
/// The four Djinn sprites, one per element from SPRITE_VENUS_DJINNI to
/// SPRITE_JUPITER_DJINNI, that FileList_ShowDjinnSprites shows in a row.
extern const s32 gFileList_DjinnSpriteIds[4];
/// `.`, the decimal point of an item's stat multiplier (`x1.2`).
extern u8 gMenu_PointString[];
/// The PP label: `PP`; `EP` in Japanese.
extern const u8 gMenu_PpString[];
/// The File Screen's action by icon position, a row of 6 for each layout:
/// 0 New Game, 1 Continue, 2 Copy, 3 Erase, 4 Battle, 5 Send.
extern s8 gFileList_ActionByPosition[];
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
extern s8 gSettingsMenu_SleepIcons[];
extern s8 gSettingsMenu_SpeechIcons[];
extern s8 gSettingsMenu_SpeedIcons[];
/// The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern u8 gStatusPanel_LevelString[];
extern u8 gMenuCursor_BobX[];
extern u8 gMenuCursor_BobY[];
/// A debug window's title: `ENERGY`.
extern u8 gDebugMenu_EnergyTitle[];
/// A debug window's title: `ITEM`.
extern u8 gDebugMenu_ItemTitle[];
/// A debug window's label: `PAGE:`.
extern u8 gDebugMenu_PageLabel[];
/// `-`, between the first and last entry numbers of a debug window's page
/// (32 a page).
extern u8 gDebugMenu_RangeDash[];
/// A debug window's title: `STATUS`.
extern u8 gDebugMenu_StatusTitle[];
/// The settings menu's slider colours (graphics/ui/slider_palette).
extern const u16 gSettingsMenu_SliderPalette[];
/// The settings menu's slider, 32 by 8 (slider.4bpp32x8).
extern const u8 gSettingsMenu_SliderTiles[];
void Import_TitleShowCamelotLogo(s32 mode);
void Import_CreditsRun(s32 mode);

struct Unit;
struct DjinnListTable;
struct ItemMenuWork;
struct PagedList;
struct PsynergyEntry;
struct Window;
struct WindowSprite;

// The item menu's sort orders (gMenu_ListOrder*): one byte per group, an
// item type, with LIST_ORDER_EQUIPPED for only the equipped items of that
// type, ending with LIST_ORDER_END.
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED 0x80
#define LIST_ORDER_END 0xff

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

s32 Menu_ResumeFieldTasks(void);
s32 Menu_SuspendFieldTasks(void);
void Window_InitializeWork(s32 unused);
s32 Window_UpdateOrCreate(struct Window **handle, s32 x, s32 y, s32 w, s32 h,
                          s32 flags);
void Window_CloseIfOpen(struct Window **handle, s32 style);
void StatusPanel_Draw(struct Window *unused, s32 member, s32 slot, s32 mode);
void Text_DrawNumberRightAligned(s32 number, struct Window *window, s32 x,
                                 s32 y);
void StatusPanel_DrawHpPp(struct Unit *unit, struct Window *window);
void Text_DrawStatComparison(const struct Unit *after,
                             const struct Unit *before, struct Window *window);
s32 Icon_CreateWithResource(struct Window *window, s32 unused, s32 x, s32 y);
s32 Icon_CreateWithResourceVariant(struct Window *window, s32 x, s32 y);
void Icon_PrepareActor(struct WindowSprite *object);
void Menu_ReloadMemberPortrait(const void *unused, s32 unit);
struct Window *MenuCursor_Create(void *source);
void Menu_CreatePartyFigures(void *source, s32 originX, s32 originY,
                             s32 spacing, s32 unused);
void Menu_ReleasePartyFigures(void);
void Menu_PlacePartyFigures(void);
void MenuCursor_Position(s32 xOffset, s32 yOffset);
void MenuCursor_Slide(s32 x, s32 y);
void PsynergyMenu_ReservedNoOp(void);
void PsynergyMenu_RefreshMemberEntriesDefault(void);
void PsynergyMenu_RefreshMemberEntries(s32 originX, s32 originY, s32 columns);
void PsynergyMenu_PositionMemberEntry(struct WindowSprite **slot, s32 index,
                                      s32 originX, s32 originY, s32 columns);
void ItemMenu_PlaceIcon(struct WindowSprite **slot, s32 slotIndex, s32 originX,
                        s32 originY, s32 columns);
void ItemMenu_RefreshEntry(s32 layout);
void ItemMenu_ShowMessage(s32 message, s32 x, s32 y);
s32 ItemMenu_Sort(u16 *inventory, s32 mode);
void Menu_CopyListOrder(s32 mode, u8 *order);
s32 PagedList_HandleInput(s32 horizontal, s32 count, s32 pageSize, s32 *cursor,
                          s32 *page);
void Palette_LightenHighlight(s32 paletteNumber);
void PagedList_DrawPageNumbers(struct Window *window, s32 itemCount,
                               s32 pageSize, s32 selectedPage, s32 rightEdge);
void Render_SetTilemapFlagRect(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette);
void Palette_LoadIconsIntoHighlight(void);
void Menu_SetPageIcons(s32 pageSize, s32 first, struct Window *window, s32 x,
                       s32 y);
void Menu_DrawCoins(struct Window *window);
void Window_SetBounds(struct Window *window, s32 x, s32 y, s32 width,
                      s32 height);
void Window_HoldTilemapUpload(void);
void Window_ReleaseTilemapUpload(void);
s32 Sound_PlayReturnOne(s32 sound);
void Menu_WatchStart(void);
void Menu_StartWatchingStart(void);
void Menu_StopWatchingStart(void);
void WindowPalette_SetNormalTextColor(void);
void WindowPalette_SetNoHpTextColor(void);
void WindowPalette_SetLowHpTextColor(void);
void DjinnFigures_ReleaseTopRow(void);
void DjinnFigures_ReleaseBottomRow(void);

#define LIST_PAGE_SIZE 5

s32 ItemMenu_Run(void);
s32 ItemMenu_RunCommands(s32 *memberOut, s32 *targetOut, s32 *itemOut);
s32 ItemMenu_Use(void);
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SpawnIconEntries(struct ItemMenuWork *menu, struct Window *window);
void ItemMenu_HideAllIcons(void);
void ItemMenu_HidePageIcons(void);
void ItemMenu_Close(void);
s32 ItemMenu_SelectMember(s32 side);
s32 ItemMenu_RunMemberSelection(u16 *memberIds, u16 *items);
void ItemMenu_OpenCategory(s32 memberId);
s32 ItemMenu_SelectTarget(s32 mode);
void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StopCompatibilityIndicators(void);
s32 ItemMenu_IsStatRaisingItem(s32 itemId);
void ItemMenu_DrawMessage(s32 unused, s32 message);
void Menu_HideEmptyEntryIcons(const u16 *items);
s32 ItemMenu_CountItems(s32 memberId);
s32 ItemMenu_GetItemQuantity(s32 memberId, s32 itemId);
s32 ItemMenu_CollectItems(struct Unit *unit, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *inventoryEntries, s32 iconStyle);
void ItemMenu_RefreshMember(s32 memberId, s32 mode);
void ItemMenu_NoOp(void);
void ItemMenu_DrawEquipPreview(s32 member, s32 slot, s32 mode, s32 target);
s32 Inventory_RemoveFirstUnflagged(s32 memberId);
s32 ItemMenu_CommandCursorX(s32 column, s32 row);
s32 ItemMenu_CommandCursorY(s32 column, s32 row);
s32 ItemMenu_ChooseCommand(void);
void ItemMenu_BuildCommands(s8 *commandStates);
void ItemMenu_DrawCommands(const s8 *commandStates, struct Window *window);
s32 Item_ClassifyUseMode(s32 memberId, s32 inventoryEntry);
void ItemMenu_TryBreak(void);
s32 ItemMenu_OpenDetail(s32 itemIndex);
s32 ItemMenu_ShowDetails(s32 value);
void ItemMenu_DrawItemDetails(struct Window *window, s32 inventoryEntry);
void ItemMenu_DrawStat(s32 statChange, s32 unused, struct Window *window, s32 x,
                       s32 y);
void ItemMenu_SetItemWindowRow5(void);
void ItemMenu_SetItemWindowRow3(void);
void ItemMenu_SetMessageWindowHeight5(void);
void ItemMenu_SetMessageWindowHeight6(void);
void ItemMenu_SetMessageWindowHeight7(void);
void ItemMenu_SetMessageWindowHeight3(void);
s32 ItemMenu_SelectGiveQuantity(s32 baseQuantity, s32 quantityRange,
                                s32 dropMode);
void ItemMenu_DrawItemHead(void);
s32 ItemMenu_ConfirmDrop(s32 item);
s32 ItemMenu_ConfirmEquip(s32 unused);
void Resource_LoadPairedBlocks(void);
s32 ItemMenu_BuildPagedList(struct PagedList *result, s32 index);
s32 ItemMenu_DrawItemDetailPage(struct Window *window, void *memberWork,
                                struct PagedList *pageState);
s32 ItemMenu_DrawNamePage(struct Window *window, s32 unused,
                          const struct PagedList *pageState);
s32 ItemMenu_ItemNamePageReturnTrue(void);
void ItemMenu_ItemNamePageNoOp(void);
s32 ItemMenu_RunList(s32 sideIndex);

s32 PsynergyMenu_Run(void);
s32 PsynergyMenu_RunSteps(s32 *outMember, s32 *unused, s32 *outAction);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 partySlot);
s32 PsynergyMenu_SelectMember(u16 *memberIds, u16 *unused);
void PsynergyMenu_RefreshMemberPsynergy(s32 memberId);
s32 PsynergyMenu_ReturnTrue(void);
void PsynergyMenu_NoOp(void);
s32 PsynergyMenu_SelectTarget(s32 targetMode);
s32 PsynergyMenu_SetShortcut(s32 memberId, s32 abilityId, s32 shortcutIndex);
s32 PsynergyMenu_DrawShortcuts(struct Window *window, s32 member);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_CollectActions(struct Unit *unit, u16 *actions, s32 mode);
s32 PsynergyMenu_BuildPagedList(struct PagedList *result, s32 index);
s32 PsynergyMenu_DrawDetailPage(struct Window *window, s32 *work,
                                struct PagedList *state);
s32 PsynergyMenu_DrawActionPage(struct Window *window, s32 unused,
                                const struct PagedList *pageState);
s32 PsynergyMenu_RunList(s32 sideIndex);
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);

s32 PartyMemberChooser_Open(void);
s32 PartyMemberChooser_Run(void);
s32 StatusScreen_Run(void);
s32 StatusScreen_RunPages(void);
s32 StatusScreen_SelectMember(s32 slot);
s32 DjinnMenu_ShowCurrentList(void);
s32 PartyMemberChooser_RunRearrange(void);
s32 PsynergyMenu_ChooseMember(void);
s32 PartyMemberChooser_MoveEntry(s32 selectedIndex, s32 direction);
void StatusScreen_Initialize(s32 unused0, s32 unused1, s32 unused2,
                             s32 unused3);
void StatusScreen_ShowMemberStatus(s32 member, s32 frame);
s32 StatusScreen_SelectCommand(void);
void StatusScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                      s32 invert);
void StatusScreen_DrawSelectionLabels(struct Window *target, s32 selected,
                                      const u8 *entries);
void StatusScreen_ShowMemberProgressMessage(struct Window *destination,
                                            s32 messageVariant,
                                            s32 preserveVariant);
void StatusScreen_DrawStatusAilments(struct Window *window, s32 memberId,
                                     s32 mode);
void Menu_BusyWait(void);
void ItemMenu_DrawMemberStatus(struct Window *window, s32 memberId, s32 flags);
s32 StatusScreen_BuildAvailability(u8 *ailments, s32 requested, s32 memberId);
s32 PagedList_Build(struct PagedList *result, s32 index);
void Menu_BuildPatternTiles(void);
void PsynergyMenu_DrawRange(struct Window *window, s32 x, s32 y, s32 range,
                            s32 unused);
s32 PsynergyMenu_DrawRangePage(struct Window *window, s32 unused,
                               struct PagedList *state);
s32 PsynergyMenu_DrawListPage(struct Window *window, s32 unused,
                              const struct PagedList *res);
s32 PsynergyMenu_SelectAction(void);
s32 PsynergyMenu_ActionReturnTrue(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 memberId);
void PsynergyMenu_PreparedIconsNoOp(void);
void PsynergyMenu_PreparedIconsSecondNoOp(void);
s32 ItemMenu_DrawEquipPage(struct Window *window, s32 unused,
                           struct PagedList *state);
s32 ItemMenu_DrawItemPage(struct Window *window, s32 unused,
                          struct PagedList *state);
s32 ItemMenu_SelectItem(void);
s32 Menu_MoveGridCursor(s32 *columnOut, s32 *rowOut, s32 direction);
s32 Menu_ReservedStatusOne(void);
void ItemMenu_DrawCategory(struct Window *window, s32 memberId, s32 mode);
void ItemMenu_DrawEquippedItemNames(struct Window *window, u16 *items);
void Menu_PlaceEntryActorsInGrid(s32 originX, s32 originY, s32 columns);
void Menu_PlaceEntryActorInGrid(struct WindowSprite *icon, s32 index,
                                s32 originX, s32 originY, s32 columns);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_PositionCategory(void);
s32 Menu_CreateEightEntryActors(struct Window *window);
void StatusScreen_ShowStatusIcons(const u8 *flags);
void ItemMenu_ResetCategory(void);
s32 StatusScreen_UpdateSelectionIcons(const u8 *enabled);

void Item_PrepareUsePalette(void);
void Item_UseNoOpCallback(void);
s32 Item_Use(s32 slot, s32 memberId, s32 targetId);
s32 Item_ReturnOne(void);
s32 Ability_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                           s32 fixedScale);
void Item_PlayUseAnimation(u32 item);
void Ability_PlayUseAnimation(s32 action);

s32 Menu_ReservedStatusZero(void);
s32 Menu_WrapIndex(s32 value, s32 modulus);
s32 Menu_SetFirstActorRowCoordinates(s32 yOffset);
s32 DjinnMenu_Run(void);
s32 DjinnMenu_RunScreen(void);
void DjinnMenu_AdjustPalettes(s32 brightnessDelta);
#ifdef UBFIX
typedef void MenuBackdropResult;
#else
typedef s32 MenuBackdropResult;
#endif
// The native wrapper forwards an implicit result; the defined interface is
// void.
MenuBackdropResult Menu_OpenBackdropScreen(void);
s32 PsynergyList_Diff(struct PsynergyEntry *before, struct PsynergyEntry *after,
                      u16 *actionsOut, s32 *gainedCountOut, s32 *lostCountOut);
s32 DjinnMenu_BuildLists(void *table);
#ifdef UBFIX
void DjinnMenu_DrawElementList(struct DjinnListTable *table);
#endif
s32 Window_SetInteriorPaletteBitRect(const struct Window *window, s32 x, s32 y,
                                     s32 width, s32 height, s32 palette);
void Window_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Window_SetInteriorRectPalette(void *window, s32 x, s32 y, s32 width,
                                   s32 height, s32 palette);
s32 DjinnMenu_ShowHelp(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 Djinn_ListUnitEntries(u16 *entriesOut, s32 memberId, s32 element);
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex);
s32 DjinnMenu_DrawStatPreview(struct Window *window, s32 x, s32 y, s32 memberId,
                              s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side);

void DjinnFigures_InitializeTopRow(void *unused0, s32 unused1);
void DjinnFigures_UpdateAllPositions(void);
void DjinnFigures_UpdateBottomRow(void);
void DjinnFigures_InitializeBottomRow(s32 unused0, s32 unused1);
s32 DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
s32 DjinnFigures_SetSlotPhase(s32 index, s32 phase);
s32 DjinnFigures_ReplaceSlot(s32 index, s32 resourceKind, s32 animation);
void Menu_StartPartyFiguresStanding(void);

s32 PsynergyList_RunCompareLoop(u32 mode);
s32 DjinnMenu_ShowMemberStatus(void);
s32 Djinn_MarkTradeOnlyRecipients(s8 *requiresTrade, s32 sourceMemberColumn);
s32 Djinn_CheckGiveBalance(s32 sourceMemberColumn, s32 targetMemberColumn);
void Djinn_CountOwned(u8 *ownedCounts);
void Resource_LoadPairedBlocksIfAvailable(void);
void IconMenu_ReleaseArrows(void);
s32 Icon_LoadResourceIntoSlot(s32 resourceId, s32 slot);
s32 Icon_CreateWithLoadedResource(struct Window *window, s32 x, s32 y,
                                  s32 resourceId);
s32 Icon_CreateStatChangeArrow(struct Window *window, s32 x, s32 y,
                               s32 variant);
s32 Icon_DrawVariantWithTileOffset(struct Window *window, s32 x, s32 y,
                                   s32 variant);

#endif
