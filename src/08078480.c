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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08078414(s32);

s32 Func_08078480(s32 arg0) {
    s32 var_r2;
    u8 *temp;

    temp = (u8 *) Func_08078414(arg0);
    var_r2 = 0;
    if (temp[2] == 1) {
        var_r2 = 1;
        goto end;
    }
    if (temp[2] == 2) {
        goto two;
    }
    if (temp[2] == 3) {
        goto two;
    }
    if (temp[2] == 4) {
        goto two;
    }
    if (temp[2] == 5) {
        goto two;
    }
    if (temp[2] == 9) {
        goto two;
    }
    goto end;
two:
    var_r2 = 2;
end:
    return var_r2;
}
