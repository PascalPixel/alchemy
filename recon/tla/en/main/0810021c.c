// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 10 of 300 in tla-en. First difference: the native issues `movs r2, #0`
// earlier (row 16) than this does (row 21).
// First draft of Func_0810021c: the item page of the status menu, the twin
// of The Broken Seal's ItemMenu_SelectItem (games/tbs/src/menu/menu/status.c).
// ⚓️ runs one loop with a refresh state, as Func_080ffcd4 does: 2 collects
// the member's items again, 1 redraws the pages. A returns 1, B -1; L and R
// switch members, putting the member's inventory back as it was. The help
// text is MsgSwitchCharacterHelp (message 4149 in English) and the message
// three after it; the name needs to be in the six catalogs before this links.
// Differ 20 of 300 (2026-10-19), instruction order only: the native stores
// result and listNeedsRedraw before it builds the owner offset, and loads the
// message window and the help message before the moves around them. state is
// declared before savedInventory for the native stack layout (differ 34).
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "battle_unit.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

struct InventoryMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[0x10];
  s8 paneIndex;
  u8 unk01d[0x0b];
  s32 statusWindow;
  u8 unk02c[4];
  s32 infoWindow;
  s32 equipWindow;
  u8 unk038[0xb8];
  s32 messageWindow;
  u8 unk0f4[0xd0];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner;
  u8 unk217[0x4d];
  s8 selectedIndexByOwner[8];
};

extern u8 MsgSwitchCharacterHelp[];

void *Unit_Get(s32 owner);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
void Menu_SpawnIconEntries(struct InventoryMenuState *menu, s32 window);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
s32 ItemMenu_CollectItems(struct BattleUnit *owner, u16 *items, s32 mode);
void RenderOutput_RedrawSavedRectFar(s32 window);
void InventoryMenu_SortByListOrder(u16 *items, s32 mode);
s32 Func_080ff7b4(struct MenuResult *result, s32 index);
void ItemMenu_DrawIcons(u16 *items, s32 mode);
void ItemMenu_DrawCategory(s32 window, s32 owner, s32 mode);
void WaitFrames(s32 frames);
s32 Func_081000e0(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
s32 Func_080f8f9c(s32 mode, s32 count, s32 rows, s32 *row, s32 *page);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct InventoryMenuState *menu,
                                           s32 ownerId);
void RenderOutput_PrepareForRedrawFar(s32 window);
void RenderOutput_ClearListFar(s32 window);

s32 Func_0810021c(void) {
  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  s32 i;
  struct MenuResult state;
  u16 savedInventory[15];

  menu = Ram_HeapSlots->menuRuntime;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  owner = Unit_Get(menu->paneOwner);
  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 10, 15, 10, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        owner = Unit_Get(menu->paneOwner);
        menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
        RenderOutput_RedrawSavedRectFar(window);
        for (i = 0; i <= 14; i++)
          savedInventory[i] = owner->inventory[i];
        InventoryMenu_SortByListOrder(menu->items, 0);
        Func_080ff7b4(&state, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        RenderOutput_RedrawSavedRectFar(menu->equipWindow);
        ItemMenu_DrawCategory(menu->equipWindow, menu->paneOwner, 1);
        listNeedsRedraw = 1;
        WaitFrames(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        Func_081000e0(window, 0, &state);
      }
      ItemMenu_DrawEquipPage(window, 0, &state);
      UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3, window, 0,
                                      -24);
      UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
      refresh = 0;
    }
    WaitFrames(1);
    navigation = Func_080f8f9c(0, state.entryCount, 5, &state.row, &state.page);
    Func_080f8a44(96, state.row * 16 + 52);
    if (navigation == 1) {
      listNeedsRedraw = 1;
      refresh = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if ((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) {
      Sound_PlayFar(111);
      ownerIndex = menu->paneIndex;
      menu->selectedIndexByOwner[menu->ownerIds[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & 0x100)
        ownerIndex++;
      else
        ownerIndex--;
      for (i = 0; i <= 14; i++)
        owner->inventory[i] = savedInventory[i];
      ownerIndex = (ownerIndex + menu->partyCount) % menu->partyCount;
      menu->selectedOwner = menu->ownerIds[ownerIndex];
      menu->paneOwner = menu->ownerIds[ownerIndex];
      menu->paneIndex = ownerIndex;
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
      refresh = 2;
    }
  }
  RenderOutput_PrepareForRedrawFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar(menu->messageWindow);
  return result;
}
