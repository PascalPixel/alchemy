#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "macros.h"
#include "owner_state.h"
#include "ram_buffer.h"
#include "task.h"
#include "types.h"
#include "window.h"
#include "djinn_menu.h"

extern u8 MsgCurrentStatus[];
extern u8 gMenu_BackdropBorderTile[];

void WindowSprite_ClearListFar(s32 window);
void WindowSprite_PrepareForRedrawFar(s32 window);
void WindowSprite_RedrawSavedRectFar(s32 window);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 Window_UpdateOrCreate(void *handle, s32 x, s32 y, s32 width, s32 height,
                          s32 style);
void Window_CloseIfOpen(void *window, s32 mode);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *block);
s32 PsynergyList_Diff(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                      u16 *out, s32 *firstCount, s32 *secondCount);
void Func_08103218(s32 window, s32 x, s32 y, s32 owner, s32 a, s32 b, s32 c,
                   s32 param, s32 d);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
void MenuIcon_SetPosition(s32 index, s32 x, s32 y);
void Input_ResetKeyRepeat(void);
void Sound_PlayFar(s32 cue);
void Menu_UpdateEntryObjectTransforms(void);
void wait(s32 frames);

s32 Func_08104540(void) {
  s32 selectedPages[2];
  s32 pageCounts[2];
  s32 gained;
  s32 lost;
  s32 redraw;
  s32 frame;
  void *leftWindowSlot;
  void *rightWindowSlot;
  s32 *helpWindowSlot;
  struct WindowWork *render;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuWork *menu;
  struct OwnerActionState *unit;
  struct DjinnPreviewWindow *window;
  u16 *listedActions;
  void *saved;
  s32 result;
  s32 tile;
  s32 pageIndex;
  s32 side;
  u8 *pairMember;
  s32 *counts;
  s32 *selected;
  s32 blink;

  menu = Ram_HeapSlots->menuRuntime;
  redraw = 1;
  result = 0;
  frame = 0;
  menu->cursor->active = 13;
  /* FAKEMATCH(temporary): selected = selectedPages, the page array read through
   * a pointer; as selectedPages itself, 12 of 436 instructions differ in
   * tla-en. */
  selected = selectedPages;
  selected[1] = selected[0] = result;
  for (side = 3; side >= 0; side--)
    menu->memberY[side] = 200;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  /* FAKEMATCH(temporary): counts = pageCounts, the count array read through a
   * pointer; as pageCounts itself, 55 of 436 instructions differ in tla-en. */
  counts = pageCounts;
  counts[1] = counts[0] = 1;
  listedActions = Heap_AllocEwram(96);
  saved = Heap_AllocEwram(332);
  unit = Unit_Get(menu->shownMember[0]);
  counts[1] = counts[0] = PsynergyList_Diff(
      unit->actionSlots, unit->actionSlots, listedActions, &gained, &lost);
  Heap_Release(saved);
  Heap_Release(listedActions);
  counts[0] = (counts[0] - 1) / 6 + 1;
  if (counts[0] == 0)
    counts[0] = 1;
  counts[1] = (counts[1] - 1) / 6 + 1;
  if (counts[1] == 0)
    counts[1] = 1;
  leftWindowSlot = &menu->window;
  Window_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, 2);
  rightWindowSlot = &menu->secondWindow;
  Window_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, 2);
  helpWindowSlot = &menu->helpWindow;
  WindowSprite_RedrawSavedRectFar(*helpWindowSlot);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Text_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, menu->statusWindow, 0,
                                0);
  Text_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, menu->statusWindow,
                                0, 16);
  side = 0;
  while (1) {
    s32 *pageCount;
    s32 *selectedPage;
    /* FAKEMATCH(temporary): selectedPage = &selected[side], taken at the top of
     * the loop; as selected[side] at each page turn, 22 of 436 instructions
     * differ in tla-en. */
    selectedPage = &selected[side];
    render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
    pressedKeys = gInput.pressed;
    repeatedKeys = gInput.repeat;
    /* FAKEMATCH(temporary): pageCount = &pageCounts[side], taken at the top of
     * the loop; as pageCounts[side] at each page turn, 227 of 436 instructions
     * differ in tla-en. */
    pageCount = &pageCounts[side];
    pairMember = menu->pairMember;
    if (redraw) {
      render->menuBusy = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar((s32)menu->secondWindow);
      Func_08103218(menu->window, 0, 0, *pairMember, 0, 0, 3, 0, 1);
      Func_08103218((s32)menu->secondWindow, 0, 0, *pairMember, 0, 0, 3,
                    selectedPages[0] + 1, 1);
      render->menuBusy = 0;
    }
    if (counts[side] > 1) {
      window = menu->secondWindow;
      for (pageIndex = 0; pageIndex < counts[side]; pageIndex++) {
        tile = 0xf031 + pageIndex;
        if (pageIndex > 8)
          tile = 0xf03f;
        if (pageIndex == selected[side])
          tile -= 0x1000;
        Window_SetTilemapEntryFar((s32)window, tile,
                                  window->width - counts[side] + pageIndex - 2,
                                  -1, 0);
      }
      Window_SetTilemapEntryFar((s32)window, 0xf128,
                                window->width - counts[side] - 3, -1, 0);
      Window_SetTilemapEntryFar((s32)window, 0xf129, window->width - 2, -1, 0);
      UiRender_MarkRow(render, window->y);
    }
    frame++;
    blink = frame % 60 - 5;
    CLAMP(blink, 0, 29);
    MenuIcon_SetPosition(0, 32, 200);
    // DEAD: the test cannot change the result, redraw = 0 alone does the same;
    // the ROM keeps the compare (its twin also steps a phase under it).
    if (redraw)
      redraw = 0;
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropBorderTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
    }
    if (pressedKeys & KEY_START) {
      Sound_PlayFar(113);
      result = -2;
      break;
    }
    if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (repeatedKeys & KEY_LEFT) {
      (*selectedPage)--;
      *selectedPage = Menu_GetModuloOfSum(*selectedPage, *pageCount);
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & KEY_RIGHT) {
      (*selectedPage)++;
      *selectedPage = Menu_GetModuloOfSum(*selectedPage, *pageCount);
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
    wait(1);
  }
  {
    // A local holding one constant: as a literal argument, 2 of 436
    // instructions differ in tla-en.
    s32 priority = TASK_PASS_VBLANK;
    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  Window_CloseIfOpen(helpWindowSlot, 1);
  wait(1);
  Window_UpdateOrCreate(helpWindowSlot, 13, 0, 17, 5, 2);
  Window_CloseIfOpen(leftWindowSlot, 1);
  Window_CloseIfOpen(rightWindowSlot, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  WindowSprite_RedrawSavedRectFar(menu->unopenedWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  wait(1);
  return result;
}
