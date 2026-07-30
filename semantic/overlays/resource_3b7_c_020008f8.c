/*
 * resource_3b7 owner at 0x020008f8, 256 bytes (0x020008f8-0x020009f7).
 *
 * The overlay's main conversation node: a two-mode dialogue (first visit vs
 * repeat, selected by the incoming argument) with three yes/no gates and four
 * shared exit tails.
 *
 * Code runs 0x020008f8-0x020009d9.  The trailing 30 bytes,
 * 0x020009da-0x020009f7, are an alignment halfword plus a literal pool of seven
 * words - 0x00000e23, 0x00000e29, 0x00000e32, 0x00000e33, 0x00000e31,
 * 0x00000e2a, 0x00000089.  They sit past the `bx r0` return and are reached
 * only by `ldr r0, [pc, ...]`, so they are data.  All seven are small line ids,
 * none is an address, so the overlay's link base does not bear on this owner.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x020008fe -> Func_080772e0    0x0200095a is a branch, not a call
 *   0x02000904 -> Func_0808a5b0    0x02000962 -> Func_0808a170
 *   0x02000910 -> Func_0808a170    0x0200096a -> Func_0808a178
 *   0x02000918 -> Func_0808a180    0x02000976 -> Func_0808a070
 *   0x02000922 -> Func_0808a170    0x0200098c -> Func_0808a170
 *   0x0200092a -> Func_08015120    0x02000994 -> Func_0808a178
 *   0x02000932 -> Func_0808a178    0x0200099c -> Func_0808a070
 *   0x0200093a -> Func_0808a070    0x020009a6 -> Func_0808a170
 *   0x02000942 -> Func_08077308    0x020009ae -> Func_0808a180
 *   0x0200094e -> Func_0808a170    0x020009b6 -> Func_0808a170
 *   0x02000956 -> Func_0808a178    0x020009be -> Func_0808a180
 *                                  0x020009c8 -> Func_0808a270
 *                                  0x020009d0 -> Func_0808a260
 * Completeness, as a multiset: Func_0808a170 x7, Func_0808a178 x4,
 * Func_0808a070 x3, Func_0808a180 x3, and one each of Func_080772e0,
 * Func_0808a5b0, Func_08015120, Func_08077308, Func_0808a270, Func_0808a260 -
 * 23 sites over 10 distinct targets, exactly the tool's
 * `sites=23 distinct_targets=10`.  The inventory row says calls=22, which is
 * the documented undercount: the field predates the corrected `bl` decoding and
 * is a lower bound on sites only.
 *
 * Func_0808a170(id) plays a line, Func_0808a178/Func_0808a180 open and close
 * the box, Func_0808a070(0, 0) is the yes/no prompt (zero-tested exactly as in
 * semantic/overlays/resource_373_c_02002f14.c).  Func_08015120(x, 5) also
 * appears in the sibling owner Func_020009f8 in this overlay, there receiving a
 * price rather than this handle; the common shape of "one varying value, the
 * constant 5" reads as a text-substitution slot rather than a transaction, but
 * that is inference from two sites, not proof.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: void.
 *
 * `goto` is used for the four shared tails rather than duplicating them; each
 * label is exactly one join point in the assembly.
 *
 * Uncertainties:
 *  - r5 is reassigned mid-owner: it holds the incoming argument until
 *    0x02000946, then the result of Func_08077308.  It is split into two
 *    variables here, per the HANDOVER warning about long-lived aliases.
 *  - The `count > 6` test at 0x0200096e can never fire: both predecessors have
 *    already established count == 0 or count <= 6.  It is transcribed anyway
 *    because it is in the bytes; treat it as redundant, not as evidence of a
 *    fourth path.
 *  - r0 is not reset between Func_080772e0 and Func_0808a5b0, so the handle is
 *    in r0 at that branch; whether Func_0808a5b0 reads it is unverified.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

typedef signed int s32;

s32 Func_080772e0();
s32 Func_08077308();
s32 Func_0808a070();
void Func_0808a5b0();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a260();
void Func_0808a270();
void Func_08015120();

void Func_020008f8(s32 repeat)
{
    s32 handle;
    s32 count;
    s32 line;

    handle = Func_080772e0(228);

    /* r0 still holds the handle here; no argument register is reset. */
    Func_0808a5b0();

    if (repeat != 0) {
        if (handle == 0) {
            line = 0xe32;
            goto say_and_close;
        }
        Func_0808a170(0xe33);
        Func_0808a178(8, 0);
        if (Func_0808a070(0, 0) == 0) {
            goto refuse;
        }
        line = 0xe31;
        goto say_and_close;
    }

    Func_0808a170(0xe23);
    Func_0808a180(8, 0);

    if (handle == 0) {
        goto close;
    }

    Func_0808a170(0xe25);
    Func_08015120(handle, 5);
    Func_0808a178(8, 0);

    if (Func_0808a070(0, 0) != 0) {
        goto close;
    }

    count = Func_08077308();

    if (count == 0) {
        Func_0808a170(0xe27);
        Func_0808a178(8, 0);
    } else {
        if (count > 6) {
            goto refuse;
        }
        Func_0808a170(0xe28);
        Func_0808a178(8, 0);
    }

    /* Redundant in every reachable state - see the note above. */
    if (count > 6) {
        goto refuse;
    }

    if (Func_0808a070(0, 0) == 0) {
        goto refuse;
    }

    line = 0xe29;

say_and_close:
    Func_0808a170(line);

close:
    Func_0808a180(8, 0);
    return;

refuse:
    Func_0808a170(0xe2a);
    Func_0808a180(8, 0);
    Func_0808a270(0x1fc, 0);        /* 254 << 1 */
    Func_0808a260(0x89, 12);
}
