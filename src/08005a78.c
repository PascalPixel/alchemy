#include "types.h"

u32 Func_08005b24(s32 arg0);
void Func_080058ac(u32 arg0);

u32 Func_08005a78(s32 record_id, void *destination)
{
    u8 *record;
    u32 index;

    record = *(u8 **)0x03001f1c;
    index = Func_08005b24(record_id);
    if (index > 15)
        return 1;
    Func_080058ac(index);
    {
        u8 *source = record + 80;
        u32 control = 0x840003fc;
        volatile u32 *dma = (volatile u32 *)0x040000d4;

        dma[0] = (u32)source;
        dma[1] = (u32)destination;
        dma[2] = control;
    }
    {
        volatile u32 *status = (volatile u32 *)0x040000d4;

        while ((status[2] & 0x80000000) != 0)
            ;
    }
    return 0;
}
