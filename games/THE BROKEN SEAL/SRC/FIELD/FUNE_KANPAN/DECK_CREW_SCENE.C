#include "TYPES.H"

extern u8 Data_0200c80c[];
extern u8 Data_0200c7a8[];
extern u8 Data_0200c764[];
extern u8 Data_0200c7ec[];
extern u8 Data_0200c888[];

void Engine_EventBegin();
void Engine_ActorSetChildValue();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Main_0808a030();
void Engine_TaskWait();
void Engine_ActorSetAnimation();
void Engine_ActorSetPosition();
s32 Engine_ActorEnableActionCallback();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_AudioPlayCue();
void FieldScene_CallPairWith10();
void FieldScene_RunScene3af_02000bb8();
void Main_0808a0b0();
void Engine_EventCloseScreen();
void Engine_ActorStop();
void Main_08009208();
void Main_0808a1d8();
void Main_08015210();
void Main_08009210();
void Engine_EventRequestExit();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

/* Ship deck: place the crew actors, run their action scripts and walk actor
 * 30 and 31 through the deck sequence. */
void FuneKanpan_RunDeckCrewScene(void)
{
    u32 i;
    s32 record;
    s32 action_c80c;
    s32 v6;
    s32 v5;
    s32 action_c7a8;
    s32 action_c764;
    s32 action_c7ec;
    s32 action_c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Call1(Main_0808a030, 0x200d160);
    Engine_TaskWait(1);
    Call1(Main_0808a030, 0x200d268);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(31, 0);
    record = Engine_ActorGet(24);
    Engine_ActorSetSpriteFlags(record, 1);
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags(record, 1);
    record = Engine_ActorGet(26);
    Engine_ActorSetSpriteFlags(record, 1);
    record = Engine_ActorGet(27);
    Engine_ActorSetSpriteFlags(record, 1);
    record = Engine_ActorGet(28);
    Engine_ActorSetSpriteFlags(record, 1);
    record = Engine_ActorGet(29);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetPosition, 22, 0x1000000, 0x2800000);
    action_c80c = (s32)Data_0200c80c;
    Engine_ActorEnableActionCallback(22, action_c80c);
    Call3(Engine_ActorSetPosition, 21, 0x10c0000, 0x2b40000);
    Value2(Engine_ActorEnableActionCallback, 22, action_c80c);
    Call3(Engine_ActorSetPosition, 24, 0xf20000, 0x25c0000);
    Call3(Engine_ActorSetPosition, 25, 0x1080000, 0x2580000);
    Call3(Engine_ActorSetPosition, 26, 0xfe0000, 0x29c0000);
    Call3(Engine_ActorSetPosition, 27, 0x11a0000, 0x2920000);
    v6 = 0;
    *(u8 *)(Engine_ActorGet(24) + 99) = v6;
    v5 = 1;
    *(u8 *)(Engine_ActorGet(25) + 99) = v5;
    *(u8 *)(Engine_ActorGet(26) + 99) = v6;
    *(u8 *)(Engine_ActorGet(27) + 99) = v5;
    action_c7a8 = (s32)Data_0200c7a8;
    Engine_ActorEnableActionCallback(24, action_c7a8);
    Value2(Engine_ActorEnableActionCallback, 25, action_c7a8);
    action_c764 = (s32)Data_0200c764;
    Engine_ActorEnableActionCallback(26, action_c764);
    Value2(Engine_ActorEnableActionCallback, 27, action_c764);
    Engine_ActorSetPosition(20, 0, 0);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call1(Engine_EventWait, 0x190);
    Call3(Engine_ActorSetPosition, 28, 0xfe0000, 0x2e40000);
    Call3(Engine_ActorSetPosition, 29, 0x180000, 0x24a0000);
    Call3(Engine_ActorSetSpeed, 28, 0x19999, 0xcccc);
    Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
    Call3(Engine_ActorSetDestination, 29, 172, 0x284);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 28, 200, 0x294);
    Call3(Engine_ActorSetSpeed, 0, 0x40000, 0x20000);
    Call3(Engine_ActorSetDestination, 0, 174, 0x26c);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 28, 180, 0x244);
    Engine_AudioPlayCue(146);
    action_c7ec = (s32)Data_0200c7ec;
    Engine_ActorEnableActionCallback(28, action_c7ec);
    Engine_ActorEnableActionCallback(29, action_c7ec);
    Engine_AudioPlayCue(240);
    Call3(Engine_ActorSetPosition, 31, 0x860000, 0x2520000);
    Call2(Engine_ActorEnableActionCallback, 31, 0x200c814);
    Engine_EventWait(10);
    Call3(Engine_ActorSetPosition, 30, 0x860000, 0x2480000);
    Call3(Engine_ActorSetSpeed, 30, 0x40000, 0x20000);
    record = Engine_ActorGet(30);
    *(s32 *)(record + 40) = 0x80000;
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 186, 0x264);
    record = Engine_ActorGet(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 30, 0x20000, 0x10000);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 216, 0x258);
    Call2(FieldScene_CallPairWith10, 30, 0xc000);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    action_c888 = (s32)Data_0200c888;
    Engine_ActorEnableActionCallback(30, action_c888);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(28, action_c888);
    Engine_EventWait(10);
    Main_0808a0b0(29, action_c888);
    Engine_EventWait(20);
    Engine_AudioPlayCue(147);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_EventWait(10);
    Main_08009208();
    Main_0808a1d8(21);
    Call3(Main_08015210, 0x1e45, 1, 0);
    Main_08009210();
    Engine_EventRequestExit(13);
}
