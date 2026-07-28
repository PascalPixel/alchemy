#include "types.h"
#include "item.h"

s32 Func_08079cbc(s32 arg0)
{
    s32 sum;
    s32 offset;
    s32 index;
    u8 *data;
    s32 j;
    s32 mask;
    u16 v;

    sum = 0;
    offset = 216;
    mask = 0x200;
    index = 15;
    while (--index >= 0) {
        v = *(u16 *)((u8 *)offset + arg0);
        if (v & mask) {
            data = (u8 *)Func_08078414(
                *(u16 *)((u8 *)offset + arg0)) + 24;
            j = 4;
            while (--j >= 0) {
                if (data[0] == 23) { sum += (s8)data[1]; }
                data += 4;
            }
        }
        offset += 2;
    }
    if (sum < 0) sum = 0;
    return sum;
}
