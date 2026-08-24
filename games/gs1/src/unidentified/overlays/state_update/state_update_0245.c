#include "types.h"

extern s32 *Func_02002206(s32);
extern void Func_020021e4(s32);
extern void Func_020021ea(s32);
extern void Func_020021ec(s32);
extern void Func_020021f8(s32);
extern void Func_02001e2a(s32);

void Func_02000634(void)
{
    s32 *record = Func_02002206(10);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_020021e4(0x312);
    Func_020021ea(0x313);
    if (heading == 103) {
        Func_020021ec(0x313);
    } else if (heading == 105) {
        Func_020021f8(0x312);
    }
    Func_02001e2a(0);
}
