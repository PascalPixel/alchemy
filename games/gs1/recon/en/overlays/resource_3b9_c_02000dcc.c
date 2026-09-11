#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for Scene_RunLargeActorPresentationSetup:
 * 101 calls, no loops, two actor-flag updates, and a complete nine-field
 * presentation-record reset across the full callable owner. */

#define Scene_RunLargeActorPresentationSetup Func_02000dcc

void Func_02002a6e();
void Func_02002ad2();
void Func_02002ad4();
void Func_02002af2();
void Func_02002afa();
void Func_02002b16();
void Func_02002b32();
void Func_02002b40();
void Func_02002b4c();
void Func_02002b56();
void Func_02002b5c();
void Func_02002b74();
void Func_02002b7e();
void Func_02002c9a();
void Func_02002cf4();
s32 Func_02003996();
void Func_020039aa();
void Func_020039de();
void Func_02003a0c();
void Func_02003a50();
void Func_02003a54();
s32 Func_02003a5e();
void Func_02003ac0();
void Func_02003ace();
void Func_02003ade();
void Func_02003aee();
void Func_02003af6();
void Func_02003af8();
void Func_02003afa();
void *Func_02003b10();
void Func_02003b12();
void Func_02003b12_a();
void Func_02003b14();
void Func_02003b18();
void Func_02003b1a();
void Func_02003b1c();
void Func_02003b44();
void Func_02003b4a();
void Func_02003b4c();
void Func_02003b60();
void Func_02003b62();
void Func_02003b7c();
void Func_02003b7c_a();
void Func_02003ba4();
void Func_02003bb4();
void Func_02003bb6();
void Func_02003bbe();
void Func_02003bbe_a();
void Func_02003bc6();
void Func_02003bc8();
void Func_02003bd6();
void Func_02003bd8();
void Func_02003be0();
void Func_02003be2();
void Func_02003bea();
void Func_02003bee();
void Func_02003bf6();
void Func_02003c02();
void Func_02003c26();
void Func_02003c30();
void Func_02003c30_a();
void Func_02003c3a();
void Func_02003c54();
void Func_02003c6c();
void *Func_02003c70();
void Func_02003c72();
void Func_02003c82();
void Func_02003c90();
void *Func_02003c92();
void Func_02003c9a();
void Func_02003c9a_a();
void Func_02003ca4();
void Func_02003cae();
void *Func_02003cb0();
void Func_02003cb8();
void Func_02003cc2();
void Func_02003cc4();
void Func_02003cc6();
void Func_02003cc8();
void Func_02003cd0();
void Func_02003cd2();
void Func_02003cda();
void Func_02003ce0();
void Func_02003cee();
void Func_02003cee_a();
void Func_02003cfc();
void Func_02003cfc_a();
void Func_02003d0a();
void Func_02003d0a_a();
void Func_02003d18();
void Func_02003d18_a();
void Func_02003d24();
void Func_02003d26();
void Func_02003d26_a();
void Func_02003d30();
void Func_02003d34();
void Func_02003d42();
void Func_02003d50();
void Func_02003d5a();
void Func_02003dfe();
void Func_02003e3c();

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02000dcc(void)
{
    s32 flag;
    void *p75;
    void *p78;
    void *p8;
    void *p81;

    if (Func_02003996(564) == 0) {
    } else {
        Func_020039aa(565);
        Func_020039de();
        Call3(Func_02003a0c, 0, 0x10000, 0x8000);
        Func_02003a50(0, 872, 120);
        Func_02002a6e(0, 40960);
        Func_02003af6(0x19999, 0x3333);
        Call4(Func_02003b10, 0x3300000, -1, 0x700000, 1);
        Func_02003b1c();
        Call3(Func_02003af8, 8, 0x3000, 20);
        Func_02003ac0(8, 1);
        Func_02003ade(8485);
        Call2(Func_02003aee, 0x8008, 0);
        flag = Func_02003a5e(0, 0);
        if (flag == 0) {
        Func_02003a54(20);
        Func_02002ad2(12, 53248);
        Func_02003afa(12, 1);
        Func_02003b18(8491);
        Func_02002ad4(16396);
        Func_02002af2(17, 0);
        Func_02002afa(0, 0x8000);
        Func_02003b12(17, 3);
        Func_02003b1a(0, 3);
        Func_02003b12_a(15, 1);
        Func_02003b7c(15, 53248, 20);
        Func_02003b44(15, 1);
        Func_02002b16(15);
        Func_02002b32(16, 0x8000);
        Func_02003b4a(16, 3);
        Func_02003b62(17, 2);
        Func_02002b4c(17, 40960);
        Func_02002b40(16401);
        Func_02002b5c(18, 45056);
        Func_02003b7c_a(18, 2);
        Func_02002b56(16402);
        Call2(Func_02002b74, 11, 0x3000);
        Func_02003bee(11, 258);
        Func_02003b14(60);
        Func_02003ba4(11, 2);
        Func_02002b7e(32779);
        Func_02003b60(13);
        Func_02003ace(1);
        Func_02003bbe(13, 2);
        Func_02003bc6(14, 2);
        Func_02003bd6(16, 2);
        Func_02003b4c(20);
        Func_02003c26(13, 0, 0);
        Call3(Func_02003c30, 14, 0x8000, 0);
        Func_02003c3a(16, 45056, 40);
        Func_02003be2(13, 3);
        Func_02003bea(14, 3);
        Func_02003c02(16, 3);
        Func_02003bb4(13, 104857, 52428);
        Func_02003bbe_a(14, 104857, 52428);
        Func_02003bc8(16, 104857, 52428);
        Func_02003bd8(13, 33599008);
        Func_02003be0(16, 33599188);
        Func_02003bb6(20);
        Func_02003c90(15, 53248, 0);
        Func_02003c9a(17, 45056, 0);
        Func_02003ca4(0, 40960, 0);
        Func_02003cae(12, 53248, 0);
        Func_02003cb8(18, 45056, 0);
        Func_02003c30_a(14, 33599068);
        Func_02003bf6(20);
        Func_02003cd0(8, 0, 0);
        Call3(Func_02003cda, 11, 0x8000, 40);
        Func_02003c82(8, 3);
        Func_02003c9a_a(11, 3);
        Call4(Func_02003d24, 0x3280000, -1, 0x560000, 1);
        Func_02003d30();
        Func_02003cc8(8, 2);
        Func_02002c9a(8);
        Func_02003c72(8, 65536, 32768);
        p75 = Func_02003c70(8);
        *(u8 *)((u8 *)(p75) + 90) &= 0xfe;
        Func_02003cc6(8, 792, 72);
        Func_02003c6c(1);
        p78 = Func_02003c92(8);
        *(u8 *)((u8 *)(p78) + 90) |= 0x1;
        Func_02002cf4(8, 0);
        Func_02003cc2(13);
        p81 = Func_02003cb0(13);
        *(s32 *)(p81 + 108) = flag;
        *(u16 *)((u8 *)(p81) + 100) = flag;
        *(u16 *)((u8 *)((u8 *)(p81) + 100) + 2) = flag;
        *(s32 *)(p81 + 36) = flag;
        *(s32 *)(p81 + 40) = flag;
        *(s32 *)(p81 + 44) = flag;
        *(s32 *)(p81 + 56) = 2147483648;
        *(s32 *)(p81 + 60) = 2147483648;
        *(s32 *)(p81 + 64) = 2147483648;
        Func_02003c54(1);
        Func_02003cee(15, 33599268);
        Func_02003cc4(20);
        Func_02003cfc(13, 33599268);
        Func_02003cd2(20);
        Func_02003d0a(17, 33599268);
        Func_02003ce0(20);
        Func_02003d18(14, 33599268);
        Func_02003cee_a(20);
        Func_02003d26(16, 33599268);
        Func_02003cfc_a(20);
        Func_02003d34(12, 33599268);
        Func_02003d0a_a(20);
        Func_02003d42(18, 33599268);
        Func_02003d18_a(60);
        Func_02003d50(0, 33599268);
        Func_02003d26_a(80);
        Func_02003e3c(66);
        Call2(Func_02003dfe, 0x8008, 0);
        Func_02003d5a();
        }
    }
}
