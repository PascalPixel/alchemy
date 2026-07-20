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

s32 Func_0808ba1c(s32);
void Func_08009088(s32, s32);
extern u8 Data_0200048a[];

void Func_08096cdc(s32 arg0, s32 arg1, s32 arg2) {
    s16 *var_sl;
    s32 temp_r0;
    s32 var_r5;

    var_r5 = 0;
    var_sl = (s16 *)Data_0200048a;
    do {
        temp_r0 = Func_0808ba1c(var_r5);
        if ((var_r5 != *var_sl) && (temp_r0 != 0) && (temp_r0 != arg0)) {
            M2C_FIELD(temp_r0, s8, 0x5B) = arg1;
            Func_08009088(temp_r0, arg2);
        }
        var_r5 += 1;
    } while (var_r5 <= 0x42);
}
