#include "types.h"

extern s16 Data_080b41ac[][33];

s32 GameFlag_IsSet(s32 flag_no);
void GameFlag_Set(s32 flag_no);
void Ability_GetMaximum(s32 value, s32 enabled);

void Func_080b26cc(s32 row_no)
{
    s16 *row;
    s16 *entry;
    s32 value;
    s32 count;
    s32 flag;

    flag = row_no + 0x400;
    if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        count = 0;
        value = Data_080b41ac[row_no][24];
        if (value != 0) {
            row = Data_080b41ac[row_no];
            entry = row + 24;
            do {
                Ability_GetMaximum(value, 1);
                count++;
                if (count > 7) {
                    break;
                }
                entry++;
                value = *entry;
            } while (value != 0);
        }
    }
}
