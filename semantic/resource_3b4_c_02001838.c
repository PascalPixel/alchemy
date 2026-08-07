/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/lib/overlay_call_targets.ts`.  The band
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

void Func_0808a018();
void Func_0808a090();
void Func_080f9010();
/* Returns the record for a numbered slot, or 0 when it has none. */
u8 *Func_0808a080();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0e0();
void Func_0808a020();
void Func_080770d0();

void Func_02001838(void)
{
    u8 *slot;

    Func_0808a018();
    Func_0808a090(0, 0x1b333, 0xd999);
    Func_0808a090(12, 0x1b333, 0xd999);
    Func_080f9010(188);

    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(12, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_0808a0e8(12);
    Func_0808a0e0(0, 0, 24);
    Func_080f9010(188);
    Func_0808a0e0(12, 0, 16);
    Func_0808a0e8(0);
    Func_0808a0b8(12, 312, 232);
    Func_0808a0e8(12);
    Func_0808a020();
    Func_080770d0(0x220);
}
