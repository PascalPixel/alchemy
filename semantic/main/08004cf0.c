typedef signed int s32;

void Func_080072f0(s32 *, s32, s32, s32);

void Func_08004cf0(s32 *input) {
    s32 values[12];
    values[0] = 0x10000;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    values[4] = 0x10000;
    values[5] = 0;
    values[6] = 0;
    values[7] = 0;
    values[8] = 0x10000;
    values[9] = 0;
    values[10] = 0;
    values[11] = 0;
    values[0] = input[0];
    values[4] = input[1];
    values[8] = input[2];
    Func_080072f0(values, 0x10000, 0, 0x030002c0);
}
