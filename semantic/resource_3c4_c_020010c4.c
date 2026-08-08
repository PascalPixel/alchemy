#include "types.h"

/*
 * resource_3c4 owner at 0x020010c4, 284 bytes: code 0x020010c4-0x020011cb and
 * the five pool words 0x02009069, 0xfffe0000, 0x00003332, 0xffff8003 and
 * 0x01000001 at 0x020011cc-0x020011df.
 *
 * The sinking beat.  It takes the subject record over, drops it one eighth of
 * a unit per tick for eight ticks, and on every other tick throws off one
 * effect with a randomised velocity through this overlay's eight-argument
 * spawner Func_02000ae8.  The subject's flag byte at +85 is set to 2 for the
 * duration, cleared to 0 before the loop and left as 3 on the way out.
 *
 * Targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 18 sites, 12 distinct
 * callees.  Note that the inventory row advertises `calls=16`; it counts
 * distinct targets and undercounts here, so the 18 sites below are the
 * accounting to trust.  Every disassembler annotation in this owner is an
 * artefact - the two Func_08009080 sites, the two Func_080000f8 sites, the two
 * Func_030003e0 sites, the two Func_080000c0 sites, the two Func_080091e0
 * sites and the two Func_080f9010 sites each carry bit-identical `bl`
 * halfwords and each print two different targets.
 *
 * The real callees are the veneers at 0x0200310c (-> Func_0808a080),
 * 0x020030f4 (-> Func_0808a018), 0x02003054 (-> Func_08009080, x2),
 * 0x0200314c (-> Func_0808a118), 0x020030ac (-> Func_080091e0, x2),
 * 0x020031ac (-> Func_080f9010, x2), 0x02003074 (-> Func_08009150),
 * 0x02003024 (-> Func_080000c0, x2), 0x02003034 (-> Func_080000f8, x2),
 * 0x0200301c (-> Func_030003e0, x2) and 0x020030fc (-> Func_0808a020), plus
 * this overlay's prologue at 0x02000ae8.
 *
 * Previously skipped because 0x02001c8c looked like an in-image address that
 * is not a function start; it was the decoding artefact for the 0x02000ae8
 * call.
 *
 * Func_080000c0 and Func_080000f8 are entries 0 and 7 of the ROM's fixed
 * dispatch stub table at 0x080000c0 (asm/080000c0.s), forwarding to
 * Func_080030f8 and Func_08004458.  Func_030003e0 is an ARM-mode helper
 * relocated into IWRAM, the same family as the 0x030001d8 square root; both
 * its call sites are `(random, 10)` and both results are used as a small
 * bounded index, so it is the remainder helper.
 *
 * UNCERTAINTY: that reading of Func_080000f8 and Func_030003e0 comes from how
 * the results are consumed here, not from their own bodies, which are outside
 * this overlay.
 *
 * The three velocity constants read as 16.16 fractions: 0x00003332 is 0.2,
 * 0x00001999 (as 6553, built by the shift chain
 * `r*13`, `<<6`, `-r*13`, `<<3`, `+r`) is 0.1 and 0xffff8003 is -0.5.  So the
 * horizontal kick is (u - 5) * 0.2 for u in 0..9 and the vertical one is
 * -0.5 - v * 0.1.
 *
 * Link base 0x02008000: the pool word 0x02009069 stored at options+36 is
 * 0x02001068 plus the Thumb bit, and 0x02001068 is a real prologue in this
 * overlay.  flags = 0x01000001 selects exactly that one optional field
 * (0x01000000 -> effect+108 = *(u32 *)(options+36)); the other 36 bytes of the
 * record are never written and never read, and the low nibble 1 is the
 * descriptor index.
 *
 * The epilogue is `add sp,#56 / pop {r3, r5} / mov r8,r3 / mov sl,r5 /
 * pop {r5, r6, r7} / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for the given slot. */
u8 *Func_020041de();
void Func_020041cc();
void Func_02004134();
void Func_02004232();
void Func_02004142();
void Func_020041a2();
void Func_020042b6();
void Func_02004196();
void Func_0200414c();
void Func_020042ea();
void Func_02004174();
s32 Func_02004190();
s32 Func_0200417e();
s32 Func_020041a2_b();
void Func_02001c8c();
void Func_0200425e();
void Func_020042b8();

                     

                     

                     

                     

/* This overlay's effect spawner: four register arguments plus four stack
 * words - (x, y, z, vx, vy, vz, flags, options). */

void Func_020010c4(void)
{
    u8 *record;
    u8 *flags;
    u8 options[40];        /* sp+16 */
    u32 tick;

    record = Func_020041de(0);

    /* No argument register is set; r0 still holds the record. */
    Func_020041cc();

    Func_02004134(record, 6);
    Func_02004232(0);
    Func_02004142(record, 1);
    Func_020041a2(record, 0);

    flags = record + 85;
    *flags = (u8)(*flags | 2);

    Func_020042b6(152);

    *(s32 *)(record + 40) = 0x40000;    /* 128 << 11 */

    /* 192 << 12 = 0x000c0000 added to z. */
    Func_02004196(record, *(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x000c0000);

    Func_0200414c(6);

    *flags = 0;
    *(u32 *)&options[36] = 0x02009069;   /* Func_02001068 + Thumb bit */

    Func_020042ea(127);

    for (tick = 0; tick <= 7; tick++) {
        s32 y;
        s32 kick;
        s32 lift;

        /* 0xfffe0000 is -0x00020000: one eighth of a unit down in 12.20. */
        y = *(s32 *)(record + 12) + (s32)0xfffe0000;
        *(s32 *)(record + 12) = y;
        *(s32 *)(record + 60) = y;

        Func_02004174(1);

        if ((tick & 1) == 0) continue;

        kick = (Func_0200417e(Func_02004190(), 10) - 5) * 0x3332;
        lift = (s32)0xffff8003 - Func_02004190(Func_020041a2_b(), 10) * 6553;

        Func_02001c8c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16),
                      kick, 0, lift, 0x01000001, options);
    }

    Func_0200425e(record, 1);
    *flags = 3;
    Func_020042b8();
}
