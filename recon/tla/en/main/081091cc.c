// First draft of Func_081091cc: draws the shop item's name and price, or its
// cannot-buy or not-broken note; the twin of ☀️'s Shop_DrawItemPrice
// (games/tbs/src/shop/shop.c), in the twin's goto layout, with a fourth kind:
// kind 3 draws message 0x1249 + price instead. The messages take the twin's
// names; MsgPriceKind3 is a placeholder. Differ 0 of 65: matches with the
// switch on the kind as the twin has it (if-gotos give 46).
#include "types.h"

extern u8 MsgCannotBuyThat[], MsgNotBroken[], MsgPrice[], MsgItemName[],
    MsgPriceKind3[];

void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);

void Func_081091cc(s32 window, s32 itemId, s32 price, s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  UiText_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window, 0, 8);
  return;
not_broken:
  UiText_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, 0, 8);
  return;
show_price:
  labelMessage = (s32)MsgPrice;
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, 0, 8);
  labelMessage -= 3;
  UiText_DrawNumberInWindowFar(price, 5, window, 32, 8);
  UiText_DrawCharacterAtOffsetFar(labelMessage, window, 72, 8);
  return;
draw:
  RenderOutput_RedrawSavedRectFar(window);
  UiText_DrawCharacterAtOffsetFar(itemId + (s32)MsgItemName, window, 0, 0);
  if (price == 0) {
    switch (priceKind) {
    case 1:
      goto cannot_buy;
    case 2:
      goto not_broken;
    }
  }
  if (priceKind == 3)
    UiText_DrawCharacterAtOffsetFar(price + (s32)MsgPriceKind3, window, 8, 8);
  else
    goto show_price;
}
