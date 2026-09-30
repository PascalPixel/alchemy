#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKorimaGoingCrossBridge[];

/* Actor 8's path and the debris script, laid out after the code. */
extern u8 KorimaMura_Actor8Path[];
extern u8 KorimaMura_DebrisScript[];

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
void Object_SetTargetAndCallback();
void Engine_GameFlagSet();
void Engine_EventEnd();

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
    Engine_EventSetMessage((s32)MsgKorimaGoingCrossBridge);
    Engine_EventShowMessage(8, 0);
    Engine_CameraSetSpeed(0x6666, 0xccc);
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
    Engine_MapCopyCellsTo(91, 0, 72, 9, v5, v6);
    Engine_EventWait(12);
    Engine_MapCopyCellsTo(91, 4, 72, 9, v5, v6);
    Engine_EventWait(9);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Engine_MapCopyCellsTo(91, 8, 72, 9, v5, v5);
    Engine_EventWait(6);
    Engine_MapCopyCellsTo(91, 13, 72, 9, v5, 6);
    Engine_EventWait(3);
    Engine_AudioPlayCue(188);
    for (i = 0; i < 10; i++) {
        u8 *obj = (u8 *)Engine_ObjectCreate(222, (148 + i * 4) << 16, 0, 0x1020000);

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
            Engine_ObjectSetScript((s32)obj, (s32)KorimaMura_DebrisScript);
        }
    }
    Call6(Engine_MapCopyCellsTo, 91, 19, 72, 9, 5, 7);
    Call6(Engine_MapCopyCellAttributes, 23, 11, 5, 7, 8, 11);
    Call3(Engine_WorkSetValuesIfNonNegative, 0, 0x40000, 0x10000);
    Engine_EventWait(10);
    Engine_ActorJump(0, 6, 0);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Call3(Object_SetTargetAndCallback, 8, 0x10000, (s32)KorimaMura_Actor8Path);
    Engine_EventWait(60);
    Engine_ActorShowEmote(0, 0x102, 60);
    Engine_GameFlagSet(0x847);
    Engine_EventEnd();
}
