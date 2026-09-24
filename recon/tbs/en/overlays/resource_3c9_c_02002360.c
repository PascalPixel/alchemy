#include "TYPES.H"

void Engine_EventBegin();
void Engine_MapCopyCellAttributes();
u8 * Engine_ActorGet();
void Engine_ActorSetPosition();
u8 * Engine_ActorSetAnimation();
s32 Engine_ActorEnableActionCallback();
void Engine_ActorSetSpriteFlags();
s32 Engine_EventGetViewCenter();
void Engine_CameraMoveTo();
void Engine_TaskWait();
void Engine_MapRedraw();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorJump();
s32 Scene_CallPairWith10();
void Engine_EventSetMessage();
void State_ApplyArgMode0AndSet10();
s32 Engine_ActorFaceDirection();
s32 Engine_ActorRunRepeatedMotion();
s32 Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
s32 Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorStartRepeatedMotion();
s32 Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_EventShowMessage();
void Engine_AudioPlayCue();
void Engine_ActorStop();
void Engine_EventShowMessageAndWait();
s32 Engine_ObjectCreate();
s32 Engine_HeapAllocate();
void Engine_ItemLoadIcon();
void Engine_VramLoad();
void Engine_HeapRelease();
s32 Engine_ActorSetSpritePriority();
void Engine_ObjectSetPosition();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_ObjectCommitPosition();
void Actor_ParkRecord();
s32 Engine_ActorSetChildValue();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
s32 Engine_TaskAddCallback();
void Scene_RunSetupSequence35c4();
u8 * Engine_MathCos();
s32 Engine_MathSin();
void Effect_Spawn();
void Engine_ActorSetDestination();
void Engine_GameFlagSet();
s32 Engine_GameStateSetReturn();
void Engine_Import0808a268();
s32 Engine_Import0808a250();
void Engine_Import08077268();


extern u8 Data_00000000[];
extern u8 Data_000000bb[];
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

struct Half {
    u16 v;
};

/* NONMATCHING: 4720 of 4708 bytes, 962 halfword edits (2026-09-24). First
 * pass: the zero shared for +85 is a one-halfword struct (movhi pool reach)
 * and 0x8000 is one variable, which places the first pool as the reference
 * has it. The shared constants (0x3000, 10) and the stack size (136) still
 * differ, so the callee-saved assignment cascades through the function. */
void Func_02002360(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p6b;
    s32 p8;
    s32 p8b;
    struct Half zero;
    s32 south;
    s32 west;
    s32 p9b;
    u8 *rec;
    s32 rec4;
    u8 *rec7;
    u8 *record;
    s32 value;
    s32 v7;
    s32 v6;
    s32 none;
    s32 base6_200e088;
    s32 base6_200e0ac;
    s32 base7_200e22c;
    s32 base7_0;
    s32 base6_54;
    s32 v9;
    s32 base3_2000240;
    s32 slot40;
    s32 slot32;
    s32 slot28;
    s32 slot24;
    s32 slot20;
    u8 *p6;
    u8 *p4;
    u8 slot96[40];
    u8 slot44[52];

    Engine_EventBegin();
    Call6(Engine_MapCopyCellAttributes, 17, 10, 4, 2, 17, 8);
    {
        u8 *a = Engine_ActorGet(1);

        zero.v = 0;
        south = 0x8000;
        *(u16 *)(a + 6) = south;
    }
    Call3(Engine_ActorSetPosition, 1, 0x1480000, 0xa80000);
    *(u16 *)(Engine_ActorGet(2) + 6) = south;
    Call3(Engine_ActorSetPosition, 2, 0x1540000, 0xc40000);
    *(u16 *)(Engine_ActorGet(3) + 6) = south;
    Call3(Engine_ActorSetPosition, 3, 0x1460000, 0xcc0000);
    *(u16 *)(Engine_ActorGet(6) + 6) = (west = 0x3000);
    Call3(Engine_ActorSetPosition, 6, 0x10c0000, 0x9a0000);
    *(u16 *)(Engine_ActorGet(21) + 6) = west;
    Call3(Engine_ActorSetPosition, 21, 0x10c0000, 0xa40000);
    rec = Value1(Engine_ActorGet, 20);
    {
        s32 shown = 10;
    
        *(u16 *)((s32)rec + 100) = shown;
    }
    {
        s32 shown = 0xd000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    Call3(Engine_ActorSetPosition, 20, 0x1260000, 0xd40000);
    Call2((void (*)())Engine_ActorSetAnimation, 20, 9);
    Call2((void (*)())Engine_ActorEnableActionCallback, 20, 0x200e074);
    record = Engine_ActorGet(20);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 0);
    rec = Value1(Engine_ActorGet, 19);
    {
        s32 shown = 10;
    
        *(u16 *)((s32)rec + 100) = shown;
    }
    {
        s32 shown = 0;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    Call3(Engine_ActorSetPosition, 19, 0x11e0000, 0xc00000);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 7);
    Call2((void (*)())Engine_ActorEnableActionCallback, 19, 0x200e074);
    record = Engine_ActorGet(19);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 0);
    *(u8 *)(Engine_EventGetViewCenter() + 85) = zero.v;
    Call4(Engine_CameraMoveTo, 0x1300000, 0x200000, 0xb40000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3((void (*)())Engine_ActorJump, 1, 2, 20);
    Call2((void (*)())Scene_CallPairWith10, 1, 0x2000);
    Call1(Engine_EventSetMessage, 0x27cf);
    Call1(State_ApplyArgMode0AndSet10, 0x1001);
    Call2((void (*)())Scene_CallPairWith10, 3, 0xa000);
    State_ApplyArgMode0AndSet10(3);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0x8000, 0);
    Value2(Scene_CallPairWith10, 2, 0xa000);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 21, 2);
    Call3(Engine_ActorSetSpeed, 21, 0xcccc, 0x6666);
    *(u8 *)(Engine_ActorGet(21) + 90) &= 254;
    Call3(Engine_ActorWalkToAndWait, 21, 0x118, 164);
    Engine_EventWait(1);
    {
        u8 *record = Engine_ActorGet(21);
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 2, 0x102, 40);
    State_ApplyArgMode0AndSet10(2);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 21, 4);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 1, 0x103, 20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 1, 1);
    State_ApplyArgMode0AndSet10(1);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 21, 1);
    Call2((void (*)())Scene_CallPairWith10, 21, 0);
    State_ApplyArgMode0AndSet10(21);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 3, 2);
    State_ApplyArgMode0AndSet10(3);
    Call2((void (*)())Engine_ActorSetAnimation, 3, 4);
    State_ApplyArgMode0AndSet10(3);
    Call3(Engine_ActorShowEmote, 21, 0x105, 40);
    Call2((void (*)())Scene_CallPairWith10, 21, 0x3000);
    State_ApplyArgMode0AndSet10(21);
    Call3((void (*)())Engine_ActorJump, 2, 2, 20);
    State_ApplyArgMode0AndSet10(2);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 21, 3);
    State_ApplyArgMode0AndSet10(21);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 1, 1);
    Call2((void (*)())Scene_CallPairWith10, 1, 0x2000);
    Value2(Engine_EventOpenMessage, 1, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xe000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    v7 = 1;
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        v7 = 0;
    }
    Engine_TaskWait(20);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call2((void (*)())Scene_CallPairWith10, 3, 0xa000);
    Call2((void (*)())Scene_CallPairWith10, 21, 0);
    Call2((void (*)())Engine_EventShowMessage, 21, 0);
    Engine_AudioPlayCue(17);
    Engine_EventWait(40);
    if (v7 != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Engine_ActorStop(20);
    Engine_ActorStop(19);
    Engine_TaskWait(1);
    rec = Engine_ActorGet(20);
    *(s32 *)((s32)rec + 24) = 0x10000;
    *(s32 *)((s32)rec + 28) = 0x10000;
    rec = Engine_ActorGet(19);
    *(s32 *)((s32)rec + 24) = 0x10000;
    *(s32 *)((s32)rec + 28) = 0x10000;
    Engine_TaskWait(1);
    State_ApplyArgMode0AndSet10(20);
    Call3(Engine_ActorFaceDirection, 21, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 40);
    Call2((void (*)())Engine_ActorSetAnimation, 20, 1);
    record = Engine_ActorGet(20);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 20, 0x3333, 0x1999);
    Call3(Engine_ActorWalkToAndWait, 20, 0x12c, 206);
    Engine_EventWait(20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 20, 2);
    Engine_EventWait(20);
    record = Engine_ActorGet(20);
    *(s32 *)((s32)record + 24) = -0x10000;
    Engine_AudioPlayCue(161);
    Call2((void (*)())Engine_ActorSetAnimation, 20, 8);
    Engine_EventWait(20);
    ((void (*)())Engine_ActorSetAnimation)(19, 1);
    record = Engine_ActorGet(19);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 1);
    Call3((void (*)())Engine_ActorJump, 19, 4, 40);
    Call3(Engine_ActorSetSpeed, 19, 0x3333, 0x1999);
    *(u8 *)(Engine_ActorGet(19) + 90) &= 254;
    Call3(Engine_ActorWalkToAndWait, 19, 0x128, 186);
    Call3(Engine_ActorSetSpeed, 19, 0x1999, 0xccc);
    Call3(Engine_ActorWalkToAndWait, 19, 0x124, 186);
    *(u8 *)(Engine_ActorGet(19) + 90) |= 1;
    record = Value1(Engine_ActorGet, 19);
    *(s32 *)((s32)record + 24) = -0x10000;
    Engine_AudioPlayCue(161);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 5);
    Engine_EventWait(20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 19, 2);
    Engine_EventWait(20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 20, 1);
    Engine_EventWait(80);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 3, 4);
    Call3((void (*)())Engine_EventShowMessageAndWait, 3, 0, 20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 19, 2);
    Engine_EventWait(40);
    State_ApplyArgMode0AndSet10(19);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 40);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 20);
    rec = Engine_ActorGet(20);
    {
        s32 shown = 0x3000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    *(s32 *)((s32)rec + 24) = 0x10000;
    p9b = (s32)Data_00000000;
    Call2((void (*)())Engine_ActorSetAnimation, 20, 1);
    Engine_EventWait(10);
    Call2((void (*)())Scene_CallPairWith10, 20, 0xd000);
    Call3((void (*)())Engine_ActorJump, 20, 6, 0);
    Engine_EventWait(10);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xa000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 21, 0xd000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 6, 0, 0);
    rec7 = Value4(Engine_ObjectCreate, 22, *(s32 *)((s32)rec + 8), (*(s32 *)((s32)rec + 12) + 0x80000), *(s32 *)((s32)rec + 16));
    if ((s32)rec7 != 0) {
        p6 = *(s32 *)((s32)rec7 + 80);
        p6[39] = p9b;
        p6[5] &= -33;
        p6[9] = ((s32)(15 & p6[9]) & -13);
        rec7[35] &= 254;
        rec7[85] = p9b;
        {
            s32 shown = 1;

            rec7[92] = shown;
        }
        *(s32 *)((s32)rec7 + 48) = 0x19999;
        *(s32 *)((s32)rec7 + 52) = 0xcccc;
        record = Value2(Engine_HeapAllocate, 17, 0x608);
        slot40 = (s32)record;
        Engine_ItemLoadIcon(220);
        Call4((void (*)())Engine_VramLoad, p6[28], 128, (slot40 + 0x400), (slot40 + 0x400));
        Engine_HeapRelease(17);
    }
    Value2(Engine_ActorSetSpritePriority, 22, 1);
    v6 = 128;
    record = Engine_ActorGet(22);
    *(s32 *)((s32)record + 8) = *(s32 *)((s32)rec + 8);
    *(s32 *)((s32)record + 12) = (v6 << 14);
    *(s32 *)((s32)record + 16) = *(s32 *)((s32)rec + 16);
    record[85] = 3;
    *(s32 *)((s32)record + 48) = 0x19999;
    *(s32 *)((s32)record + 52) = 0xcccc;
    *(s32 *)((s32)record + 24) = 0xc000;
    *(s32 *)((s32)record + 28) = 0xc000;
    if ((s32)rec7 != 0) {
        rec7[85] = 3;
        *(s32 *)((s32)rec7 + 72) = 0x9999;
        *(s32 *)((s32)rec7 + 68) = 0xcccc;
        *(s32 *)((s32)rec7 + 40) = 0x80000;
        Call4(Engine_ObjectSetPosition, (s32)rec7, 0x1340000, (v6 << 14), 0xa40000);
    }
    Call3(Engine_ObjectMotionSetPositionAndCommit, 22, 0x134, 164);
    Call3((void (*)())Engine_ActorSetPosition, 22, 0, 0);
    Call2((void (*)())Engine_ActorSetSpritePriority, 21, 0);
    if ((s32)rec7 != 0) {
        Call1(Engine_AudioPlayCue, 0x135);
        Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)rec7, 0);
        *(s32 *)((s32)rec7 + 40) = 0x40000;
        Call4(Engine_ObjectSetPosition, (s32)rec7, 0x13a0000, (v6 << 14), 0x890000);
        Engine_ObjectCommitPosition((s32)rec7);
        Call1(Engine_AudioPlayCue, 0x135);
        *(u8 *)(*(s32 *)((s32)rec7 + 80) + 9) = ((-13 & *(u8 *)(*(s32 *)((s32)rec7 + 80) + 9)) | 4);
        *(s32 *)((s32)rec7 + 40) = 0x60000;
        Call4(Engine_ObjectSetPosition, (s32)rec7, 0x11d0000, (v6 << 14), 0x920000);
        Engine_ObjectCommitPosition((s32)rec7);
        Call1(Engine_AudioPlayCue, 0x135);
        *(s32 *)((s32)rec7 + 40) = 0x50000;
        Call4(Engine_ObjectSetPosition, (s32)rec7, 0x12c0000, (v6 << 14), 0x9a0000);
        Engine_ObjectCommitPosition((s32)rec7);
        Engine_TaskWait(6);
        *(s32 *)((s32)rec7 + 8) = 0;
        *(s32 *)((s32)rec7 + 12) = 0;
        *(s32 *)((s32)rec7 + 16) = 0;
        Actor_ParkRecord((s32)rec7);
    }
    Call3(Engine_ActorShowEmote, 21, 0x100, 0);
    Call3(Engine_ActorShowEmote, 6, 0x100, 0);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 30);
    Call2((void (*)())Engine_ActorSetSpritePriority, 21, 1);
    *(u8 *)(Engine_ActorGet(21) + 35) |= 1;
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 2, 2);
    State_ApplyArgMode0AndSet10(2);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 3, 1);
    Call3((void (*)())Engine_EventShowMessageAndWait, 3, 0, 40);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 20, 1);
    Engine_EventWait(20);
    State_ApplyArgMode0AndSet10(20);
    Call3(Engine_ActorFaceDirection, 0, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 6, 0x3000, 0);
    Call2((void (*)())Scene_CallPairWith10, 21, 0x3000);
    Call3(Engine_ActorShowEmote, 21, 0x101, 0);
    Call3(Engine_ActorShowEmote, 6, 0x101, 0);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Call3(Engine_ActorShowEmote, 3, 0x101, 60);
    Call3(Engine_ActorShowEmote, 20, 0x108, 40);
    State_ApplyArgMode0AndSet10(20);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 40);
    Value2(Engine_EventOpenMessage, 1, 0);
    if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 0) {
        Call2((void (*)())Engine_ActorSetAnimation, 1, 3);
        v7 = 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Call2((void (*)())Engine_ActorSetAnimation, 1, 4);
        v7 = 0;
    }
    State_ApplyArgMode0AndSet10(1);
    if (v7 != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Engine_EventWait(20);
    record = Engine_ActorGet(24);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 0);
    Call2((void (*)())Engine_ActorSetChildValue, 24, 7);
    rec = Engine_ActorGet(24);
    none = 0;
    *(s32 *)((s32)rec + 28) = -0x10000;
    *(s32 *)((s32)rec + 24) = 0x1999;
    rec[85] = none;
    *(s32 *)((s32)rec + 12) = 0x400000;
    *(s32 *)((s32)rec + 16) = 0x9e0000;
    *(s32 *)((s32)rec + 8) = 0x1300000;
    record = Engine_ActorGet(25);
    Call2((void (*)())Engine_ActorSetSpriteFlags, (s32)record, 0);
    Call2((void (*)())Engine_ActorSetChildValue, 25, 7);
    rec = Engine_ActorGet(25);
    *(s32 *)((s32)rec + 28) = -0x10000;
    *(s32 *)((s32)rec + 24) = 0x1999;
    rec[85] = none;
    *(s32 *)((s32)rec + 12) = 0x600000;
    *(s32 *)((s32)rec + 8) = 0x1300000;
    *(s32 *)((s32)rec + 16) = 0x9e0000;
    Call3(Engine_ActorShowEmote, 21, 0x100, 0);
    Call3(Engine_ActorShowEmote, 6, 0x100, 0);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 21, 0xd000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 6, 0, 0);
    base6_200e088 = 0x200e088;
    Call2((void (*)())Engine_ActorEnableActionCallback, 24, base6_200e088);
    Call2((void (*)())Engine_ActorEnableActionCallback, 25, base6_200e088);
    Engine_AudioPlayCue(145);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x60000, 0x60000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x4063ff, 0);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(20);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 0);
    Engine_ColorBufferInterpolate(24);
    Engine_TaskWait(60);
    Call2(Engine_TaskAddCallback, 0x200b6d1, 0xc80);
    Engine_AudioPlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x4063ff, 0);
    Engine_ColorBufferInterpolate(120);
    base6_200e0ac = 0x200e0ac;
    Call2((void (*)())Engine_ActorEnableActionCallback, 24, base6_200e0ac);
    Call2((void (*)())Engine_ActorEnableActionCallback, 25, base6_200e0ac);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x203210, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Engine_AudioPlayCue(63);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 1);
    Call3((void (*)())Engine_ActorFaceDirection, 19, 0, 0);
    record = Engine_ActorGet(19);
    *(s32 *)((s32)record + 24) = 0x10000;
    Call3((void (*)())Engine_ActorJump, 19, 4, 40);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 19, 1);
    Call3((void (*)())Engine_EventShowMessageAndWait, 19, 0, 20);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 21, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 6, 0x3000, 20);
    v6 = 128;
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 20, 1);
    State_ApplyArgMode0AndSet10(20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 3, 1);
    Call2((void (*)())Scene_CallPairWith10, 3, (v6 << 8));
    State_ApplyArgMode0AndSet10(3);
    Call2((void (*)())Scene_CallPairWith10, 20, 0);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 20, 4);
    State_ApplyArgMode0AndSet10(20);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 19, 2);
    State_ApplyArgMode0AndSet10(19);
    Call3(Engine_ActorShowEmote, 1, 0x100, 20);
    Call2((void (*)())Scene_CallPairWith10, 1, 0x6000);
    State_ApplyArgMode0AndSet10(1);
    Call2(Scene_CallPairWith10, 20, 0xd000);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 20, 3);
    Call3((void (*)())Engine_EventShowMessageAndWait, 20, 0, 20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 1, 1);
    State_ApplyArgMode0AndSet10(1);
    Call3(Engine_ActorFaceDirection, 19, 0xb000, 20);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 19, 1);
    State_ApplyArgMode0AndSet10();
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 0, 1);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 1, 1);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 2, 1);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 3, 1);
    Call3((void (*)())Engine_ActorFaceDirection, 0, (v6 << 8), 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, (v6 << 8), 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Value2(Scene_CallPairWith10, 3, 0xa000);
    Call3(Engine_ActorShowEmote, 21, 0x101, 80);
    State_ApplyArgMode0AndSet10(21);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 40);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 20, 1);
    State_ApplyArgMode0AndSet10(20);
    Call3((void (*)())Engine_ActorFaceDirection, 0, (v6 << 8), 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, (v6 << 8), 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, (v6 << 8), 0);
    Value3(Engine_ActorFaceDirection, 3, (v6 << 8), 20);
    Call2((void (*)())Scene_CallPairWith10, 20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 21, 3);
    Call3((void (*)())Engine_ActorFaceDirection, 21, 0xb000, 60);
    Call3(Engine_ActorFaceDirection, 21, 0x3000, 40);
    ((void (*)())Engine_ActorRunRepeatedMotion)(19, 1);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3((void (*)())Engine_ActorFaceDirection, 21, 0, 40);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorFaceDirection, 20, 0xd000, 40);
    Call2((void (*)())Scene_CallPairWith10, 20, 0xb000);
    State_ApplyArgMode0AndSet10(0x2014);
    Call3(Engine_ActorFaceDirection, 21, 0x3000, 40);
    Call3(Engine_ActorFaceDirection, 21, 0x3000, 20);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 21, 3);
    Call3((void (*)())Engine_ActorFaceDirection, 19, 0, 40);
    Call3((void (*)())Engine_ActorFaceDirection, 19, 0xb000, 20);
    Call3(Engine_ActorShowEmote, 19, 0x101, 40);
    State_ApplyArgMode0AndSet10(0x2013);
    Call2(Scene_CallPairWith10, 21, 0x3000);
    Call3(Engine_ActorShowEmote, 21, 0x101, 20);
    State_ApplyArgMode0AndSet10(21);
    Value3(Engine_ActorFaceDirection, 20, 0xb000, 20);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 80);
    State_ApplyArgMode0AndSet10(21);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 19, 3);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 20);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 21, 2);
    State_ApplyArgMode0AndSet10(21);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x102, 60);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 21, 1);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 20, 0x108, 40);
    Call2((void (*)())Engine_ActorSetAnimation, 20, 4);
    Call3((void (*)())Engine_EventShowMessageAndWait, 0x2014, 0, 40);
    Call2((void (*)())Engine_ActorRunRepeatedMotion, 21, 1);
    Engine_EventWait(20);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 3, 0xa000, 20);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 20);
    Call2((void (*)())Engine_ActorSetAnimation, 19, 3);
    Call2((void (*)())Engine_ActorSetAnimationAndWait, 20, 3);
    Call3((void (*)())Engine_ActorFaceDirection, 21, 0xb000, 20);
    Call1(State_ApplyArgMode0AndSet10, 0xa015);
    Call3(Engine_ActorSetSpeed, 6, 0x10000, (v6 << 8));
    Call3(Engine_ActorSetSpeed, 21, 0x10000, (v6 << 8));
    base7_200e22c = 0x200e22c;
    Call2((void (*)())Engine_ActorEnableActionCallback, 21, base7_200e22c);
    Engine_EventWait(20);
    Value2(Engine_ActorEnableActionCallback, 6, base7_200e22c);
    Call2((void (*)())Scene_CallPairWith10, 1, 0x6000);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 1, 2);
    State_ApplyArgMode0AndSet10(1);
    Engine_AudioPlayCue(17);
    Call2(Engine_ColorBufferApplyTarget, 0x40250d, 1);
    Engine_ColorBufferInterpolate(40);
    Call2(Scene_CallPairWith10, 20, 0xd000);
    State_ApplyArgMode0AndSet10(20);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 2, (v6 << 8), 0);
    Value3(Engine_ActorFaceDirection, 3, (v6 << 8), 0);
    Scene_RunSetupSequence35c4();
    Call2((void (*)())Engine_ActorSetChildValue, 20, 7);
    ((void (*)())Engine_ActorSetChildValue)(19, 7);
    Engine_EventWait(20);
    Call2(Engine_ActorSetChildValue, 20, 0x100);
    Call2(Engine_ActorSetChildValue, 19, 0x100);
    Engine_EventWait(20);
    Scene_RunSetupSequence35c4();
    Call3((void (*)())Engine_ActorJump, 3, 2, 20);
    State_ApplyArgMode0AndSet10(3);
    Call2((void (*)())Scene_CallPairWith10, 19, 0);
    State_ApplyArgMode0AndSet10(19);
    Scene_RunSetupSequence35c4();
    rec4 = Engine_ActorGet(19);
    *(s32 *)(slot96 + 4) = 7;
    *(s32 *)(slot96 + 36) = 0x20083a1;
    *(s32 *)(slot96 + 8) = 0x10000;
    *(s32 *)(slot96 + 12) = 0x10000;
    p8 = slot96;
    base7_0 = 0;
        base6_54 = 84;
    do {
        slot32 = (base7_0 << 12);
        record = Engine_MathCos((base7_0 << 12));
        *(s32 *)(base6_54 + 4) = 0;
        *(s32 *)base6_54 = (s32)record;
        value = Engine_MathSin(slot32);
        *(s32 *)(base6_54 + 8) = (value << 1);
        *(s32 *)base6_54 += (*(s32 *)base6_54 << 1);
        p4 = *(s32 *)(rec4 + 8);
        slot28 = (s32)p4;
        p4 = *(s32 *)(base6_54 + 4);
        base7_0 = (base7_0 + 1);
        Call7(Effect_Spawn, slot28, *(s32 *)(rec4 + 12), *(s32 *)(rec4 + 16), (s32)p4, (value << 1), 0x1090000, p8);
        v9 = base6_54;
    } while ((u32)base7_0 <= 16);
    Engine_AudioPlayCue(212);
    Engine_TaskWait(6);
    Scene_RunSetupSequence35c4();
    rec4 = Value1(Engine_ActorGet, 20);
    *(s32 *)(slot44 + 4) = 7;
    *(s32 *)(slot44 + 36) = 0x20083a1;
    *(s32 *)(slot44 + 8) = 0x10000;
    *(s32 *)(slot44 + 12) = 0x10000;
    p8b = slot44;
    base7_0 = 0;
    p6b = v9;
    do {
        slot24 = (base7_0 << 12);
        record = Engine_MathCos((base7_0 << 12));
        *(s32 *)(p6b + 4) = 0;
        *(s32 *)p6b = (s32)record;
        value = Engine_MathSin(slot24);
        *(s32 *)(p6b + 8) = (value << 1);
        *(s32 *)p6b += (s32)(*(s32 *)p6b << 1);
        p4 = *(s32 *)(rec4 + 8);
        slot20 = (s32)p4;
        p4 = *(s32 *)(p6b + 4);
        base7_0 = (base7_0 + 1);
        Call7(Effect_Spawn, slot20, *(s32 *)(rec4 + 12), *(s32 *)(rec4 + 16), (s32)p4, (value << 1), 0x1090000, p8b);
    } while ((u32)base7_0 <= 16);
    Engine_AudioPlayCue(212);
    Call3((void (*)())Engine_ActorJump, 2, 6, 20);
    Engine_AudioPlayCue(54);
    State_ApplyArgMode0AndSet10(2);
    Call2((void (*)())Engine_ActorSetAnimation, 20, 4);
    State_ApplyArgMode0AndSet10(20);
    Scene_RunSetupSequence35c4();
    Call3(Engine_ActorSetSpeed, 20, 0x3333, 0x1999);
    Call3(Engine_ActorSetSpeed, 19, 0x3333, 0x1999);
    *(u8 *)(Engine_ActorGet(20) + 90) &= 254;
    *(u8 *)(Engine_ActorGet(19) + 90) &= 254;
    Call4(Engine_ActorSetDestination, 20, 0x126, 196, 254);
    Call3(Engine_ActorSetDestination, 19, 0x126, 196);
    Value2(Engine_TaskAddCallback, 0x200b7c5, 0xc80);
    Call2((void (*)())Engine_ActorStartRepeatedMotion, 1, 2);
    Call2((void (*)())Engine_EventShowMessage, 1, 0);
    Call1(Engine_GameFlagSet, 0x234);
    Call2((void (*)())Engine_EventShowMessage, 2, 0);
    Call1(Engine_GameFlagSet, 0x235);
    Scene_RunSetupSequence35c4();
    Engine_EventWait(20);
    Scene_RunSetupSequence35c4();
    Engine_EventWait(20);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Value2(Engine_GameStateSetReturn, (s32)Data_000000bb, 3);
    Call2((void (*)())Engine_Import0808a268, (s32)Data_000000bb, 9);
    Value2(Engine_Import0808a250, 98, 0);
    Engine_Import08077268();
    Call1(Engine_GameFlagSet, 0x351);
    p10 = (s32)Data_000000bb;
    p11 = base7_0;
}
