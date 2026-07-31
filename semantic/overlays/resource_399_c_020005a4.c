typedef signed int s32;

/*
 * resource_399 scripted step at 0x020005a4, 56 bytes.
 *
 * Complete owner: `push {lr}` prologue at 0x020005a4 and the matching
 * interworking return `pop {r0} / bx r0` at 0x020005ce.  Because the popped
 * register is r0, r0 holds the return address rather than a result: the owner
 * is `void`.  The final word pair 0x020005d4/0x020005d8 is the literal pool
 * (the two message ids), branched past by the return.
 *
 * Call targets were resolved with `bun tools/overlay_call_targets.ts
 * resource_399 05a4`, which reads an overlay `bl` as `target = stored + 2` and
 * follows the import veneer to the main-image address in its trailing word.
 * Six call sites, five distinct imports:
 *   Func_0808a018 / Func_0808a020  scripted-section brackets
 *   Func_080770c0(flag)            story-flag query, result tested
 *   Func_0808a170(line)            message line, two ids, one per arm
 *   Func_0808a180(20, 0)           wait/advance step used with two arguments
 *
 * The two Func_0808a170 sites are genuinely two call sites (one per arm of the
 * flag test), not one shared site: each arm loads its own pool word and the
 * `beq`/`b.n` diamond rejoins only at the following `movs r0,#20`.
 *
 * Uncertainty: the flag id passed to Func_080770c0 is r0=3, a small literal
 * rather than the usual 0x8xx-range story flag, so it may be a query of a
 * different kind (a party or slot selector) that happens to share the import.
 */

/* Imports, named by the main-image address in the trailing word of the
 * overlay veneer each call site reaches.  Old-style declarations: arities vary
 * between sites elsewhere in this overlay. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a170();
void Func_0808a180();
s32 Func_080770c0();

void Func_020005a4(void)
{
    Func_0808a018();

    if (Func_080770c0(3) != 0) {
        Func_0808a170(0x1575);
    } else {
        Func_0808a170(0x155b);
    }

    Func_0808a180(20, 0);
    Func_0808a020();
}
