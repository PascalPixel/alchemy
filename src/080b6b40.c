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

s32 Func_080770c0(s32);
void *Func_08077008(s32);
s32 Func_080b6a60(u16 *);

s32 Func_080b6b40(s32 arg0, u16 *arg1) {
    u16 sp4[8];
    u16 *pbase;
    s32 var_r5;
    s32 temp_r7_2;
    u16 *var_r6;
    s32 var_r8;
    s32 var_r9;
    u16 *var_r2;
    s32 temp_r7;
    s16 temp_r3;
    s32 temp_r0;
    void *temp_r0_2;

    var_r6 = arg1;
    var_r8 = 0;
    var_r9 = 6;
    if (Func_080770c0(0x16C) != 0) {
        var_r9 = 3;
    }
    if (arg0 & 1) {
        pbase = sp4;
        temp_r0 = Func_080b6a60(pbase);
        if (var_r8 < temp_r0) {
            var_r2 = pbase;
            var_r5 = temp_r0;
            do {
                temp_r7 = *var_r2;
                var_r2 += 1;
                temp_r3 = M2C_FIELD(Func_08077008(temp_r7), s16, 0x38);
                if (temp_r3 > 0) {
                    if (var_r6 != NULL) {
                        *var_r6 = temp_r7;
                        var_r6 += 1;
                    }
                    var_r8 += 1;
                }
                var_r5 -= 1;
            } while (var_r5 != 0);
        }
    }
    if (arg0 & 2) {
        var_r5 = 0x80;
        temp_r7_2 = var_r9 + 0x80;
        if (var_r5 < temp_r7_2) {
            do {
                temp_r0_2 = Func_08077008(var_r5);
                if ((M2C_FIELD(temp_r0_2, u8, 0x12A) != 0) && ((s32) M2C_FIELD(temp_r0_2, s16, 0x38) > 0)) {
                    if (var_r6 != NULL) {
                        *var_r6 = (u16) var_r5;
                        var_r6 += 1;
                    }
                    var_r8 += 1;
                }
                var_r5 += 1;
            } while (var_r5 < temp_r7_2);
        }
    }
    if (var_r6 != NULL) {
        *var_r6 = 0xFF;
    }
    return var_r8;
}
