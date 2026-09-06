#include "types.h"

#define RunEventScript01 Func_0200069c

/* Retained candidate for the complete 636-byte owner. The three pointer
 * queries conditionally forward the record's +8 and +16 words. */

void Func_02001c08();
void Func_02001c66();
void Func_02001ccc();
void *Func_0200216a();
void *Func_02002188();
void *Func_020021d2();
void *Func_020021e0();
void Func_020021ee();
void *Func_020021fa();
void Func_020021fc();
void Func_02002204();
void *Func_02002214();
void Func_02002226();
u8 *Func_0200222a();
u8 *Func_0200223e();
u8 *Func_02002256();
void Func_02002258();
void Func_0200226a();
void Func_0200226c();
void *Func_0200227c();
void Func_02002284();
void Func_02002284_a();
void Func_0200228c();
void Func_0200228e();
void Func_0200229c();
void Func_020022a4();
void Func_020022a6();
void Func_020022a8();
void Func_020022b6();
void *Func_020022bc();
void Func_020022ea();
void Func_020022f8();
void Func_0200230e();
void Func_0200231a();
void Func_02002324();
void Func_02002326();
void Func_0200232c();
void Func_02002332();
void Func_02002334();
void Func_0200233c();
void Func_0200234c();
void Func_0200237a();
void Func_02002384();
void Func_0200238c();
void Func_02002394();
void Func_02002398();
void Func_020023a4();
void Func_020023a4_a();
void Func_020023b0();
void Func_020023b4();
void Func_020023bc();
void Func_020023cc();
void Func_020023d4();
void Func_02002406();
void Func_02002410();
void Func_02002410_a();
void Func_0200241a();
void Func_02002424();
void Func_02002490();
void Func_02002490_a();

void RunEventScript01(void)
{
    void *p1;
    void *p11;
    void *p13;
    u8 *record;
    u8 *work;

    p1 = Func_0200216a(3);
    Func_02002188();
    Func_020022a6(17);
    Func_02002204(5326);
    Func_02002226(32777, 0, 20);
    Func_020022bc(29);
    Func_020021d2(0, 65536, 32768);
    Func_020021e0(1, 65536, 32768);
    Func_020021ee(2, 65536, 32768);
    Func_020021fc(3, 65536, 32768);
    p11 = Func_020021fa(3);
    *(u8 *)((u8 *)(p11) + 35) &= 0xfe;
    Func_0200228e(3, 2);
    p13 = Func_02002214(0);
    *(u8 *)((u8 *)(p13) + 35) &= 0xfe;
    Func_020022a4(0, 2);
    record = Func_0200222a(0);
    if (record != 0) {
        Func_02002258(1, *(u32 *)(record + 8), *(u32 *)(record + 16));
    }
    record = Func_0200223e(0);
    if (record != 0) {
        Func_0200226c(2, *(u32 *)(record + 8), *(u32 *)(record + 16));
    }
    if (p1 != 0) {
        record = Func_02002256(0);
        if (record != 0) {
            Func_02002284(3, *(u32 *)(record + 8), *(u32 *)(record + 16));
        }
        Func_0200227c(3, 33594924);
    }
    Func_02002284_a(0, 33594804);
    Func_0200228c(1, 33594844);
    Func_0200229c(2, 33594884);
    Func_0200226a(10);
    Func_0200230e(0, 49152, 0);
    Func_0200231a(1, 49152, 0);
    Func_02002326(3, 49152, 0);
    Func_02002332(2, 49152, 40);
    Func_020022ea(8, 11);
    Func_020022a8(10);
    Func_020022f8(8, 8);
    Func_020022b6(20);
    Func_02001c08(8);
    Func_0200234c(32776, 0);
    Func_02002324(0, 2);
    Func_0200232c(1, 2);
    Func_02002334(3, 2);
    Func_0200233c(2, 2);
    Func_02002398(0, 256, 0);
    Func_020023a4(1, 256, 0);
    Func_020023b0(3, 256, 0);
    Func_020023bc(2, 256, 60);
    Func_02001c66(11);
    Func_020023b4(32776, 0, 10);
    Func_02002384(0, 1);
    Func_0200238c(1, 1);
    Func_02002394(3, 1);
    Func_020023a4_a(2, 1);
    Func_020023d4(32776, 0);
    Func_02002406(0, 258);
    Func_02002410(1, 258);
    Func_0200241a(3, 258);
    Func_02002424(2, 258);
    Func_0200237a(40);
    Func_02001ccc(11);
    Func_02002410_a(32776, 0);
    work = *(u8 **)0x03001ebc;
    *(u32 *)(work + 0x1c0) = 0x200;
    *(u32 *)(work + 0x188) = 0x40;
    *(u8 *)0x0200036c = 3;
    Func_02002490(7213, 19);
    Func_02002490_a(36, 0);
    Func_020023cc();
}
