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

void Func_08079374(s32 arg0) {
    s32 mask;
    u8 *base;

    mask = ~(1 << (7 & arg0));
    base = (u8 *) 0x02000040;
    arg0 = (u32) (arg0 << 0x14) >> 0x17;
    base[arg0] = (u8) (base[arg0] & mask);
}
