typedef signed int s32;

/*
 * resource_399 scripted step at 0x020002b8, 204 bytes.
 *
 * Complete owner: `push {lr}` at 0x020002b8 and the interworking return
 * `pop {r0} / bx r0` at 0x02000372.  The popped register is r0, so it holds
 * the return address and nothing is returned — the owner is `void`.  Bytes
 * 0x02000376-0x02000383 are alignment plus the three-word literal pool.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 02b8 --annotate`; an overlay `bl` stores `target - 2`, so the disassembler's
 * own annotations are wrong.  23 sites, 11 distinct imports.  Per-target
 * multiset against the assembly, which is what the C below spells:
 *   Func_0808a018 x1, Func_080770c0 x1, Func_0808a170 x2, Func_0808a180 x3,
 *   Func_0808a1f0 x2, Func_0808a010 x4, Func_0808a100 x4, Func_0808a148 x2,
 *   Func_0808a190 x1, Func_0808a1b8 x2, Func_0808a020 x1.
 * (The inventory's calls=20 counts distinct-ish targets and predates the
 * corrected `bl` decoding; it is a lower bound, not a check.)
 *
 * Shape: one scripted section bracketed by Func_0808a018/Func_0808a020, with
 * the story-flag query Func_080770c0(0x881) choosing between two arms.  The
 * two arms are the SAME sequence on channel 10 and differ in exactly two
 * places — the message id (0x163c against 0x152d) and one step call, where the
 * flag-set arm uses Func_0808a190(10, 0) and the flag-clear arm uses
 * Func_0808a180(10, 0).  They are written out separately rather than merged so
 * the call multiset stays exact; the near-identity is itself the check that
 * neither arm was mis-transcribed.
 *
 * Constants built by shifts: `movs r1,#129 ; lsls #1` is 258 (a pose id), and
 * `movs r1,#192 ; lsls #6` is 0x3000 — three sixteenths of a turn in the
 * 16-bit angle unit Func_0808a1b8 takes.
 *
 * Uncertainty: Func_0808a190's interface is not established elsewhere in the
 * semantic tree; only that it is reached here with two arguments, in the
 * position its sibling arm uses Func_0808a180.
 */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
void Func_0808a010();           /* frame wait */
void Func_0808a018();           /* scripted-section open */
void Func_0808a020();           /* scripted-section close */
void Func_0808a100();
void Func_0808a148();           /* scene request */
void Func_0808a170();           /* message line */
void Func_0808a180();
void Func_0808a190();
void Func_0808a1b8();           /* orientation over frames */
void Func_0808a1f0();
s32 Func_080770c0();            /* story-flag query */

void Func_020002b8(void)
{
    Func_0808a018();

    if (Func_080770c0(0x881) != 0) {
        Func_0808a170(0x163c);
        Func_0808a180(10, 0);
        Func_0808a1f0(10, 258);
        Func_0808a010(40);
        Func_0808a100(10, 1);
        Func_0808a010(20);
        Func_0808a148(10, 0, 20);
        Func_0808a190(10, 0);
        Func_0808a1b8(10, 0x3000, 10);
        Func_0808a100(10, 9);
    } else {
        Func_0808a170(0x152d);
        Func_0808a180(10, 0);
        Func_0808a1f0(10, 258);
        Func_0808a010(40);
        Func_0808a100(10, 1);
        Func_0808a010(20);
        Func_0808a148(10, 0, 20);
        Func_0808a180(10, 0);
        Func_0808a1b8(10, 0x3000, 10);
        Func_0808a100(10, 9);
    }

    Func_0808a020();
}
