typedef int s32;

/*
 * resource_380 owner at 0x02002674, 108 bytes: the talk handler for
 * actor 9, one of a per-actor bank of seven published callbacks.
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record, so neither the call-graph
 * sweep nor `tools/overlay_unindexed.ts` lists it. Found by
 * `bun tools/overlay_published.ts resource_380`.
 *
 * IT CORROBORATES resource_380_c_0200227c.c. That owner enables and
 * places exactly six actors -- 5, 9, 11, 10, 14, 13 -- and this
 * contiguous published run hands one callback to each of them:
 *
 *     0x02002674   actor 9    (this file, branching)
 *     0x020026e0   actor 5    (its twin, branching)
 *     0x0200274c   actor 10   (32-byte stub)
 *     0x0200276c   actor 11   (32-byte stub)
 *     0x0200278c   actor 13   (32-byte stub)
 *     0x020027ac   actor 14   (32-byte stub)
 *     0x020027cc   actor 9 again, a different line and a different tail
 *
 * The scene wiring and its interaction handlers were written together;
 * neither half is reachable from the other by a call.
 *
 * TWIN. 0x020026e0 is byte-identical to this owner apart from the actor
 * id (5 rather than 9) and the three dialogue ids (0x10c9/0x107a/0x107c
 * rather than 0x10cb/0x1079/0x107b). The two story-flag ids, 0x83e and
 * 0x83c, are the same in both. Diffed before drafting, per the
 * standing twin rule.
 *
 * Shape: the standard scripted-scene bracket (Func_0808a018 /
 * Func_0808a020) around a two-level story-flag test. If flag 0x83e is
 * set the actor says one line and the handler ends; otherwise flag
 * 0x83c chooses between two alternative lines, and the longer path
 * additionally runs Func_0808a150(actor, 0, 0), waits ten frames, and
 * then closes with the same Func_0808a180(actor, 0).
 *
 * Branch senses read individually rather than pattern-matched: `beq` at
 * 0x02002682 takes the else arm when the first test returns zero, and
 * `bne` at 0x0200269c takes the 0x107b arm when the second returns
 * nonzero. The polarities are opposite, which is exactly the trap this
 * overlay family sets.
 *
 * Complete owner: `push {lr}` at 0x02002674 through `pop {r0} / bx r0`
 * at 0x020026c8-0x020026ca, then the five-word literal pool
 * 0x020026cc-0x020026df (0x83e, 0x10cb, 0x83c, 0x1079, 0x107b); the
 * next owner's prologue -- its twin -- begins at 0x020026e0.
 *
 * All four `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (tools/overlay_call_targets.ts); the naive
 * pc-relative decode is wrong here as everywhere in this overlay.
 *
 * Uncertainty: the roles of Func_0808a150's second and third arguments
 * and of Func_0808a180's second are open; both are zero at every site
 * in this bank.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern s32 Func_080770c0(s32 flagId);

void Func_02002674(void)
{
    Func_0808a018();
    if (Func_080770c0(0x83e)) {
        Func_0808a170(0x10cb);
        Func_0808a180(9, 0);
    } else {
        if (Func_080770c0(0x83c) == 0)
            Func_0808a170(0x1079);
        else
            Func_0808a170(0x107b);
        Func_0808a150(9, 0, 0);
        Func_0808a010(10);
        Func_0808a180(9, 0);
    }
    Func_0808a020();
}
