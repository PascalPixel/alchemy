// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 22 of 365 in tla-en. First difference: this issues `movs r0, #0` earlier (row
// 13) than the native does (row 23).
// First draft of Func_080feec0: the status page of the status menu, the twin
// of The Broken Seal's CharacterMenu_SelectCommand
// (games/tbs/src/menu/menu/status.c): pane 0 walks the member's ailments,
// pane 1 the Djinn and element rows; A returns 1, B -1, L and R switch
// members. The help text is MsgSwitchCharacterHelp (message 4149 in English)
// and the message after it; the name needs to be in the six catalogs before
// this links. Differ 53 of 365 (2026-10-19): one instruction short, as the
// native takes the ailment list's address with movs and add (sp) where agscc
// adds sp directly; the setup's registers and the stack reloads of count
// around it differ, the rest is branch offsets.
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

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height,
                            s32 frame);
s32 Party_SumDjinnCountsFar(s32 owner);
void Func_080f8ab4(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void *Unit_Get(s32 owner);
void Func_080ff370(s32 window, s32 ownerId, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert);
void RenderOutput_ClearListFar(s32 window);
void WaitFrames(s32 frames);
void CharacterMenu_DrawSelectionLabels(s32 window, s32 selected, u8 *entries);
void Func_080ff2e8(s32 window, s32 selected, s32 hasDjinn);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
void PsynergyMenu_CallIconRoutineWithValue(struct CharacterMenuState *menu,
                                           s32 ownerId);
void RenderOutput_PrepareForRedrawFar(s32 window);

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
  result = 0;
  window = menu->statusWindow;
  pane = 0;
  UiWindow_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  helpWindow = menu->helpWindow;
  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  selected = 0;
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

        limit = count;
        pane = (pane + 2) % 2;
        redraw = 0;
        RenderOutput_RedrawSavedRectFar(helpWindow);
        if (pane == 0) {
          selected = (selected + limit) % limit;
          if (hasAilments == 0) {
            UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 1, window,
                                            0, -24);
            UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, window, 80,
                                            -24);
          }
        } else {
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
        RenderOutput_ClearListFar(helpWindow);
        WaitFrames(1);
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
      WaitFrames(1);
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
  RenderOutput_PrepareForRedrawFar(menu->helpWindow);
  RenderOutput_RedrawSavedRectFar(window);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = -16;
  return result;
}
