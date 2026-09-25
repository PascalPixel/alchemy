/* NONMATCHING: 2508 bytes, candidate 2500, 1008 differing halfwords, 469
 * halfword edits (2026-09-25). Scene_RunScriptedActorPresentation, meant for
 * FIELD/VINASU_CHOJO/F_008B4.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: 1008 differing halfwords. Fresh decompile; Scene_RunScene3c9
 * callee renamed Local_02001280 (overlay-local at 0x02001280). WALL: far —
 * hand-write from inspect --asm. Recon scripted_actor_presentation.c fails
 * xgcc. */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x02000a6c: 0x00000000  constant; Value_00000000 if the pool must hold it
 *   0x02000a70: 0x02450000
 *   0x02000a74: 0x00000245  constant; Value_00000245 if the pool must hold it
 *   0x02000a78: 0x00000212  constant; Value_00000212 if the pool must hold it
 *   0x02000a7c: 0x00000213  constant; Value_00000213 if the pool must hold it
 *   0x02000a80: 0x00000209  constant; Value_00000209 if the pool must hold it
 *   0x02000a84: 0x00000203  constant; Value_00000203 if the pool must hold it
 *   0x02000e98: 0xffe00000
 *   0x02000e9c: 0x00000212  constant; Value_00000212 if the pool must hold it
 *   0x02000ea0: 0x00006666  constant; Value_00006666 if the pool must hold it
 *   0x02000ea4: 0x0000cccc  constant; Value_0000cccc if the pool must hold it
 *   0x02000ea8: 0x00000202  constant; Value_00000202 if the pool must hold it
 *   0x02000eac: 0x00004013  constant; Value_00004013 if the pool must hold it
 *   0x02000eb0: 0x00000121  constant; Value_00000121 if the pool must hold it
 *   0x02000eb4: 0x00002757  constant; Value_00002757 if the pool must hold it
 *   0x02000eb8: 0x00009999  constant; Value_00009999 if the pool must hold it
 *   0x02000ebc: 0x0004cccc
 *   0x02000ec0: 0x00002014  constant; Value_00002014 if the pool must hold it
 *   0x02000ec4: 0x00000105  constant; Value_00000105 if the pool must hold it
 *   0x02000ec8: 0x00019999
 *   0x02000ecc: 0x00003333  constant; Value_00003333 if the pool must hold it
 *   0x02000ed0: 0x00000103  constant; Value_00000103 if the pool must hold it
 *   0x02000ed4: 0x00000101  constant; Value_00000101 if the pool must hold it
 *   0x02000ed8: 0x00002013  constant; Value_00002013 if the pool must hold it
 *   0x0200124c: 0x00000101  constant; Value_00000101 if the pool must hold it
 *   0x02001250: 0x00006666  constant; Value_00006666 if the pool must hold it
 *   0x02001254: 0x00000ccc  constant; Value_00000ccc if the pool must hold it
 *   0x02001258: 0x00008015  constant; Value_00008015 if the pool must hold it
 *   0x0200125c: 0x00000103  constant; Value_00000103 if the pool must hold it
 *   0x02001260: 0x0000a014  constant; Value_0000a014 if the pool must hold it
 *   0x02001264: 0x00000105  constant; Value_00000105 if the pool must hold it
 *   0x02001268: 0x0000a015  constant; Value_0000a015 if the pool must hold it
 *   0x0200126c: 0x03001ebc  SceneWorkCell
 *   0x02001270: 0xffe80000
 *   0x02001274: 0x00008001  constant; Value_00008001 if the pool must hold it
 *   0x02001278: 0x00001002  constant; Value_00001002 if the pool must hold it
 *   0x0200127c: 0x0200dfc4  overlay data at 0x02005fc4
 */

extern u8 Data_00000000[];
void State_ApplyArgMode0AndSet10();
s32 Scene_CallPairWith10();
void Local_02001280();
void Engine_TaskWait();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
s32 Engine_ActorGet();
s32 Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_ActorSetDestination();
void Engine_CameraMoveTo();
void Engine_ActorSetPositionAndCommit();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetAnimation();
void Main_0808a118();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_ActorWalkTo();
void Engine_AudioPlayCue();
void Engine_ActorWalkToAndWait();
void Engine_ActorWaitForMove();
s32 Engine_ObjectMotionArmCallback();
void Engine_EventSetMessage();
void Engine_ActorRunRepeatedMotion();
s32 Engine_EventGetViewCenter();
void Engine_CameraWaitForMove();
void Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessageAndWait();
void Engine_ActorStartRepeatedMotion();
void Engine_CameraSetSpeed();
void Engine_MapRedraw();
void Engine_ActorJump();
void Engine_EventEnd();
s32 Engine_ActorEnableActionCallback();
s32 Engine_EventOpenMessage();
void Main_0808a0b0();
void Engine_EventCloseScreen();

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

void Scene_RunScriptedActorPresentation(void)
{
    u32 i;
    s32 p8;
    u8 *rec7;
    u8 *rec8;
    u8 *record;
    s32 none;
    s32 v5;
    s32 base6_4013;
    s32 base5_2014;
    s32 v6;
    s32 base6_8015;
    s32 base5_a014;
    s32 base6_3001ebc;
    s32 base5_8001;
    s32 base5_200dfc4;

    rec7 = Value1(Engine_ActorGet, 18);
    Engine_EventBegin();
    Local_02001280(1, 0);
    Local_02001280(2, 0);
    Local_02001280(3, 0);
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    none = 0;
    rec7[85] = none;
    {
        u8 value = *(volatile u8 *)&rec7[35];
    
        rec7[35] = (u8)(value | 2);
    }
    record = Engine_ActorGet(18);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(18, 1);
    Call3(Engine_ActorSetPosition, 18, 0x2440000, 0x1520000);
    rec8 = Value1(Engine_ActorGet, 0);
    rec8[85] = none;
    Engine_ActorSetSpritePriority(0, 1);
    Call3(Engine_ActorSetPosition, 0, 0x2450000, 0x1200000);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 18, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    *(u8 *)(Engine_ActorGet(0) + 90) &= 254;
    Call3(Engine_ActorSetDestination, 18, 0x244, 221);
    Call3(Engine_ActorSetPositionAndCommit, 0, 0x245, 171);
    Call3(Engine_ActorSetDestination, 18, 0x212, 211);
    Call3(Engine_ActorSetPositionAndCommit, 0, 0x213, 161);
    Call3(Engine_ActorSetDestination, 18, 0x208, 191);
    Call3(Engine_ActorSetPositionAndCommit, 0, 0x209, 141);
    record = Engine_ActorGet(18);
    Engine_ActorSetSpriteFlags((s32)record, 1);
    Call3(Engine_ActorSetDestination, 18, 0x203, 171);
    Call3(Engine_ActorSetPositionAndCommit, 0, 0x204, 121);
    Call1(Engine_AudioPlayCue, 0x120);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 6);
    Main_0808a118(0);
    *(s32 *)((s32)rec8 + 8) = 0x2040000;
    *(s32 *)((s32)rec8 + 12) = 0x80000;
    *(s32 *)((s32)rec8 + 16) = 0x940000;
    {
        s32 shown = 0x8000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    rec8[85] = 3;
    p8 = (s32)Data_00000000;
    Engine_AudioPlayCue(152);
    *(s32 *)((s32)rec8 + 40) = 0x40000;
    Engine_AudioPlayCue(152);
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags((s32)record, 1);
    Call3(Engine_ActorSetPositionAndCommit, 0, 0x1f8, 148);
    v5 = 1;
    Engine_EventWait(10);
    *(u8 *)(Engine_ActorGet(0) + 90) |= v5;
    *(s32 *)((s32)rec8 + 12) = -0x200000;
    {
        s32 shown = 0x4000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Engine_EventWait(20);
    Call1(Engine_AudioPlayCue, 0x134);
    Call3(Engine_ActorSetPositionAndCommit, 18, 0x20c, 191);
    record = Engine_ActorGet(18);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Call3(Engine_ActorSetPositionAndCommit, 18, 0x212, 211);
    Call3(Engine_ActorSetPositionAndCommit, 18, 0x244, 221);
    Call3(Engine_ActorSetDestination, 18, 0x244, 0x152);
    {
        u8 *record = Engine_ActorGet(0);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | v5);
    }
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Local_02001280(1, 1);
    Local_02001280(2, 1);
    Local_02001280(3, 1);
    Call3(Engine_ActorWalkTo, 0, 0x1ec, 164);
    Call3(Engine_ActorWalkTo, 1, 0x202, 164);
    Call3(Engine_ActorWalkTo, 2, 0x1ec, 140);
    Call3(Engine_ActorWalkToAndWait, 3, 0x202, 140);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Engine_ObjectMotionArmCallback(0, 0x4000, 0);
    Engine_ObjectMotionArmCallback(1, 0x4000, 0);
    Engine_ObjectMotionArmCallback(2, 0x4000, 0);
    Engine_ObjectMotionArmCallback(3, 0x4000, 0);
    Engine_ActorWaitForMove(18);
    Engine_ActorSetPosition(18, 0, 0);
    base6_4013 = 0x4013;
    Call1(Engine_AudioPlayCue, 0x121);
    Call1(Engine_EventSetMessage, 0x2757);
    State_ApplyArgMode0AndSet10(base6_4013);
    Engine_ObjectMotionArmCallback(0, 0x8000, 0);
    Engine_ObjectMotionArmCallback(1, 0x8000, 0);
    Engine_EventWait(2, 0x8000, 0);
    Value2(Scene_CallPairWith10, 3, 0x8000);
    *(u8 *)(Engine_EventGetViewCenter() + 85) = p8;
    Call2(Engine_ActorSetSpeed, 0x4cccc, 0x9999);
    Call4(Engine_CameraMoveTo, 0x1300000, 0x200000, 0x9e0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    base5_2014 = 0x2014;
    State_ApplyArgMode0AndSet10(20, 0xd000);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_AudioPlayCue(61);
    State_ApplyArgMode0AndSet10(base5_2014);
    Engine_ActorSetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(base6_4013);
    Scene_CallPairWith10(20, 0xb000);
    Call3(Engine_ActorShowEmote, 20, 0x105, 40);
    State_ApplyArgMode0AndSet10(base5_2014);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 19, 0x100, 0);
    Call3(Engine_ActorShowEmote, 20, 0x100, 20);
    Call3(Engine_ObjectMotionArmCallback, 6, 0x3000, 0);
    Call3(Engine_ObjectMotionArmCallback, 19, 0x5000, 0);
    Call3(Engine_ObjectMotionArmCallback, 20, 0x5000, 20);
    Call2(Engine_ActorSetAnimationAndWait, 0x19999, 0x3333);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xc20000, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 21, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 21, 0x110, 200);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    State_ApplyArgMode0AndSet10(base5_2014);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    State_ApplyArgMode0AndSet10(19);
    Engine_ActorSetAnimationAndWait(21, 3);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 40);
    State_ApplyArgMode0AndSet10(base5_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    Engine_EventShowMessageAndWait(19, 0, 40);
    Call3(Engine_ActorShowEmote, 19, 0x106, 40);
    Scene_CallPairWith10(19, 0x8000);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ObjectMotionArmCallback(6, 0, 0);
    Call3(Engine_ActorShowEmote, 21, 0x103, 40);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ObjectMotionArmCallback, 19, 0x3000, 0);
    Engine_EventWait(20, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(21, 1);
    v6 = 160;
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ObjectMotionArmCallback, 20, 0x5000, 0);
    Value2(Scene_CallPairWith10, 19, (v6 << 7));
    Call3(Engine_ActorShowEmote, 19, 0x108, 20);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 20);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ObjectMotionArmCallback(20, 0x8000, 40);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventShowMessageAndWait(base5_2014, 0, 40);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Value3(Engine_ObjectMotionArmCallback, 20, (v6 << 7), 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 20, 0x105, 0);
    Call3(Engine_ActorShowEmote, 19, 0x105, 80);
    Engine_ActorStartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ActorSetAnimationAndWait(21, 3);
    State_ApplyArgMode0AndSet10(21);
    Engine_ActorStartRepeatedMotion(19, 1);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Call3(Engine_ActorShowEmote, 20, 0x105, 60);
    Engine_EventShowMessageAndWait(base5_2014, 0, 20);
    Scene_CallPairWith10(21, 0xb000);
    State_ApplyArgMode0AndSet10(21);
    Value2(Scene_CallPairWith10, 6, 0x3000);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(20);
    Scene_CallPairWith10(21, 0xd000);
    Engine_ActorSetAnimationAndWait(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ActorWalkToAndWait(6, 0, 0);
    Engine_ActorRunRepeatedMotion(21, 1);
    State_ApplyArgMode0AndSet10(21);
    Engine_ActorSetAnimationAndWait(20, 3);
    State_ApplyArgMode0AndSet10(base5_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    State_ApplyArgMode0AndSet10(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 0);
    Call3(Engine_ActorShowEmote, 19, 0x101, 80);
    Engine_ObjectMotionArmCallback(19, 0x8000, 0);
    Engine_ObjectMotionArmCallback(20, 0x8000, 0);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xb40000, 1);
    Call3(Engine_ActorWalkToAndWait, 21, 0x106, 176);
    base6_8015 = 0x8015;
    Engine_ObjectMotionArmCallback(21, 0x8000, 40);
    Engine_MapRedraw(21, 0, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(base6_8015);
    Call3(Engine_ActorShowEmote, 19, 0x100, 20);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ActorStartRepeatedMotion(21, 2);
    State_ApplyArgMode0AndSet10(base6_8015);
    Call3(Engine_ActorShowEmote, 20, 0x103, 40);
    Call3(Engine_EventShowMessageAndWait, 0xa014, 0, 20);
    Call3(Engine_ActorShowEmote, 21, 0x105, 20);
    State_ApplyArgMode0AndSet10(base6_8015);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x101, 40);
    base5_a014 = 0xa014;
    State_ApplyArgMode0AndSet10(base6_8015);
    Engine_ActorSetAnimationAndWait(20, 4);
    State_ApplyArgMode0AndSet10(base5_a014);
    Engine_ActorSetAnimationAndWait(19, 3);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 60);
    Engine_ObjectMotionArmCallback(21, 0x8000, 20);
    Call3(Engine_EventShowMessageAndWait, 0xa015, 0, 40);
    Call3(Engine_ActorShowEmote, 6, 0x105, 120);
    Call3(Engine_ObjectMotionArmCallback, 20, 0x105, 60);
    State_ApplyArgMode0AndSet10(base5_a014);
    Engine_ObjectMotionArmCallback(21, 0, 40);
    Engine_ActorSetAnimation(19, 3);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventShowMessageAndWait(base6_8015, 0, 20);
    State_ApplyArgMode0AndSet10(base5_a014);
    Call3(Engine_ActorStartRepeatedMotion, 21, 0x100, 40);
    Engine_ActorSetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    Call3(Engine_ActorShowEmote, 6, 0x105, 40);
    Call3(Engine_ActorShowEmote, 20, 0x108, 40);
    State_ApplyArgMode0AndSet10(base5_a014);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    State_ApplyArgMode0AndSet10(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(20, 2);
    State_ApplyArgMode0AndSet10(base5_a014);
    Engine_ActorSetAnimation(19, 4);
    State_ApplyArgMode0AndSet10(0x2013);
    base6_3001ebc = 0x3001ebc;
    *(s32 *)((*(s32 *)base6_3001ebc + 0x1c0)) = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1f80000, -0x180000, 0xa80000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    base5_8001 = 0x8001;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(1, 1);
    State_ApplyArgMode0AndSet10(base5_8001);
    Call3(Engine_EventShowMessageAndWait, 3, 0x101, 40);
    State_ApplyArgMode0AndSet10(3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Call3(Engine_EventShowMessageAndWait, 0x1002, 0, 40);
    Engine_ActorJump(1, 2, 20);
    State_ApplyArgMode0AndSet10(base5_8001);
    Value2(Engine_EventOpenMessage, base5_8001, 0);
    Engine_ActorSetAnimation(0, 0, 0);
    Engine_ObjectMotionArmCallback(2, 0x4000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0x2000, 0);
    if (Value2(Engine_EventWait, 0, 0) == 1) {
        *(u16 *)((*(s32 *)base6_3001ebc + 0x1d8)) += 1;
    }
    ((void (*)())Engine_EventWait)(20);
    State_ApplyArgMode0AndSet10(1);
    Engine_ObjectMotionArmCallback(2, 0, 0);
    Value3(Engine_ObjectMotionArmCallback, 3, 0x8000, 20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    base5_200dfc4 = 0x200dfc4;
    Engine_ActorEnableActionCallback(1, base5_200dfc4);
    Value2(Engine_ActorEnableActionCallback, 2, base5_200dfc4);
    Main_0808a0b0(3, base5_200dfc4);
    Engine_EventWait(20);
    Engine_EventEnd();
}
