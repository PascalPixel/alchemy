#include "types.h"
#include "scene.h"
#include "abi/menu/draw/draw_flag_bit_table.h"

void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
    s32 row;
    s32 y;
    s32 flag;
    char label[5];
    char bits[17];

    Menu_Run();
    Menu_SetMode(0x0803742c, window, 48, 0);

    flag = start_flag << 8;
    for (row = 0; row != 16; row++) {
        s32 i;

        y = row * 8 + 16;

        for (i = 0; i != 5; i++) {
            label[i] = 0;
        }
        Menu_Place(flag, 3, (s32)label);
        Menu_SetMode((s32)label, window, 0, y);
        Menu_SetMode(0x08037428, window, 32, y);

        for (i = 0; i < 16; i++) {
            s32 val = Menu_Check(flag);
            bits[i] = (val != 0) + 48;
            flag++;
        }
        bits[i] = 0;
        Menu_SetMode((s32)bits, window, 48, y);
    }
}
