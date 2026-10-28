// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 235 in tla-en. First difference: this issues `ldr r1, [r3, #0]` earlier
// (row 33) than the native does (row 35). No single empty asm or one-pass block
// reaches 0.
// First draft of Func_080ffcd4: the Psynergy page of the status menu, the
// twin of The Broken Seal's PsynergyMenu_SelectAction
// (games/tbs/src/menu/menu/status.c). ⚓️ runs one loop with a refresh state
// (2 rebuilds the member's list, 1 redraws the pages). A returns 1, B -1; L
// and R switch members. MsgSwitchCharacterHelp is message 4149 in English;
// it needs its name in the six catalogs before this links. Differ 8 of 235
// (2026-10-19), instruction order only: the native loads the message window
// and the help message before the moves around them and the owner id after
// the menu's ownerId address, here the other way round.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

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

struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[0x10];
  s8 tabIndex;
  u8 unk01d[0x0b];
  s32 statusWindow;
  u8 unk02c[4];
  s32 infoWindow;
  u8 unk034[0xbc];
  s32 messageWindow;
  u8 unk0f4[0xd0];
  u16 psynergies[32];
  u16 ownerTable[8];
  u8 psynergyCount;
  u8 ownerCount;
  u8 ownerId;
  u8 unk217[0x4d];
  s8 selectedIndexByOwner[8];
};

extern u8 MsgSwitchCharacterHelp[];

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
void Menu_SpawnIconEntries(struct PsynergyMenuState *menu, s32 window);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Func_080ff850(void);
s32 GameFlag_Test(s32 flag);
void *Unit_Get(s32 owner);
s32 Func_080fd6f0(void *unit, u16 *actions, s32 mode);
s32 Func_080ff7b4(struct MenuResult *result, s32 index);
void ItemMenu_HideAllIcons(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
void WaitFrames(s32 frames);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, struct MenuResult *state);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state);
s32 Func_080f8f9c(s32 mode, s32 count, s32 rows, s32 *row, s32 *page);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyMenuState *menu,
                                           s32 ownerId);
void RenderOutput_PrepareForRedrawFar(s32 window);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);

s32 Func_080ffcd4(void) {
  struct PsynergyMenuState *menu;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  struct MenuResult state;

  menu = Ram_HeapSlots->menuRuntime;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  UiWindow_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
  Func_080ff850();
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        menu->psynergyCount =
            Func_080fd6f0(Unit_Get(menu->ownerId), menu->psynergies, 0);
        Func_080ff7b4(&state, 0);
        ItemMenu_HideAllIcons();
        PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
        listNeedsRedraw = 1;
        WaitFrames(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        PsynergyMenu_DrawListPage(window, 0, &state);
      }
      PsynergyMenu_DrawRangePage(window, 0, &state);
      refresh = 0;
    }
    WaitFrames(1);
    navigation = Func_080f8f9c(0, state.entryCount, 5, &state.row, &state.page);
    Func_080f8a44(55, state.row * 16 + 60);
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
      ownerIndex = menu->tabIndex;
      menu->selectedIndexByOwner[menu->ownerTable[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & 0x100)
        ownerIndex++;
      else
        ownerIndex--;
      ownerIndex = (ownerIndex + menu->ownerCount) % menu->ownerCount;
      menu->selectedOwner = menu->ownerTable[ownerIndex];
      menu->ownerId = menu->ownerTable[ownerIndex];
      menu->tabIndex = ownerIndex;
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[ownerIndex]);
      refresh = 2;
    }
  }
  RenderOutput_PrepareForRedrawFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar(menu->messageWindow);
  return result;
}
