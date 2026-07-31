typedef signed short s16;

/*
 * Resource 3ae, owner at 0x020008cc (140 bytes including its five-word pool at
 * 0x02000944..0x02000956).
 *
 * Role known in advance from the call graph: `Func_0200086c`, the overlay's
 * first exported entry, dispatches on the workspace room id and calls this
 * owner for room 0x6b with no arguments.  So it is the per-room setup hook for
 * that room and takes none.
 *
 * Prologue `push {r5,lr}` at 0x020008cc; epilogue `pop {r5} / pop {r0} /
 * bx r0` at 0x0200093c.  The return address is popped into r0, so r0 does NOT
 * survive and the owner is void.
 *
 * r5 holds `Data_02000240`, the cross-overlay workspace.  The selector read
 * twice at byte offset 450 (`[225]`, signed halfword) is the same secondary
 * selector `Func_0200086c` compares against 90 — a sub-state of the room, not
 * the room id itself (that lives at `[224]`).
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x020008e0, 0x02000902, 0x02000912, 0x0200091c -> Func_080770c0 (flag test)
 *   0x020008ea                                     -> Func_080770c8 (flag set)
 *   0x020008ee                                     -> Func_02000ba0 (prologue)
 *   0x0200090c                                     -> Func_080770d0
 *   0x0200092e                                     -> Func_0808a0f0
 *   0x02000938                                     -> Func_0808a1b8
 *
 * Nine call sites against the inventory's calls=8; the inventory count is
 * documented as systematically low, and every `bl` in the 140-byte span is
 * accounted for above, so the set is complete.
 *
 * Func_080770c0 is the event-flag test (its result is branched on at all four
 * sites); Func_080770c8 is called for effect only, immediately after a failed
 * test of the same id 0x8ac, which is the read-then-set "do this once" idiom.
 * Func_080770d0 is a third flag entry point, also called for effect.
 *
 * Placement arguments to Func_0808a0f0 are 16.16 fixed point: `movs r1,#176 /
 * lsls r1,r1,#15` is (176/2) << 16 == 88.0, and `movs r2,#163 / lsls r2,r2,#19`
 * is (163*8) << 16 == 1304.0.  The Func_0808a1b8 argument 0 is the facing
 * value that the sibling `semantic/overlays/resource_3ae_c_020002dc.c` passes
 * as 0x4000, i.e. a 16-bit angle.
 *
 * Uncertainties: 0x8ac / 0x8a9 / 0x911 are read as event-flag ids from their
 * use as the sole argument of the Func_080770cX family; 0x109 is in the same
 * position but far below the rest of the band, so it may be a different id
 * space — it is preserved as written.  Both selector compares are signed.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
int Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a0f0();
void Func_0808a1b8();

void Func_02000ba0(void);

extern s16 Data_02000240[];

void Func_020008cc(void)
{
    if (Data_02000240[225] == 1) {
        if (Func_080770c0(0x8ac) == 0) {
            Func_080770c8(0x8ac);
            Func_02000ba0();
        }
    }

    if (Data_02000240[225] == 2) {
        if (Func_080770c0(0x109) == 0) {
            Func_080770d0(0x8a9);
        }
    }

    if (Func_080770c0(0x911) != 0 && Func_080770c0(0x8a9) == 0) {
        /* 88.0 and 1304.0 in 16.16 fixed point */
        Func_0808a0f0(12, 88 << 16, 1304 << 16);
        Func_0808a1b8(12, 0, 0);
    }
}
