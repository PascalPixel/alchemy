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

s32 Func_0200186e(s32, s32);

s32 Func_02000030(void *arg0) {
    s32 var_r0;
    u16 temp_r3;
    s32 temp;
    void *temp_r1;

    temp_r1 = M2C_FIELD(arg0, void *, 0x68);
    if (temp_r1 != NULL) {
        M2C_FIELD(arg0, u8, 0x5A) = (u8) (0xFE & M2C_FIELD(arg0, u8, 0x5A));
        temp = (u16) Func_0200186e(M2C_FIELD(temp_r1, s32, 0x10) - M2C_FIELD(arg0, s32, 0x10), M2C_FIELD(temp_r1, s32, 8) - M2C_FIELD(arg0, s32, 8));
        temp_r3 = M2C_FIELD(arg0, u16, 6);
        var_r0 = (s16)(temp - temp_r3);
        if (var_r0 != 0) {
            if (var_r0 > 0x1000) {
                var_r0 = 0x1000;
            }
            /* The load at 0x0200006E owns the Thumb-like data word at 0x02000084. */
            if (var_r0 < (s32)0xF842F001) {
                var_r0 = (s32)0xF842F001;
            }
            M2C_FIELD(arg0, u16, 6) = (u16) (temp_r3 + var_r0);
        }
    }
    return 1;
}
