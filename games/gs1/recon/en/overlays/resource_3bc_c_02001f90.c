#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunTimedArrivalSequence:
 * state-routed scene setup, bounded event polling, and all 30 calls. */

#define FieldScene_RunTimedArrivalSequence Func_02001f90

void Func_02002156();
void Func_020021b6();
void Func_020021f2();
void Func_02002ba0();
void Func_02004836();
s32 Func_02004a4c();
void Func_02004bf4();
void Func_02004c78();
void Func_0200565c();
void Func_020058b0();
void Func_02005b84();
void Func_020068a0();
void Func_020069b0();
void Func_020069ee();
void Func_02006a00();
void Func_02006a60();
void Func_02006a7a();
void Func_02006abe();
void Func_02006ad0();
void Func_02006ad2();
void Func_02006ad8();
void Func_02006aec();
void Func_02006af6();
void Func_02006af8();
void Func_02006b4c();
void Func_02006b4e();
void Func_02006b66();
void Func_02006b6a();
void Func_02006b82();
void Func_02006b8c();

void Func_02001f90(s32 scene)
{
    s32 state;

    if (*(s16 *)0x02000402 == 2) {
        Func_02004836();
        return;
    }
    Func_020069b0();
    state = Func_02004a4c(scene, 3);
    if (state == 0) {
    Func_02006a7a(8379);
    Func_02002156();
    Func_02006ad2(196608, 24576);
    Func_02006aec(40370176, -1, 12058624, 1);
    Func_02006af8();
    Func_020069ee(30);
    Func_02006abe(scene, 0);
    Func_020021b6();
    Func_02006a00(60);
    Func_02006ad0(scene, 0);
    Func_0200565c(0, 504, 200);
    Func_02006af6(0, 0, 0);
    Func_020021f2();
    Func_02006a60(0, 98304, 49152);
    Func_02005b84(0, 680, 200);
    while (*(s16 *)(*(u32 *)0x03001ebc + 386) != 5 && state <= 239) {
        Func_020068a0(1);
        state++;
    }
    Func_02002ba0();
    Func_02006b4c(0, 49152, 20);
    Func_02006b66(0, 259, 60);
    Func_02006b4e(scene, 0);
    Func_020058b0(0);
    Func_02006b8c(0, 0);
    Func_02004bf4(scene, 3);
    *(u16 *)(*(u32 *)0x03001ebc + 386) = 0;
    } else if (state == 1) {
        Func_02006b6a(0x20ba);
        Func_02006b82(scene, 0);
    }
    Func_02004c78(state, scene, 3);
    Func_02006ad8();
}
