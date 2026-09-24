#include "TYPES.H"

void Engine_AudioPlayCue();
void Engine_EventBegin();
void Engine_CameraMoveTo();
void Engine_TaskWait();
void Engine_MapRedraw();
s32 Engine_ActorSetPosition();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_EventOpenScreen();
void Main_0808a2e0();
void Engine_EventWaitForScreen();
void Engine_EventWait();
s32 Engine_ActorSetAnimationAndWait();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void Main_0808a0b0();
void Main_0808a2d0();
void Main_0808a2d8();
void Engine_EventCloseScreen();
void Engine_GameFlagClear();
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

void HaidiaIe_RunScene015B4(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Call4(Engine_CameraMoveTo, 0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 0, 0x300000, 0x15a0000);
    Call3(Engine_ActorSetPosition, 25, 0x4e0000, 0x1660000);
    Call3(Engine_ActorSetPosition, 23, 0x670000, 0x1560000);
    Call3(Engine_ActorSetPosition, 24, 0x700000, 0x1680000);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Engine_ActorSetAnimation(0, 16);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 24) = -0x10000;
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(25, 7);
    record = Engine_ActorGet(25);
    {
        u8 *motion = *(u8 **)(record + 80);
        s32 shown = 0x1555;

        *(u16 *)(motion + 30) = shown;
    }
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        u8 *work = *(u8 **)0x03001ebc;

        *(s32 *)(work + 0x1c0) = 0x100;
    }
    Engine_EventOpenScreen();
    Main_0808a2e0();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorFaceDirection, 23, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 24, 3);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 10);
    Engine_ActorSetSpritePriority(0, 3);
    Engine_ActorSetSpritePriority(25, 3);
    Call3(Engine_ActorSetSpeed, 23, 0x26666, 0x13333);
    v5 = 128;
    record = Engine_ActorGet(23);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Call2(Engine_ActorEnableActionCallback, 23, 0x200aa48);
    Engine_EventWait(24);
    Call3(Engine_ActorSetSpeed, 24, 0x26666, 0x13333);
    record = Engine_ActorGet(24);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Call2(Main_0808a0b0, 24, 0x200ab2c);
    Engine_EventWait(40);
    Main_0808a2d0();
    Main_0808a2d8();
    Engine_TaskWait(20);
    Main_0808a2d8();
    Engine_TaskWait(60);
    Main_0808a2d8();
    Engine_TaskWait(20);
    Main_0808a2d0();
    Engine_EventWait(40);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 120;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call1(Engine_GameFlagClear, 0x834);
    Engine_EventRequestExit(9);
    Engine_EventEnd();
}
