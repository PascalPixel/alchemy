// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 6 of 184 in tla-en. First difference: the native issues `mov fp, r3` earlier
// (row 17) than this does (row 20). No single empty asm or one-pass block
// reaches 0.
// Near miss, differ 6 of 184 (2026-10-19): the twin of The Broken Seal's
// PsynergyMenu_SelectOwner. Written as while (1) the loop keeps the native's
// order (as for (;;), jump.c moves the B test and the cursor call above the
// loop's top: differ 43; with goto, 73). Left, in the setup: the native
// issues the moves of pending, the 0x204 offset's shift and the message
// window's base one or two slots earlier. The initializers in nine other
// orders and three minutes of permute are no nearer. MsgChooseCharacter is
// message 4406 in English; it needs its name in the six catalogs before this
// links (ja 4403, de 4870, es/fr/it 4866 by the text of The Broken Seal's
// MsgChooseCharacter; unverified against those ROMs).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

struct OwnerPageState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *ownerWindow;
  u8 unk014[8];
  s8 ownerIndex;
  u8 unk01d;
  s8 ownerCount;
  u8 unk01f[0xd1];
  void *messageWindow;
  u8 unk0f4[0xd0];
  u16 psynergies[32];
  u16 ownerIds[8];
  u8 psynergyCount;
  u8 unk215;
  u8 selectedIds[6];
  u16 page;
};

extern u8 MsgChooseCharacter[];

void *Unit_Get(s32 owner);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 page);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
s32 Func_080fd6f0(void *unit, u16 *list, s32 mode);
void Func_080fd6b0(u16 *list);
void Func_081005e4(s32 x, s32 y, s32 columns);
void Func_080facd8(u16 *list);
void Func_08104ef8(s32 first);
void Menu_HighlightPageEntry(void *window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

s32 Func_080feb58(void) {
  struct OwnerPageState *menu =
      (struct OwnerPageState *)Ram_HeapSlots->menuRuntime;
  s32 selection = menu->ownerIndex;
  s32 count = menu->ownerCount;
  s32 pending = 2;
  s32 page = menu->page;
  s32 result;

  Unit_Get(menu->ownerIds[selection]);
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter, menu->messageWindow,
                                  0, 0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter + 1,
                                  menu->messageWindow, 0, 16);
  while (1) {
    // PERM
    if (pending) {
      selection = (selection + count) % count;
      Unit_Get(menu->ownerIds[selection]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[selection]);
      menu->psynergyCount = Func_080fd6f0(Unit_Get(menu->ownerIds[selection]),
                                          menu->psynergies, 0);
      Func_080fd6b0(menu->psynergies);
      Func_081005e4(96, 96, 8);
      Func_080facd8(menu->psynergies);
      if (pending == 2) {
        Func_08104ef8(selection / 4 * 4);
        WaitFrames(1);
      }
      Menu_HighlightPageEntry(menu->ownerWindow, selection, count);
    }
    Func_080f8a44((selection % 4) * 24 - 10, 16);
    WaitFrames(1);
    if (((volatile struct InputState *)&gInput)->pressed & 1) {
      Audio_PlayCue(112);
      result = 1;
      break;
    }
    if (((volatile struct InputState *)&gInput)->pressed & 2) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    pending = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->ownerIndex = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->selectedIds[0] = menu->ownerIds[selection];
  return result;
}
