typedef signed int s32;

extern s32 Data_0200f764[];

void Func_020064cc(s32, s32, s32, s32, s32, s32);
void Func_020064de(s32, s32, s32, s32, s32, s32);
void Func_020064fa(s32, s32, s32, s32, s32, s32);

void Func_02000ee0(s32 i)
{
    s32 a = Data_0200f764[i * 2];
    s32 b = Data_0200f764[i * 2 + 1];

    Func_020064cc(1, 0x50, 1, 3, a, b);
    Func_020064de(2, 0x50, 1, 1, a + 1, b);
    Func_020064fa(a, b - 0x3f, 1, 1, a, b - 0x3e);
}
