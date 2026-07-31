typedef signed short s16;
typedef signed int s32;

/*
 * Resource 370 predicate at 0x02000384.
 *
 * Complete owner: `push {lr}` at 0x02000384 through the interworking return
 * `pop {r1} / bx r1` at 0x020003bc.  Because the popped return address lands
 * in r1 rather than r0, r0 survives the epilogue and IS the result, so this
 * owner returns a value (HANDOVER section 0, epilogue rule).
 *
 * The three words at 0x020003c0-0x020003cb are this owner's literal pool; the
 * span's control flow branches straight past them to the epilogue, so they are
 * data (0x0000023e, 0x02000240, 0x00000002).
 *
 * The single `bl` at 0x0200038a resolves through the overlay import veneer at
 * file offset 0x1454 to the main-image routine Func_080770c0
 * (bun tools/overlay_call_targets.ts resource_370 -- the disassembler's own
 * annotation for an overlay `bl` is wrong by construction).
 *
 * 0x02000240 is the shared cross-overlay scene workspace; the byte-exact
 * siblings assets/code/resource_39a_c_02000050.c, resource_3b2_c_02000d48.c
 * and resource_3b7_c_02000044.c already model it as `extern s16
 * Data_02000240[]` and read the same halfword index 224 (byte offset 448).
 * This owner additionally reads index 287 (byte offset 574 = 0x23e).
 *
 * Uncertainty: the byte-exact sources in the tree spell small compared
 * constants as `(s32)&Value_000000NN` so that the compiler pools them.  That
 * is a code-generation device, not meaning; the comparison value here is
 * simply 2 and is written as such.
 */

extern s16 Data_02000240[];

/* Import reached through the veneer at file offset 0x1454.  Old-style
 * declaration: overlay imports may take different argument counts at
 * different sites, but the return type is needed because it is tested. */
s32 Func_080770c0();

s32 Func_02000384(void)
{
    s32 mode;

    /* 162 << 1 = 324 is the only argument set at the call site. */
    if (Func_080770c0(324) == 0) {
        return 0;
    }

    /* ldrsh, so both reads are signed halfwords. */
    if (Data_02000240[287] == 2) {
        return 0;
    }

    /*
     * negs/orrs/lsrs #31/negs is the compiler's "x != 0" expanded to a full
     * -1 mask: the result is 0 when index 224 holds 2 and -1 otherwise.
     */
    mode = Data_02000240[224] ^ 2;
    return -(mode != 0);
}
