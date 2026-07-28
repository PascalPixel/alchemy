#include "types.h"
#include "runtime_interfaces.h"


s32 Func_08020b14(const u8 *str) {
    u8 *base;
    u16 *dst;
    s32 count;
    s32 offset;
    s32 count_out;
    s32 width_out;

    base = *(u8 **)0x03001E8C;
    count = 0;
    if (*str != 0) {
        dst = (u16 *)(base + 0xEB0);
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    offset = 0xEB0 + count * 2;
    *(u16 *)(base + offset) = 0;
    Func_08018850(0, &count_out, &width_out, 0);
    return count_out;
}
