#include "types.h"

u8 *Func_0200138a();        /* scene/actor record accessor */
void Func_02001490();
void Func_02001386();       /* opens a scripted sequence */
s32 Func_0200136c();        /* tests a flag id; nonzero when set */
void Func_0200141e();       /* queues a cue / script id */
void Func_0200142e();
void Func_02001436();
void Func_02001456();
void Func_020013ba();       /* closes the scripted sequence */

void Func_020004bc(void)
{
    u8 *record;

    record = Func_0200138a(0);

    if ((u32)((*(u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02001490(6, 18);
        return;
    }

    Func_02001386();

    if (Func_0200136c(0x909) != 0) {
        Func_0200141e(0x1947);
        Func_02001436(18, 0);
    } else {
        Func_0200142e(0x18f5);
        Func_02001456(18, 0);
    }

    Func_020013ba();
}
