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

s8 Func_080770c0(s32);

s32 Func_0800d7f8(void *arg0) {
    M2C_FIELD(arg0, s8 *, 0x57) = Func_080770c0(M2C_FIELD(((M2C_FIELD(arg0, s16 *, 4) * 4) + M2C_FIELD(arg0, s32 *, 0)), s32 *, 4));
    M2C_FIELD(arg0, s16 *, 4) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 4) + 2);
    return 1;
}
