#include "EDITION.H"
#include "SHOP.H"
#include "PARTY_STATE.H"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 MsgYourCoins[];

void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
s32 UiText_DrawNumberInWindowFar(s32, s32, s32, s32, s32);
void Shop_DrawMoney(void)
{
    struct ShopRuntime *shop;
    s32 window;

    shop = gMenuWork;
    window = shop->money_window;
    if (window != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((s32)MsgYourCoins, window, 0, 0);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0x20, 8);
#else
        /* The Japanese count comes first and its coins label after it. */
        s32 msg = (s32)MsgYourCoins;

        UiText_DrawCharacterAtOffsetFar(msg, window, 0, 0);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0, 8);
        /* The coins label, two messages before. */
        UiText_DrawCharacterAtOffsetFar(msg - 2, window, 48, 8);
#endif
    }
}
