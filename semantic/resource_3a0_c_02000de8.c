typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3a0 mode entry at 0x02000de8 (94 bytes, 6 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000de8, the two branches
 * (`bne` at 0x02000e08 to 0x02000e20 and `b` at 0x02000e1e to 0x02000e2c)
 * stay inside the body, and the walk stops at the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000e40-0x02000e44.  So the executable
 * extent is exactly 0x02000de8-0x02000e45 (94 bytes).  The popped register is
 * r0, so the popped value is the return address and the owner is `void`.
 *
 * Pool map — the walk never reaches these as instructions:
 *   0x02000e46  0x0000       alignment
 *   0x02000e48  0x03001ebc   the workspace POINTER
 * 0x03001ebc is even and far below the 0x02008000 link base, so it is an
 * absolute IWRAM address, and it is loaded from rather than used directly.
 * The next prologue begins at 0x02000e4c.
 *
 * All six call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3a0 0de8 0e48`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x02000dee -> veneer 0x020013d0 -> Func_0808a080
 *   0x02000e02 -> veneer 0x020013d8 -> Func_0808a090
 *   0x02000e0e -> veneer 0x02001430 -> Func_0808a100
 *   0x02000e1a -> veneer 0x02001418 -> Func_0808a0e0
 *   0x02000e28 -> veneer 0x02001410 -> Func_0808a0d8
 *   0x02000e3c -> veneer 0x02001498 -> Func_0808a248
 * The two arms of the `bne` call DIFFERENT imports — 0x02000e1a reaches
 * Func_0808a0e0 and 0x02000e28 reaches Func_0808a0d8 — so they must not be
 * merged even though the argument lists look alike.  That is exactly the class
 * of error a per-target multiset catches and a total cannot.
 *
 * `movs r1,#128 / lsls r1,#8` builds 0x8000 and `movs r2,#128 / lsls r2,#7`
 * builds 0x4000: two VALUES, not displacements, despite the family's habit of
 * building displacements that way.  `movs r2,#16 / negs r2` is -16.
 *
 * `movs r2,#228 / lsls r2,#1` is 456, a displacement into the workspace, and
 * HANDOVER records +448 and +472 as the neighbouring published slots — this is
 * a third one and must not be conflated with either.
 */

extern u8 *Data_03001ebc;      /* workspace pointer */

u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_0808a090();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a0d8();
void Func_0808a248();

void Func_02000de8(s32 mode)
{
    Func_0808a080(0)[0x55] = 0;
    Func_0808a090(0, 0x8000, 0x4000);

    if (mode == 6) {
        Func_0808a100(0, 2);
        Func_0808a0e0(0, 0, -16);
    } else {
        Func_0808a0d8(0, 2, -16);
    }

    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_0808a248(mode);
}
