typedef signed int s32;

extern s32 *Data_03001ee0;
extern s32 Func_02005190(s32);
extern s32 Func_020051e6(s32);
extern void Func_020051b0(s32);

void Func_020015b4(void) {
    s32 *p;

    if (Func_02005190(512) == 0) {
        p = Data_03001ee0;
        p[6] = Func_020051e6(0);
        Func_020051b0(512);
    }
}
