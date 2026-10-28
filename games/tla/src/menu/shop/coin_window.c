#include "types.h"
#include "graphics.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "shop.h"

struct ShopWork_08109188 {
  u8 unk00[12];
  void *coinWindow;
};

extern char MsgYourCoins[];

void WindowSprite_PrepareForRedrawFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

void Func_08109188(void) {
  void *window =
      ((struct ShopWork_08109188 *)Ram_HeapSlots->menuRuntime)->coinWindow;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 message;
#endif

  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
#if LANGUAGE == LANGUAGE_JAPANESE
    // "Coins" (MsgCoins) after the count, two messages before MsgYourCoins.
    message = (s32)MsgYourCoins;
    Text_DrawCharacterAtOffsetFar(message, window, 0, 0);
    message -= 2;
    Text_DrawNumberInWindowFar(gAdventure.coins, 6, (s32)window, 0, 8);
    Text_DrawCharacterAtOffsetFar(message, window, 48, 8);
#else
    Text_DrawCharacterAtOffsetFar((s32)MsgYourCoins, window, 0, 0);
    Text_DrawNumberInWindowFar(gAdventure.coins, 6, (s32)window, 32, 8);
#endif
  }
}
