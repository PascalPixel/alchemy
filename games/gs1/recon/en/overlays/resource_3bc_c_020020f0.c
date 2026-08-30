#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunFourStepActorMotion:
 * state-routed scene setup, four coordinate stages, and all 40 calls. */

#define FieldScene_RunFourStepActorMotion Func_020020f0

void Func_0200499e();
s32 Func_02004bb4();
void Func_02004dd0();
void Func_02004e4c();
void Func_020057b2();
void Func_02005a8c();
void Func_02006b02();
void Func_02006b10();
void Func_02006b18();
void Func_02006b24();
void Func_02006b32();
void Func_02006b44();
void Func_02006b4e();
void Func_02006b52();
void Func_02006b64();
void Func_02006b72();
void Func_02006ba8();
void *Func_02006bda();
void Func_02006bdc();
void Func_02006bde();
void Func_02006be4();
void Func_02006c00();
void Func_02006c1e();
void Func_02006c26();
void Func_02006c32();
void Func_02006c4c();
void Func_02006c50();
void Func_02006c58();
void Func_02006c68();
void Func_02006c70();
void Func_02006c92();
void Func_02006cac();
void Func_02006cae();
void Func_02006cb2();
void Func_02006cd2();
void Func_02006cd4();
void Func_02006d2a();
void Func_02006d3e();
void Func_02006d56();
void Func_02006d68();

void Func_020020f0(s32 scene)
{
    s32 state;
    void *actor;
    s32 x;
    s32 y;

    if (*(s16 *)0x02000402 == 2) {
        Func_0200499e();
        return;
    }
    Func_02006b18();
    state = Func_02004bb4(scene, 4);
    if (state == 0) {
        Func_02006bde(8383);
        Func_02006c32(196608, 24576);
        Func_02006c4c(56098816, -1, 11010048, 1);
        Func_02006c58();
        Func_02006b4e(30);
        Func_02006c1e(scene, 0);
        Func_02006c26(scene, 0);
        Func_020057b2(0, 816, 200);
        Func_02006ba8(0, 65536, 32768);
        Func_02006bdc(0, 840, 200);
        Func_02006c68(0, 49152, 20);
        Func_02006cd4();
        Func_02006cae(-1, -1, -1, 0);
        Func_02006be4(0, 32768, 16384);
        actor = Func_02006bda(0);
        x = *(s32 *)(actor + 8);
        y = *(s32 *)(actor + 12);
        Func_02006c00(0, 32768, 16384);
        Func_02006c50(0, 10);
        Func_02006b02(actor, x, y + 0x60000, *(s32 *)(actor + 16));
        Func_02006b10(actor);
        Func_02006c70(0, 14);
        Func_02006b24(actor, x + 0x400000, y + 0x60000, *(s32 *)(actor + 16));
        Func_02006b32(actor);
        Func_02006c92(0, 10);
        Func_02006b44(actor, x + 0x400000, y + 0x3c0000, *(s32 *)(actor + 16));
        Func_02006b52(actor);
        Func_02006cb2(0, 15);
        Func_02006b64(actor, x + 0x700000, y + 0x3c0000, *(s32 *)(actor + 16));
        Func_02006b72(actor);
        Func_02006cd2(0, 12);
        Func_02006d2a(scene, 0);
        Func_02005a8c(0);
        Func_02006d68(0, 0);
        Func_02004dd0(scene, 4);
    } else if (state == 1) {
        Func_02006d3e(0x20be);
        Func_02006d56(scene, 0);
    }
    Func_02004e4c(state, scene, 4);
    Func_02006cac();
}
