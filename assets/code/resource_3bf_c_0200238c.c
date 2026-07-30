typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

s32 Func_020079b4(s32);
void Func_020079b0(s32, s32);
void Func_02007b5e(s32);
void Func_020079e4(s32);
void Func_020079a0(s32, s32, s32, s32, s32, s32);
void Func_020079fe(s32);
void Func_020079b6(s32, s32, s32, s32, s32, s32);
void Func_02007a14(s32);
void Func_02004690(void);
void Func_02007a0e(s32);

void Func_0200238c(void)
{
    u8 *g = Data_03001ebc;

    if (*(s16 *)(g + 3256) == 0) {
        return;
    }
    if (Func_020079b4(2376) != 0) {
        return;
    }
    Func_020079b0(5416, 1);
    Func_02007b5e(188);
    Func_020079e4(1);
    {
        s32 k5 = 3, k6 = 55;

        Func_020079a0(6, 77, 1, 2, k5, k6);
        Func_020079fe(5);
        Func_020079b6(7, 77, 1, 2, k5, k6);
    }
    Func_02007a14(1);
    Func_02004690();
    Func_02007a0e(2376);
}
