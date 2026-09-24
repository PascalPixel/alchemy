#include "TYPES.H"

u8 * Engine_ActorGet();
void Main_08009278();
void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorWalkToAndWait();
void Engine_GameFlagSet();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
s32 Engine_ActorSetSpeed();
void Engine_ActorSetSpritePriority();
void Engine_AudioPlayCue();
void Engine_ActorSetDestination();
s32 Engine_EventWait();
void Engine_ActorSetPosition();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MessageShowCentered();
void Engine_MapCopyCellAttributes();
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void SoruSekizo_RunStatueDropScene(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *p5;
    s32 zero;

    rec8 = Engine_ActorGet(17);
    Call4(Main_08009278, 2, 0x1100000, 0x800000, 0);
    Call4(Main_08009278, 2, 0x1200000, 0x800000, 0);
    if ((s32)rec8 == 0) {
    } else {
        p5 = *(s32 *)((s32)rec8 + 16);
        Engine_EventBegin();
        if (((s32)p5 >> 20) != 8) {
        } else {
            if (Value1(Engine_GameFlagIsSet, 0x207) == 0) {
                record = Value1(Engine_ActorGet, 0);
                if ((u32)(*(s32 *)(record + 16) >> 19) <= 17) {
                    Call3(Engine_ActorWalkToAndWait, 0, 0x121, 158);
                    record = Engine_ActorGet(0);
                    {
                        s32 shown = 0xc000;
                    
                        *(u16 *)(record + 6) = shown;
                    }
                }
            }
            if (Value1(Engine_GameFlagIsSet, 0x816) == 0) {
            } else {
                if (Value1(Engine_GameFlagIsSet, 0x817) == 0) {
                } else {
                    Call1(Engine_GameFlagSet, 0x818);
                    Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
                    Call4(Engine_CameraMoveTo, 0x11e0000, -1, 0x920000, 1);
                    Engine_CameraWaitForMove();
                    *(u8 *)(Engine_ActorGet(17) + 90) &= 254;
                    Value3(Engine_ActorSetSpeed, 17, 0x30000, 0x10000);
                    zero = 0;
                    rec8[85] = zero;
                    Engine_ActorSetSpritePriority(17, 3);
                    Engine_AudioPlayCue(189);
                    Call3(Engine_ActorSetDestination, 17, 0x120, 178);
                    ((void (*)())Engine_EventWait)(8);
                    Call3(Engine_ActorSetPosition, 18, 0x1200000, 0xb20000);
                    *(s32 *)((s32)rec8 + 56) = -0x80000000;
                    *(s32 *)((s32)rec8 + 60) = -0x80000000;
                    *(s32 *)((s32)rec8 + 64) = -0x80000000;
                    *(s32 *)((s32)rec8 + 8) = zero;
                    *(s32 *)((s32)rec8 + 12) = zero;
                    *(s32 *)((s32)rec8 + 16) = zero;
                    *(s32 *)((s32)rec8 + 36) = zero;
                    *(s32 *)((s32)rec8 + 40) = zero;
                    *(s32 *)((s32)rec8 + 44) = zero;
                    Engine_ActorSetPosition(17, 0, 0);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Engine_AudioPlayCue(141);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
                    Engine_EventWait(35);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
                    Engine_EventWait(20);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
                    Engine_EventWait(30);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
                    Engine_EventWait(40);
                    Call1(Engine_AudioPlayCue, 0x121);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
                    Engine_EventWait(60);
                    Engine_AudioPlayCue(188);
                    if (Value1(Engine_GameFlagIsSet, 0x80b) != 0) {
                        if (Value1(Engine_GameFlagIsSet, 0x80c) != 0) {
                            if (Value1(Engine_GameFlagIsSet, 0x80d) != 0) {
                                if (Value1(Engine_GameFlagIsSet, 0x80e) != 0) {
                                    Call1(Engine_GameFlagSet, 0x80f);
                                }
                            }
                        }
                    }
                    Engine_EventWait(40);
                    Call2(Engine_MessageShowCentered, 0x1038, 1);
                    Call6(Engine_MapCopyCellAttributes, 0, 1, 2, 1, 17, 8);
                    Call6(Engine_MapCopyCellAttributes, 17, 9, 2, 1, 17, 7);
                }
            }
        }
        Engine_EventEnd();
    }
}
