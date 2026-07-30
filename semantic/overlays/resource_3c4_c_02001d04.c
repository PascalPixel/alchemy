typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3c4 owner at 0x02001d04, 600 bytes: code 0x02001d04-0x02001f55, an
 * alignment halfword at 0x02001f56 and the pool word 0x02009a99 at
 * 0x02001f58-0x02001f5b.
 *
 * The pillar-swap beat.  It walks the three slots 12, 13 and 14 (paired with
 * the flags 512, 513 and 514).  For a slot whose presentation byte at
 * block+9 already has both bits 2 and 3 set it runs the settle routine at
 * 0x02001038 and stops; otherwise it looks for a slot standing on whole-tile
 * z 9 whose flag is still clear, and then either swaps that slot's position
 * with the first already-set slot found below it, or, if there is none,
 * proceeds with the slot itself.  The chosen slot is then reset, framed by two
 * camera moves, landed by 0x02001c2c, has the tag counters of slots 10 and 11
 * nudged in opposite directions depending on which side of tile x 8 it ended
 * on, gets 0x02009a99 installed as its per-frame handler, and finally has its
 * flag published.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 47 sites, 13 distinct
 * callees, all 47 accounted for below.  Thirty-two of the sites are
 * Func_0808a080 and every one of them carries the same `bl` halfwords, so the
 * disassembler prints thirty-two different targets for one callee - the
 * clearest instance of the encoding bug in this overlay.
 *
 * Resolved callees: veneers 0x020030f4 (-> Func_0808a018), 0x0200310c
 * (-> Func_0808a080, x32), 0x020030d4 (-> Func_080770c0, x3), 0x020030dc
 * (-> Func_080770c8, x2), 0x0200313c (-> Func_0808a0f0), 0x02003174
 * (-> Func_0808a208), 0x0200318c (-> Func_0808a228), 0x0200317c
 * (-> Func_0808a210), 0x02003184 (-> Func_0808a218) and 0x020030fc
 * (-> Func_0808a020); plus this overlay's prologues at 0x02001038 and
 * 0x02001c2c and its routine at 0x02001abc.
 *
 * 0x02001abc is reported `unknown` only because it is not an inventory row of
 * its own; it is a real `push {r5, r6, r7, lr}` prologue that also saves
 * r8/r9/sl.  Previously this owner was skipped because 0x020039d4 (its call
 * site) and 0x02003aee (the 0x02001c2c site) looked like in-image addresses
 * that are not function starts; both were decoding artefacts.
 *
 * Link base 0x02008000, one more witness: the pool word 0x02009a99 installed
 * at record+108 is 0x02001a98 plus the Thumb bit, and 0x02001a98 is a real
 * prologue in this overlay - the same per-frame-handler shape 0x02000ae8 uses.
 *
 * NOTE on register lifetimes: the swap block reuses r6 - the outer loop
 * counter - as a scratch record pointer.  That is safe because every path
 * through the swap block leaves the loop for good (it joins at 0x02001e56 and
 * runs unconditionally to the single exit), so the counter is dead there.  The
 * reconstruction keeps `slotFlag` and `chosen` as separate locals rather than
 * reproducing the reuse.
 *
 * UNCERTAINTY: on the no-match path the swap block is skipped and `chosen` is
 * the loop counter itself rather than a scanned index; that is what
 * `mov r9, r6` at 0x02001e3a says, and it is the value used for the rest of
 * the beat.
 *
 * The epilogue is `add sp,#112 / pop {r3, r5, r6, r7} / mov r8..fp /
 * pop {r5, r6, r7} / pop {r0} / bx r0`, so the owner is void.
 */

typedef struct Slot_02001d04 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
    s32 w20;               /* +20 */
    u8 pad24[16];
    s32 w40;               /* +40 */
    u8 pad44[16];
    s32 w60;               /* +60 */
} Slot_02001d04;

void Func_0808a018();      /* scripted-action bracket: open */
Slot_02001d04 *Func_0808a080();
s32 Func_080770c0();       /* reads a published flag */
void Func_080770c8();      /* publishes a flag */
void Func_0808a0f0();
void Func_0808a208();
u8 *Func_0808a228();       /* returns a record; its byte at +85 is cleared */
void Func_0808a210();
void Func_0808a218();
void Func_02001038();      /* this overlay's settle routine */
void Func_02001c2c();      /* this overlay's landing routine */
void Func_02001abc();

void Func_02001d04(void)
{
    s32 index;
    s32 slot;
    s32 chosen;
    s32 scan;
    s32 saved[3];          /* sl -> sp+8, sp+12, sp+16 */

    /* No argument register is set. */
    Func_0808a018();

    for (index = 0, slot = 12; index <= 2; index++, slot++) {
        /* record+80 is the presentation block pointer; bits 2 and 3 of its
         * byte at +9 mark a pillar that is already settled. */
        u8 *block = *(u8 **)((u8 *)Func_0808a080(slot) + 80);

        if ((block[9] & 12) == 12 && Func_080770c0(index + 512) == 0) {
            Func_02001038(Func_0808a080(slot));
            Func_0808a0f0(slot, 0, 0);
            Func_080770c8(index + 512);
            Func_0808a020();
            return;
        }

        if ((Func_0808a080(slot)->z >> 20) != 9) continue;
        if (Func_080770c0(index + 512) != 0) continue;

        /* Func_080770c0 returned 0 here, and that zero is the register the
         * original uses as the source for all five resets below. */
        Func_0808a080(slot)->w20 = 0;
        Func_0808a080(slot)->w40 = 0;
        Func_0808a080(slot)->w60 = (s32)0x80000000;      /* 128 << 24 */
        ((u8 *)Func_0808a080(slot))[85] = 0;
        *(u16 *)((u8 *)Func_0808a080(slot) + 100) = 0;

        chosen = index;

        /* Look below this slot for one whose flag is already published; the
         * first such slot swaps positions with it. */
        for (scan = 0; scan < index; scan++) {
            if (Func_080770c0(scan + 512) == 0) {
                s32 other = scan + 12;

                saved[0] = Func_0808a080(slot)->x;
                saved[1] = Func_0808a080(slot)->y;
                saved[2] = Func_0808a080(slot)->z;

                Func_0808a080(slot)->x = Func_0808a080(other)->x;
                Func_0808a080(slot)->y = Func_0808a080(other)->y;
                Func_0808a080(slot)->z = Func_0808a080(other)->z;

                Func_0808a080(other)->x = saved[0];
                Func_0808a080(other)->y = saved[1];
                Func_0808a080(other)->z = saved[2];

                chosen = scan;
                break;
            }
        }

        {
            s32 target = chosen + 12;
            u8 *shaken;

            Func_0808a080(target)->w20 = 0;
            Func_0808a080(target)->w40 = 0;
            Func_0808a080(target)->w60 = (s32)0x80000000;
            ((u8 *)Func_0808a080(target))[85] = 0;
            *(u16 *)((u8 *)Func_0808a080(target) + 100) = 0;

            /* 192 << 10 and 192 << 7. */
            Func_0808a208(0x30000, 0x6000);

            /* No argument register is set; the result's byte at +85 is
             * cleared. */
            shaken = Func_0808a228();
            shaken[85] = 0;

            /* 168 << 16, 128 << 12, 184 << 16. */
            Func_0808a210(0x00a80000, 0x00080000, 0x00b80000, 1);

            /* No argument register is set. */
            Func_0808a218();

            Func_02001c2c(target);

            /* The two counter slots move in opposite directions depending on
             * which side of whole-tile x 8 the pillar came to rest. */
            if ((Func_0808a080(target)->x >> 20) == 8) {
                *(u16 *)((u8 *)Func_0808a080(10) + 100) += 1;
                *(u16 *)((u8 *)Func_0808a080(11) + 100) -= 1;
            } else {
                *(u16 *)((u8 *)Func_0808a080(10) + 100) -= 1;
                *(u16 *)((u8 *)Func_0808a080(11) + 100) += 1;
            }

            /* 0x02009a99 is Func_02001a98 plus the Thumb bit: the per-frame
             * handler installed at record+108. */
            *(unsigned int *)((u8 *)Func_0808a080(target) + 108) = 0x02009a99;

            Func_02001abc(40);

            ((u8 *)Func_0808a080(target))[35] |= 2;

            Func_080770c8(chosen + 512);
        }

        break;             /* every path through here leaves the loop */
    }

    Func_0808a020();
}
