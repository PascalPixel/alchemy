#include "TYPES.H"

struct ActorMotion {
    u8 pad0[100];
    u16 step[2];
    s32 phase;
    s32 callback;
};

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_AudioPlayCue();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ObjectMotionSetPositionAndCommit();
s32 Engine_ActorSetAnimation();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorFaceDirection();
void FieldScene_RunActorTenFourStepSequence();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetSpriteFlags();
void Main_0808a168();
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

struct Half { u16 v; };
extern u8 Value_02009771;
extern u8 Value_0200bd34;

/* The zero is a one-halfword struct, so its movhi pool load reaches 64 bytes
 * and the pool lands where the ROM has it; the motion callbacks are Value_
 * link symbols. */
void ArutinYama_Func02001838(void)
{
    struct Half p10;
    s32 p8;
    u8 *rec3;
    s32 rec7;
    s32 record;
    s32 v6;

    rec3 = Value1(Engine_ActorGet, 10);
    Engine_EventBegin();
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x12a0000, -1, 0x1510000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x1270000, 0x200000, 0xd40000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    p8 = (s32)rec3 + 100;
    p10.v = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    *(s32 *)((s32)rec3 + 72) = 0x6666;
    ((struct ActorMotion *)rec3)->callback = (s32)&Value_02009771;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x134, 0x123);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x137, 215);
    ((struct ActorMotion *)rec3)->callback = 0;
    rec3[91] = p10.v;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    FieldScene_RunActorTenFourStepSequence();
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 40);
    Call4(Engine_CameraMoveTo, 0x14e0000, -1, 0xf40000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&Value_02009771;
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x140, 232);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x154, 0x106);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x176, 0x106);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_EventWait(16);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0xf000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0xd000, 40);
    Engine_AudioPlayCue(153);
    record = Engine_ActorGet(10);
    *(s32 *)(record + 40) = 0x40000;
    Engine_ActorSetAnimation(10, 2);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x17c, 248);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    Call4(Engine_CameraMoveTo, 0x1300000, -1, 0xd70000, 1);
    ((struct ActorMotion *)rec3)->phase = 0;
    ((struct ActorMotion *)rec3)->step[0] = 0;
    ((struct ActorMotion *)rec3)->step[1] = 0;
    ((struct ActorMotion *)rec3)->callback = (s32)&Value_02009771;
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x149, 219);
    ((struct ActorMotion *)rec3)->callback = 0;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    *(u8 *)(Engine_ActorGet(9) + 85) = p10.v;
    Call6(Engine_MapCopyCellAttributes, 3, 0, 1, 1, 17, 13);
    Call6(Engine_MapCopyCellAttributes, 3, 0, 1, 1, 18, 13);
    Call6(Engine_MapCopyCellAttributes, 3, 0, 1, 1, 19, 13);
    Call3(Engine_ActorSetSpeed, 10, 0x16666, 0xb333);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_AudioPlayCue(153);
    v6 = 160;
    record = Engine_ActorGet(10);
    *(s32 *)(record + 40) = (v6 << 11);
    Call2((void (*)())Engine_ActorSetAnimation, 10, 3);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x127, 215);
    Engine_ActorSetAnimation(10, 1);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_AudioPlayCue(153);
    record = Engine_ActorGet(10);
    *(s32 *)(record + 40) = (v6 << 11);
    Engine_ActorSetAnimation(10, 3);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 0x104, 215);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Call6(Engine_MapCopyCellAttributes, 4, 0, 1, 1, 17, 13);
    Call6(Engine_MapCopyCellAttributes, 2, 0, 1, 1, 18, 13);
    Call6(Engine_MapCopyCellAttributes, 4, 0, 1, 1, 19, 13);
    rec7 = Engine_ActorGet(0);
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Engine_CameraMoveTo(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Engine_CameraWaitForMove();
    Call3(Main_0808a168, 10, 0x10000, (s32)&Value_0200bd34);
    Call1(Engine_GameFlagSet, 0x904);
    Engine_EventEnd();
}
