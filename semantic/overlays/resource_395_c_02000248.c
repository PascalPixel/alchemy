typedef signed int s32;

/*
 * Resource 395 scripted scene at 0x02000248 (60 bytes, 0x02000248-0x02000283).
 *
 * Complete owner: `push {lr}` prologue at 0x02000248 and the matching
 * `pop {r0} / bx r0` interworking return at 0x02000272.  The popped return
 * address lands in r0, so nothing is returned and the owner is `void`.
 * 0x02000276-0x02000283 is the alignment halfword plus this owner's three-word
 * literal pool (0x845, 0x151f, 0x14c8); it sits after the epilogue and is never
 * reached as code.
 *
 * Twin of 0x02000284 in the same overlay.  The two bodies are identical over
 * all 60 bytes except for three constants: the dialogue ids 0x151f/0x151e and
 * 0x14c8/0x14ec, and the slot 9/8 passed to Func_0808a180.  The story flag
 * (0x845) and the four callees are shared, which is the cheapest available
 * correctness proof for both files.
 *
 * All six call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); overlay_show.ts's own `bl`
 * annotations are off by one veneer entry here and were not used.  Placed
 * multiset, matching the six sites exactly: Func_0808a170 x2 and one each of
 * Func_0808a018, Func_080770c0, Func_0808a180, Func_0808a020.  The inventory
 * reports calls=6.
 *
 * Uncertainty: the two Func_0808a170 arguments are dialogue/cue ids in this
 * overlay's usual band, but which of the pair is the "flag set" line is not
 * established beyond the branch polarity (`beq` takes the 0x14c8 arm when the
 * flag reads zero).
 */

void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */

void Func_02000248(void)
{
    Func_0808a018();

    if (Func_080770c0(0x845) != 0) {
        Func_0808a170(0x151f);
    } else {
        Func_0808a170(0x14c8);
    }

    Func_0808a180(9, 0);
    Func_0808a020();
}
