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
 * resource_3b4 owner at 0x02001838, 136 bytes: code 0x02001838-0x020018b7 and
 * the pool words 0x0001b333 and 0x0000d999 at 0x020018b8.
 *
 * Sibling of 0x0200138c for slot 12.  It uses the 0x1b333 / 0xd999 pair and
 * has no Func_0808a010 step at all; everything else is the same sequence.
 */

void Func_02003d64();
void Func_02003d8e();
void Func_02003d98();
void Func_02003e56();
u8 *Func_02003d9c();
void Func_02003dbe();
void Func_02003de4();
void Func_02003de6();
void Func_02003e84();
void Func_02003df6();
void Func_02003e04();
void Func_02003df0();
void Func_02003e16();
void Func_02003dda();
void Func_02003dca();

                     
/* Returns the record for a numbered slot, or 0 when it has none. */

                     

                     

void Func_02001838(void)
{
    u8 *slot;

    Func_02003d64();
    Func_02003d8e(0, 0x1b333, 0xd999);
    Func_02003d98(12, 0x1b333, 0xd999);
    Func_02003e56(188);

    slot = Func_02003d9c(0);
    if (slot != 0) {
        Func_02003dbe(12, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_02003de4(12);
    Func_02003de6(0, 0, 24);
    Func_02003e84(188);
    Func_02003df6(12, 0, 16);
    Func_02003e04(0);
    Func_02003df0(12, 312, 232);
    Func_02003e16(12);
    Func_02003dda();
    Func_02003dca(0x220);
}
