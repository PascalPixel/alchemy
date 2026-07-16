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

extern u8 Data_03001e50[];

s32 Func_0800488c(void) {
    register u8 *base asm("r3") = Data_03001e50;

    return 0x03007800 - M2C_FIELD(base, s32 *, 4);
}
