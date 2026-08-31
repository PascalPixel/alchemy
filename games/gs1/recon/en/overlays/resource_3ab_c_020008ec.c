#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_020008ec

/* Audited retained supplemental scene body.
 * The complete production span preserves 30 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_020022ca();
s32 Func_020022d4();
void Func_02002310();
s32 Func_0200235e();
void Func_02002372();
void Func_0200238c();
void Func_0200239a();
s32 Func_020023aa();
void Func_020023b2();
s32 Func_020023b4();
s32 Func_020023be();
void Func_020023c0();
void Func_020023c2();
void Func_020023d8();
void Func_020023fa();
void Func_02002406();
void Func_02002418();
void Func_02002420();
void Func_02002438();
void Func_0200247c();
void Func_0200249a();
void Func_0200249c();
void Func_020024aa();
void Func_020024c0();
void Func_020024c2();
void Func_020024c6();
void Func_020024c8();
void Func_020024e0();
void Func_0200252a();

void Func_020008ec(void)
{
    s32 base;

    Func_02002372(9233);
    if (Func_020022ca(2369) != 0) {
        if (Func_020022d4(2381) == 0) {
            Func_020023c2(8, 257, 60);
            base = 9435;
            Func_0200239a(base);
            Func_020023b2(8, 0);
            Func_0200238c(8, 1);
            Func_020023b2(base + 1);
            Func_02002310(2479);
        } else {
            Func_020023c0(7093);
        }
    }
    Func_020023d8(8, 0);
    Func_02002406(9234);
    if (Func_0200235e(2369) != 0) {
        Func_02002418(9437);
    } else {
        Func_02002420(7094);
    }
    Func_02002438(9, 0);
    if (Func_020023aa(2369) != 0) {
        if (Func_020023b4(2381) == 0) {
            if (Func_020023be(2479) == 0) {
                Func_0200252a();
                Func_0200247c(8, 0, 0);
                Func_020024c6(8, 257, 60);
                base = 9435;
                Func_0200249c(base);
                Func_020023fa(2479);
            } else {
                Func_020024aa(9447);
            }
            Func_020024c2(8, 0);
            Func_0200249a(8, 1);
            Func_020024c0(9436);
        } else {
            Func_020024c8();
        }
    }
    Func_020024e0(8, 0);
}
