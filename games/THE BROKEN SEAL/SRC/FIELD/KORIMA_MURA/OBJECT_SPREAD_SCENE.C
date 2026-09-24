#include "TYPES.H"

extern u8 Data_0200ae34[];

void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_ActorShowEmote();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetAnimation();
s32 Engine_ActorGet();
void Engine_ActorJump();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_AudioPlayCue();
void Engine_MapCopyCellsTo();
s32 Engine_ObjectCreate();
void Engine_ActorSetSpriteFlags();
s32 Engine_RandomNext();
void Engine_ObjectSetAnimation();
void Engine_ObjectSetScript();
void Engine_MapCopyCellAttributes();
void Engine_WorkSetValuesIfNonNegative();
void Main_0808a168();
void Engine_GameFlagSet();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void KorimaMura_RunObjectSpreadScene(void)
{
    u32 i;
    s32 v5;
    s32 v6;

    Engine_EventBegin();
    Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
    Call4(Engine_CameraMoveTo, 0xa80000, 0, 0xf60000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 8, 0x100, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Call1(Engine_EventSetMessage, 0x1786);
    Engine_EventShowMessage(8, 0);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Call4(Engine_CameraMoveTo, 0xa80000, 0, 0xea0000, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 0, 174, 0x116);
    Call3(Engine_ActorFaceDirection, 0, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(8, 3);
    Engine_EventShowMessage(8, 0);
    Call3(Engine_ActorFaceDirection, 8, 0x9000, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    *(u8 *)(Engine_ActorGet(8) + 90) &= 254;
    Call3(Engine_ActorSetSpeed, 8, 0x20000, 0x10000);
    Engine_ActorJump(8, 2, 0);
    Engine_ObjectMotionSetPositionAndCommit(8, 224, 197);
    Engine_AudioPlayCue(176);
    Engine_EventWait(10);
    Engine_ObjectMotionSetPositionAndCommit(8, 234, 200);
    Engine_EventWait(10);
    Engine_AudioPlayCue(198);
    v5 = 5;
    v6 = 4;
    Engine_EventWait(30);
    Call6(Engine_MapCopyCellsTo, 91, 0, 72, 9, v5, v6);
    Engine_EventWait(12);
    Call6(Engine_MapCopyCellsTo, 91, 4, 72, 9, v5, v6);
    Engine_EventWait(9);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call6(Engine_MapCopyCellsTo, 91, 8, 72, 9, v5, v5);
    Engine_EventWait(6);
    Call6(Engine_MapCopyCellsTo, 91, 13, 72, 9, v5, 6);
    Engine_EventWait(3);
    Engine_AudioPlayCue(188);
    for (i = 0; i < 10; i++) {
        u8 *obj = (u8 *)Value4(Engine_ObjectCreate, 222, (148 + i * 4) << 16, 0, 0x1020000);

        if (obj != 0) {
            obj[85] = 0;
            Engine_ActorSetSpriteFlags(obj, 0);
            ((struct Flags9 *)*(u8 **)(obj + 80))->mode = 0;
            {
                u16 delay = (((u32)(Engine_RandomNext() * 40)) >> 16) + 40;

                *(u16 *)(obj + 100) = delay;
            }
            *(s32 *)(obj + 36) = (s32)(((i & 3) << 16) + 0x10000) >> 1;
            *(s32 *)(obj + 44) = 0x10000;
            if (i & 1) {
                *(s32 *)(obj + 36) = -((s32)(((i & 3) << 16) + 0x10000) >> 1);
            }
            Engine_ObjectSetAnimation(obj, 1);
            Call2(Engine_ObjectSetScript, (s32)obj, 0x200ae20);
        }
    }
    Call6(Engine_MapCopyCellsTo, 91, 19, 72, 9, 5, 7);
    Call6(Engine_MapCopyCellAttributes, 23, 11, 5, 7, 8, 11);
    Call3(Engine_WorkSetValuesIfNonNegative, 0, 0x40000, 0x10000);
    Engine_EventWait(10);
    Engine_ActorJump(0, 6, 0);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Call3(Main_0808a168, 8, 0x10000, (s32)Data_0200ae34);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 0, 0x102, 60);
    Call1(Engine_GameFlagSet, 0x847);
    Engine_EventEnd();
}
