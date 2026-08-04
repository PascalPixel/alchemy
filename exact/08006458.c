#include "types.h"

s32 Func_080030f8(s32);

void Func_08006458(void)
{
    u32 work;
    u32 count;

    count = 0;
    if (*(volatile s32 *)0x02002080 != 0) {
        work = 0x02002080;
loop:
        Func_080030f8(1);
        count += 1;
        if (count <= 0x927BFU) {
            if (*(volatile s32 *)work != 0) {
                goto loop;
            }
        }
    }
}
