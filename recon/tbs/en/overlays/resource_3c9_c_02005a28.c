/* NONMATCHING: 360 bytes, candidate 358, 55 differing halfwords / 55 edits.
 * VinasuChojo_SpawnRisingSparks, meant for FIELD/VINASU_CHOJO/RISING_SPARKS.C
 * as a single-overlay unit binding Engine_* and Main_* at their import veneers
 * (runtime = listing offset + 0x8000). Remaining: the null test on the source
 * actor reloads it through r3 where the reference uses r1 and reuses that copy
 * for the x load; the RandomNext mask and the pooled zero (Value_00000000,
 * held in r9 for the sprite flags) schedule after the unknown_64 store instead
 * of before it, which moves the mid-function literal pool from after the
 * priority mask to after the MathSin call. Cross-jumped scale tails and the
 * scroll word >> 16 read already match.
 * 2026-09-27: complete extent 02005a28..02005b90, with pools at 5aa0 and
 * 5b5c. Exact 5b90 consumes +64 as orbit angle, +68 as source actor and
 * +30 as radius adjustment. The old Scene_RunScene3c9 name resolves to
 * four owners; bind the audited callback explicitly at runtime 0200db90.
 * Fresh stable baseline is 358/360, 58 differing halfwords / 58 edits.
 * H1 signed-mask trial is byte-identical (cmp): both source 0x0ffff000 and
 * -0x1000 compile to pool -4096. Direct own-ROM pool read confirms the
 * reference is 0x0ffff000, not -4096; the initial decoded-pool inference
 * was wrong. Halfword-store simplification removes the upper mask bits.
 * H2 transfers the proven Value_0ffff000 word-valued link constant from
 * this overlay's 36d0 draft: 358/360, 55 differing halfwords / 55 edits.
 * It restores the literal word and r3 mask / r2 store pointer, but does not
 * move the pooled zero before the angle store. The pool remains after
 * MathSin, and source null-test/coordinate copies still use the wrong
 * lifetimes. Full normalized diff reviewed. Stop after this follow-up;
 * no declaration, pointer-spelling or register-only sweep was attempted.
 * No exact source or shared header edits; no DONE credit.
 *
 * 2026-09-27 H3 Title/Lamp boundary transfer: Title ResetCounter expands
 * the destination before an aggregate HI zero; exact MAKYURI_CHOJO/LAMP.C
 * shares a literal Half zero with its angle member-store producer. A local
 * InitializeAngle(u16 *, u32, Half *) combines those boundaries. Full score
 * is 358/360, 53 halfwords / 37 aligned edits, with the same no-frame setup.
 * Prediction failed: the zero still loads after the angle store; second
 * pool moved from relative 118 to 140, not reference 134. The complete
 * normalized diff retains the source-null-copy and factory setup residuals.
 * -da/-fsched-verbose=5 assembly is byte-identical to ordinary compilation.
 * RTL explains the failure: the pointer store is direct mem:HI, removing
 * the member-store's generated HI-zero producer, so aggregate zero 90 is
 * independent and scheduled just before MathSin. This is a counterexample,
 * not an admitted canonical or credit. One causal follow-up may restore
 * the actor member-store boundary without changing types or call ABI.
 *
 * H4 restores FieldActor *destination and destination->unknown_64. Full
 * result 358/360, 56 halfwords / 50 edits: zero and pool remain late (140).
 * Diagnostics equal ordinary assembly. CSE substitutes the already-zero
 * frame phase 45 into Half 90 before the angle member-store; the stored
 * angle does not consume that producer. Local allocation restores a HI
 * constant and sched2 places it before MathSin, not before the angle store.
 * Admission failed again. LAMP's exact zero publication is after the angle
 * store, which permits sharing that store's generated HI zero instead of
 * the branch-known SI phase. One final producer-order test is supported by
 * this ancestry difference; no other variant or null-copy sweep is admitted.
 *
 * H5 retains H4's actor boundary but publishes zero after the angle store,
 * matching LAMP's actual producer order. Result 360/360, 60 halfwords /
 * 39 aligned edits. Both pools and the full normalized diff were checked:
 * the first pool is exact, second starts at relative 138 rather than 134,
 * and zero still loads after strh. CSE still forwards phase 45 into the
 * address-taken Half 90; the hoped-for angle HI producer is not retained.
 * Diagnostic assembly equals ordinary assembly. Equal size is not a match:
 * source-null copy, factory arguments, zero lifetime and pool remain wrong.
 * Three bounded models exhausted; preserve each in Git, then restore the
 * simpler 358/55/55 baseline. No new source-interface fact admits another
 * trial. H3 is 36c683a70, H4 d71c79f84, H5 41f33c962. The canonical body
 * below is restored and byte-compared with the initial 358/55/55 baseline.
 * No DONE or alignment credit; exact consumers remain untouched. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct FieldView {
    u8 unknown_000[0xe8];
    s32 scroll_y;
};

extern struct FieldView *Data_03001e70;
extern u8 Data_0200e734[];
extern u8 Value_00000000;
extern u8 Value_0ffff000;

void VinasuChojo_UpdateOrbitingSpark(union FieldObject *object);

void VinasuChojo_SpawnRisingSparks(void)
{
    struct FieldActor *source = Engine_ActorGet(23);
    struct FieldView *view = Data_03001e70;
    s32 offset = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    struct FieldActor *spark;
    u8 zero;
    struct FieldSprite *sprite;
    u32 phase;

    if (view->scroll_y >> 16 <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(23, 0x1300000, 0xa40000);
            Engine_ActorGet(23)->scale_x = 0x10000;
            Engine_ActorGet(23)->scale_y = 0x10000;
        } else {
            Engine_ActorSetPosition(23, 0x1300000, 0xab0000);
            Engine_ActorGet(23)->scale_x = 0x14ccc;
            Engine_ActorGet(23)->scale_y = 0x14ccc;
        }
    } else {
        Engine_ActorSetPosition(23, 0, 0);
    }
    if (source != 0) {
        phase = gFrameCount & 15;
        if (phase == 0) {
            spark = Engine_ObjectCreate(284, source->x.fixed + 0x80000, source->y.fixed + offset + 0x80000, source->z.fixed);
            offset = Engine_MathDivide(offset, 0x60000);
            offset <<= 16;
            if (spark != 0) {
                sprite = spark->sprite;
                Engine_ObjectSetScript(spark, Data_0200e734);
                Engine_ObjectSetPalette(spark, 5);
                spark->motion_flags = phase;
                spark->unknown_64 = Engine_RandomNext() & (u32)&Value_0ffff000;
                zero = (u8)(u32)&Value_00000000;
                spark->unknown_66 = phase;
                *(struct FieldActor **)spark->unknown_68 = source;
                spark->update = VinasuChojo_UpdateOrbitingSpark;
                spark->speed = (Engine_MathSin((offset & 0xfffff) >> 4) * 24) >> 16;
                sprite->flags = zero;
                sprite->priority = source->sprite->priority;
            }
        }
    }
}
