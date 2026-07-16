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

s32 Func_08018038(s32, s32);
s32 Func_08018850(s32, s32, s32, s32);

void Func_08018790(s32 arg0, s32 arg1, s32 arg2) {
    Func_08018850(Func_08018038(arg0, 0), arg1, arg2, 0);
}
