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

s32 Func_080072ec(s32, s32, s32);

void Func_080fb690(s32 arg0, s32 arg1) {
    register s32 *address asm("r2") = (s32 *)0x02004000;

    Func_080072ec(arg0, arg1, *address);
}
