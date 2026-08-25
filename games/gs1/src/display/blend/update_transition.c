#include "types.h"

extern u8 Data_03001ac0;
extern volatile u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001cd4;
extern u16 Data_03001cf8;

s32 Func_080022ec(s32 numerator, s32 denominator);

#define BlendTransition_Update Func_08003adc

void BlendTransition_Update(void)
{
    if (Data_03001c98 != 0) {
        {
            volatile u16 *blend_control;
            u32 control;

            if (Data_03001cd4 != 0) {
                control = Data_03001cf8 | 0x80;
                blend_control = (volatile u16 *)0x04000050;
            } else {
                control = Data_03001cf8 | 0xc0;
                blend_control = (volatile u16 *)0x04000050;
            }
            *blend_control = control;
        }
        {
            u8 *remaining = &Data_03001ac0;
            s32 delta;
            s32 level;
            s32 step;

            (*remaining)--;
            level = Data_03001ca8;
            delta = Data_03001aec - Data_03001ca8;
            step = *remaining;
            level += Func_080022ec(delta * step, Data_03001c98);
            *(volatile u16 *)0x04000054 = level;
            if (*remaining == 0)
                Data_03001c98 = 0;
        }
    }
}
