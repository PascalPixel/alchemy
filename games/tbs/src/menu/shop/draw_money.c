#include "edition.h"
#include "shop.h"
#include "party_state.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "graphics.h"
extern struct ShopRuntime *gMenuWork;

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
        s32 msg = (s32)MsgYourCoins;

        UiText_DrawCharacterAtOffsetFar(msg, window, 0, 0);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0, 8);
        UiText_DrawCharacterAtOffsetFar(msg - 2, window, 48, 8);
#endif
    }
}
