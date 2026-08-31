#include "types.h"

#define FieldScene_RunEarlySequence Func_02000524

/* Audited retained field-scene body at 0x02000524.
 * The complete production body preserves 49 decoded calls, 1 loop(s), and 26 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void *Func_020013e0();
void Func_020013f8();
s32 Func_0200140a();
void Func_0200140c();
void Func_02001422();
void Func_02001432();
void Func_0200144a();
void Func_0200145a();
void Func_0200145c();
void Func_02001470();
void Func_020014e2();
void *Func_020014e8();
void Func_0200150e();
void Func_02001526();
void Func_0200152e();
void Func_02001538();
void Func_0200153a();
void *Func_0200153e();
void Func_02001554();
void Func_02001558();
void Func_0200156a();
void *Func_0200157a();
void *Func_02001594();
void Func_02001598();
void Func_0200159c();
void Func_020015b2();
void Func_020015bc();
void Func_020015cc();
void Func_020015d8();
void Func_0200160c();
void Func_02001618();
void Func_02001624();
void Func_02001668();
void Func_02001674();
void Func_0200167e();
void Func_02001680();
void *Func_02001686();
void Func_02001690();
void Func_020016a4();
void Func_020016b6();
void Func_020016ca();
void Func_020016d2();
void Func_020016d6();
void Func_02001704();
void Func_02001732();
void Func_02001740();
void Func_020017bc();

void Func_02000524(void)
{
    void *p1;
    void *p14;
    void *p26;
    void *p3;
    void *p32;
    void *p39;
    s32 i1;

    p1 = Func_020013e0();
    for (i1 = 8; i1 < 65; i1++) {
        if (Func_0200140a(8) != 0) {
        }
    }
    p3 = Func_0200153e(188);
    Func_020013f8(42, 33, 2, 2);
    Func_0200140c(42, 35, 2, 2);
    Func_0200144a(4);
    Func_02001422(40, 33, 2, 2);
    Func_02001432(40, 35, 2, 2);
    Func_02001470(4);
    Func_02001598(158);
    Func_0200145a(33, 42, 8, 17, 1, 2);
    Func_0200145c();
    Func_020014e2(0, 32768, 16384);
    p14 = Func_020014e8(0, 448, 256, 50339964);
    *(u8 *)((u8 *)(p14) + 85) = 0;
    Func_0200152e(0, 2);
    Func_02001526(0, 2, 0);
    Func_0200153a(0, 2, -4);
    Func_020015bc(0, 2);
    Func_02001558(0, 0, -4);
    Func_0200150e(10);
    Func_0200160c();
    Func_02001618();
    Func_02001624();
    Func_02001538();
    Func_02001554();
    p26 = Func_0200157a(0);
    *(u8 *)((u8 *)(p26) + 85) = 0;
    Func_0200167e(123);
    Func_020015b2(0, 2, -16);
    Func_02001668();
    Func_02001674();
    Func_02001680();
    p32 = Func_02001594();
    *(u16 *)((u8 *)(p32) + 102) = 0;
    Func_0200159c(-1, -1, 58982);
    Func_0200156a();
    *(s32 *)(p32 + 12) = *(s32 *)(p32 + 12);
    Func_02001704(229);
    *(s32 *)(p32 + 104) = 0;
    *(u16 *)((u8 *)(p32) + 102) = 4;
    Func_020015d8(0, 65536, 65536, 4);
    *(s32 *)(p32 + 12) = *(s32 *)(p32 + 20);
    *(u8 *)((u8 *)(p32) + 91) = 0;
    Func_02001740(152);
    *(s32 *)(p32 + 104) = 1;
    Func_020015cc();
    *(s32 *)(p32 + 40) = 196608;
    *(u16 *)((u8 *)(p32) + 100) = 0;
    *(u16 *)((u8 *)(p32) + 100) = 0;
    p39 = Func_02001686(18);
    *(u16 *)((u8 *)(p39) + 100) = 0;
    *(u16 *)((u8 *)((u8 *)(p39) + 100) + 2) = 0;
    *(s32 *)(p39 + 72) = 26214;
    *(s32 *)(p39 + 108) = 33588977;
    Func_020016b6(18, 78643, 39321, 33588977);
    Func_020016ca(18, 28, 460);
    Func_020016d6(18, 24, 448);
    Func_020017bc(229);
    Func_020016d2(18);
    Func_02001690(0, 65536, 65536);
    Func_020016b6(4);
    Func_020016a4(-1, -1);
    Func_020016ca(40);
    Func_02001732(18, 1);
}
