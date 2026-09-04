#include "far_runtime.h"
#include "shop.h"

void UiText_DrawAt(s32, s32, s32, s32);
void UiNumber_DrawAt(s32, s32, s32, s32, s32);
extern u8 Value_00000182;

/* 引数の組合せに応じて固定番号の処理を選ぶ。 */
void Shop_DrawItemPrice(
    s32 window,
    s32 item_name_message,
    s32 price,
    s32 price_mode)
{
    s32 message;

    while (window != 0)
        goto draw;
    goto done;
show_mode_one:
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
    UiText_DrawAt(
        item_name_message + (s32)&Value_00000182,
        window,
        0,
        0);
    if (price != 0)
        goto show_price;
    if (price_mode == 1)
        goto show_mode_one;
    if (price_mode != 2)
        goto show_price;
    UiText_DrawAt(0xc93, window, 0, 8);
done:
    return;
}
