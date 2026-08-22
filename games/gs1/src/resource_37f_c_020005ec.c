#include "types.h"

extern s32 *Func_020021be(s32);
extern void Func_0200219e(s32);
extern void Func_020021a4(s32);
extern void Func_020021a6(s32);
extern void Func_020021b4(s32);
extern void Func_02001de6(s32);

void Func_020005ec(void)
{
    s32 *record = Func_020021be(9);
    s32 heading;

    if (record == 0) {
        return;
    }
    heading = record[2] >> 20;
    Func_0200219e(0x310);
    Func_020021a4(0x311);
    if (heading == 99) {
        Func_020021a6(0x311);
    } else if (heading == 101) {
        Func_020021b4(0x310);
    }
    Func_02001de6(0);
}
