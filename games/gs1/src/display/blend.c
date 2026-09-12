#include "types.h"
#include "global_cells.h"

extern u8 Data_03001ac0;
extern volatile u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001cd4;
extern u16 Data_03001cf8;

s32 Func_080022ec(s32 numerator, s32 denominator);

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



void Blend_SetDarkenTarget16(s32 duration)
{
    Data_03001cd4 = 0;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0x10;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}



void Blend_SetDarkenTarget0(s32 duration)
{
    Data_03001cd4 = 0;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}



void Blend_SetBrightenTarget16(s32 duration)
{
    Data_03001cd4 = 1;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0x10;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}



void Blend_SetBrightenTarget0(s32 duration)
{
    Data_03001cd4 = 1;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}



void Blend_ConfigureTransition(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
{
    Data_03001cd4 = mode;
    Data_03001cf8 = coefficient & 0x3f;
    if (start > 0x10U) {
        Data_03001aec = Data_03001ca8;
    } else {
        Data_03001aec = start;
    }
    Data_03001ca8 = target;
    Data_03001ac0 = (Data_03001c98 = duration);
}

s32 WaitFrames(s32);

void Blend_WaitForTransition(void)
{
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            WaitFrames(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}
