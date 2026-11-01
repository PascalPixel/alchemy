// Draft, differ 8 of 87, skeleton gap 4 (tla-en, 2026-11-01). Twin of
// DebugFlagEditor_DrawTable: the same calls, strings, loops and locals, so its
// C already is this draft. Pointer prototypes, s8 buffers, flag in the for
// init or before the first draw, row last among the locals, (row + 2) * 8 and
// no val local leave differ at 8; a pointer clear, y += 8, a while loop,
// row < 16, i < 5 and bits[16] = 0 are all worse.
// Instruction order only, in
// the loop setup after the second draw. sched2 ties row = 0 (r1, priority 3,
// two dependents) with the copy of bits into r8 (priority 3, one dependent) and
// takes row = 0 first; the listing takes the bits copy and the label end
// (sp + 33) first, which needs a dependence or cost the C does not create. No
// reshaping moves it (y as its own variable, do-while, i at function scope,
// flag set before the calls, a pointer clear, i != 16, u8 buffers), nor a
// 4-minute permuter run (12,000 candidates). No empty asm before any one
// statement lowers it.
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
