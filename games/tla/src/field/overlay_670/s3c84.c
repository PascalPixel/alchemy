#include "ov670.h"

void Func_02005a4c(void);
void Func_02005be4(s32 frames);

void Func_02003c84(void)
{
    Func_02005c9c(212);
    do {
        /* FAKEMATCH: the one-pass block ends CSE's block, so each fade builds 0x7fff afresh as the native does; without the three blocks 0x7fff, 0x10000 and 1 stay in callee-saved registers across the fades (differ 68). */
        Ov670_FadeTo(0x7fff, 0);
        Func_02005be4(1);
    } while (0);
    WaitFrames(1);
    Ov670_FadeTo(0x10000, 0);
    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1b4 = 1;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Ov670_TaskAdd(Func_02002d64, 0x480);
    WaitFrames(60);
    Scheduler_RemoveCallbackFar(Func_02002d64);
    WaitFrames(1);

    Func_02005c9c(212);
    do {
        /* FAKEMATCH: as above. */
        Ov670_FadeTo(0x7fff, 0);
        Func_02005be4(1);
    } while (0);
    WaitFrames(1);
    Ov670_CamMove(146 << 18, -1, 248 << 16, 0);
    Func_02005a4c();
    WaitFrames(1);
    Ov670_FadeTo(0x10000, 0);
    Func_02005be4(1);
    WaitFrames(1);
    Ov670_TaskAdd(Func_02002d64, 0x480);
    WaitFrames(60);
    Scheduler_RemoveCallbackFar(Func_02002d64);
    WaitFrames(1);

    Func_02005c9c(212);
    do {
        /* FAKEMATCH: as above. */
        Ov670_FadeTo(0x7fff, 0);
        Func_02005be4(1);
    } while (0);
    WaitFrames(1);
    Ov670_CamMove(248 << 16, -1, 134 << 18, 0);
    Func_02005a4c();
    WaitFrames(1);
    Ov670_FadeTo(0x10000, 0);
    Func_02005be4(1);
    WaitFrames(1);
    Ov670_TaskAdd(Func_02002d64, 0x480);
    WaitFrames(180);
    Scheduler_RemoveCallbackFar(Func_02002d64);
    WaitFrames(1);

    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1b4 = 1;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Ov670_Wait(10);
    Func_02005bc4(10);
}
