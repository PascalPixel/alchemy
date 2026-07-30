typedef signed int s32;

extern void Func_02002e6e(s32);
extern s32 Func_02002e7e(s32, s32);
extern s32 Func_02002e0e(s32, s32);

s32 Func_020012c4(void)
{
    Func_02002e6e(0x2549);
    Func_02002e7e(1, 0);
    return Func_02002e0e(0, 0) == 0;
}
