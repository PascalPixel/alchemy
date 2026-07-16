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

s32 Func_0809163c(s32);
s32 Func_08092f84();

void Func_08093040(s32 arg0, s32 arg1, s32 arg2) {
    Func_08092f84();
    Func_0809163c(arg2);
}
