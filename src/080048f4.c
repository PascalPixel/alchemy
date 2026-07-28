#include "types.h"

void *Func_080048f4(s32 kind, s32 size)
{
    u32 *base;
    s32 offset;
    u32 aligned_size;
    u32 next;
    u32 address;
    u32 next_address;
    u32 result;

    base = (u32 *)0x03001E50;
    offset = kind * 4;
    result = *(u32 *)((u8 *)base + offset);
    if (result == 0) {
        address = base[0];
        aligned_size = (((u32)size + 3) >> 2) * 4;
        next = address + aligned_size;
        if (next >= (u32)(129 << 18)) {
            address = base[1];
            next_address = address + aligned_size;
            if (next_address > 0x030077FFU) {
                return NULL;
            }
            base[1] = next_address;
            *(u32 *)((u8 *)base + offset) = address;
            return (void *)address;
        }
        base[0] = next;
        *(u32 *)((u8 *)base + offset) = address;
        return (void *)address;
    }
    return (void *)result;
}
