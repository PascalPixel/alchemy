#include "TYPES.H"

extern u8 Data_00001c6f[];

void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorStartRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
s32 Engine_ActorShowEmote();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorSetDestination();
void Main_08009190();
void Main_08009188();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
s32 Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
s32 Engine_GameFlagIsSet();
void Engine_EventShowMessage();
u8 * Engine_ActorGet();
void Engine_TaskWait();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_ActorJump();
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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void HaidiaBabi_RunEventScript01(void)
{
    u32 i;
    u8 *record;
    s32 base5_1c6f;

    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 0, 0x239, 0x189);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Call1(Engine_EventSetMessage, 0x1c66);
    Engine_EventShowMessageAndWait(8, 0, 80);
    Call3(Engine_ActorShowEmote, 8, 0x101, 60);
    Engine_ActorStartRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(80);
    Call3(Engine_ActorSetSpeed, 8, 0xcccc, 0x6666);
    Call3(Engine_ActorSetDestination, 8, 0x248, 0x196);
    Main_08009190(11);
    Main_08009188(12);
    Engine_ActorSetAnimation(8, 12);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorShowEmote, 8, 0x108, 40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorShowEmote, 0, 0x105, 60);
    Engine_ActorSetAnimationAndWait(8, 13);
    Call3(Engine_ActorShowEmote, 8, 0x103, 0);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 12);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 0, 0x102, 60);
    Engine_ActorSetAnimation(8, 13);
    Value2(Engine_EventOpenMessage, 8, 0);
    if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    if (Value1(Engine_GameFlagIsSet, 0x81c) != 0) {
        Call3(Engine_ActorShowEmote, 8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Call3(Engine_ActorShowEmote, 8, 0x107, 60);
    /* FAKEMATCH: message 0x1c6f comes from a link symbol so GCC keeps it in
     * one register for both messages instead of folding base + 3 into a
     * second pool constant. */
    base5_1c6f = (s32)Data_00001c6f;
    Engine_EventSetMessage(base5_1c6f);
    Value2(Engine_EventOpenMessage, 8, 0);
    if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    if (Value1(Engine_GameFlagIsSet, 0x81c) != 0) {
        Call3(Engine_ActorShowEmote, 8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Call3(Engine_ActorShowEmote, 8, 0x107, 60);
    Engine_EventSetMessage((base5_1c6f + 3));
    Engine_EventShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 13);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    record = Engine_ActorGet(0);
    {
        s32 shown = 0;
    
        *(u16 *)((s32)record + 6) = shown;
    }
    Engine_TaskWait(1);
    *(u8 *)(Engine_ActorGet(0) + 90) &= 254;
    Call3(Engine_ActorSetDestination, 0, 0x22e, 0x184);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    Engine_ActorSetAnimation(8, 14);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 8, 0x24a, 0x190);
    Engine_EventWait(40);
    Call3(Engine_ActorWalkToAndWait, 8, 0x244, 0x17e);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 40);
    {
        u8 *record = Engine_ActorGet(0);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Engine_ActorFaceDirection, 8, 0xc000, 8);
    Engine_ActorFaceDirection(8, 0, 8);
    Call3(Engine_ActorFaceDirection, 8, 0x4000, 8);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 10);
    Engine_ActorJump(8, 4, 20);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorJump(8, 4, 20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Call3(Engine_ActorSetSpeed, 8, 0x6666, 0x3333);
    Call3(Engine_ActorWalkToAndWait, 8, 0x23c, 0x180);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Call1(Engine_GameFlagSet, 0x81e);
    Call1(Engine_GameFlagSet, 0x203);
    Engine_EventEnd();
}
