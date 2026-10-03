#include "TYPES.H"
#include "IO_REG.H"

extern const u8 Menu_HexDigitsString[];
extern const u8 Menu_ColonString[];
void RenderOutput_PrepareForRedraw(void);
void UiText_DrawStringInWindow(s32 text, s32 window, s32 x, s32 y);
void Text_FormatHex(s32 value, s32 width, s32 buf);
s32 GameFlag_TestFar(s32 flag);

extern volatile u32 gKeysRepeat;
extern volatile u32 Data_03001c94;
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);

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
            s32 val = GameFlag_TestFar(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        UiText_DrawStringInWindow((s32)bits, window, 48, y);
    }
}

/* Flag grid input (debug menu): the cursor picks a flag on a 16x16 page (column,
   row); A toggles it, B or Select leaves, the D-pad moves the cursor and L/R
   turn pages, ten at a time with Start held. Returns 1 when the page or a
   flag changed. */
s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
    s32 *row = &cursor[1];

    if (gKeysRepeat & KEY_A) {
        s32 flag = ((*page << 4) + *row << 4) + cursor[0];

        if (GameFlag_TestFar(flag))
            GameFlag_ClearBitFar(flag);
        else
            GameFlag_SetBitFar(flag);
        return 1;
    }
    if ((Data_03001c94 & 2) || (gKeysRepeat & KEY_SELECT))
        return -1;
    if (gKeysRepeat & KEY_UP) {
        if (--*row < 0)
            *row = 15;
    } else if (gKeysRepeat & KEY_DOWN) {
        if (++*row > 15)
            *row = 0;
    } else if (gKeysRepeat & KEY_LEFT) {
        if (--cursor[0] < 0)
            cursor[0] = 15;
    } else if (gKeysRepeat & KEY_RIGHT) {
        if (++cursor[0] > 15)
            cursor[0] = 0;
    } else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START)) {
        *page -= 10;
        if (*page < 0)
            *page = 15;
        return 1;
    } else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START)) {
        *page += 10;
        if (*page > 15)
            *page = 0;
        return 1;
    } else if (gKeysRepeat & KEY_L) {
        if (--*page < 0)
            *page = 15;
        return 1;
    } else if (gKeysRepeat & KEY_R) {
        if (++*page > 15)
            *page = 0;
        return 1;
    }
    return 0;
}

void Menu_ReservedNoOp(void)
{
}
