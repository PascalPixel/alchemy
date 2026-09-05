#include "types.h"

extern u8 Data_03001cd4;
extern u16 Data_03001cf8;
extern volatile u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001ac0;

void Blend_SetBrightenTarget0(s32 duration)
{
    Data_03001cd4 = 1;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}
