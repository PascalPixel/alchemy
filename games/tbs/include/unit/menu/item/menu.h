#ifndef ALCHEMY_UNIT_MENU_ITEM_MENU_H
#define ALCHEMY_UNIT_MENU_ITEM_MENU_H

#include "types.h"

struct BtlUnit;
struct DjinnListTable;
struct InventoryMenuState;
struct MenuResult;
struct OwnerActionSlot;
struct RenderInput;
struct RenderOutput;
struct UiWindow;

s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
void UiWindow_InitializeWork(s32 unused);
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags);
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style);
void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode);
void UiText_DrawNumberRightAlignedFar(s32 num, s32 win, s32 x, s32 y);
void Ui_DrawValuePairRows(struct BtlUnit *unit, s32 win);
void UiText_DrawStatComparison(const struct BtlUnit *alt, const struct BtlUnit *base, s32 work);
s32 UiIcon_CreateWithResource(struct RenderInput *window, s32 unused, s32 x, s32 y);
s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);
void UiIcon_PrepareObject(struct RenderOutput *object);
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit);
s32 UiMenu_CreateCursor(void *source);
void PsynergyMenu_InitializeEntryObjects(void *source, s32 origin_x, s32 origin_y, s32 spacing, s32 unused);
void Menu_ReleaseEntryObjects(void);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset);
void UiMenu_SlideCursor(s32 x, s32 y);
void PsynergyMenu_ReservedNoOp(void);
void PsynergyMenu_RefreshOwnerEntriesDefault(void);
void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns);
void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index, s32 origin_x, s32 origin_y, s32 columns);
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0, s32 cols);
void ItemMenu_RefreshEntry(s32 layout);
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode);
void Menu_CopyListOrder(s32 mode, u8 *order);
s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor, s32 *page);
void Palette_LightenBankHighlight(s32 bank);
void Menu_DrawPageIndicator(s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void Render_SetTilemapFlagRect(const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field);
void Palette_CopyObjectBankToBackground14(void);
void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void UiText_DrawWorkValueWithLabel(s32 work);
void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width, s32 height);
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

#endif
