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
 * resource_3b4 owner at 0x020015f0, 164 bytes: code 0x020015f0-0x02001683 and
 * the pool words 0x0000024a, 0x02000240, 0x0001b333 and 0x0000d999 at
 * 0x02001684.
 *
 * The slot-10 member of the presentation family whose other members are
 * 0x0200138c (slot 8), 0x0200149c (slot 9) and 0x02001838 (slot 12).  This one
 * is additionally skipped outright while the shared selector
 * Data_02000240[293] is already 10, and it closes with an extra
 * Func_0808a010(10) before Func_0808a020.
 *
 * The halfword views at +10 and +18 are the integer parts of the 16.16 words
 * at +8 and +16, and are read with `ldrsh`, so they are signed.
 */

extern s16 Data_02000240[];

void Func_02003b2a();
void Func_02003b54();
void Func_02003b5e();
void Func_02003c1c();
u8 *Func_02003b62();
void Func_02003b84();
void Func_02003baa();
void Func_02003bac();
void Func_02003b6a();
void Func_02003c50();
void Func_02003bc2();
void Func_02003bd0();
void Func_02003bbe();
void Func_02003be4();
void Func_02003b9a();
void Func_02003bae();

                     
/* Returns the record for a numbered slot, or 0 when it has none. */

                     

                     

void Func_020015f0(void)
{
    u8 *slot;

    if (Data_02000240[293] == 10) return;

    Func_02003b2a();
    Func_02003b54(0, 0x1b333, 0xd999);
    Func_02003b5e(10, 0x1b333, 0xd999);
    Func_02003c1c(188);

    slot = Func_02003b62(0);
    if (slot != 0) {
        Func_02003b84(10, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_02003baa(10);
    Func_02003bac(0, 0, 24);
    Func_02003b6a(4);
    Func_02003c50(188);
    Func_02003bc2(10, 0, 16);
    Func_02003bd0(0);
    Func_02003bbe(10, 264, 360);
    Func_02003be4(10);
    Func_02003b9a(10);
    Func_02003bae();
}
