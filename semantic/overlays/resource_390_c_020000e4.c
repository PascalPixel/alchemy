typedef unsigned short u16;
typedef unsigned int u32;

/*
 * resource_390 owner at 0x020000e4, 72 bytes: an interaction handler.  If the
 * subject stands inside the approach band, enter the interaction; otherwise
 * show a refusal message and play the "cannot" gesture.  Either way the beat is
 * bracketed by the family's begin/end pair.
 *
 * TRANSPOSED from semantic/overlays/resource_390_c_0200009c.c, the reference
 * member of this four-owner family.  The two owners are byte-identical over
 * all 36 halfwords except: the entry call's two immediates (13, 16 -> 14, 17),
 * the message-id pool word (0x16ad -> 0x16af), the act call's first immediate
 * (16 -> 17).  Every call was
 * re-resolved with 'bun tools/overlay_call_targets.ts resource_390 00e4'
 * rather than transposed with the body.
 *
 * Complete owner: 'push {r5, lr}' and the single interworking epilogue at
 * 0x02000118.  Control-flow walk: one forward diamond rejoining before
 * the epilogue, so the alignment halfword and the three pool words after
 * 'bx r0' are unreachable - 58 code + 14 non-code = 72, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so the owner is void.
 *
 * Calls: 6 sites over 6 targets, from
 * 'bun tools/overlay_call_targets.ts resource_390 00e4'.  The inventory
 * advertises 6; the field is a floor, not an equality test.
 *
 * The guard is the documented "band guard" family - 'ldrh +6', 'adds
 * 0xffff5fff', 'cmp 0x3ffe', 'bhi' - an unsigned half-open range test on a
 * wrapped 16-bit position word, not a mask and not a sign trick.  It selects
 * the 0x5001..0x9fff window.
 *
 * UNCERTAINTY: what the record's halfword at +6 measures is not established
 * beyond its use as the wrapped position word this family guards on.
 */

struct Approach390Subject {
    u16 unknown_00[3];
    u16 position;                   /* 0x06, wrapped 16-bit */
};

void Func_0808a018();               /* begin an uninterruptible beat */
void Func_0808a020();               /* end it */
struct Approach390Subject *Func_0808a080();  /* subject record by id */
void Func_0808a170();               /* show a message by id */
void Func_0808a180();               /* play a gesture (id, parameter) */
void Func_080b0008();               /* enter the interaction (id, parameter) */

void Func_020000e4(void)
{
    u16 position = Func_0808a080(0)->position;

    Func_0808a018();
    if ((u32)(position + 0xffff5fff) <= 0x3ffe) {
        Func_080b0008(14, 17);
    } else {
        Func_0808a170(0x16af);
        Func_0808a180(17, 0);
    }
    Func_0808a020();
}
