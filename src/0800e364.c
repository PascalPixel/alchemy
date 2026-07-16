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

void Func_0800e364(void *arg0, s32 arg1, s32 arg2) {
    s8 var_r1;

    if (arg1 == 0) {
        M2C_FIELD(arg0, s32 *, 0x18) = arg2;
        return;
    }
    if (arg1 == 1) {
        M2C_FIELD(arg0, s32 *, 0x18) = (s32) (M2C_FIELD(arg0, s32 *, 0x18) + arg2);
        return;
    }
    var_r1 = 0;
    if (M2C_FIELD(arg0, s32 *, 0x18) == arg2) {
        var_r1 = 1;
    }
    M2C_FIELD(arg0, s8 *, 0x57) = var_r1;
}
