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

u32 Func_080c23e8(u32 arg0) {
    u8 *table;
    u8 *p;
    u32 temp2;
    u32 temp_r3;

    if (arg0 > 0xABU) {
        return 1U;
    }
    table = Data_080c7420;
    p = table + (arg0 * 8);
    temp2 = (u32) p[2] << 0x1B;
    temp_r3 = temp2 >> 0x1C;
    {
        u32 var_r0;
        if (temp_r3 != 0U) {
            var_r0 = temp_r3;
        } else {
            var_r0 = 1U;
        }
        return var_r0;
    }
}
