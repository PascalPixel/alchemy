#include "FAR_RUNTIME.H"
#include "SHOP.H"

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
extern u8 MsgItemName;

/* Draws an item's name in the window and, on the line below, "Price" with
 * the price and "Coins". With no price, mode 1 shows "We can't buy that."
 * and mode 2 "It's not broken." instead. */
void Shop_DrawItemPrice(s32 window, s32 item, s32 price, s32 price_mode)
{
    s32 message;

    if (window == 0)
        goto done;
    goto draw;
cannot_buy:
    UiText_DrawCharacterAtOffsetFar(0xc92, window, 0, 8);
    goto done;
show_price:
    message = 0xc8b;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
    message -= 3;
    UiText_DrawNumberInWindowFar(price, 5, window, 0x20, 8);
    UiText_DrawCharacterAtOffsetFar(message, window, 0x48, 8);
    goto done;
draw:
    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, 0, 0);
    if (price != 0)
        goto show_price;
    switch (price_mode) {
    case 1:
        goto cannot_buy;
    case 2:
        UiText_DrawCharacterAtOffsetFar(0xc93, window, 0, 8);
        break;
    default:
        goto show_price;
    }
done:
    return;
}
