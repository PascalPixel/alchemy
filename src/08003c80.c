typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 Data_03001cd4;
extern s16 Data_03001cf8;
extern u8 Data_03001ca8;
extern u8 Data_03001aec;
extern volatile u8 Data_03001c98;
extern u8 Data_03001ac0;

void Func_08003c80(s8 arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4) {
    Data_03001cd4 = arg0;
    Data_03001cf8 = arg1 & 0x3f;
    if (arg2 > 0x10U) {
        Data_03001aec = Data_03001ca8;
    } else {
        Data_03001aec = arg2;
    }
    Data_03001ca8 = arg3;
    Data_03001ac0 = (Data_03001c98 = arg4);
}
