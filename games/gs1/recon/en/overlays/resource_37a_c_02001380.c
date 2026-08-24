#include "types.h"

/*
 * Starts the Sol Temple trap event: prepare actor 16, build the five paired
 * effect rows, then hand control to the room callbacks.
 */

void Func_02001616();
void Func_0200163e();
void Func_02001666();
void Func_0200168e();
void Func_020016b6();
void Func_020016d6();
void Func_02001750();
void Func_02001778();
void Func_020017a0();
void Func_020017c8();
void Func_020017f0();
void Func_020025ec();
void Func_020039ce();
void Func_02003aa8();
void Func_02003d58();
void Func_02003d96();
void Func_02003db0();
void Func_02003dc2();
void Func_02003dd8();
void Func_02003de2();
void Func_02003de4();
void Func_02003dea();
void Func_02003e00();
void Func_02003e0e();
void Func_02003e12();
void Func_02003e22();
void Func_02003e28();
void Func_02003e3a();
void Func_02003e50();
void Func_02003e5c();
void Func_02003e62();
void Func_02003e68();
void Func_02003e70();
void Func_02003ea8();
void Func_02003eae();
void Func_02003ece();
void Func_02003ed4();
void Func_02003ede();
void Func_02003ef6();
void Func_02003f06();
void Func_02003f1e();
void Func_02003f2e();
void Func_02003f46();
void Func_02003f56();
void Func_02003f6e();
void Func_02003f7e();
void Func_02003f9e();
extern u32 Data_03001ebc;

void Func_02003fbe();
void Func_02003fca();

#define StartSolShindenTrapEvent Func_02001380

void StartSolShindenTrapEvent(void)
{
    u32 scene_state;
    s32 outer_pair;
    s32 second_pair;
    s32 middle_pair;
    s32 fourth_pair;
    s32 inner_pair;

    Func_02003d58();
    Func_020025ec();
    Func_02003de2(4120);
    {
        register s32 actor = 16;
        register s32 scale = 128;
        scale <<= 7;
        Func_02003e0e(actor, scale, 20);
    }
    {
        register s32 actor = 16;
        register s32 scale = 128;
        scale <<= 1;
        Func_02003e22(actor, scale, 0);
    }
    Func_02003de4(16, 6, 30);
    Func_02003e5c(37617664, -1, 11403264, 1);
    Func_02003e68();
    Func_02003d96(30);
    Func_020039ce(32784, 20);
    for (outer_pair = 0; outer_pair != 4; outer_pair++) {
        Func_02003ece(246);
        Func_02001616();
        Func_02003db0(12);
        Func_02003ede(246);
        Func_02001750();
        Func_02003dc2(12);
    }
    for (second_pair = 0; second_pair != 6; second_pair++) {
        Func_02003ef6(246);
        Func_0200163e();
        Func_02003dd8(8);
        Func_02003f06(246);
        Func_02001778();
        Func_02003dea(8);
    }
    for (middle_pair = 0; middle_pair != 8; middle_pair++) {
        Func_02003f1e(246);
        Func_02001666();
        Func_02003e00(6);
        Func_02003f2e(246);
        Func_020017a0();
        Func_02003e12(6);
    }
    for (fourth_pair = 0; fourth_pair != 10; fourth_pair++) {
        Func_02003f46(246);
        Func_0200168e();
        Func_02003e28(4);
        Func_02003f56(246);
        Func_020017c8();
        Func_02003e3a(4);
    }
    for (inner_pair = 0; inner_pair != 12; inner_pair++) {
        Func_02003f6e(246);
        Func_020016b6();
        Func_02003e50(2);
        Func_02003f7e(246);
        Func_020017f0();
        Func_02003e62(2);
    }
    Func_020016d6();
    Func_02003e70(6);
    Func_02003aa8(32784, 6);
    {
        register s32 actor = 16;
        register s32 horizontal_scale = 128;
        register s32 vertical_scale = 128;
        horizontal_scale <<= 10;
        vertical_scale <<= 9;
        Func_02003eae(actor, horizontal_scale, vertical_scale);
    }
    Func_02003ed4(16, 576, 280);
    scene_state = Data_03001ebc;
    *(s32 *)(scene_state + 448) = 256;
    *(s32 *)(scene_state + 456) = 32;
    Func_02003fbe();
    Func_02003fca();
    Func_02003ea8(2067);
    Func_02003f9e(3);
}
