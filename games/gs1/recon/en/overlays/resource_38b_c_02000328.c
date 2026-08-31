#include "types.h"

#define FieldScene_RunEarlySequence Func_02000328

/* Audited retained field-scene body at 0x02000328.
 * The complete production body preserves 50 decoded calls, 1 loop(s), and 13 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_020013e4();
void *Func_020013e8();
s32 Func_0200140a();
void *Func_02001460();
void Func_02001462();
void Func_02001496();
void Func_020014b0();
void Func_020014b4();
void Func_020014b8();
void Func_020014ba();
void *Func_020014da();
void *Func_020014e2();
void Func_020014e8();
void *Func_0200151a();
void *Func_02001522();
void Func_02001524();
void Func_0200152e();
void Func_02001538();
void Func_02001548();
void Func_0200154a();
void Func_02001552();
void Func_02001560();
void Func_02001568();
void Func_02001574();
void Func_0200157a();
void Func_0200157c();
void Func_02001582();
void Func_02001584();
void Func_02001598();
void Func_020015ac();
void Func_020015b6();
void *Func_020015c0();
void Func_020015c8();
void Func_020015da();
void Func_020015dc();
void Func_020015e6();
void Func_020015ee();
void Func_020015f6();
void Func_0200160e();
void Func_02001612();
void Func_0200163e();
void Func_02001666();
void Func_0200166a();
void Func_02001676();
void Func_0200167a();
void Func_020016ca();
void Func_020016ea();

void Func_02000328(void)
{
    void *p1;
    void *p16;
    void *p17;
    void *p24;
    void *p3;
    void *p38;
    void *p7;
    s32 i1;

    p1 = Func_020013e8();
    for (i1 = 8; i1 < 65; i1++) {
        if (Func_0200140a(8) != 0) {
        }
    }
    p3 = Func_0200151a(188);
    Func_02001522(158);
    Func_020013e4(36);
    Func_02001462(0, 32768, 16384);
    p7 = Func_02001460(0);
    *(u8 *)((u8 *)(p7) + 85) = 0;
    Func_020014b8(0, 2);
    Func_020014b4(0, 0, -4);
    Func_020014ba(0, 3, -16);
    Func_0200152e(0, 3);
    Func_02001538(0, 2);
    Func_02001496(16);
    Func_02001574(12);
    Func_020014b0();
    p16 = Func_020014da(0);
    p17 = Func_020014e2(11);
    Func_020014da();
    Func_02001582(11, 1);
    Func_02001552(0, 2);
    Func_020014e8(20);
    Func_0200151a(0, 13107, 6553);
    Func_02001524(11, 13107, 6553);
    p24 = Func_02001522(0);
    *(u8 *)((u8 *)(p24) + 90) &= 0xfe;
    *(u8 *)((u8 *)(p17) + 85) = *(u8 *)((u8 *)(p24) + 90);
    *(s32 *)(p16 + 24) = 0;
    Func_020015da(0, 258, 0);
    Func_02001584(0, 16);
    Func_02001568(11, 111, 196);
    *(s32 *)(p16 + 24) = 65536;
    Func_0200157c(0, 128, 185);
    Func_0200154a(20);
    *(s32 *)(p16 + 24) = 0;
    Func_0200160e(0, 258);
    Func_020015b6(0, 16);
    Func_02001598(11, 121, 190);
    *(s32 *)(p16 + 24) = 65536;
    Func_020015ac(0, 141, 189);
    Func_0200157a(20);
    *(s32 *)(p16 + 24) = 0;
    Func_0200163e(0, 258);
    Func_020015e6(0, 16);
    Func_020015c8(11, 132, 186);
    *(s32 *)(p16 + 24) = 65536;
    p38 = Func_020015c0(0);
    *(u8 *)((u8 *)(p38) + 90) |= 0x1;
    Func_020015dc(0);
    Func_020015f6(0, 166, 185);
    Func_0200166a(0, 32768, 20);
    Func_0200167a(11, 2);
    Func_020016ca(0, 11);
    Func_02001548(10);
    Func_02001666(6004);
    Func_02001676(11, 0);
    Func_020016ea();
    Func_02001560(10);
    Func_020015ee();
    Func_02001612();
}
