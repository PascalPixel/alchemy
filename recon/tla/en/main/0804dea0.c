// Under -mthumb-slow-loads (2026-10-26): differ 8 of 87 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
#include "types.h"
extern const u8 gMenu_HexDigitsString[];

extern const u8 gMenu_ColonString[];

void WindowSprite_PrepareForRedraw(void);
void Text_DrawStringInWindow(s32 text, s32 window, s32 x, s32 y);
void Text_FormatHex(s32 value, s32 width, s32 buf);
s32 GameFlag_Test(s32 flag);

void Menu_DrawFlagBitTable(s32 window, s32 startFlag) {
  s32 row;
  s32 y;
  s32 flag;
  char label[5];
  char bits[17];

  WindowSprite_PrepareForRedraw();
  Text_DrawStringInWindow((s32)gMenu_HexDigitsString, window, 48, 0);

  flag = startFlag << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, (s32)label);
    Text_DrawStringInWindow((s32)label, window, 0, y);
    Text_DrawStringInWindow((s32)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 val = GameFlag_Test(flag);
      bits[i] = (val != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    Text_DrawStringInWindow((s32)bits, window, 48, y);
  }
}
