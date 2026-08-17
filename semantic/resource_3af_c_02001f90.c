#include "types.h"

extern void Func_02006280(void);
extern void Func_02006340(s32, s32);
extern s32 Func_020062ae(s32);
extern void Func_0200624c(s32, s32);
extern void Func_020062aa(s32);
extern void Func_02006210(s32);
extern void Func_020062b6(s32);
extern void Func_0200621c(s32);
extern void Func_0200633c(s32, s32);
extern s32 Func_020062da(s32);
extern void Func_02006278(s32, s32);
extern s32 Func_020062e6(s32);
extern void Func_02006284(s32, s32);
extern s32 Func_020062f2(s32);
extern void Func_02006290(s32, s32);
extern s32 Func_020062fe(s32);
extern void Func_0200629c(s32, s32);
extern s32 Func_0200630a(s32);
extern void Func_020062a8(s32, s32);
extern s32 Func_02006316(s32);
extern void Func_020062b4(s32, s32);
extern void Func_0200638a(s32, s32, s32);
extern void Func_0200634c(s32, u8 *);
extern void Func_020063a2(s32, s32, s32);
extern void Func_02006362(s32, u8 *);
extern void Func_020063b8(s32, s32, s32);
extern void Func_020063c6(s32, s32, s32);
extern void Func_020063d4(s32, s32, s32);
extern void Func_020063e0();
extern u8 *Func_02006386(s32);
extern u8 *Func_02006392(s32);
extern u8 *Func_0200639e(s32);
extern u8 *Func_020063a8(s32);
extern void Func_020063ce(s32, u8 *);
extern void Func_020063d6(s32, u8 *);
extern void Func_020063e8(s32, u8 *);
extern void Func_0200643a(s32, s32, s32);
extern void Func_0200651c(void);
extern void Func_02006530(void);
extern void Func_020063d0(s32);
extern void Func_02006466(s32, s32, s32);
extern void Func_02006472(s32, s32, s32);
extern void Func_0200642c(s32, s32, s32);
extern void Func_02006436(s32, s32, s32);
extern void Func_0200646a(s32, s32, s32);
extern void Func_0200647e(s32, s32, s32);
extern void Func_0200645c(s32, s32, s32);
extern void Func_02006490(s32, s32, s32);
extern void Func_020064a4(s32, s32, s32);
extern void Func_020065ba(s32);
extern void Func_0200648c(s32, u8 *);
extern void Func_02006494(s32, u8 *);
extern void Func_020065d2(s32);
extern void Func_020064ee(s32, s32, s32);
extern void Func_020064ae(s32, s32);
extern void Func_02006474();
extern void Func_0200650a(s32, s32, s32);
extern void Func_020064c8(s32, s32, s32);
extern s32 *Func_020064be(s32);
extern void Func_02006510(s32, s32, s32);
extern s32 Func_020064d6(s32);
extern void Func_020064ba(s32);
extern void Func_02006500(s32, s32, s32);
extern void Func_0200653c(s32, s32, s32);
extern void Func_02005bfa(s32, s32);
extern void Func_02002db6(void);
extern void Func_020064e8(s32);
extern void Func_02006532(s32, u8 *);
extern void Func_020064f8(s32);
extern void Func_02006540();
extern void Func_02006506(s32);
extern void Func_02006566(s32, u8 *);
extern void Func_02006514();
extern void Func_02006692(s32);
extern void Func_0200667e(void);
extern void Func_0200668a(void);
extern void Func_02006578(s32);
extern void Func_0200657e(s32);
extern void Func_02006584(s32);
extern void Func_0200658a(s32);
extern void Func_02006642(s32);
extern void Func_02006534(s32, s32, s32);
extern void Func_0200669e(s32);

extern u8 Data_0200d160[];
extern u8 Data_0200d268[];
extern u8 Data_0200c80c[];
extern u8 Data_0200c7a8[];
extern u8 Data_0200c764[];
extern u8 Data_0200c7ec[];
extern u8 Data_0200c814[];
extern u8 Data_0200c888[];
extern s32 *Data_03001ebc;

void Func_02001f90(void)
{
    Func_02006280();
    Func_02006340(0, 15);
    Func_0200624c(Func_020062ae(0), 0);
    Func_020062aa((s32)Data_0200d160);
    Func_02006210(1);
    Func_020062b6((s32)Data_0200d268);
    Func_0200621c(1);
    Func_0200633c(31, 0);
    Func_02006278(Func_020062da(24), 1);
    Func_02006284(Func_020062e6(25), 1);
    Func_02006290(Func_020062f2(26), 1);
    Func_0200629c(Func_020062fe(27), 1);
    Func_020062a8(Func_0200630a(28), 1);
    Func_020062b4(Func_02006316(29), 1);
    Func_0200638a(22, 0x1000000, 0x2800000);
    {
        u8 *t = Data_0200c80c;
        Func_0200634c(22, t);
        Func_020063a2(21, 0x10C0000, 0x2B40000);
        Func_02006362(22, t);
    }
    Func_020063b8(24, 0xF20000, 0x25C0000);
    Func_020063c6(25, 0x1080000, 0x2580000);
    Func_020063d4(26, 0xFE0000, 0x29C0000);
    Func_020063e0(27, 0x11A0000, 0x2920000);
    {
        s32 z = 0;
        s32 o;
        *(Func_02006386(24) + 99) = z;
        o = 1;
        *(Func_02006392(25) + 99) = o;
        *(Func_0200639e(26) + 99) = z;
        *(Func_020063a8(27) + 99) = o;
    }
    {
        u8 *t = Data_0200c7a8;
        Func_020063ce(24, t);
        Func_020063d6(25, t);
    }
    {
        u8 *t = Data_0200c764;
        Func_020063e0(26, t);
        Func_020063e8(27, t);
    }
    Func_0200643a(20, 0, 0);
    Data_03001ebc[0x70] = 0x202;
    Func_0200651c();
    Func_02006530();
    Func_020063d0(400);
    Func_02006466(28, 0xFE0000, 0x2E40000);
    Func_02006472(29, 0x180000, 0x24A0000);
    Func_0200642c(28, 0x19999, 0xCCCC);
    Func_02006436(29, 0x19999, 0xCCCC);
    Func_0200646a(29, 172, 0x284);
    Func_0200647e(28, 200, 0x294);
    Func_0200645c(0, 0x40000, 0x20000);
    Func_02006490(0, 174, 0x26C);
    Func_020064a4(28, 180, 0x244);
    Func_020065ba(146);
    {
        u8 *t = Data_0200c7ec;
        Func_0200648c(28, t);
        Func_02006494(29, t);
    }
    Func_020065d2(240);
    Func_020064ee(31, 0x860000, 0x2520000);
    Func_020064ae(31, (s32)Data_0200c814);
    Func_02006474(10);
    Func_0200650a(30, 0x860000, 0x2480000);
    Func_020064c8(30, 0x40000, 0x20000);
    {
        s32 x = 0x80;
        Func_020064be(30)[10] = x << 12;
    }
    Func_02006510(30, 186, 0x264);
    Func_02006474(Func_020064d6(30), 1);
    Func_020064ba(10);
    Func_02006500(30, 0x20000, 0x10000);
    Func_0200653c(30, 216, 0x258);
    Func_02005bfa(30, 0xC000);
    Func_02002db6();
    Func_020064e8(10);
    {
        u8 *t = Data_0200c888;
        Func_02006532(30, t);
        Func_020064f8(10);
        Func_02006540(28, t);
        Func_02006506(10);
        Func_02006566(29, t);
    }
    Func_02006514(20);
    Func_02006692(147);
    Func_0200667e();
    Func_0200668a();
    Func_02006578(24);
    Func_0200657e(25);
    Func_02006584(26);
    Func_0200658a(27);
    Func_02006540(10);
    Func_02006514();
    Func_02006642(21);
    Func_02006534(0x1E45, 1, 0);
    Func_02006530();
    Func_0200669e(13);
}
