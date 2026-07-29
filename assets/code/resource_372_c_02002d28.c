typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern void Func_02007428(void);
extern void Func_020074c8(s32, s32);
extern void Func_0200742e(s32);
extern void Func_02007508(s32, s32, s32);
extern void Func_02007508_b(s32, u8 *);
extern s32 Func_02007426(s32);
extern void Func_02007528(s32);
extern void Func_0200754a(s32, s32, s32);
extern void Func_0200753a(s32);
extern void Func_0200751a(s32, s32);
extern void Func_02007564(s32, s32, s32);
extern void Func_0200752c(s32, s32);
extern void Func_02007576(s32, s32, s32);
extern void Func_02007592(s32, s32, s32);
extern void Func_02007532(s32, s32);
extern void Func_02007498(s32);
extern u8 *Func_020074d6(s32);
extern s32 Func_02007404(void);
extern s32 Func_020073ea(s32, s32);
extern void Func_020074be(s32);
extern void Func_020074b4(s32);
extern void Func_020074d8(void);

extern u8 Value_0000030d;
extern u8 Data_0200cec8[];

void Func_02002d28(void)
{
    s32 k2000 = 0x2000;
    s32 kea5 = 0xEA5;
    s32 kea4 = 0xEA4;
    u8 *tbl;

    Func_02007428();
    Func_020074c8(10, 1);
    Func_0200742e(10);
    Func_02007508(10, 0, 20);
    if (Func_02007426((s32)&Value_0000030d) != 0) {
        Func_02007528(kea5);
        Func_0200754a(10, 0, 10);
    } else {
        Func_0200753a(kea4);
        Func_0200751a(10, 1);
        Func_02007564(10, 0, 10);
        Func_0200752c(10, 2);
        Func_02007576(10, 0, 10);
    }
    Func_02007592(10, k2000, 20);
    Func_02007532(10, 5);
    Func_02007498(10);
    {
        u8 *o;
        s32 v;
        o = Func_020074d6(10);
        v = Func_020073ea(Func_02007404(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(o + 0x64) = v;
        Func_02007508_b(10, tbl);
    }
    Func_020074be(20);
    Func_020074b4((s32)&Value_0000030d);
    Func_020074d8();
}
