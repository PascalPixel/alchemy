#include "EDITION.H"
#include "FAR_RUNTIME.H"
#include "SHOP.H"
#include "UI.H"
#include "text/MSG_IDS.H"

void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);

void Shop_DrawItemPrice(s32 window, s32 item, s32 price, s32 price_mode)
{
    s32 message;

    if (window == 0)
        goto done;
    goto draw;
cannot_buy:
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window, 0, 8);
#else
    UiText_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window, 8, 8);
#endif
    goto done;
show_price:
    message = (s32)MsgPrice;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 8);
#else
    UiText_DrawCharacterAtOffsetFar(message, window, 8, 8);
#endif
    message -= 3;
#if defined(TBS_EDITION_FR)
    UiText_DrawNumberInWindowFar(price, 5, window, 24, 8);
    UiText_DrawCharacterAtOffsetFar(message, window, 64, 8);
#elif defined(TBS_EDITION_ES)
    UiText_DrawNumberInWindowFar(price, 5, window, 32, 8);
    UiText_DrawCharacterAtOffsetFar(message, window, 80, 8);
#else
    UiText_DrawNumberInWindowFar(price, 5, window, 32, 8);
    UiText_DrawCharacterAtOffsetFar(message, window, 72, 8);
#endif
    goto done;
draw:
    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, 0, 0);
    if (price != 0)
        goto show_price;
    switch (price_mode) {
    case 1:
        goto cannot_buy;
    case 2:
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, 0, 8);
#else
        UiText_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, 8, 8);
#endif
        break;
    default:
        goto show_price;
    }
done:
    return;
}
