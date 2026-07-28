#include "types.h"

s32 Func_080030f8(s32);

void Func_08006488(void)
{
    s32 work;
    u32 count;
    s32 idle;

    count = 0;
    if (*(volatile s32 *)0x020023AC != 0) {
        work = 0x020023AC;
loop:
        Func_080030f8(1);
        count += 1;
        idle = 0;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != idle) {
                goto loop;
            }
        }
    }
}
