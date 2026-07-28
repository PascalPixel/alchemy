typedef signed int s32;

extern s32 Func_02002784();
extern s32 Func_0200276a();
extern s32 Func_02002864();
extern s32 Func_02002874();
extern s32 Func_02002894();
extern s32 Func_0200279a();
extern s32 Func_020027a0();
extern s32 Func_020027c4();

void Func_02000550(void) {
    Func_02002784();
    if (Func_0200276a(3) != 0) {
        Func_02002864(0x1574);
        Func_02002874(20, 0);
    } else {
        Func_02002874(0x1557);
        Func_02002894(20, 0);
        Func_0200279a(0x82a);
        Func_020027a0(0x82c);
    }
    Func_020027c4();
}
