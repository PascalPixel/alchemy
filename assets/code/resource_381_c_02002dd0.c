typedef unsigned int u32;
typedef signed int s32;

s32 Func_020061c4(u32, s32);
void Func_02006294(s32, s32);
void Func_02005cb0(s32);

void Func_02002dd0(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02006294(a, Func_020061c4(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        Func_02005cb0(a);
    }
}
