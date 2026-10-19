#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

/// The sanctum's view of the shop work (heap slot 220).
struct ShopRuntime {
  u8 unk000[0x507];
  s8 partyAction;
};

s32 Shop_ServicePrice(s32 unitId, s32 serviceKind);
s32 Shop_CanServe(s32 unitId, s32 serviceKind);
s32 Shop_MsgByMode(s32 message);
void RenderOutput_PrepareForRedrawFar(s32 window);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);

/// Shows the selected member's service price, or that no treatment is needed.
// As ☀️'s Shop_DrawSelectMsg. The service kind's variable takes the message
// once the kind is no longer needed, as the native's register does.
void Shop_DrawSelectMsg(s32 window, s32 unitId) {
  s32 kindOrMessage;
  s32 price;
  struct ShopRuntime *shop;

  shop = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(barrier): the work loaded before the service kind's offset is
   * built in r2, as native; the clobber keeps that build below the load. As
   * plain C, 2 of 45 instructions differ in tla-en. */
  asm("" : "+l"(shop) : : "r2");
  kindOrMessage = shop->partyAction;
  price = Shop_ServicePrice(unitId, kindOrMessage);
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    if (Shop_CanServe(unitId, kindOrMessage) != 0)
      kindOrMessage = (s32)MsgReviveCost;
    else
      kindOrMessage = (s32)MsgNoHealingNeeded;
    kindOrMessage = Shop_MsgByMode(kindOrMessage);
    UiText_DrawQuantity(price, 5);
    UiText_DrawResourceFar(kindOrMessage, window, 0, 0);
  }
}
