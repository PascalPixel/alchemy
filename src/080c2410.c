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

extern u8 Data_080c7420[];

u32 Func_080c2410(u32 arg0) {
    u32 temp_r3;
    u32 var_r0;
    u8 *table;

    if (arg0 > 0xABU) {
        return 0U;
    }
    table = Data_080c7420;
    temp_r3 = table[(arg0 * 8) + 2] >> 5;
    if (temp_r3 != 0) {
        var_r0 = temp_r3;
    } else {
        var_r0 = 0;
    }
    return var_r0;
}
