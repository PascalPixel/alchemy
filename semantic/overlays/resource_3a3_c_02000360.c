typedef signed int s32;

/*
 * Resource 3a3 interaction handler at 0x02000360 (116 bytes, 0x02000360 ..
 * 0x020003d3, of which 0x020003c0 .. 0x020003d3 is the literal pool).
 *
 * Complete owner: `push {lr}` at 0x02000360 and the interworking return
 * `pop {r0} / bx r0` at 0x020003bc.  The popped register is r0, so by the
 * epilogue rule the owner returns nothing.
 *
 * This is one of three byte-for-byte sibling handlers in this overlay --
 * 0x02000360, 0x020003d4 and 0x02000448 -- which differ only in their gate
 * flag id, their two cue ids, the extra cue id, and the two small actor/slot
 * numbers.  Reading them together settles every argument position; the only
 * structural difference is that this member uses Func_0808a190 in its first
 * arm where the other two use Func_0808a180.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts`, not from
 * the disassembler's `bl` annotations (an overlay `bl` stores the target's
 * image offset minus two).  All 13 call sites are placed:
 *   Func_080770c0 x2, Func_0808a018 x2, Func_0808a170 x3, Func_0808a190 x1,
 *   Func_0808a180 x1, Func_0808a020 x2, Func_02000338 x1.
 *
 * Naming note: the byte-exact siblings in `assets/code/resource_3a3_c_*.c`
 * spell these imports by the address their call site prints (Func_02001164
 * for Func_0808a018, Func_020011f2 for Func_0808a170, and so on).  Those are
 * the same imports under the pre-correction names; the resolved main-image
 * addresses are used here.
 */

/* Imports.  Old-style declarations: a single import is reached with different
 * argument counts at different sites across this overlay. */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a018();       /* opens a scripted sequence */
void Func_0808a170();       /* queues a cue / script id */
void Func_0808a180();
void Func_0808a190();
void Func_0808a020();       /* closes the scripted sequence */
void Func_080b0008();

/* Overlay-local: the facing test at 0x02000338, byte-exact in
 * `assets/code/resource_3a3_c_02000338.c`.  Returns 1 when the slot-0 record's
 * angle lies in the tested arc. */
s32 Func_02000338(void);

void Func_02000360(void)
{
    if (Func_080770c0(0x242) == 0) {
        Func_0808a018();
        Func_0808a170(0x18e7);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Func_0808a190(15, 0);
        Func_0808a020();
        return;
    }

    if (Func_02000338() != 0) {
        Func_080b0008(19, 15);
        return;
    }

    Func_0808a018();
    Func_0808a170(0x18ea);
    if (Func_080770c0(0x909) != 0) {
        Func_0808a170(0x1941);
    }
    Func_0808a180(15, 0);
    Func_0808a020();
}
