#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern void Func_02004dbc(u8 *);

u8 *Func_02000048(void)
{
    s16 *table = Data_02000240;
    u8 *result;
    s32 value = table[225];
    s32 low = 15;

    if (value <= 17) {
        if (value >= low) {
            result = Data_0200dcc8;
        } else {
            result = Data_0200dab8;
        }
    } else {
        result = Data_0200dab8;
    }
    Func_02004dbc(result);
    return result;
}
