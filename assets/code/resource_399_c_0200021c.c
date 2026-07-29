typedef signed int s32;

extern s32 Func_02002450(void);
extern s32 Func_02002436(s32);
extern s32 Func_02002530(s32);
extern s32 Func_02002538(s32);
extern s32 Func_02002548(s32, s32);
extern s32 Func_0200247c(void);

void Func_0200021c(void) {
    Func_02002450();
    if (Func_02002436(3) != 0) {
        Func_02002530(0x1570);
    } else {
        Func_02002538(0x1529);
    }
    {
        s32 arg = 0;
        s32 mode = 8;
        Func_02002548(mode, arg);
    }
    Func_0200247c();
}
