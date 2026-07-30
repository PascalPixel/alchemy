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
 * resource_3b4 owner at 0x02001fd8, 220 bytes.  The span is not all code: an
 * eleven-entry jump table occupies 0x02001ffc-0x02002027 and the literal pool
 * 0x020020ac-0x020020b3 closes it, leaving 0x02001fd8-0x02001ffb and
 * 0x02002028-0x020020ab executable.
 *
 * A round-robin ticker.  The counter is the in-image word `Data_0200af80`
 * (file offset 0x2f80 under this overlay's 0x02008000 link base -- the overlay
 * image is writable EWRAM, so this really is stored state).  It is incremented
 * every call and wraps to 0 past 16, and only ticks 2..12 do anything.
 *
 * The dispatch is `ldr r3,[r3,r2] / mov pc,r3` over the table at 0x02001ffc,
 * whose base is the pool word 0x02009ffc -- 0x1ffc + 0x8000, which is a third
 * independent witness for the link base.  Its entries are in-image code
 * addresses in the same form.  Every odd tick and every out-of-range tick land
 * on the shared exit, so they are the empty cases below.
 *
 * NOTE for the inventory: this row reports `calls=0`, which is wrong -- the
 * table disassembles as plausible code and hides the fourteen real call sites.
 * HANDOVER already records that `calls` can undercount for exactly this shape.
 *
 * The epilogue is `pop {r0} / bx r0`, so the owner is void.  The `mov pc,r3`
 * at 0x02001ff8 is a dispatch, not a return.
 */

/* Round-robin counter, in-image at file offset 0x2f80. */
extern s32 Data_0200af80;

/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
void Func_0808a0f0();
/* Overlay-local, at 0x020020c4. */
void Func_020020c4();

void Func_02001fd8(void)
{
    s32 tick;
    s32 slot;

    Data_0200af80++;
    if (Data_0200af80 > 16) Data_0200af80 = 0;

    tick = Data_0200af80;
    if ((u32)(tick - 2) > 10) return;

    /* Ticks 4..10 share one call site at 0x0200209a; tick 2 has its own at
     * 0x020020a4.  Ticks 3, 5, 7, 9 and 11 have their own table entries but
     * all point at the shared exit. */
    switch (tick) {
    case 2:
        Func_020020c4(12, 1);
        return;

    case 4:
        slot = 11;
        break;

    case 6:
        slot = 10;
        break;

    case 8:
        slot = 9;
        break;

    case 10:
        slot = 8;
        break;

    case 12:
        /* Place any of slots 8..11 whose scene flag is still clear.  Every
         * coordinate is a `movs`/`lsls` pair in the assembly. */
        if (Func_080770c0(0xee7) == 0) Func_0808a0f0(8, 0xe80000, 0x3680000);
        if (Func_080770c0(0xee8) == 0) Func_0808a0f0(9, 0x1280000, 0x3380000);
        if (Func_080770c0(0xee9) == 0) Func_0808a0f0(10, 0x1480000, 0x2f80000);
        if (Func_080770c0(0xeea) == 0) Func_0808a0f0(11, 0x1680000, 0x3680000);
        return;

    default:
        return;
    }

    Func_020020c4(slot, 0);
}
