#include "types.h"

extern s8 Data_03001cd4;
extern s16 Data_03001cf8;
extern u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001ac0;

void Func_08003c80(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
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
