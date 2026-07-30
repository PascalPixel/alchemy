typedef signed int s32;

extern void Func_020036be(s32, s32, s32, s32, s32, s32);
extern void Func_02003774(s32, s32, s32);

void Func_020011d8(void)
{
    s32 width = 25;
    s32 height = 48;

    Func_020036be(24, 48, 1, 2, width, height);
    Func_02003774(12, 0x80000, 0x80000);
}
