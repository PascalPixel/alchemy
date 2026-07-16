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

s32 Func_0800ca98(void *);

s32 Func_0800d674(void *arg0) {
    if ((u32) M2C_FIELD(arg0, u8 *, 0x60) > 0x3BU) {
        M2C_FIELD(arg0, u8 *, 0x60) = 0U;
        goto block_3;
    }
    if (Func_0800ca98(arg0) != 0) {
block_3:
        M2C_FIELD(arg0, u16 *, 4) = (u16) (M2C_FIELD(arg0, u16 *, 4) + 1);
        return 1;
    }
    return 0;
}
