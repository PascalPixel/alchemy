#include "types.h"

struct LinkState_08005fcc {
    u8 mode;
    u8 initialized;
    u8 received_hi;
    u8 received_lo;
    u8 padding04[5];
    u8 error;
    u8 padding0a;
    u8 tick;
    u8 padding0c[8];
    s32 transfer_state;
};

extern struct LinkState_08005fcc Data_02002240;

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

    initialized = Data_02002240.initialized;
    keys = *(volatile u32 *)key_base;
    mask = 0x88;

    if (initialized == 0) {
        masked_keys = keys;
        masked_keys &= mask;
        if (masked_keys == 8) {
            zero = keys & 4;
            if (zero == 0 && Data_02002240.transfer_state == -1) {
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
                Data_02002240.mode = masked_keys;
            }
            Data_02002240.initialized = 1;
        }
        Data_02002240.tick++;
    }

    packed = Data_02002240.received_lo | (Data_02002240.received_hi << 8);
    if (Data_02002240.mode == 8)
        packed |= mask & 0x80;
    result = packed;
    if (Data_02002240.error != 0)
        result |= 0x1000;
    if ((u32)(keys << 26) >> 30 > 1)
        result |= 0x2000;
    return result;
}
