typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 *Data_03001e70;
extern void Func_02005cbc(s32);
extern void Func_02005cd2(s32);
extern void Func_02005c38(s32);
extern void Func_02005cf6(s32);
extern void Func_02005d5a(s32, s32, s32);
extern u8 *Func_02005d00(s32);
extern s32 Func_02005ccc(s32);
extern void Func_02005d7c(s32, s32, s32);
extern u8 *Func_02005d22(s32);
extern void Func_02005d92(s32, s32, s32);
extern u8 *Func_02005d38(s32);
extern void Func_02005daa(s32, s32, s32);
extern u8 *Func_02005d50(s32);
extern void Func_02005dc0(s32, s32, s32);
extern u8 *Func_02005d66(s32);
extern void Func_020059a4(void);
extern void Func_02005d6c(void);

extern u8 Data_0200d418[];
extern u8 Value_00000903;
extern u8 Data_02000240[];

void Func_020019c0(s32 a)
{
    s32 m = 0x3000;

    *(s32 *)(Data_03001e70 + 0xEC) = 0x410000;
    Func_02005cbc(a);
    Func_02005cd2((s32)Data_0200d418);
    Func_02005c38(1);
    Func_02005cf6(24);
    Func_02005d5a(23, 0xEE0000, 0x2720000);
    *(u16 *)(Func_02005d00(23) + 6) = m;
    if (Func_02005ccc((s32)&Value_00000903)) {
        Func_02005d7c(22, 0xA20000, 0x27A0000);
        *(u16 *)(Func_02005d22(22) + 6) = m;
        Func_02005d92(21, 0xA20000, 0x2A40000);
        {
            u8 *q = Func_02005d38(21);
            s32 x = 0xD0;
            *(u16 *)(q + 6) = x << 8;
        }
    } else {
        Func_02005daa(22, 0xA00000, 0x28C0000);
        *(u16 *)(Func_02005d50(22) + 6) = m;
        Func_02005dc0(21, 0xA60000, 0x29C0000);
        {
            u8 *q = Func_02005d66(21);
            s32 x = 0xB0;
            *(u16 *)(q + 6) = x << 8;
        }
    }
    {
        s16 *tb = (s16 *)Data_02000240;
        if (tb[0xE1] == 6)
            Func_020059a4();
    }
    Func_02005d6c();
}
