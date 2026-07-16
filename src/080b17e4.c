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

void *Func_08077018(s32);
s32 Func_08077028(s32, s32);
s32 Func_08077228(s32, u8);
s32 Func_08077230(s32);
s32 Func_08077298(s16);
s32 Func_080b0574(s32);
s32 Func_080b10cc();
s32 Func_080b1868(s32, s32);
s32 Func_080b196c(s32, s32);
s32 Func_080f9010(s32);

void Func_080b17e4(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r9;
    s32 var_r5;
    s32 var_sl;
    void *temp_r0;

    var_r5 = arg2;
    temp_r0 = Func_08077018(arg1);
    var_sl = 0;
    temp_r9 = Func_08077228(arg0, M2C_FIELD(temp_r0, u8 *, 2));
    Func_080f9010(0x65);
    if (var_sl < var_r5) {
        do {
            var_sl = Func_08077028(arg0, arg1);
            Func_08077230(0 - M2C_FIELD(temp_r0, s16 *, 0));
            var_r5 -= 1;
            Func_08077298(M2C_FIELD(temp_r0, s16 *, 0));
            Func_080b10cc();
        } while (var_r5 != 0);
    }
    Func_080b0574(0xCA1);
    if (Func_080b1868(arg0, var_sl) != 0) {
        Func_080b196c(arg0, temp_r9);
    }
}
