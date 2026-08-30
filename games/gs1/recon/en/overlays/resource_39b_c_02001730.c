#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunActorExchangeChoreography:
 * all 53 calls, the two-pass exchange, and fourteen actor/effect memory
 * operations across the complete actor-3/8 presentation owner. */

#define FieldScene_RunActorExchangeChoreography Func_02001730

void *Func_02003c18();
void *Func_02003c2e();
void *Func_02003c38();
void *Func_02003c40();
void Func_02003c82();
void Func_02003c90();
void Func_02003c9a();
void Func_02003c9e();
void Func_02003cac();
void Func_02003cba();
void *Func_02003cc2();
void Func_02003cd8();
void Func_02003d08();
void Func_02003d0a();
void Func_02003d14();
void *Func_02003d18();
void Func_02003d1a();
void Func_02003d32();
void Func_02003d36();
void Func_02003d3c();
void *Func_02003d46();
void Func_02003d4a();
void Func_02003d4c();
void Func_02003d6a();
void Func_02003d74();
void Func_02003d78();
void Func_02003d7c();
void Func_02003d7e();
void Func_02003d82();
void *Func_02003d92();
void Func_02003d94();
void Func_02003d98();
void Func_02003daa();
void Func_02003dac();
void Func_02003db4();
void Func_02003dc4();
void Func_02003dc6();
void *Func_02003dc8();
void Func_02003dde();
void Func_02003de8();
void *Func_02003dee();
void Func_02003df2();
void Func_02003df6();
void Func_02003e1a();
void Func_02003e20();
void Func_02003e34();
void Func_02003e70();
void Func_02003eaa();
void Func_02003eb8();

void Func_02001730(void)
{
    void *p1;
    void *p16;
    void *p2;
    void *p24;
    void *p29;
    void *p3;
    void *p38;
    void *p4;
    void *p44;
    void *p47;
    s32 i1;

    p1 = Func_02003c18();
    p2 = Func_02003c2e(12);
    *(s32 *)(p2 + 24) = 4294901760;
    p3 = Func_02003c38(13);
    *(s32 *)(p3 + 24) = 4294901760;
    p4 = Func_02003c40(14);
    *(s32 *)(p4 + 24) = 4294901760;
    Func_02003c90(3, 8912896, 12058624);
    Func_02003c9e(0, 8912896, 19398656);
    Func_02003cac(8, 8912896, 9961472);
    Func_02003c82(3, 98304, 49152);
    Func_02003c90(8, 98304, 49152);
    Func_02003c9a(0, 52428, 26214);
    Func_02003d32(52428, 6553);
    Func_02003d4c(8912896, -1, 12058624, 0);
    Func_02003d98();
    Func_02003dac();
    Func_02003d14(3, 1);
    p16 = Func_02003cc2(8);
    *(u8 *)((u8 *)(p16) + 90) &= 0xfe;
    Func_02003cba(20);
    for (i1 = 1; i1 >= 0; i1--) {
        Func_02003d0a(3, 152, 168, 0);
        Func_02003cd8(10);
        Func_02003d08(8, 33597640);
        Func_02003d36(3);
        Func_02003d82(3, 49152, 30);
        Func_02003d6a(3, 1);
        p24 = Func_02003d18(3);
        *(u8 *)((u8 *)(p24) + 90) &= 0xfe;
        Func_02003d4c(3, 136, 184);
        Func_02003d1a(10);
        Func_02003d4a(8, 33597472);
        Func_02003d78(3);
        p29 = Func_02003d46(3);
        *(u8 *)((u8 *)(p29) + 90) |= 0x1;
        Func_02003d3c(30);
        Func_02003d7e(3, 120, 168);
        Func_02003d4c(5);
        Func_02003d7c(8, 33597556);
        Func_02003daa(3);
        Func_02003df6(3, 49152, 30);
        Func_02003dde(3, 1);
        Func_02003d74(15);
        p38 = Func_02003d92(3);
        *(u8 *)((u8 *)(p38) + 90) &= 0xfe;
        Func_02003dc6(3, 136, 184);
        Func_02003d94(15);
        Func_02003dc4(8, 33597472);
        Func_02003df2(3);
        Func_02003e1a(3, 1);
        p44 = Func_02003dc8(3);
        *(u8 *)((u8 *)(p44) + 90) |= 0x1;
    }
    Func_02003dc4(20);
    Func_02003e70(3, 258, 60);
    p47 = Func_02003dee(3);
    *(s32 *)(p47 + 108) = 33590617;
    Func_02003eaa(0, 1);
    Func_02003de8(30);
    Func_02003e34(0, 136, 264);
    Func_02003eb8();
    Func_02003db4(0, 0, 3, 3, 7, 9);
    Func_02003e20();
}
