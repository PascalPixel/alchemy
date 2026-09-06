#include "types.h"

#define RunEventScript01 Func_02001db0

/* Audited 49-call script for the complete 0x02001db0 owner.
 * Recovered from the bounded canonical owner. */

void Func_02002ac4();
void Func_0200582a();
void Func_0200601c();
void Func_02006028();
void Func_02006092(void *, s32);
void *Func_020060a0();
void *Func_020060b6();
void Func_020060c2();
void *Func_020060f4();
void Func_02006126();
void Func_02006146();
void Func_02006154();
void Func_02006164();
void *Func_0200616a();
void Func_02006172();
void *Func_02006176();
void Func_0200617c();
void *Func_02006180();
void *Func_02006186_a();
void *Func_02006186_b();
void Func_02006196();
void Func_020061c6();
void Func_020061c8();
void Func_020061d2();
void Func_020061dc();
void Func_020061e4();
void Func_020061ea();
void Func_020061ee();
void Func_020061f2();
void Func_020061f6();
void Func_020061fe();
void Func_02006200();
void Func_02006206();
void Func_02006208();
void *Func_0200620a_a();
void Func_0200620a_b();
void Func_0200620e();
void Func_02006216();
void *Func_0200623c();
void Func_0200624a();
void Func_0200626c();
void Func_02006286();
void Func_0200628e();
void Func_02006296();
void Func_020062b4();
void Func_02006318();
void Func_0200636c();
void Func_02006374();
void Func_02006378();

void RunEventScript01(void)
{
    void *p8;
    void *p10;
    void *p21;
    void *p22;
    void *p23;

    Func_020060a0();
    Func_020060b6(33608032);
    Func_0200601c(1);
    Func_020060c2(33608200);
    Func_02006028(1);
    Func_02006146(21, 16252928, 47710208);
    Func_02006186_a(0, 15);
    p8 = Func_020060f4(0);
    Func_02006092(p8, 0);
    *(s32 *)(*(u8 **)0x03001ebc + 448) = 514;
    Func_0200623c();
    Func_02006126(21, 104857, 52428);
    Func_02006172(21, 242, 692);
    Func_0200617c(21, 196, 678);
    Func_02006186_b(21, 182, 654);
    Func_020061c6(21, 2);
    Func_020061e4(7748);
    Func_0200582a(40981);
    Func_02006164(0, 157286, 78643);
    Func_02006196(0, 154, 609);
    Func_020062b4(146);
    p21 = Func_0200616a(24);
    *(u16 *)((u8 *)(p21) + 100) = 0;
    p22 = Func_02006176(25);
    *(u16 *)((u8 *)(p22) + 100) = 0;
    p23 = Func_02006180(26);
    *(u16 *)((u8 *)(p23) + 100) = 0;
    Func_020061f2(24, 2097152, 31719424);
    Func_02006200(25, 5505024, 32505856);
    Func_0200620e(26, 1048576, 39059456);
    Func_020061c8(24, 157286, 78643);
    Func_020061d2(25, 157286, 78643);
    Func_020061dc(26, 157286, 78643);
    Func_020061ee(24, 33604844);
    Func_020061f6(25, 33604844);
    Func_020061fe(26, 33604844);
    Func_02006286(24, 3);
    Func_0200628e(25, 3);
    Func_02006296(26, 3);
    do {
        Func_02006154(1);
        p10 = Func_0200620a_a(24);
    } while (*(s16 *)(p10 + 100) == 0);
    Func_02002ac4();
    Func_0200626c(21, 196, 612);
    Func_0200624a(24);
    Func_02006208(10);
    Func_0200636c();
    Func_02006378();
    Func_02006216(10);
    Func_020061ea();
    Func_02006318(21);
    Func_0200620a_b(7749, 1, 0);
    Func_02006206();
    Func_02006374(12);
}
