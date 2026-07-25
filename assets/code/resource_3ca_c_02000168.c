typedef signed int s32;

extern s32 Data_020097f4;
extern s32 Data_020097f0;

void Func_02000168(void) {
    s32 n = Data_020097f4 + 1;

    Data_020097f4 = n;
    if (n == 40) {
        if (Data_020097f0 > 4) {
            Data_020097f0 -= 1;
            Data_020097f4 = 0;
        }
    }
}
