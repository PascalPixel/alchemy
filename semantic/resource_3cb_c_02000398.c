#include "types.h"

/*
 * Resource 3cb link receive stage at 0x02000398 (420 bytes,
 * 0x02000398-0x0200053b).
 *
 * Complete owner: the `push {r5, r6, r7, lr}` + high-register save +
 * `sub sp, #48` prologue at 0x02000398 and the matching unwind at
 * 0x0200051e-0x0200052e, ending `pop {r1} / bx r1`.  The popped return address
 * goes to r1, so r0 survives and IS the result: `mov r0, r9` at 0x0200051c
 * puts the status there.
 *
 * The twelve bytes at 0x02000530-0x0200053b are this owner's literal pool
 * (0x02002238, 0x03001f64, 0x0000080c).  Execution reaches `bx r1` at
 * 0x0200052e and never falls into them; the `movs`/`subs`/`lsrs` halfwords the
 * disassembler prints there are data.  Func_0200053c begins immediately after.
 *
 * All fifteen call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=15.  Every one is an import veneer:
 *   0x020003b0 -> 0x0200173c -> Func_08000170   (open, 340)
 *   0x020003d0 -> 0x02001714 -> Func_080000c0   (wait 1)
 *   0x020003ea -> 0x02001774 -> Func_080003a8   (poll)
 *   0x0200040e -> 0x02001714 -> Func_080000c0   (wait 2)
 *   0x02000416 -> 0x020017bc -> Func_08015020   (fetch string 0x080c)
 *   0x0200047e -> 0x02001804 -> Func_08077008   (block buffer)
 *   0x02000484 -> 0x0200176c -> Func_08000388   (start)
 *   0x020004ac -> 0x02001714 -> Func_080000c0   (wait 1)
 *   0x020004c6 -> 0x02001774 -> Func_080003a8   (poll)
 *   0x020004e6 -> 0x02001714 -> Func_080000c0   (wait 2)
 *   0x020004ee -> 0x02001744 -> Func_08000178   (close)
 *   0x020004fa -> 0x0200173c -> Func_08000170   (open, 320)
 *   0x02000502 -> 0x020017fc -> Func_08077000
 *   0x02000506 -> 0x0200176c -> Func_08000388   (start)
 *   0x02000518 -> 0x02001744 -> Func_08000178   (close)
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  Both pool addresses used as memory here are
 * below the image: 0x02002238 is the link driver's transferred-length halfword
 * (the same word Func_020007b0 clears when it resets the link RAM block), and
 * 0x03001f64 is the IWRAM link-state halfword whose low two bits
 * Func_0200008c also tests for "a link is up".
 *
 * Behaviour: receive three 340-byte blocks and then one 320-byte block.  Each
 * block is opened with Func_08000170(size), started with Func_08000388, and
 * polled with Func_080003a8 until it reports zero; the transfer is rejected if
 * the length word ever exceeds the expected size or does not equal it at the
 * end.  A single 900-tick budget spans the whole session, and a per-block
 * counter allows up to 24 further ticks after the budget runs out or the link
 * drops before the session is abandoned.  Any failure returns -1.
 *
 * Per block, the payload is edited in place: string 0x080c is fetched into a
 * 48-byte stack scratch as halfwords, its leading non-zero run (at most five)
 * is measured, the block's first fifteen bytes are shifted right by that
 * length, the string's characters are written narrowed into the gap and
 * byte 14 is zeroed.  The block's flag byte at +298 is counted into the
 * result, so a successful session returns how many of the three blocks carried
 * it.
 *
 * Uncertainties: the identity of the 0x0000080c string id and of the byte at
 * +298 are not established; Func_08077000(1) between the two stages is issued
 * for its side effect only.  The length comparisons at 0x020003c6 are unsigned
 * (`bls`) while the stage-two one at 0x0200049c is signed (`ble`); both
 * operands are zero-extended halfword loads, so the two agree in practice and
 * are written as one comparison here.  The scratch buffer is read as halfwords
 * and stored as bytes, which truncates; that is what the assembly does.
 */

/* Old-style declarations: arities vary per site across this overlay. */
s32 Func_02001aee();
s32 Func_02001ae6();
s32 Func_02001b60();
s32 Func_02001b24();
void Func_02001bd4();
void Func_02001c84();
void Func_02001bf2();
s32 Func_02001bc2();
s32 Func_02001c3c();
s32 Func_02001bfc();
s32 Func_02001c34();
s32 Func_02001c38();
void Func_02001d00();
void Func_02001c74();
s32 Func_02001c5e();

                    

                     

/* Used for their return values. */

s32 Func_02000398(void)
{
    volatile u16 *transferred = (volatile u16 *)0x02002238;
    volatile u16 *linkState = (volatile u16 *)0x03001f64;
    u16 scratch[24];
    s32 expected = 340;
    s32 handle;
    s32 budget = 900;
    s32 result = 0;
    u32 block;
    s32 stalls;
    s32 status;
    u8 *payload;

    handle = Func_02001aee(340);

    for (block = 0; block <= 2; block++) {
        payload = Func_02001ae6(block + 128);
        status = Func_02001b60();
        stalls = 0;
        if (status == -1) {
            result = status;
            goto close;
        }

        for (;;) {
            status = Func_02001b24();
            if (status == 0) {
                break;
            }
            if ((s32)*transferred > expected) {
                goto fail;
            }
            budget--;
            Func_02001bd4(1);
            if (budget < 0 || (*linkState & 3) != 3) {
                stalls++;
                if (stalls > 24) {
                    goto fail;
                }
            }
        }

        if ((s32)*transferred != expected) {
            goto fail;
        }

        if (payload[298] != 0) {
            result++;
        }

        Func_02001c84(2);
        Func_02001bf2(0x080c, scratch);

        {
            s32 inserted = 0;
            s32 offset;

            if (scratch[0] != 0) {
                do {
                    inserted++;
                    if (inserted > 4) {
                        break;
                    }
                } while (scratch[inserted] != 0);
            }

            for (offset = 14; offset >= inserted; offset--) {
                payload[offset] = payload[offset - inserted];
            }
            for (offset = 0; offset < inserted; offset++) {
                payload[offset] = (u8)scratch[offset];
            }
            payload[14] = 0;
        }
    }

    Func_02001bc2(handle);
    expected = 320;
    handle = Func_02001c3c(320);
    Func_02001bfc(1);
    status = Func_02001c34();
    stalls = 0;
    if (status == -1) {
        result = status;
        goto close;
    }

    for (;;) {
        status = Func_02001c38();
        if (status == 0) {
            break;
        }
        if ((s32)*transferred > expected) {
            goto fail;
        }
        budget--;
        Func_02001d00(1);
        if (budget < 0 || (*linkState & 3) != 3) {
            stalls++;
            if (stalls > 24) {
                goto fail;
            }
        }
    }

    if ((s32)*transferred != expected) {
        goto fail;
    }
    Func_02001c74(2);
    goto close;

fail:
    result = -1;

close:
    Func_02001c5e(handle);
    return result;
}
