#include "types.h"
#include "edition.h"

// The price line reads "Price", the price, then "Coins". Japanese indents
// the line and its notes by 8 pixels, French draws the price 8 pixels nearer
// its label, Spanish and Italian the coins label 8 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SHOP_PRICE_LABEL_X 8
#else
#define SHOP_PRICE_LABEL_X 0
#endif
#if LANGUAGE == LANGUAGE_FRENCH
#define SHOP_PRICE_NUMBER_X 24
#else
#define SHOP_PRICE_NUMBER_X 32
#endif
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_PRICE_COINS_X 80
#else
#define SHOP_PRICE_COINS_X 72
#endif

/// MsgItemCondition starts a list: "----------", "Processed", "Rusted".
extern u8 MsgCannotBuyThat[], MsgNotBroken[], MsgPrice[], MsgItemName[],
    MsgItemCondition[];
void WindowSprite_RedrawSavedRectFar(s32 window);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);

/// Draws the shop item's name and price, or its cannot-buy or not-broken note;
/// kind 3 draws the item condition message numbered by the price instead.
// The twin of ☀️'s Shop_DrawItemPrice (games/tbs/src/shop/shop.c), in the
// twin's goto layout, with kind 3 added. It matches with the switch on the
// kind as the twin has it (if-gotos give differ 46).
void Func_081091cc(s32 window, s32 itemId, s32 price, s32 priceKind) {
  s32 labelMessage;

  if (window == 0)
    return;
  goto draw;
cannot_buy:
  Text_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window,
                                  SHOP_PRICE_LABEL_X, 8);
  return;
not_broken:
  Text_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, SHOP_PRICE_LABEL_X,
                                  8);
  return;
show_price:
  labelMessage = (s32)MsgPrice;
  Text_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
  labelMessage -= 3;
  Text_DrawNumberInWindowFar(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
  Text_DrawCharacterAtOffsetFar(labelMessage, window, SHOP_PRICE_COINS_X, 8);
  return;
draw:
  WindowSprite_RedrawSavedRectFar(window);
  Text_DrawCharacterAtOffsetFar(itemId + (s32)MsgItemName, window, 0, 0);
  if (price == 0) {
    switch (priceKind) {
    case 1:
      goto cannot_buy;
    case 2:
      goto not_broken;
    }
  }
  if (priceKind == 3)
    Text_DrawCharacterAtOffsetFar(price + (s32)MsgItemCondition, window, 8,
                                    8);
  else
    goto show_price;
}
