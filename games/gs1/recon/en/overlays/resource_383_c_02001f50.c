#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_SelectAndConfigureActorPair:
 * 61 calls, a signed scene-selector branch, shared-object state, and both
 * workspace transitions across the complete decoded owner. */

#define FieldScene_SelectAndConfigureActorPair Func_02001f50

void Func_0200202a();
void Func_02004318();
void Func_020065f0();
void Func_0200661c();
void Func_02006654();
void Func_0200666a();
void Func_0200668c();
void Func_0200669c();
void Func_020066ca();
void Func_020066d6();
void Func_0200673c();
void Func_02006bdc();
void Func_02006c8a();
void Func_02006c92();
void Func_02006c9e();
void Func_02006d00();
void *Func_02006d4a();
void *Func_02006d52();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6a();
void Func_02006d8e();
void Func_02006da4();
void Func_02006daa();
void Func_02006db6();
void Func_02006db8();
void Func_02006dc2();
void Func_02006dc4();
void Func_02006df0();
void Func_02006df6();
void Func_02006e00();
void Func_02006e04();
void Func_02006e0a();
void Func_02006e1a();
void Func_02006e20();
void Func_02006e22();
void Func_02006e30();
void Func_02006e3e();
void Func_02006e48();
void Func_02006e5a();
void Func_02006e64();
void Func_02006e6c();
void *Func_02006e76();
void Func_02006e86();
void Func_02006e8c();
void *Func_02006e8e();
void Func_02006e9c();
void *Func_02006ea0();
void Func_02006eb4();
void Func_02006ec4();
void Func_02006eca();
void Func_02006ee2();
void Func_02006ee4();
void *Func_02006eec();
void Func_02006ef0();
void *Func_02006ef8();
void Func_02006f7e();
void Func_02006fec();
void Func_02006ff8();

void Func_02001f50(void)
{
    s16 selector;
    s32 selected_actor;
    u8 *workspace;
    u8 *object;
    void *p5;
    void *p51;
    void *p52;
    void *p53;
    void *p59;
    void *p6;
    void *p60;

    Func_02006c9e();
    workspace = *(u8 **)0x03001ebc;
    Func_02006bdc(33606088);
    Func_02006c8a(263);
    Func_02006c92(592);
    p5 = Func_02006d4a(24, 1);
    p6 = Func_02006d52(25, 1);
    Func_02006d5a(2, 1);
    Func_02006d62(0, 1);
    Func_02006d6a(1, 1);
    Func_02006da4(1, 2, 0);
    Func_02006db6(0, 2, 0);
    Func_02006db8(24, 2, 0);
    Func_02006dc2(25, 2, 0);
    Func_02006d00(10);
    selector = *(s16 *)(workspace + 386);
    selected_actor = 24;
    if (selector >= 202 && selector <= 203) {
        Func_02006df6(4772);
        Func_02006e30(25, 258);
        Func_02006df0(25, 2);
        Func_020065f0(25, 20);
        selected_actor = 25;
    }
    if (selector == 201 || selector == 203) {
        Func_02006e20(4771);
        Func_02006e5a(24, 258);
        Func_02006e1a(24, 2);
        Func_0200661c(24, 20);
        selected_actor = 24;
    }
    Func_02006e04(2, 1);
    Func_02006e3e(2, selected_actor, 0);
    Func_02006e48(1, 2, 0);
    Func_02006e48(1, 2);
    Func_02006d8e(20);
    Func_02006e6c(4773);
    Func_02006654(1, 20);
    Func_02006e64(2, 2);
    Func_02006daa(20);
    Func_0200666a(2, 20);
    Func_020066d6(2, selected_actor);
    Func_02006f7e();
    Func_02006dc4(60);
    Func_02006e8c(24, 2);
    Func_0200668c(24, 20);
    Func_02006e9c(25, 2);
    Func_0200669c(25, 20);
    Func_02006ef0(2, 256, 60);
    Func_0200673c();
    Func_02006eb4(1, 2);
    Func_02006ec4(0, 2);
    Func_02006e0a(20);
    Func_020066ca(1, 20);
    Func_02006ee4(2, 1, 0);
    Func_02006e22(20);
    Func_02006eca(2, 3);
    Func_02006e30(40);
    p51 = Func_02006e76(0);
    object = *(u8 **)(p51 + 80);
    object[9] = (object[9] & ~12) | 4;
    p52 = Func_02006e8e(1);
    object = *(u8 **)(p52 + 80);
    object[9] = (object[9] & ~12) | 4;
    p53 = Func_02006ea0(2);
    object = *(u8 **)(p53 + 80);
    object[9] = (object[9] & ~12) | 4;
    *(u32 *)(workspace + 456) = 24;
    *(u32 *)(workspace + 448) = 513;
    Func_02006fec();
    Func_02006ff8();
    Func_02004318();
    Func_02006e48(14, 45, 3, 1, 14, 44);
    Func_02006e86(2131);
    p59 = Func_02006eec(24);
    *(u16 *)((u8 *)(p59) + 100) = 5;
    p60 = Func_02006ef8(25);
    *(u16 *)((u8 *)(p60) + 100) = 4;
    Func_02006e00(33598369, 3200);
    *(u32 *)(workspace + 448) = 521;
    Func_02006ee2();
}
