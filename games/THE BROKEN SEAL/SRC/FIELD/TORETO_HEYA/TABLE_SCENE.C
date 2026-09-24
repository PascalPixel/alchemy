#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_AudioPlayCue();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetSpeed();
s32 Engine_ActorGet();
void Engine_ActorSetSpritePriority();
s32 Engine_ActorSetPosition();
s32 Engine_ActorEnableActionCallback();
void Main_0808a0b0();
void Engine_EventWait();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Local_02001424();
void Engine_EventShowMessage();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorShowEmote();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetAttachedEffect();
void Engine_GameStateSetReturn();
void Engine_Import0808a250();
void Engine_EventEnd();


extern u8 Data_0000002d[];
extern u8 Data_02000240[];

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

void ToretoHeya_RunTableScene(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base3_2000240;

    rec8 = Value1(Engine_GameFlagIsSet, 3);
    Engine_EventBegin();
    Engine_AudioPlayCue(17);
    Call1(Engine_EventSetMessage, 0x14ce);
    Call3(Engine_EventShowMessageAndWait, 0x8009, 0, 20);
    Engine_AudioPlayCue(29);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    *(u8 *)(Engine_ActorGet(3) + 35) &= 254;
    Engine_ActorSetSpritePriority(3, 2);
    *(u8 *)(Engine_ActorGet(0) + 35) &= 254;
    Engine_ActorSetSpritePriority(0, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (rec8 != 0) {
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            ((void (*)())Engine_ActorSetPosition)(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Engine_ActorEnableActionCallback(3, 0x2009e2c);
    }
    Value2(Engine_ActorEnableActionCallback, 0, 0x2009db4);
    Value2(Engine_ActorEnableActionCallback, 1, 0x2009ddc);
    Call2(Main_0808a0b0, 2, 0x2009e04);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 40);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 8);
    Engine_EventWait(20);
    Local_02001424(8);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 60);
    Local_02001424(11);
    Call3(Engine_EventShowMessageAndWait, 0x8008, 0, 10);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(3, 1);
    Engine_ActorRunRepeatedMotion(2, 1);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 2, 0x102);
    Engine_EventWait(40);
    Local_02001424(11);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    {
        u8 *work = *(u8 **)0x03001ebc;

        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 64;
    }
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Engine_GameStateSetReturn((s32)Data_0000002d, 19);
    Engine_Import0808a250(36, 0);
    Engine_EventEnd();
}
