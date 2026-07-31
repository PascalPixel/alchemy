typedef signed int s32;

/*
 * Resource 3cb three-digit number renderer at 0x02001294 (76 bytes,
 * 0x02001294-0x020012df).
 *
 * Complete owner: `push {r5, r6, r7, lr}` / `sub sp, #8` prologue at
 * 0x02001294 and the matching `add sp, #8 / pop {r5, r6, r7} / pop {r1} /
 * bx r1` interworking return at 0x020012d4.  The popped return address goes to
 * r1, so r0 survives and IS the result — here the value returned by the
 * trailing Func_08009128 call.
 *
 * The four bytes at 0x020012dc-0x020012df are this owner's literal pool: the
 * single word 0x000003e7 (999), loaded by the `ldr r3, [pc, #68]` at
 * 0x02001296.  It is data, not the `lsls` the disassembler prints.
 *
 * All four call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2):
 *   0x020012aa -> veneer 0x0200170c -> Func_030003ac
 *   0x020012bc -> veneer 0x0200178c -> Func_08009180
 *   0x020012c4 -> veneer 0x02001704 -> Func_03000380
 *   0x020012d0 -> veneer 0x02001784 -> Func_08009128
 * sites=4 matches the inventory's calls=4.
 *
 * Func_03000380 and Func_030003ac are relocated IWRAM helpers (the same band
 * as the relocated square root at 0x030001d8 documented in HANDOVER §0).  The
 * dataflow identifies them: both are called as f(value, 10), the result of
 * Func_030003ac is passed straight to the glyph call as the digit, and the
 * result of Func_03000380 replaces the running value for the next iteration.
 * So Func_030003ac is the remainder helper and Func_03000380 the quotient
 * helper.  They are reached by ordinary `bl` through this overlay's veneer
 * table, not through a `call_via` register, so nothing here loads r3/r4 as a
 * target.
 *
 * Behaviour: clamp the argument to 999, then emit exactly three decimal digits
 * right to left through Func_08009180 at columns 16, 15 and 14, row 8, with
 * glyph set 27 and two stacked arguments of 1.  Leading zeros are emitted —
 * the loop always runs three times and never tests the running value.
 *
 * Uncertainties: the two stack words at [sp, #0] and [sp, #4] are both the
 * constant 1 held in r7 and their meaning is not established; and the final
 * Func_08009128 is reached with no argument register set by this owner, so r0
 * still holds the last quotient.  That residue is written as an argument-less
 * call here because nothing in the owner intends it, but a callee that reads
 * incoming r0 would observe it.  The clamp at 0x0200129c is signed, so a
 * negative argument is rendered as-is.
 */

/* Old-style declarations: arities vary per site across this overlay. */
s32 Func_03000380();
s32 Func_030003ac();
s32 Func_08009128();
void Func_08009180();

/* Used for their return values. */

s32 Func_02001294(s32 value)
{
    s32 column;

    if (value > 999) {
        value = 999;
    }

    for (column = 0; column <= 2; column++) {
        s32 digit = Func_030003ac(value, 10);

        Func_08009180(27, digit, 16 - column, 8, 1, 1);
        value = Func_03000380(value, 10);
    }

    return Func_08009128();
}
