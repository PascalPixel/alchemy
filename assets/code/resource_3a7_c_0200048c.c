typedef signed int s32;

extern void Func_02000600(void);
extern s32 Func_02001596(s32);
extern s32 Func_020008f6(void);
extern s32 Func_020015a8(s32);
extern void Func_02001602(s32);
extern void Func_020008c6(void);
extern void Func_020015c4(s32);

void Func_0200048c(void)
{
    Func_02000600();
    if (Func_02001596(0x9AA) != 0) {
        return;
    }
    if (Func_020008f6() == 0) {
        return;
    }
    if (Func_020015a8(0x207) != 0) {
        return;
    }
    Func_02001602(0x50);
    Func_020008c6();
    Func_020015c4(0x9AA);
}
