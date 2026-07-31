typedef signed int s32;

/*
 * resource_3c1 owner at 0x02000068, 72 bytes: a two-line examine handler.  Show
 * the opening line, face the subject, then show one of two follow-up lines
 * depending on the beat guard, and release the subject.
 *
 * Reference member of a two-owner pair: 0x02000068 and 0x020000b0 are
 * byte-identical over all 33 halfwords except the single pool word holding the
 * message-id base (0x25b8 against 0x25dc).  Their BL halfwords are
 * bit-identical, which under the +2 rule makes them provably the same seven
 * call sites.  Found by the sort-by-span scan.
 *
 * Complete owner: 'push {r5, r6, lr}' and the single interworking epilogue at
 * 0x020000a4.  Control-flow walk: one diamond rejoining at 0x0200009c, so the
 * alignment halfword at 0x020000aa and the one pool word after it are
 * unreachable.  66 code + 6 non-code = 72, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so the owner is void.
 *
 * Calls: 7 sites over 5 targets, from
 * 'bun tools/overlay_call_targets.ts resource_3c1 0068'.  The inventory
 * advertises 6; the field is a floor, not an equality test.  Func_0808a170 is
 * ONE target reached from three sites with three different ids - the two
 * follow-up sites are separate arms of the diamond and must not be merged.
 *
 * Func_0808a070(0, 0) is the established skip-beat guard.  This overlay uses
 * the '== 0' polarity, and both polarities occur in the wider tree, so the
 * comparison is read at the site rather than assumed.
 *
 * The three message ids are consecutive from one pooled base: base, base + 1,
 * base + 2, built with 'adds r0,r5,#1' / 'adds r0,r5,#2' rather than pooled
 * separately.
 */

#define EXAMINE3C1_MESSAGE_BASE 0x25b8

void Func_0808a010();               /* wait for the beat to settle */
s32 Func_0808a070();                /* skip-beat guard */
void Func_0808a170();               /* show a message by id */
void Func_0808a178();               /* turn the subject to face the camera */
void Func_0808a180();               /* release the subject */

void Func_02000068(s32 subject)
{
    Func_0808a170(EXAMINE3C1_MESSAGE_BASE);
    Func_0808a178(subject, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(EXAMINE3C1_MESSAGE_BASE + 1);
    } else {
        Func_0808a170(EXAMINE3C1_MESSAGE_BASE + 2);
    }
    Func_0808a180(subject, 0);
}
