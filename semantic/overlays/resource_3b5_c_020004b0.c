typedef unsigned char u8;

/*
 * Resource 3b5, owner at 0x020004b0 (56 bytes; 38 bytes of code, two bytes of
 * alignment, and the 16-byte literal pool at 0x020004d8-0x020004e7).
 *
 * Complete owner: `push {lr}` at 0x020004b0 and `pop {r1} / bx r1` at
 * 0x020004d2.  It pops into r1, so r0 survives and IS the result.
 *
 * Role: exported entry point.  The overlay image begins with a table of
 * eight-byte `ldr r4, [pc, #0] / bx r4 / .word target` veneers; its fifth
 * entry holds 0x020084b1, which under this overlay's proven 0x02008000 link
 * base is Func_020004b0 + the Thumb bit.  So this is an exported selector, and
 * the three values it returns are in-image script addresses (file offsets
 * 0x1a9c, 0x17a8 and 0x14a8), each the head of a 12-byte-per-record cutscene
 * script of the kind laid out in `assets/code/resource_3b5_overlay.s`.
 *
 * Following project convention the raw pool value is kept for the data
 * addresses rather than renumbering them to file offsets.
 *
 * Call target resolved with `tools/overlay_call_targets.ts`.
 */

/* Old-style declaration: overlay import arities are not fixed per name. */
int Func_080770c0();

void *Func_020004b0(void)
{
    if (Func_080770c0(0x950) != 0) {
        return (void *)0x02009a9c;
    }
    if (Func_080770c0(0x962) != 0) {
        return (void *)0x020097a8;
    }
    return (void *)0x020094a8;
}
