#include "types.h"
#include "edition.h"
#include "version.h"
#include "input.h"
#include "ram_buffer.h"
#include "menu_result.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

// The rows of a page of the list and where the cursor points at its first row:
// the Japanese page lists six abilities, a row higher.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define CURSOR_X 58
#define CURSOR_Y 52
#else
#define PAGE_ROWS 5
#define CURSOR_X 55
#define CURSOR_Y 60
#endif

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

s32 Window_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                          s32 frame);
void Menu_SpawnIconEntries(struct PsynergyMenuState *menu, s32 window);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Menu_BuildPatternTiles(void);
s32 GameFlag_Test(s32 flag);
void *Unit_Get(s32 owner);
s32 Func_080fd6f0(void *unit, u16 *actions, s32 mode);
s32 Func_080ff7b4(struct MenuResult *result, s32 index);
void ItemMenu_HideAllIcons(void);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
void wait(s32 frames);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, struct MenuResult *state);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state);
s32 PagedList_HandleInput(s32 mode, s32 count, s32 rows, s32 *row, s32 *page);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyMenuState *menu,
                                           s32 ownerId);
void WindowSprite_PrepareForRedrawFar(s32 window);
void WindowSprite_RedrawSavedRectFar(s32 window);
void WindowSprite_ClearListFar(s32 window);

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
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
  Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
  Menu_BuildPatternTiles();
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        menu->psynergyCount =
            Func_080fd6f0(Unit_Get(menu->ownerId), menu->psynergies, 0);
        Func_080ff7b4(&state, 0);
        ItemMenu_HideAllIcons();
        PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
        listNeedsRedraw = 1;
        wait(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        PsynergyMenu_DrawListPage(window, 0, &state);
      }
      PsynergyMenu_DrawRangePage(window, 0, &state);
#if TLA_PATCH_2
      // German, Spanish, French and Italian draw the help again after each
      // page.
      Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
      Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 64, -24);
#endif
      refresh = 0;
    }
    wait(1);
    navigation = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
                                       &state.row, &state.page);
    Func_080f8a44(CURSOR_X, state.row * 16 + CURSOR_Y);
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
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->messageWindow);
  return result;
}
