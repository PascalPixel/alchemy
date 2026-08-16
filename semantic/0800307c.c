#include "types.h"

typedef void (*InterruptHandler)(void);

extern u16 Data_04000208;
extern u16 Data_04000200;
extern u16 Data_04000004;
extern InterruptHandler Data_030000e0[];
extern void Func_08003008(void);

void Func_0800307c(s32 index, s32 setting, InterruptHandler handler)
{
    u32 saved;
    u32 mask;
    u32 value;

    if ((u32)index <= 13) {
        saved = *(volatile u16 *)&Data_04000208;
        *(volatile u16 *)&Data_04000208 = (u32)&Data_04000208;

        mask = 1;
        value = *(volatile u16 *)&Data_04000200;
        mask <<= index;
        value &= ~mask;
        if (handler != 0)
            value |= mask;
        *(volatile u16 *)&Data_04000200 = value;

        if ((u32)index <= 2) {
            u32 select = 8 << index;
            u32 clear = ~select;
            if (index == 2) {
                select |= setting << 8;
                clear &= 0xff;
            }
            value = *(volatile u16 *)&Data_04000004 & clear;
            if (handler != 0)
                value |= select;
            *(volatile u16 *)&Data_04000004 = value;
        }

        if (handler != 0)
            Data_030000e0[index] = handler;
        else
            Data_030000e0[index] = Func_08003008;

        *(volatile u16 *)&Data_04000208 = saved;
    }
}
