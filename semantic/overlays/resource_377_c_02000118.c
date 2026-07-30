typedef signed int s32;

/*
 * resource_377 owner at 0x02000118, 72 bytes.  Complete owner: 'push {r5, r6, lr}'
 * prologue and the interworking return 'pop {r5, r6} ; pop {r0} ; bx r0'.  r0
 * holds the popped return address, so the owner returns nothing.
 *
 * A short scripted beat: select a cue, attach it to the incoming object, then
 * branch on a runtime query to pick one of two follow-up cues, and close.
 *
 * Literal pool: one word, 0x000022b9, at owner offset 0x44 (the 'movs r0,r0'
 * at 0x0200015a is alignment, and the epilogue never falls into either).  It is
 * a plain cue id, not an address; the two follow-up cues are that id + 1 and
 * + 2, which is why r5 is kept live across all three calls.
 *
 * All seven calls were resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 0118 --json' (7 sites,
 * 5 distinct veneer targets):
 *   0x0120 -> 0x19c4 -> Func_0808a170(id)
 *   0x0128 -> 0x19cc -> Func_0808a178(object, 0)
 *   0x0130 -> 0x1924 -> Func_0808a070(0, 0)      result tested
 *   0x013a -> 0x1904 -> Func_0808a010(10)
 *   0x0140 -> 0x19c4 -> Func_0808a170(id + 1)
 *   0x0148 -> 0x19c4 -> Func_0808a170(id + 2)
 *   0x0150 -> 0x19d4 -> Func_0808a180(object, 0)
 * Multiset check: Func_0808a170 appears three times in the assembly and three
 * times below.  The disassembler's own 'bl' annotations are wrong, as on every
 * overlay, and were not used.
 */

void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a180();

void Func_02000118(void *object)
{
    s32 cue = 0x22B9;

    Func_0808a170(cue);
    Func_0808a178(object, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(cue + 1);
    } else {
        Func_0808a170(cue + 2);
    }

    Func_0808a180(object, 0);
}
