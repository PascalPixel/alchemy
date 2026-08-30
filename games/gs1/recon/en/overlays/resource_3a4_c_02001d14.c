#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunMultiPhasePresentation:
 * 68 calls, bounded completion polling, repeated actor-state phases, and
 * terminal coordinate forwarding across the complete decoded owner. */

#define FieldScene_RunMultiPhasePresentation Func_02001d14

void Func_02004b58();
void *Func_02005894();
void *Func_020058b6();
void Func_020058c2();
void *Func_020058da();
void Func_02005964();
void Func_02005968();
void Func_0200597c();
void Func_02005996();
void Func_0200599e();
void *Func_020059aa();
void Func_020059ac();
void Func_020059b2();
void Func_020059b6();
void Func_020059c2();
void Func_020059ca();
void Func_020059ce();
void Func_020059e4();
void *Func_020059f4();
void Func_020059fe();
void Func_02005a06();
void Func_02005a0c();
void Func_02005a0e();
void Func_02005a10();
void Func_02005a18();
void Func_02005a1a();
void *Func_02005a20();
void Func_02005a22();
void Func_02005a3c();
void Func_02005a46();
void Func_02005a50();
void Func_02005a68();
void Func_02005a6c();
void *Func_02005a86();
void Func_02005a90();
void Func_02005a98();
void Func_02005aa4();
void Func_02005aa6();
void Func_02005aac();
void Func_02005ac4();
void Func_02005ac6();
void Func_02005ada();
void Func_02005af6();
void Func_02005af8();
void Func_02005b16();
void Func_02005b28();
void Func_02005b2e();
void Func_02005b42();
void Func_02005b48();
void Func_02005b54();
void Func_02005b5e();
void *Func_02005b68();
void Func_02005b6c();
void Func_02005b88();
void Func_02005b9a();
void Func_02005ba2();
void Func_02005bcc();
void Func_02005be0();
void Func_02005c52();
void Func_02005c66();
void Func_02005c6a();
void Func_02005c72();
void Func_02005ca2();

void Func_02001d14(void)
{
    volatile u32 *sequence_done;
    void *scene_actor;
    void *p1;
    void *p2;
    void *p26;
    void *p3;
    void *p31;
    void *p4;
    void *p43;
    s32 i1;

    p1 = Func_020058b6(10);
    p2 = Func_02005894();
    p3 = Func_020058da(10);
    p4 = Func_020059aa(157286, 19660);
    Func_020059c2(18284544, 4194304, 14155776, 1);
    Func_020059ce();
    Func_02005a6c(147);
    Func_02005964(10, 2);
    Func_020058c2(40);
    Func_020059b6(10, 12288, 20);
    Func_020059c2(10, 20480, 20);
    Func_020059ce(10, 32768, 40);
    Func_020059fe(52428, 6553);
    Func_02005a18(8388608, 4194304, 13238272, 1);
    *(s32 *)(p1 + 104) = 0;
    *(u16 *)((u8 *)p1 + 100) = 0;
    *(u16 *)((u8 *)p1 + 102) = 0;
    *(s32 *)(p1 + 72) = 26214;
    *(s32 *)(p1 + 108) = 33593201;
    Func_02005964(10, 78643, 39321, 26214);
    Func_02005996(10, 212, 200);
    Func_020059ca(10, 103, 200);
    *(s32 *)(p1 + 108) = 0;
    *(u8 *)((u8 *)p1 + 91) = 0;
    Func_0200597c(10);
    Func_02005a0c(10, 1);
    Func_02005b42(229);
    Func_02005968(65536, 0, 65536);
    Func_0200599e(4);
    Func_0200597c(-1, -1, 58982);
    Func_020059b2(20);
    Func_02005aa6(10, 20480, 40);
    p26 = Func_020059f4(10);
    *(u8 *)((u8 *)(p26) + 90) &= 0xfe;
    Func_02005a10(10);
    Func_02005a0e(10);
    Func_020059ac();
    Func_02005ba2(153);
    p31 = Func_02005a20(10);
    *(s32 *)(p31 + 40) = 262144;
    Func_02005a86(10, 3);
    Func_02005a68(10, 86, 214);
    Func_02005a98(10, 1);
    Func_02005a46(10);
    Func_020059e4();
    Func_02005a22(10);
    Func_02005be0(229);
    Func_02005a06(131072, 0, 65536);
    Func_02005a3c(8);
    Func_02005a1a(-1, -1, 58982);
    Func_02005a50(40);
    p43 = Func_02005a86(10);
    *(u8 *)((u8 *)(p43) + 90) |= 0x1;
    Func_02005b54(10, 12288, 20);
    Func_02005b5e(10, 0, 40);
    *(s32 *)(p1 + 104) = 0;
    *(u16 *)((u8 *)p1 + 100) = 0;
    *(u16 *)((u8 *)p1 + 102) = 0;
    *(s32 *)(p1 + 108) = 33593201;
    Func_02005ac4(10);
    Func_02005af6(10, 120, 215);
    *(s32 *)(p1 + 108) = 0;
    *(u8 *)((u8 *)p1 + 91) = 0;
    Func_02005b2e(10, 1);
    Func_02005aac(16);
    Func_02005c6a(229);
    Func_02005a90(65536, 0, 65536);
    Func_02005ac6(4);
    Func_02005aa4(-1, -1, 58982);
    Func_02005ada(40);
    Func_02005bcc(10, 0, 10);
    Func_02005ca2(147);
    Func_02005b9a(10, 2);
    Func_02005af8(80);
    Func_02005b88(10, 3);
    Func_02004b58(8519680, 0, 11010048, 0);
    Func_02005b16(60);
    sequence_done = (volatile u32 *)0x03001c94;
    for (i1 = 0; *sequence_done == 0 && i1 <= 59; i1++) {
        Func_02005b28(1);
    }
    scene_actor = (void *)Func_02005b68(0);
    Func_02005c52(314572, 39321);
    Func_02005c66(*(u32 *)((u8 *)scene_actor + 8),
                  *(u32 *)((u8 *)scene_actor + 12),
                  *(u32 *)((u8 *)scene_actor + 16), 1);
    Func_02005c72();
    Func_02005b48(0x905);
    Func_02005b6c();
}
