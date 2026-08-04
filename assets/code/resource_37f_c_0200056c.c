#include "types.h"

extern s32 *Func_0200213e(s32);
extern void Func_0200211c(s32);
extern void Func_02002122(s32);
extern void Func_02002124(s32);
extern void Func_02002130(s32);

void Func_0200056c(void)
{
    s32 *record = Func_0200213e(9);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200211c(0x302);
    Func_02002122(0x303);
    if (heading == 93) {
        Func_02002124(0x303);
    } else if (heading == 95) {
        Func_02002130(0x302);
    }
}
