typedef signed int s32;

void Func_080a1c6c(s32 *, s32, s32, s32, s32);

void Func_080a1cb0(s32 arg0) {
    s32 *var_r5;
    s32 *var_r7;
    s32 temp_r3;
    s32 var_r6;
    s32 var_r8;
    s32 base;

    base = *(s32 *)0x03001F2C;
    var_r8 = 0x38;
    if (arg0 != 1) {
        var_r8 = 0x28;
    }
    var_r5 = (s32 *)(base + 0x48);
    var_r6 = 0;
    var_r7 = var_r5;
    do {
        temp_r3 = *var_r7++;
        if (temp_r3 != 0) {
            Func_080a1c6c(var_r5, var_r6, 0x74, var_r8, 5);
        }
        var_r6++;
        var_r5++;
    } while (var_r6 <= 0xE);
}
