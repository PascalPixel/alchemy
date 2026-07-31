typedef signed int s32;

/*
 * Resource 3a3 interaction handler at 0x020003d4 (116 bytes, 0x020003d4 ..
 * 0x02000447, of which 0x02000434 .. 0x02000447 is the literal pool).
 *
 * Complete owner: `push {lr}` at 0x020003d4, interworking return
 * `pop {r0} / bx r0` at 0x02000430.  r0 is the popped return address, so the
 * owner returns nothing.
 *
 * Middle member of the 0x02000360 / 0x020003d4 / 0x02000448 sibling triplet.
 * This one and 0x02000448 are instruction-for-instruction identical; they
 * differ only in the gate flag id (0x241 vs 0x240), the two cue ids, the extra
 * cue id, and the two small actor/slot numbers.  0x02000360 differs in one
 * further place, using Func_0808a190 in its first arm.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`.  All 13 call
 * sites are placed: Func_080770c0 x2, Func_0808a018 x2, Func_0808a170 x3,
 * Func_0808a180 x2, Func_0808a020 x2, Func_02000338 x1.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a018();       /* opens a scripted sequence */
void Func_0808a170();       /* queues a cue / script id */
void Func_0808a180();
void Func_0808a020();       /* closes the scripted sequence */
void Func_080b0008();

/* Overlay-local facing test at 0x02000338 (byte-exact sibling source). */
s32 Func_02000338(void);

void Func_020003d4(void)
{
    if (Func_080770c0(0x241) == 0) {
        Func_0808a018();
        Func_0808a170(0x18ed);
        Func_0808a180(20, 0);
        Func_0808a020();
        return;
    }

    if (Func_02000338() != 0) {
        Func_080b0008(20, 17);
        return;
    }

    Func_0808a018();
    Func_0808a170(0x18ee);
    if (Func_080770c0(0x909) != 0) {
        Func_0808a170(0x1943);
    }
    Func_0808a180(17, 0);
    Func_0808a020();
}
