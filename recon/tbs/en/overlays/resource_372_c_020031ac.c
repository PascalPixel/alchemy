/* NONMATCHING: 2716 bytes, candidate 2692, 1133 differing halfwords, 601
 * halfword edits (2026-09-25). Scene_RunActorGroupDepartureSequence, meant
 * for FIELD/HAIDIA_ARASHI/F_031AC.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: 1133 differing halfwords. Fresh decompile with
 * Local_030003e0 aliased to Func_02004634 (IWRAM import). WALL: far from
 * exact — hand-write from inspect --asm next; prior Func_02 recon was
 * 1309. */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x02003394: 0x00000000  constant; Value_00000000 if the pool must hold it
 *   0x02003398: 0x058b0000
 *   0x0200339c: 0x00013333
 *   0x020033a0: 0x00009999  constant; Value_00009999 if the pool must hold it
 *   0x020033a4: 0x0200cd6c  overlay data at 0x02004d6c
 *   0x020033a8: 0x05940000
 *   0x020033ac: 0x052d0000
 *   0x020033b0: 0x0200ce04  overlay data at 0x02004e04
 *   0x020033b4: 0x0200ce30  overlay data at 0x02004e30
 *   0x020033b8: 0x0200ce5c  overlay data at 0x02004e5c
 *   0x020033bc: 0x0200ce88  overlay data at 0x02004e88
 *   0x020033c0: 0x00000101  constant; Value_00000101 if the pool must hold it
 *   0x020037ec: 0x0200ceb4  overlay data at 0x02004eb4
 *   0x020037f0: 0x0000e666  constant; Value_0000e666 if the pool must hold it
 *   0x020037f4: 0x043c0000
 *   0x020037f8: 0x0000cccc  constant; Value_0000cccc if the pool must hold it
 *   0x020037fc: 0x03820000
 *   0x02003800: 0x03950000
 *   0x02003804: 0x00010003
 *   0x02003808: 0x0200bce5  Local_02003ce4 (overlay function)
 *   0x0200380c: 0x0200cedc  overlay data at 0x02004edc
 *   0x02003810: 0x000007ae  constant; Value_000007ae if the pool must hold it
 *   0x02003814: 0x043e0000
 *   0x02003818: 0x050c0000
 *   0x0200381c: 0x05210000
 *   0x02003820: 0x05350000
 *   0x02003824: 0x05390000
 *   0x02003828: 0x04ac0000
 *   0x02003abc: 0x00000002  constant; Value_00000002 if the pool must hold it
 *   0x02003ac0: 0x0200be19  Local_02003e18 (overlay function)
 *   0x02003ac4: 0x00010003
 *   0x02003ac8: 0x00007fff  constant; Value_00007fff if the pool must hold it
 *   0x02003acc: 0x04cd0000
 *   0x02003ad0: 0x0200cec8  overlay data at 0x02004ec8
 *   0x02003ad4: 0x04e60000
 *   0x02003ad8: 0x05050000
 *   0x02003c30: 0x051f0000
 *   0x02003c34: 0x0200c5b9  Local_020045b8 (overlay function)
 *   0x02003c38: 0x04f90000
 *   0x02003c3c: 0x042e0000
 *   0x02003c40: 0x04f60000
 *   0x02003c44: 0x00010003
 */

extern u8 Data_00000000[];
extern u8 Data_00000002[];
void SceneState_SetFlag210AndConfigureRegion40_89();
void FieldScene_RunScene372_02003c48();
s32 Engine_ActorGet();
void Engine_EventWait();
void Engine_ActorSetSpeed();
s32 Engine_ActorEnableActionCallback();
s32 Func_02004634();
#define Local_030003e0 Func_02004634
s32 Engine_WorkSetValuesIfNonNegative();
s32 Engine_RandomNext();
void Engine_CameraSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_CameraMoveTo();
s32 Main_0808a0b0();
void Engine_ActorFaceDirection();
void FieldScene_BuildPlacementGrid();
void Engine_ActorFaceEachOther();
void Engine_ActorShowEmote();
void Engine_TaskWait();
void Engine_ActorSetSpriteFlags();
s32 Engine_TaskAddCallback();
void Engine_MapRedraw();
void Engine_AudioPlayCue();
void Engine_TaskRemoveCallback();
void Engine_ActorSetChildValue();
void Engine_MapRenderWaitForValues();
void Main_0808a2d0();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
s32 Engine_EventGetViewCenter();
void Engine_ActorStop();
s32 Engine_ActorSetAnimation();
s32 Engine_ActorSetPosition();
void Engine_MapClearLayerEntryFlag();
void Engine_GameFlagSet();
void Main_08009188();
void Engine_GameFlagClear();
void Main_0808a058();
void Main_0808a2e0();
void Main_0808a5e8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_RunActorGroupDepartureSequence(void)
{
    u32 i;
    u8 *p10;
    s32 p10b;
    u8 *p11;
    u8 *p11b;
    u8 *p11c;
    u8 *p11d;
    u8 *p11e;
    s32 p8;
    u8 *rec;
    s32 rec7;
    u8 *rec8;
    s32 record;
    s32 base5_200cd6c;
    s32 none;
    s32 base5_200ceb4;
    s32 v5;
    s32 base5_200cec8;
    s32 v1;
    u8 *slot0;

    rec = Value1(Engine_ActorGet, 19);
    rec8 = Value1(Engine_ActorGet, 27);
    p11 = *(s32 *)((s32)rec8 + 80);
    p10 = *(s32 *)((s32)rec + 80);
    Call2(Engine_CameraSetSpeed, 0x10000, 0x2000);
    Call4(Engine_CameraMoveTo, 0x6e0000, -1, 0x58b0000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 26, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 0, 0x13333, 0x9999);
    Call3(Engine_ActorSetSpeed, 22, 0x13333, 0x9999);
    base5_200cd6c = 0x200cd6c;
    Engine_ActorEnableActionCallback(8, base5_200cd6c);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(26, base5_200cd6c);
    Engine_ActorFaceDirection();
    Engine_EventWait(10);
    Call2((void (*)())Local_030003e0, 0, base5_200cd6c);
    Engine_EventWait(10);
    Engine_CameraMoveTo();
    Engine_ActorEnableActionCallback(22, base5_200cd6c);
    Engine_EventWait(128);
    SceneState_SetFlag210AndConfigureRegion40_89();
    Call4(Engine_ActorGet, 0xae0000, -1, 0x5940000, 1);
    Engine_EventWait(104);
    Call4(Engine_CameraMoveTo, 0x990000, -1, 0x52d0000, 1);
    Call3(Engine_ActorWalkToAndWait, 9, 158, 0x4f8);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    ((void (*)())Engine_ActorEnableActionCallback)(8);
    Value2(Engine_ActorEnableActionCallback, 8, 0x200ce04);
    Value2(Engine_ActorEnableActionCallback, 26, 0x200ce30);
    Value2(Engine_ActorEnableActionCallback, 0, 0x200ce5c);
    Value2(Main_0808a0b0, 22, 0x200ce88);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_TaskWait(145);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 26, 0x101, 0);
    Call3(Engine_EventWait, 22, 0x101, 0);
    Call3(Engine_ActorShowEmote, 8, 0x101, 0);
    Call3(Engine_ActorShowEmote, 9, 0x101, 60);
    FieldScene_BuildPlacementGrid(26, 8, 0);
    Engine_ActorFaceEachOther(22, 0, 0);
    Engine_EventWait(20);
    rec7 = Value1(Engine_ActorGet, 0);
    record = Engine_RandomNext();
    *(u16 *)(rec7 + 100) = (Local_030003e0(record, 20) + 20);
    none = 0;
    p8 = (s32)Data_00000000;
    rec7 = Value1(Engine_ActorGet, 22);
    record = Engine_RandomNext();
    *(u16 *)(rec7 + 100) = (Local_030003e0(record, 20) + 20);
    rec7 = Value1(Engine_ActorGet, 26);
    record = Engine_RandomNext();
    *(u16 *)(rec7 + 100) = (Local_030003e0(record, 20) + 20);
    rec7 = Value1(Engine_ActorGet, 8);
    record = Engine_RandomNext();
    *(u16 *)(rec7 + 100) = (Local_030003e0(record, 20) + 20);
    rec7 = Value1(Engine_ActorGet, 9);
    record = Engine_RandomNext();
    *(u16 *)(rec7 + 100) = (Local_030003e0(record, 20) + 20);
    base5_200ceb4 = 0x200ceb4;
    Engine_ActorEnableActionCallback(9, base5_200ceb4);
    Engine_EventWait(30);
    Value2(Engine_ActorEnableActionCallback, 0, base5_200ceb4);
    Value2(Engine_ActorEnableActionCallback, 26, base5_200ceb4);
    Value2(Engine_WorkSetValuesIfNonNegative, 22, base5_200ceb4);
    Engine_ActorEnableActionCallback(8, base5_200ceb4);
    Engine_EventWait(10);
    Engine_AudioPlayCue(17);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(30);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_AudioPlayCue(145);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(60);
    Main_0808a2d0();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(1);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Call2(Engine_CameraSetSpeed, 0x80000, 0x80000);
    Call4(Engine_CameraMoveTo, 0xd90000, -1, 0x43c0000, 1);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(40);
    Engine_ActorSetChildValue(19, 0);
    record = Engine_ActorGet(19);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Engine_ActorGet(27);
    Engine_ActorSetSpriteFlags(record, 0);
    *(s32 *)((s32)rec8 + 24) = 0xcccc;
    *(s32 *)((s32)rec8 + 28) = 0xcccc;
    rec8[35] &= 254;
    p11[9] = ((s32)(-13 & p11[9]) | 4);
    *(s32 *)((s32)rec + 8) = 0xc80000;
    *(s32 *)((s32)rec + 12) = 0xc80000;
    *(s32 *)((s32)rec + 56) = 0xc80000;
    *(s32 *)((s32)rec + 60) = 0xc80000;
    *(s32 *)((s32)rec + 16) = 0x3820000;
    *(s32 *)((s32)rec + 64) = 0x3820000;
    slot0 = ((s32)rec + 85);
    slot0[0] = p8;
    rec[35] &= 254;
    p10[9] &= -13;
    v5 = 128;
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 56) = (v5 << 24);
    record = Value0(Engine_EventGetViewCenter);
    *(s32 *)(record + 60) = (v5 << 24);
    record = Value0(Engine_EventGetViewCenter);
    *(s32 *)(record + 64) = (v5 << 24);
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 36) = none;
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 40) = none;
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 44) = none;
    Engine_TaskWait(1);
    Call4(Engine_CameraMoveTo, 0xf70000, 0x800000, 0x3950000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ActorSetAnimation, 0x10000, 2);
    Engine_ColorBufferInterpolate(30);
    Engine_TaskWait(30);
    Value2(Engine_TaskAddCallback, 0x200bce5, 0xc80);
    Value2(Engine_ActorEnableActionCallback, 19, 0x200cedc);
    Call2(Engine_CameraSetSpeed, 0x20000, 0x7ae);
    Call4(Engine_CameraMoveTo, 0xaf0000, 0x600000, 0x43e0000, 1);
    for (i = 0; (i >> 16) != 8; i += 0x10000) {
        Engine_TaskWait(1);
    }
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_MapRenderWaitForValues();
    record = Engine_ActorSetPosition();
    *(s32 *)(record + 56) = -0x80000000;
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 60) = -0x80000000;
    record = Engine_EventGetViewCenter();
    *(s32 *)(record + 64) = -0x80000000;
    v5 = 0;
    record = Engine_ActorSetAnimation();
    *(s32 *)(record + 36) = v5;
    record = Value0(Engine_EventGetViewCenter);
    *(s32 *)(record + 40) = v5;
    record = Value0(Engine_EventGetViewCenter);
    *(s32 *)(record + 44) = v5;
    Call1(Engine_TaskRemoveCallback, 0x200bce5);
    Engine_ActorStop(19);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(19, 0);
    *(s32 *)((s32)rec8 + 24) = 0x14000;
    *(s32 *)((s32)rec8 + 28) = 0x14000;
    p11[35] = 2;
    *(s32 *)((s32)p11 + 24) = 0x14000;
    *(s32 *)((s32)rec + 24) = 0x20000;
    *(s32 *)((s32)rec + 28) = 0x20000;
    *(s32 *)((s32)rec + 8) = v5;
    *(s32 *)((s32)rec + 16) = v5;
    *(s32 *)((s32)rec + 56) = v5;
    *(s32 *)((s32)rec + 64) = v5;
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(23, 8);
    Call3(Engine_ActorSetPosition, 9, 0xa90000, 0x4f00000);
    Call3(Engine_ActorSetAnimation, 9, 0xc000, 0);
    Engine_ActorSetAnimation(9, 9);
    Call3(Engine_ActorSetPosition, 26, 0x970000, 0x50c0000);
    Call3(Engine_ActorFaceDirection, 26, 0x8000, 0);
    Call2((void (*)())Engine_ActorSetAnimation, 26, 5);
    Call3(Engine_ActorSetPosition, 8, 0xaa0000, 0x5210000);
    Call3(Engine_ActorFaceDirection, 8, 0x6000, 0);
    Engine_ActorSetAnimation(8, 5);
    Call3(Engine_ActorSetPosition, 0, 0xb90000, 0x5350000);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 0);
    Engine_ActorSetAnimation(0, 17);
    Call3(Engine_ActorSetPosition, 22, 0xa90000, 0x5680000);
    Call3(Engine_ActorFaceDirection, 22, 0x4000, 0);
    Engine_ActorSetAnimation(22, 0);
    Call4(Engine_CameraMoveTo, 0xa60000, 0, 0x5390000, 0);
    Engine_MapRedraw();
    slot0[0] = v5;
    *(s32 *)((s32)rec + 56) = -0x80000000;
    *(s32 *)((s32)rec + 60) = -0x80000000;
    *(s32 *)((s32)rec + 64) = -0x80000000;
    FieldScene_RunScene372_02003c48();
    Call3(Engine_ActorSetPosition, 27, 0xda0000, 0x4980000);
    Call4(Engine_CameraMoveTo, 0xd20000, 0, 0x4ac0000, 0);
    Engine_MapRedraw();
    *(s32 *)((s32)rec8 + 24) = 0x20000;
    *(s32 *)((s32)rec8 + 28) = 0x20000;
    Call2(Engine_TaskAddCallback, 0x200be19, 0xc80);
    Engine_ActorStop(10);
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_TaskWait(1);
    rec8 = Value1(Engine_ActorGet, 10);
    p11b = *(s32 *)((s32)rec8 + 80);
    rec8[35] &= 254;
    *(s32 *)((s32)rec8 + 24) = 0x10000;
    *(s32 *)((s32)rec8 + 28) = 0x10000;
    {
        s32 shown = 0xd000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    p11b[9] &= (v5 - 13);
    Engine_ActorSetAnimation(10, 0);
    rec8 = Value1(Engine_ActorGet, 24);
    p11c = *(s32 *)((s32)rec8 + 80);
    rec8[35] &= 254;
    *(s32 *)((s32)rec8 + 24) = 0x10000;
    *(s32 *)((s32)rec8 + 28) = 0x10000;
    p11c[9] &= (v5 - 13);
    {
        s32 shown = 0xb000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Engine_ActorSetAnimation(24, 5);
    rec8 = Value1(Engine_ActorGet, 25);
    p11d = *(s32 *)((s32)rec8 + 80);
    rec8[35] &= 254;
    *(s32 *)((s32)rec8 + 24) = 0x10000;
    *(s32 *)((s32)rec8 + 28) = 0x10000;
    {
        s32 shown = 0xb000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    p11d[9] &= (v5 - 13);
    Main_08009188(25, 5);
    rec8 = Value1(Engine_ActorGet, 27);
    p11e = *(s32 *)((s32)rec8 + 80);
    FieldScene_RunScene372_02003c48();
    *(s32 *)((s32)rec + 12) = 0x300000;
    *(s32 *)((s32)rec + 8) = 0xd60000;
    *(s32 *)((s32)rec + 16) = 0x4c00000;
    *(s32 *)((s32)rec + 56) = -0x80000000;
    *(s32 *)((s32)rec + 60) = -0x80000000;
    *(s32 *)((s32)rec + 64) = -0x80000000;
    p11e[9] = ((s32)((v5 - 13) & p11e[9]) | 4);
    Engine_ActorSetPosition(27);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 25, 0xc000, 20);
    Call1(Engine_GameFlagSet, 0x166);
    Engine_MapClearLayerEntryFlag(0);
    Engine_MapClearLayerEntryFlag(1);
    Engine_TaskRemoveCallback(2);
    Engine_MapClearLayerEntryFlag(3);
    Engine_MapClearLayerEntryFlag(4);
    Engine_MapClearLayerEntryFlag(5);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(160);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x7fff, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(80);
    Engine_EventWait(100);
    Call1(Engine_TaskRemoveCallback, 0x200be19);
    *(s32 *)((s32)p11e + 24) = *(s32 *)((s32)rec8 + 24);
    Call1(Engine_GameFlagClear, 0x166);
    Main_08009188(0);
    Main_08009188(1);
    Main_08009188(2);
    Main_08009188(3);
    Main_08009188(4);
    Main_08009188(5);
    FieldScene_BuildPlacementGrid();
    Call3(Engine_ActorSetPosition, 9, 0xa50000, 0x4cd0000);
    Engine_ActorSetAnimation(9, 1);
    rec = Engine_ActorGet(9);
    {
        s32 shown = 0xe000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    record = Engine_RandomNext();
    *(u16 *)((s32)rec + 100) = (Local_030003e0(record, 90) + 60);
    base5_200cec8 = 0x200cec8;
    {
        s32 shown = 1;
    
        *(u16 *)((s32)rec + 102) = shown;
    }
    Engine_ActorEnableActionCallback(9, base5_200cec8);
    Call3(Engine_ActorSetPosition, 26, 0xa50000, 0x4e60000);
    Engine_ActorSetAnimation(26, 1);
    rec = Value1(Engine_ActorGet, 26);
    {
        s32 shown = 0xe000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    record = Engine_RandomNext();
    *(u16 *)((s32)rec + 100) = (Local_030003e0(record, 90) + 60);
    *(u16 *)(((s32)rec + 100) + 2) = (s32)Data_00000002;
    Engine_ActorEnableActionCallback(26, base5_200cec8);
    Call3(Engine_TaskWait, 22, 0x980000, 0x5050000);
    Engine_ActorEnableActionCallback(22, 1);
    rec = Engine_ActorGet(22);
    {
        s32 shown = 0xe000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    record = Engine_RandomNext();
    *(u16 *)((s32)rec + 100) = (Local_030003e0(record, 90) + 60);
    {
        s32 shown = 3;
    
        *(u16 *)((s32)rec + 102) = shown;
    }
    Engine_ActorEnableActionCallback(22, base5_200cec8);
    Call3(Engine_ActorSetPosition, 8, 0xb40000, 0x51f0000);
    rec = Engine_ActorGet(8);
    {
        s32 shown = 0xe000;
    
        *(u16 *)((s32)rec + 6) = shown;
    }
    record = Engine_RandomNext();
    *(u16 *)((s32)rec + 100) = (Local_030003e0(record, 90) + 60);
    {
        s32 shown = 4;
    
        *(u16 *)((s32)rec + 102) = shown;
    }
    Engine_ActorEnableActionCallback(8, base5_200cec8);
    Engine_ActorSetAnimation(8, 6);
    *(u8 *)(Engine_ActorGet(22) + 35) &= 254;
    v1 = (254 & *(u8 *)(Engine_ActorGet(8) + 35));
    *(u8 *)(Engine_ActorGet(8) + 35) &= 254;
    p10b = v1;
    Value2(Engine_TaskAddCallback, 0x200c5b9, 0xc80);
    Call3(Engine_ActorSetPosition, 0, 0xb50000, 0x4f90000);
    record = Engine_ActorGet(0);
    {
        s32 shown = 0xe000;
    
        *(u16 *)(record + 6) = shown;
    }
    Engine_ActorSetAnimation(0, 1);
    Call4(Engine_CameraMoveTo, 0xb50000, 0, 0x4f90000, 0);
    Engine_MapRedraw();
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(24, 0, 0);
    Engine_ActorSetPosition(25, 0, 0);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(27, 0, 0);
    Call3(Engine_ActorSetPosition, 17, 0x900000, 0x42e0000);
    Call3(Engine_ActorSetPosition, 18, 0x1140000, 0x4f60000);
    Engine_TaskWait(60);
    Call2(Engine_ColorBufferApplyTarget, 0x10003, 1);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(80);
    Engine_EventWait(60);
    Main_0808a5e8();
    Engine_EventWait(60);
    Main_0808a058(1);
    Main_0808a2e0();
}
