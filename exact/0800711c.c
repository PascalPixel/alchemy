#include "types.h"

struct FlashInfo_0800711c {
    u8 filler0[36];
    u16 control;
};

void Func_08006ac0(void *);
u16 Func_08007098(u16);

u16 Func_0800711c(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;
    u16 count;

    if ((u32)sector > 15)
        return 0x80ff;

    Func_08006ac0(savedCode);
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) |
        ((struct FlashInfo_0800711c *)0x08007c10)->control;

    current = sector << 4;
    current = (u16)(current << 1);
    count = 0;
next_sector:
    retries = 2;
    goto attempt;
retry_failed:
    retries -= 1;
    if (retries == 0)
        goto advance;
attempt:
    result = Func_08007098(current);
    if (result != 0)
        goto retry_failed;
advance:
    current += 1;
    if (result != 0)
        goto done;
    count += 1;
    if ((u32)count <= 31)
        goto next_sector;

done:
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) | 3;
    return result;
}
