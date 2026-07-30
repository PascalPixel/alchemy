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
 * resource_3b4 owner at 0x02001120, 60 bytes: code 0x02001120-0x02001157 and
 * the pool word 0x03001ebc at 0x02001158.
 *
 * Run selector 2 of the shared step at 0x020010b8, then -- only while slot 0's
 * halfword at +6 is 0x4000 -- age a counter in the workspace that
 * `Data_03001ebc` points at.  The counter is the signed halfword at +412
 * (`movs r2,#206 / lsls r2,#1`); once it passes 12 the notification at
 * Func_0808a2a0 is posted and the counter is reset to zero.
 *
 * 0x02001108 and 0x02001114 are the plain selector-0 and selector-1 wrappers
 * onto the same step; 0x0200115c is this owner's selector-3 twin.
 *
 * `Data_03001ebc` is the workspace pointer the byte-exact
 * resource_3b4_c_0200006c.c already uses, with the same `u8 *` spelling.
 */

extern u8 *Data_03001ebc;

void Func_020010b8();
/* Returns the record for a numbered slot. */
u8 *Func_0808a080();
void Func_0808a2a0();

void Func_02001120(void)
{
    u8 *slot;
    s16 *counter;

    Func_020010b8(2);

    slot = Func_0808a080(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        counter = (s16 *)(Data_03001ebc + 412);
        if (*counter > 12) {
            /* r0 still holds `slot` here; whether the callee reads it is
             * unverified, so no argument is asserted. */
            Func_0808a2a0();
            *counter = 0;
        }
    }
}
