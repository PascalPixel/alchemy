#include "types.h"

extern s16 Data_080b41ac[][33];

s32 Func_080770c0(s32);
void Func_080770c8(s32);
void Func_08077240(s32, s32);

void Func_080b26cc(s32 arg0) {
    s16 *row;
    s16 *entry;
    s32 value;
    s32 count;
    s32 flag;

    flag = arg0 + 0x400;
    if (Func_080770c0(flag) == 0) {
        Func_080770c8(flag);
        count = 0;
        value = Data_080b41ac[arg0][24];
        if (value != 0) {
            row = Data_080b41ac[arg0];
            entry = row + 24;
            do {
                Func_08077240(value, 1);
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
