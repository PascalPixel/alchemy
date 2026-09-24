#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Local_02000f80(void);
extern u8 Data_0000183a;

/* Xian training scene: actor 15 speaks the lines for the given mode, then actors 19 and 20 are placed and actor 15 walks up to them. */
void ShianJiin_RunMasterScene(s32 mode)
{
    s32 msg;

    Call3((void (*)())Engine_ActorSetSpeed, 15, 0xcccc, 0x6666);
    Engine_EventWait(60);
    msg = (s32)&Data_0000183a;
    Engine_EventSetMessage(msg);
    if (mode == 0) {
        Engine_EventSetMessage(msg - 1);
        Call3((void (*)())Engine_ActorShowEmote, 15, 0x101, 60);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventSetMessage(0x18ae);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 4);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 3);
        Engine_EventWait(20);
    }
    if (mode == 2) {
        Engine_EventSetMessage(0x18ac);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
    }
    Engine_EventShowMessageAndWait(15, 0, 20);
    Local_02000f80();
    Engine_ActorRunRepeatedMotion(15, 3);
    Call3((void (*)())Engine_ActorSetPosition, 19, 0xe80000, 0xa80000);
    Call3((void (*)())Engine_ActorSetPosition, 20, 0xe80000, 0xa80000);
    Engine_ActorGet(19)->y.fixed = 0xc0000;
    Engine_ActorGet(19)->target_y = ACTOR_NO_TARGET;
    Engine_ActorGet(19)->scale_x = 0xcccc;
    Engine_ActorGet(19)->sprite->rotation = 0x8000;
    Engine_AudioPlayCue(124);
    Engine_EventWait(40);
    Call3((void (*)())Engine_ActorWalkToAndWait, 15, 216, 152);
    Call3((void (*)())Engine_ActorFaceDirection, 15, 0x2000, 30);
}
