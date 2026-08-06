#include "types.h"






















/* Run the complete actor-3/actor-8 exchange and its two repeated beats. */
extern void Func_02003c18();
extern u8 * Func_02003c2e();
extern u8 * Func_02003c38();
extern u8 * Func_02003c40();
extern void Func_02003c90();
extern void Func_02003c9e();
extern void Func_02003cac();
extern void Func_02003c82();
extern void Func_02003c9a();
extern void Func_02003d32();
extern void Func_02003d4c();
extern void Func_02003d98();
extern void Func_02003dac();
extern void Func_02003d14();
extern u8 * Func_02003cc2();
extern void Func_02003cba();
extern void Func_02003d0a();
extern void Func_02003cd8();
extern void Func_02003d08();
extern void Func_02003d36();
extern void Func_02003d82();
extern void Func_02003d6a();
extern u8 * Func_02003d18();
extern void Func_02003d1a();
extern void Func_02003d4a();
extern void Func_02003d78();
extern u8 * Func_02003d46();
extern void Func_02003d3c();
extern void Func_02003d7e();
extern void Func_02003d7c();
extern void Func_02003daa();
extern void Func_02003df6();
extern void Func_02003dde();
extern void Func_02003d74();
extern u8 * Func_02003d92();
extern void Func_02003dc6();
extern void Func_02003d94();
extern void Func_02003dc4();
extern void Func_02003df2();
extern void Func_02003e1a();
extern u8 * Func_02003dc8();
extern void Func_02003e70();
extern u8 * Func_02003dee();
extern void Func_02003eaa();
extern void Func_02003de8();
extern void Func_02003e34();
extern void Func_02003eb8();
extern void Func_02003db4();
extern void Func_02003e20();
void Func_02001730(void)
{
    u8 *actor;
    s32 pass;

    Func_02003c18();
    actor = Func_02003c2e(12); *(s32 *)(actor + 24) = -0x10000;
    actor = Func_02003c38(13); *(s32 *)(actor + 24) = -0x10000;
    actor = Func_02003c40(14); *(s32 *)(actor + 24) = -0x10000;
    Func_02003c90(3, 0x880000, 0xb80000);
    Func_02003c9e(0, 0x880000, 0x1280000);
    Func_02003cac(8, 0x880000, 0x980000);
    Func_02003c82(3, 0x18000, 0xc000);
    Func_02003c90(8, 0x18000, 0xc000);
    Func_02003c9a(0, 0xcccc, 0x6666);
    Func_02003d32(0xcccc, 0x1999);
    Func_02003d4c(0x880000, -1, 0xb80000, 0);
    Func_02003d98();
    Func_02003dac();
    Func_02003d14(3, 1);
    actor = Func_02003cc2(8); actor[90] &= (u8)~1;
    Func_02003cba(20);

    for (pass = 0; pass < 2; pass++) {
        Func_02003d0a(3, 152, 168);
        Func_02003cd8(10);
        Func_02003d08(8, (void *)0x0200a8c8);
        Func_02003d36(3);
        Func_02003d82(3, 0xc000, 30);
        Func_02003d6a(3, 1);
        actor = Func_02003d18(3); actor[90] &= (u8)~1;
        Func_02003d4c(3, 136, 184);
        Func_02003d1a(10);
        Func_02003d4a(8, (void *)0x0200a820);
        Func_02003d78(3);
        actor = Func_02003d46(3); actor[90] |= 1;
        Func_02003d3c(30);
        Func_02003d7e(3, 120, 168);
        Func_02003d4c(5);
        Func_02003d7c(8, (void *)0x0200a874);
        Func_02003daa(3);
        Func_02003df6(3, 0xc000, 30);
        Func_02003dde(3, 1);
        Func_02003d74(15);
        actor = Func_02003d92(3); actor[90] &= (u8)~1;
        Func_02003dc6(3, 136, 184);
        Func_02003d94(15);
        Func_02003dc4(8, (void *)0x0200a820);
        Func_02003df2(3);
        Func_02003e1a(3, 1);
        actor = Func_02003dc8(3); actor[90] |= 1;
    }

    Func_02003dc4(20);
    Func_02003e70(3, 0x102, 60);
    actor = Func_02003dee(3); *(void **)(actor + 108) = (void *)0x02008d59;
    Func_02003eaa(0, 1);
    Func_02003de8(30);
    Func_02003e34(0, 136, 264);
    Func_02003eb8();
    Func_02003db4(0, 0, 3, 3, 7, 9);
    Func_02003e20();
}
