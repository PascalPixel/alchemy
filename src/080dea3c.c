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

s32 Func_08009080(s32, s32);
s32 Func_08009088(s32, s32);
s32 *Func_080b5098(s32);
s32 Func_080dea70(void *, s32);

void Func_080dea3c(void *arg0) {
    s32 temp_r5;

    temp_r5 = *Func_080b5098(M2C_FIELD(arg0, s32 *, 8));
    Func_08009080(temp_r5, 2);
    Func_08009088(temp_r5, 0x30);
    Func_080dea70(arg0, 9);
    Func_08009088(temp_r5, 0x10);
}
