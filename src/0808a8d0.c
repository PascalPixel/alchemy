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

extern u8 Data_0809f1a8[];

s8 Func_0808a8d0(s32 arg0) {
    register u8 *base asm("r3") = Data_0809f1a8;

    return (s8) M2C_FIELD((base + (arg0 * 8)), u8 *, 2);
}
