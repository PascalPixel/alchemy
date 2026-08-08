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
 * resource_3b4 owner at 0x02000f40, 156 bytes: code 0x02000f40-0x02000fd5, two
 * bytes of alignment, and the pool word 0x0000e666 at 0x02000fd8.
 *
 * The parameterised twin of 0x02000fdc.  Its caller 0x020010b8 passes
 * `0x100 | selector` with selector in 0..3, and this owner splits the word:
 * bit 8 (`movs r3,#128 / lsls r3,#1 / ands`) gates the camera retune, while
 * the low byte scaled by four (`movs r3,#255 / ands / lsls #2`) yields the two
 * column bases 4 * selector + 77 and 4 * selector + 13.
 *
 * The first base is the stack column of both Func_080091b8 repaints; the
 * second is both the register and the stack column of the two Func_080091c0
 * repaints.  r5 and r6 hold them across the calls and the epilogue is
 * `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0`, so the owner is void.
 */

void Func_02003560();
void Func_02003458();
void Func_02003466();
void Func_02003460();
void Func_020033fe();
void Func_02003478();
void Func_02003492();
void Func_020034a4();

                     

void Func_02000f40(s32 request)
{
    s32 wide = ((request & 0xff) << 2) + 77;
    s32 narrow = ((request & 0xff) << 2) + 13;

    if ((request & 0x100) != 0) {
        Func_02003560(157);
        Func_02003458(0x20000, 0x20000, 0x10000);
        Func_02003466(-1, -1, 0xe666);
        Func_02003460(79, 29, 1, 3, wide, 40);
        Func_020033fe(40);
    }

    Func_02003478(80, 29, 1, 3, wide, 40);
    Func_02003492(narrow, 40, 1, 1, narrow, 41);
    Func_020034a4(narrow, 40, 1, 1, narrow, 42);
}
