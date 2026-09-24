#include "TYPES.H"

void Engine_EventBegin();
void Main_0808a030();
void Engine_TaskWait();
void Engine_ActorDestroy();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void FieldScene_RunEncounterClosingSequence();
void Engine_EventEnd();


extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* NONMATCHING: 216 of 216 bytes, 2 halfword edits (2026-09-24). The
 * game-state base load for the +450 test comes after the 225 index here;
 * the reference loads the base first (sched1 order). */
void Func_020019c0(s32 a0, s32 a1)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v3;

    *(s32 *)(*(s32 *)0x03001e70 + 236) = 0x410000;
    Engine_EventBegin();
    Call1(Main_0808a030, 0x200d418);
    Engine_TaskWait(1);
    Engine_ActorDestroy(24);
    Call3(Engine_ActorSetPosition, 23, 0xee0000, 0x2720000);
    v5 = 192;
    record = Engine_ActorGet(23);
    *(u16 *)(record + 6) = (v5 << 6);
    if (Value1(Engine_GameFlagIsSet, 0x903) != 0) {
        Call3(Engine_ActorSetPosition, 22, 0xa20000, 0x27a0000);
        record = Engine_ActorGet(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa20000, 0x2a40000);
        record = Engine_ActorGet(21);
        v3 = 208;
    } else {
        Call3(Engine_ActorSetPosition, 22, 0xa00000, 0x28c0000);
        record = Engine_ActorGet(22);
        *(u16 *)(record + 6) = (v5 << 6);
        Call3(Engine_ActorSetPosition, 21, 0xa60000, 0x29c0000);
        record = Engine_ActorGet(21);
        v3 = 176;
    }
    *(u16 *)(record + 6) = (v3 << 8);
    if (Data_02000240_t[225][0] == 6) {
        FieldScene_RunEncounterClosingSequence();
    }
    Engine_EventEnd();
}
