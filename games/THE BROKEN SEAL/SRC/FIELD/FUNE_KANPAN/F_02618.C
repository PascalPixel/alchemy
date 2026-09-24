#include "TYPES.H"

void Engine_EventBegin();
void Main_0808a030();
void Engine_TaskWait();
void Engine_CameraFollowActor();
void Engine_ActorSetAnimation();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetPosition();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void FieldScene_RunScene3af_02000bb8();
s32 Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
s32 FieldScene_CallPairWith10();
void Engine_ActorJump();
void Engine_AudioPlayCue();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorFaceDirection();
s32 Engine_ActorSetDestination();
void Engine_ActorSetSpritePriority();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_ActorWalkTo();
void Engine_ActorSetAttachedEffect();
void Engine_GameStateSetReturn();
void Engine_Import0808a250();
void Engine_EventEnd();


extern u8 Data_0000006d[];
extern u8 Data_0000006f[];
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][1];

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Runs the deck scene: the leader jumps and walks, actors 22 and 25 move into place, and the scene sets its return map. */
void FuneKanpan_Func02002618(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Call1(Main_0808a030, 0x200d418);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(25, 1);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(21, 5);
    record = Engine_ActorGet(21);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetPosition(0, 0, 0);
    record = Engine_ActorGet(0);
    {
        s32 shown = 0x4000;
    
        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    Call3(Engine_ActorSetPosition, 0, 0xd80000, 0x24a0000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 0, 216, 0x256);
    Engine_EventWait(20);
    Value2(FieldScene_CallPairWith10, 0, 0x6000);
    Engine_ActorJump(0, 2, 10);
    Call3(Engine_ActorSetSpeed, 0, 0x19999, 0xcccc);
    Call3(Engine_ActorWalkToAndWait, 0, 194, 0x270);
    Engine_AudioPlayCue(181);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
    {
        /* FAKEMATCH: a one-halfword struct zero is loaded from a halfword pool entry with the reach the reference pool placement needs */
        struct { u16 v; } zero;

        zero.v = 0;
        *(u8 *)(Engine_ActorGet(25) + 85) = zero.v;
    }
    Call3(Engine_ActorSetSpeed, 25, 0x20000, 0x10000);
    Call3(Engine_ActorSetDestination, 25, 216, 0x264);
    Engine_AudioPlayCue(149);
    Engine_ActorSetSpritePriority(22, 2);
    Engine_ActorSetAnimation(22, 5);
    record = Engine_ActorGet(22);
    *(s32 *)(record + 40) = 0x80000;
    *(s32 *)(record + 72) = 0xb333;
    *(s32 *)(record + 24) = 0x1a000;
    *(s32 *)(record + 28) = 0x1a000;
    *(s32 *)(record + 108) = 0x20088c1;
    *(s32 *)(record + 68) = 0x8000;
    Call3(Engine_ActorSetSpeed, 22, 0x60000, 0x30000);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 22, 182, 0x26a);
    record = Engine_ActorGet(22);
    Engine_ActorSetSpriteFlags(record, 0);
    Value2(FieldScene_CallPairWith10, 0, 0xa000);
    Engine_ActorJump(0, 6, 80);
    Call3(Engine_ActorSetDestination, 25, 232, 0x234);
    Call3(Engine_ActorWalkToAndWait, 0, 204, 0x262);
    Call3(Engine_ActorWalkToAndWait, 0, 208, 0x256);
    Call3(Engine_ActorWalkToAndWait, 0, 248, 0x256);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3(Engine_ActorWalkTo, 0, 250, 0x248);
    Call3(Engine_ActorWalkTo, 1, 240, 0x258);
    Call3(Engine_ActorWalkTo, 2, 254, 0x258);
    Call3(Engine_ActorWalkToAndWait, 3, 248, 0x268);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 20);
    Engine_AudioPlayCue(149);
    Engine_EventWait(40);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 2, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Engine_EventWait(60);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 3, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkTo, 0, 248, 0x234);
    Call3(Engine_ActorWalkTo, 1, 248, 0x234);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkTo, 2, 248, 0x234);
    Call3(Engine_ActorWalkTo, 3, 248, 0x234);
    Engine_EventWait(20);
    Data_02000240_t[226][0] = (s32)Data_0000006f;
    do { Data_02000240_t[227][0] = 30; } while (0); /* FAKEMATCH: keeps row 227 from being derived from row 226 */
    ((u8 *)Data_02000240_t)[0x22b] = 3;
    Engine_GameStateSetReturn((s32)Data_0000006d, 16);
    Engine_Import0808a250(62, 3);
    Engine_EventEnd();
}
