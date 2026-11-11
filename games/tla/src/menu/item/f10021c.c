#include "types.h"
#include "edition.h"
#include "version.h"
#include "input.h"
#include "ram_buffer.h"
#include "battle_unit.h"
#include "menu_result.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

// The equip window's width in tiles, narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_WINDOW_WIDTH 13
#else
#define EQUIP_WINDOW_WIDTH 15
#endif

// The rows one page of the item list shows: Japanese fits six, the other
// editions five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

// The French list puts its cursor 4 pixels further right.
#if LANGUAGE == LANGUAGE_FRENCH
#define CURSOR_X 100
#else
#define CURSOR_X 96
#endif

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
s32 Window_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                          s32 frame);
void Menu_SpawnIconEntries(struct InventoryMenuState *menu, s32 window);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
s32 ItemMenu_CollectItems(struct BattleUnit *owner, u16 *items, s32 mode);
void WindowSprite_RedrawSavedRectFar(s32 window);
s32 InventoryMenu_SortByListOrder(u16 *items, s32 mode);
s32 Func_080ff7b4(struct MenuResult *result, s32 index);
void ItemMenu_DrawIcons(u16 *items, s32 mode);
void ItemMenu_DrawCategory(s32 window, s32 owner, s32 mode);
void wait(s32 frames);
s32 Func_081000e0(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
s32 PagedList_HandleInput(s32 mode, s32 count, s32 rows, s32 *row, s32 *page);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct InventoryMenuState *menu,
                                           s32 ownerId);
void WindowSprite_PrepareForRedrawFar(s32 window);
void WindowSprite_ClearListFar(s32 window);

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
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, EQUIP_WINDOW_WIDTH, 10, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
  Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        owner = Unit_Get(menu->paneOwner);
        menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
        WindowSprite_RedrawSavedRectFar(window);
        for (i = 0; i <= 14; i++)
          savedInventory[i] = owner->inventory[i];
        InventoryMenu_SortByListOrder(menu->items, 0);
        Func_080ff7b4(&state, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        WindowSprite_RedrawSavedRectFar(menu->equipWindow);
        ItemMenu_DrawCategory(menu->equipWindow, menu->paneOwner, 1);
        listNeedsRedraw = 1;
        wait(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        Func_081000e0(window, 0, &state);
      }
      ItemMenu_DrawEquipPage(window, 0, &state);
#if TLA_PATCH_1
      Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
      Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
#endif
      refresh = 0;
    }
    wait(1);
    navigation = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
                                       &state.row, &state.page);
    Func_080f8a44(CURSOR_X, state.row * 16 + 52);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
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
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->messageWindow);
  return result;
}
