// Draft, differ 4 of 141: draws the sanctum's party page, four members at a
// time with page arrows: ☀️'s Shop_DrawParty (games/tbs/src/shop/shop.c)
// shape over a page, each member dimmed when Shop_CanServe refuses the chosen
// service. Left: instruction order only, two rows before the loop (the
// native sets the counter's r10 before the page index reload from the stack;
// here mov sl and the ldr each sit one slot later).
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopRuntime {
  u8 unk000[0xf8];
  void *partyMemberIcons[4];
  u8 unk108[0x30];
  s32 partyMemberScale[4];
  u8 unk148[0x382];
  s16 partyMemberIds[18];
  u16 previousPageIcon;
  u16 nextPageIcon;
  u8 unk4f2[0x12];
  s8 partyMemberCount;
  u8 unk505[2];
  s8 partyAction;
};

void RenderOutput_PrepareForRedrawFar(void *window);
void Animation_ApplyChildArgumentFar(void *group, s32 argument);
s32 Shop_CanServe(s32 unitId, s32 serviceKind);

void Shop_DrawServiceParty(void *window, s32 selected) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 serviceKind = shop->partyAction;
  s32 first = selected / 4 * 4;
  s32 count = shop->partyMemberCount;
  struct RenderOutput *icon;
  s32 i;

  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (first != 0) {
      icon = RenderOutput_CreateFar(shop->previousPageIcon, 0x40000000, window,
                                    88, -12);
      icon->kind = 0;
      icon->active = 17;
      icon->animStep = 0;
    }
    if (first + 4 < count) {
      icon = RenderOutput_CreateFar(shop->nextPageIcon, 0x40000000, window, 88,
                                    0);
      icon->kind = 0;
      icon->active = 15;
      icon->animStep = 0;
    }
    for (i = 0; i < 4; i++) {
      s32 unitId = shop->partyMemberIds[first + i];

      if (shop->partyMemberIcons[i] != 0) {
        if (first + i == selected)
          Animation_ApplyChildArgumentFar(shop->partyMemberIcons[i], 30);
        else
          Animation_ApplyChildArgumentFar(shop->partyMemberIcons[i], 1);
        shop->partyMemberScale[i] = 0x10000;
        if (!Shop_CanServe(unitId, serviceKind))
          shop->partyMemberScale[i] = 0xb333;
      }
    }
  }
}
