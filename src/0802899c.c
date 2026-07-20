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

void Func_0801c2d0(void);
void Func_080284dc(void);
void Func_080287a8(s32);
void Func_08028808(s32, s32, s32);
s32 Func_080286a0(s32, s32);
void Func_0802851c(void);
void Func_0801c2e4(void);

s32 Func_0802899c(s32 arg0, s32 arg1) {
    Func_0801c2d0();
    Func_080284dc();
    Func_080287a8(1);
    Func_080287a8(0xF);
    Func_080287a8(2);
    Func_080287a8(7);
    Func_08028808(0x11, 7, 0);
    arg1 = Func_080286a0(arg0, arg1 - 1);
    Func_0802851c();
    Func_0801c2e4();
    return arg1;
}
