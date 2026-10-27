// Near miss, differ 36 of 436 (2026-10-26; 93 before). The twin's result = 0
// before frame, with selectedPages cleared from it (80); the page row marked
// through UiRender_MarkRow with the signed row (54); the help window's
// address kept in its own slot pointer beside the left and right ones, and
// pageCount declared before counts (36): the frame's spill slots now follow
// the native's for the declared pointers. Left: the heap slot's load before
// sub sp in the prologue, and the three loop-invariant addresses (the pair
// owner's, the selected page's and the page count's) spilled in another order
// (the native keeps the page count's lowest, at sp+20, as if it were made
// last, by loop.c rather than a declaration).
// First draft of Func_08104540: the Djinn menu's view of the shown member's
// current status, the twin of ☀️'s DjinnMenu_ShowOwnerStatus
// (games/tbs/src/menu/menu/djinn_preview.c), without its blink phase: both
// panes of Func_08103218 for the pair's first member, page arrows over the
// right one, left and right page through, Start leaves the menu (-2) and A,
// B, L or R the view (-1). MsgCurrentStatus takes the twin's name (04315 in
// the English catalog, still unnamed there). Differ 95 of 436, register
// and stack slot choice mostly: the native gives r11 the selected page's
// address and keeps the frame's eleven spills in another order (the page
// count's address lowest at sp+20, the redraw flag highest at sp+60). The
// count's address taken at the top of the loop matters (232 with
// pageCounts[side] at each use).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "window.h"
#include "iwram_call.h"
#include "owner_state.h"
#include "macros.h"

struct DjinnMenuWork {
  u8 unk000[0x10];
  struct UiWindow *statusWindow;
  struct RenderOutput *cursor;
  u8 unk018[0x28 - 0x18];
  s32 window;
  s32 unknownWindow02c;
  u8 unk030[0x34 - 0x30];
  struct UiWindow *listWindow;
  s32 secondWindow;
  u8 unk03c[0xf0 - 0x3c];
  s32 helpWindow;
  u8 unk0f4[0x128 - 0xf4];
  u16 ownerY[4];
  u8 unk130[0x216 - 0x130];
  u8 shownOwner[2];
  u8 unk218[0x25c - 0x218];
  u8 pairOwner[2];
};

extern char MsgCurrentStatus[];
extern u8 gMenu_BackdropFrameTile[];

void RenderOutput_ClearListFar(void *window);
void RenderOutput_PrepareForRedrawFar(s32 window);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 width, s32 height,
                            s32 style);
void UiWindow_CloseIfOpen(void *window, s32 mode);
void UiWindow_SetTilemapEntryFar(struct UiWindow *window, s32 tile, s32 x,
                                 s32 y, s32 mode);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void MemFree(void *block);
s32 Func_08101860(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                  u16 *out, s32 *firstCount, s32 *secondCount);
void Func_08103218(s32 window, s32 x, s32 y, s32 owner, s32 a, s32 b, s32 c,
                   s32 param, s32 d);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
void MenuIcon_SetPosition(s32 index, s32 x, s32 y);
void Input_ResetKeyRepeat(void);
void Audio_PlayCue(s32 cue);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Menu_UpdateEntryObjectTransforms(void);
void WaitTicks(s32 ticks);

s32 Func_08104540(void) {
  s32 selectedPages[2];
  s32 pageCounts[2];
  s32 gained;
  s32 lost;
  s32 redraw;
  s32 frame;
  s32 *leftWindowSlot;
  s32 *rightWindowSlot;
  s32 *helpWindowSlot;
  struct UiRenderWork *render;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuWork *menu;
  struct OwnerActionState *unit;
  struct UiWindow *window;
  u16 *listedActions;
  void *saved;
  s32 result;
  s32 tile;
  s32 pageIndex;
  s32 side;
  s32 *pageCount;
  s32 *counts;
  s32 blink;

  menu = Ram_HeapSlots->menuRuntime;
  redraw = 1;
  result = 0;
  frame = 0;
  menu->cursor->active = 13;
  selectedPages[1] = selectedPages[0] = result;
  for (side = 3; side >= 0; side--)
    menu->ownerY[side] = 200;
  RenderOutput_ClearListFar(menu->listWindow);
  WaitTicks(1);
  counts = pageCounts;
  counts[1] = counts[0] = 1;
  listedActions = Runtime_BumpAllocateAlternatePool(96);
  saved = Runtime_BumpAllocateAlternatePool(332);
  unit = Unit_Get(menu->shownOwner[0]);
  counts[1] = counts[0] = Func_08101860(unit->actionSlots, unit->actionSlots,
                                        listedActions, &gained, &lost);
  MemFree(saved);
  MemFree(listedActions);
  counts[0] = (counts[0] - 1) / 6 + 1;
  if (counts[0] == 0)
    counts[0] = 1;
  counts[1] = (counts[1] - 1) / 6 + 1;
  if (counts[1] == 0)
    counts[1] = 1;
  leftWindowSlot = &menu->window;
  UiWindow_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, 2);
  rightWindowSlot = &menu->secondWindow;
  UiWindow_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, 2);
  helpWindowSlot = &menu->helpWindow;
  RenderOutput_RedrawSavedRectFar(*helpWindowSlot);
  RenderOutput_RedrawSavedRectFar((s32)menu->statusWindow);
  UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, menu->statusWindow, 0,
                                  0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, menu->statusWindow,
                                  0, 16);
  side = 0;
  while (1) {
    render = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    pressedKeys = gInput.pressed;
    repeatedKeys = gInput.repeat;
    pageCount = &pageCounts[side];
    if (redraw) {
      render->menuBusy = 1;
      RenderOutput_PrepareForRedrawFar(menu->window);
      RenderOutput_PrepareForRedrawFar(menu->secondWindow);
      Func_08103218(menu->window, 0, 0, menu->pairOwner[0], 0, 0, 3, 0, 1);
      Func_08103218(menu->secondWindow, 0, 0, menu->pairOwner[0], 0, 0, 3,
                    selectedPages[0] + 1, 1);
      render->menuBusy = 0;
    }
    if (counts[side] > 1) {
      window = (struct UiWindow *)menu->secondWindow;
      for (pageIndex = 0; pageIndex < counts[side]; pageIndex++) {
        tile = 0xf031 + pageIndex;
        if (pageIndex > 8)
          tile = 0xf03f;
        if (pageIndex == selectedPages[side])
          tile -= 0x1000;
        UiWindow_SetTilemapEntryFar(
            window, tile, window->width - counts[side] + pageIndex - 2, -1, 0);
      }
      UiWindow_SetTilemapEntryFar(window, 0xf128,
                                  window->width - counts[side] - 3, -1, 0);
      UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
      UiRender_MarkRow(render, (s16)window->y);
    }
    frame++;
    blink = frame % 60 - 5;
    CLAMP(blink, 0, 29);
    MenuIcon_SetPosition(0, 32, 200);
    if (redraw)
      redraw = 0;
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropFrameTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
    }
    if (pressedKeys & 8) {
      Audio_PlayCue(113);
      result = -2;
      break;
    }
    if (pressedKeys & 0x303) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    if (repeatedKeys & 0x20) {
      selectedPages[side]--;
      selectedPages[side] =
          Menu_GetModuloOfSum(selectedPages[side], *pageCount);
      Audio_PlayCue(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & 0x10) {
      selectedPages[side]++;
      selectedPages[side] =
          Menu_GetModuloOfSum(selectedPages[side], *pageCount);
      Audio_PlayCue(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
    WaitTicks(1);
  }
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0x480);
  ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  UiWindow_CloseIfOpen(helpWindowSlot, 1);
  WaitTicks(1);
  UiWindow_UpdateOrCreate(helpWindowSlot, 13, 0, 17, 5, 2);
  UiWindow_CloseIfOpen(leftWindowSlot, 1);
  UiWindow_CloseIfOpen(rightWindowSlot, 1);
  RenderOutput_RedrawSavedRectFar((s32)menu->listWindow);
  RenderOutput_RedrawSavedRectFar(menu->unknownWindow02c);
  RenderOutput_RedrawSavedRectFar((s32)menu->statusWindow);
  ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  WaitTicks(1);
  return result;
}
