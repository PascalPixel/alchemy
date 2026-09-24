/* NONMATCHING: 572 bytes, candidate 564, 262 differing halfwords
 * (2026-09-24). Single-overlay unit binding Engine_* at their import veneers.
 * Remaining: the body matches instruction for instruction (0x200a820 and the
 * 254 mask hoisted to r7/r8, counter r5 from 1 down); the reference also sets
 * sl to 0 before the loop and never reads it, which costs the sl save and
 * restore (8 bytes) and shifts everything after. Not the dbra reversal
 * (has_call forbids it), not a bitfield (that gives a -2 mask); some dead
 * zero pseudo survives to global-alloc there. */
#include "TYPES.H"

void Engine_EventBegin();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorSetSpeed();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorWalkTo();
void Engine_ActorEnableActionCallback();
void Engine_ActorWaitForMove();
void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_CameraMoveToActor();
void Engine_ActorWalkToAndWait();
void Engine_CameraWaitForMove();
void Engine_MapCopyCellAttributes();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Local_02001730(void)
{
    s32 i;
    s32 record;

    Engine_EventBegin();
    record = Engine_ActorGet(12);
    *(s32 *)(record + 24) = -0x10000;
    record = Value1(Engine_ActorGet, 13);
    *(s32 *)(record + 24) = -0x10000;
    record = Engine_ActorGet(14);
    *(s32 *)(record + 24) = -0x10000;
    Call3(Engine_ActorSetPosition, 3, 0x880000, 0xb80000);
    Call3(Engine_ActorSetPosition, 0, 0x880000, 0x1280000);
    Call3(Engine_ActorSetPosition, 8, 0x880000, 0x980000);
    Call3(Engine_ActorSetSpeed, 3, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 8, 0x18000, 0xc000);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x880000, -1, 0xb80000, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_ActorRunRepeatedMotion(3, 1);
    *(u8 *)(Engine_ActorGet(8) + 90) &= 254;
    Engine_EventWait(20);
    for (i = 1; i >= 0; i--) {
        Engine_ActorWalkTo(3, 152, 168);
        Engine_EventWait(10);
        Call2(Engine_ActorEnableActionCallback, 8, 0x200a8c8);
        Engine_ActorWaitForMove(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        *(u8 *)(Engine_ActorGet(3) + 90) &= 254;
        Engine_ActorWalkTo(3, 136, 184);
        Engine_EventWait(10);
        Engine_ActorEnableActionCallback(8, 0x200a820);
        Engine_ActorWaitForMove(3);
        *(u8 *)(Engine_ActorGet(3) + 90) |= 1;
        Engine_EventWait(30);
        Engine_ActorWalkTo(3, 120, 168);
        Engine_EventWait(5);
        Call2(Engine_ActorEnableActionCallback, 8, 0x200a874);
        Engine_ActorWaitForMove(3);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 30);
        Engine_ActorRunRepeatedMotion(3, 1);
        Engine_EventWait(15);
        *(u8 *)(Engine_ActorGet(3) + 90) &= 254;
        Engine_ActorWalkTo(3, 136, 184);
        Engine_EventWait(15);
        Engine_ActorEnableActionCallback(8, 0x200a820);
        Engine_ActorWaitForMove(3);
        Engine_ActorRunRepeatedMotion(3, 1);
        *(u8 *)(Engine_ActorGet(3) + 90) |= 1;
    }
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 3, 0x102, 60);
    record = Engine_ActorGet(3);
    *(s32 *)(record + 108) = 0x2008d59;
    Engine_CameraMoveToActor(0, 1);
    Engine_EventWait(30);
    Call3(Engine_ActorWalkToAndWait, 0, 136, 0x108);
    Engine_CameraWaitForMove();
    Call6(Engine_MapCopyCellAttributes, 0, 0, 3, 3, 7, 9);
    Engine_EventEnd();
}
