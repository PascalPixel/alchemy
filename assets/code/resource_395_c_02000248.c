typedef signed int s32;

extern void Func_02001be4(void);
extern s32 Func_02001bd2(s32);
extern void Func_02001c7c(s32);
extern void Func_02001c84(s32);
extern void Func_02001c9c(s32, s32);
extern void Func_02001c10(void);

void Func_02000248(void)
{
    Func_02001be4();
    if (Func_02001bd2(0x845) != 0) {
        Func_02001c7c(0x151F);
    } else {
        Func_02001c84(0x14C8);
    }
    Func_02001c9c(9, 0);
    Func_02001c10();
}
