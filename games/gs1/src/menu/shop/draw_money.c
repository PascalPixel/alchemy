#include "shop.h"

void UiText_DrawAt(s32, s32, s32, s32);
s32 UiNumber_DrawAt(s32, s32, s32, s32, s32);
void Shop_DrawMoney(void)
{
    struct ShopRuntime *shop;
    s32 window;

    shop = SHOP_RUNTIME;
    window = shop->money_window;
    if (window != 0) {
        UiText_DrawAt(0xc8a, window, 0, 0);
        UiNumber_DrawAt(SHOP_PARTY_STATE.money, 6, window, 0x20, 8);
    }
}
