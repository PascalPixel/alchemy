#include "types.h"

/* Palette RAM, walked backwards from the last entry of a sixteen-colour bank. */
extern s16 Data_050000de[];

extern u8 Value_00000000;

void Func_02000dfc(void)
{
    s16 value = (s16) (s32) (u32) &Value_00000000;
    u32 index = 0;
    s16 *slot = Data_050000de;

    do {
        index++;
        *slot-- = value;
    } while (index <= 6);
}
