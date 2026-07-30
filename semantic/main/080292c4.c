#include "types.h"

void Func_08016478(s32 handle);
void Func_0801e940(u8 *text, s32 handle, u32 x, u32 y);
void Func_08029274(u32 value, u32 count, u8 *output);
s32 Func_080770c0(s32 flag);

void Func_080292c4(s32 handle, s32 selection)
{
    u8 numberText[5];
    u8 stateText[17];
    s32 row;
    s32 flag;
    s32 y;
    u8 *states = stateText;
    u8 *number = numberText;

    Func_08016478(handle);
    Func_0801e940((u8 *)0x0803742c, handle, 48, 0);

    flag = selection << 8;
    y = 16;
    row = 0;
    do {
        u8 *cursor;

        cursor = number;
        do {
            *cursor++ = 0;
        } while (cursor != number + 5);

        Func_08029274(flag, 3, number);
        Func_0801e940(number, handle, 0, y);
        Func_0801e940((u8 *)0x08037428, handle, 32, y);

        {
            u32 column = 0;

            do {
                states[column] = (Func_080770c0(flag) != 0) + '0';
                flag++;
                column++;
            } while (column <= 15);
        }
        states[16] = 0;

        Func_0801e940(states, handle, 48, y);
        y += 8;
        row++;
    } while (row != 16);
}
