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

s32 Func_080772b8(void *);

s32 Func_0802706c(void *arg0) {
    s32 sign_byte;
    s32 kind;
    s32 temp_r2;
    s32 var_r5;
    u8 temp_r3;

    var_r5 = 0;
    temp_r2 = *(u8 *)((u8 *)(arg0) + 1) & 0xF;
    if (temp_r2 == 1) {
        var_r5 = 1;
    }
    if (temp_r2 == 0xB) {
        var_r5 = 2;
    }
    sign_byte = (*(s8 *)((u8 *)(arg0) + 3)); kind = (s32)(sign_byte);
    temp_r3 = kind;
    if (temp_r3 == 3) {
        var_r5 = 3;
    }
    if (temp_r3 == 4) {
        var_r5 = 4;
    }
    if (temp_r3 == 0x40) {
        var_r5 = 6;
    }
    if (Func_080772b8(*(u8 *)((u8 *)(arg0) + 3)) != 0) {
        var_r5 = 5;
    }
    return var_r5;
}
