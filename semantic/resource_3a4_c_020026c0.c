typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x020026c0, 324 bytes: triple-actor
 * reconciliation -- the same marker-drop / flag-gated configure motif
 * as resource_3a4_c_020025c0.c and _02002490.c, unrolled over actors
 * 9, 11 and 12 with consecutive story flags, then a local callback
 * install and the standard closing window choice.
 *
 * Per-actor block, three copies with only ids/flags/coords changing:
 * `Func_02003460(id)` (drop marker, clear +89 bit), then if flag F is
 * set: Func_0808a100(id, 5), a Func_080091c0 panel, and
 * `Func_0808a080(id)[0x23] |= 2` -- the exact motif of the sibling
 * rows.  The (id, flag) pairs: (9, 0x200), (11, 0x201), (12, 0x204).
 * The row opens with Func_080091e0(Func_0808a080(9), 0), the
 * established seat-record idiom.
 *
 * Then Func_080000d0(0x0200b429, 200 << 4): 0x0200b429 is image offset
 * 0x3428 | 1 at the 0x02008000 link base -- a local callback install,
 * the same Func_080000d0 idiom the byte-exact 0x3a44 uses with its
 * 0x39c8 | 1 pointer.  The callback body at 0x3428 sits between the
 * 0x3028 monster and Func_02003460; it is not yet drafted.
 *
 * Closing: flag 0x327 picks Func_080091c0(30, 82, ...) + window 46,
 * else (28, 82, ...) + window 47 -- the same 46/47 window pair row
 * 0x25c0 chooses on flag 0x326.
 *
 * Complete owner: `push {lr}` at 0x020026c0, `sub sp, #8`, through
 * `add sp, #8 / pop {r0} / bx r0` at 0x20027f0-0x20027f4, trailing
 * literal pool at 0x20027f6-0x2002803, immediately followed by the
 * next unindexed candidate at 0x02002804.  No arguments, void.
 *
 * All 25 bl sites resolve with the `+2` stored-displacement rule; 21
 * veneers, four overlay-local:
 *   0x20026c6, 0x2002702, 0x200273e, 0x200277c -> Func_0808a080
 *   0x20026cc -> Func_080091e0             (record, 0), established
 *   0x20026d0 -> image offset 0x40c        local Func_0200040c
 *   0x20026d6, 0x2002712, 0x200274e -> image offset 0x3460
 *   0x20026de (0x200), 0x2002718 (0x201), 0x2002756 (0x204),
 *     0x2002796 (0x327) -> Func_080770c0   test story flag
 *   0x20026ea, 0x2002724, 0x2002762 -> Func_0808a100
 *   0x20026fc, 0x2002738, 0x2002776, 0x20027ae, 0x20027d8
 *     -> Func_080091c0
 *   0x2002790 -> Func_080000d0             install local callback
 *   0x20027c2, 0x20027ec -> Func_08009180  six-arg window call
 */

s32 Func_080770c0();           /* test story flag, established */
u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_080091e0();          /* (record, n), established */
void Func_0808a100();          /* (id, n) */
void Func_080091c0();          /* six-argument panel call */
void Func_08009180();          /* six-argument window call */
void Func_080000d0();          /* install callback (ptr, param) */
void Func_0200040c();          /* local, image offset 0x40c */
void Func_02003460(s32 selector);   /* local, resource_3a4_c_02003460.c */

void Func_020026c0(void)
{
    Func_080091e0(Func_0808a080(9), 0);
    Func_0200040c();

    Func_02003460(9);
    if (Func_080770c0(0x200) != 0) {
        Func_0808a100(9, 5);
        Func_080091c0(0, 0, 1, 1, 26, 26);
        Func_0808a080(9)[0x23] |= 2;
    }

    Func_02003460(11);
    if (Func_080770c0(0x201) != 0) {
        Func_0808a100(11, 5);
        Func_080091c0(1, 0, 1, 1, 17, 10);
        Func_0808a080(11)[0x23] |= 2;
    }

    Func_02003460(12);
    if (Func_080770c0(0x204) != 0) {
        Func_0808a100(12, 5);
        Func_080091c0(1, 0, 1, 1, 26, 15);
        Func_0808a080(12)[0x23] |= 2;
    }

    Func_080000d0(0x0200b429, 200 << 4);    /* Func_02003428 | 1 */

    if (Func_080770c0(0x327) != 0) {
        Func_080091c0(30, 82, 1, 1, 29, 81);
        Func_08009180(46, 28, 29, 17, 1, 2);
    } else {
        Func_080091c0(28, 82, 1, 1, 29, 81);
        Func_08009180(47, 28, 29, 17, 1, 2);
    }
}
