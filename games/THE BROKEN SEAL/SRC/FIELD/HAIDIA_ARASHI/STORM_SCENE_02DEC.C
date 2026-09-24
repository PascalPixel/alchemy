#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
s32 Engine_ActorSetSpeed();
s32 Engine_ActorWalkToAndWait();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_ActorEnableActionCallback();
void Engine_EventWait();
void Engine_CameraWaitForMove();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorFaceEachOther();
void Engine_ActorSetAnimation();
void Scene_RunActorGroupDepartureSequence();
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

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void HaidiaArashi_RunScene02DEC(void)
{
    u32 i;
    u8 *rec;
    u8 *record;
    s32 v5;
    u8 *p4;

    if (Value1(Engine_GameFlagIsSet, 0x840) == 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x841) != 0) {
        } else {
            Engine_EventBegin();
            Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 22, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 26, 0x10000, 0x8000);
            Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
            Call3(Engine_ActorWalkToAndWait, 0, 217, 0x557);
            record = Value1(Engine_ActorGet, 0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(22, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 22, 235, 0x557);
            Call3(Engine_ActorFaceDirection, 22, 0xb000, 0);
            record = Value1(Engine_ActorGet, 0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(26, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 26, 199, 0x557);
            Call3(Engine_ActorFaceDirection, 26, 0xd000, 0);
            Call3(Engine_ActorSetPosition, 25, 0xf70000, 0x4ba0000);
            Call3(Engine_ActorFaceDirection, 25, 0x6000, 0);
            record = Value1(Engine_ActorGet, 8);
            p4 = *(s32 *)((s32)record + 80);
            ((struct Flags35 *)record)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 1;
            rec = Value1(Engine_ActorGet, 0);
            p4 = *(s32 *)((s32)rec + 80);
            ((struct Flags35 *)rec)->flags &= 254;
            ((struct Flags9 *)p4)->mode = 2;
            record = Value1(Engine_ActorGet, 0);
            if ((s32)record != 0) {
                Engine_ActorSetPosition(8, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 8, 221, 0x569);
            Call3(Engine_ActorFaceDirection, 8, 0xb000, 60);
            Engine_ActorRunRepeatedMotion(26, 2);
            Call1(Engine_EventSetMessage, 0xec6);
            Engine_EventShowMessageAndWait(26, 0, 40);
            Call3(Engine_ActorSetPosition, 9, 0x650000, 0x4ad0000);
            Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
            Call3(Engine_EventShowMessageAndWait, 0x1009, 0, 10);
            Call3(Engine_ActorFaceDirection, 26, 0xa000, 0);
            Call2(Engine_CameraSetSpeed, 0x13333, 0x2666);
            Call4(Engine_CameraMoveTo, 0x650000, -1, 0x4ad0000, 1);
            Call3(Engine_ActorSetSpeed, 9, 0x16666, 0xb333);
            Call2(Engine_ActorEnableActionCallback, 9, 0x200cd1c);
            Engine_EventWait(60);
            Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
            Call4(Engine_CameraMoveTo, 0xbb0000, -1, 0x5300000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(40);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 20);
            Engine_ActorRunRepeatedMotion(9, 2);
            Call3(Engine_EventShowMessageAndWait, 0x4009, 0, 20);
            Call2(Engine_CameraSetSpeed, 0x20000, 0x4000);
            Call4(Engine_CameraMoveTo, 0xdd0000, -1, 0x5690000, 1);
            Engine_ActorFaceActor(0, 8, 0);
            Engine_ActorFaceActor(22, 8, 0);
            Call3(Engine_ActorFaceDirection, 26, 0x3000, 80);
            Call4(Engine_CameraMoveTo, 0xb60000, -1, 0x5500000, 1);
            Call3(Engine_ActorWalkToAndWait, 8, 182, 0x568);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_EventWait(30);
            Engine_ActorSetAnimationAndWait(8, 3);
            Engine_EventWait(10);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorRunRepeatedMotion(26, 2);
            Engine_EventShowMessageAndWait(26, 0, 10);
            Call3(Engine_ActorFaceDirection, 9, 0xe000, 40);
            Call3(Engine_ActorFaceDirection, 9, 0x3000, 20);
            Engine_ActorSetAnimationAndWait(9, 3);
            Engine_EventShowMessage(9, 0);
            Engine_ActorFaceEachOther(26, 8, 0);
            Engine_ActorFaceEachOther(22, 0, 0);
            Engine_EventWait(40);
            Engine_ActorFaceActor(0, 9, 0);
            Engine_ActorFaceActor(22, 9, 0);
            Engine_ActorFaceActor(26, 9, 0);
            Engine_ActorFaceActor(8, 9, 0);
            Engine_ActorRunRepeatedMotion(9, 2);
            Engine_EventWait(20);
            Engine_EventShowMessageAndWait(9, 0, 10);
            Engine_ActorSetAnimation(0, 3);
            Engine_ActorSetAnimation(26, 3);
            Engine_ActorSetAnimation(22, 3);
            v5 = 1;
            Engine_ActorSetAnimationAndWait(8, 3);
            rec[35] |= v5;
            record = Value1(Engine_ActorGet, 8);
            record[35] |= v5;
            Scene_RunActorGroupDepartureSequence();
            Call1(Engine_GameFlagSet, 0x841);
            Engine_EventEnd();
        }
    }
}
