#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 Data_03001cd4;
extern s16 Data_03001cf8;
extern u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001ac0;

void Func_08003bb4(s32 arg0) {
    Data_03001cd4 = 0;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = arg0;
    Data_03001ac0 = Data_03001c98;
}
