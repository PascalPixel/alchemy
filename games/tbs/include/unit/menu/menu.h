#ifndef ALCHEMY_UNIT_MENU_MENU_H
#define ALCHEMY_UNIT_MENU_MENU_H

#include "types.h"

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
void Icon_PrepareObject(struct WindowSprite *object);
void PsynergyMenu_CallIconRoutineWithValue(const void *unused, s32 unit);
struct Window *MenuCursor_Create(void *source);
void PsynergyMenu_InitializeEntryObjects(void *source, s32 originX, s32 originY,
                                         s32 spacing, s32 unused);
void Menu_ReleaseEntryObjects(void);
void Menu_UpdateEntryObjectTransforms(void);
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
void Palette_LightenBankHighlight(s32 bank);
void PagedList_DrawPageNumbers(struct Window *window, s32 itemCount,
                               s32 pageSize, s32 selectedPage, s32 rightEdge);
void Render_SetTilemapFlagRect(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette);
void Palette_CopyObjectBankToBackground14(void);
void Menu_SetPageIcons(s32 pageSize, s32 first, struct Window *window, s32 x,
                       s32 y);
void Text_DrawWorkValueWithLabel(struct Window *window);
void Window_SetBounds(struct Window *window, s32 x, s32 y, s32 width,
                      s32 height);
void Window_HoldTilemapUpload(void);
void Window_ReleaseTilemapUpload(void);
s32 Audio_PlayCueReturnOne(s32 cue);
void Menu_CancelSoundTick(void);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
void WindowPalette_SetColor15(void);
void WindowPalette_SetColor2(void);
void WindowPalette_SetColor4(void);
void DjinnFigures_ClearSlotsAndSchedule(void);
void DjinnFigures_ClearSlotsAndScheduleAlt(void);

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
void EquipmentMenu_StartCompatibilityIndicators(void);
s32 ItemMenu_IsSpecial(s32 itemId);
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
s32 PsynergyMenu_SetShortcut(s32 memberId, s32 actionId, s32 shortcutIndex);
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

s32 MemberChooser_Open(void);
s32 MemberChooser_Run(void);
s32 SummaryScreen_Run(void);
s32 SummaryScreen_RunPages(void);
s32 SummaryScreen_SelectMember(s32 slot);
s32 DjinnMenu_ShowCurrentList(void);
s32 MemberChooser_RunRearrange(void);
s32 PsynergyMenu_ChooseMember(void);
s32 MemberChooser_MoveEntry(s32 selectedIndex, s32 direction);
void Menu_InitializeSelectorCursorAndEntries(s32 unused0, s32 unused1,
                                             s32 unused2, s32 unused3);
void SummaryScreen_ShowMemberStatus(s32 member, s32 frame);
s32 SummaryScreen_SelectCommand(void);
void SummaryScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert);
void SummaryScreen_DrawSelectionLabels(struct Window *target, s32 selected,
                                       const u8 *entries);
void SummaryScreen_ShowMemberProgressMessage(struct Window *destination,
                                             s32 messageVariant,
                                             s32 preserveVariant);
void SummaryScreen_DrawStatusAilments(struct Window *window, s32 memberId,
                                      s32 mode);
void Menu_BusyWait(void);
void ItemMenu_DrawMemberStatus(struct Window *window, s32 memberId, s32 flags);
s32 SummaryScreen_BuildAvailability(u8 *ailments, s32 requested, s32 memberId);
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
s32 Shop_DrawItemPage(struct Window *window, s32 unused,
                      struct PagedList *state);
s32 ItemMenu_SelectItem(void);
s32 Menu_MoveGridCursor(s32 *columnOut, s32 *rowOut, s32 direction);
s32 Menu_ReservedStatusOne(void);
void ItemMenu_DrawCategory(struct Window *window, s32 memberId, s32 mode);
void ItemMenu_DrawEquippedItemNames(struct Window *window, u16 *items);
void Menu_PlaceEntryObjectsInGrid(s32 originX, s32 originY, s32 columns);
void Menu_PlaceEntryObjectInGrid(struct WindowSprite *icon, s32 index,
                                 s32 originX, s32 originY, s32 columns);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_PositionCategory(void);
s32 Menu_CreateEightEntryObjects(struct Window *window);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ResetCategory(void);
s32 SummaryScreen_UpdateSelectionIcons(const u8 *enabled);

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
s32 Menu_SetFirstObjectRowCoordinates(s32 yOffset);
s32 DjinnMenu_Run(void);
s32 DjinnMenu_RunScreen(void);
void DjinnMenu_AdjustPaletteBank(s32 brightnessDelta);
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
s32 DjinnFigures_ReplaceSlot(s32 index, s32 resourceKind, s32 value);
void Menu_StartPartyFiguresWalking(void);

s32 PsynergyList_RunCompareLoop(u32 mode);
s32 DjinnMenu_ShowMemberStatus(void);
s32 Djinn_MarkBalancedEntries(s8 *requiresTrade, s32 sourceMemberColumn);
s32 Djinn_CheckTurnBalance(s32 sourceMemberColumn, s32 targetMemberColumn);
void Djinn_CountTurns(u8 *ownedCounts);
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
