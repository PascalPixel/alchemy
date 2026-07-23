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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02004e8c(void *, s32);
void Func_02004f8c(void *, s32);

void Func_02000030(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s8 *, 0x55) = 0;
    M2C_FIELD((arg0 + 0x55), s8 *, 4) = 8;
    Func_02004e8c(arg0, 0);
    Func_02004f8c(arg0, arg1);
}
