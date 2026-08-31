#include "types.h"

#define FieldScene_RunPrimarySequence Func_020010de

/* Audited retained field-scene body at 0x020010de.
 * The complete production body preserves 54 decoded calls, 0 loop(s), and 16 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void *Func_02002d4e();
void *Func_02002d60();
void Func_02002d74();
s32 Func_02002d90();
void *Func_02002da8();
void Func_02002dac();
void Func_02002dd0();
void Func_02002dde();
void Func_02002dfc();
s32 Func_02002e34();
void Func_02002e3c();
void Func_02002e48();
void Func_02002e64();
void Func_02002e7a();
void Func_02002e94();
void Func_02002e9e();
void Func_02002ecc();
void *Func_02002edc();
void Func_02002ede();
s32 Func_02002eec();
void Func_02002ef0();
s32 Func_02002ef6();
void Func_02002ef8();
void Func_02002f04();
void Func_02002f08();
void Func_02002f0c();
void Func_02002f0e();
void Func_02002f10();
void Func_02002f2a();
void Func_02002f3a();
void Func_02002f5a();
void Func_02002f68();
void Func_02002f76();
void Func_02002f78();
void Func_02002f8e();
void Func_02002f94();
s32 Func_02002f96();
void Func_02002f9c();
void *Func_02002fa2();
void Func_02002faa();
void Func_02002fac();
void *Func_02002fe8();
void *Func_02002ff0();
void Func_02002ff4();
void *Func_02002ffa();
void *Func_02003002();
void Func_02003030();

void Func_020010de(void)
{
    void *p1;
    void *p27;
    void *p4;
    void *p47;
    void *p5;
    void *p50;
    void *p51;
    void *p52;
    void *p53;

    p1 = Func_02002da8(0);
    if (Func_02002d90() == 0) {
        Func_02002dd0();
        p4 = Func_02002d60();
        p5 = Func_02002d4e(6);
        Func_02002e94(152);
        Func_02002d74();
        *(s32 *)(p1 + 48) = 196608;
        *(s32 *)(p1 + 52) = 131072;
        *(s32 *)(p1 + 40) = 262144;
        *(u8 *)((u8 *)(p1) + 85) &= 0x7e;
        Func_02002dde();
        Func_02002e3c(0);
        Func_02002dac();
        Func_02002dfc();
        *(u8 *)((u8 *)(p1) + 85) = 0;
        Func_02002e3c();
    } else {
    }
    if (Func_02002e34(512) == 0) {
        Func_02002e48(512);
        Func_02002e64();
        Func_02002ef8(65536, 8192);
        Func_02002ef8(8, 1);
        Func_02002f0c();
        Func_02002e7a(60);
        Func_02002ef6(8, 49152, 20);
        Func_02002f10(8, 258);
        Func_02002ef8(8, 2);
        Func_02002e9e(20);
        Func_02002ecc(8, 65536, 32768);
        Func_02002ef0(8, 792, 248);
        Func_02002f76(152);
        p27 = Func_02002edc(8);
        *(s32 *)(p27 + 40) = 524288;
        Func_02002f10(8, 792, 280, 524288);
        Func_02002ede(20);
        Func_02002f5a(8, 49152, 20);
        Func_02002ef0(30);
        Func_02002f04();
    }
    if (Func_02002eec(512) != 0 && Func_02002ef6(513) == 0) {
        Func_02002f08(513);
        Func_02002f0e(770);
        Func_02002f2a();
        Func_02002fac(8, 258);
        Func_02002f94(8, 2);
        Func_02002f3a(20);
        Func_02002f68(8, 131072, 65536);
        Func_02002f8e(8, 760, 280);
        Func_02002f9c(8, 760, 312);
        Func_02002faa(8, 792, 312);
        Func_02002f78(10);
        Func_02002ff4(8, 49152, 20);
        p47 = Func_02002fa2(8);
        *(s32 *)(p47 + 108) = 33590521;
        Func_02002fa2();
    }
    if (Func_02002f96(770) != 0) {
        p50 = Func_02002fe8(8);
        p51 = Func_02002ff0(0);
        *(s32 *)(p50 + 48) = *(s32 *)(p51 + 48);
        p52 = Func_02002ffa(8);
        p53 = Func_02003002(0);
        *(s32 *)(p52 + 52) = *(s32 *)(p53 + 52);
        Func_02003030(8);
    }
}
