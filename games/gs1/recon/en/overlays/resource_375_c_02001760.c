#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunProgressDependentActorSequence:
 * 60 calls, nested resource predicates, and terminal actor-coordinate
 * forwarding across the complete callable owner. */

#define FieldScene_RunProgressDependentActorSequence Func_02001760

s32 Func_020031ca();
s32 Func_020031d6();
void Func_020031fe();
void Func_0200321e();
void Func_02003220();
void Func_02003254();
s32 Func_0200328e();
void Func_020032a0();
void Func_020032b4();
void Func_020032c2();
void Func_020032c6();
s32 Func_020032c8();
void Func_020032cc();
void Func_020032dc();
void Func_020032de();
void Func_020032e4();
void Func_020032e6();
void Func_020032fe();
void Func_02003322();
void Func_0200332a();
void Func_0200333c();
void Func_02003340();
void Func_0200334c();
void Func_02003352();
void Func_02003360();
void Func_0200336a();
void Func_02003370();
void Func_02003378();
void Func_0200338a();
void Func_02003390();
void Func_02003392();
void Func_02003398();
void Func_0200339c();
void Func_020033a2();
void Func_020033a6();
void Func_020033b2();
void Func_020033b4();
void Func_020033b8();
void Func_020033bc();
void Func_020033be();
void Func_020033ce();
void *Func_020033e0();
void Func_020033f6();
void Func_02003404();
void Func_0200340a();
void Func_02003414();
void Func_02003416();
void Func_0200341a();
void Func_0200342a();
void Func_02003438();
void Func_0200344a();
void Func_02003464();
void Func_02003470();
void Func_02003482();
void Func_0200348c();

void Func_02001760(void)
{
    void *actor;

    if (Func_020031ca(0x839) != 0) {
    } else {
        if (Func_020031d6(0x82f) != 0) {
            Func_020031fe();
            Func_0200328e(11, 2);
            Func_020032b4(0xe8b);
            Func_020032cc(11, 0);
            Func_02003220();
        } else {
            Func_0200321e();
            Func_02003254(11);
            Func_020032b4(11, 1);
            Func_020032dc(0xe85);
            Func_020032fe(11, 0, 20);
            Func_02003322(0, 256, 30);
            Func_02003352(6422528, -1, 0x011b0000, 1);
            Func_020032b4(0, 94, 0x125);
            Func_02003340(0, 40960, 0);
            if (Func_0200328e(0) != 0) {
                Func_020032e4(1);
            }
            Func_020032de(1, 110, 0x117);
            Func_0200336a(1, 40960, 40);
            Func_0200332a(11, 2);
            Func_020032a0(40);
            Func_02003360(11, 0);
            if (Func_020032c8(0, 0) == 0) {
                Func_0200334c(11, 2);
                Func_020032c2(20);
                Func_02003378(0xe87);
                Func_02003390(11, 0);
                Func_020032c6(0x82f);
            } else {
                Func_02003370(11, 2);
                Func_020032e6(20);
                Func_0200339c(0xe88);
                Func_020033be(11, 0, 40);
                Func_02003398(11, 0, 0);
                Func_02003378(11, 1);
                Func_02003392(11, 4, 40);
                Func_0200338a(11, 6);
                Func_02003404(11, 0x101, 40);
                Func_020033f6(11, 0, 10);
                Func_020033a6(11, 1);
                Func_0200333c(10);
                Func_020033bc(11, 3);
                Func_02003416(11, 0, 10);
                Func_020033ce(11, 3);
                Func_0200340a(0, 0x0001000b, 0x02009ce0);
                Func_02003414(1, 0x0001000b, 0x02009ce0);
                Func_020033b4(11, 0x02009bdc);
                Func_020033b2(0);
                Func_020033b8(1);
                Func_02003464(0, 16384, 0);
                Func_02003470(1, 16384, 60);
                Func_02003482(0, 0x105, 0);
                Func_0200348c(1, 0x105, 120);
                Func_020033a2(0x839);
            }
            Func_0200342a(1, 2);
            actor = Func_020033e0(0);
            if (actor != 0) {
                Func_0200341a(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
            }
            Func_02003438(1);
            Func_0200344a(1, 0, 0);
            Func_020033f6();
        }
    }
}
