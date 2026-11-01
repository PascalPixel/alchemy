// Draft, differ 2 of 365, skeleton gap 1 (tla-en, 2026-11-01)
// Func_080feec0: the status page of the status menu, the twin of The Broken
// Seal's StatusScreen_SelectCommand (games/tbs/src/menu/menu/status.c): pane 0
// walks the member's ailments, pane 1 the Djinn and element rows; A returns 1,
// B -1, L and R switch members. Same locals, loops, key tests and calls; this
// game has no party-figure task or cursor icon step, and draws the help before
// choosing the pane. The help text is MsgSwitchCharacterHelp (message 4149 in
// English) and the message after it; the name needs to be in the six catalogs
// before this links.
// What is left is one swapped pair before the member switch's
// PsynergyMenu_CallIconRoutineWithValue: the native loads menu (ldr r0, [sp])
// before the owner (ldrh r1), this the other way round. Tried without moving
// it: parameter types u8, u16, u32, void * and const void *; the owner in a
// block temporary (u16, s32) or cast; menu cast; member declared last. The
// owner through member, through an ids pointer, or selectedIds[0] from
// selectedOwner, are all much worse. Scheduling, so a near miss.
// The setup order came from the reload registers' rotation: pane, selected,
// result, window, then helpWindow after the call and selected's zero before
// it (the search over these orders found no better); redraw = 0 before
// limit = count is the twin's own order.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct CharacterMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[0x10];
  s8 ownerIndex;
  u8 unk01d[0x0b];
  s32 statusWindow;
  u8 unk02c[4];
  s32 helpWindow;
  u8 unk034[0x1d0];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 selectedIds[2];
  u8 unk218[0x20];
  s16 rowY[4];
};

extern u8 MsgSwitchCharacterHelp[];

s32 Window_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
s32 Party_SumDjinnCountsFar(s32 owner);
void Func_080f8ab4(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void *Unit_Get(s32 owner);
void Func_080ff370(s32 window, s32 ownerId, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId);
void WindowSprite_RedrawSavedRectFar(s32 window);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert);
void WindowSprite_ClearListFar(s32 window);
void wait(s32 frames);
void CharacterMenu_DrawSelectionLabels(s32 window, s32 selected, u8 *entries);
void Func_080ff2e8(s32 window, s32 selected, s32 hasDjinn);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct CharacterMenuState *menu,
                                           s32 ownerId);
void WindowSprite_PrepareForRedrawFar(s32 window);

s32 Func_080feec0(void) {
  struct CharacterMenuState *menu;
  s32 pane;
  s32 result;
  s32 selected;
  s8 count;
  s32 hasAilments;
  s32 done;
  s32 hasDjinn;
  s32 redraw;
  s32 member;
  s32 window;
  s32 helpWindow;
  s32 i;
  u8 entries[8];

  menu = Ram_HeapSlots->menuRuntime;
  pane = 0;
  selected = 0;
  result = 0;
  window = menu->statusWindow;
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  helpWindow = menu->helpWindow;
  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  Func_080f8ab4(-10, 88);

  while (done == 0 && !GameFlag_Test(0x150)) {
    Unit_Get(menu->selectedIds[0]);
    Func_080ff370(window, menu->selectedIds[0], 1);
    count = CharacterMenu_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (!GameFlag_Test(0x150)) {
      if (redraw) {
        s32 limit;

        redraw = 0;
        limit = count;
        pane = (pane + 2) % 2;
        WindowSprite_RedrawSavedRectFar(helpWindow);
        if (pane == 0) {
          selected = (selected + limit) % limit;
          if (hasAilments == 0) {
            Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 1, window,
                                            0, -24);
            Text_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 80,
                                            -24);
          }
        } else {
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
        WindowSprite_ClearListFar(helpWindow);
        wait(1);
        if (pane == 0)
          CharacterMenu_DrawSelectionLabels(helpWindow, selected, entries);
        else
          Func_080ff2e8(helpWindow, selected, hasDjinn);
      }
      if (pane == 0)
        Func_080f8a44(-10, selected * 16 + 88);
      else if (selected <= 3)
        Func_080f8a44(24, selected * 8 + 48);
      else
        Func_080f8a44(48, selected * 8 + 80);
      wait(1);
      if (gInput.repeat & 0xf0)
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
      if (KEYS_PRESSED & 1) {
        Sound_PlayFar(112);
        done = 1;
        result = 1;
        break;
      }
      if (KEYS_PRESSED & 2) {
        Sound_PlayFar(113);
        done = 1;
        result = -1;
        break;
      }
      if (gInput.repeat & 0x40) {
        Sound_PlayFar(111);
        redraw = 1;
        selected--;
      }
      if (gInput.repeat & 0x80) {
        Sound_PlayFar(111);
        redraw = 1;
        selected++;
      }
      if (gInput.repeat & 0x10) {
        Sound_PlayFar(111);
        redraw = 1;
        pane++;
      }
      if (gInput.repeat & 0x20) {
        Sound_PlayFar(111);
        redraw = 1;
        pane--;
      }
      if ((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) {
        Sound_PlayFar(111);
        member = menu->ownerIndex;
        if (gInput.repeat & 0x100)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[member];
        menu->selectedIds[0] = menu->ownerIds[member];
        menu->ownerIndex = member;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[member]);
        break;
      }
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(window);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = -16;
  return result;
}
