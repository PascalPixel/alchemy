#include "SHOP.H"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];

void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
s32 UiText_DrawNumberInWindowFar(s32, s32, s32, s32, s32);
void Shop_DrawMoney(void)
{
    struct ShopRuntime *shop;
    s32 window;

    shop = gMenuWork;
    window = shop->money_window;
    if (window != 0) {
        UiText_DrawCharacterAtOffsetFar(0xc8a, window, 0, 0);
        UiText_DrawNumberInWindowFar(SHOP_PARTY_STATE.money, 6, window, 0x20, 8);
    }
}
