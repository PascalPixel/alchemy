#include "types.h"

s32 Func_080022ec(s32, s32);

extern volatile u16 Data_03001cf8;
extern volatile u16 Data_03001cf8_a;

void Func_08003adc(void)
{
    u8 counter;
    u8 start;
    u32 result;

    if (*(volatile u8 *)0x03001C98 != 0) {
        if (*(volatile u8 *)0x03001CD4 != 0) {
            u16 sample = Data_03001cf8;
            result = (u32)sample | 0x80;
        } else {
            u16 sample = Data_03001cf8_a;
            result = (u32)sample | 0xC0;
        }
        *(volatile u16 *)0x04000050 = (u16)result;
        *(volatile u8 *)0x03001AC0 += 0xFF;
        start = *(volatile u8 *)0x03001CA8;
        *(volatile u16 *)0x04000054 = start + Func_080022ec(
            (*(volatile u8 *)0x03001AEC - *(volatile u8 *)0x03001CA8)
                * *(volatile u8 *)0x03001AC0,
            *(volatile u8 *)0x03001C98);
        counter = *(volatile u8 *)0x03001AC0;
        if (counter == 0) {
            *(volatile u8 *)0x03001C98 = counter;
        }
    }
}
