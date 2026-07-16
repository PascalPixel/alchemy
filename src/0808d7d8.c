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

s32 Func_080072f0(s32);
void *Func_0808d48c(s32, s32);
s32 Func_080916b0();
s32 Func_08091750();
s32 Func_08092b94(s32);
s32 Func_08092f84(s32, s32);

s32 Func_0808d7d8(s32 arg0) {
    s32 temp_r3;
    s32 var_r6;
    void *temp_r0;

    temp_r0 = Func_0808d48c(6, arg0);
    var_r6 = -1;
    if (temp_r0 != NULL) {
        temp_r3 = M2C_FIELD(temp_r0, s32 *, 8);
        if (temp_r3 != 0) {
            if (temp_r3 < 0x10000) {
                Func_080916b0();
                Func_08092b94(M2C_FIELD(temp_r0, s32 *, 8));
                Func_08092f84(-1, 0);
                var_r6 = 0;
                Func_08091750();
            } else {
                Func_080072f0(arg0);
                var_r6 = 0;
            }
        }
    }
    return var_r6;
}
