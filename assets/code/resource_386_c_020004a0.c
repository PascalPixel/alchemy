typedef signed int s32;

extern void Func_02000ae0(void);
extern s32 Func_02000abe(s32);
extern void Func_02000b48(s32);
extern void Func_02000b50(s32);
extern void Func_02000b68(s32, s32);
extern void Func_02000b0c(void);

void Func_020004a0(void)
{
    Func_02000ae0();
    if (Func_02000abe(0x85B) == 0) {
        Func_02000b48(0x1382);
    } else {
        Func_02000b50(0x1CF4);
    }
    Func_02000b68(18, 0);
    Func_02000b0c();
}
