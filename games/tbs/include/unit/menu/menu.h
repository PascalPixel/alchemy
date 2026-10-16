#ifndef ALCHEMY_UNIT_MENU_MENU_H
#define ALCHEMY_UNIT_MENU_MENU_H

#include "types.h"

struct BattleUnit;
struct DjinnListTable;
struct InventoryMenuState;
struct MenuResult;
struct OwnerActionSlot;
struct RenderInput;
struct RenderOutput;
struct UiWindow;

// The item menu's sort orders (gMenu_ListOrder*): one byte per group, an
// item type, with LIST_ORDER_EQUIPPED for only the equipped items of that
// type, ending with LIST_ORDER_END.
#define LIST_ORDER_TYPE_MASK 0x7f
#define LIST_ORDER_EQUIPPED 0x80
#define LIST_ORDER_END 0xff

s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
void UiWindow_InitializeWork(s32 unused);
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags);
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style);
void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode);
void UiText_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y);
void Ui_DrawValuePairRows(struct BattleUnit *unit, s32 window);
void UiText_DrawStatComparison(const struct BattleUnit *after,
                               const struct BattleUnit *before, s32 window);
s32 UiIcon_CreateWithResource(struct RenderInput *window, s32 unused, s32 x,
                              s32 y);
s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);
void UiIcon_PrepareObject(struct RenderOutput *object);
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit);
s32 UiMenu_CreateCursor(void *source);
void PsynergyMenu_InitializeEntryObjects(void *source, s32 originX, s32 originY,
                                         s32 spacing, s32 unused);
void Menu_ReleaseEntryObjects(void);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_PositionCursor(s32 x_offset, s32 yOffset);
void UiMenu_SlideCursor(s32 x, s32 y);
void PsynergyMenu_ReservedNoOp(void);
void PsynergyMenu_RefreshOwnerEntriesDefault(void);
void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns);
void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
                                     s32 originX, s32 originY, s32 columns);
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0,
                       s32 cols);
void ItemMenu_RefreshEntry(s32 layout);
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode);
void Menu_CopyListOrder(s32 mode, u8 *order);
s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor,
                         s32 *page);
void Palette_LightenBankHighlight(s32 bank);
void Menu_DrawPageIndicator(s32 window, s32 itemCount, s32 page_size,
                            s32 selected_page, s32 right_edge);
void Render_SetTilemapFlagRect(const struct UiWindow *object, s32 x, s32 y,
                               s32 width, s32 height, u32 palette);
void Palette_CopyObjectBankToBackground14(void);
void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void UiText_DrawWorkValueWithLabel(s32 work);
void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width,
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

s32 RunAssetSelectionScreen(void);
s32 ItemMenu_RunCommands(s32 *owner_out, s32 *target_out, s32 *item_out);
s32 ItemMenu_Use(void);
void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1);
void ItemMenu_HideAllIcons(void);
void ItemMenu_HidePageIcons(void);
void ItemMenu_Close(void);
s32 ItemMenu_PrepOwner(s32 pane);
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items);
void ItemMenu_OpenCategory(s32 owner_id);
s32 ItemMenu_SelectTarget(s32 mode);
void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StartCompatibilityIndicators(void);
s32 ItemMenu_IsSpecial(s32 item_id);
void ItemMenu_DrawMsg(s32 unused, s32 message);
void Menu_HideEmptyEntryIcons(const u16 *items);
s32 ItemMenu_Count(s32 owner_id);
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);
s32 ItemMenu_Collect(struct BattleUnit *owner, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
void ItemMenu_RefreshOwner(s32 owner_id, s32 mode);
void InventoryMenu_NoOp(void);
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 ItemMenu_CmdCursorX(s32 column, s32 row);
s32 ItemMenu_CmdCursorY(s32 column, s32 row);
s32 ItemMenu_ChooseCmd(void);
void ItemMenu_BuildCmd(s8 *command_states);
void ItemMenu_DrawCmd(const s8 *command_states, s32 window);
s32 Item_ClassifyUseMode(s32 owner, s32 itemId);
void ItemMenu_TryBreak(void);
s32 ItemMenu_OpenDetail(s32 item_index);
s32 Menu_SelectQuantity(s32 value);
void ItemMenu_DrawItemDetails(s32 window, s32 item);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);
void ItemMenu_SetItemWin5(void);
void ItemMenu_SetItemWin3(void);
void ItemMenu_SetMsgWin5(void);
void ItemMenu_SetMsgWin6(void);
void ItemMenu_SetMsgWin7(void);
void ItemMenu_SetMsgWin3(void);
s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single);
void ItemMenu_DrawItemHead(void);
s32 ItemMenu_ConfirmDrop(s32 a0);
s32 ItemMenu_ConfirmEquip(s32 unused);
void Resource_LoadPairedBlocks(void);
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index);
s32 ItemMenu_DrawItemDetailPage(s32 window, void *work,
                                struct MenuResult *state);
s32 ItemMenu_DrawNamePage(s32 window, s32 unused,
                          const struct MenuResult *state);
s32 InventoryMenu_ItemNamePageReturnTrue(void);
void InventoryMenu_ItemNamePageNoOp(void);
s32 ItemMenu_RunList(s32 pane);

s32 Menu_OpenConfirmPrompt(void);
s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action);
s32 PsynergyMenu_ClassifySelectedPsynergy(void);
s32 PsynergyMenu_SelectPartySlot(s32 party_slot);
s32 PsynergyMenu_SetupActionIcons(u16 *ownerIds, u16 *unused);
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id);
s32 PsynergyMenu_ReturnTrue(void);
void PsynergyMenu_NoOp(void);
s32 PsynergyMenu_SelectTarget(s32 mode);
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut);
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);
void PsynergyMenu_CreateEntryGrid(void);
void PsynergyMenu_CloseWindows(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
s32 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *actions,
                                s32 mode);
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work,
                                struct MenuResult *state);
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused,
                                const struct MenuResult *state);
s32 PsynergyMenu_RunList(s32 pane);
s32 PsynergyMenu_IsActionRestricted(s32 no);

s32 Menu_OpenCharacterSelector(void);
s32 CharacterSelector_Run(void);
s32 ActionMenu_Open(void);
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
void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected,
                                       const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination,
                                         s32 message_variant,
                                         s32 preserve_variant);
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 ownerId,
                                      s32 mode);
void Runtime_RunBusyWaitDelay(void);
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 id);
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index);
void Menu_BuildPatternTiles(void);
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused,
                              const struct MenuResult *res);
s32 PsynergyMenu_SelectAction(void);
s32 PsynergyMenu_ActionReturnTrue(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id);
void PsynergyMenu_PreparedIconsNoOp(void);
void PsynergyMenu_PreparedIconsSecondNoOp(void);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_SelectItem(void);
s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2);
s32 Menu_ReservedStatusOne(void);
void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
void Menu_PlaceEntryObjectsInGrid(s32 originX, s32 originY, s32 phase);
void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
                                 s32 originX, s32 originY, s32 phase);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
void ItemMenu_PosCategory(void);
s32 Menu_CreateEightEntryObjects(s32 resource);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ResetCategory(void);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);

void Item_PrepareUsePalette(void);
void Item_UseNoOpCallback(void);
s32 Item_Use(s32 slot, s32 owner_id, s32 targetId);
s32 Item_ReturnOne(void);
s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 sourceId, s32 targetId,
                                s32 fixed_scale);
void Item_PlayUseAnimation(u32 item);
void Ability_PlayUseAnimation(s32 action);

s32 Menu_ReservedStatusZero(void);
s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1);
s32 Menu_SetFirstObjectRowCoordinates(s32 arg0);
s32 ItemMenu_Run(void);
s32 DjinnMenu_RunScreen(void);
void Graphics_AdjustPaletteBank(s32 d);
s32 Menu_OpenBackdropScreen(void);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *first_count, s32 *second_count);
s32 Menu_ComputeEntryValues(void *table);
/* DjinnMenu_DrawElementList is called before its definition with no prototype
 * in scope. */
s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                            s32 arg5);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width,
                                      s32 height, s32 palette);
s32 DjinnMenu_ShowHelp(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element);
s32 Menu_RunPairedEntryAction(s32 mode, s32 param);
s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner, s32 give,
                              s32 take, s32 mode, s32 page, s32 side);

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1);
void FourObjectMotion_UpdateAllPositions(void);
void FourObjectMotion_UpdateBottomRow(void);
void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1);
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value);
void Menu_EnableAllItemObjects(void);

s32 OwnerAction_RunCompareLoop(u32 mode);
s32 DjinnMenu_ShowOwnerStatus(void);
s32 Djinn_MarkBalancedEntries(s8 *table, s32 self);
s32 Djinn_CheckTurnBalance(s32 from, s32 to);
void Djinn_CountTurns(u8 *counts);
void Resource_LoadPairedBlocksIfAvailable(void);
void Menu_ResetTwoResourceEntries(void);
s32 UiIcon_LoadResourceIntoSlot(s32 resourceId, s32 slot);
s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y,
                                    s32 resourceId);
s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y,
                                 s32 variant);
s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y,
                                     s32 variant);

#endif
