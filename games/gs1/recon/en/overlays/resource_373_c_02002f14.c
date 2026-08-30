#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunPrimaryActorSequence:
 * 64 calls, actor-coordinate handoffs, and conditional event progress. */

#define FieldScene_RunPrimaryActorSequence Func_02002f14

void Func_02008ed4();
s32 Func_02008f26();
void Func_02008f54();
void Func_02008f80();
void Func_02008f8c();
void Func_02008fe2();
void Func_02008fe8();
void Func_02008ff2();
void Func_02009004();
void Func_0200900a();
void Func_02009014();
void Func_02009016();
void Func_02009034();
void Func_02009042();
void Func_0200904a();
void Func_0200904e();
void Func_02009054();
void Func_02009056();
void Func_0200905a();
void Func_02009080();
void Func_02009082();
void Func_02009088();
void Func_0200909e();
s32 Func_020090aa();
void Func_020090b0();
void Func_020090bc();
void Func_020090c0();
void Func_020090cc();
void Func_020090d0();
void Func_020090d8();
void Func_020090e6();
void Func_020090f4();
void Func_02009100();
void Func_02009108();
void Func_0200910c();
void Func_0200911a();
void Func_0200912a();
void Func_0200913c();
s32 Func_02009140();
void Func_0200914e();
void Func_02009152();
void Func_0200915a();
void Func_02009166();
void Func_0200916a();
void Func_02009178();
void Func_0200917a();
void Func_0200918e();
void Func_02009192();
void Func_02009194();
void Func_0200919a();
void Func_020091a0();
void Func_020091a6();
void Func_020091a8();
void Func_020091b0();
void Func_020091ba();
void Func_020091c0();
void Func_020091ea();

void Func_02002f14(void)
{
    u8 *scene_state;
    u8 *actor;

    Func_02008ed4();
    Func_0200900a(0x01650000, -1, 0x02e20000, 1);
    Func_02008f54(0, 0x16f, 0x2e9);
    Func_02008fe8(0, 40960, 0);
    actor = (u8 *)Func_02008f26(0);
    if (actor != 0) {
        Func_02008f8c(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_02008f80(1, 346, 745);
    Func_02009014(1, 53248, 20);
    Func_02008ff2(0x1c53);
    Func_0200900a(1, 0);
    Func_02008fe2(9, 2);
    Func_0200904e(9, 256, 0);
    Func_02009042(9, 12288, 10);
    Func_0200904e(9, 20480, 10);
    Func_0200905a(9, 12288, 40);
    Func_02009054(9, 0, 20);
    Func_02009004(0, 3);
    Func_02009014(1, 3);
    Func_02009034(9, 1);
    Func_02009088(9, 20480, 10);
    Func_02009082(9, 0, 20);
    Func_0200904a(1, 1);
    Func_020090bc(1, 0x103, 40);
    Func_0200909e(1, 0, 10);
    Func_02009056(9, 3);
    Func_020090b0(9, 0, 20);
    Func_020090cc(0, 32768, 0);
    Func_020090d8(1, 4096, 40);
    Func_02009080(9, 4);
    Func_020090d0(9, 0);
    Func_020090f4(0, 45056, 0);
    Func_02009100(1, 53248, 10);
    Func_020090c0(1, 2);
    Func_02009016(20);
    Func_02009108(1, 0, 10);
    Func_020090c0(9, 3);
    Func_0200911a(9, 0, 20);
    Func_0200914e(0, 258, 0);
    Func_0200915a(1, 258, 80);
    Func_0200913c(1, 0, 20);
    Func_0200910c(9, 2);
    Func_0200914e(9, 0, 20);
    Func_0200916a(1, 4096, 20);
    Func_02009152(1, 0);
    if (Func_020090aa(0, 0) == 0) {
        Func_020091a0(1, 0x105, 60);
    } else {
        scene_state = *(u8 **)0x03001ebc;
        (*(u16 *)(scene_state + 472))++;
    }
    Func_02009194(1, 0, 20);
    Func_020091b0(1, 53248, 10);
    Func_0200918e(0x1c60);
    Func_020091a6(1, 0);
    Func_02009166(9, 3);
    Func_020091c0(9, 0, 20);
    Func_02009178(1, 3);
    Func_020090e6(20);
    Func_020091ea(1, 4096, 20);
    Func_02009192(1, 3);
    Func_0200919a(0, 3);
    Func_0200919a(1, 2);
    actor = (u8 *)Func_02009140(0);
    if (actor != 0) {
        Func_0200917a(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_020091a8(1);
    Func_020091ba(1, 0, 0);
    Func_0200912a(772);
    Func_0200914e();
}
