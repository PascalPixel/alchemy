/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
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
 * resource_3b4 owner at 0x02001ac8, 152 bytes: code 0x02001ac8-0x02001b5b and
 * the pool word 0x000fffff at 0x02001b5c.
 *
 * Settle slot 8 after a push.  Its word at +12 is cleared to a marker of 2 in
 * the byte at +35 when it has reached zero, the shared repaint at 0x020019e8
 * runs, the byte at +85 becomes 3, and then the X tile (the word at +8 divided
 * by 0x100000) chooses the per-column follow-up: 40, 42 and 41 each have their
 * own repaint owner, while 37, 38 and 39 share an inline repaint that also
 * clears the +85 byte and restores +12 to 0x200000 (`movs r3,#128 /
 * lsls r3,#14`).  Any other column does nothing further.
 *
 * r6 holds the constant 0 stored into the +85 byte on the shared path.  The
 * epilogue is `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0`, so the owner is
 * void.
 */

/* Returns the record for a numbered slot. */







extern s32 * Func_02004010();
extern s32 * Func_02004028();
extern void Func_020034da();
extern s32 * Func_02004038();
extern void Func_020035a4();
extern void Func_0200355a();
extern void Func_0200358c();
extern void Func_02004014();
extern s32 * Func_02004082();
extern s32 * Func_0200408c();
void Func_02001ac8(void)
{
    s32 *slot = Func_02004010(8);
    s32 x = slot[2] / 0x100000;   /* +8  */

    if (slot[3] == 0) {           /* +12 */
        ((u8 *)Func_02004028(8))[35] = 2;
    }

    Func_020034da();
    ((u8 *)Func_02004038(8))[85] = 3;

    if (x == 40) {
        Func_020035a4();
    } else if (x == 42) {
        Func_0200355a();
    } else if (x == 41) {
        Func_0200358c();
    } else if (x == 39 || x == 38 || x == 37) {
        Func_02004014(61, 36, 1, 1, x, 42);
        ((u8 *)Func_02004082(8))[85] = 0;
        Func_0200408c(8)[3] = 0x200000;   /* +12 */
    }
}
