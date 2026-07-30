typedef signed int s32;
typedef unsigned char u8;

extern s32 Func_02004b08(s32, s32);
extern void Func_02004ace(s32);
extern u8 *Func_02004b04(s32);

void Func_020019e4(void)
{
    u8 *owner;

    Func_02004b08(8, 0x0200B3B8);
    Func_02004ace(0x203);
    owner = Func_02004b04(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}
