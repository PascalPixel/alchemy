#include "types.h"

#define Function Func_02000a24

extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 Data_03001ebc[];
extern u8 Data_00000010[];

s32 Func_020025c8();
void Func_020025d2();
void Func_02002630();
void Func_02002640();
s32 Func_02002648();
void Func_02002650();
void Func_02002654();
void Func_0200266c();
void Func_0200268e();
s32 Func_020026c0_a(s32);
void Func_020026c0_b(s32, s32);
void Func_020026cc(s32, s32);
void Func_020026d8();
s32 Func_020026e0();
void Func_020026e4();
void Func_020026f0();
void Func_020026fc();
void Func_020026f6();
void Func_0200270a();
s32 Func_02002712();
s32 Func_0200271e();
s32 Func_0200272a();
s32 Func_02002736();
s32 Func_02002742();
s32 Func_0200274e();
s32 Func_02002756(s32);
s32 Func_0200275a();
void Func_02002708();
void Func_02002714();
void Func_02002720();
void Func_0200272c();
void Func_02002738();
s32 Func_02002766();
s32 Func_02002764(s32);
void Func_0200276c();
s32 Func_02002772();
s32 Func_0200277e();
s32 Func_0200277e_a(s32);
s32 Func_0200278a();
s32 Func_02002796(s32);
s32 Func_020027a6(s32);
s32 Func_020027ae();
s32 Func_020027ba();
s32 Func_020027c6();
s32 Func_020027d2();
s32 Func_020027de();
void Func_020027e6();
void Func_02002828();
void Func_020027ea();
void Func_020027f2();
s32 Func_0200280c();
s32 Func_02002818();
s32 Func_02002830();
s32 Func_0200283c();
s32 Func_02002800();
void Func_02002800_a();
s32 Func_02002808(s32);
void Func_0200280a();
void Func_0200281c();
void Func_0200281c_a(void);
s32 Func_02002824();
s32 Func_02002824_a(s32);
void Func_02002826();
void Func_02002838();
s32 Func_0200244a();
void Func_020022a0();
void Func_0200185a();
void Func_02001b5e();

static __inline__ s32 Value1(s32 (*f)(), s32 a0) { return f(a0); }
static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2,
                             s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Function(void)
{
    s32 selector;
    s32 actor;
    u8 *work = *(u8 **)Data_03001ebc;
    s32 off = 448;

    *(s32 *)(work + off) = 0x204;
    if (Value1(Func_020025c8, 0x814) != 0) {
        Func_0200270a(141);
        Call3(Func_020025d2, 0x10000, 0x10000, 0x10000);
        Func_020026f6();
    }

    selector = Data_02000240_t[225][0] - 1;
    if ((u32)selector > 15)
        return;

    switch (selector) {
    case 0:
    case 1:
        if (Value1(Func_02002648, 0x81a) == 0)
            return;
        Call6(Func_02002630, 1, 109, 4, 81, 1, 1);
        Call6(Func_02002640, 0, 70, 30, 42, 1, 1);
        Call6(Func_02002654, 0, 29, 3, 1, 3, 2);
        Call6(Func_0200266c, 0, 29, 3, 2, 3, 1);
        Func_02002650();
        break;

    case 2:
        actor = Func_020026e0(9);
        Func_0200268e(actor, 0);
        break;

    case 7:
        Data_02000240_t[288][0] = (s16)Data_00000010;
        Data_02000240_t[289][0] = 8;
        if (Value1(Func_020026c0_a, 0x802) != 0)
            return;
        Func_0200185a();
        break;

    case 10:
    case 11:
    case 12:
        actor = Func_02002712(9);  Func_020026c0_b(actor, 0);
        actor = Func_0200271e(10); Func_020026cc(actor, 0);
        actor = Func_0200272a(11); Func_020026d8(actor, 0);
        actor = Func_02002736(12); Func_020026e4(actor, 0);
        actor = Func_02002742(13); Func_020026f0(actor, 0);
        actor = Func_0200274e(14); Func_020026fc(actor, 0);
        actor = Func_0200275a(15); Func_02002708(actor, 0);
        actor = Func_02002766(16); Func_02002714(actor, 0);
        actor = Func_02002772(17); Func_02002720(actor, 0);
        actor = Func_0200277e(18); Func_0200272c(actor, 0);
        actor = Func_0200278a(19); Func_02002738(actor, 0);
        if (Value1(Func_02002756, 0x804) == 0)
            Func_02001b5e();
        if (Value1(Func_02002764, 0x303) != 0)
            Call3(Func_020027e6, 9, 0x5d80000, 0x880000);
        else if (Value1(Func_0200277e_a, 0x302) != 0)
            Call3(Func_02002800_a, 9, 0x5f80000, 0x880000);
        if (Value1(Func_02002796, 0x301) != 0)
            Call3(Func_02002828, 10, 0x7180000, 0x880000);
        else if (Value1(Func_020027a6, 0xc0 << 2) != 0)
            Call3(Func_02002828, 10, 0x7380000, 0x880000);
        break;

    case 13:
    case 14:
    case 15:
        actor = Func_02002800(9);  Func_020027ae(actor, 0);
        actor = Func_0200280c(10); Func_020027ba(actor, 0);
        actor = Func_02002818(11); Func_020027c6(actor, 0);
        actor = Func_02002824(12); Func_020027d2(actor, 0);
        actor = Func_02002830(13); Func_020027de(actor, 0);
        actor = Func_0200283c(14); Func_020027ea(actor, 0);
        if (Value1(Func_02002808, 0x825) == 0)
            Func_020022a0();
        Func_0200244a(1);
        Func_02002826(141 * 4);
        if (Value1(Func_02002824_a, 0x821) != 0) {
            Call6(Func_0200280a, 0, 71, 100, 71, 1, 1);
            Call6(Func_0200281c, 122, 20, 120, 30, 1, 2);
            Call6(Func_02002838, 122, 20, 1, 2, 120, 30);
            Func_0200281c_a();
        }
        break;

    default:
        break;
    }
}
