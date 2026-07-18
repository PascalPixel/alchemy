typedef signed int s32;

s32 Func_080022ec(s32, s32);
void Func_080150a8(s32, s32, s32, s32, s32);

void Func_080a14f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 value = arg0;
    s32 digits = 1;

    while (digits <= 15) {
        value = Func_080022ec(value, 10);
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    arg2 -= digits << 3;
    Func_080150a8(arg0, digits, arg1, arg2, arg3);
}
