typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3cb link handshake driver at 0x020007b0 (176 bytes,
 * 0x020007b0-0x0200085f).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x020007b0 and the matching
 * interworking return `pop {r5, r6} / pop {r1} / bx r1` at 0x02000838, which
 * every path reaches through the shared `adds r0, r6, #0` at 0x02000836.  The
 * popped return address goes to r1, so r0 survives and IS the result — the
 * status word held in r6.
 *
 * The 32 bytes at 0x02000840-0x0200085f are this owner's literal pool
 * (0x00000302, 0x020023a0, 0x02002220, 0x04000208, 0x02002080, 0x02002008,
 * 0x020023ac, 0x02002238), with two alignment bytes at 0x0200083e.  Execution
 * reaches `bx r1` at 0x0200083c and never falls into them.
 *
 * All nine call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=9 and the placed multiset has nine entries:
 *   0x020007b6 -> veneer 0x0200180c -> Func_080770c0
 *   0x020007c4 -> veneer 0x02001714 -> Func_080000c0
 *   0x020007c8 -> 0x02000580  (this overlay, real prologue)
 *   0x020007d4 -> veneer 0x02001714 -> Func_080000c0
 *   0x020007d8 -> 0x02000398  (this overlay, real prologue)
 *   0x020007e6 -> 0x02000398  (this overlay, real prologue)
 *   0x020007f4 -> veneer 0x02001714 -> Func_080000c0
 *   0x020007f8 -> 0x02000580  (this overlay, real prologue)
 *   0x02000808 -> veneer 0x0200182c -> Func_080770e8
 * The two in-image targets are genuine function starts (both are strict rows
 * of this overlay with their own `push` prologues), so they are ordinary
 * calls, not hidden-context targets.
 *
 * Link base: resource_3cb is linked at 0x02008000, so every pool address here
 * is BELOW the image and is ordinary RAM or I/O — 0x04000208 is IME and the
 * 0x020020xx/0x020022xx/0x020023xx words are the link driver's RAM block.
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear (polarity forced by Func_020002d8's one-shot timer).
 * Flag 0x302 is the follower/host role flag that Func_0200008c writes.
 *
 * Behaviour: run the two-phase exchange in the order the local role requires.
 * The host (flag 0x302 clear) waits, runs Func_02000580, waits, then runs
 * Func_02000398; the follower runs Func_02000398 first, then waits and runs
 * Func_02000580.  A negative return from either phase aborts.  On success the
 * result is published through Func_080770e8(1008, status) and returned.  On
 * any failure the link RAM block is reset with interrupts masked, and the
 * negative status is returned.
 *
 * The interrupt mask idiom is the one the byte-exact
 * assets/code/resource_3cb_c_0200003c.c already spells: the IME pointer value
 * is stored back into IME, which writes 0x0208 and therefore clears bit 0.
 * The previous IME value is restored afterwards.
 *
 * Uncertainties: r4 holds the saved IME across the reset block without being
 * saved by the prologue — this owner only pushes r5/r6/lr.  Nothing observable
 * depends on it, and the bytes are not in doubt (the same idiom is recorded
 * for this family in HANDOVER §0), so it is transcribed rather than "fixed".
 * The meanings of the individual RAM fields cleared in the reset block and of
 * the 0x80 written to 0x02002221 are not established here.
 */

/* Old-style declarations: arities vary per site across this overlay. */
void Func_080000c0();
void Func_080770e8();

/* Used for their return values. */
s32 Func_080770c0();
s32 Func_02000398();
s32 Func_02000580();

s32 Func_020007b0(void)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u8 *linkBlock = (u8 *)0x02002220;
    u16 savedInterruptMaster;
    s32 status = 0;
    s32 phaseResult;

    if (Func_080770c0(0x302) == 0) {
        *(u8 *)0x020023a0 = 0;

        Func_080000c0(5);
        status = Func_02000580();
        if (status >= 0) {
            Func_080000c0(5);
            status = Func_02000398();
            phaseResult = status;
            if (phaseResult >= 0) {
                goto publish;
            }
        }
    } else {
        *(u8 *)0x020023a0 = 0;

        status = Func_02000398();
        phaseResult = status;
        if (phaseResult >= 0) {
            Func_080000c0(10);
            status = Func_02000580();
            if (status >= 0) {
                goto publish;
            }
        }
    }

    goto resetLink;

publish:
    /* Both role branches converge on this one physical publication site. */
    Func_080770e8(1008, phaseResult);
    status = phaseResult;
    return status;

resetLink:

    savedInterruptMaster = *interruptMaster;
    *interruptMaster = (u16)(u32)interruptMaster;

    linkBlock[1] = 0x80;
    *(u32 *)0x02002080 = 0;
    *(u16 *)0x02002008 = 0;
    *(u32 *)0x020023ac = 0;
    linkBlock[3] = 0;
    linkBlock[2] = 0;
    *(u16 *)0x02002238 = 0;

    *interruptMaster = savedInterruptMaster;

    return status;
}
