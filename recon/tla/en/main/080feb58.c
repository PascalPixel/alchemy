// Near miss, differ 43 of 184: the twin of The Broken Seal's
// PsynergyMenu_SelectOwner. The native keeps the loop in source order (the page
// block, the A test, the B test, the cursor call, b to the top); here jump.c
// moves the B test and the cursor call above the loop's top and enters with a
// jump. Written with goto the order is right but loop.c no longer hoists the
// 0x204 offset and the list address (differ 73). The native also copies the
// hoisted offset (mov r6, r8) where this reuses r8. MsgChooseCharacter is
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

void *UnitGet(s32 owner);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Func_080fee40(s32 owner, s32 page);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
s32 Func_080fd6f0(void *unit, u16 *list, s32 mode);
void Func_080fd6b0(u16 *list);
void Func_081005e4(s32 x, s32 y, s32 columns);
void Func_080facd8(u16 *list);
void Func_08104ef8(s32 first);
void Func_08104d5c(void *window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
s32 Func_08104c00(s32 *selection, s32 count, s32 columns);
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

  UnitGet(menu->ownerIds[selection]);
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter, menu->messageWindow,
                                  0, 0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter + 1,
                                  menu->messageWindow, 0, 16);
  for (;;) {
    // PERM
    if (pending) {
      selection = (selection + count) % count;
      UnitGet(menu->ownerIds[selection]);
      page = (page + 3) % 3;
      Func_080fee40(menu->ownerIds[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[selection]);
      menu->psynergyCount = Func_080fd6f0(UnitGet(menu->ownerIds[selection]),
                                          menu->psynergies, 0);
      Func_080fd6b0(menu->psynergies);
      Func_081005e4(96, 96, 8);
      Func_080facd8(menu->psynergies);
      if (pending == 2) {
        Func_08104ef8(selection / 4 * 4);
        WaitFrames(1);
      }
      Func_08104d5c(menu->ownerWindow, selection, count);
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
    pending = Func_08104c00(&selection, count, 4);
  }
  menu->ownerIndex = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->selectedIds[0] = menu->ownerIds[selection];
  return result;
}
