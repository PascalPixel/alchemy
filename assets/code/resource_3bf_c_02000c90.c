typedef signed int s32;

extern s32 Data_0200f714[];

void Func_0200627c(s32, s32, s32, s32, s32, s32);
void Func_0200628e(s32, s32, s32, s32, s32, s32);
void Func_020062a2(s32, s32, s32, s32, s32, s32);

void Func_02000c90(s32 i)
{
    s32 a = Data_0200f714[i * 2];
    s32 b = Data_0200f714[i * 2 + 1];

    Func_0200627c(0, 0x4d, 1, 3, a, b);
    Func_0200628e(1, 0x4d, 1, 1, a + 1, b);
    Func_020062a2(a, b - 0x30, 1, 1, a, b - 0x2e);
}
