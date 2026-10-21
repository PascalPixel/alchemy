#ifndef ALCHEMY_UNIT_MENU_MENU_H
#define ALCHEMY_UNIT_MENU_MENU_H

#include "types.h"

struct BattleUnit;
struct DjinnListTable;
struct InventoryMenuState;
struct MenuResult;
struct OwnerActionSlot;
struct UiWindow;
struct RenderOutput;

// The item menu's sort orders (gMenu_ListOrder*): one byte per group, an
// item type, with LIST_ORDER_EQUIPPED for only the equipped items of that
// type, ending with LIST_ORDER_END.
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED 0x80
#define LIST_ORDER_END 0xff

/// The item menu's four sort orders (InventoryMenu_SortByListOrder), which
/// L + A cycles through in this order.
enum ListSort {
  /// The equipped items, the items that are not equipment, then the other
  /// equipment (gMenu_ListOrderMode0).
  LIST_SORT_EQUIPPED_THEN_ITEMS = 0,
  /// The equipped items, the other equipment, then the rest
  /// (gMenu_ListOrderMode1).
  LIST_SORT_EQUIPPED_THEN_EQUIPMENT = 1,
  LIST_SORT_EQUIPMENT_FIRST = 2, ///< gMenu_ListOrderMode2
  /// The items that are not equipment first (gMenu_ListOrderDefault).
  LIST_SORT_ITEMS_FIRST = 3,
  LIST_SORT_COUNT = 4
};

s32 Menu_ResumeFieldTasks(void);
s32 Menu_SuspendFieldTasks(void);
void UiWindow_InitializeWork(s32 unused);
s32 UiWindow_UpdateOrCreate(struct UiWindow **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 flags);
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style);
void Menu_DrawOwnerStatusPanel(struct UiWindow *unused, s32 owner, s32 slot,
                               s32 mode);
void UiText_DrawNumberRightAligned(s32 number, struct UiWindow *window, s32 x,
                                   s32 y);
void Ui_DrawValuePairRows(struct BattleUnit *unit, struct UiWindow *window);
void UiText_DrawStatComparison(const struct BattleUnit *after,
                               const struct BattleUnit *before,
                               struct UiWindow *window);
s32 UiIcon_CreateWithResource(struct UiWindow *window, s32 unused, s32 x,
                              s32 y);
s32 UiIcon_CreateWithResourceVariant(struct UiWindow *window, s32 x, s32 y);
void UiIcon_PrepareObject(struct RenderOutput *object);
void PsynergyMenu_CallIconRoutineWithValue(const void *unused, s32 unit);
struct UiWindow *UiMenu_CreateCursor(void *source);
void PsynergyMenu_InitializeEntryObjects(void *source, s32 originX, s32 originY,
                                         s32 spacing, s32 unused);
void Menu_ReleaseEntryObjects(void);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_PositionCursor(s32 xOffset, s32 yOffset);
void UiMenu_SlideCursor(s32 x, s32 y);
void PsynergyMenu_ReservedNoOp(void);
void PsynergyMenu_RefreshOwnerEntriesDefault(void);
void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns);
void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
                                     s32 originX, s32 originY, s32 columns);
void ItemMenu_PlaceIcon(struct RenderOutput **slot, s32 slotIndex, s32 originX,
                        s32 originY, s32 columns);
void ItemMenu_RefreshEntry(s32 layout);
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode);
void Menu_CopyListOrder(s32 mode, u8 *order);
s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 pageSize, s32 *cursor,
                         s32 *page);
void Palette_LightenBankHighlight(s32 bank);
void Menu_DrawPageIndicator(struct UiWindow *window, s32 itemCount,
                            s32 pageSize, s32 selectedPage, s32 rightEdge);
void Render_SetTilemapFlagRect(const struct UiWindow *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette);
void Palette_CopyObjectBankToBackground14(void);
void Menu_SetPageIcons(s32 pageSize, s32 first, struct UiWindow *window, s32 x,
                       s32 y);
void UiText_DrawWorkValueWithLabel(struct UiWindow *window);
void UiWindow_SetBounds(struct UiWindow *window, s32 x, s32 y, s32 width,
                        s32 height);
void UiWork_SetMenuBusy(void);
void UiWork_ClearMenuBusy(void);
s32 Audio_PlayCueReturnOne(s32 cue);
void Menu_CancelSoundTick(void);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
void UiPalette_SetColor15(void);
void UiPalette_SetColor2(void);
void UiPalette_SetColor4(void);
void FourObjectMotion_ClearSlotsAndSchedule(void);
void FourObjectMotion_ClearSlotsAndScheduleAlt(void);

#define LIST_PAGE_SIZE 5

s32 ItemMenu_Run(void);
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
s32 ItemMenu_Use(void);
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SpawnIconEntries(struct InventoryMenuState *state,
                           struct UiWindow *window);
void ItemMenu_HideAllIcons(void);
void ItemMenu_HidePageIcons(void);
void ItemMenu_Close(void);
s32 ItemMenu_SelectMember(s32 pane);
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items);
void ItemMenu_OpenCategory(s32 ownerId);
s32 ItemMenu_SelectTarget(s32 mode);
void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StartCompatibilityIndicators(void);
s32 ItemMenu_IsSpecial(s32 itemId);
void ItemMenu_DrawMessage(s32 unused, s32 message);
void Menu_HideEmptyEntryIcons(const u16 *items);
s32 ItemMenu_CountItems(s32 ownerId);
s32 InventoryMenu_GetItemQuantity(s32 ownerId, s32 itemId);
s32 ItemMenu_CollectItems(struct BattleUnit *unit, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *inventoryEntries, s32 iconStyle);
void ItemMenu_RefreshOwner(s32 ownerId, s32 mode);
void InventoryMenu_NoOp(void);
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target);
s32 Inventory_RemoveFirstUnflagged(s32 ownerId);
s32 ItemMenu_CommandCursorX(s32 column, s32 row);
s32 ItemMenu_CommandCursorY(s32 column, s32 row);
s32 ItemMenu_ChooseCommand(void);
void ItemMenu_BuildCommands(s8 *commandStates);
void ItemMenu_DrawCommands(const s8 *commandStates, struct UiWindow *window);
s32 Item_ClassifyUseMode(s32 ownerId, s32 inventoryEntry);
void ItemMenu_TryBreak(void);
s32 ItemMenu_OpenDetail(s32 itemIndex);
s32 Menu_SelectQuantity(s32 value);
void ItemMenu_DrawItemDetails(struct UiWindow *window, s32 inventoryEntry);
void ItemMenu_DrawStat(s32 statChange, s32 unused, struct UiWindow *window,
                       s32 x, s32 y);
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
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index);
s32 ItemMenu_DrawItemDetailPage(struct UiWindow *window, void *ownerWork,
                                struct MenuResult *pageState);
s32 ItemMenu_DrawNamePage(struct UiWindow *window, s32 unused,
                          const struct MenuResult *pageState);
s32 InventoryMenu_ItemNamePageReturnTrue(void);
void InventoryMenu_ItemNamePageNoOp(void);
s32 ItemMenu_RunList(s32 paneIndex);

s32 PsynergyMenu_Run(void);
s32 Menu_ResolveSelectedAction(s32 *outOwner, s32 *unused, s32 *outAction);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 partySlot);
s32 PsynergyMenu_SelectMember(u16 *ownerIds, u16 *unused);
void PsynergyMenu_RefreshOwnerPsynergy(s32 ownerId);
s32 PsynergyMenu_ReturnTrue(void);
void PsynergyMenu_NoOp(void);
s32 PsynergyMenu_SelectTarget(s32 targetMode);
s32 PsynergyMenu_SetShortcut(s32 ownerId, s32 actionId, s32 shortcutIndex);
s32 PsynergyMenu_DrawShortcuts(struct UiWindow *window, s32 owner);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_CollectActions(struct BattleUnit *unit, u16 *actions,
                                s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index);
s32 PsynergyMenu_DrawDetailPage(struct UiWindow *window, s32 *work,
                                struct MenuResult *state);
s32 PsynergyMenu_DrawActionPage(struct UiWindow *window, s32 unused,
                                const struct MenuResult *pageState);
s32 PsynergyMenu_RunList(s32 paneIndex);
s32 PsynergyMenu_IsActionRestricted(s32 actionEntry);

s32 Menu_OpenCharacterSelector(void);
s32 CharacterSelector_Run(void);
s32 StatusMenu_Run(void);
s32 Menu_RunActionFlow(void);
s32 CharacterMenu_SelectOwner(s32 slot);
s32 DjinnMenu_ShowCurrentList(void);
s32 CharacterSelector_RunRearrange(void);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_MoveEntry(s32 selectedIndex, s32 direction);
void Menu_InitSelectorCursorAndEntries(s32 unused0, s32 unused1, s32 unused2,
                                       s32 unused3);
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame);
s32 CharacterMenu_SelectCommand(void);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert);
void CharacterMenu_DrawSelectionLabels(struct UiWindow *target, s32 selected,
                                       const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination,
                                         s32 messageVariant,
                                         s32 preserveVariant);
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 ownerId,
                                      s32 mode);
void Runtime_RunBusyWaitDelay(void);
void ItemMenu_DrawOwnerStatus(struct UiWindow *window, s32 ownerId, s32 flags);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId);
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index);
void Menu_BuildPatternTiles(void);
void PsynergyMenu_DrawRange(struct UiWindow *window, s32 x, s32 y, s32 range,
                            s32 unused);
s32 PsynergyMenu_DrawRangePage(struct UiWindow *window, s32 unused,
                               struct MenuResult *state);
s32 PsynergyMenu_DrawListPage(struct UiWindow *window, s32 unused,
                              const struct MenuResult *res);
s32 PsynergyMenu_SelectAction(void);
s32 PsynergyMenu_ActionReturnTrue(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 ownerId);
void PsynergyMenu_PreparedIconsNoOp(void);
void PsynergyMenu_PreparedIconsSecondNoOp(void);
s32 ItemMenu_DrawEquipPage(struct UiWindow *window, s32 unused,
                           struct MenuResult *state);
s32 Shop_DrawItemPage(struct UiWindow *window, s32 unused,
                      struct MenuResult *state);
s32 ItemMenu_SelectItem(void);
s32 Menu_MoveGridCursor(s32 *columnOut, s32 *rowOut, s32 direction);
s32 Menu_ReservedStatusOne(void);
void ItemMenu_DrawCategory(struct UiWindow *window, s32 ownerId, s32 mode);
void ItemMenu_DrawEquippedItemNames(struct UiWindow *window, u16 *items);
void Menu_PlaceEntryObjectsInGrid(s32 originX, s32 originY, s32 columns);
void Menu_PlaceEntryObjectInGrid(struct RenderOutput *icon, s32 index,
                                 s32 originX, s32 originY, s32 columns);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_PosCategory(void);
s32 Menu_CreateEightEntryObjects(struct UiWindow *window);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ResetCategory(void);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);

void Item_PrepareUsePalette(void);
void Item_UseNoOpCallback(void);
s32 Item_Use(s32 slot, s32 ownerId, s32 targetId);
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
void Graphics_AdjustPaletteBank(s32 brightnessDelta);
#ifdef UBFIX
typedef void MenuBackdropResult;
#else
typedef s32 MenuBackdropResult;
#endif
// The native wrapper forwards an implicit result; the defined interface is
// void.
MenuBackdropResult Menu_OpenBackdropScreen(void);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *before,
                          struct OwnerActionSlot *after, u16 *actionsOut,
                          s32 *gainedCountOut, s32 *lostCountOut);
s32 DjinnMenu_BuildLists(void *table);
#ifdef UBFIX
void DjinnMenu_DrawElementList(struct DjinnListTable *table);
#endif
s32 UiWindow_SetInteriorPaletteBitRect(const struct UiWindow *window, s32 x,
                                       s32 y, s32 width, s32 height,
                                       s32 palette);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_SetInteriorRectPalette(void *window, s32 x, s32 y, s32 width,
                                     s32 height, s32 palette);
s32 DjinnMenu_ShowHelp(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 Djinn_ListOwnerEntries(u16 *entriesOut, s32 ownerId, s32 element);
s32 DjinnMenu_DrawPairPreview(s32 mode, s32 pageIndex);
s32 DjinnMenu_DrawStatPreview(struct UiWindow *window, s32 x, s32 y,
                              s32 ownerId, s32 removePairSlot, s32 addPairSlot,
                              s32 previewMode, s32 page, s32 side);

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1);
void FourObjectMotion_UpdateAllPositions(void);
void FourObjectMotion_UpdateBottomRow(void);
void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1);
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 index, s32 resourceKind, s32 value);
void Menu_StartPartyFiguresWalking(void);

s32 OwnerAction_RunCompareLoop(u32 mode);
s32 DjinnMenu_ShowOwnerStatus(void);
s32 Djinn_MarkBalancedEntries(s8 *requiresTrade, s32 sourceOwnerColumn);
s32 Djinn_CheckTurnBalance(s32 sourceOwnerColumn, s32 targetOwnerColumn);
void Djinn_CountTurns(u8 *ownedCounts);
void Resource_LoadPairedBlocksIfAvailable(void);
void Menu_ResetTwoResourceEntries(void);
s32 UiIcon_LoadResourceIntoSlot(s32 resourceId, s32 slot);
s32 UiIcon_CreateWithLoadedResource(struct UiWindow *window, s32 x, s32 y,
                                    s32 resourceId);
s32 UiIcon_CreateStatChangeArrow(struct UiWindow *window, s32 x, s32 y,
                                 s32 variant);
s32 UiIcon_DrawVariantWithTileOffset(struct UiWindow *window, s32 x, s32 y,
                                     s32 variant);

#endif
