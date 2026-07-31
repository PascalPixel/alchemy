typedef signed int s32;

extern s32 Data_02009dd4;

extern void Func_02002b7a(s32);
extern void Func_02002d50(s32);
extern void Func_02002b8c(s32);
extern void Func_02002d62(s32);

void Func_020012b4(void)
{
    s32 step = Data_02009dd4;

    if (step == 0) {
        Func_02002b7a(0);
        Func_02002d50(20);
    } else if (step == 20) {
        Func_02002b8c(1);
        Func_02002d62(8);
    }
    step = Data_02009dd4 + 1;
    Data_02009dd4 = step;
    if (step == 30) {
        Data_02009dd4 = 0;
    }
}
