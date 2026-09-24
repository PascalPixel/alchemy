/* NONMATCHING: 2816 of 2816 bytes, 254 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 254 halfword edits, size exact: the psynergy draft with the emote, speed, walk and camera-speed calls spelled through CallN (constants reloaded per call) and the pair-facing calls direct (their constants shared in r8-fp). What remains is which facing constant lands in r9 versus fp, the scratch register used to build each shared constant (r2 here, r3 in the reference), and the matching argument order. */
#include "TYPES.H"

void Main_08077260();
void Engine_EventBegin();
void Engine_MapCopyCellsTo();
void Engine_CameraMoveTo();
void Engine_MapRedraw();
void Engine_TaskWait();
u8 * Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetPosition();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorShowEmote();
void SceneActor_SetPairZeroAndValue();
void Engine_EventSetMessage();
u8 * Event_SayThenWait();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetAnimation();
void Engine_EventShowMessage();
void Engine_ActorEnableActionCallback();
void Main_0808a168();
void Engine_ActorStartAction();
void Engine_ActorStop();
void Engine_ActorFaceDirection();
void Main_0808a0b0();
void Engine_CameraSetSpeed();
s32 Engine_EventGetViewCenter();
s32 Engine_PartyGiveItem();
void Engine_ActorJump();
void Engine_ActorFollow();
void Engine_ActorStartRepeatedMotion();
void Engine_MessageShowCentered();
void Engine_EventCloseScreen();
void Engine_EventRequestExit();
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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

static __inline__ void Call5(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    f(a0, a1, a2, a3, a4);
}

void Local_020017c8(void)
{
    u32 i;
    u8 *record;
    s32 base5_200ac00;
    s32 v5;
    s32 base5_200ac90;
    s32 v6;
    s32 base5_200adf0;

    Main_08077260(1);
    Engine_EventBegin();
    Call6((void (*)())Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
    Call4((void (*)())Engine_CameraMoveTo, 0xb40000, 0x100000, 0x26a0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    record = Engine_ActorGet(22);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(23);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(24);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(26);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(29);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(16, 1);
    Engine_ActorSetSpritePriority(15, 1);
    Engine_ActorSetPosition(0, 0xd00000, 0x32e0000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3((void (*)())Engine_ActorShowEmote, 12, 0x101, 40);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 20);
    Engine_EventSetMessage(0x11fa);
    Call2((void (*)())Event_SayThenWait, 12, 10);
    Call3((void (*)())Engine_ActorShowEmote, 11, 0x102, 20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Call2((void (*)())Event_SayThenWait, 11, 10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Call2((void (*)())Event_SayThenWait, 11, 10);
    Call3((void (*)())Engine_ActorShowEmote, 12, 0x100, 40);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
    Call3((void (*)())Engine_ActorWalkToAndWait, 12, 184, 0x26a);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 60);
    Call2((void (*)())Event_SayThenWait, 12, 20);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkToAndWait, 11, 168, 0x26a);
    SceneActor_SetPairZeroAndValue(11, 0xf000, 10);
    Engine_ActorSetAnimation(11, 4);
    Call2((void (*)())Event_SayThenWait, 11, 20);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Call3((void (*)())Engine_ActorSetSpeed, 30, 0x26666, 0x13333);
    Engine_ActorSetPosition(30, 0x6e0000, 0x2e80000);
    Engine_TaskWait(2);
    Engine_ActorSetAnimation(30, 3);
    Engine_ActorEnableActionCallback(30, 0x200ac14);
    Engine_EventWait(40);
    base5_200ac00 = 0x200ac00;
    Main_0808a168(11, 0x1001e, base5_200ac00);
    Main_0808a168(12, 0x1001e, base5_200ac00);
    Engine_ActorStartAction(30);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_EventWait(60);
    Call3((void (*)())Engine_ActorShowEmote, 11, 0x105, 0);
    Call3((void (*)())Engine_ActorShowEmote, 12, 0x105, 120);
    Engine_ActorFaceDirection(11, 0x1000, 0);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 80);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    Call3((void (*)())Engine_ActorShowEmote, 12, 0x101, 80);
    Engine_ActorFaceDirection(11, 0x3000, 0);
    Call3((void (*)())Engine_ActorWalkToAndWait, 12, 184, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    Call3((void (*)())Engine_ActorShowEmote, 12, 0x101, 40);
    Call3((void (*)())Engine_ActorShowEmote, 11, 0x101, 40);
    Call3((void (*)())Engine_ActorWalkToAndWait, 11, 168, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    Call3((void (*)())Engine_ActorShowEmote, 11, 0x101, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Call2((void (*)())Event_SayThenWait, 11, 20);
    Engine_ActorSetAnimationAndWait(12, 3);
    Call2((void (*)())Event_SayThenWait, 12, 10);
    Call3((void (*)())Engine_ActorShowEmote, 11, 0x100, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Call2((void (*)())Event_SayThenWait, 11, 10);
    record = Engine_ActorGet(30);
    if ((s32)record != 0) {
        Engine_ActorSetPosition(31, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    v5 = 254;
    Engine_TaskWait(2);
    *(u8 *)(Engine_ActorGet(30) + 35) &= v5;
    *(u8 *)(Engine_ActorGet(31) + 35) &= v5;
    Engine_ActorSetSpritePriority(30, 2);
    Engine_ActorSetSpritePriority(31, 2);
    Call3((void (*)())Engine_ActorSetSpeed, 31, 0x39999, 0x1cccc);
    Engine_ActorSetAnimation(31, 2);
    base5_200ac90 = 0x200ac90;
    Engine_ActorEnableActionCallback(31, base5_200ac90);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(30, 3);
    Call3((void (*)())Engine_ActorSetSpeed, 30, 0x4cccc, 0x26666);
    Main_0808a0b0(30, base5_200ac90);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 2);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Call2((void (*)())Event_SayThenWait, 12, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Call2((void (*)())Event_SayThenWait, 11, 20);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x26666, 0x13333);
    Call3((void (*)())Engine_ActorSetSpeed, 12, 0x26666, 0x13333);
    Engine_ActorEnableActionCallback(11, 0x200acf8);
    Engine_EventWait(10);
    Call2((void (*)())Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    v6 = 0;
    *(u8 *)(Engine_EventGetViewCenter() + 85) = v6;
    Call4((void (*)())Engine_CameraMoveTo, 0xd70000, 0x100000, 0x3210000, 1);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(12, 0x200ad74);
    Engine_ActorStartAction(12);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 120);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    ((void (*)())Event_SayThenWait)(13, 20);
    Engine_ActorFaceDirection(0, 0, 0);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(1, 0xb000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(20);
    Call2((void (*)())Event_SayThenWait, 16, 10);
    Call3((void (*)())Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkToAndWait, 16, 216, 0x320);
    Engine_ActorFaceDirection(16, 0x4000, 0);
    Engine_PartyGiveItem(180, 0);
    Call3((void (*)())Engine_ActorWalkToAndWait, 16, 0x108, 0x320);
    Engine_ActorFaceDirection(16, 0x6000, 0);
    Call3((void (*)())Engine_ActorShowEmote, 1, 0x102, 40);
    SceneActor_SetPairZeroAndValue(1, 0xf000, 10);
    Call2((void (*)())Event_SayThenWait, 1, 10);
    Engine_ActorSetAnimationAndWait(17, 4);
    Call2((void (*)())Event_SayThenWait, 17, 10);
    Engine_ActorFaceDirection(1, 0x1000, 0);
    Call3((void (*)())Engine_ActorShowEmote, 1, 0x103, 20);
    Engine_ActorJump(1, 4, 60);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(14, 0xd000, 10);
    Call2((void (*)())Event_SayThenWait, 14, 60);
    Call3((void (*)())Engine_ActorShowEmote, 1, 0x102, 0);
    Call3((void (*)())Engine_ActorShowEmote, 16, 0x102, 0);
    Call3((void (*)())Engine_ActorShowEmote, 17, 0x102, 0);
    Call3((void (*)())Engine_ActorShowEmote, 18, 0x102, 0);
    Call3((void (*)())Engine_ActorShowEmote, 19, 0x102, 80);
    Call3((void (*)())Engine_ActorShowEmote, 17, 0x100, 0);
    Call2((void (*)())Event_SayThenWait, 17, 60);
    Engine_ActorFollow(0, 17);
    Engine_ActorFollow(1, 17);
    Call3((void (*)())Engine_ActorWalkToAndWait, 17, 216, 0x320);
    Engine_ActorFaceDirection(17, 0x4000, 0);
    Call2((void (*)())Event_SayThenWait, 17, 60);
    Engine_PartyGiveItem(207, 0);
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Call3((void (*)())Engine_ActorWalkToAndWait, 17, 0x110, 0x330);
    Engine_ActorFaceDirection(17, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 10);
    Call2((void (*)())Event_SayThenWait, 1, 10);
    Engine_ActorFaceDirection(14, 0x3000, 0);
    SceneActor_SetPairZeroAndValue(0, 0, 10);
    Call3((void (*)())Engine_ActorShowEmote, 0, 0x101, 60);
    Engine_ActorRunRepeatedMotion(16, 1);
    Call2((void (*)())Event_SayThenWait, 16, 10);
    SceneActor_SetPairZeroAndValue(1, 0xf000, 10);
    Call3((void (*)())Engine_ActorShowEmote, 1, 0x101, 20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Call2((void (*)())Event_SayThenWait, 16, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_ActorSetAnimation(18, 4);
    Engine_EventShowMessage(18, 0);
    Engine_ActorStartRepeatedMotion(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorSetAnimationAndWait(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(28, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_ActorJump(15, 2, 10);
    Engine_ActorJump(15, 4, 40);
    Call2((void (*)())Event_SayThenWait, 15, 10);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 20);
    SceneActor_SetPairZeroAndValue(15, 0xd000, 10);
    Call2((void (*)())Event_SayThenWait, 15, 10);
    SceneActor_SetPairZeroAndValue(15, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(15, 0x5000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(14, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(29, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(15, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(13, 3);
    Engine_ActorSetAnimation(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(22, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimationAndWait(28, 3);
    Engine_EventWait(80);
    Engine_ActorJump(11, 4, 0);
    Engine_ActorJump(14, 4, 0);
    Engine_ActorJump(17, 4, 0);
    Engine_ActorJump(20, 4, 0);
    Engine_ActorJump(23, 4, 0);
    Engine_ActorJump(26, 4, 0);
    Engine_ActorJump(29, 4, 0);
    Engine_ActorJump(12, 4, 0);
    Engine_ActorJump(15, 4, 0);
    Engine_ActorJump(18, 4, 0);
    Engine_ActorJump(21, 4, 0);
    Engine_ActorJump(24, 4, 0);
    Engine_ActorJump(27, 4, 0);
    Engine_ActorJump(13, 4, 0);
    Engine_ActorJump(16, 4, 0);
    Engine_ActorJump(19, 4, 0);
    Engine_ActorJump(22, 4, 0);
    Engine_ActorJump(25, 4, 0);
    Engine_ActorJump(28, 4, 0);
    Engine_MessageShowCentered(0x1214, 1);
    v5 = 1;
    Engine_EventWait(80);
    *(u8 *)(Engine_ActorGet(0) + 35) |= v5;
    {
        u8 *record = Engine_ActorGet(1);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | v5);
    }
    Call3((void (*)())Engine_ActorShowEmote, 0, 0x102, 0);
    Call3((void (*)())Engine_ActorShowEmote, 1, 0x102, 80);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x5000, 20);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    base5_200adf0 = 0x200adf0;
    Engine_ActorEnableActionCallback(0, base5_200adf0);
    Engine_EventWait(20);
    Call2((void (*)())Engine_CameraSetSpeed, 0x6666, 0xccc);
    Call4((void (*)())Engine_CameraMoveTo, 0xd80000, 0x100000, 0x3890000, 1);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(1, base5_200adf0);
    Engine_EventWait(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 60;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}
