#include "types.h"
#include "resource.h"
#include "runtime_mem.h"

extern u8 Data_03001388[];

void Func_080e46f0(s32 resource_id)
{
    u16 buf[64];
    u16 *pal = (u16 *)0x05000000;
    u16 *dst;
    s32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 tr;
    s32 tg;
    s32 tb;

    Mem_Copy(buf, GetResource(resource_id), 128, Data_03001388);
    buf[0] = 0;
    dst = buf;
    i = 0;
    do {
        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        tr = dst[i] & 0x1f;
        tg = (dst[i] >> 5) & 0x1f;
        tb = (dst[i] >> 10) & 0x1f;
        if (r < tr) {
            r++;
        } else if (r > tr) {
            r--;
        }
        if (g < tg) {
            g++;
        } else if (g > tg) {
            g--;
        }
        if (b < tb) {
            b++;
        } else if (b > tb) {
            b--;
        }
        dst[i] = (b << 10) | (g << 5) | r;
        pal++;
        i++;
    } while (i != 64);
    Mem_Copy((u16 *)0x05000000, buf, 128, Data_03001388);
}
