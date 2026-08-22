/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x020018e0, 160 bytes: code 0x020018e0-0x02001977 and
 * the pool words 0x00000206 and 0x00000207 at 0x02001978.
 *
 * Dispatch on slot 0's halfword at +6, which this overlay uses throughout as a
 * quadrant/facing field with the values 0, 0x4000, 0x8000 and 0xc000.
 *
 * Two of the four arms first repaint one collision cell -- but only while
 * scene flag 0x206 is already set -- and then move flag 0x207: the 0xc000 arm
 * clears it through Func_080770d0 and the 0 arm sets it through Func_080770c8,
 * which is the pair those two imports form elsewhere in this overlay.  The
 * 0x8000 arm has no flag work and instead branches on slot 0's height word at
 * +12; r0 still holds the record there because nothing has clobbered it on
 * that path.
 *
 * The epilogue is `add sp,#8 / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for a numbered slot. */

/* Scene flag test: zero means not yet set. */


/* Scene flag clear and set. */







extern s32 * Func_02003e28();
extern s32 Func_02003e00();
extern void Func_02003de8();
extern void Func_02003e2e();
extern void Func_02003ef2();
extern void Func_02003ef8();
extern s32 Func_02003e3c();
extern void Func_02003e24();
extern void Func_02003e62();
extern void Func_02003f1e();
extern void Func_020034ca();
extern void Func_02003f38();
void Func_020018e0(void)
{
    s32 *slot = Func_02003e28(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (Func_02003e00(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Func_02003de8(46, 43, 1, 1, fifth, last); }
        }
        Func_02003e2e(0x207);
        Func_02003ef2();
    } else if (facing == 0x4000) {
        Func_02003ef8();
    } else if (facing == 0) {
        if (Func_02003e3c(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Func_02003e24(58, 36, 1, 1, fifth, last); }
        }
        Func_02003e62(0x207);
        Func_02003f1e();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            Func_020034ca();
        } else {
            Func_02003f38();
        }
    }
}
