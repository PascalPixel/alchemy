#include "TYPES.H"

void Func_08016478(void);
#define RenderOutput_PrepareForRedraw Func_08016478
void Func_0801e940(s32 text, s32 window, s32 x, s32 y);
#define UiText_DrawStringInWindow Func_0801e940
void Func_08029274(s32 value, s32 width, s32 buf);
s32 Func_080770c0(s32 flag);
#define BattleFlag_Test Func_080770c0

void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
    s32 row;
    s32 y;
    s32 flag;
    char label[5];
    char bits[17];

    RenderOutput_PrepareForRedraw();
    UiText_DrawStringInWindow(0x0803742c, window, 48, 0);

    flag = start_flag << 8;
    for (row = 0; row != 16; row++) {
        s32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++) {
            label[i] = 0;
        }
        Func_08029274(flag, 3, (s32)label);
        UiText_DrawStringInWindow((s32)label, window, 0, y);
        UiText_DrawStringInWindow(0x08037428, window, 32, y);

        for (i = 0; i < 16; i++) {
            s32 val = BattleFlag_Test(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        UiText_DrawStringInWindow((s32)bits, window, 48, y);
    }
}
