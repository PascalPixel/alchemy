#include "FAR_RUNTIME.H"
#include "SHOP.H"

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
extern u8 Value_00000182;

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
    UiText_DrawAt(0xc92, window, 0, 8);
    goto done;
show_price:
    message = 0xc8b;
    UiText_DrawAt(message, window, 0, 8);
    message -= 3;
    UiNumber_DrawAt(price, 5, window, 0x20, 8);
    UiText_DrawAt(message, window, 0x48, 8);
    goto done;
draw:
    UiWindow_Commit(window);
    UiText_DrawAt(item + (s32)&Value_00000182, window, 0, 0);
    if (price != 0)
        goto show_price;
    switch (price_mode) {
    case 1:
        goto cannot_buy;
    case 2:
        UiText_DrawAt(0xc93, window, 0, 8);
        break;
    default:
        goto show_price;
    }
done:
    return;
}
