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

void Func_080153c8(s32, s32, s32, s32, s32);

s32 Func_080ab1f4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Func_080153c8(M2C_FIELD(arg0, u16 *, 0xC) + arg1 + 1, M2C_FIELD(arg0, u16 *, 0xE) + arg2 + 1, arg3, arg4, arg5);
}
