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

s32 Func_08003f3c(u8);

void *Func_08096c48(void *arg0, void *arg1) {
    void *var_r0;
    void *var_r5;

    var_r5 = arg0;
    var_r0 = NULL;
    if (var_r5 != NULL) {
        if (arg1 == NULL) {
            M2C_FIELD(var_r5, u8 *, 0x1D) = (u8) (M2C_FIELD(var_r5, u8 *, 0x1D) | 1);
        } else {
            Func_08003f3c(M2C_FIELD(var_r5, u8 *, 0x1C));
            M2C_FIELD(var_r5, u8 *, 0x1C) = (u8) M2C_FIELD(arg1, u8 *, 0x1C);
            M2C_FIELD(var_r5, u8 *, 0x1D) = (u8) (M2C_FIELD(var_r5, u8 *, 0x1D) | 1);
            var_r5 = arg1;
        }
        var_r0 = var_r5;
    }
    return var_r0;
}
