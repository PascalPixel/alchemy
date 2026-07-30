typedef signed int s32;

extern void Func_02002f12(s32);
extern s32 Func_02002f22(s32, s32);
extern s32 Func_02002eb2(s32, s32);

s32 Func_02001368(void)
{
    Func_02002f12(0x254A);
    Func_02002f22(3, 0);
    return Func_02002eb2(0, 0) == 0;
}
