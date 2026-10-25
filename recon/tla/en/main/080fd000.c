// First draft of Func_080fd000: the party member row of the Psynergy menu,
// the twin of The Broken Seal's PsynergyMenu_SelectMember
// (games/tbs/src/menu/menu/psynergy.c). ⚓️ moves the selection through
// Menu_MoveGridCursor (which returns whether to redraw, 2 to scroll the row) and
// keeps it on the stack.
// Differ 12 of 298 (2026-10-19), instruction order only: the native copies
// ownerIds (mov r8, r0) before the count and the 0x26c offset, and loads
// gInput from the pool before it finishes the 0x26c offset. Four minutes of
// permute reached differ 8 only by wrapping the setup in a one-pass block.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct MenuIcon {
  u8 unk00[5];
  u8 active;
};

struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  s32 rowWindow;
  u8 unk014[8];
  s8 tabIndex;
  u8 unk01d;
  s8 tabCount;
  u8 unk01f[5];
  s32 psynergyWindow;
  s32 statusWindow;
  s32 shortcutWindow;
  s32 infoWindow;
  u8 unk034[0x144];
  struct MenuIcon *paneIcon[2];
  u8 unk180[0x94];
  u8 psynergyCount;
  u8 unk215;
  u8 ownerId;
  u8 unk217;
  struct MenuIcon *cursorIcon;
  u8 unk21c[0x50];
  u8 mode;
};

void *Unit_Get(s32 owner);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
void Menu_SpawnIconEntries(struct PsynergyMenuState *menu, s32 window);
struct MenuIcon *RenderOutput_CreateFromResourceFar(s32 kind, s32 id,
                                                    s32 window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void PsynergyMenu_RefreshOwnerPsynergy(s32 ownerId);
void Func_080f8170(s32 window, s32 ownerId, s32 x, s32 y);
void Func_080fd55c(s32 window, s32 ownerId);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyMenuState *menu,
                                           s32 ownerId);
void Func_08104ef8(s32 first);
void WaitFrames(s32 frames);
void Menu_HighlightPageEntry(s32 window, s32 selection, s32 count);
void RenderOutput_ClearListFar(s32 window);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawWorkValueWithLabel(s32 window);
void Func_080f8a44(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Func_080fd6f0(void *unit, u16 *actions, s32 mode);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);

s32 Func_080fd000(u16 *ownerIds) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  void *unit;
  s32 window;
  s32 shown;
  u16 *actions;
  s8 fieldCount;
  s32 shortcutCount;

  menu = Ram_HeapSlots->menuRuntime;
  selection = menu->tabIndex;
  count = menu->tabCount;
  redraw = 2;
  result = 0;
  shown = 0;
  menu->mode = result;
  unit = Unit_Get(ownerIds[selection]);
  if (UiWindow_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries(menu, menu->psynergyWindow);
  if (UiWindow_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, 2)) {
    menu->cursorIcon = RenderOutput_CreateFromResourceFar(
        2, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->active = 13;
  }
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      window = menu->statusWindow;
      unit = Unit_Get(ownerIds[selection]);
      PsynergyMenu_RefreshOwnerPsynergy(ownerIds[selection]);
      Func_080f8170(window, ownerIds[selection], 0, 0);
      Func_080fd55c(menu->shortcutWindow, ownerIds[selection]);
      PsynergyMenu_CallIconRoutineWithValue(menu, ownerIds[selection]);
      if (redraw == 2) {
        Func_08104ef8(selection / 4 * 4);
        WaitFrames(1);
      }
      Menu_HighlightPageEntry(menu->rowWindow, selection, count);
      if (!GameFlag_Test(0x151) && !shown) {
        RenderOutput_ClearListFar(menu->infoWindow);
        RenderOutput_RedrawSavedRectFar(menu->infoWindow);
        UiText_DrawWorkValueWithLabel(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_ClearBit(0x151);
      }
    }
    Func_080f8a44((selection % 4) * 24 - 10, 16);
    WaitFrames(1);
    if (KEYS_PRESSED & 1) {
      if (menu->psynergyCount) {
        Audio_PlayCue(112);
        result = ownerIds[selection];
        break;
      }
      Audio_PlayCue(114);
    }
    if ((KEYS_PRESSED & 0x200) || (KEYS_PRESSED & 0x100)) {
      result = ownerIds[selection];
      if (KEYS_PRESSED & 0x200)
        menu->mode = 1;
      else
        menu->mode = 2;
      actions = MemAlloc(32 * sizeof(u16));
      fieldCount = Func_080fd6f0(unit, actions, 1);
      MemFree(actions);
      shortcutCount = fieldCount;
      if (shortcutCount == 0) {
        menu->mode = shortcutCount;
        Audio_PlayCue(114);
      } else {
        Audio_PlayCue(112);
        break;
      }
    }
    if (KEYS_PRESSED & 2) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->tabIndex = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->ownerId = ownerIds[selection];
  menu->paneIcon[0]->active = 13;
  menu->paneIcon[1]->active = 13;
  return result;
}
