typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
s32 Func_080022ec(s32 arg0, s32 arg1);
s32 Func_08011d94(s8 *arg0, s32 arg1, s32 arg2) {
    s32 temp_r6, temp_r5, temp_r3, result;
    temp_r6 = *arg0++ << 0x13;
    temp_r5 = *arg0 << 0x13;
    temp_r3 = arg0[1] << 0x13;
    arg1 = arg1 + arg2;
    if ((u32)arg1 == 0xF) {
        result = temp_r5;
    } else if ((u32)arg1 <= 0xEU) {
        result = temp_r6 + Func_080022ec((temp_r5 - temp_r6) * arg1, 0xF);
    } else {
        arg1 = arg1 - 0xF;
        result = temp_r5 + Func_080022ec((temp_r3 - temp_r5) * arg1, 0xF);
    }
    return result;
}
