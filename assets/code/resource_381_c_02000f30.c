typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];
extern void Func_02004528(void);
extern void Func_02000f9c(void);

s32 Func_02000f30(void)
{
    if (Data_02000240[225] == 10) {
        Func_02004528();
        Func_02000f9c();
    }
    return 0;
}
