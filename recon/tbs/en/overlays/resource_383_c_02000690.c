/* NONMATCHING: complete 316-byte owner including switch table and own pool;
 * candidate 280, 124 differing halfwords, 73 aligned edits (2026-09-27).
 * Actor-25 event-table entries reference this callback; message 0x12ad is
 * the refusal response. Restored EventEnd and all three data bindings.
 * Three bounded hypotheses: volatile aggregate reads gave 332/316 and
 * 93 edits; a full actor-tail aggregate gave 340/316 and 90 edits; a signed
 * state pointer and shared update tail move the pool to the end but merge
 * the two forward arms. Signed loads still expand to ldrh plus shifts;
 * the reference keeps separate forward arms and a pool-loaded decrement.
 * The old 288-byte comparison omitted part of this owner's own pool.
 * Bounded H1: exact PROMPT.C's actor24 response and state-match writer
 * distinguish signed script indices from unsigned post-callback updates.
 * Transfer that boundary into two local inline forward operations, with
 * the backwards -1 bound to its observed pool constant. Read the entire
 * 316-byte listing/diff; event tables contain two actor25 references to
 * runtime callback 02008691. Entry state sets actor25's step to 4 and
 * dialogue-pose code consumes the same action tables.
 * H1 restores unsigned arithmetic and the decrement pool word but not
 * the distinct forward arms or signed memory producers: 280/316 bytes,
 * 124 differing halfwords, 73 edits, 107 wrong instructions. The initial
 * and local-allocation RTL retain both inline callback paths; final code
 * merges them. Shared parent next (pseudo 36) is set five times across
 * the cases. A cached HI switch value supplies the later indices, and
 * normalization also loses its required reload. Normal/diagnostic text
 * is identical. No DONE credit; test per-case publication next. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const u8 Data_0200d8bc[];
extern const u8 Data_0200d858[];
extern const u8 *Data_0200e4d8[][4];
extern const u8 Value_ffffffff;

/* Like the actor24 response, choose the script with a signed state, then
 * reload its halfword after the callback before publishing the next step. */
static __inline__ s32 Actor25_AdvanceResponse(s16 *side, s32 half)
{
    Actor_EnableActionCallback(25, Data_0200e4d8[half][*side]);
    return *(u16 *)side - half * 2 + 1;
}

void KuupuappuHeya_RunActor25Response(void)
{
    struct FieldActor *actor;
    s16 *side;
    u32 facing;
    s32 half;
    s32 next;

    actor = Engine_ActorGet(25);
    facing = actor->facing & 0xf000;
    side = (s16 *)&actor->unknown_64;
    half = *side >> 1;
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage(0x12ad);
    Engine_EventShowMessage(25, 0);
    Actor_SetSpeed(25, 0x38000, 0x1c000);
    switch (*side) {
    case 4:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, Data_0200d8bc);
            next = 2;
        } else {
            Engine_ActorEnableActionCallback(25, Data_0200d858);
            next = 3;
        }
        break;
    case 0:
    case 2:
        if (facing > 0x2000 && facing < 0xa000) {
            next = Actor25_AdvanceResponse(side, half);
            break;
        }
        goto back;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            next = Actor25_AdvanceResponse(side, half);
            break;
        }
    back:
        Actor_EnableActionCallback(25, Data_0200e4d8[half ^ 1][*side]);
        /* FAKEMATCH: the backward step owns the ROM's pool-loaded -1. */
        next = *(u16 *)side - half * 2 + (s32)&Value_ffffffff;
        break;
    default:
        goto normalize;
    }
    *(u16 *)side = next;
normalize:
    *(u16 *)side &= 3;
    Engine_ActorStartAction(25);
    Engine_EventEnd();
}
