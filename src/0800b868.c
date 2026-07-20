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

s32 Func_08185000(s16);
s32 Func_0800b798(s16);

void Func_0800b868(void *arg0) {
    s32 var_r0;
    s32 zero;
    void *temp_r0;
    if (arg0 != NULL) {
        temp_r0 = (void *)Func_08185000(M2C_FIELD(arg0, s16, 0));
        if (M2C_FIELD(temp_r0, u8, 0) != 0) {
            var_r0 = M2C_FIELD(temp_r0, s32, 0xC);
            if (var_r0 == 0) {
                var_r0 = Func_0800b798(M2C_FIELD(arg0, s16, 0));
            }
            M2C_FIELD(arg0, u8, 4) = M2C_FIELD(temp_r0, u8, 4);
            M2C_FIELD(arg0, s32, 0xC) = M2C_FIELD(temp_r0, s32, 0x10);
            M2C_FIELD(arg0, s32, 8) = var_r0;
            M2C_FIELD(arg0, u8, 7) = M2C_FIELD(temp_r0, u8, 0xA);
            zero = 0;
            M2C_FIELD(arg0, u8, 0x16) = 0xFF;
            M2C_FIELD(arg0, s32, 0x10) = zero;
            M2C_FIELD(arg0, u8, 0x14) = zero;
        }
    }
}
