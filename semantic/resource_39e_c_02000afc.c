#include "types.h"

/*
 * resource_39e owner at 0x02000afc, 216 bytes: the overlay's SCENE-EXIT
 * routine. It spins until slot 12's pending-work counter drains, resets
 * that slot's fields, plays one of three closing lines chosen by story
 * flag, and hands slot 12 a descriptor on the way out.
 *
 * Complete owner: `push {r5, lr}` at 0x02000afc through
 * `pop {r5} / pop {r0} / bx r0` at 0x02000bcc-0x02000bd0. There are TWO
 * literal pools INSIDE the body -- 0x02000b7a-0x02000b8b (hopped by the
 * `b.n 0x2000b92` at 0x02000b78) and 0x02000bc0-0x02000bcb (hopped by the
 * `b.n 0x2000bcc` at 0x02000bbe) -- so a reader who stops at the first
 * pool gets a 128-byte "function" instead of 216. The next owner's
 * prologue is exactly at 0x02000bd4. **216 bytes**, measured to the
 * epilogue.
 *
 * Published population (sweep B). All 19 `bl` sites resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_39e afc bd4`; the tool
 * reports sites=19 and 19 bl lines were transcribed.
 *
 * THE DRAIN LOOP is the one real loop: entry jumps straight to its bottom
 * test. Each iteration re-fetches the slot-12
 * record with Func_0808a080(12) and re-reads the signed 32-bit counter at
 * record[12]; the body calls Func_080000c0(1), which is the only call in
 * this overlay to that import. The record is re-fetched EVERY time --
 * eight separate Func_0808a080(12) calls in this function, none of them
 * cached -- so the pointer is treated as invalidated by each call. Explicit
 * labels retain the machine's wait-then-test lexical order.
 *
 * The byte at record[91] is used as a two-state gate: cleared to 0 before
 * the drain loop, set to 1 after the fields are reset, and cleared again
 * at the very end. It is reached by `adds r0, #91` off the record base
 * each time, never by a struct displacement.
 *
 * THE THREE-WAY FLAG CHOICE reads as a plain else-if chain and the branch
 * sense is UNIFORM here (both tests fire their line when the flag IS
 * set), which is worth stating explicitly because resource_3af mixes the
 * two senses inside one overlay:
 *   flag 0x895 set -> line 0x1a5b
 *   else flag 0x89b set -> line 0x189e
 *   else -> line 0x182a
 * 0x895 is set by this overlay's 0x02002ad0 and 0x89b by its 0x02001dbc,
 * so the closing line reports which of the two beats the player reached.
 *
 * The final Func_0808a098 argument 0x0200c638 is EVEN, so by the odd-word
 * rule it is a data address (image offset 0x4638), not a published Thumb
 * function pointer.
 *
 * Uncertainty: the meaning of record[12] (drained), record[60]
 * (0x80000000), record[40] (0), record[6] (0x4000) and record[91] is not
 * established beyond the accesses above. Func_080000c0(1) is presumed a
 * frame-advance because it is what the drain loop spins on, but that is
 * inference from context, not evidence.
 */

extern void Func_02004e58(s32 arg0);
extern s32 Func_02004f16(s32 flagId);
extern s32 Func_02004f28(s32 flagId);
extern void Func_02004edc(void);
extern void Func_02004fa0(void);
extern u8 *Func_02004f0a(s32 slot);
extern u8 *Func_02004f1e(s32 slot);
extern u8 *Func_02004f2a(s32 slot);
extern u8 *Func_02004f34(s32 slot);
extern u8 *Func_02004f40(s32 slot);
extern u8 *Func_02004f48(s32 slot);
extern u8 *Func_02004fa2(s32 slot);
extern u8 *Func_02004fae(s32 slot);
extern void Func_02004fd4(s32 slot, u8 *descriptor);
extern void Func_02004fd0(s32 slot, s32 a1, s32 a2);
extern void Func_02005000(s32 dialogueId);
extern void Func_02005012(s32 dialogueId);
extern void Func_0200502c(s32 dialogueId);
extern void Func_02005044(s32 slot, s32 a1);

void Func_02000afc(void)
{
    Func_02004edc();

    Func_02004f0a(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        Func_02004e58(1);
testPendingWork:
    if (*(s32 *)(Func_02004f1e(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(Func_02004f2a(12) + 12) = 0;

    *(s32 *)(Func_02004f34(12) + 60) = 128 << 24;

    *(s32 *)(Func_02004f40(12) + 40) = 0;

    Func_02004f48(12)[91] = 1;

    Func_02004fd0(12, 0, 0);

    if (Func_02004f16(0x895) != 0) {
        Func_02005000(0x1a5b);
    } else if (Func_02004f28(0x89b) != 0) {
        Func_02005012(0x189e);
    } else {
        Func_0200502c(0x182a);
    }

    Func_02005044(12, 0);

    *(u16 *)(Func_02004fa2(12) + 6) = 128 << 7;

    Func_02004fae(12)[91] = 0;

    Func_02004fd4(12, (u8 *)0x0200c638);
    Func_02004fa0();
}
