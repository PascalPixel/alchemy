#include "shop.h"

void Func_08015080(s32, s32, s32, s32);
s32 Func_080150b0(s32, s32, s32, s32, s32);
void Shop_DrawMoney(void)
{
    struct ShopRuntime *shop;
    s32 window;

    shop = SHOP_RUNTIME;
    window = shop->money_window;
    if (window != 0) {
        Func_08015080(0xc8a, window, 0, 0);
        Func_080150b0(SHOP_PARTY_STATE.money, 6, window, 0x20, 8);
    }
}
