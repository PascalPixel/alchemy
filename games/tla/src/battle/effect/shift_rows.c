#include "types.h"
#include "dma.h"
#include "iwram_call.h"

void Func_080e9250(u8 *base, s32 stride, s32 rows)
{
    u8 *source = base + (rows - 2) * stride;
    u8 *destination = base + (rows - 1) * stride;
    s32 i;

    for (i = rows - 1; i != 1; i--)
    {
        Dma_Set(source, destination, 0x84000000 | (stride / 4), (volatile u32 *)0x040000d4);
        source -= stride;
        destination -= stride;
    }
    Iwram_FillWords(base, stride, 0);
}
