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

s32 Func_080c23c0(u32 arg0) {
    register s32 var_r1 asm("r1");
    register u8 *entry asm("r2");
    register u8 *table asm("r3");

    if (arg0 > 0xABU) {
        return 0;
    }
    var_r1 = 0;
    table = (u8 *)0x080C7420;
    entry = table + (arg0 * 8);
    if ((entry[2] << 0x1F) != 0) {
        var_r1 = 1;
    }
    return var_r1;
}
