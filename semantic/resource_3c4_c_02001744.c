#include "types.h"

/*
 * resource_3c4 owner at 0x02001744, 436 bytes: code 0x02001744-0x020018df and
 * the six pool words 0x00000202, 0x03020000, 0x00009999, 0xffff0000,
 * 0x0000e666 and 0x00000972 at 0x020018e0-0x020018f7.
 *
 * The sibling of 0x02001550: the same beat for slot 19 on whole-tile x 48,
 * with one extra guard.  Gate on the slot's tile, then on Func_080770c0(0x202)
 * returning nonzero; spawn a sixteen-effect rain from a scattered band while
 * the slot sinks; then the fixed sequence of placements, camera moves and two
 * identical shake pairs; finally publish selector 0x972.
 *
 * Against 0x02001550 the differences are exactly: slot 19 for 18, tile 48 for
 * 46, the extra Func_080770c0 guard, the four position constants, the two
 * placement rows (45/14 for 49/8), and selector 0x972 for 0x971.  The control
 * flow is otherwise identical.
 *
 * Targets resolved with `tools/lib/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 32 sites, 18 distinct
 * callees, all 32 accounted for below.  The four Func_0808a080 sites, the four
 * Func_0808a010 sites and the four Func_080091f0 sites each carry
 * bit-identical `bl` halfwords and each print four different targets, so every
 * disassembler annotation in this owner is an artefact.
 *
 * Resolved callees: veneers 0x020030f4 (-> Func_0808a018), 0x0200310c
 * (-> Func_0808a080, x4), 0x020030d4 (-> Func_080770c0), 0x020030ec
 * (-> Func_0808a010, x4), 0x020031ac (-> Func_080f9010, x3), 0x02003024
 * (-> Func_080000c0), 0x02003034 (-> Func_080000f8, x2), 0x0200309c
 * (-> Func_080091c0, x2), 0x02003144 (-> Func_0808a100), 0x0200306c
 * (-> Func_080090d0), 0x0200313c (-> Func_0808a0f0), 0x0200308c
 * (-> Func_08009180, x2), 0x020030bc (-> Func_080091f0, x4), 0x020030c4
 * (-> Func_080091f8), 0x020030dc (-> Func_080770c8) and 0x020030fc
 * (-> Func_0808a020); plus this overlay's prologue at 0x02000ae8 and its
 * routine at 0x020009f4 (see resource_3c4_c_02001550.c on why the tool reports
 * that one `unknown`).
 *
 * Previously skipped because 0x02002172 (the 0x020009f4 site) and 0x020022dc
 * (the 0x02000ae8 site) both looked like in-image addresses that are not
 * function starts.  Both were decoding artefacts.
 *
 * flags = 0x00090000 for the rain selects exactly the two optional fields the
 * options record initialises: 0x00010000 -> options+4 (7) and 0x00080000 ->
 * options+8/options+12 (both 0x9999).  The remaining 28 bytes are never
 * written and never read.
 *
 * The epilogue is `add sp,#56 / pop {r3} / mov r8,r3 / pop {r5, r6, r7} /
 * pop {r0} / bx r0`, so the owner is void.  Both early exits branch to the
 * same physical Func_0808a020 close site as the completed sequence.
 */

typedef struct Slot_02001744 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001744;

void Func_02004842();
Slot_02001744 *Func_02004860();
s32 Func_02004838();
void Func_0200485c();
s32 Func_02002172();
Slot_02001744 *Func_0200489e();
void Func_0200494a();
void Func_020047ca();
Slot_02001744 *Func_020048b8();
u32 Func_020047ec();
u32 Func_020047fe();
void Func_020022dc();
void Func_020048a8();
void Func_020048fe();
Slot_02001744 *Func_02004924();
void Func_0200496e();
void Func_0200489c();
void Func_020048de();
void Func_0200498c();
void Func_02004a02();
void Func_020048f6();
void Func_02004934();
void Func_02004942();
void Func_02004978();
void Func_02004a3e();
void Func_0200492e();
void Func_0200496c();
void Func_0200497a();
void Func_02004986();
void Func_020049b4();
void Func_020049aa();
void Func_020049ce();
                           /* scripted-action bracket: open */
                               
                           /* queried as a condition */

                           /* ROM dispatch[0] -> Func_080030f8 */
                           /* ROM dispatch[7] -> Func_08004458 */
                           /* four register arguments plus two stack words */

                     
                           /* four register arguments plus two stack words */

                     
                           /* scripted-action bracket: close */

/* This overlay's routine at 0x020009f4; returns a handle. */

/* This overlay's effect spawner: four register arguments plus four stack
 * words - (x, y, z, vx, vy, vz, flags, options). */

void Func_02001744(void)
{
    u8 options[40];        /* sp+16 */
    s32 handle;
    u32 step;

    /* No argument register is set. */
    Func_02004842();

    if ((Func_02004860(19)->x >> 20) != 48) {
        goto close_sequence;
    }

    if (Func_02004838(0x202) == 0) {
        goto close_sequence;
    }

    Func_0200485c(30);

    /* 137 << 17 = 0x01120000. */
    handle = Func_02002172(0x03020000, 0, 0x01120000, 223);

    *(s32 *)&options[12] = 0x9999;
    *(s32 *)&options[4] = 7;
    *(s32 *)&options[8] = 0x9999;

    ((u8 *)Func_0200489e(19))[85] = 0;

    Func_0200494a(185);

    for (step = 0; step <= 15; step++) {
        Slot_02001744 *slot;
        s32 x;
        s32 z;

        Func_020047ca(3);

        /* 0xffff0000 is -0x00010000: one sixteenth of a unit down per tick. */
        slot = Func_020048b8(19);
        slot->y = slot->y + (s32)0xffff0000;

        /* Both reductions use `lsrs`, so they are unsigned.  192 << 18 and
         * 224 << 16 are the band's origins. */
        x = (s32)(((Func_020047ec() << 4) >> 16) << 16) + 0x03000000;
        z = (s32)(((Func_020047fe() * 18u) >> 16) << 16) + 0x00e00000;

        Func_020022dc(x, 0, z, 0, 0, 0, 0x00090000, options);
    }

    Func_020048a8(51, 8, 1, 1, 45, 14);
    Func_020048fe(30);

    ((u8 *)Func_02004924(19))[35] |= 2;

    Func_0200496e(19, 3);
    Func_0200489c(handle);

    Func_020048de(45, 4, 1, 1, 48, 14);

    /* 194 << 18, 232 << 16. */
    Func_0200498c(21, 0x03080000, 0x00e80000);
    Func_02004a02(188);

    /* 160 << 11 = 0x00050000, 128 << 9 = 0x00010000. */
    Func_020048f6(58, 8, 45, 14, 1, 3);
    Func_02004934(0, 0x00050000, 0x00010000);
    Func_02004942(-1, -1, 0xe666);

    Func_02004978(20);
    Func_02004a3e(188);

    Func_0200492e(59, 8, 45, 14, 1, 3);
    Func_0200496c(0, 0x00050000, 0x00010000);
    Func_0200497a(-1, -1, 0xe666);

    /* No argument register is set. */
    Func_02004986();

    Func_020049b4(10);
    Func_020049aa(0x972);

close_sequence:
    Func_020049ce();
}
