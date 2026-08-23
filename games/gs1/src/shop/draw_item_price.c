#include "far_runtime.h"
#include "shop.h"

void Func_08015080(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);
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
    Func_08015080(0xc92, window, 0, 8);
    goto done;
show_price:
    message = 0xc8b;
    Func_08015080(message, window, 0, 8);
    message -= 3;
    Func_080150b0(price, 5, window, 0x20, 8);
    Func_08015080(message, window, 0x48, 8);
    goto done;
draw:
    Func_08015270(window);
    Func_08015080(
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
    Func_08015080(0xc93, window, 0, 8);
done:
    return;
}
