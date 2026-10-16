#include "types.h"
#include "ram_buffer.h"

struct PsynergyMenuState {
  u8 unk000[0x24];
  s32 psynergyWindow;
  u8 unk028[0x19c];
  u16 psynergies[40];
  u8 psynergyCount;
};

extern char MsgPsynergyMenuEmpty[];

void *UnitGet(s32 owner);
s32 Func_080fd6f0(void *owner, u16 *out, s32 mode);
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies);
void RenderOutput_RedrawSavedRectFar(s32 window);
void PsynergyMenu_RefreshOwnerEntries(s32 x, s32 y, s32 count);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// Collect the owner's Psynergy and redraw the list; indicate when it is empty.
void PsynergyMenu_RefreshOwnerPsynergy(s32 ownerId) {
  u16 *psynergies;
  struct PsynergyMenuState *menu;
  void *owner;

  menu = Ram_HeapSlots->menuRuntime;
  owner = UnitGet(ownerId);
  psynergies = menu->psynergies;
  menu->psynergyCount = Func_080fd6f0(owner, psynergies, 2);
  RenderOutput_RedrawSavedRectFar(menu->psynergyWindow);
  PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyMenuEmpty,
                                    menu->psynergyWindow, 0, 0x18);
  }
}
