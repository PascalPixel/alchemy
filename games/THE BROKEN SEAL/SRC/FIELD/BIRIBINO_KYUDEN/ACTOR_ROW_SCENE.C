#include "TYPES.H"
#include "FIELD_EVENT.H"


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

/* Pans the camera over the palace, lines up four actors in a row and runs their scripted beat. */
void BiribinoKyuden_RunActorRowScene(void)
{
    u16 zero;
    struct FieldActor *actor;

    Engine_EventBegin();
    Call4((void (*)())Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Call4((void (*)())Engine_CameraMoveTo, 0x2740000, -1, 0x2ec0000, 0);
    Call6(Engine_MapCopyCellAttributes, 38, 55, 4, 1, 38, 45);
    Call6(Engine_MapCopyCellAttributes, 42, 55, 4, 1, 38, 46);
    Engine_ActorGet(0)->facing = 0;
    Call3((void (*)())Engine_ActorSetPosition, 0, 0x2410000, 0x2f80000);
    Engine_ActorGet(19)->facing = 0;
    Call3((void (*)())Engine_ActorSetPosition, 19, 0x2500000, 0x2f80000);
    Engine_ActorGet(17)->facing = 0x9000;
    Call3((void (*)())Engine_ActorSetPosition, 17, 0x2960000, 0x2fc0000);
    Call3((void (*)())Engine_ActorSetPosition, 21, 0x2680000, 0x2d80000);
    Call3((void (*)())Engine_ActorSetPosition, 22, 0x2780000, 0x2d80000);
    Call3((void (*)())Engine_ActorSetPosition, 23, 0x2880000, 0x2d80000);
    Call3((void (*)())Engine_ActorSetPosition, 24, 0x2980000, 0x2d80000);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(21), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(22), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(23), 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(24), 0);
    actor = Engine_ActorGet(21);
    zero = 0;
    actor->motion_flags = zero;
    Engine_ActorGet(22)->motion_flags = zero;
    Engine_ActorGet(23)->motion_flags = zero;
    Engine_ActorGet(24)->motion_flags = zero;
    Engine_ActorGet(21)->y.fixed = -0x40000;
    Engine_ActorGet(22)->y.fixed = -0x40000;
    Engine_ActorGet(23)->y.fixed = -0x40000;
    Engine_ActorGet(24)->y.fixed = -0x40000;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = 0x201;
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call3((void (*)())Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3((void (*)())Engine_ActorWalkTo, 19, 0x274, 0x2fc);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x264, 0x2fc);
    Engine_ActorSetAnimation(19, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventSetMessage(0x1746);
    Call3((void (*)())Engine_EventShowMessageAndWait, 19, 0, 10);
    Call3((void (*)())Engine_ActorWalkToAndWait, 19, 0x26e, 0x30c);
    Call3((void (*)())Engine_ActorFaceDirection, 19, 0xc000, 10);
    Engine_ActorRunRepeatedMotion(17, 2);
    Call3((void (*)())Engine_EventShowMessageAndWait, 17, 0, 10);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_GameFlagClear(0x12f);
    Engine_GameFlagSet(0x202);
    Engine_EventEnd();
}
