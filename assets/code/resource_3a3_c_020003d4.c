typedef signed int s32;

extern s32 Func_02001262(s32);
extern void Func_0200128a(void);
extern void Func_02001318(s32);
extern void Func_02001330(s32, s32);
extern void Func_020012a4(void);
extern s32 Func_02000732(void);
extern void Func_020013b6(s32, s32);
extern void Func_020012b4(void);
extern void Func_02001342(s32);
extern s32 Func_020012a0(s32);
extern void Func_02001352(s32);
extern void Func_0200136a(s32, s32);
extern void Func_020012de(void);

void Func_020003d4(void)
{
    if (Func_02001262(0x241) == 0) {
        Func_0200128a();
        Func_02001318(0x18ED);
        Func_02001330(20, 0);
        Func_020012a4();
        return;
    }

    if (Func_02000732() != 0) {
        Func_020013b6(20, 17);
        return;
    }

    Func_020012b4();
    Func_02001342(0x18EE);
    if (Func_020012a0(0x909) != 0) {
        Func_02001352(0x1943);
    }
    Func_0200136a(17, 0);
    Func_020012de();
}
