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

s32 Func_08009098(s32, s32);
s32 Func_0808ba1c();

void Func_0809259c(s32 arg0, s32 arg1) {
    s32 temp_r0;
    s32 var_r5;

    var_r5 = arg1;
    temp_r0 = Func_0808ba1c();
    if ((temp_r0 != 0) && (var_r5 > 0)) {
        if (var_r5 > 3) {
            var_r5 = 3;
        }
        Func_08009098(temp_r0, ((3 - var_r5) << 7) + 0x0809EBFC);
    }
}
