typedef signed int s32;

/*
 * Resource 3a3 interaction handler at 0x02000448 (116 bytes, 0x02000448 ..
 * 0x020004bb, of which 0x020004aa .. 0x020004bb is alignment plus the literal
 * pool).
 *
 * Complete owner: `push {lr}` at 0x02000448, interworking return
 * `pop {r0} / bx r0` at 0x020004a6, so the owner returns nothing.
 *
 * Last member of the 0x02000360 / 0x020003d4 / 0x02000448 triplet, and
 * instruction-for-instruction identical to 0x020003d4.  The gate flag id is
 * built inline as `movs r0,#144 / lsls r0,#2` = 576 = 0x240 instead of being
 * pooled, which is the only encoding difference.
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

void Func_02000448(void)
{
    if (Func_080770c0(0x240) == 0) {
        Func_0808a018();
        Func_0808a170(0x18f1);
        Func_0808a180(21, 0);
        Func_0808a020();
        return;
    }

    if (Func_02000338() != 0) {
        Func_080b0008(21, 16);
        return;
    }

    Func_0808a018();
    Func_0808a170(0x18f2);
    if (Func_080770c0(0x909) != 0) {
        Func_0808a170(0x1945);
    }
    Func_0808a180(16, 0);
    Func_0808a020();
}
