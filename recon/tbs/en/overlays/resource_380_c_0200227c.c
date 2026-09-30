/* 2026-09-27 shared inline actor-link boundary for actor 9/11/10:
 * complete output is byte-identical to the canonical 388-byte draft,
 * including the 15 differing halfwords / 14 aligned edits. The leader
 * reloads and script/zero order do not move. Restore the direct blocks;
 * helper ownership is not the missing source boundary. DONE +0.
 * 2026-09-27 halfword-zero transfer from exact TORETO_HEYA/MAP_PATCH.C:
 * a one-halfword record for the actor-14 zero emits 376/388 bytes,
 * 189 differing halfwords / 53 aligned edits. The missing saved high
 * register changes the prologue and every later pool reach. Reject this
 * storage model; restore the 388-byte, 15-halfword canonical draft.
 * NONMATCHING: complete 388-byte owner; restored best baseline 388 bytes,
 * 15 differing halfwords / 14 aligned edits (2026-09-27).
 * Remaining: actor 5/9/10 leader stores reload through r3 instead of r1;
 * script/zero high-register setup order; actor 14 motion/zero/call ordering.
 * Exact PARTY_INTRO.C consumer proves linked_object at +0x68 and flag +0x5a.
 * Typed union model with FIELD_EVENT.H interfaces and a named script:
 * H1 400 bytes / 121 halfwords / 53 edits, preserved at 4fffb36b0.
 * H2 one-pass script/zero initialization restored script r8 / mask r6:
 * 388 bytes / 16 halfwords / 15 edits, preserved at ec461ea87.
 * H2 still differed in the leader reloads, script/zero and actor 14 ordering,
 * plus actor 13 OR destination. The stronger original baseline stays active.
 * STOP: typed-layout/lifetime axis bounded; no new DONE.
 * 2026-09-27 Sol spark-ring: fresh full score reconfirms 388 bytes / 15
 * halfwords / 14 edits. Allocator reports a reciprocal lifetime constraint,
 * but its named reciprocal repair requires a scalar XOR site and refuses
 * this draft's volatile byte read; catalogue inapplicable, no variants run.
 * FAKEMATCH: legacy call wrappers, volatile flag read and dead final read
 * retain the original baseline's evaluation and register lifetime choices. */
#include "TYPES.H"

s32 Object_GetById();
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

    rec4 = Value1(Object_GetById, 0);
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
    record = Object_GetById(5);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    none = 0;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Object_GetById(9);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Object_GetById(11);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    record = Object_GetById(10);
    *(s32 *)((s32)record + 104) = rec4;
    record[90] |= 1;
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    rec7 = Object_GetById(14);
    *(s32 *)((s32)rec7 + 104) = rec4;
    rec7[90] |= 1;
    *(s32 *)((s32)rec7 + 24) = 0x10000;
    *(s32 *)((s32)rec7 + 28) = 0x10000;
    rec7[85] = *(u8 *)(Object_GetById(11) + 85);
    *(s32 *)((s32)rec7 + 12) = none;
    Engine_ObjectSetScript((s32)rec7, 0x200cbd0);
    record = Object_GetById(13);
    *(s32 *)((s32)record + 104) = rec4;
    {
        u8 value = *(volatile u8 *)&record[90];
    
        record[90] = (u8)(value | 1);
    }
    Engine_ObjectSetScript((s32)record, 0x200cbd0);
    Call0((void (*)())Engine_EventEnd);
    p10 = (1 | record[90]);
}
