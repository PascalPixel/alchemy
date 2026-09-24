#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "EVENT_RUNTIME.H"
extern struct EventRuntime *Data_03001ebc;
void Func_0200a3a4();
s32 Func_0200a46c();
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_SetupStagedActors(void)
{
    s32 record;

    Event_Begin();
    Data_03001ebc->value_1c0 = 0x100;
    Data_03001ebc->value_1c8 = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(8, 0x2400000, 0xe80000);
    Event_Wait(1);
    Event_SetMessage(0x101a);
    Func_0200a3a4(8, 6);
    Actor_SetPosition(8, 0x2400000, 0x1180000);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 0x240, 216);
    Event_Wait(20);
    Actor_Jump(5, 2, 0);
    Event_Wait(30);
    Func_0200a3a4(5, 6);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(6);
    Actor_FaceDirection(8, 0x9000, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(10);
    Actor_FaceDirection(8, 0xc000, 0);
    Event_Wait(10);
    Actor_Jump(8, 6, 0);
    Actor_SetSpeed(8, 0x30000, 0x20000);
    Actor_WalkToAndWait(8, 0x240, 184);
    Event_Wait(40);
    Func_0200a3a4(8, 6);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(40);
    Data_03001ebc->value_1c0 = 0x202;
    Data_03001ebc->value_1c8 = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(8, 0x1a80000, 0xc80000);
    Task_Wait(1);
    Func_0200a3a4(8, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Actor_SetPosition(8, 0x2400000, 0xb80000);
    Camera_MoveTo(0x23e0000, -1, 0xb40000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x23e0000, -1, 0x9d0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(1, 2);
    Func_0200a3a4(1, 20);
    Actor_SetAnimationAndWait(5, 3);
    Func_0200a3a4(5, 6);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 4);
    Func_0200a3a4(8, 80);
    Actor_ShowEmote(8, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(8, 0xc000, 0);
    Event_Wait(30);
    Func_0200a3a4(8, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(5, 0x101, 0);
    Event_Wait(60);
    Actor_FaceDirection(8, 0xc000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(8, 4);
    Func_0200a3a4(8, 6);
    Actor_FaceEachOther(0, 1, 0);
    Event_Wait(40);
    Actor_FaceEachOther(0, 5, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(8, 3);
    Func_0200a3a4(8, 6);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Event_Wait(40);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 3);
    Func_0200a3a4(8, 6);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x2400000, -1, 0xd70000, 1);
    Actor_FaceDirection(8, 0x4000, 0);
    Event_Wait(10);
    Actor_Jump(8, 6, 0);
    Actor_WalkToAndWait(8, 0x240, 217);
    Event_Wait(20);
    Actor_WalkToAndWait(8, 0x240, 0x141);
    Actor_SetPosition(8, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Camera_MoveTo(0x2400000, -1, 0x880000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(5, 0x10000, 0x8000);
    Actor_SetAnimation(1, 2);
    record = Value1(Func_0200a46c, 0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(5, 2);
    record = Value1(Func_0200a46c, 0);
    if (record != 0) {
        Actor_SetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(5);
    Actor_SetPosition(5, 0, 0);
    GameFlag_Clear(0x12f);
    Data_03001ebc->value_1c0 = 0x204;
    Data_03001ebc->value_1c8 = 16;
    Event_End();
}
