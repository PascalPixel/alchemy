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

s32 Func_0800d9f0(void *arg0) {
    s32 first;
    s32 second;
    s32 third;
    void *temp_r3;

    temp_r3 = M2C_FIELD(arg0, s32 *, 0) + (M2C_FIELD(arg0, s16 *, 4) * 4);
    temp_r3 += 4;
    first = M2C_FIELD(temp_r3, s32 *, 0);
    temp_r3 += 4;
    second = M2C_FIELD(temp_r3, s32 *, 0);
    temp_r3 += 4;
    third = M2C_FIELD(temp_r3, s32 *, 0);
    Func_0800d130(arg0, first, second, third);
    M2C_FIELD(arg0, s16 *, 4) = (s16) ((u16) M2C_FIELD(arg0, s16 *, 4) + 4);
    return 1;
}
