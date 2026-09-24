/* NONMATCHING: 388 bytes, candidate 388, 15 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * reload register choice only: the leader copy for stores to +104 takes r1 in
 * the reference for actors 5, 9 and 10 (r3 here), and the script and zero
 * high-register setup reloads through r3 then r1; the reference seems to have
 * r1 among its spill registers. Sweeps and zero placements did not help. */
#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorEnableActionCallback();
void Engine_ActorSetPosition();
void Engine_ObjectSetScript();
u8 * Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Local_0200227c(void)
{
    u32 i;
    s32 p10;
    s32 rec4;
    u8 *rec7;
    u8 *record;
    s32 none;

    rec4 = Value1(Engine_ActorGet, 0);
    Engine_EventBegin();
    Engine_ActorEnableActionCallback(5, 1);
    Engine_ActorEnableActionCallback(9, 1);
    Engine_ActorEnableActionCallback(11, 1);
    Engine_ActorEnableActionCallback(10, 1);
    Engine_ActorEnableActionCallback(14, 1);
    Engine_ActorEnableActionCallback(13, 1);
    Call3(Engine_ActorSetPosition, 5, 0x1db0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 9, 0x1eb0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 11, 0x1cb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 10, 0x1fb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1680000);
    Call3(Engine_ActorSetPosition, 13, 0x1d70000, 0x1320000);
    record = Engine_ActorGet(5);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    none = 0;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Engine_ActorGet(9);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Engine_ActorGet(11);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Engine_ActorGet(10);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    rec7 = Engine_ActorGet(14);
    *(s32 *)((s32)rec7 + 104) = rec4;
    rec7[90] |= 1;
    *(s32 *)((s32)rec7 + 24) = 0x10000;
    *(s32 *)((s32)rec7 + 28) = 0x10000;
    rec7[85] = *(u8 *)(Engine_ActorGet(11) + 85);
    *(s32 *)((s32)rec7 + 12) = none;
    Engine_ObjectSetScript((s32)rec7, 0x200cbd0);
    record = Engine_ActorGet(13);
    *(s32 *)((s32)record + 104) = rec4;
    {
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    Call0((void (*)())Engine_EventEnd);
    p10 = (1 | record[90]);
}
