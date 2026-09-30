/* Near miss: score 120: two reordered instructions, adds r2, #33 and str
   r2, [sp, #0], around the bit characters' store. */
#include "TYPES.H"
extern const u8 Menu_HexDigitsString[];

extern const u8 Menu_ColonString[];

void RenderOutput_PrepareForRedraw(void);
void UiText_DrawStringInWindow(s32 text, s32 window, s32 x, s32 y);
void Text_FormatHex(s32 value, s32 width, s32 buf);
s32 GameFlag_Test(s32 flag);

void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
    s32 row;
    s32 y;
    s32 flag;
    char label[5];
    char bits[17];

    RenderOutput_PrepareForRedraw();
    UiText_DrawStringInWindow((s32)Menu_HexDigitsString, window, 48, 0);

    flag = start_flag << 8;
    for (row = 0; row != 16; row++) {
        s32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++) {
            label[i] = 0;
        }
        Text_FormatHex(flag, 3, (s32)label);
        UiText_DrawStringInWindow((s32)label, window, 0, y);
        UiText_DrawStringInWindow((s32)Menu_ColonString, window, 32, y);

        for (i = 0; i < 16; i++) {
            s32 val = GameFlag_Test(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        UiText_DrawStringInWindow((s32)bits, window, 48, y);
    }
}
