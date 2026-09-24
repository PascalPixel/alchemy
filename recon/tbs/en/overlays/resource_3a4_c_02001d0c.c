/* NONMATCHING: 788 of 788 bytes, 29 differing halfwords (2026-09-24). The
 * owner starts at 0x02001d0c: the four prologue instructions before the old
 * region entry 0x02001d14 belong to it (region, source-paths key and
 * overlay-assembly start moved). Written on the 3a4:02001838 pattern: the
 * motion callbacks are Value_ link symbols, the zero is a one-halfword struct
 * (its short pool reach forces the mid-function pool as in the ROM), and the
 * step addresses are kept in p8/p11. Binds Data_03001c94. Remaining: sched2
 * order in the two motion setups; the reference stores phase (+104) first,
 * then the steps, then +72 and the callback; here the +104 store has no
 * memory dependence on the step stores and sinks. */
#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorStop();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_AudioPlayCue();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_ActorFaceDirection();
void Engine_ActorSetSpeed();
void Engine_ObjectMotionSetPositionAndCommit();
u8 * Engine_ActorSetAnimation();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorSetSpriteFlags();
void SceneState_StoreParamsAndInstallTask();
void Engine_GameFlagSet();
void Engine_EventEnd();


extern u8 Data_00000000[];

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

struct ActorMotion {
    u8 pad0[100];
    u16 step[2];
    s32 phase;
    s32 callback;
};

struct Half { u16 v; };
extern u8 Value_02009771;
extern s32 Data_03001c94;

void Func_02001d0c(void)
{
    struct Half zero;
    u8 *p8;
    u8 *p11;
    u8 *rec3;
    u8 *record;
    u32 n;

    rec3 = (u8 *)Value1(Engine_ActorGet, 10);
    Engine_EventBegin();
    Engine_ActorStop(10);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x1170000, 0x400000, 0xd80000, 1);
    Engine_CameraWaitForMove();
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 20);
    Call3(Engine_ActorFaceDirection, 10, 0x8000, 40);
    Call2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x800000, 0x400000, 0xca0000, 1);
    zero.v = 0;
    p11 = rec3 + 102;
    *(s32 *)(rec3 + 104) = 0;
    p8 = rec3 + 100;
    *(s32 *)(rec3 + 72) = 0x6666;
    *(u16 *)p11 = 0;
    *(u16 *)p8 = 0;
    *(s32 *)(rec3 + 108) = (s32)&Value_02009771;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 212, 200);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 103, 200);
    *(s32 *)(rec3 + 108) = 0;
    rec3[91] = zero.v;
    Engine_EventWait(10);
    Engine_ActorSetAnimation(10, 1);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 40);
    *(u8 *)(Engine_ActorGet(10) + 90) &= 254;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_AudioPlayCue(153);
    record = Engine_ActorGet(10);
    *(s32 *)((s32)record + 40) = 0x40000;
    Call2((void (*)())Engine_ActorSetAnimation, 10, 3);
    Engine_ObjectMotionSetPositionAndCommit(10, 86, 214);
    Engine_ActorSetAnimation(10, 1);
    record = Engine_ActorGet(10);
    Engine_ActorSetSpriteFlags((s32)record, 1);
    Engine_EventWait(10);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0, 0x10000);
    Engine_EventWait(8);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    {
        u8 *record = Engine_ActorGet(10);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Call3(Engine_ActorFaceDirection, 10, 0x3000, 20);
    Engine_ActorFaceDirection(10, 0, 40);
    *(s32 *)(rec3 + 104) = 0;
    *(u16 *)p8 = 0;
    *(u16 *)p11 = 0;
    *(s32 *)(rec3 + 108) = (s32)&Value_02009771;
    Call3(Engine_ActorSetSpeed, 10, 0x13333, 0x9999);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 10, 120, 215);
    *(s32 *)(rec3 + 108) = 0;
    rec3[91] = zero.v;
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(16);
    Engine_AudioPlayCue(229);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0, 0x10000);
    Engine_EventWait(4);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(10, 0, 10);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimation(10, 3);
    Call4(SceneState_StoreParamsAndInstallTask, 0x820000, 0, 0xa80000, 0);
    Engine_EventWait(60);
    n = 0;
    if (Data_03001c94 == 0) {
        do {
            n++;
            Engine_EventWait(1);
            if (n > 59) {
                break;
            }
        } while (Data_03001c94 == 0);
    }
    rec3 = (u8 *)Engine_ActorGet(0);
    Call2(Engine_CameraSetSpeed, 0x4cccc, 0x9999);
    Engine_CameraMoveTo(*(s32 *)(rec3 + 8), *(s32 *)(rec3 + 12), *(s32 *)(rec3 + 16), 1);
    Engine_CameraWaitForMove();
    Call1(Engine_GameFlagSet, 0x905);
    Engine_EventEnd();
}
