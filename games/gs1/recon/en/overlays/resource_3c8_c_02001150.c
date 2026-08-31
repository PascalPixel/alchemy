#include "types.h"

#define FieldScene_RunPrimarySequence Func_02001150

/* Audited retained field-scene body at 0x02001150.
 * This reconstruction preserves 72 decoded calls, 0 loop(s), and 11 explicit memory operation(s)
 * across the complete production body. The approved compiler changes its
 * scheduling, allocation, control-flow lowering, and literal placement. */

void Func_02005ff0();
void *Func_02006002();
void Func_02006020();
void Func_0200602e();
void Func_02006036();
void *Func_0200603c();
void Func_0200604e();
void *Func_02006050();
void Func_02006064();
void *Func_02006074();
void Func_02006098();
void Func_020060a4();
void Func_020060a6();
void Func_020060ae();
void Func_020060c6();
void Func_020060ec();
void Func_020060f2();
void Func_020060f8();
s32 Func_020060fa();
void Func_020060fc();
s32 Func_02006104();
void Func_02006106();
void Func_0200610c();
void Func_02006116();
void Func_02006120();
s32 Func_02006126();
void Func_02006128();
void Func_0200612c();
void Func_02006136();
void Func_02006138();
void Func_02006146();
void Func_02006154();
void Func_02006156();
void Func_02006166();
void Func_02006176();
void Func_02006186();
void Func_02006196();
void Func_020061a6();
void Func_020061c8();
void Func_020061dc();
void Func_020061ec();
void Func_020061fc();
void Func_02006200();
void Func_0200620c();
void Func_0200620e();
void Func_02006216();
void Func_0200621c();
void Func_02006220();
void Func_0200622c();
void Func_02006236();
void Func_0200623c();
void Func_0200624c();
void Func_0200625c();
void Func_02006262();
void Func_0200626c();
void Func_0200627c();
void Func_02006302();
void Func_02006328();
void Func_02006350();
void Func_020063fe();
void Func_02006416();
void Func_02006422();
void Func_0200642c();
void Func_02006430();
void Func_0200643c();

void Func_02001150(void)
{
    void *p1;
    void *p11;
    void *p7;
    void *p8;

    p1 = Func_02006002(0);
    Func_02005ff0();
    Func_02006020(0, 33608220);
    Func_0200602e(0);
    Func_020060a6(0, 6);
    *(s32 *)(p1 + 40) = 262144;
    Func_02006036(0, 262144, 131072);
    p7 = Func_0200603c(0);
    *(u8 *)((u8 *)(p7) + 90) &= 0xfe;
    p8 = Func_02006050(0);
    *(u8 *)((u8 *)(p8) + 90) &= 0xfe;
    Func_02006098(0);
    Func_0200604e(1);
    p11 = Func_02006074(0);
    *(u8 *)((u8 *)(p11) + 90) |= 0x1;
    Func_02006064(20);
    *(s32 *)(p1 + 108) = 33588709;
    Func_02006154(0, 258, 60, 33588709);
    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);
    *(s32 *)(p1 + 108) = 0;
    Func_020060a4();
    Func_020060c6();
    Func_02006136(0, 1);
    Func_020060ae(9864, 1);
    Func_02006200(65536, 0);
    Func_02006200(65541, 0);
    Func_02006216(120);
    Func_020060ec(100);
    Func_02006262(142);
    Func_020060f8(30);
    Func_02006220(32767, 0);
    Func_02006236(60);
    Func_0200610c(70);
    if (Func_020060fa(2434) == 0 && Func_02006104(2435) == 0) {
        Func_02006120(2434);
        Func_02006128(2435);
    }
    if (Func_02006126(2434) == 0) {
        Func_02006138(2434);
        Func_02006146(2435);
        Func_020060f2(103, 27, 89, 27, 7, 8);
        Func_02006106(41, 90, 27, 92, 3, 2);
        Func_02006116(41, 90, 29, 93, 3, 2);
        Func_02006126(41, 90, 27, 94, 3, 2);
        Func_02006136(41, 90, 27, 96, 3, 2);
        Func_02006146(41, 90, 29, 97, 3, 2);
        Func_02006156(41, 96, 25, 91, 3, 2);
        Func_02006166(41, 92, 25, 93, 3, 2);
        Func_02006176(41, 96, 25, 95, 3, 2);
        Func_02006186(41, 96, 25, 97, 3, 2);
        Func_02006196(41, 96, 27, 96, 3, 2);
        Func_020061a6(41, 96, 29, 97, 3, 2);
    } else {
        Func_0200620e(2435);
        Func_0200621c(2434);
        Func_020061c8(111, 27, 89, 27, 7, 8);
        Func_020061dc(41, 90, 25, 91, 3, 2);
        Func_020061ec(41, 90, 25, 93, 3, 2);
        Func_020061fc(41, 90, 25, 95, 3, 2);
        Func_0200620c(41, 90, 25, 97, 3, 2);
        Func_0200621c(41, 90, 27, 96, 3, 2);
        Func_0200622c(41, 90, 29, 97, 3, 2);
        Func_0200623c(41, 94, 27, 92, 3, 2);
        Func_0200624c(41, 96, 29, 93, 3, 2);
        Func_0200625c(41, 94, 27, 94, 3, 2);
        Func_0200626c(41, 96, 27, 96, 3, 2);
        Func_0200627c(41, 96, 29, 97, 3, 2);
    }
    Func_02006416(65536, 0);
    Func_0200642c(20);
    Func_02006302(40);
    Func_020063fe(32768, 4096);
    Func_02006416(29884416, -1, 35520512, 1);
    Func_02006422();
    Func_02006328(50);
    Func_02006430(29884416, -1, 27721728, 1);
    Func_0200643c();
    Func_02006350();
}
