#include "types.h"

extern s32 *Func_0200217e(s32);
extern void Func_0200215e(s32);
extern void Func_02002164(s32);
extern void Func_02002168(s32);
extern void Func_02002174(s32);

void Func_020005ac(void)
{
    s32 *record = Func_0200217e(10);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200215e(0x300);
    Func_02002164(0x301);
    if (heading == 115) {
        Func_02002168(0x300);
    } else if (heading == 113) {
        Func_02002174(0x301);
    }
}
