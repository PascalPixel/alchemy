// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 134 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8). No
// single empty asm or one-pass block reaches 0.
// First draft of Func_0810928c: draws one page of four party figures in the
// shop: the page arrows, the selected member's figure animating (30, others
// 1) and members who cannot equip the candidate item dimmed to scale 0xcccc.
// The twin of ☀️'s Shop_DrawParty (games/tbs/src/shop/shop.c), which shows
// the whole party at once. Near miss, differ 8 of 134, instruction order
// only: the native spills itemId before it builds the work address (here
// after the first move), and moves the loop's zero to r8 and reloads page
// earlier in the loop setup.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopRuntime {
  u8 unk000[0xf8];
  void *partyMemberIcons[4];
  u8 unk108[0x30];
  s32 partyMemberScale[4];
  u8 unk148[0x382];
  s16 partyMemberIds[8];
  u8 unk4da[0x14];
  u16 previousPageIcon;
  u16 nextPageIcon;
  u8 unk4f2[0x12];
  s8 partyMemberCount;
};

void RenderOutput_PrepareForRedrawFar(struct RenderInput *window);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Item_IsCompatibleWithOwnerFar(s32 owner, s32 item);

void Func_0810928c(struct RenderInput *window, s32 selected, s32 itemId) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 page = selected / 4;
  s32 first = page * 4;
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
    for (i = 0; i <= 3; i++) {
      s32 owner = shop->partyMemberIds[first + i];
      void *figure = shop->partyMemberIcons[i];

      if (figure != NULL) {
        if (first + i == selected)
          Animation_ApplyChildArgumentFar(figure, 30);
        else
          Animation_ApplyChildArgumentFar(figure, 1);
        shop->partyMemberScale[i] = 0x10000;
        if (!Item_IsCompatibleWithOwnerFar(owner, itemId))
          shop->partyMemberScale[i] = 0xcccc;
      }
    }
  }
}
