#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_AudioPlayCue();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Effect_Spawn();
void Engine_EventWait();
void Main_0808a5e8();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagSet();
void Engine_EventEnd();



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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

void ShianJiin_Func02000518(void)
{
    s32 *p8;
    s32 rec8;
    s32 record;
    u8 slot16[40];

    rec8 = Value1(Engine_ActorGet, 9);
    Engine_EventBegin();
    Call1(Engine_EventSetMessage, 0x17b4);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Call3(Engine_ActorWalkToAndWait, 0, 168, 0x188);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
    Engine_AudioPlayCue(132);
    record = Engine_ActorGet(9);
    *(s32 *)(record + 40) = 0x140000;
    record = Engine_ActorGet(9);
    *(s32 *)(record + 72) = 0x40000;
    Call3(Engine_ActorSetSpeed, 9, 0x30000, 0x18000);
    Call3(Engine_ActorSetDestination, 9, 152, 0x188);
    Engine_ActorWaitForMove(9);
    record = Engine_ActorGet(9);
    *(s32 *)(record + 72) = 0x10000;
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_AudioPlayCue(132);
    p8 = (s32 *)slot16;
    p8[1] = 7;
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0x8000, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), -0x8000, 0, 0, 0x10000, p8);
    Engine_EventWait(30);
    Main_0808a5e8();
    Call6(Engine_MapCopyCellAttributes, 10, 24, 1, 1, 10, 22);
    Call1(Engine_GameFlagSet, 0x892);
    Engine_EventEnd();
}
