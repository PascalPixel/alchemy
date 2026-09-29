/* The Xian master's lines to his pupils before the training starts. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgShianCannotPushHands[];
extern u8 MsgShianGreatWarriorTrain[];
extern u8 MsgShianWarriorsCannotUse[];

/* FAKEMATCH: call sites spelled through this wrapper pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void ShianJiin_WalkByFacing(void);

/* Actor 15 speaks the lines for the given mode, then actors 19 and 20 are
 * placed and actor 15 walks up to them. The first mode's line is the one
 * before its name, as the game counts down from it in instructions. */
void ShianJiin_RunMasterScene(s32 mode)
{
    s32 msg;

    Call3((void (*)())Engine_ActorSetSpeed, 15, 0xcccc, 0x6666);
    Engine_EventWait(60);
    msg = (s32)MsgShianCannotPushHands;
    Engine_EventSetMessage(msg);
    if (mode == 0) {
        Engine_EventSetMessage(msg - 1);
        Call3((void (*)())Engine_ActorShowEmote, 15, 0x101, 60);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventSetMessage((s32)MsgShianWarriorsCannotUse);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 4);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(15, 0, 20);
        Engine_ActorSetAnimationAndWait(15, 3);
        Engine_EventWait(20);
    }
    if (mode == 2) {
        Engine_EventSetMessage((s32)MsgShianGreatWarriorTrain);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
    }
    Engine_EventShowMessageAndWait(15, 0, 20);
    ShianJiin_WalkByFacing();
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
