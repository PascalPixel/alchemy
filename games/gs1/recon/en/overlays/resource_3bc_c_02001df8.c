#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSecondArrivalSequence:
 * state-routed scene setup and all 40 calls with their scene arguments. */

#define FieldScene_RunSecondArrivalSequence Func_02001df8

void Func_0200469a();
s32 Func_020048b0();
void Func_02004aaa();
void Func_02004b24();
void Func_020054a8();
void Func_02005766();
void Func_02005960();
void Func_020059ce();
void Func_020059de();
void Func_020059ee();
void Func_02005a00();
void Func_02005a0c();
void Func_02005a5c();
void Func_02006814();
void Func_0200684c();
void Func_0200688a();
void Func_0200689e();
void Func_020068da();
void Func_02006910();
void Func_0200691e();
void Func_02006920();
void Func_0200692e();
void Func_02006930();
void Func_02006948();
void Func_02006956();
void Func_02006984();
void Func_020069a4();
void Func_020069a8();
void Func_020069be();
void Func_020069c6();
void Func_020069ca();
void Func_020069e8();
void Func_020069ea();
void Func_02006a00();
void Func_02006a08();
void Func_02006a16();
void Func_02006a2e();
void Func_02006a42();

void Func_02001df8(s32 scene)
{
    s32 state;

    if (*(s16 *)0x02000402 == 2) {
        Func_0200469a();
        return;
    }
    Func_02006814();
    state = Func_020048b0(scene, 2);
    if (state == 0) {
    Func_020068da(8375);
    Func_0200692e(196608, 24576);
    Func_02006948(24641536, -1, 9961472, 1);
    Func_02006956();
    Func_0200684c(30);
    Func_0200691e(scene, 0);
    Func_020054a8(0, 280, 200);
    Func_0200689e(0, 98304, 49152);
    Func_02005a00(0, 280, 152);
    Func_02005a0c(0, 296, 152);
    Func_0200688a(10);
    Func_020069c6();
    Func_020069a8(-1, -1, -1, 0);
    Func_02006984(0, 49152, 15);
    Func_020069e8();
    Func_020069ca(-1, -1, -1, 0);
    Func_020069a4(0, 0, 15);
    Func_02006a08();
    Func_020069ea(-1, -1, -1, 0);
    Func_020069c6(0, 16384, 15);
    Func_020069be(scene, 0);
    Func_02005960(96, 40, 0);
    Func_020059ce(128, 40, 10);
    Func_02006910(30);
    Func_020059de(160, 40, 10);
    Func_02006920(30);
    Func_020059ee(160, 72, 10);
    Func_02006930(30);
    Func_02006a00(scene, 0);
    Func_02005a5c();
    Func_02005766(0);
    Func_02006a42(0, 0);
    Func_02004aaa(scene, 2);
    } else if (state == 1) {
        Func_02006a16(0x20b6);
        Func_02006a2e(scene, 0);
    }
    Func_02004b24(state, scene, 2);
    Func_02006984();
}
