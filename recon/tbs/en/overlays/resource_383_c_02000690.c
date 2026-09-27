/* NONMATCHING: complete 316-byte owner including switch table and own pool;
 * candidate 284, 122 differing halfwords, 76 aligned edits (2026-09-27).
 * Read-only alias-view audit (2026-09-27): exact PROMPT.C actor24 response
 * uses one s16 pointer at actor+100 for indices and u16 pointer casts for
 * updates, not a byte/halfword union. Opening-sequence and state-match
 * writers likewise publish whole halfwords; FieldActor declares unknown_64
 * as u16. These sources do not establish a second record view that could
 * invalidate the cached HI or normalization forwarding. No union experiment
 * run: it would lack the requested new caller/state ownership evidence.
 * Required independent signed reads and post-store reload remain unresolved.
 * Completion H3: a one-pass signed read operation for switch and script
 * consumers does not admit the required independent ldrsh producers.
 * Initial read boundaries survive as notes, not memory invalidations: the
 * HI value remains shared, both forward arms still merge, and normalization
 * still forwards the stored value instead of reloading. The 76->74 aligned
 * change is only table-base/arithmetic scheduling; no owner bytes earned.
 * Full normalized diff read; normal and diagnostic text agree. Reject this
 * read-boundary model (dff213f78); H2 restored byte-for-byte. Do not tune
 * the non-admitted scheduling improvement or retry scalar read wrappers.
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
 * is identical. H1 is preserved in 0ce80a3c5 with no DONE credit.
 * H2: publish the forward update inside each callback operation instead
 * of returning through parent next, as the actor24 sibling does before
 * its clamp. The local add and store now use r3, but an extra copy feeds
 * normalization and the two forward arms STILL merge. The shared next
 * variable is therefore not the cause of their collapse. Result:
 * 284/316 bytes, 122 differing halfwords, 76 edits, 105 wrong insns.
 * Keep the explicit signed-index/unsigned-update ownership as this draft;
 * the lower 73-edit return-value model remains in H1's commit. Neither
 * model recovers the signed memory producers or normalization reload.
 * Full normalized diff read; normal/diagnostic text identical. Stop after
 * the one model and causal follow-up, without volatile/full-tail aggregate
 * retries. A new trial needs a supported state-read producer that avoids
 * both cached-HI sharing and the late forward-arm merge. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const u8 Data_0200d8bc[];
extern const u8 Data_0200d858[];
extern const u8 *Data_0200e4d8[][4];
extern const u8 Value_ffffffff;

/* Like the actor24 response, choose the script with a signed state, then
 * reload its halfword after the callback before publishing the next step. */
static __inline__ void Actor25_AdvanceResponse(s16 *side, s32 half)
{
    Actor_EnableActionCallback(25, Data_0200e4d8[half][*side]);
    *(u16 *)side = *(u16 *)side - half * 2 + 1;
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
            Actor25_AdvanceResponse(side, half);
            goto normalize;
        }
        goto back;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            Actor25_AdvanceResponse(side, half);
            goto normalize;
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
