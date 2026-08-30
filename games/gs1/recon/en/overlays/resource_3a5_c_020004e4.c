#include "types.h"

/* AUDITED ACTOR-LAYOUT SCRIPT for FieldScene_ConfigureActorLayout:
 * scene-state dispatch, shared actor initialization, and terminal transition. */

#define FieldScene_ConfigureActorLayout Func_020004e4

void Func_0200222c();
void Func_02002240();
void Func_02002246();
void Func_02002254();
void Func_0200226c();
void Func_02002278();
void Func_0200228c();
void Func_020022a0();
void Func_020022be();
void Func_020022d2();
void Func_020022ec();
void Func_020022f8();
void Func_0200230e();
void Func_02002322();
void Func_0200233c();
void Func_02002348();
void Func_02002362();
void Func_0200236c();
void Func_02002380();
void Func_02002394();
void Func_020023a8();
void Func_020023bc();
void Func_020023d6();
void Func_020023e2();
void Func_020023f6();
void Func_02002408();
void Func_02002434();
void Func_0200244c();
void Func_02002460();
void Func_0200246a();
void Func_0200247e();
void Func_02002492();
void Func_020024a4();
void Func_020024ae();
void Func_020024c0();
void Func_020024d4();
void Func_020024e6();
void Func_02002500();
void Func_02002592();
void Func_0200259c();
void Func_020025a6();
void Func_020025b0();
void Func_020025ba();
void Func_020025c4();
void Func_02002652();
void Func_02002678();

void Func_020004e4(void)
{
    s32 i1;
    s16 scene_state;

    Func_02002246(0x201);
    scene_state = *(s16 *)(0x02000240 + 448);
    if (scene_state == 89) {
    Func_0200222c(70, 68, 4, 2, 22, 7);
    Func_02002240(70, 68, 4, 2, 8, 10);
    Func_02002254(70, 68, 4, 2, 23, 21);
    Func_0200226c(70, 68, 4, 1, 23, 23);
    Func_02002278(70, 68, 4, 2, 16, 42);
    Func_0200228c(70, 68, 4, 2, 36, 44);
    Func_020022a0(70, 68, 4, 2, 14, 55);
    } else if (scene_state == 90) {
    Func_020022be(70, 68, 4, 2, 42, 5);
    Func_020022d2(70, 68, 4, 2, 20, 11);
    Func_020022ec(70, 68, 4, 1, 20, 13);
    Func_020022f8(70, 68, 4, 2, 14, 12);
    Func_0200230e(70, 68, 4, 2, 56, 18);
    Func_02002322(70, 68, 4, 2, 7, 22);
    Func_0200233c(70, 68, 4, 1, 7, 24);
    Func_02002348(70, 68, 4, 2, 44, 23);
    Func_02002362(70, 68, 4, 1, 44, 25);
    Func_0200236c(70, 68, 4, 2, 38, 24);
    Func_02002380(70, 68, 4, 2, 26, 28);
    Func_02002394(70, 68, 4, 2, 17, 35);
    Func_020023a8(70, 68, 4, 2, 50, 36);
    Func_020023bc(70, 68, 4, 2, 34, 43);
    Func_020023d6(70, 68, 4, 1, 34, 45);
    Func_020023e2(70, 68, 4, 2, 6, 46);
    Func_020023f6(70, 68, 4, 2, 27, 55);
    Func_02002408(70, 68, 4, 2, 43, 56);
    } else if (scene_state == 91) {
    Func_02002434(69, 99, 4, 2, 8, 16);
    Func_0200244c(69, 99, 4, 2, 6, 20);
    Func_02002460(69, 99, 4, 2, 10, 23);
    Func_0200246a(69, 99, 4, 2, 8, 14);
    Func_0200247e(69, 99, 4, 2, 6, 18);
    Func_02002492(69, 99, 4, 1, 6, 20);
    Func_020024a4(69, 99, 4, 2, 10, 21);
    Func_020024ae(0, 121, 5, 7, 8, 32);
    Func_020024c0(0, 121, 5, 7, 43, 32);
    Func_020024d4(6, 120, 3, 1, 9, 5);
    Func_020024e6(9, 120, 3, 1, 44, 5);
    Func_02002500(9, 0, 3, 3, 9, 6);
    }
    Func_02002592(8, 0, 0);
    Func_0200259c(9, 0, 0);
    Func_020025a6(10, 0, 0);
    Func_020025b0(11, 0, 0);
    Func_020025ba(12, 0, 0);
    Func_020025c4(13, 0, 0);
    for (i1 = 100; i1 <= 107; i1++) {
        Func_02002652(i1, 0, 0);
    }
    if (scene_state != 92) {
        Func_02002678(0, 262144, 65536, 8192, 65536, 32768, 16384);
    }
}
