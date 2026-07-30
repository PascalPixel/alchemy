typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_0808a490(s32);
extern u8 Value_000001ff;

s32 Func_080a46b4(s32 arg0, s32 arg1) {
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r6;
    s32 var_r3;
    s32 var_r7;
    u32 var_r7_2;
    u8 *temp_r8;
    u32 temp_r2;
    void *temp_r5;

    temp_r3 = (s32)&Value_000001ff;
    temp_r6 = arg1;
    temp_r6 &= temp_r3;
    temp_r5 = Func_08077018(temp_r6);
    var_r7 = -1;
    if (Func_0808a490(temp_r6) != 0) {
        return 0;
    }
    temp_r8 = Func_08077080(0x3FFF & M2C_FIELD(temp_r5, u16 *, 0x28));
    if (M2C_FIELD(temp_r5, u16 *, 0x28) != 0) {
        if ((M2C_FIELD(temp_r5, u8 *, 2) == 0) || ((M2C_FIELD(temp_r5, u8 *, 0xC) != 3) && (Func_08077218(arg0, temp_r6) != 0))) {
            var_r7 = 1;
        }
        if (var_r7 == 1) {
            temp_r2 = M2C_FIELD(temp_r8, u8 *, 1);
            if (0x40 & temp_r2) {
                temp_r2_2 = M2C_FIELD(temp_r8, u8 *, 8);
                temp_r2_2 ^= 0xFF;
                var_r7_2 = (u32) ((0 - temp_r2_2) | temp_r2_2) >> 0x1F;
                var_r3 = 2;
            } else {
                temp_r3 = 0x80 & temp_r2;
                var_r7_2 = (u32) ((0 - temp_r3) | temp_r3) >> 0x1F;
                var_r3 = 0;
            }
            var_r7 = var_r3 - var_r7_2;
        }
    }
    return var_r7;
}
