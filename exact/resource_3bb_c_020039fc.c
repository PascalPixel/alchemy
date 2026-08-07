#include "types.h"

/* STILL-OPEN: adopt --where differing_bytes=35/232. alchemist.ts exhausted
 * (tiers: original-order, unaligned; 43 licensed compiles tried, no
 * improving move found). overlay_mode_cohort singles sweep: no config
 * closes it either. Genuinely unfixed within budget. */

/*
 * resource_3bb owner at 0x020039fc, 232 bytes (0x020039fc-0x02003ae3):
 * code 0x020039fc-0x02003ab9, a seven-word literal pool at 0x02003aba that
 * the body branches over with the `b.n` at 0x02003ab8, and the epilogue tail
 * at 0x02003ad4.  The owner is therefore complete within the row.
 *
 * Prologue at 0x020039fc saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #4`; the epilogue restores all of them and ends
 * `pop {r0} / bx r0`, so r0 holds the popped return address and the owner is
 * void.
 *
 * Seven arguments.  r0-r3 arrive in registers and are immediately parked in
 * r8, sl, r9 and the frame word; three more are read from the caller's stack
 * at sp+36, sp+40 and sp+44, which is exactly 36 bytes of prologue (16 + 12 +
 * 4 pushed, plus the 4-byte frame) above the incoming argument area.  Five of
 * them are stored as halfwords and two as words, which is how they are typed
 * below.
 *
 * All ten `bl` sites are placed and reach ten distinct callees, matching the
 * inventory row's calls=10.  Targets come from tools/lib/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations: 0x3dd8 -> Func_08000148, 0x3de8 -> Func_08000170, 0x3f50 ->
 * Func_0808a080 (twice), 0x3ee8 -> Func_080770c0, 0x3df8 -> Func_080001a8,
 * 0x3e18 -> Func_080001d0, 0x3e10 -> Func_080001c8, 0x3da8 ->
 * Func_080000d0, 0x3df0 -> Func_08000178.
 *
 * The pool word 0x0200b639 is `Func_02003638 + 1` - this overlay's own
 * prologue at file offset 0x3638 carrying the Thumb bit - so the
 * Func_080000d0 call installs that routine as a task under key 0xc76.  It is
 * a third independent witness for the 0x02008000 link base, alongside
 * 0x0200b459 = Func_02003458 + 1 in Func_02003ae4 and 0x02008715 =
 * Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c.  Under the same base 0x0200c194 is
 * in-image data at file offset 0x4194.
 *
 * Shape: build a descriptor.  Func_08000148(59, 0x7170) returns the record
 * whose fields from +216 to +236 this owner fills in; Func_08000170(512)
 * opens the handle that the rest of the sequence works through.  When
 * Func_080770c0(0x109) is clear, participant `second` is mirrored about
 * `first` - x becomes twice the fourth argument minus the first
 * participant's x, and z is copied across.  The handle is then bound to the
 * in-image data at 0x4194, its measured extent is stored at +216 and fed
 * back sign-extended from 16 bits, the 0x3638 task is installed, and the
 * handle is closed.
 *
 * Uncertainties: only the fields at +216, +218, +220 and +222..+236 of the
 * Func_08000148 record, and +8 / +16 of the participant records, are
 * asserted.  What the seven arguments select is not established here beyond
 * their widths and the mirroring above.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020077f6();
s32 Func_02007810();
u8 *Func_020079a8();
u8 *Func_020079b0();
s32 Func_02007950();
void Func_02007886();
s32 Func_020078aa();
void Func_020078b6();
void Func_02007856();
void Func_020078a4();

/* This overlay's own routine at file offset 0x3638, installed as a task. */
void Func_0200b638();

/* In-image descriptor at file offset 0x4194. */
extern u8 Data_0200c194[];

void Func_020039fc(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_020077f6(59, 0x7170);
    handle = Func_02007810(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_020079a8(first);
    second_record = Func_020079b0(second);

    if (Func_02007950(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02007886(Data_0200c194, handle);

    extent = Func_020078aa();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_020078b6((s16)extent, 512, handle);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_02007856((s32)Func_0200b638 + 1, 0xc76);

    Func_020078a4(handle);
}
