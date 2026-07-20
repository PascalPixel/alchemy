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

void *Func_08077018(s32);
s32 Func_08077218(s32, s32);
void *Func_08077080(s32);

s32 Func_08025180(s32 arg0, s32 arg1) {
    s32 var_r0;
    void *temp_r0;

    if (arg1 == 0) {
        return 1;
    }
    temp_r0 = Func_08077018(arg1);
    var_r0 = 1;
    if (M2C_FIELD(temp_r0, u8, 0xC) == 3) {
        return var_r0;
    }
    if (M2C_FIELD(temp_r0, u16, 0x28) == 0) {
        return var_r0;
    }
    if ((M2C_FIELD(temp_r0, u8, 2) != 0) && (Func_08077218(arg0, arg1) == 0)) {
        return var_r0;
    }
    if ((0x80 & M2C_FIELD(Func_08077080((s32) M2C_FIELD(temp_r0, u16, 0x28)), u8, 1)) == 0) {
        return 2;
    }
    return 0;
}
