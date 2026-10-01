/* NONMATCHING: inversion I2 408/404 bytes, 104 differing halfwords / 41 edits.
 * 2026-09-27 Sol lane audit: fresh full and aligned normalized diffs agree
 * with I2. Complete [02000d2c,02000ec0), all four pools, frame 4 and every
 * phase exit/back-edge checked. The extra sine half-turn rematerialization
 * and absent accumulator copy remain, together with actor/limit r8/sl roles.
 * DRIFT's flag-store issue and Haidia's nullable-lookup lifetime have no
 * matching source boundary here; Actor_Get already returns FieldActor *.
 * Do not reopen the previously rejected early rotation snapshot or scope
 * sweeps. This owner remains C not yet written; no alignment changes.
 * Canonical I2 body restored after rejected O1 witness in 1cf42a2d3.
 * Outward O1 (2026-09-27): snapshot the u16 sprite rotation at rise-loop
 * entry before advancing acc, then add its whole part to that snapshot.
 * Prediction: rotation gets r2 before acc's add, whole-part extraction r3.
 * Admission: all three phase entries/back-edges, frame 4 and four pools.
 * New diagnostic fact: I2 local allocation gives whole-part p56 r2 and
 * loaded rotation p57 r3; sched2 cannot issue that load until the increment
 * in r3 has been consumed by add r7,r7,r3. This is a register dependence,
 * not merely the scheduler choosing a different order among ready insns.
 * O1's early load (insn 120) survives through life, but combine deletes
 * 120/121/122 and folds the memory value into later zero-extension 137,
 * after acc's add 129 and shift 135. Final bytes are identical to I2.
 * Full normalized diff and byte comparison confirm every admission is
 * preserved, but the predicted local shape is not. Diagnostic -da output
 * equals ordinary assembly. Reject O1; one-trial budget exhausted.
 * Actor/limit are not a declaration-order tie: both have 12 weighted uses,
 * with life lengths 95/122; global allocation ranks actor first. No source
 * boundary explaining a reversal was established, so that axis was not tried.
 * Missing evidence: a legitimate operand/value boundary that survives
 * combine and keeps the rotation load independent of the increment register.
 * No new DONE bytes. I2 remains canonical; O1 survives in Git, not here.
 *
 * Inversion I2 408/404 bytes, 104 differing halfwords / 41 edits.
 * Retained canonical model (2026-09-27): structured loops with named phase
 * exits, not break. All three ROM control-flow admission checks now pass:
 * fall through into the update body, conditionally exit forward, then wait
 * (and reload rotation in fall/settle) before an unconditional back-edge.
 * The reference exit sites are 0db6/0dee/0e4c; back-edges are 0dbe/0df8/0e56.
 * Full normalized diff: topology equal, frame 4, four pool words retained.
 * Baseline was 408/404, 152 differing halfwords / 69 edits, topology different.
 * I2 keeps LOOP_BEG/CONT/END annotations while goto's named label is not
 * expand_end_loop's recognized loop-end label. loop.c reports no final
 * conditional branch; flow2 shows each preheader falling into its update
 * block, with the wait block returning unconditionally. Diagnostic -da
 * assembly equals ordinary compilation. No code-generation flags changed.
 * Residual: actor/limit r8/sl roles, first rotation-load/add ordering,
 * settle's half-turn setup and rematerialization instead of add r0,fp,
 * its missing accumulator copy, plus resulting displacements/alignment.
 * Both structural trials are complete. Preserve this admitted topology;
 * do not resume angle/ownership/declaration sweeps. No new DONE bytes.
 *
 * Inversion I1 404/404 bytes, 125 differing halfwords / 63 edits.
 * Complete normalized diff inspected (2026-09-27). Explicit phase labels
 * restore direct update-body entry, forward exit branches, and wait/reload
 * followed by an unconditional back-edge in all three phases. Frame grows
 * from 4 to 8; fall's 0x80000 increment is carried across its cosine call
 * and reused to initialize settle. The fourth pool word becomes an immediate.
 * This phase-local labeled candidate is binary-identical to old H1 at
 * 0f0536a63, despite retaining H2's separate accumulator scopes. It supplies
 * no new matching bytes and is a diagnostic witness, not an adoption.
 * Baseline -da output is assembly-identical to ordinary compilation. Its
 * initial RTL already contains the rotated loop: stmt.c expand_end_loop
 * recognizes break's loop-end target and moves update/test behind wait;
 * loop and flow dumps inherit that shape. The admission invariant is direct
 * body entry and a forward exit before wait/reload/unconditional back-edge,
 * without changing wait counts, angle arithmetic or state ownership.
 * H2 baseline 408/404 bytes, 152 differing halfwords / 69 aligned edits.
 * Phase-local loops remove the increment spill: frame 4 now matches, as do
 * all four pool words. The compiler rotates each loop, keeps actor in r8
 * instead of sl, and rematerialises the sine half-turn instead of using fp.
 * Retained for the corrected frame; H1 in 0f0536a63 has 63 aligned edits.
 * STOP: complete model plus two structural follow-ups exhausted; no adoption.
 * H1 404/404 bytes, 125 differing halfwords / 63 aligned edits.
 * The unsigned whole-angle halfword view restores both shifts inside the
 * settle loop and x in r9; increment sharing still adds one stack word.
 * H1 does not match: the fourth pool word and several register lifetimes differ.
 * H0 408/404 bytes, 141 differing halfwords / 89 aligned edits (2026-09-26).
 * Complete own-ROM extent 02000d2c..02000ec0, including four pool words.
 * FIELD_EVENT.H and exact FieldScene_RunSharedSetPiece establish actor y at
 * +0x0c and sprite rotation at +0x1e. The old draft called y "z" and declared
 * ActorGet as an integer-returning unprototyped service.
 * Baseline 408/404 bytes, 141 halfword edits (2026-09-24).
 * H0 keeps an 8-byte frame instead of 4: CSE carries the fall increment
 * into settle, and GCSE carries its whole-angle value around the back edge.
 * Calls all resolve correctly; typed views alone do not change this shape.
 * Budget: complete typed model and at most two structural follow-ups. */
#include "FIELD_EVENT.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}

static inline s32 Math_Sin(s32 angle)
{
    return Engine_MathSin(angle);
}

static inline s32 Math_Cos(s32 angle)
{
    return Engine_MathCos(angle);
}


void FieldScene_RunSharedSetPiece(s32 delay);

union SwingAngle {
    u32 fixed;
    struct {
        u16 fraction;
        u16 whole;
    } part;
};

void ArutinYama_SwingActorIntoSetPiece(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 x;
    s32 y;
    u32 angle;
    u32 limit;
    u32 half;
    s32 c;
    s32 s;

    actor = Actor_Get(10);
    sprite = actor->sprite;
    x = actor->x.fixed;
    y = actor->y.fixed;
    Event_Begin();
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(20);
    /* FAKEMATCH: named phase exits preserve direct loop entry while keeping
     * the structured-loop annotations needed for the four-byte frame. */
    {
        union SwingAngle acc;

        acc.fixed = 0;
        limit = 0x8fff;
        for (;;) {
            acc.fixed += 0x80000;
            sprite->rotation += acc.part.whole;
            c = Math_Cos(sprite->rotation + 0x4000);
            actor->x.fixed = (c << 4) + x;
            angle = sprite->rotation;
            if (angle > limit)
                goto risen;
            Task_Wait(1);
        }
risen:;
    }
    {
        union SwingAngle acc;

        acc.fixed = 0;
        limit = 0x7000;
        for (;;) {
            acc.fixed += 0x80000;
            sprite->rotation = angle - acc.part.whole;
            c = Math_Cos(sprite->rotation + 0x4000);
            actor->x.fixed = (c << 4) + x;
            angle = sprite->rotation;
            if (angle <= limit)
                goto fallen;
            Task_Wait(1);
            angle = sprite->rotation;
        }
fallen:;
    }
    {
        union SwingAngle acc;

        half = 0x8000;
        acc.fixed = 0x80000;
        for (;;) {
            acc.fixed = (acc.part.whole + (acc.fixed >> 19)) << 16;
            limit = acc.part.whole;
            sprite->rotation = limit + angle;
            c = Math_Cos(sprite->rotation + 0x4000);
            s = Math_Sin(sprite->rotation + half);
            actor->x.fixed = (c << 4) + x;
            if (sprite->rotation > half)
                actor->y.fixed = y - (s << 3);
            if ((s32)(sprite->rotation + limit) > 0xbfff)
                goto settled;
            Task_Wait(1);
            angle = sprite->rotation;
        }
settled:;
    }
    Task_Wait(1);
    {
        s32 shown = 0xc000;

        sprite->rotation = shown;
    }
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FieldScene_RunSharedSetPiece(5);
    Event_End();
}
