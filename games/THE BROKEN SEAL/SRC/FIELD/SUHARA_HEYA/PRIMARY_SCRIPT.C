#include "SUHARA.H"

extern u8 MsgSuharaIodem[];

void Motion_LaunchFromFocusedObject();
void Audio_PlayCueFromEventWork(void);

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a register the compiler then keeps across later calls. A
 * value-returning call also sets r0 last of its arguments. */
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/*
 * Runs the primary script for this scene: a long fixed sequence driving
 * actors 10, 19, 20, 21, 30 and 40 through position, pose and timing steps,
 * guarded by an initial skip check.
 */
void Scene_RunPrimaryScript(void)
{
    u8 *record;
    Call1(Engine_GameFlagSet, 2480);
    if (Value1(Engine_GameFlagIsSet, 2442) == 0) {
        Call1(Engine_AudioPlayCue, 30);
        Engine_EventBegin();
        Call4(Engine_CameraMoveTo, 24117248, -1, 6815744, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 368, 160);
        Call3(Engine_ActorFaceDirection, 0, 49152, 0);
        Call4(Motion_LaunchFromFocusedObject, 19, 0, -16, 49152);
        Call1(Engine_ActorWaitForMove, 19);
        Engine_CameraWaitForMove();
        Call1(Engine_EventSetMessage, (s32)MsgSuharaIodem);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 20, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 19, 2);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Call3(Engine_ActorWalkByAndWait, 19, 0, -16);
        Call3(Engine_ActorFaceDirection, 19, 0, 0);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorFaceDirection, 19, 57344, 0);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorFaceDirection, 19, 0, 0);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Call3(Engine_ActorWalkByAndWait, 19, 0, -24);
        Call3(Engine_ActorWalkByAndWait, 19, 48, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorShowEmote, 20, 258, 40);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 21, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorShowEmote, 19, 263, 40);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorStartRepeatedMotion, 20, 2);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorSetSpeed, 0, 78643, 39321);
        Call3(Engine_ActorWalkToAndWait, 0, 368, 104);
        Call3(Engine_ActorWalkByAndWait, 0, 16, 0);
        Call3(Engine_ActorFaceDirection, 0, 0, 0);
        Call1(Engine_EventWait, 20);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorSetAnimationAndWait, 19, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorShowEmote, 19, 258, 50);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 20, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 21, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 40);
        Call2(Engine_ActorSetAnimationAndWait, 19, 3);
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 21, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceEachOther, 19, 0, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 20, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 40);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 21, 0);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 40);
        Call2(Engine_ActorSetAnimation, 0, 3);
        Call2(Engine_ActorSetAnimationAndWait, 19, 3);
        Call1(Engine_EventWait, 30);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorSetAnimationAndWait, 20, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 0, 256, 0);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 40);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 0, 256, 50);
        Call3(Engine_ActorFaceActor, 0, 21, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 30);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 21, 3);
        Call1(Engine_EventWait, 30);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 19, 258, 40);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorSetAnimationAndWait, 20, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 19, 257, 50);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 20);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 20, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 19, 256, 40);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceEachOther, 19, 0, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorSetAnimationAndWait, 20, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 0, 258, 0);
        Call3(Engine_ActorShowEmote, 19, 258, 80);
        Call3(Engine_ActorShowEmote, 21, 258, 50);
        Call2(Engine_EventShowMessage, 21, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 21, 0);
        Call2(Engine_ActorRunRepeatedMotion, 21, 3);
        Call1(Engine_EventWait, 20);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceDirection, 19, 49152, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_ActorRunRepeatedMotion, 19, 2);
        Call1(Engine_EventWait, 10);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 20, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 20, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceActor, 0, 20, 0);
        Call3(Engine_ActorFaceDirection, 19, 8192, 0);
        Call1(Engine_EventWait, 30);
        Call2(Engine_ActorSetAnimationAndWait, 19, 3);
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorStartRepeatedMotion, 20, 2);
        Call2(Engine_ActorRunRepeatedMotion, 21, 2);
        Call1(Engine_EventWait, 30);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorFaceEachOther, 19, 0, 20);
        Call3(Engine_ActorWalkByAndWait, 19, -12, 0);
        Call1(Engine_EventWait, 20);
        Value2(Engine_EventOpenMessage, 19, 0);
        if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Call1(Engine_EventWait, 20);
            Call2(Engine_EventShowMessage, 19, 0);
            gEventWork->message++;
        } else {
            Call1(Engine_EventWait, 10);
            gEventWork->message++;
            Call2(Engine_EventShowMessage, 19, 0);
        }
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 19, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call3(Engine_ActorShowEmote, 19, 258, 50);
        Call2(Engine_EventShowMessage, 19, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorSetAnimationAndWait, 0, 3);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 19, 3);
        Call1(Engine_EventWait, 30);
        Call1(Engine_AudioPlayCue, 30);
        Call3(Engine_ActorSetSpeed, 19, 78643, 39321);
        Call2(Engine_ActorSetAnimation, 19, 2);
        record = (u8 *)Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Call3(Engine_ActorSetDestination, 19, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Call1(Engine_ActorWaitForMove, 19);
        Call3(Engine_ActorSetPosition, 19, 0, 0);
        Call1(Engine_EventWait, 10);
        Audio_PlayCueFromEventWork();
        Engine_EventEnd();
    }
}
