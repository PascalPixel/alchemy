#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunMultiActorPresentation:
 * 95 calls, four actor-coordinate lookups, two conditional presentation
 * paths, and all literal-backed sequence identifiers across the complete
 * callable owner. */

#define FieldScene_RunMultiActorPresentation Func_020004a8

void Func_02001026();
s32 Func_02001fb2();
void Func_02001fc6();
void Func_02001fe2();
void Func_0200201e();
void Func_0200202a();
void Func_02002038();
void Func_0200203a();
void Func_02002050();
void Func_02002068();
void *Func_0200206e();
void Func_0200208e();
void Func_02002094();
void *Func_020020aa();
void Func_020020ac();
void Func_020020bc();
void Func_020020c0();
void Func_020020ca();
void *Func_020020e6();
void Func_020020e8();
void Func_020020fc();
void Func_020020fe();
void Func_02002106();
void Func_02002118();
void *Func_02002122();
void Func_02002124();
void Func_02002138();
void Func_0200213e();
void Func_02002142();
void Func_0200215c();
void Func_0200215e();
void Func_02002160();
void Func_0200216a();
void Func_02002174();
void Func_0200219c();
void Func_020021b6();
void Func_020021c0();
void Func_020021c4();
void Func_020021d6();
void Func_020021d8();
s32 Func_020021e0();
void Func_020021ee();
void Func_020021f8();
void Func_02002206();
void Func_02002210();
void Func_02002226();
void Func_02002228();
void Func_02002232();
void Func_02002234();
void Func_02002236();
void Func_0200225a();
void Func_0200226a();
void Func_02002272();
void Func_02002276();
void Func_0200227e();
void Func_02002282();
void Func_02002286();
void Func_0200228e();
void Func_0200229e();
void Func_020022a6();
void Func_020022ae();
void Func_020022b6();
void Func_020022b8();
void Func_020022d0();
void Func_020022d2();
void Func_020022de();
void Func_020022e0();
void Func_020022ea();
void Func_020022f6();
void Func_02002304();
void Func_02002306();
void Func_02002308();
void Func_02002312();
void Func_0200231e();
void Func_02002320();
void Func_02002326();
void Func_02002336();
void Func_02002338();
void Func_02002342();
void Func_02002372();
void Func_02002376();
void Func_0200238a();
void Func_02002394();
void Func_020023cc();
void Func_02002400();
void Func_0200240c();

void Func_020004a8(void)
{
    void *actor;

    if (Func_02001fb2(0x941) == 0) {
    } else {
        Func_02001fc6(0x94d);
        Func_02001fe2();
        Func_02002038(12, 9437184, 26214400);
        Func_0200201e(12, 98304, 49152);
        Func_0200203a(12, 184, 400);
        Func_02002050(12);
        Func_02002068(12, 1);
        Func_020020bc(12, 12288, 0);
        Func_02002094(0, 1);
        Func_0200202a(30);
        Func_020020fe(32768, 4096);
        Func_02002118(12582912, -1, 28311552, 1);
        actor = Func_0200206e(0);
        if (actor != 0) {
            Func_020020ac(13, *(s32 *)((u8 *)actor + 8), *(s32 *)((u8 *)actor + 16));
        }
        Func_0200208e(13, 0x14ccc, 0xa666);
        Func_020020aa(13, 168, 464);
        Func_020020c0(13);
        Func_02002124(13, 49152, 0);
        actor = Func_020020aa(0);
        if (actor != 0) {
            Func_020020e8(2, *(s32 *)((u8 *)actor + 8), *(s32 *)((u8 *)actor + 16));
        }
        Func_020020ca(2, 0x14ccc, 0xa666);
        Func_020020e6(2, 152, 488);
        Func_020020fc(2);
        Func_02002160(2, 49152, 0);
        actor = Func_020020e6(0);
        if (actor != 0) {
            Func_02002124(3, *(s32 *)((u8 *)actor + 8), *(s32 *)((u8 *)actor + 16));
        }
        Func_02002106(3, 0x14ccc, 0xa666);
        Func_02002122(3, 168, 488);
        Func_02002138(3);
        Func_0200219c(3, 49152, 0);
        actor = Func_02002122(0);
        if (actor != 0) {
            Func_02002160(1, *(s32 *)((u8 *)actor + 8), *(s32 *)((u8 *)actor + 16));
        }
        Func_02002142(1, 0x14ccc, 0xa666);
        Func_0200215e(1, 184, 488);
        Func_02002174(1);
        Func_020021d8(1, 49152, 0);
        Func_0200213e(30);
        Func_020021b6(1, 1);
        Func_020021d6(0x250d);
        Func_020021ee(1, 0);
        Func_0200215c(30);
        Func_020021c4(3, 3);
        Func_0200216a(10);
        Func_020021f8(0x250e);
        Func_02002210(3, 0);
        Func_02002234(2, 258, 70);
        Func_02002206(2, 0, 0);
        Func_02002210(0, 2, 0);
        Func_02002226(0x250f);
        Func_02002236(2, 0);
        Func_02002228(3, 0, 0);
        Func_02002232(1, 0, 0);
        Func_020021c0(30);
        if (Func_020021e0(0, 0) == 0) {
            Func_0200225a(0x2510);
            Func_02002272(1, 0);
        } else {
            Func_0200226a(0x2511);
            Func_02002282(1, 0);
        }
        Func_020022a6(13, 256, 70);
        Func_02002286(0x2512);
        Func_0200229e(13, 0);
        Func_02002276(0, 2);
        Func_0200227e(1, 2);
        Func_02002286(2, 2);
        Func_0200228e(3, 2);
        Func_020022d2(0, 49152, 0);
        Func_020022de(1, 49152, 0);
        Func_020022ea(2, 49152, 0);
        Func_020022f6(3, 49152, 0);
        Func_020022b6(13, 2);
        Func_02002372(13, 0, -16);
        Func_020022b8(13);
        Func_020022d0(13, 1);
        Func_02002306(0x2513);
        Func_0200231e(13, 0);
        Func_02002342(12, 256, 65);
        Func_02002320(0x2514);
        Func_02002338(12, 0);
        Func_02002308(13, 3);
        Func_020022ae(80);
        Func_020022e0(12, 0x6666, 0x3333);
        Func_020023cc(12, -13, 0);
        Func_02002312(12);
        Func_02002376(12, 16384, 0);
        Func_02002394(12, 258, 70);
        Func_02002372(0x2515);
        Func_0200238a(12, 0);
        Func_02002336(12, 168, 432);
        Func_02002304(40);
        Func_02002400();
        Func_0200240c();
        Func_02002312(20);
        Func_02002326();
        Func_02001026();
    }
}
