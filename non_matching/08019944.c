#include "types.h"

u32 Func_08019944(u32 value, u32 clear)
{
    u32 name_offset;
    u32 value_offset;
    u8 *base;
    u16 name;
    u32 index;
    u32 result;
    u32 zero;

    base = *(u8 **)0x03001E8C;
    result = 0;
    index = 0;
    value_offset = 0x12bc;
    zero = index;
    name_offset = 0x12dc;
    name = *(u16 *)(name_offset + (u32)base);
    if (name == value) {
        result = *(u32 *)(value_offset + (u32)base);
        if (clear != 0) {
            *(u32 *)(value_offset + (u32)base) = zero;
            *(u16 *)(name_offset + (u32)base) = zero;
        }
    } else {
loop:
        index++;
        value_offset += 4;
        name_offset += 2;
        if (index <= 7) {
            if (*(u16 *)(name_offset + (u32)base) == value) {
                result = *(u32 *)(value_offset + (u32)base);
                if (clear != 0) {
                    *(u32 *)(value_offset + (u32)base) = zero;
                    *(u16 *)(name_offset + (u32)base) = zero;
                }
            } else {
                goto loop;
            }
        }
    }
    return result;
}
