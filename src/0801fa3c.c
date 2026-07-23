typedef signed int s32;
typedef signed short s16;

s32 Func_080056cc(void);
s32 Func_08005920(s32, void *);
void Func_08005cf8(void);
void Func_0801776c(s32, s32);
extern char Data_02000000;
extern char Value_0000000a;
extern char Value_0000000b;

s32 Func_0801fa3c(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = Func_080056cc();
    if (found != 0) {
        Func_0801776c((s32)&Value_0000000a, 1);
        result = -9;
    } else {
        void *base = &Data_02000000;

        found = Func_08005920(arg0, base);
        base = (char *)base + 0x1000;
        found |= Func_08005920(arg0 + 3, base);
        if (found != 0) {
            Func_0801776c((s32)&Value_0000000b, 1);
            result = -3;
        }
    }
    Func_08005cf8();
    return result;
}
