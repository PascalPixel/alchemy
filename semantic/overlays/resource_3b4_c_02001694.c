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
 * resource_3b4 owner at 0x02001694, 280 bytes: code 0x02001694-0x02001795, two
 * bytes of alignment, and the pool words 0x0000024a, 0x02000240, 0x0001b333,
 * 0x0000d999 and 0x000fffff at 0x02001798.
 *
 * The slot-11 member of the presentation family (0x0200138c slot 8,
 * 0x0200149c slot 9, 0x020015f0 slot 10, 0x02001838 slot 12).  Like the
 * slot-10 member it is skipped outright while the shared selector
 * Data_02000240[293] already names this slot.
 *
 * What is new here is a pair of pre-nudges: slot 11 is stepped by +8 when slot
 * 0 stands to its right and by -8 when slot 0 stands to its left, each column
 * being the word at +8 divided by 0x100000.  Both comparisons re-read both
 * records rather than caching them, which is why Func_0808a080 appears four
 * times before the aim; that is what the assembly does.
 *
 * The halfword views at +10 and +18 are the integer parts of the 16.16 words
 * at +8 and +16, and are read with `ldrsh`, so they are signed.
 */

extern s16 Data_02000240[];

void Func_0808a018();
void Func_0808a090();
void Func_080f9010();
/* Returns the record for a numbered slot, or 0 when it has none. */
s32 *Func_0808a080();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0b8();
void Func_0808a010();
void Func_0808a020();

void Func_02001694(void)
{
    u8 *slot;

    if (Data_02000240[293] == 11) return;

    Func_0808a018();
    Func_0808a090(0, 0x1b333, 0xd999);
    Func_0808a090(11, 0x1b333, 0xd999);
    Func_080f9010(188);

    if (Func_0808a080(0)[2] / 0x100000 > Func_0808a080(11)[2] / 0x100000) {
        Func_0808a0e0(11, 8, 0);
    }
    if (Func_0808a080(0)[2] / 0x100000 < Func_0808a080(11)[2] / 0x100000) {
        Func_0808a0e0(11, -8, 0);
    }

    Func_0808a0e8(11);

    slot = (u8 *)Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(11, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_0808a0e8(11);
    Func_0808a0e0(0, 0, 24);
    Func_0808a010(4);
    Func_080f9010(188);
    Func_0808a0e0(11, 0, 16);
    Func_0808a0e8(0);
    Func_0808a0b8(11, 344, 360);
    Func_0808a0e8(11);
    Func_0808a010(10);
    Func_0808a020();
}
