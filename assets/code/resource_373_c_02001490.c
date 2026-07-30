typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02007482(s32);
extern void Func_02007460(void);
extern void Func_02007508(s32, s32);
extern void Func_0200757c(s32, s32, s32);
extern void Func_0200751c(s32, s32);
extern void Func_02007590(s32, s32, s32);
extern void Func_020074c8(s32, s32);
extern void Func_0200748e(s32);
extern void Func_020075a8(s32, s32, s32);
extern void Func_020074f0(s32, s32);
extern void Func_020074ee(s32);
extern void Func_020074ae(s32);
extern s32 Func_02007568(s32, s32);
extern u8 *Func_020074f2(s32);
extern void Func_0200759c(s32);
extern void Func_020075c4(s32, s32);
extern u8 *Func_0200750a(s32);
extern void Func_02007526(s32, s32);
extern void Func_020074fa(void);
extern s32 Data_0200d72d[];
extern s32 Data_0200e248[];

void Func_02001490(s32 a, s32 b)
{
    u8 *o;
    s32 v;
    o = Func_02007482(22);
    Func_02007460();
    Func_02007508(22, 2);
    Func_0200757c(22, 0x100, 20);
    Func_0200751c(0, 2);
    Func_02007590(0, 0x102, 40);
    Func_020074c8(0, a);
    Func_0200748e(10);
    {
        s32 t = 0x103;
        Func_020075a8(22, t, 0);
    }
    Func_020074f0(22, b);
    Func_020074ee(0);
    v = 0x10000;
    Func_020074ae(20);
    Func_02007568(22, 2);
    *(s32 *)(o + 24) = v;
    *(s32 *)(o + 28) = v;
    {
        u8 *c = Func_020074f2(0);
        *(s32 *)(c + 24) = v;
        *(s32 *)(c + 28) = v;
    }
    Func_0200759c(0xfce);
    Func_020075c4(22, 0);
    *(s32 *)(Func_0200750a(22) + 0x6c) = (s32)Data_0200d72d;
    Func_02007526(22, (s32)Data_0200e248);
    Func_020074fa();
}
