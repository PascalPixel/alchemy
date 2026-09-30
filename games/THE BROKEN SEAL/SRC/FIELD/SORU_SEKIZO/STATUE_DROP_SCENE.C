#include "TYPES.H"
#include "CALL.H"
extern u8 MsgSoruSomethingClicked[];

u8 * Engine_ActorGet();
void SetMapCellCollision();
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

void SoruSekizo_RunStatueDropScene(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *p5;
    s32 zero;

    rec8 = Engine_ActorGet(17);
    Call4(SetMapCellCollision, 2, 0x1100000, 0x800000, 0);
    Call4(SetMapCellCollision, 2, 0x1200000, 0x800000, 0);
    if ((s32)rec8 == 0) {
    } else {
        p5 = *(s32 *)((s32)rec8 + 16);
        Engine_EventBegin();
        if (((s32)p5 >> 20) != 8) {
        } else {
            if (Engine_GameFlagIsSet(0x207) == 0) {
                record = Engine_ActorGet(0);
                if ((u32)(*(s32 *)(record + 16) >> 19) <= 17) {
                    Call3(Engine_ActorWalkToAndWait, 0, 0x121, 158);
                    record = Engine_ActorGet(0);
                    {
                        s32 shown = 0xc000;
                    
                        *(u16 *)(record + 6) = shown;
                    }
                }
            }
            if (Engine_GameFlagIsSet(0x816) == 0) {
            } else {
                if (Engine_GameFlagIsSet(0x817) == 0) {
                } else {
                    Engine_GameFlagSet(0x818);
                    Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
                    Call4(Engine_CameraMoveTo, 0x11e0000, -1, 0x920000, 1);
                    Engine_CameraWaitForMove();
                    *(u8 *)(Engine_ActorGet(17) + 90) &= 254;
                    Value3(Engine_ActorSetSpeed, 17, 0x30000, 0x10000);
                    zero = 0;
                    rec8[85] = zero;
                    Engine_ActorSetSpritePriority(17, 3);
                    Engine_AudioPlayCue(189);
                    Engine_ActorSetDestination(17, 0x120, 178);
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
                    Engine_AudioPlayCue(0x121);
                    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
                    Engine_EventWait(10);
                    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
                    Engine_EventWait(60);
                    Engine_AudioPlayCue(188);
                    if (Engine_GameFlagIsSet(0x80b) != 0) {
                        if (Engine_GameFlagIsSet(0x80c) != 0) {
                            if (Engine_GameFlagIsSet(0x80d) != 0) {
                                if (Engine_GameFlagIsSet(0x80e) != 0) {
                                    Engine_GameFlagSet(0x80f);
                                }
                            }
                        }
                    }
                    Engine_EventWait(40);
                    Engine_MessageShowCentered((s32)MsgSoruSomethingClicked, 1);
                    Engine_MapCopyCellAttributes(0, 1, 2, 1, 17, 8);
                    Engine_MapCopyCellAttributes(17, 9, 2, 1, 17, 7);
                }
            }
        }
        Engine_EventEnd();
    }
}
