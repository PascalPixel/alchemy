/*
 * Draft of resource_3b1 0x0200812c (FuneHeya_RunWalkerStep), from games/THE
 * BROKEN SEAL/SRC/FIELD/FUNE_HEYA; the range links as disassembly (section
 * .text.x0200812c of the overlay listing).
 *
 * Remaining: in the shared advance tail the reference loads the zero it
 * stores into rise_counter from the literal pool after the step store
 * (strh, then ldrb from the pool). Written as a plain zero below, GCC also
 * takes it from the pool but schedules that load above the step store, a
 * swap of two halfwords; u8, u16 and s32 spellings, a function-scope zero,
 * step = step + 1 and storing the zero first all keep or worsen the swap.
 * The unit matched only while the zero was the address of a link-time
 * symbol at 0.
 *
 * 2026-10-02 (slice 13), from the pass dumps: the pool zero is the 16-bit
 * all-zero mask of the step store (every field store expands through the
 * bit-field path; CSE then stores that mask register into rise_counter).
 * Here it is created before the step store, in the same block, and the
 * scheduler keeps that order on a priority tie. The game's order needs the
 * zero's instruction after the step store in the chain: then it ties with
 * the address copy and wins because the copy is anti-dependent on the
 * store. local-alloc moves a constant's set down to its use only when the
 * register is set in one basic block and used once in another, so the
 * game's 16-bit zero was set in a different block from the byte store.
 * Three written-out copies of the tail, a label between its two statements
 * (plain zero, movs) and a step pointer (movs) do not give that.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 StagedActor_CountdownUntilPositionUnset(u8 *object);
void StagedActor_AdvanceCounter98(u8 *object);

struct Walker {
    u8 unknown_00[0x4c];
    s32 countdown;
    u8 unknown_50[0x16];
    s16 step;
};

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Ship cabin walker: advance the actor's scripted walk one step, turning, walking and waiting on the leader between steps. */
void FuneHeya_RunWalkerStep(struct FieldActor *obj)
{
    struct FieldActor *leader;
    struct Walker *walker;

    walker = (struct Walker *)obj;
    leader = Object_GetById(8);
    switch (walker->step) {
    case 0:
        obj->facing = 0xb000;
        goto advance;
    case 2:
        obj->facing = 0;
        goto advance;
    case 4:
        Object_SetMode(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1d40000, 0x200000, 0x2780000);
        walker->countdown = 60;
        walker->step++;
        break;
    case 5:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Object_SetMode(obj, 1);
            obj->rise_counter = 0;
            if (leader->unknown_5b == 0) {
                obj->rise_enabled = 1;
            }
            walker->step++;
        }
        break;
    case 7:
        if (leader->unknown_5b == 0) {
            Object_SetMode(obj, 3);
            obj->rise_enabled = 2;
        }
    advance:
        walker->step++;
        *(u8 *)&obj->rise_counter = (u16)0;
        break;
    case 9:
        Object_SetMode(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1e00000, 0x200000, 0x2580000);
        walker->countdown = 60;
        walker->step++;
        if (leader->unknown_5b == 0) {
            obj->rise_enabled = 3;
        }
        break;
    case 10:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Object_SetMode(obj, 1);
            obj->rise_counter = 0;
            walker->step++;
        }
        break;
    case 1:
    case 3:
    case 6:
    case 8:
    case 11:
        StagedActor_AdvanceCounter98((u8 *)obj);
        break;
    case 12:
        walker->step = 0;
        break;
    }
}
