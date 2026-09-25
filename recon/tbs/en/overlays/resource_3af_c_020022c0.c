/* NONMATCHING: 856 bytes, candidate 856, 84 differing halfwords, 56 halfword
 * edits (2026-09-25). FieldScene_RunActorSequence, meant for
 * FIELD/FUNE_KANPAN/F_022C0.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Corrected movement callees and callback symbols; halfword zero
 * fields restore the mid-function pools. The second zero pool is still four
 * bytes early, with actor/loop register lifetimes differing.
 * WALL: Late zero pool reach and actor/loop allocation. */
#include "TYPES.H"

extern u8 Data_00000000[];
extern u8 Data_0200c8c4[];
extern u8 Data_0200c8b0[];
extern u8 Data_0200c8d8[];
void FieldScene_RunScene3af_02000bb8();
void FieldScene_CallPairWith10();
s32 Engine_TaskWait();
void Engine_ActorSetSpriteFlags();
void ObjectDispatch_SetSingleChildField26Far(s32 object, s32 value);
void Engine_EventBegin();
void Main_0808a030();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetChildValue();
s32 Engine_ActorEnableActionCallback();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorStop();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_EventOpenScreen();
void Main_08009208();
void Main_08009210();
void Main_08015210();
void Main_0808a0b0();
void Engine_AudioPlayCue();
void Main_0808a1d8();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_EventRequestExit();
void Event_ClearStatus1c6Far();
void Event_WaitValue1c8FramesFar();

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

void FieldScene_RunActorSequence(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    s32 base5_200c8c4;
    s32 base5_200c8b0;
    s32 base5_200c8d8;
    s32 base5_0;
    s32 base5_200c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = Engine_ActorGet(0);
    ObjectDispatch_SetSingleChildField26Far(record, 0);
    Call1(Main_0808a030, 0x200d160);
    Engine_TaskWait(1);
    Call1(Main_0808a030, 0x200d340);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 22, 0xb00000, 0x2b80000);
    record = Engine_ActorGet(22);
    {
        s32 shown = 0xd000;

        *(u16 *)(record + 6) = shown;
    }
    Call3(Engine_ActorSetPosition, 21, 0x1080000, 0x2960000);
    record = Engine_ActorGet(21);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Call3(Engine_ActorSetPosition, 24, 0xb80000, 0x2a00000);
    Call3(Engine_ActorSetPosition, 25, 0xca0000, 0x2b40000);
    Call3(Engine_ActorSetPosition, 26, 0xfc0000, 0x2860000);
    Call3(Engine_ActorSetPosition, 27, 0x1000000, 0x2ae0000);
    Call3(Engine_ActorSetPosition, 28, 0xac0000, 0x2780000);
    Call3(Engine_ActorSetPosition, 29, 0x1000000, 0x26e0000);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        *(u8 *)(Engine_ActorGet(24) + 99) = zero.v;
        *(u8 *)(Engine_ActorGet(25) + 99) = 1;
        *(u8 *)(Engine_ActorGet(26) + 99) = zero.v;
        *(u8 *)(Engine_ActorGet(27) + 99) = 2;
    }
    Call3(Engine_ActorSetPosition, 20, 0, 0);
    Engine_ActorEnableActionCallback(24, (s32)Data_0200c8c4);
    Value2(Engine_ActorEnableActionCallback, 25, (s32)Data_0200c8c4);
    Engine_ActorEnableActionCallback(26, (s32)Data_0200c8b0);
    Value2(Engine_ActorEnableActionCallback, 27, (s32)Data_0200c8b0);
    Engine_ActorEnableActionCallback(28, (s32)Data_0200c8d8);
    Value2(Engine_ActorEnableActionCallback, 29, (s32)Data_0200c8d8);
    Engine_ActorSetChildValue(24, 3);
    Engine_ActorSetChildValue(25, 3);
    Engine_ActorSetChildValue(26, 3);
    Engine_ActorSetChildValue(27, 3);
    Engine_ActorSetChildValue(28, 3);
    Engine_ActorSetChildValue(29, 3);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    rec8 = Engine_ActorGet(31);
    *(s32 *)((s32)rec8 + 24) = 0x1999;
    *(s32 *)((s32)rec8 + 28) = 0x1999;
    *(s32 *)((s32)rec8 + 8) = 0xc20000;
    *(s32 *)((s32)rec8 + 16) = 0x2820000;
    base5_0 = 0;
    do {
        *(s32 *)((s32)rec8 + 24) += 0xf5c;
        *(s32 *)((s32)rec8 + 28) += 0xf5c;
        base5_0 = (base5_0 + 1);
        Engine_TaskWait(1);
    } while ((u32)base5_0 <= 15);
    rec8 = Engine_ActorGet(30);
    *(s32 *)((s32)rec8 + 24) = 0x11999;
    *(s32 *)((s32)rec8 + 28) = 0x11999;
    *(s32 *)((s32)rec8 + 8) = 0xc20000;
    *(s32 *)((s32)rec8 + 12) = 0x500000;
    *(s32 *)((s32)rec8 + 16) = 0x2820000;
    {
        s32 shown = 0x5000;

        *(u16 *)((s32)rec8 + 6) = shown;
    }
    *(s32 *)((s32)rec8 + 68) = 0x6666;
    *(s32 *)((s32)rec8 + 72) = 0x20000;
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    Engine_ActorSetPosition(31, 0, 0);
    record = Engine_ActorGet(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x19999, 0xcccc);
    rec8 = (u8 *)Engine_ActorGet(0);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        rec8[85] = zero.v;
    }
    Call3(Engine_ActorWalkToAndWait, 0, 216, 0x264);
    Call3(Engine_ActorSetSpeed, 30, 0x19999, 0xcccc);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 196, 0x258);
    Call3(Engine_ObjectMotionSetPositionAndCommit, 30, 216, 0x258);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 28, 0x19999, 0xcccc);
    base5_200c888 = 0x200c888;
    Engine_ActorEnableActionCallback(28, base5_200c888);
    Call2(FieldScene_CallPairWith10, 30, 0xd000);
    FieldScene_RunScene3af_02000bb8();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(30, base5_200c888);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
    Main_0808a0b0(29, base5_200c888);
    Engine_EventWait(20);
    Event_ClearStatus1c6Far();
    Event_WaitValue1c8FramesFar();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_EventWait(10);
    Main_08009208();
    Main_0808a1d8(21);
    Call3(Main_08015210, 0x1e45, 1, 0);
    Main_08009210();
    Engine_EventRequestExit(14);
}
