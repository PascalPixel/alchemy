#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunDualArrivalSequence:
 * state-routed scene setup, four actor field groups, and all 44 calls. */

#define FieldScene_RunDualArrivalSequence Func_02001c20

void *Func_020044c8();
s32 Func_020046de();
void Func_02004904();
void Func_0200497e();
void Func_020056ea();
void Func_0200575e();
void Func_020057c4();
void Func_020057cc();
void Func_02005838();
void Func_02005848();
void Func_020058b6();
void Func_020064fa();
void Func_020065e4();
void Func_020065ee();
void Func_02006610();
void Func_0200661e();
void Func_0200663c();
void *Func_02006642();
void Func_0200665e();
void Func_0200666c();
void Func_02006678();
void Func_02006690();
void Func_020066a0();
void *Func_020066d8();
void *Func_020066fc();
void Func_02006704();
void Func_02006708();
void *Func_0200672a();
void Func_02006748();
void *Func_0200674a();
void Func_02006752();
void Func_0200675c();
void Func_0200676a();
void Func_02006770();
void Func_02006776();
void Func_0200677a();
void Func_02006782();
void Func_0200678a();
void Func_020067de();
void Func_02006822();
void Func_0200685a();
void Func_02006870();
void Func_02006888();
void Func_0200689c();

void Func_02001c20(s32 scene)
{
    void *p17;
    void *p19;
    void *p23;
    void *p25;
    s32 state;

    if (*(s16 *)0x02000402 == 2) {
        Func_020044c8();
        return;
    }
    Func_02006642();
    state = Func_020046de(scene, 1);
    if (state == 0) {
    Func_02006708(8370);
    Func_0200675c(196608, 24576);
    Func_02006776(9961472, -1, 13107200, 1);
    Func_02006782();
    Func_02006678(30);
    Func_02006748(scene, 0);
    Func_020056ea(104, 68, 0);
    Func_02006690(60);
    Func_0200575e(168, 96, 10);
    Func_020066a0(70);
    Func_02006770(scene, 0);
    Func_020057cc();
    Func_020064fa(2);
    p17 = Func_020066d8(10);
    *(u8 *)((u8 *)(p17) + 85) = 0;
    *(s32 *)(p17 + 48) = 52428;
    *(s32 *)(p17 + 52) = 26214;
    Func_020065ee(*(s32 *)(p17 + 8), *(s32 *)(p17 + 16), 262144);
    p19 = Func_020066fc(11);
    *(u8 *)((u8 *)(p19) + 85) = 0;
    *(s32 *)(p19 + 48) = 52428;
    *(s32 *)(p19 + 52) = 26214;
    Func_02006610(*(s32 *)(p19 + 8), *(s32 *)(p19 + 16), 4194304);
    Func_0200661e(p19);
    Func_02006704(45);
    p23 = Func_0200672a(10);
    *(u8 *)((u8 *)(p23) + 85) = 0;
    *(s32 *)(p23 + 48) = 52428;
    *(s32 *)(p23 + 52) = 26214;
    Func_0200663c(*(s32 *)(p23 + 8), *(s32 *)(p23 + 16), 4194304);
    p25 = Func_0200674a(11);
    *(u8 *)((u8 *)(p25) + 85) = 0;
    *(s32 *)(p25 + 48) = 52428;
    *(s32 *)(p25 + 52) = 26214;
    Func_0200665e(*(s32 *)(p25 + 8), *(s32 *)(p25 + 16), 262144);
    Func_0200666c(p25);
    Func_02006752(15);
    Func_02006822(scene, 0);
    Func_020057c4(104, 68, 0);
    Func_0200676a(30);
    Func_02005838(168, 96, 10);
    Func_0200677a(40);
    Func_02005848(104, 68, 10);
    Func_0200678a(70);
    Func_0200685a(scene, 0);
    Func_020058b6();
    Func_020065e4(2);
    Func_0200689c(0, 0);
    Func_02004904(scene, 1);
    } else if (state == 1) {
        Func_02006870(0x20b1);
        Func_02006888(scene, 0);
    }
    Func_0200497e(state, scene, 1);
    Func_020067de();
}
