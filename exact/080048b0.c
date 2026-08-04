#include "types.h"

s32 Func_080048b0(s32 kind, s32 size)
{
    u32 *base;
    s32 offset;
    s32 aligned_size;
    u32 address;
    u32 next_address;
    u32 next;
    u32 result;

    base = (u32 *)0x03001E50;
    offset = kind * 4;
    result = *(u32 *)((u8 *)base + offset);
    if (result == 0) {
        result = base[1];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = result + aligned_size;
        if (next > 0x030077FFU) {
            address = base[0];
            next_address = address + aligned_size;
            if (next_address >= 0x02040000U) {
                return 0;
            }
            base[0] = next_address;
            *(u32 *)((u8 *)base + offset) = address;
            return (s32)address;
        }
        base[1] = next;
        *(u32 *)((u8 *)base + offset) = result;
        return (s32)result;
    }
    return (s32)result;
}
