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
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_08185000(s32);
s32 Func_0800b798(s32);
extern u8 Data_00000000[];

void *Func_0800bbc0(s32 arg0) {
    void *sp0;
    void *temp_r0;
    void *var_r2;
    void *var_r4;
    void *var_r5;
    s32 var_r0;
    s32 var_r1;
    s32 *temp_r2;
    u8 var_r3;
    void *zero;

    sp0 = NULL;
    temp_r0 = Func_08185000(arg0);
    var_r2 = *(void **)0x03001E5C;
    var_r5 = NULL;
    var_r4 = sp0;
    if (M2C_FIELD(temp_r0, u8, 0) != 0) {
        var_r1 = 0;
        var_r3 = M2C_FIELD(var_r2, u8, 4);
        while (var_r3 != 0) {
            var_r1++;
            var_r2 += 0x18;
            if (var_r1 > 0x3F) {
                goto done;
            }
            var_r3 = M2C_FIELD(var_r2, u8, 4);
        }
        var_r4 = var_r2;
done:
        if (var_r4 != NULL) {
            zero = Data_00000000;
            var_r0 = M2C_FIELD(temp_r0, s32, 0xC);
            var_r5 = var_r4;
            M2C_FIELD(var_r5, s16, 0) = arg0;
            if (var_r0 == 0) {
                var_r0 = Func_0800b798(arg0);
            }
            temp_r2 = M2C_FIELD(temp_r0, s32 *, 0x10);
            M2C_FIELD(var_r5, s32, 8) = var_r0;
            M2C_FIELD(var_r5, s32 *, 0xC) = temp_r2;
            M2C_FIELD(var_r5, u8, 7) = M2C_FIELD(temp_r0, u8, 0xA);
            M2C_FIELD(var_r5, u8, 0x16) = 0xFF;
            M2C_FIELD(var_r5, s32, 0x10) = *temp_r2;
            M2C_FIELD(var_r5, u8, 0x14) = (u32)zero;
            M2C_FIELD(var_r5, u8, 4) = M2C_FIELD(temp_r0, u8, 4);
            M2C_FIELD(var_r5, u8, 5) = (u32)zero;
        }
    }
    return var_r5;
}
