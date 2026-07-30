typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern void Func_02005b18(s32, s32, s32);
extern u8 *Func_02005abe(s32);
extern void Func_02005b32(s32, s32, s32);
extern u8 *Func_02005ad8(s32);
extern void Func_02005bbc(s32, s32);
extern void Func_02005b50(s32, s32, s32);
extern u8 *Func_02005af6(s32);
extern void Func_02005bdc(s32, s32);
extern void Func_02005b70(s32, s32, s32);
extern u8 *Func_02005b16(s32);
extern void Func_02005b88(s32, s32, s32);
extern u8 *Func_02005b2e(s32);
extern void Func_02005b9a(s32, s32, s32);
extern void Func_02005ba4(s32, s32, s32);
extern void Func_02005bae(s32, s32, s32);

void Func_020017a0(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Func_02005b18(21, 0x1060000, 0x2C20000);
    *(u16 *)(Func_02005abe(21) + 6) = m;
    Func_02005b32(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Func_02005ad8(24) + 6) = z;
    }
    Func_02005bbc(24, 1);
    Func_02005b50(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Func_02005af6(25) + 6) = x << 8;
    }
    Func_02005bdc(25, 1);
    Func_02005b70(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Func_02005b16(26) + 6) = x << 8;
    }
    Func_02005b88(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Func_02005b2e(27) + 6) = m;
    Func_02005b9a(22, 0, 0);
    Func_02005ba4(23, 0, 0);
    Func_02005bae(20, 0, 0);
}
