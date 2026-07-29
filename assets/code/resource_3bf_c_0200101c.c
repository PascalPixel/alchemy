typedef signed int s32;

extern s32 Value_00000f2d;

s32 Func_0200205c(s32, s32);
void Func_02006646(s32);

void Func_0200101c(void)
{
    if (Func_0200205c(11, 5) != 0) {
        Func_02006646((s32)&Value_00000f2d);
    }
}
