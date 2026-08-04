#include "types.h"

extern u8 Data_02002240[];

s32 Func_08005fcc(void)
{
    s32 masked_keys;
    s32 keys;
    s32 result;
    s32 packed;
    u8 zero;
    u8 initialized;
    volatile u8 *key_base = (volatile u8 *)0x04000128;
    volatile u16 *wait_control;
    volatile u16 *interrupt_enable;
    volatile u16 *interrupt_flags;
    s32 mask;
    s32 temp;

    initialized = Data_02002240[1];
    keys = *(volatile u32 *)key_base;
    mask = 0x88;

    if (initialized == 0) {
        masked_keys = keys;
        masked_keys &= mask;
        if (masked_keys == 8) {
            zero = keys & 4;
            if (zero == 0 && *(s32 *)(Data_02002240 + 0x14) == -1) {
                wait_control = (volatile u16 *)0x04000208;
                interrupt_enable = (volatile u16 *)0x04000200;
                *wait_control = zero;
                temp = -0x81;
                *interrupt_enable = (*interrupt_enable & temp) | 0x40;
                temp = 1;
                *wait_control = temp;
                temp -= 66;
                key_base[1] &= temp;
                interrupt_flags = (volatile u16 *)0x04000202;
                *interrupt_flags = 0xc0;
                *(volatile u32 *)0x0400010c = 0xc963;
                Data_02002240[0] = masked_keys;
            }
            Data_02002240[1] = 1;
        }
        Data_02002240[0xb]++;
    }

    packed = Data_02002240[3] | (Data_02002240[2] << 8);
    if (Data_02002240[0] == 8)
        packed |= mask & 0x80;
    result = packed;
    if (Data_02002240[9] != 0)
        result |= 0x1000;
    if ((u32)(keys << 26) >> 30 > 1)
        result |= 0x2000;
    return result;
}
