#include "types.h"
#include "graphics.h"
#include "ram_buffer.h"
#include "party_state.h"

struct ShopWork_08109188
{
    u8    unk00[12];
    void *coinWindow;
};

extern char MsgYourCoins[];

void RenderOutput_PrepareForRedrawFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

void Func_08109188(void)
{
    void *window = ((struct ShopWork_08109188 *)Ram_HeapSlots->menuRuntime)->coinWindow;

    if (window != NULL)
    {
        RenderOutput_PrepareForRedrawFar(window);
        UiText_DrawCharacterAtOffsetFar((s32)MsgYourCoins, window, 0, 0);
        UiText_DrawNumberInWindowFar(gPartyState.coins, 6, (s32)window, 32, 8);
    }
}
