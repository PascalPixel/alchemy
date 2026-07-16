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

s32 Func_08002f40(s32 arg0) {
    register s32 *table asm("r3");
    register s32 index asm("r0");

    table = (s32 *)0x08320000;
    index = arg0 << 2;
    asm volatile("" : "+r" (table), "+r" (index));
    return *(s32 *)((u8 *)table + index);
}
