typedef signed int s32;

/*
 * Resource 395 scripted scene at 0x02000284 (60 bytes, 0x02000284-0x020002bf).
 *
 * Complete owner: `push {lr}` prologue at 0x02000284 and the matching
 * `pop {r0} / bx r0` interworking return at 0x020002ae.  The popped return
 * address lands in r0, so nothing is returned and the owner is `void`.
 * 0x020002b2-0x020002bf is the alignment halfword plus this owner's three-word
 * literal pool (0x845, 0x151e, 0x14ec), after the epilogue and never reached as
 * code.
 *
 * Twin of 0x02000248: the two bodies are byte-identical over all 60 bytes
 * except the dialogue ids (0x151e/0x151f, 0x14ec/0x14c8) and the slot passed to
 * Func_0808a180 (8/9).  Both test the same story flag 0x845 and call the same
 * four imports in the same order.
 *
 * All six call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), not from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the six sites exactly: Func_0808a170 x2 and one each of
 * Func_0808a018, Func_080770c0, Func_0808a180, Func_0808a020.  The inventory
 * reports calls=6.
 *
 * Uncertainty: as in the twin, the pairing of the two dialogue ids with the
 * flag states rests only on the branch polarity (`beq` takes the 0x14ec arm
 * when the flag reads zero).
 */

void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */

void Func_02000284(void)
{
    Func_0808a018();

    if (Func_080770c0(0x845) != 0) {
        Func_0808a170(0x151e);
    } else {
        Func_0808a170(0x14ec);
    }

    Func_0808a180(8, 0);
    Func_0808a020();
}
