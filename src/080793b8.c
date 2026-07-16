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

extern u8 Data_02000040[];

u8 Func_080793b8(s32 arg0) {
    register u8 *base asm("r3");
    arg0 = (u32) (arg0 << 0x14) >> 0x17;
    asm volatile("" : "+r" (arg0));
    base = Data_02000040;
    asm volatile("" : "+r" (base) : "r" (arg0));
    return base[arg0];
}
